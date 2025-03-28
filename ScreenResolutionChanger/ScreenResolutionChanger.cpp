#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tchar.h>

void PrintCurrentDisplaySettings(DEVMODEA *deviceMode)
{
	if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, deviceMode)) {
		std::cout << "Current display settings:" << std::endl;
		//std::wcout << L" - display: " << deviceMode->dmDeviceName << std::endl;
		std::cout << " - color depth : " << deviceMode->dmBitsPerPel << " bit/pix" << std::endl;
		std::cout << " - width : " << deviceMode->dmPelsWidth << std::endl;
		std::cout << " - height : " << deviceMode->dmPelsHeight << std::endl;
		std::cout << " - frequence : " << deviceMode->dmDisplayFrequency << " Hz" << std::endl;
	}
	else {
		std::cerr << "Ошибка EnumDisplaySettings: " << GetLastError() << std::endl;
	}
}

void ChangeCurrentDisplaySettings(int colorDepth, int width, int height, int frequence)
{
	DEVMODEA dm = { 0 };
	dm.dmSize = sizeof(dm);
	dm.dmBitsPerPel = colorDepth;
	dm.dmPelsHeight = height;
	dm.dmPelsWidth = width;
	dm.dmDisplayFrequency = frequence;
	dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

	LONG result = ChangeDisplaySettingsA(&dm, CDS_FULLSCREEN);

	if (result != DISP_CHANGE_SUCCESSFUL)
	{
		std::cerr << "Ошибка ChangeDisplaySettingsA: " << result << std::endl;
	}
}

void PrintProcessNameAndId(DWORD processId)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
		}
	}

	std::wcout << "(PID: "<< processId << ") PNAME: " << szProcessName << std::endl;
}	

void GetAllProcess()
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		cProcesses = cbNeeded / sizeof(DWORD);

		for (i = 0; i < cProcesses; i++)
		{
			if (aProcesses[i] != 0)
			{
				PrintProcessNameAndId(aProcesses[i]);
			}
		}
	}
}

int main()
{
	DEVMODEA deviceMode = { 0 };
	deviceMode.dmSize = sizeof(deviceMode);
	
	PrintCurrentDisplaySettings(&deviceMode);
	
	DEVMODEA deviceModeDefaultSetup = { 0 };
	deviceModeDefaultSetup.dmSize = sizeof(deviceModeDefaultSetup);
	deviceModeDefaultSetup = deviceMode;

	int choice = NULL;
	while (choice != 4)
	{
		std::cout << "------------------------" << std::endl;
		std::cout << "What do want to do?" << std::endl;
		std::cout << "1. Change display settings." << std::endl;
		std::cout << "2. Go to default display settings." << std::endl;
		std::cout << "3. Set custom resolution for apllication." << std::endl;
		std::cout << "4. Close application." << std::endl;
		std::cout << "------------------------" << std::endl;
		std::cout << "Your choice(input number):" << std::endl;
		std::cin >> choice;

		if (choice == 1)
		{
			int colorDepth, width, height, frequency;
			std::cout << "Input color depth(current: " << deviceMode.dmBitsPerPel << " bit/pix):" << std::endl;
			std::cin >> colorDepth;
			std::cout << "Input width(current: " << deviceMode.dmPelsWidth << "):" << std::endl;
			std::cin >> width;
			std::cout << "Input height(current: " << deviceMode.dmPelsHeight << "):" << std::endl;
			std::cin >> height;
			std::cout << "Input frequency(current: " << deviceMode.dmDisplayFrequency << " Hz):" << std::endl;
			std::cin >> frequency;
		
			ChangeCurrentDisplaySettings(colorDepth, width, height, frequency);
		}
		else if (choice == 2)
		{
			ChangeDisplaySettingsA(&deviceModeDefaultSetup, 0);
		}
		else if (choice == 3)
		{
			GetAllProcess();
		}
		PrintCurrentDisplaySettings(&deviceMode);
	}
}
