#include "materialconfig.h"

MaterialSystem_Config_t Settings::MaterialConfig::config = {};
MaterialSystem_Config_t MaterialConfig::backupConfig = {};
bool Settings::MaterialConfig::enabled = false;

//TODO: Config Saving for this.

void MaterialConfig::OverrideConfig(MaterialSystem_Config_t *gameConfig, bool forceUpdate) {
	static bool bFirst = true;
	if (bFirst) {
		MaterialConfig::backupConfig = *gameConfig;
		bFirst = false;
	}
	/* Initialize the config struct we are modifiying with the values from the game. */
	if (!Settings::MaterialConfig::config.cheatConfigInitialized) {
		Settings::MaterialConfig::config = *gameConfig;
		Settings::MaterialConfig::config.cheatConfigInitialized = true;
	}
	/*
	if( Settings::MaterialConfig::potatoMode ){ // crashes sometimes because CSGO sucks
	Settings::DisablePostProcessing::enabled = true;
	Settings::MaterialConfig::config.bFilterLightmaps = false;
	//Settings::MaterialConfig::config.m_bFastNoBump = true;
	Settings::MaterialConfig::config.bShowSpecular = false;
	Settings::MaterialConfig::config.bShowDiffuse = false;
	Settings::MaterialConfig::config.m_bSupportFlashlight = false;
	Settings::MaterialConfig::config.bShowLowResImage = true;
	Settings::MaterialConfig::config.skipMipLevels = 16;
	//Settings::MaterialConfig::config.m_nForceAnisotropicLevel = 0;
	*gameConfig = Settings::MaterialConfig::config;
	return;
	}*/

	if (!Settings::MaterialConfig::enabled) {
		return;
	}

	*gameConfig = Settings::MaterialConfig::config;
}