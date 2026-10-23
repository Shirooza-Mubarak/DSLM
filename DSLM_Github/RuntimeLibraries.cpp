#include <windows.h>
#include <delayimp.h>
#include <string>
#include <cstring>

static FARPROC WINAPI LoadRuntimeLibrary(
	unsigned notification, PDelayLoadInfo info)
{
	if (notification != dliNotePreLoadLibrary)
		return nullptr;

	const wchar_t* folder = nullptr;

	if (_stricmp(info->szDll, "dcamapi.dll") == 0)
		folder = L"CameraFiles\\";
	else if (_stricmp(info->szDll, "PI_GCS2_DLL_x64.dll") == 0)
		folder = L"StageFiles\\";
	else if (_stricmp(info->szDll, "RTC5DLLx64.dll") == 0)
		folder = L"ScannerFiles\\";
	else
		return nullptr;

	wchar_t executablePath[32768];

	DWORD length = GetModuleFileNameW(
		nullptr, executablePath, 32768);

	if (length == 0 || length >= 32768)
		return nullptr;

	std::wstring path(executablePath, length);
	size_t separator = path.find_last_of(L"\\/");

	if (separator == std::wstring::npos)
		return nullptr;

	path.resize(separator + 1);

	// Go from x64\Release or x64\Debug back to the DSLM folder.
	path += L"..\\..\\";
	path += folder;

	const char* name = info->szDll;

	while (*name)
		path += static_cast<wchar_t>(*name++);

	HMODULE library = LoadLibraryExW(
		path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

	return reinterpret_cast<FARPROC>(library);
}

extern "C" const PfnDliHook __pfnDliNotifyHook2 =
LoadRuntimeLibrary;