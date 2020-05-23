#pragma once

#include "Crc32xZlib.h"
#include "lwip/ip_addr.h"
#include "ConfigurationCommon.h"
#include "HalCommon.h"
#include "BoardConfiguration.h"
#include "CameraConfiguration.h"

namespace Configuration
{

class ConfigurationAgent
{
public:

    ConfigurationAgent();

    ~ConfigurationAgent();

    static inline ConfigurationAgent *Instance()
    {
        if (_configurations == nullptr)
        {
            _configurations = new ConfigurationAgent();
        }
        return _configurations;
    }

    BoardConfiguration *GetBoardConfiguration() {return &_boardConfiguration;}
    CameraConfiguration *GetCameraConfiguration() {return &_cameraConfiguration;}

    void DefaulAllConfigurations();
    void UseDefaultConfiguration();

private:

static ConfigurationAgent *_configurations;
BoardConfiguration _boardConfiguration = {};
CameraConfiguration _cameraConfiguration = {};

private:
    /// @brief	Hide Copy constructor.
    ConfigurationAgent(const ConfigurationAgent &) = delete;

    /// @brief	Hide Assignment operator.
    ConfigurationAgent &operator=(const ConfigurationAgent &) = delete;

    /// @brief	Hide Move constructor.
    ConfigurationAgent(ConfigurationAgent &&) = delete;

    /// @brief	Hide Move assignment operator.
    ConfigurationAgent &operator=(ConfigurationAgent &&) = delete;
};

} // namespace Configuration