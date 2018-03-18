#include "score_addon_remotecontrol.hpp"
#include <score/plugins/customfactory/FactorySetup.hpp>
#include <RemoteControl/ApplicationPlugin.hpp>
#include <RemoteControl/Settings/Factory.hpp>

#include <RemoteControl/Scenario/Scenario.hpp>
#include <RemoteControl/Scenario/Loop.hpp>

#include <Scenario/score_plugin_scenario.hpp>
#include <score_plugin_deviceexplorer.hpp>

score_addon_remotecontrol::score_addon_remotecontrol() :
    QObject {}
{
}

score_addon_remotecontrol::~score_addon_remotecontrol()
{

}

score::GUIApplicationPlugin*score_addon_remotecontrol::make_guiApplicationPlugin(
        const score::GUIApplicationContext& app)
{
    return new RemoteControl::ApplicationPlugin{app};
}

std::vector<std::unique_ptr<score::InterfaceListBase> > score_addon_remotecontrol::factoryFamilies()
{
    return make_ptr_vector<score::InterfaceListBase,
            RemoteControl::ProcessComponentFactoryList
            >();
}

std::vector<std::unique_ptr<score::InterfaceBase>> score_addon_remotecontrol::factories(
        const score::ApplicationContext& ctx,
        const score::InterfaceKey& key) const
{
    return instantiate_factories<
            score::ApplicationContext,
            FW<score::SettingsDelegateFactory,
            RemoteControl::Settings::Factory>,
            FW<RemoteControl::ProcessComponentFactory,
                RemoteControl::ScenarioComponentFactory,
                RemoteControl::LoopComponentFactory>
            >(ctx, key);
}

auto score_addon_remotecontrol::required() const
  -> std::vector<score::PluginKey>
{
    return {
      score_plugin_scenario::static_key(),
      score_plugin_deviceexplorer::static_key()
    };
}
