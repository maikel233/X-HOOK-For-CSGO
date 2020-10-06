#pragma once
#include <windows.h>
#include <Psapi.h>
#include <stdint.h>
#include <fstream>
#include "Vector.h"
//uintptr_t FindPattern(const char* module, const char* pattern_string, const char* mask);
DWORD FindPattern(std::string moduleName, std::string pattern);
//uintptr_t FindPatternMask(std::string module, BYTE* mask, char* szMask);
//uintptr_t findPattern3(const wchar_t* module, const char* pattern);