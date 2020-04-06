#pragma once
#include <stdio.h>
#include <cstdint>
#include <cstring>
#include <cctype>
#include "ConfigurationCommon.h"

namespace Configuration
{

class BaseConfiguration
{
public:
    template <class T>
    /// @brief Update configuration, if there is a change returns true.
    bool UpdateConfig(T& config, T newConfig)
    {
       if (config != newConfig)
       {
           config = newConfig;
           return true;
       }
       return false;
    }

    /// @brief Set to default configuraiton
    virtual void DefaultConfiguration() = 0;
    
    /// @brief Deserialize the confguration
    virtual bool Deserialize(const char * json) = 0;
    
    /// @brief Serialize the configuration
    virtual bool Serialize(char * json, int length) = 0;
    
    /// @brief Constructor
    BaseConfiguration(const char *name) : _enabled(false)
    {
        if (name == nullptr)
            return;
        size_t size = strlen(name);
        if (size < _name.size())
        {
            memcpy(_name.data(), name, size + 1);
        }
    }

    /// @brief Destructor
    ~BaseConfiguration()
    {
    }

    /// @brief Reset base variables
    void Default()
    {
        _enabled = false;
        _name = {};
    }

    /// @brief Check if the configuration is enabled
    bool IsEnabled()
    {
        return _enabled;
    }

    /// @brief Get the configuration name
    const DeviceName GetName() const
    {
        return _name;
    }

protected:
    DeviceName _name = {};
    bool _enabled = false;

private:
    /// @brief	Hide Copy constructor.
    BaseConfiguration(const BaseConfiguration &) = delete;

    /// @brief	Hide Assignment operator.
    BaseConfiguration &operator=(const BaseConfiguration &) = delete;

    /// @brief	Hide Move constructor.
    BaseConfiguration(BaseConfiguration &&) = delete;

    /// @brief	Hide Move assignment operator.
    BaseConfiguration &operator=(BaseConfiguration &&) = delete;
};
} // namespace Configuration