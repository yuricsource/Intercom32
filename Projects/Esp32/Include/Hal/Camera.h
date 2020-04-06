/*
 * Camera.h
 *
 *  Created on: 5 Dec 2019
 *      Author: yuri
 */

#ifndef MAIN_INCLUDE_HAL_CAMERA_CAMERA_H_
#define MAIN_INCLUDE_HAL_CAMERA_CAMERA_H_

#include "../../components/http_server/my_http_server.h"
// #include "bitmap.h"
#include "HalCommon.h"
#include "Gpio.h"
#include "esp_err.h"
#include "driver/ledc.h"
#include "Camera/Driver/esp_camera.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_event_legacy.h"

namespace Hal
{

class Camera
{

public:

	Camera(Gpio *IoPins);
    
	void Init();
	
	void DeInit();
	
	const camera_fb_t * GetFrameBuffer();
	
	bool Capture();

	bool SetFrameBufferCount(uint8_t frameCount);

	int SetResolution(CameraFrameSize frameSize);

	int SetImageFormat(CameraPixelFormat format);

	int SetQuality(uint8_t quality);
	
	int SetContrast(int contrast);

	int SetBrightness(int brightness);

	int SetSaturation(int saturation);

	int SetGainCeiling (CameraGainCeiling gain);

	int SetColourBar(bool colourBar);
	
	int SetAutoWhiteBalance(bool autoBalance);

	int SetAutoGainControl(bool autoGain);

	int SetAutoExposureControl(bool autoExposure);

	int SetHorizontalMirror(bool horizontalMirror);
	
	int SetVerticalMirror(bool verticalMirror);

	int SetAutoWhiteBalanceGain(bool autoBalanceGain);

	int SetManualGainCeiling(uint8_t autoGainCeiling);

	int SetExposureTime(uint16_t exposureTime);

	int SetAutoExposureDsp(bool exposureDsp);

	int SetDownsizeEn(bool downsize);

	int SetBPC(bool bpc);

	int SetWPC(bool wpc);

	int SetRawGma(bool RawGma);

	int SetLensCorrection(bool lensCorrection);
	
	int SetSpecialEffect(CameraSpecialEffect effect);

	int SetWhiteBalanceMode(CameraWhiteBalanceMode mode);

	int SetAutoExposureLevel(int level);

	~Camera();
	
private:

	bool initialized = false;
	camera_fb_t *_frameBuffer = nullptr;
    camera_config_t _cameraConfig = {};

};
} // namespace Hal

#endif /* MAIN_INCLUDE_HAL_CAMERA_CAMERA_H_ */
