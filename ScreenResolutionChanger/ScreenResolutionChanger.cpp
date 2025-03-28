#include <iostream>
#include <windows.h>


void PrintCurrentDisplaySettings()
{
	DEVMODEA deviceMode = { 0 };
	deviceMode.dmSize = sizeof(deviceMode);

	if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &deviceMode)) {
		std::cout << "Current display settings:" << std::endl;
		std::cout << " - color depth : " << deviceMode.dmBitsPerPel << " bit/pix" << std::endl;
		std::cout << " - width : " << deviceMode.dmPelsWidth << std::endl;
		std::cout << " - height : " << deviceMode.dmPelsHeight << std::endl;
		std::cout << " - frequence : " << deviceMode.dmDisplayFrequency << " Hz" << std::endl;
	}
	else {
		std::cerr << "Ошибка EnumDisplaySettings: " << GetLastError() << std::endl;
	}
}

int main()
{
	PrintCurrentDisplaySettings();
	return 0;
}
