#pragma once
#include <Windows.h>
#include <ctime>
#include <chrono>
#include <thread>
#include <iostream>
#include <tchar.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <d3d9.h>

#include "xhook.h"
#include "Interfaces.h"
#include "SDK\NetvarManager.h"
#include "Utils/FindPattern.h"
#include "Hooks.h"
#include "Renderer.h"
#include "Settings.h"
#include "Utils/Util.h"
#include <wininet.h>
#include "Features.h"
#include "Config.h"

clock_t Timer;

//.DLL
#define CLIENT "client.dll"
#define ENGINE "engine.dll"
#define MATERIALSYSTEM "materialsystem.dll"
#define VGUI2 "vgui2.dll"
#define VPHYSICS "vphysics.dll"
#define MATCHMAKING "matchmaking.dll"
#define LOCALIZE "localize.dll"
#define VGUIMATSURFACE "vguimatsurface.dll"
#define INPUTSYSTEM "inputsystem.dll"
#define XHOOKCURRGAME "Counter-Strike: Global Offensive"
#define XHOOKCURRGAMEX "XHOOK for Counter-Strike: Global Offensive"
#define XHOOKCURRVERSION "V1.35"
