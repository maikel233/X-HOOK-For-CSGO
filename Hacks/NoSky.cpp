#include "../Features.h"

//#include "skybox.h"

bool Settings::NoSky::enabled = false;
bool Settings::SkyBox::enabled = false;
int Settings::SkyBox::skyBoxNumber = 0;
ColorVar Settings::NoSky::color = ImColor(0, 0, 0, 255);
float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f, a1 = 0.0f;

std::unordered_map<MaterialHandle_t, ImColor> skyboxMaterials;
std::unordered_map<MaterialHandle_t, ImColor> skyboxMaterials2;

const char* skyBoxNames[] = {
        "cs_baggage_skybox_", // 0
        "cs_tibet", "embassy",
        "italy",
        "jungle",
        "office",
        "nukeblank",
        "sky_venice",
        "sky_cs15_daylight01_hdr",
        "sky_cs15_daylight02_hdr",
        "sky_cs15_daylight03_hdr",
        "sky_cs15_daylight04_hdr",
        "sky_csgo_cloudy01",
        "sky_csgo_night_flat",
        "sky_csgo_night02",
        "sky_csgo_night02b",
        "sky_day02_05",
        "sky_day02_05_hdr",
        "sky_dust",
        "vertigo",
        "vertigo_hdr",
        "vertigoblue_hdr",
        "vietnam", // 21
        "sky_lunacy",
        "sky_borealis01",
        "cliff",
        "blue",
        "city1",
        "neb1",
        "xen8",
        "sky2mh_" // 29
};


void SkyBox::FrameStageNotify(ClientFrameStage_t stage) {

    if (!pEngine->IsInGame() && skyboxMaterials.size() > 0) {
        for (const auto& it : skyboxMaterials) {
            IMaterial* mat = pMaterial->GetMaterial(it.first);

            if (!mat)
                continue;

            mat->GetColorModulate(&r1, &g1, &b1);
            a1 = mat->GetAlphaModulation();

            mat->ColorModulate(r1, g1, b1);
            mat->AlphaModulate(a1);
        }

        skyboxMaterials.clear();
        skyboxMaterials2.clear();
    }

    if (pEngine->IsInGame() && Settings::ESP::enabled && Settings::SkyBox::enabled) {
        static auto LoadSky = reinterpret_cast<void(__fastcall*)(const char*)>(offsets.sigs.LoadSky);
        LoadSky(skyBoxNames[Settings::SkyBox::skyBoxNumber]);
        return;
    }

    if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END)
        return;

    

        for (MaterialHandle_t i = pMaterial->FirstMaterial(); i != pMaterial->InvalidMaterial(); i = pMaterial->NextMaterial(i))
        {
            IMaterial* mat = pMaterial->GetMaterial(i);


        if (!mat || strcmp(mat->GetTextureGroupName(), TEXTURE_GROUP_SKYBOX) != 0)
            continue;
        

        if (skyboxMaterials.find(i) == skyboxMaterials.end()) {
            mat->GetColorModulate(&r1, &g1, &b1);
            a1 = mat->GetAlphaModulation();
            skyboxMaterials.emplace(i, ImColor(r1, g1, b1, a1));
            skyboxMaterials2.emplace(i, ImColor(r1, g1, b1, a1));
        }

        ImColor color = (Settings::NoSky::enabled && Settings::ESP::enabled && !Settings::SkyBox::enabled)
            ? Settings::NoSky::color.Color() : skyboxMaterials2.find(i)->second;

        if (skyboxMaterials.at(i) != color) {
            mat->ColorModulate(color);
            mat->AlphaModulate(color.Value.w);

            skyboxMaterials.at(i) = color;
        }
    }
}