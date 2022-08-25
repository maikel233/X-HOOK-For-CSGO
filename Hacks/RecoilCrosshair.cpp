#include "../Features.h"

bool Settings::Recoilcrosshair::enabled = false;
bool Settings::Recoilcrosshair::showOnlyWhenShooting = false;

void Recoilcrosshair::Paint()
{
	if (!Settings::ESP::enabled)
		return;
	
	if (!Settings::Recoilcrosshair::enabled)
		return;

	if (!pEngine->IsInGame())
		return;


	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer || !localplayer->GetAlive())
		return;

	if (localplayer->GetShotsFired() < 1 && Settings::Recoilcrosshair::showOnlyWhenShooting)
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
	if (!activeWeapon)
		return;

	if (!activeWeapon->IsRifle())
		return;

	Vector punchAngle = *localplayer->GetAimPunchAngle();

	int ScreenWidth, ScreenHeight;
	pEngine->GetScreenSize(ScreenWidth, ScreenHeight);

	int x = (int)(ScreenWidth * 0.5f);
	int y = (int)(ScreenHeight * 0.5f);
	int dx = ScreenWidth / 90;
	int dy = ScreenHeight / 90;

	int crosshairX = (int)(x - (dx * punchAngle.y));
	int crosshairY = (int)(y + (dy * punchAngle.x));

	// outline horizontal
	Draw::FilledRectangle(Vector2D(crosshairX - 4, crosshairY - 1), Vector2D(crosshairX + 5, crosshairY + 2), Color(0, 0, 0, 170));
	// outline vertical
	Draw::FilledRectangle(Vector2D(crosshairX - 1, crosshairY - 4), Vector2D(crosshairX + 2, crosshairY + 5), Color(0, 0, 0, 170));
	// line horizontal
	Draw::Line(Vector2D(crosshairX - 3, crosshairY), Vector2D(crosshairX + 4, crosshairY), Color(255, 255, 255, 255));
	// line vertical
	Draw::Line(Vector2D(crosshairX, crosshairY + 3), Vector2D(crosshairX, crosshairY - 4), Color(255, 255, 255, 255));
}