#include <RemoteControl/DocumentPlugin.hpp>

#include <core/document/Document.hpp>
#include <core/document/DocumentModel.hpp>
#include <iscore/document/DocumentInterface.hpp>

#include <RemoteControl/Scenario/Scenario.hpp>
#include <RemoteControl/Settings/Model.hpp>


#include <Device/Node/DeviceNode.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <QBuffer>
#include <QJsonObject>
#include <QJsonDocument>
#include <iscore/serialization/VisitorCommon.hpp>
#include <Scenario/Application/ScenarioActions.hpp>
#include <iscore/model/path/PathSerialization.hpp>
#include <iscore/actions/ActionManager.hpp>
#include <iscore/actions/Action.hpp>
namespace RemoteControl
{
DocumentPlugin::DocumentPlugin(
    const iscore::DocumentContext& doc,
    Id<iscore::DocumentPlugin> id,
    QObject* parent):
  iscore::DocumentPlugin{doc, std::move(id), "RemoteControl::DocumentPlugin", parent},
  receiver{doc, 10212}
{
  auto& set = m_context.app.settings<Settings::Model>();
  if(set.getEnabled())
  {
    create();
  }

  con(set, &Settings::Model::EnabledChanged,
      this, [=] (bool b) {
    if(b)
      create();
    else
      cleanup();
  }, Qt::QueuedConnection);
}

DocumentPlugin::~DocumentPlugin()
{
}

void DocumentPlugin::on_documentClosing()
{
  cleanup();
}

void DocumentPlugin::create()
{
  if(m_root)
    cleanup();

  auto& doc = m_context.document.model().modelDelegate();
  auto scenar = safe_cast<Scenario::ScenarioDocumentModel*>(&doc);
  auto& cstr = scenar->baseScenario().constraint();
  m_root = new Constraint(
             getStrongId(cstr.components()),
             cstr,
             *this,
             this);
  cstr.components().add(m_root);
}

void DocumentPlugin::cleanup()
{
  if(!m_root)
    return;

  // Delete
  auto& doc = m_context.document.model().modelDelegate();
  auto scenar = safe_cast<Scenario::ScenarioDocumentModel*>(&doc);
  auto& cstr = scenar->baseScenario().constraint();

  cstr.components().remove(m_root);
  m_root = nullptr;
}

Receiver::Receiver(
    const iscore::DocumentContext& doc,
    quint16 port):
  m_server{"i-score-ctrl",  QWebSocketServer::NonSecureMode},
  m_dev{doc.plugin<Explorer::DeviceDocumentPlugin>()}
{
  if (m_server.listen(QHostAddress::Any, port))
  {
    connect(&m_server, &QWebSocketServer::newConnection,
            this, &Receiver::onNewConnection);
  }

  m_answers.insert(std::make_pair("Trigger", [&] (const QJsonObject& obj, const WSClient&)
  {
    auto it = obj.find("Path");
    if(it == obj.end())
      return;

    auto path = iscore::unmarshall<Path<Scenario::TimeNodeModel>>((*it).toObject());
    if(!path.valid())
      return;

    Scenario::TimeNodeModel& tn = path.find(doc);
    tn.trigger()->triggeredByGui();
  }));

  m_answers.insert(std::make_pair(iscore::StringConstant().Message, [this] (const QJsonObject& obj, const WSClient&)
  {
    // The message is stored at the "root" level of the json.
    auto it = obj.find(iscore::StringConstant().Address);
    if(it == obj.end())
      return;

    auto message = iscore::unmarshall<::State::Message>(obj);
    m_dev.updateProxy.updateRemoteValue(message.address, message.value);
  }));


  m_answers.insert(std::make_pair("Play", [&] (const QJsonObject&, const WSClient&)
  {
    doc.app.actions.action<Actions::Play>().action()->trigger();
  }));
  m_answers.insert(std::make_pair("Pause", [&] (const QJsonObject&, const WSClient&)
  {
    doc.app.actions.action<Actions::Play>().action()->trigger();
  }));
  m_answers.insert(std::make_pair("Stop", [&] (const QJsonObject&, const WSClient&)
  {
    doc.app.actions.action<Actions::Stop>().action()->trigger();
  }));
  m_answers.insert(std::make_pair("EnableListening", [&] (const QJsonObject& obj, const WSClient& c)
  {
    auto it = obj.find(iscore::StringConstant().Address);
    if(it == obj.end())
      return;

    auto addr = iscore::unmarshall<::State::Address>((*it).toObject());
    auto d = m_dev.list().findDevice(addr.device);
    if(d)
    {
      d->valueUpdated
          .connect<Receiver, &Receiver::on_valueUpdated>(
            *this);
      d->setListening(addr, true);

      m_listenedAddresses.insert(std::make_pair(addr, c));
    }
  }));
  m_answers.insert(std::make_pair("DisableListening", [&] (const QJsonObject& obj, const WSClient&)
  {
    auto it = obj.find(iscore::StringConstant().Address);
    if(it == obj.end())
      return;

    auto addr = iscore::unmarshall<::State::Address>((*it).toObject());
    auto d = m_dev.list().findDevice(addr.device);
    if(d)
    {
      d->valueUpdated
          .disconnect<Receiver, &Receiver::on_valueUpdated>(
            *this);
      d->setListening(addr, false);
      m_listenedAddresses.erase(addr);
    }
  }));
}


Receiver::~Receiver()
{
  m_server.close();
  for(auto c : m_clients) delete c.socket;
}


void Receiver::registerTimeNode(Path<Scenario::TimeNodeModel> tn)
{
  if(ossia::find(m_activeTimeNodes, tn) != m_activeTimeNodes.end())
    return;

  m_activeTimeNodes.push_back(tn);

  QJsonObject mess;
  mess[iscore::StringConstant().Message] = "TriggerAdded";
  mess[iscore::StringConstant().Path] = toJsonObject(tn);
  mess[iscore::StringConstant().Name] = tn.find(m_dev.context()).metadata().getName();
  QJsonDocument doc{mess};
  auto json = doc.toJson();

  for(auto client : m_clients)
  {
    client.socket->sendTextMessage(json);
  }
}


void Receiver::unregisterTimeNode(Path<Scenario::TimeNodeModel> tn)
{
  if(ossia::find(m_activeTimeNodes, tn) == m_activeTimeNodes.end())
    return;

  m_activeTimeNodes.remove(tn);

  QJsonObject mess;
  mess[iscore::StringConstant().Message] = "TriggerRemoved";
  mess[iscore::StringConstant().Path] = toJsonObject(tn);
  QJsonDocument doc{mess};
  auto json = doc.toJson();

  for(auto client : m_clients)
  {
    client.socket->sendTextMessage(json);
  }
}


void Receiver::onNewConnection()
{
  WSClient client{m_server.nextPendingConnection()};

  connect(client.socket, &QWebSocket::textMessageReceived,
          this, [=] (const auto& b) { this->processTextMessage(b, client); });
  connect(client.socket, &QWebSocket::binaryMessageReceived,
          this, [=] (const auto& b) { this->processBinaryMessage(b, client); });
  connect(client.socket, &QWebSocket::disconnected,
          this, &Receiver::socketDisconnected);

  {
    QJsonObject mess;
    mess[iscore::StringConstant().Message] = "DeviceTree";
    mess["Nodes"] = toJsonObject(m_dev.rootNode());
    QJsonDocument doc{mess};
    client.socket->sendTextMessage(doc.toJson());
  }

  {
    QJsonObject mess;
    mess[iscore::StringConstant().Message] = "TriggerAdded";
    for(auto path : m_activeTimeNodes)
    {
      mess[iscore::StringConstant().Path] = toJsonObject(path);
      mess[iscore::StringConstant().Name] = path.find(m_dev.context()).metadata().getName();
      QJsonDocument doc{mess};
      auto json = doc.toJson();
      client.socket->sendTextMessage(json);
    }
  }

  m_clients.push_back(client);
}


void Receiver::processTextMessage(const QString& message, const WSClient& w)
{
  processBinaryMessage(message.toLatin1(), w);
}


void Receiver::processBinaryMessage(QByteArray message, const WSClient& w)
{
  QJsonParseError error;
  auto doc = QJsonDocument::fromJson(std::move(message), &error);
  if(error.error)
    return;

  auto obj = doc.object();
  auto it = obj.find(iscore::StringConstant().Message);
  if(it == obj.end())
    return;

  auto mess = (*it).toString();
  auto answer_it = m_answers.find(mess);
  if(answer_it == m_answers.end())
    return;

  answer_it->second(obj, w);
}


void Receiver::socketDisconnected()
{
  QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

  if (pClient)
  {
    auto it = ossia::find_if(m_listenedAddresses, [=] (const auto& pair) {
      if(pair.second.socket == pClient)
        return true;
      return false;
    });
    if(it != m_listenedAddresses.end())
      m_listenedAddresses.erase(it);

    m_clients.removeAll(WSClient{pClient});
    pClient->deleteLater();
  }
}

void Receiver::on_valueUpdated(const ::State::Address& addr, const ossia::value& v)
{
  auto it = m_listenedAddresses.find(addr);
  if(it != m_listenedAddresses.end())
  {
    ::State::Message m{::State::AddressAccessor{addr}, v};

    JSONObject::Serializer s;
    s.readFrom(m);
    s.obj[iscore::StringConstant().Message] = iscore::StringConstant().Message;
    QWebSocket* w = it->second.socket;
    w->sendTextMessage(QJsonDocument(s.obj).toJson());
  }

}

}
