#include "Features.h"


bool Settings::lbyindicator::enabled = false;
static float normalYawDiff;
static Color col;

static float fakeAngle;
static float realAngle;


void lbyindicator::CreateMove( CUserCmd* cmd ) {
    if ( !Settings::lbyindicator::enabled || !pEngine->IsInGame() )
        return;

	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!pLocal || !pLocal->GetAlive())
		return;

	float yawDiff = *pLocal->GetLowerBodyYawTarget() - cmd->viewangles.y;
	Math::NormalizeYaw(yawDiff);
	normalYawDiff = yawDiff;

	if ((AntiAim::IsAirborne() ? Settings::AntiAim::Airborne::Yaw::enabled :
		AntiAim::IsMoving() ? Settings::AntiAim::Moving::Yaw::enabled :
		Settings::AntiAim::Standing::Yaw::enabled)) {
		fakeAngle = AntiAim::lastFakeYaw;
		realAngle = AntiAim::lastRealYaw;
	}
	else {
		fakeAngle = 0;
		realAngle = cmd->viewangles.y;
	}
}

auto get_angle_from_speed = [](float speed)
{
auto ideal_angle = RAD2DEG(std::atan2(30.f, speed));
ideal_angle = Math::ClampYaw(ideal_angle);
return ideal_angle;
};

void lbyindicator::Paint() {
	if (!Settings::lbyindicator::enabled || !pEngine->IsInGame())
		return;

	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());

	if (!pLocal || !pLocal->GetAlive())
		return;


	/*show lby and shit*/
	Vector vecLby, vecReal, vecFake, vecCircle;
	Vector org = pLocal->GetVecOrigin(), vOrg;
	Vector end, vEnd;

	Vector Origin = pLocal->GetVecOrigin();

	auto speed = Origin.Length2D();

	static float m_circle_yaw;

	auto ideal_speed_angle = get_angle_from_speed(speed);

	m_circle_yaw = Math::NormalizeYaw2(m_circle_yaw + ideal_speed_angle);

	//	Math::AngleVectors(*player->GetEyeAngles(), forward);
	Math::AngleVectors3(Vector(0, Math::NormalizeYaw2(*pLocal->GetLowerBodyYawTarget()), 0), &vecLby);
	Math::AngleVectors3(Vector(0, Math::NormalizeYaw2(realAngle), 0), &vecReal);
	Math::AngleVectors3(Vector(0, Math::NormalizeYaw2(fakeAngle), 0), &vecFake);
	Vector circle_angle = Vector(0, Math::NormalizeYaw2(m_circle_yaw), 0);
	vecCircle = circle_angle.Direction();

	char lbyText[5]; 
	snprintf(lbyText, 5, "%.0f", (*pLocal->GetLowerBodyYawTarget()));


	end = org + (vecFake * 70);

	if (Draw::WorldToScreen(org, vOrg) && Draw::WorldToScreen(end, vEnd))
	{
		Draw::Text(vOrg.x, vOrg.y - 20, lbyText, esp_font, Color(255, 255, 255, 255));
		Draw::Line(Vector2D(vOrg.x, vOrg.y), Vector2D(vEnd.x, vEnd.y), Color(0, 255, 0, 255));
	}
	end = org + (vecReal * 70);

	if (Draw::WorldToScreen(org, vOrg) && Draw::WorldToScreen(end, vEnd))
	{
		Draw::Line(Vector2D(vOrg.x, vOrg.y), Vector2D(vEnd.x, vEnd.y), Color(255, 0, 0, 255));
	}
	end = org + (vecLby * 70);

	if (Draw::WorldToScreen(org, vOrg) && Draw::WorldToScreen(end, vEnd))
	{
		Draw::Line(Vector2D(vOrg.x, vOrg.y), Vector2D(vEnd.x, vEnd.y), Color(0, 0, 255, 255));
	}
	Vector vel = pLocal->GetVelocity().Normalized();

	end = org + (vecCircle * (vel.Length2D()));

	if (Draw::WorldToScreen(org, vOrg) && Draw::WorldToScreen(end, vEnd))
	{
		Draw::Line(Vector2D(vOrg.x, vOrg.y), Vector2D(vEnd.x, vEnd.y), Color(255, 0, 255, 255));
	}


	static int width = 0;
	static int height = 0;
	static int radarCenterX = 0;
	static int radarCenterY = 0;
	static int radarNorthEndY = 0;
	static int radar125EndX = 0; // 90 + 35
	static int radar125EndY = 0;
	static int radar55EndX = 0; // 90 - 35
	static int radar55EndY = 0;

	// Seemed a bit slow, so did some autistic tweaks
	if (width == 0) {
		pEngine->GetScreenSize(width, height);
		radarCenterX = width * .3;
		radarCenterY = height * .5;
		radarNorthEndY = radarCenterY - 55;
		radar125EndX = 55 * cos(DEG2RAD(90 + 35)) + radarCenterX;
		radar125EndY = height * .5 - (55 * sin(DEG2RAD(90 + 35)));
		radar55EndX = 55 * cos(DEG2RAD(90 - 35)) + radarCenterX;
		radar55EndY = height * .5 - (55 * sin(DEG2RAD(90 - 35)));
	}

	float realYawDiff = normalYawDiff;

	if (AntiAim::isAntiAiming) {
		realYawDiff = *pLocal->GetLowerBodyYawTarget() - AntiAim::lastRealYaw;
		float fakeYawDiff = (*pLocal->GetLowerBodyYawTarget()) - AntiAim::lastFakeYaw;
		Math::NormalizeYaw(realYawDiff);
		Math::NormalizeYaw(fakeYawDiff);
		Draw::Line(Vector2D(radarCenterX, radarCenterY),
			Vector2D(55 * cos(DEG2RAD(90 - fakeYawDiff)) + radarCenterX,
				radarCenterY - (55 * sin(DEG2RAD(90 - fakeYawDiff)))), Color(10, 10, 200));
	}
	if (std::abs(realYawDiff) >= 35.0f) {
		col = Color(0, 235, 0);
	}
	else {
		col = Color(196, 5, 5);
	}
	
	Draw::OutlinedCircle(radarCenterX, radarCenterY, 32, 55, Color(0, 0, 0));
	Draw::Circle(Vector2D(radarCenterX + 55, radarCenterY + 55), 16, 5, Color(col.r, col.g, col.b));

	Draw::Line(Vector2D(radarCenterX, radarCenterY), Vector2D(radarCenterX, radarNorthEndY), Color(0, 0, 0)); // Const Line facing North
	
	Draw::Line(Vector2D(radarCenterX, radarCenterY), Vector2D(radar125EndX, radar125EndY),
		Color(0, 0, 0)); // Const Line Left-of N-Line
	Draw::Line(Vector2D(radarCenterX, radarCenterY), Vector2D(radar55EndX, radar55EndY),
		Color(0, 0, 0)); // Const Line Right-of N-Line
	
	Draw::Line(Vector2D(radarCenterX, radarCenterY), Vector2D(55 * cos(DEG2RAD(90 - realYawDiff)) + radarCenterX,
		radarCenterY -
		(55 * sin(DEG2RAD(90 - realYawDiff)))),
		Color(col.r, col.g, col.b)); // Real Yaw Line

	Draw::Text(Vector2D(radarCenterX, radarNorthEndY - 20), lbyText, esp_font, Color(20, 0, 0));
}



