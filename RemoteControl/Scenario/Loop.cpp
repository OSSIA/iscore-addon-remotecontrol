#include "Loop.hpp"
#include <Scenario/Process/Algorithms/Accessors.hpp>
namespace RemoteControl
{

LoopComponent::LoopComponent(
        const Id<iscore::Component>& id,
        Loop::ProcessModel& scenario,
        const LoopComponent::system_t& doc,
        const iscore::DocumentContext& ctx,
        QObject* parent_obj):
    ProcessComponent{scenario, id, "LoopComponent", parent_obj},
    m_hm{*this, scenario, doc, ctx, this}
{
}

template<>
ConstraintComponent* LoopComponent::make<ConstraintComponent, Scenario::ConstraintModel>(
        const Id<iscore::Component>& id,
        Scenario::ConstraintModel& elt,
        const LoopComponent::system_t& doc,
        const iscore::DocumentContext& ctx,
        QObject* parent)
{
    return new ConstraintComponent{id, elt, doc, ctx, parent};
}

template<>
EventComponent* LoopComponent::make<EventComponent, Scenario::EventModel>(
        const Id<iscore::Component>& id,
        Scenario::EventModel& elt,
        const LoopComponent::system_t& doc,
        const iscore::DocumentContext& ctx,
        QObject* parent)
{
    return new EventComponent{id, elt, doc, ctx, parent};
}

template<>
TimeNodeComponent* LoopComponent::make<TimeNodeComponent, Scenario::TimeNodeModel>(
        const Id<iscore::Component>& id,
        Scenario::TimeNodeModel& elt,
        const LoopComponent::system_t& doc,
        const iscore::DocumentContext& ctx,
        QObject* parent)
{
    return new TimeNodeComponent{id, elt, doc, ctx, parent};
}

template<>
StateComponent* LoopComponent::make<StateComponent, Scenario::StateModel>(
        const Id<iscore::Component>& id,
        Scenario::StateModel& elt,
        const LoopComponent::system_t& doc,
        const iscore::DocumentContext& ctx,
        QObject* parent)
{
    return new StateComponent{id, elt, doc, ctx, parent};
}

void LoopComponent::removing(const Scenario::ConstraintModel& elt, const ConstraintComponent& comp)
{
}

void LoopComponent::removing(const Scenario::EventModel& elt, const EventComponent& comp)
{
}

void LoopComponent::removing(const Scenario::TimeNodeModel& elt, const TimeNodeComponent& comp)
{
}

void LoopComponent::removing(const Scenario::StateModel& elt, const StateComponent& comp)
{
}

}
