//#include "SkyChanger.h"
//
//#include "settings.h"
//#include "offsets.h"
//#include "interfaces.h"
//
//bool Settings::SkyBox::enabled = false;
//int Settings::SkyBox::skyBoxNumber = 0;
//
//std::vector<std::string> Settings::SkyBox::SkyOptions = { "Off", "cs_baggage_skybox_", "cs_tibet", "embassy", "italy","jungle","nukeblank", "office","sky_cs15_daylight01_hdr","sky_cs15_daylight02_hdr", "sky_cs15_daylight03_hdr","sky_cs15_daylight04_hdr","sky_csgo_cloudy01","sky_csgo_night02","sky_csgo_night02b","sky_dust","sky_venice","vertigo","vietnam" };
//
//namespace SkyChanger
//{
//	bool SkyNeedsUpdate = true;
//
//	void PaintTraverse()
//	{
//		if (!SkyNeedsUpdate)
//			return;
//
//		if (!pEngine->IsInGame())
//			return;
//
//		if (!Settings::SkyBox::skyBoxNumber)
//			return;
//
//		if (pCvar->FindVar("sv_skyname")->strString != Settings::SkyBox::SkyOptions[Settings::SkyBox::skyBoxNumber])
//		{
//			static auto LoadSky = reinterpret_cast< void(__fastcall*)(const char*) >(offsets.sigs.LoadSky);
//			if (!LoadSky);// Utilities::Warn("LoadSky signature unsuccessful");
//			else LoadSky(Settings::SkyBox::SkyOptions[Settings::SkyBox::skyBoxNumber].c_str());
//		}
//
//		SkyNeedsUpdate = false;
//	}
//}
//
//
