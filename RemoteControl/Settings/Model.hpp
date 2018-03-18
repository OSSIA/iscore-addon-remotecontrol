#pragma once
#include <score/plugins/settingsdelegate/SettingsDelegateModel.hpp>
#include <score_addon_remotecontrol_export.h>

namespace RemoteControl
{
namespace Settings
{
class SCORE_ADDON_REMOTECONTROL_EXPORT Model :
        public score::SettingsDelegateModel
{
        Q_OBJECT

    Q_PROPERTY(bool Enabled READ getEnabled WRITE setEnabled NOTIFY EnabledChanged FINAL)

        bool m_Enabled = false;
    public:
        Model(QSettings& set, const score::ApplicationContext& ctx);

        SCORE_SETTINGS_PARAMETER_HPP(bool, Enabled)
};

SCORE_SETTINGS_PARAMETER(Model, Enabled)

}
}
