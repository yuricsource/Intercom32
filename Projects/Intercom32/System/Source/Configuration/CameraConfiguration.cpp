#include "CameraConfiguration.h"
#include "Hardware.h"
#include "ArduinoJson.h"
#include <array>
#include "Logger.h"

using Hal::Camera;
using Hal::Hardware;
using Hal::CameraFrameSize;
using Utilities::Logger;

namespace Configuration
{

CameraConfiguration::CameraConfiguration() : BaseConfiguration("Camera Config")
{
}

CameraConfiguration::~CameraConfiguration()
{
}

bool CameraConfiguration::Deserialize(const char *json)
{
    StaticJsonDocument<JsonConversionLength> doc;
    DeserializationError error = deserializeJson(doc, json);
    auto &changes = _configuration.GeneralConfig.Changes.Flags;

    if (error)
    {
        Logger::LogError(Logger::LogSource::Configuration,"Deserialize error: %s\n",error.c_str());
        return false;
    }

    if (doc["framesize"].isNull() == false)
        changes.FrameSize = UpdateConfig(_configuration.FrameSize, (CameraFrameSize) doc["framesize"].as<uint8_t>());

    if (doc["pixelformat"].isNull() == false)
        changes.PixelFormat = UpdateConfig(_configuration.PixelFormat, (CameraPixelFormat) doc["pixelformat"].as<uint8_t>());

    if (doc["framecount"].isNull() == false)
        changes.FrameBufferCount = UpdateConfig(_configuration.FrameBufferCount, doc["framecount"].as<uint8_t>());

    if (doc["quality"].isNull() == false)
        changes.Quality = UpdateConfig(_configuration.Quality, doc["quality"].as<uint8_t>());

    if (doc["contrast"].isNull() == false)
        changes.Contrast = UpdateConfig(_configuration.Contrast, doc["contrast"].as<int>());

    if (doc["brightness"].isNull() == false)
        changes.Brightness = UpdateConfig(_configuration.Brightness, doc["brightness"].as<int>());

    if (doc["saturation"].isNull() == false)
        changes.Saturation = UpdateConfig(_configuration.Saturation, doc["saturation"].as<int>());

    if (doc["gainceiling"].isNull() == false)
        changes.GainCeiling = UpdateConfig(_configuration.GainCeiling, (CameraGainCeiling) doc["gainceiling"].as<uint8_t>());

    if (doc["colorbar"].isNull() == false)
        changes.ColourBar = UpdateConfig(_configuration.ColourBar, doc["colorbar"].as<bool>());

    if (doc["awb"].isNull() == false)
        changes.AutoWhiteBalance = UpdateConfig(_configuration.AutoWhiteBalance, doc["awb"].as<bool>());

    if (doc["agc"].isNull() == false)
        changes.AutoGainControl = UpdateConfig(_configuration.AutoGainControl, doc["agc"].as<bool>());

    if (doc["aec"].isNull() == false)
        changes.AutoExposure = UpdateConfig(_configuration.AutoExposure, doc["aec"].as<bool>());

    if (doc["hmirror"].isNull() == false)
        changes.HorizontalMirror = UpdateConfig(_configuration.HorizontalMirror, doc["hmirror"].as<bool>());

    if (doc["vflip"].isNull() == false)
        changes.VerticalMirror = UpdateConfig(_configuration.VerticalMirror, doc["vflip"].as<bool>());

    if (doc["awb_gain"].isNull() == false)
        changes.AutoBalanceGain = UpdateConfig(_configuration.AutoBalanceGain, doc["awb_gain"].as<bool>());

    if (doc["agc_gain"].isNull() == false)
        changes.ManualGainCeiling = UpdateConfig(_configuration.ManualGainCeiling, doc["agc_gain"].as<uint8_t>());

    if (doc["aec_value"].isNull() == false)
        changes.ExposureTime = UpdateConfig(_configuration.ExposureTime, doc["aec_value"].as<uint16_t>());

    if (doc["aec2"].isNull() == false)
        changes.ExposureDsp = UpdateConfig(_configuration.ExposureDsp, doc["aec2"].as<bool>());

    if (doc["dcw"].isNull() == false)
        changes.DownsizeEN = UpdateConfig(_configuration.DownsizeEN, doc["dcw"].as<bool>());

    if (doc["bpc"].isNull() == false)
        changes.Bpc = UpdateConfig(_configuration.Bpc, doc["bpc"].as<bool>());

    if (doc["wpc"].isNull() == false)
        changes.Wpc = UpdateConfig(_configuration.Wpc, doc["wpc"].as<bool>());

    if (doc["raw_gma"].isNull() == false)
        changes.RawGma = UpdateConfig(_configuration.RawGma, doc["raw_gma"].as<bool>());

    if (doc["lenc"].isNull() == false)
        changes.LensCorrection = UpdateConfig(_configuration.LensCorrection, doc["lenc"].as<bool>());

    if (doc["special_effect"].isNull() == false)
        changes.SpecialEffect = UpdateConfig(_configuration.SpecialEffect, (CameraSpecialEffect)doc["special_effect"].as<uint8_t>());

    if (doc["wb_mode"].isNull() == false)
        changes.WhiteBalanceMode = UpdateConfig(_configuration.WhiteBalanceMode, (CameraWhiteBalanceMode)doc["wb_mode"].as<uint8_t>());

    if (doc["ae_level"].isNull() == false)
        changes.AutoExposureLevel = UpdateConfig(_configuration.AutoExposureLevel, doc["ae_level"].as<int>());

    return true;
}

bool CameraConfiguration::Serialize(char *json, int length)
{

    if (json == nullptr)
        return false;

    StaticJsonDocument<JsonConversionLength> doc;

    doc["framesize"] = static_cast<uint8_t>(_configuration.FrameSize);
    doc["pixelformat"] = static_cast<uint8_t>(_configuration.PixelFormat);
    doc["framecount"] = _configuration.FrameBufferCount;
    doc["quality"] = _configuration.Quality;
    doc["contrast"] = _configuration.Contrast;
    doc["brightness"] = _configuration.Brightness;
    doc["saturation"] = _configuration.Saturation;
    doc["gainceiling"] = static_cast<uint8_t>(_configuration.GainCeiling);
    doc["colorbar"] = _configuration.ColourBar;
    doc["awb"] = _configuration.AutoWhiteBalance;
    doc["agc"] = _configuration.AutoGainControl;
    doc["aec"] = _configuration.AutoExposure;
    doc["hmirror"] = _configuration.HorizontalMirror;
    doc["vflip"] = _configuration.VerticalMirror;
    doc["awb_gain"] = _configuration.AutoBalanceGain;
    doc["agc_gain"] = _configuration.ManualGainCeiling;
    doc["aec_value"] = _configuration.ExposureTime;
    doc["aec2"] = _configuration.ExposureDsp;
    doc["dcw"] = _configuration.DownsizeEN;
    doc["bpc"] = _configuration.Bpc;
    doc["wpc"] = _configuration.Wpc;
    doc["raw_gma"] = _configuration.RawGma;
    doc["lenc"] = _configuration.LensCorrection;
    doc["special_effect"] = static_cast<uint8_t>(_configuration.SpecialEffect);
    doc["wb_mode"] = static_cast<uint8_t>(_configuration.WhiteBalanceMode);
    doc["ae_level"] = static_cast<uint8_t>(_configuration.AutoExposureLevel);
    
    uint16_t jsonLength = measureJson(doc) + 1;

    if (length < jsonLength)
        return false;

    char temp[JsonConversionLength];

    serializeJson(doc, temp);

    memcpy(json, temp, jsonLength + 1);

    return true;
}

void CameraConfiguration::ApplyConfiguration()
{
    Camera &camera = Hardware::Instance()->GetCamera();
    if (_configuration.GeneralConfig.Changes.Flags.FrameSize)
        camera.SetResolution(_configuration.FrameSize);

    if (_configuration.GeneralConfig.Changes.Flags.PixelFormat)
        camera.SetImageFormat(_configuration.PixelFormat);

    if (_configuration.GeneralConfig.Changes.Flags.FrameBufferCount)
        camera.SetFrameBufferCount(_configuration.FrameBufferCount);

    if (_configuration.GeneralConfig.Changes.Flags.Quality)
        camera.SetQuality(_configuration.Quality);

    if (_configuration.GeneralConfig.Changes.Flags.Contrast)
        camera.SetContrast(_configuration.Contrast);

    if (_configuration.GeneralConfig.Changes.Flags.Brightness)
        camera.SetBrightness(_configuration.Brightness);

    if (_configuration.GeneralConfig.Changes.Flags.Saturation)
        camera.SetSaturation(_configuration.Saturation);

    if (_configuration.GeneralConfig.Changes.Flags.GainCeiling)
        camera.SetGainCeiling(_configuration.GainCeiling);

    if (_configuration.GeneralConfig.Changes.Flags.ColourBar)
        camera.SetColourBar(_configuration.ColourBar);

    if (_configuration.GeneralConfig.Changes.Flags.AutoWhiteBalance)
        camera.SetAutoWhiteBalance(_configuration.AutoWhiteBalance);

    if (_configuration.GeneralConfig.Changes.Flags.AutoGainControl)
        camera.SetAutoGainControl(_configuration.AutoGainControl);

    if (_configuration.GeneralConfig.Changes.Flags.AutoExposure)
        camera.SetAutoExposureControl(_configuration.AutoExposure);

    if (_configuration.GeneralConfig.Changes.Flags.HorizontalMirror)
        camera.SetHorizontalMirror(_configuration.HorizontalMirror);

    if (_configuration.GeneralConfig.Changes.Flags.VerticalMirror)
        camera.SetVerticalMirror(_configuration.VerticalMirror);

    if (_configuration.GeneralConfig.Changes.Flags.AutoBalanceGain)
        camera.SetAutoWhiteBalanceGain(_configuration.AutoBalanceGain);

    if (_configuration.GeneralConfig.Changes.Flags.ManualGainCeiling)
        camera.SetManualGainCeiling(_configuration.ManualGainCeiling);

    if (_configuration.GeneralConfig.Changes.Flags.ExposureTime)
        camera.SetExposureTime(_configuration.ExposureTime);

    if (_configuration.GeneralConfig.Changes.Flags.ExposureDsp)
        camera.SetAutoExposureDsp(_configuration.ExposureDsp);

    if (_configuration.GeneralConfig.Changes.Flags.DownsizeEN)
        camera.SetDownsizeEn(_configuration.DownsizeEN);

    if (_configuration.GeneralConfig.Changes.Flags.Bpc)
        camera.SetBPC(_configuration.Bpc);

    if (_configuration.GeneralConfig.Changes.Flags.Wpc)
        camera.SetWPC(_configuration.Wpc);

    if (_configuration.GeneralConfig.Changes.Flags.RawGma)
        camera.SetRawGma(_configuration.RawGma);

    if (_configuration.GeneralConfig.Changes.Flags.LensCorrection)
        camera.SetLensCorrection(_configuration.LensCorrection);

    if (_configuration.GeneralConfig.Changes.Flags.SpecialEffect)
        camera.SetSpecialEffect(_configuration.SpecialEffect);

    if (_configuration.GeneralConfig.Changes.Flags.WhiteBalanceMode)
        camera.SetWhiteBalanceMode(_configuration.WhiteBalanceMode);

    if (_configuration.GeneralConfig.Changes.Flags.AutoExposureLevel)
        camera.SetAutoExposureLevel(_configuration.AutoExposureLevel);

    _configuration.GeneralConfig.Changes.AllChanges = 0;
}

void CameraConfiguration::DefaultConfiguration()
{
    auto &changes = _configuration.GeneralConfig.Changes.Flags;
    // Camera Driver Settings
    changes.FrameSize = UpdateConfig(_configuration.FrameSize, CameraFrameSize::CameraFrameSizeXGA);
    changes.ModelType = UpdateConfig(_configuration.ModelType, CameraModelType::CameraOV2640);
    changes.PixelFormat = UpdateConfig(_configuration.PixelFormat, CameraPixelFormat::CameraPixelFormatJPEG);

    // Sensor Settings
    changes.FrameBufferCount = UpdateConfig(_configuration.FrameBufferCount, static_cast<uint8_t>(1));
    changes.Quality = UpdateConfig(_configuration.Quality, static_cast<uint8_t>(10));
    changes.Contrast = UpdateConfig(_configuration.Contrast, 0);
    changes.Brightness = UpdateConfig(_configuration.Brightness, 0);
    changes.Saturation = UpdateConfig(_configuration.Saturation, 0);
    changes.GainCeiling = UpdateConfig(_configuration.GainCeiling, CameraGainCeiling::Gain2);
    changes.ColourBar = UpdateConfig(_configuration.ColourBar, false);
    changes.AutoWhiteBalance = UpdateConfig(_configuration.AutoWhiteBalance, false);
    changes.AutoGainControl = UpdateConfig(_configuration.AutoGainControl, true);
    changes.AutoExposure = UpdateConfig(_configuration.AutoExposure, true);
    changes.HorizontalMirror = UpdateConfig(_configuration.HorizontalMirror, true);
    changes.VerticalMirror = UpdateConfig(_configuration.VerticalMirror, true);
    changes.AutoBalanceGain = UpdateConfig(_configuration.AutoBalanceGain, true);
    changes.ManualGainCeiling = UpdateConfig(_configuration.ManualGainCeiling, static_cast<uint8_t>(5));
    changes.ExposureTime = UpdateConfig(_configuration.ExposureTime, static_cast<uint16_t>(204));
    changes.ExposureDsp = UpdateConfig(_configuration.ExposureDsp, true);
    changes.DownsizeEN = UpdateConfig(_configuration.DownsizeEN, true);
    changes.Bpc = UpdateConfig(_configuration.Bpc, false);
    changes.Wpc = UpdateConfig(_configuration.Wpc, false);
    changes.RawGma = UpdateConfig(_configuration.RawGma, false);
    changes.LensCorrection = UpdateConfig(_configuration.LensCorrection, true);
    changes.SpecialEffect = UpdateConfig(_configuration.SpecialEffect, CameraSpecialEffect::None);
    changes.WhiteBalanceMode = UpdateConfig(_configuration.WhiteBalanceMode, CameraWhiteBalanceMode::Auto);
    changes.AutoExposureLevel = UpdateConfig(_configuration.AutoExposureLevel, 0);

    ApplyConfiguration();
}

} // namespace Configuration