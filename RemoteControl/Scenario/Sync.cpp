#include "Sync.hpp"

namespace RemoteControl
{

Sync::Sync(
        const Id<iscore::Component>& id,
        Scenario::TimeSyncModel& timeSync,
        const DocumentPlugin& doc,
        QObject* parent_comp):
    Component{id, "SyncComponent", parent_comp}
{
}

}
