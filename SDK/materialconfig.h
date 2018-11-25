#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"

namespace MaterialConfig {
	extern MaterialSystem_Config_t backupConfig;
	//Hooks
	void OverrideConfig(MaterialSystem_Config_t *gameConfig, bool forceUpdate);
}