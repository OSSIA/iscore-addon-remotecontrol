#pragma once
#include <Scenario/Document/TimeSync/TimeSyncModel.hpp>
#include <RemoteControl/DocumentPlugin.hpp>


namespace RemoteControl
{
class Sync final :
        public iscore::Component
{
        COMMON_COMPONENT_METADATA("79bdaa90-7e1d-41e5-ab1a-8f68fd0807bf")
    public:
        Sync(
                const Id<iscore::Component>& id,
                Scenario::TimeSyncModel& timeSync,
                const DocumentPlugin& doc,
                QObject* parent_comp);

};
}
