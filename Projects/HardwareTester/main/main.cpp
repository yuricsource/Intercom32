
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <esp_attr.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "Hardware.h"
#include "Tests.h"
#include "Rng.h"
#include "esp_http_server.h"
#include "CameraStreamTest.h"
#include <cstring>

void executetMenu(char Test)
{
	switch (Test)
	{

	case 't':
	case 'T':
		LedMenu();
		break;
	case 'r':
	case 'R':
		SoftwareResetTest();
		break;
	case 'f':
	case 'F':
		PutCpuToSleep();
		break;
	case 's':
	case 'S':
		TestSpiffs();
		break;
		break;
		break;
	case 'w':
	case 'W':
		WifiMenu();
		break;
	case 'c':
	case 'C':
		CameraMenu();
		break;
	case 'd':
	case 'D':
		TestTimer();
		break;
	default:
		break;
	}

	printf("\n");
	printf("Main menu:\n");
	printf("----------\n");
	printf("[T] - Led Menu\n");
	printf("[R] - Software Reset Test\n");
	printf("[F] - Deep Sleep for 5 Seconds.\n");
	printf("[S] - Test SPIFFS\n");
	printf("[W] - WiFi Menu\n");
	printf("[C] - Camera Menu\n");
	printf("[D] - Test Timer [ON/OFF]\n");
}

extern "C" void app_main(void)
{
	Hal::Hardware::Instance();
 	printf("Hardware Tester for ESP32\n");

	TestClass testClass;
	char test = 0;
	
	Hal::Hardware::Instance()->GetWifi().Disable();
	Hal::Hardware::Instance()->GetWifi().SetSsid("Camera Wifi", strlen("Camera Wifi"));
	Hal::Hardware::Instance()->GetWifi().SetPassword("123cam123", strlen("123cam123"));
	Hal::Hardware::Instance()->GetWifi().SetMode(Hal::WifiModeConfiguration::HotSpot);
	Hal::Hardware::Instance()->GetWifi().SetAuthentication(Hal::WifiAuthenticationMode::Wpa2Psk);
	Hal::Hardware::Instance()->GetWifi().Enable();
	// Hal::Hardware::Instance()->GetCamera().Init();
	// startCameraServer();

	while (1)
	{
		executetMenu(test);
		test = ReadKey();
	}
}
