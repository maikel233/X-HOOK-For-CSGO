#include "Features.h"

bool Settings::SniperCrosshair::enabled = false;
ColorVar Settings::SniperCrosshair::color = ImColor(255, 255, 255, 255);

void SniperCrosshair::Paint() {
	if (!Settings::SniperCrosshair::enabled)
		return;

	if (!pEngine->IsInGame())
		return;

	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!pLocal || !pLocal->GetAlive())
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(
		pLocal->GetActiveWeapon());
	if (!activeWeapon)
		return;

	if (activeWeapon->GetCSWpnData()->GetWeaponType() != CSWeaponType::WEAPONTYPE_SNIPER_RIFLE)
		return;

	int width, height;
	pEngine->GetScreenSize(width, height);

	int x = (int)(width * 0.5f);
	int y = (int)(height * 0.5f);

	// outline horizontal
	Draw::FilledRectangle(Vector2D(x - 4, y - 1), Vector2D(x + 5, y + 2), Color(0, 0, 0, 170));
	// outline vertical
	Draw::FilledRectangle(Vector2D(x - 1, y - 4), Vector2D(x + 2, y + 5), Color(0, 0, 0, 170));

	// line horizontal
	Draw::Line(Vector2D(x - 3, y), Vector2D(x + 4, y),
		Color::FromImColor(Settings::SniperCrosshair::color.Color()));
	// line vertical
	Draw::Line(Vector2D(x, y + 3), Vector2D(x, y - 4),
		Color::FromImColor(Settings::SniperCrosshair::color.Color()));
}
