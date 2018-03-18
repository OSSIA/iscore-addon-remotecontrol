#include <RemoteControl/ApplicationPlugin.hpp>
#include <RemoteControl/DocumentPlugin.hpp>

#include <core/document/Document.hpp>
#include <core/document/DocumentModel.hpp>

#include <score/tools/IdentifierGeneration.hpp>
namespace RemoteControl
{
ApplicationPlugin::ApplicationPlugin(
        const score::GUIApplicationContext& app):
    GUIApplicationPlugin {app}
{

}

void ApplicationPlugin::on_createdDocument(
        score::Document& doc)
{
    doc.model().addPluginModel(
                new DocumentPlugin{doc.context(), getStrongId(doc.model().pluginModels()), &doc.model()});

}

}

