#include "ConfigurationAgent.h"
#include "ApplicationAgent.h"

namespace Configuration
{

ConfigurationAgent *ConfigurationAgent::_configurations;

ConfigurationAgent::ConfigurationAgent()
{
    if (_configurations == nullptr)
    {
        _configurations = this;
    }
    else
    {
        printf("Error!\n");
    }
}

ConfigurationAgent::~ConfigurationAgent()
{
}

void ConfigurationAgent::UseDefaultConfiguration()
{
    _boardConfiguration.DefaultConfiguration();
    _cameraConfiguration.DefaultConfiguration();
    Applications::ApplicationAgent::Instance()->GetWifi().ConfigurationUpdated();
}

void ConfigurationAgent::DefaulAllConfigurations()
{
    _boardConfiguration.DefaultConfiguration();
    _cameraConfiguration.DefaultConfiguration();
    
    // Save afterwards
}

} // namespace Configuration