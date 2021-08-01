#include "../Features.h"

bool Settings::AngleIndicator::enabled = false;
bool Settings::AngleIndicator::Veloc = false;
static float fakeAngle;
static float realAngle;
static float velocity;

void AngleIndicator::PostPredictionCreateMove( CUserCmd* cmd ) {

	if (!Settings::AngleIndicator::enabled && !Settings::AngleIndicator::Veloc)
		return;

    if ( !pEngine->IsInGame() )
        return;

    C_BasePlayer* pLocal = ( C_BasePlayer* ) pEntityList->GetClientEntity( pEngine->GetLocalPlayer() );
    if ( !pLocal || !pLocal->GetAlive() )
        return;

    if ( ( AntiAim::IsAirborne() ? Settings::AntiAim::Airborne::Yaw::enabled :
           AntiAim::IsMoving() ? Settings::AntiAim::Moving::Yaw::enabled :
           Settings::AntiAim::Standing::Yaw::enabled ) ) {
        fakeAngle = AntiAim::lastFakeYaw;
        realAngle = AntiAim::lastRealYaw;
        velocity = pLocal->GetVelocity().Length2D();
    } else {
        fakeAngle = 0;
        realAngle = cmd->viewangles.y;
        velocity = pLocal->GetVelocity().Length2D();
    }
}

void AngleIndicator::Paint() {
	
	if (!Settings::AngleIndicator::enabled && !Settings::AngleIndicator::Veloc)
		return;

	if (!pEngine->IsInGame())
		return;

	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!pLocal || !pLocal->GetAlive())
		return;

	// Creation of the text

	std::ostringstream stream;
	stream << Math::RoundFloat(realAngle);
	std::string angleText(stream.str());

	std::string text(XorStr("Real: "));
	text.append(angleText);

	std::ostringstream stream2;
	stream2 << Math::RoundFloat(fakeAngle);
	std::string fakeText(stream2.str());
	std::string text2(XorStr("Fake: "));
	text2.append(fakeText);

	std::ostringstream stream3;
	stream3 << Math::RoundFloat(*pLocal->GetLowerBodyYawTarget());
	std::string lbyText(stream3.str());
	std::string text3(XorStr("LBY: "));
	text3.append(lbyText);

	std::ostringstream stream4;
	stream4 << Math::RoundFloat(velocity);
	std::string veloText(stream4.str());
	std::string text4(XorStr("Velocity: "));
	text4.append(veloText);
	// Calculation of text position on screen

	std::ostringstream stream5;
	stream5 << (AntiAim::IsStanding() ? XorStr("Standing") : (AntiAim::IsMoving() ? XorStr("Moving") :
		(AntiAim::IsAirborne() ? XorStr("Airborne") : XorStr("Unknown"))));
	std::string stateText(stream5.str());
	std::string text5(XorStr("Current State: "));
	text5.append(stateText);

	int width;
	int height;
	pEngine->GetScreenSize(width, height);

	int textX = (width - (width - (width * 85 / 100)));
	int textY = height - (height - (height * 53 / 100));
	int textX2 = (width - (width - (width * 85 / 100)));
	int textY2 = height - (height - (height * 51 / 100));
	int textX3 = (width - (width - (width * 85 / 100)));
	int textY3 = height - (height - (height * 55 / 100));
	int textX4 = (width - (width - (width * 85 / 100)));
	int textY4 = height - (height - (height * 57 / 100));
	int textX5 = (width - (width - (width * 85 / 100)));
	int textX6 = (width - (width - (width * 85 / 100)));
	int textY5 = height - (height - (height * 59 / 100));
	int textY6 = height - (height - (height * 61 / 100));


	Draw::Text(textX, textY, text.c_str(), esp_font, Color(255, 255, 255, 255));
	Draw::Text(textX2, textY2, text2.c_str(), esp_font, Color(255, 255, 255, 255));
	Draw::Text(textX3, textY3, text3.c_str(), esp_font, Color(255, 255, 255, 255));
	Draw::Text(textX4, textY4, text4.c_str(), esp_font, Color(255, 255, 255, 255));
	Draw::Text(textX5, textY5, text5.c_str(), esp_font, Color(255, 255, 255, 255));
	if (AntiAim::CanEdge())
	{
		Draw::Text(textX6, textY6, "Edging", esp_font, Color(255, 255, 255, 255));
	}
	else
	{
		//NEIN NEIN NEIN!
	}
}
