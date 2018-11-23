#include "Features.h"
#include <vector>
bool Settings::InventoryChanger::enabled = false;
bool Settings::InventoryChanger::medalsEnabled = false;
int Settings::Misc::misc_Rank = 18;
int Settings::Misc::misc_Level = 1;
int Settings::Misc::misc_XP = 1;
int Settings::Misc::misc_Wins = 1;
int Settings::Misc::misc_Friendly = 1;
int Settings::Misc::misc_Leader = 1;
int Settings::Misc::misc_Teacher = 1;

std::vector<uint32_t> Settings::InventoryChanger::medals = { };
bool Settings::InventoryChanger::equipped_medal_override;
uint32_t Settings::InventoryChanger::equipped_medal;

