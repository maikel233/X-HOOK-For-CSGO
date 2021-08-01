#pragma once

#include <Windows.h>
#include "Vector.h"
#include "getvfunc.h"
#include "Color.h"
#include "offsets.h"
#include "Definitions.h"
#include "CView.h"
#include "IBaseClientDll.h"
#include "IClientMode.h"
#include "IAppSystem.h"
#include "IInputSystem.h"
#include "IMaterialSystem.h"
#include "IVModelRender.h"
#include "IClientEntity.h"
#include "IClientEntityList.h"
#include "CGlobalVars.h"
#include "IVPanel.h"
#include "ISurface.h"
#include "IEngineTrace.h"
#include "IVModelInfo.h"
#include "IPhysicalSurfaceProps.h"
#include "CPlayerResource.h"
#include "iLocalize.h"
#include "IVDebugOverlay.h"
#include "CEffects.h"
#include "ICvar.h"
#include "KeyValues.h"
#include "IGameEventManager.h"
#include "CClientState.h"
#include "IEngineClient.h"
#include "CNetworkStringTable.h"
#include "IPrediction.h"
#include "GlowObject.h"
//#include "ICommandLine.h" // Param check
#include "materialsystem_config.h"
#include "IMaterial.h"