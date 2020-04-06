

#include <stdio.h>
#include <string.h>

#include "Camera.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "Camera/Driver/esp_camera.h"
#include "string.h"

namespace Hal
{

Camera::Camera(Gpio *IoPins)
{
    // Implement assert and check in here
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    _cameraConfig.ledc_channel = LEDC_CHANNEL_0;
    _cameraConfig.ledc_timer = LEDC_TIMER_0;
    _cameraConfig.pin_d0 = 5;
    _cameraConfig.pin_d1 = 18;
    _cameraConfig.pin_d2 = 19;
    _cameraConfig.pin_d3 = 21;
    _cameraConfig.pin_d4 = 36;
    _cameraConfig.pin_d5 = 39;
    _cameraConfig.pin_d6 = 34;
    _cameraConfig.pin_d7 = 35;
    _cameraConfig.pin_xclk = 0;
    _cameraConfig.pin_pclk = 22;
    _cameraConfig.pin_vsync = 25;
    _cameraConfig.pin_href = 23;
    _cameraConfig.pin_sscb_sda = 26;
    _cameraConfig.pin_sscb_scl = 27;
    _cameraConfig.pin_reset = -1;
    _cameraConfig.pin_pwdn = 32;
    _cameraConfig.xclk_freq_hz = 20000000;
    _cameraConfig.jpeg_quality = 10;
    _cameraConfig.fb_count = 1;
    _cameraConfig.frame_size = static_cast<framesize_t>(CameraFrameSize::CameraFrameSizeSXGA);
    _cameraConfig.pixel_format = static_cast<pixformat_t>(CameraPixelFormat::CameraPixelFormatJPEG);
}

const camera_fb_t *Camera::GetFrameBuffer()
{
    return _frameBuffer;
}

bool Camera::SetFrameBufferCount(uint8_t frameCount)
{
    if (initialized || frameCount == 0 || frameCount > 10)
        return false;

    _cameraConfig.fb_count = frameCount;
    return true;
}

bool Camera::Capture()
{
    _frameBuffer = esp_camera_fb_get();
    if (_frameBuffer == nullptr)
    {
        printf("Camera capture failed\n");
        return false;
    }
    return true;
}

void Camera::DeInit()
{
    if (initialized)
    {
        esp_camera_deinit();
        initialized = false;
    }
}

int Camera::SetAutoExposureLevel(int level)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_ae_level(s, static_cast<int>(level));
    }
    return 0;
}

int Camera::SetWhiteBalanceMode(CameraWhiteBalanceMode mode)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_wb_mode(s, static_cast<int>(mode));
    }
    return 0;
}

int Camera::SetSpecialEffect(CameraSpecialEffect effect)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_special_effect(s, static_cast<int>(effect));
    }
    return 0;
}

int Camera::SetLensCorrection(bool lensCorrection)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_lenc(s, lensCorrection);
    }
    return 0;
}

int Camera::SetRawGma(bool rawGma)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_raw_gma(s, rawGma);
    }
    return 0;
}

int Camera::SetWPC(bool wpc)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_wpc(s, wpc);
    }
    return 0;
}

int Camera::SetBPC(bool bpc)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_bpc(s, bpc);
    }
    return 0;
}


int Camera::SetDownsizeEn(bool downsize)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_dcw(s, downsize);
    }
    return 0;
}

int Camera::SetAutoExposureDsp(bool exposureDsp)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_aec2(s, exposureDsp);
    }
    return 0;
}

int Camera::SetExposureTime(uint16_t exposureTime)
{
    if(exposureTime > 1200)
        return -1;
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_aec_value(s, exposureTime);
    }
    return 0;
}

int Camera::SetManualGainCeiling(uint8_t manualGainCeiling)
{
    if(manualGainCeiling > 30)
        return -1;
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_agc_gain(s, manualGainCeiling);
    }
    return 0;
}

int Camera::SetAutoWhiteBalanceGain(bool autoBalanceGain)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_awb_gain(s, static_cast<int>(autoBalanceGain));
    }
    return 0;
}

int Camera::SetVerticalMirror(bool verticalMirror)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_vflip(s, static_cast<int>(verticalMirror));
    }
    return 0;
}

int Camera::SetHorizontalMirror(bool horizontalMirror)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_hmirror(s, static_cast<int>(horizontalMirror));
    }
    return 0;
}

int Camera::SetAutoExposureControl(bool autoExposure)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_exposure_ctrl(s, static_cast<int>(autoExposure));
    }
    return 0;
}

int Camera::SetAutoGainControl(bool autoGain)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_gain_ctrl(s, static_cast<int>(autoGain));
    }
    return 0;
}

int Camera::SetAutoWhiteBalance(bool autoBalance)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_whitebal(s, static_cast<int>(autoBalance));
    }
    return 0;
}

int Camera::SetColourBar(bool colourBar)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_colorbar(s, static_cast<int>(colourBar));
    }
    return 0;
}

int Camera::SetGainCeiling(CameraGainCeiling gain)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_gainceiling(s, static_cast<gainceiling_t>(gain));
    }
    return 0;
}

int Camera::SetSaturation(int saturation)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_saturation(s, saturation);
    }
    return 0;
}

int Camera::SetBrightness(int brightness)
{
    if (brightness < -2 || brightness > 2)
        return -1;
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_brightness(s, brightness);
    }
    return 0;
}

int Camera::SetContrast(int contrast)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_contrast(s, contrast);
    }
    return 0;
}

int Camera::SetQuality(uint8_t quality)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_quality(s, quality);
    }
    return 0;
}

int Camera::SetResolution(CameraFrameSize frameSize)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_framesize(s, static_cast<framesize_t>(frameSize));
    }
    return 0;
}

int Camera::SetImageFormat(CameraPixelFormat format)
{
    if (initialized)
    {
        sensor_t *s = esp_camera_sensor_get();
        return s->set_pixformat(s, static_cast<pixformat_t>(format));
    }
    return 0;
}

void Camera::Init()
{
    if (initialized)
        return;

    esp_err_t err = esp_camera_init((camera_config_t *)&_cameraConfig);
    if (err != ESP_OK)
    {
        printf("Camera init failed with error 0x%x", err);
        return;
    }

    initialized = true;
}

Camera::~Camera()
{
}

} // namespace Hal
