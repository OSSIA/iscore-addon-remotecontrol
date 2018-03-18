#include "Model.hpp"
#include <QSettings>

namespace RemoteControl
{
namespace Settings
{

namespace Parameters
{
        const score::sp<ModelEnabledParameter> Enabled{
            QStringLiteral("RemoteControl/Enabled"),
                    false};

        static auto list() {
            return std::tie(Enabled);
        }
}

Model::Model(QSettings& set, const score::ApplicationContext& ctx)
{
    score::setupDefaultSettings(set, Parameters::list(), *this);
}

SCORE_SETTINGS_PARAMETER_CPP(bool, Model, Enabled)
}
}
