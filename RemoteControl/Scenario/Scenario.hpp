#pragma once
#include <RemoteControl/Scenario/Constraint.hpp>
#include <RemoteControl/Scenario/Event.hpp>
#include <RemoteControl/Scenario/Sync.hpp>
#include <RemoteControl/Scenario/State.hpp>
#include <Scenario/Document/Components/ScenarioComponent.hpp>
#include <QMetaObject>

namespace RemoteControl
{
class ScenarioBase :
        public ProcessComponent_T<Scenario::ProcessModel>
{
        COMPONENT_METADATA("fce752e0-e37a-4b71-bc2a-65366ec87152")

    public:
       ScenarioBase(
               Scenario::ProcessModel& scenario,
               DocumentPlugin& doc,
                const Id<iscore::Component>& id,
               QObject* parent_obj);

       template<typename Component_T, typename Element>
       Component_T* make(
               const Id<iscore::Component>& id,
               Element& elt)
       {
           return new Component_T{id, elt, system(), this};
       }

       template<typename... Args>
       bool removing(Args&&...) { return true; }
       template<typename... Args>
       void removed(Args&&...) { }
};

using ScenarioComponent = HierarchicalScenarioComponent<
    ScenarioBase,
    Scenario::ProcessModel,
    Constraint,
    Event,
    Sync,
    State>;


using ScenarioComponentFactory = ProcessComponentFactory_T<ScenarioComponent>;
}
