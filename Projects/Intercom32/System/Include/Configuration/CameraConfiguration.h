#pragma once

#include "Crc32xZlib.h"
#include "lwip/ip_addr.h"
#include "ConfigurationCommon.h"
#include "HalCommon.h"
#include "BaseConfiguration.h"

namespace Configuration
{

using Hal::CameraFrameSize;
using Hal::CameraGainCeiling;
using Hal::CameraModelType;
using Hal::CameraPixelFormat;
using Hal::CameraSpecialEffect;
using Hal::CameraWhiteBalanceMode;
using Utilities::Crc32xZlib;

struct CameraConfigurationData
{
    /// @brief	General Camera configuration.
    struct GeneralConfiguration
    {
        /// @brief	Configuration flags
        union _Settings {
            struct
            {
                bool StreamEnabled : 1;
                bool FaceRecognition : 1;
                bool MovementDetection : 1;
                uint32_t _NotUsed : 29;
            } Flags;
            uint32_t AllFlags;
        } Settings;
        /// @brief	Configuration flags
        union _Changes {
            struct
            {
                bool StreamEnabled : 1;
                bool FaceRecognition : 1;
                bool MovementDetection : 1;
                bool PixelFormat : 1;
                bool FrameSize : 1;
                bool ModelType : 1;
                bool FrameBufferCount : 1;
                bool Quality : 1;
                bool Contrast : 1;
                bool Brightness : 1;
                bool Saturation : 1;
                bool GainCeiling : 1;
                bool ColourBar : 1;
                bool AutoWhiteBalance : 1;
                bool AutoGainControl : 1;
                bool AutoExposure : 1;
                bool HorizontalMirror : 1;
                bool VerticalMirror : 1;
                bool AutoBalanceGain : 1;
                bool ManualGainCeiling : 1;
                bool ExposureTime : 1;
                bool ExposureDsp : 1;
                bool DownsizeEN : 1;
                bool Bpc : 1;
                bool Wpc : 1;
                bool RawGma : 1;
                bool LensCorrection : 1;
                bool SpecialEffect : 1;
                bool WhiteBalanceMode : 1;
                bool AutoExposureLevel : 1;
                uint64_t _NotUsed : 29;
            } Flags;
            uint64_t AllChanges;
        } Changes;
    };

    GeneralConfiguration GeneralConfig = {};
    CameraPixelFormat PixelFormat = CameraPixelFormat::CameraPixelFormatJPEG;
    CameraFrameSize FrameSize = CameraFrameSize::CameraFrameSizeSVGA;
    CameraModelType ModelType = CameraModelType::CameraNone;
    uint8_t FrameBufferCount = 1;
    uint8_t Quality = 10;
    int Contrast = 0;
    int Brightness = 0;
    int Saturation = 0;
    CameraGainCeiling GainCeiling = CameraGainCeiling::Gain2;
    bool ColourBar = false;
    bool AutoWhiteBalance = false;
    bool AutoGainControl = true;
    bool AutoExposure = true;
    bool HorizontalMirror = true;
    bool VerticalMirror = true;
    bool AutoBalanceGain = true;
    uint8_t ManualGainCeiling = 5;
    uint16_t ExposureTime = 204;
    bool ExposureDsp = true;
    bool DownsizeEN = true;
    bool Bpc = false;
    bool Wpc = true;
    bool RawGma = true;
    bool LensCorrection = true;
    CameraSpecialEffect SpecialEffect = CameraSpecialEffect::None;
    CameraWhiteBalanceMode WhiteBalanceMode = CameraWhiteBalanceMode::Auto;
    int AutoExposureLevel = 0;

    CameraConfigurationData() : GeneralConfig()
    {
    }

    uint32_t GetCRC() const
    {
        return Crc32xZlib::GetCrc((unsigned char *)this, sizeof(CameraConfigurationData), Crc32xZlib::Polynomial);
    }
};

class CameraConfiguration : public BaseConfiguration
{
public:
    CameraConfiguration();
    ~CameraConfiguration();
    bool Deserialize(const char * json);
    bool Serialize(char * json, int length);
    const CameraConfigurationData *GetConfiguration() { return &_configuration; }
    void DefaultConfiguration();
    void ApplyConfiguration();

private:
    CameraConfigurationData _configuration = {};
    static constexpr uint16_t JsonConversionLength = 1024;

private:
    /// @brief	Hide Copy constructor.
    CameraConfiguration(const CameraConfiguration &) = delete;

    /// @brief	Hide Assignment operator.
    CameraConfiguration &operator=(const CameraConfiguration &) = delete;

    /// @brief	Hide Move constructor.
    CameraConfiguration(CameraConfiguration &&) = delete;

    /// @brief	Hide Move assignment operator.
    CameraConfiguration &operator=(CameraConfiguration &&) = delete;
};

} // namespace Configuration