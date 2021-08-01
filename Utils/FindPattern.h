#pragma once
#include <windows.h>
#include <Psapi.h>
#include <fstream>

DWORD FindPattern(std::string moduleName, std::string pattern);
