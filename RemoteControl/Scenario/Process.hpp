#pragma once
#include <RemoteControl/DocumentPlugin.hpp>
#include <Process/Process.hpp>
#include <iscore/model/Component.hpp>
#include <iscore/model/ComponentFactory.hpp>
#include <Scenario/Document/Components/ProcessComponent.hpp>
#include <iscore_addon_remotecontrol_export.h>
#include <iscore/plugins/customfactory/ModelFactory.hpp>

namespace RemoteControl
{
class ISCORE_ADDON_REMOTECONTROL_EXPORT ProcessComponent :
        public Process::GenericProcessComponent<DocumentPlugin>
{
        ABSTRACT_COMPONENT_METADATA(RemoteControl::ProcessComponent, "b8a691ea-5352-468d-b78c-04e420c252d1")
    public:
        ProcessComponent(
                Process::ProcessModel& proc,
                DocumentPlugin& doc,
                const Id<iscore::Component>& id,
                const QString& name,
                QObject* parent);

        virtual ~ProcessComponent();
};

template<typename Process_T>
using ProcessComponent_T = Process::GenericProcessComponent_T<ProcessComponent, Process_T>;

class ISCORE_ADDON_REMOTECONTROL_EXPORT ProcessComponentFactory :
        public iscore::GenericComponentFactory<
            Process::ProcessModel,
            DocumentPlugin,
            ProcessComponentFactory>
{
        ISCORE_ABSTRACT_COMPONENT_FACTORY(RemoteControl::ProcessComponent)
    public:
        virtual ~ProcessComponentFactory();
        virtual ProcessComponent* make(
                Process::ProcessModel& proc,
                DocumentPlugin& doc,
                const Id<iscore::Component>&,
                QObject* paren_objt) const = 0;
};

template<
        typename ProcessComponent_T>
class ProcessComponentFactory_T :
        public iscore::GenericComponentFactoryImpl<ProcessComponent_T, ProcessComponentFactory>
{
    public:
        using model_type = typename ProcessComponent_T::model_type;
        ProcessComponent* make(
                Process::ProcessModel& proc,
                DocumentPlugin& doc,
                const Id<iscore::Component>& id,
                QObject* paren_objt) const final override
        {
            return new ProcessComponent_T{static_cast<model_type&>(proc), doc, id, paren_objt};
        }
};

using ProcessComponentFactoryList =
    iscore::GenericComponentFactoryList<
            Process::ProcessModel,
            DocumentPlugin,
            ProcessComponentFactory>;
}
