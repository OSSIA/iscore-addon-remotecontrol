#pragma once
#include <RemoteControl/Settings/Model.hpp>
#include <score/plugins/settingsdelegate/SettingsDelegateView.hpp>
class QCheckBox;
class QDoubleSpinBox;
namespace RemoteControl
{
namespace Settings
{

class View :
        public score::GlobalSettingsView
{
        Q_OBJECT
    public:
        View();
        void setEnabled(bool);

    Q_SIGNALS:
        void enabledChanged(bool);

    private:
        QWidget* getWidget() override;
        QWidget* m_widg{};

        QCheckBox* m_enabled{};

};

}
}
