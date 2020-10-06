#include <iomanip>
#include "Features.h"
#include "Draw.h"
#include "Util_Items.h"


bool Settings::ESP::enabled = false;
bool Settings::ESP::AntiOBS = false;
ButtonCode_t Settings::ESP::key = ButtonCode_t::KEY_Z;
TeamColorType Settings::ESP::teamColorType = TeamColorType::RELATIVE_;
HealthColorVar Settings::ESP::enemyColor = ImColor(240, 60, 60, 255);
HealthColorVar Settings::ESP::enemyVisibleColor = ImColor(240, 185, 60, 255);
HealthColorVar Settings::ESP::allyColor = ImColor(60, 60, 240, 255);
HealthColorVar Settings::ESP::allyVisibleColor = ImColor(60, 185, 240, 255);
HealthColorVar Settings::ESP::tColor = ImColor(240, 60, 60, 255);
HealthColorVar Settings::ESP::tVisibleColor = ImColor(240, 185, 60, 255);
HealthColorVar Settings::ESP::ctColor = ImColor(60, 60, 240, 255);
HealthColorVar Settings::ESP::ctVisibleColor = ImColor(60, 185, 240, 255);
HealthColorVar Settings::ESP::localplayerColor = ImColor(117, 43, 73, 255);
ColorVar Settings::ESP::bombColor = ImColor(242, 48, 193, 255);
ColorVar Settings::ESP::bombDefusingColor = ImColor(100, 48, 242, 255);
ColorVar Settings::ESP::hostageColor = ImColor(242, 48, 193, 255);
ColorVar Settings::ESP::defuserColor = ImColor(132, 48, 242, 255);
ColorVar Settings::ESP::weaponColor = ImColor(255, 255, 255, 255);
ColorVar Settings::ESP::chickenColor = ImColor(255, 255, 255, 255);
ColorVar Settings::ESP::fishColor = ImColor(255, 255, 255, 255);
ColorVar Settings::ESP::smokeColor = ImColor(32, 224, 22, 255);
ColorVar Settings::ESP::decoyColor = ImColor(32, 224, 22, 255);
ColorVar Settings::ESP::flashbangColor = ImColor(224, 207, 22, 255);
ColorVar Settings::ESP::grenadeColor = ImColor(224, 22, 22, 255);
ColorVar Settings::ESP::molotovColor = ImColor(224, 22, 22, 255);
ColorVar Settings::ESP::Skeleton::color = ImColor(255, 255, 255, 255);
ColorVar Settings::ESP::SkeletonBacktrack::color = ImColor(255, 0, 0, 255);

ColorVar Settings::ESP::Spread::color = ImColor(15, 200, 45, 255);
ColorVar Settings::ESP::Spread::spreadLimitColor = ImColor(20, 5, 150, 255);
bool Settings::ESP::Glow::enabled = false;
HealthColorVar Settings::ESP::Glow::allyColor = ImColor(0, 50, 200, 200);
HealthColorVar Settings::ESP::Glow::enemyColor = ImColor(200, 0, 50, 200);
HealthColorVar Settings::ESP::Glow::enemyVisibleColor = ImColor(200, 200, 50, 200);
HealthColorVar Settings::ESP::Glow::localplayerColor = ImColor(117, 43, 73, 200);
ColorVar Settings::ESP::Glow::weaponColor = ImColor(200, 0, 50, 200);
ColorVar Settings::ESP::Glow::grenadeColor = ImColor(200, 0, 50, 200);
ColorVar Settings::ESP::Glow::defuserColor = ImColor(100, 100, 200, 200);
ColorVar Settings::ESP::Glow::chickenColor = ImColor(100, 200, 100, 200);
bool Settings::ESP::Filters::legit = false;
bool Settings::ESP::Filters::visibilityCheck = false;
bool Settings::ESP::Filters::smokeCheck = false;
bool Settings::ESP::Filters::enemies = false;
bool Settings::ESP::Filters::allies = false;
bool Settings::ESP::Filters::bomb = false;
bool Settings::ESP::Filters::hostages = false;
bool Settings::ESP::Filters::defusers = false;
bool Settings::ESP::Filters::weapons = false;
bool Settings::ESP::Filters::chickens = false;
bool Settings::ESP::Filters::fishes = false;
bool Settings::ESP::Filters::throwables = false;
bool Settings::ESP::Filters::localplayer = false;
bool Settings::ESP::Info::name = false;
bool Settings::ESP::Info::clan = false;
bool Settings::ESP::Info::steamId = false;
bool Settings::ESP::Info::rank = false;
bool Settings::ESP::Info::health = false;
bool Settings::ESP::Info::weapon = false;
bool Settings::ESP::Info::Ammo = false;
bool Settings::ESP::Info::scoped = false;
bool Settings::ESP::Info::reloading = false;
bool Settings::ESP::Info::flashed = false;
bool Settings::ESP::Info::planting = false;
bool Settings::ESP::Info::hasDefuser = false;
bool Settings::ESP::Info::defusing = false;
bool Settings::ESP::Info::distance = false;
bool Settings::ESP::Info::grabbingHostage = false;
bool Settings::ESP::Info::rescuing = false;
bool Settings::ESP::Info::location = false;
bool Settings::ESP::Info::lby = false;
bool Settings::ESP::Info::money = false;
bool Settings::ESP::Info::resolveInfo = false;
bool Settings::ESP::Boxes::enabled = false;
BoxType Settings::ESP::Boxes::type = BoxType::FRAME_2D;
bool Settings::ESP::Bars::enabled = false;
BarColorType Settings::ESP::Bars::colorType = BarColorType::HEALTH_BASED;
BarType Settings::ESP::Bars::type = BarType::HORIZONTAL;
bool Settings::ESP::Tracers::enabled = false;
TracerType Settings::ESP::Tracers::type = TracerType::BOTTOM;
bool Settings::ESP::BulletTracers::enabled = false;
bool Settings::ESP::FOVCrosshair::enabled = false;
bool Settings::ESP::FOVCrosshair::filled = false;
ColorVar Settings::ESP::FOVCrosshair::color = ImColor(180, 50, 50, 255);
bool Settings::ESP::PlayerIndicator::enabled = false;
bool Settings::ESP::Skeleton::enabled = false;
bool Settings::ESP::SkeletonBacktrack::enabled = false;
bool Settings::ESP::Sounds::enabled = false;
int Settings::ESP::Sounds::time = 1000;
float Settings::ESP::Sounds::MinimumVolume = 0;
bool Settings::NoScopeBorder::enabled = false;
bool Settings::ESP::HeadDot::enabled = false;
float Settings::ESP::HeadDot::size = 2.f;

bool Settings::ESP::AutoWall::debugView = false;
bool Settings::ESP::AutoAim::drawTarget = false;
Vector Settings::ESP::AutoAim::target = { 0, 0, 0 };
bool Settings::ESP::BoneMap::draw = false;
bool Settings::ESP::BoneMap::justDrawDots = false;

bool Settings::ESP::Spread::enabled = false;
bool Settings::ESP::Spread::spreadLimit = false;

//Test variable to test stuff out.....
bool Settings::ESP::Test::testfunction1 = false;
bool Settings::ESP::Test::testfunction2 = false;
bool Settings::ESP::Test::testfunction3 = false;


bool Settings::Debug::AnimLayers::draw = false;

inline void CrossProduct(const Vector& a, const Vector& b, Vector& result)
{
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
}

__forceinline float VecDotProduct(const Vector& a, const Vector& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}


class Footstep
{
public:
	long expiration;
	int entityId;
	Vector position;
};

Vector viewanglesBackup;
std::vector<Footstep> footsteps;

const char* ESP::ranks[] = {
	"Unranked",
	"Silver I",
	"Silver II",
	"Silver III",
	"Silver IV",
	"Silver Elite",
	"Silver Elite Master",

	"Gold Nova I",
	"Gold Nova II",
	"Gold Nova III",
	"Gold Nova Master",
	"Master Guardian I",
	"Master Guardian II",

	"Master Guardian Elite",
	"Distinguished Master Guardian",
	"Legendary Eagle",
	"Legendary Eagle Master",
	"Supreme Master First Class",
	"The Global Elite"
};

static float GetArmourHealth(float flDamage, int ArmorValue) {
	float flCurDamage = flDamage;

	if (flCurDamage == 0.0f || ArmorValue == 0)
		return flCurDamage;

	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	float flNew = flCurDamage * flArmorRatio;
	float flArmor = (flCurDamage - flNew) * flArmorBonus;

	if (flArmor > ArmorValue) {
		flArmor = ArmorValue * (1.0f / flArmorBonus);
		flNew = flCurDamage - flArmor;
	}

	return flNew;
}

static bool GetLocation(Vector pos, int& x, int& y, int& w, int& h)
{
	// Variables
	Vector vOrigin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = pos;
	min = pos - Vector(1, 1, 1);
	max = pos + Vector(1, 1, 1);

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (pDebugOverlay->ScreenPosition(points[3], flb) || pDebugOverlay->ScreenPosition(points[5], brt)
		|| pDebugOverlay->ScreenPosition(points[0], blb) || pDebugOverlay->ScreenPosition(points[4], frt)
		|| pDebugOverlay->ScreenPosition(points[2], frb) || pDebugOverlay->ScreenPosition(points[1], brb)
		|| pDebugOverlay->ScreenPosition(points[6], blt) || pDebugOverlay->ScreenPosition(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	x = (int)left;
	y = (int)top;
	w = (int)(right - left);
	h = (int)(bottom - top);

	return true;
}

static bool GetBox(C_BaseEntity* entity, int& x, int& y, int& w, int& h)
{
	// Variables
	Vector vOrigin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = entity->GetVecOrigin();
	min = entity->GetCollideable()->OBBMins() + vOrigin;
	max = entity->GetCollideable()->OBBMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (pDebugOverlay->ScreenPosition(points[3], flb) || pDebugOverlay->ScreenPosition(points[5], brt)
		|| pDebugOverlay->ScreenPosition(points[0], blb) || pDebugOverlay->ScreenPosition(points[4], frt)
		|| pDebugOverlay->ScreenPosition(points[2], frb) || pDebugOverlay->ScreenPosition(points[1], brb)
		|| pDebugOverlay->ScreenPosition(points[6], blt) || pDebugOverlay->ScreenPosition(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	x = (int)left;
	y = (int)top;
	w = (int)(right - left);
	h = (int)(bottom - top);

	return true;
}


ImColor ESP::GetESPPlayerColor(C_BasePlayer* player, bool visible)
{
	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer)
		return ImColor(255, 255, 255, 255);

	ImColor playerColor;

	if (player == localplayer)
	{
		playerColor = Settings::ESP::localplayerColor.Color(player);
	}
	else
	{
		if (Settings::ESP::teamColorType == TeamColorType::RELATIVE_)
		{
			if (player->GetTeam() != localplayer->GetTeam())
			{
				if (visible)
					playerColor = Settings::ESP::enemyVisibleColor.Color(player);
				else
					playerColor = Settings::ESP::enemyColor.Color(player);
			}
			else
			{
				if (visible)
					playerColor = Settings::ESP::allyVisibleColor.Color(player);
				else
					playerColor = Settings::ESP::allyColor.Color(player);
			}
		}
		else if (Settings::ESP::teamColorType == TeamColorType::ABSOLUTE_)
		{
			if (player->GetTeam() == TeamID::TEAM_TERRORIST)
			{
				if (visible)
					playerColor = Settings::ESP::tVisibleColor.Color(player);
				else
					playerColor = Settings::ESP::tColor.Color(player);
			}
			else if (player->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST)
			{
				if (visible)
					playerColor = Settings::ESP::ctVisibleColor.Color(player);
				else
					playerColor = Settings::ESP::ctColor.Color(player);
			}
		}
	}

	if (player->GetImmune())
	{
		playerColor.Value.x *= 0.45f;
		playerColor.Value.y *= 0.45f;
		playerColor.Value.z *= 0.45f;
	}

	return playerColor;
}

static void CalcAngle(Vector src, Vector dst, Vector& angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D();
	angles.y = atan(delta.y / delta.x) * 57.295779513082f;
	angles.x = atan(delta.z / hyp) * 57.295779513082f;
	if (delta.x >= 0.0)
		angles.y += 180.0f;
	angles.z = 0;
}

//pDebugOverlay->ScreenPosition
static void Draw3DBox(Vector* boxVectors, Color color) {
	Vector boxVectors0, boxVectors1, boxVectors2, boxVectors3,
		boxVectors4, boxVectors5, boxVectors6, boxVectors7;


	if (Draw::WorldToScreen(boxVectors[0], boxVectors0) &&
		Draw::WorldToScreen(boxVectors[1], boxVectors1) &&
		Draw::WorldToScreen(boxVectors[2], boxVectors2) &&
		Draw::WorldToScreen(boxVectors[3], boxVectors3) &&
		Draw::WorldToScreen(boxVectors[4], boxVectors4) &&
		Draw::WorldToScreen(boxVectors[5], boxVectors5) &&
		Draw::WorldToScreen(boxVectors[6], boxVectors6) &&
		Draw::WorldToScreen(boxVectors[7], boxVectors7)) {

		/*
		.+--5---+
		.8 4    6'|
		+--7---+'  11
		9   |  10  |
		|  ,+--|3--+
		|.0    | 2'
		+--1---+'
		*/

		Vector2D lines[12][2];
		//bottom of box
		lines[0][0] = { boxVectors0.x, boxVectors0.y };
		lines[0][1] = { boxVectors1.x, boxVectors1.y };
		lines[1][0] = { boxVectors1.x, boxVectors1.y };
		lines[1][1] = { boxVectors2.x, boxVectors2.y };
		lines[2][0] = { boxVectors2.x, boxVectors2.y };
		lines[2][1] = { boxVectors3.x, boxVectors3.y };
		lines[3][0] = { boxVectors3.x, boxVectors3.y };
		lines[3][1] = { boxVectors0.x, boxVectors0.y };

		lines[4][0] = { boxVectors0.x, boxVectors0.y };
		lines[4][1] = { boxVectors6.x, boxVectors6.y };

		// top of box
		lines[5][0] = { boxVectors6.x, boxVectors6.y };
		lines[5][1] = { boxVectors5.x, boxVectors5.y };
		lines[6][0] = { boxVectors5.x, boxVectors5.y };
		lines[6][1] = { boxVectors4.x, boxVectors4.y };
		lines[7][0] = { boxVectors4.x, boxVectors4.y };
		lines[7][1] = { boxVectors7.x, boxVectors7.y };
		lines[8][0] = { boxVectors7.x, boxVectors7.y };
		lines[8][1] = { boxVectors6.x, boxVectors6.y };


		lines[9][0] = { boxVectors5.x, boxVectors5.y };
		lines[9][1] = { boxVectors1.x, boxVectors1.y };

		lines[10][0] = { boxVectors4.x, boxVectors4.y };
		lines[10][1] = { boxVectors2.x, boxVectors2.y };

		lines[11][0] = { boxVectors7.x, boxVectors7.y };
		lines[11][1] = { boxVectors3.x, boxVectors3.y };

		for (int i = 0; i < 12; i++) {

			Draw::Line(static_cast<int>(lines[i][0].x), static_cast<int>(lines[i][0].y), static_cast<int>(lines[i][1].x), static_cast<int>(lines[i][1].y), color);
		}
	}
}

static void DrawHitbox(matrix3x4a_t* matrix, Vector bbmin, Vector bbmax, int bone, Color color) {
	Vector points[] = {
		Vector(bbmin.x, bbmin.y, bbmin.z),
		Vector(bbmin.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmin.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmin.y, bbmax.z),
		Vector(bbmax.x, bbmin.y, bbmax.z)
	};

	Vector pointsTransformed[8];

	for (int index = 0; index < 8; ++index) {
		if (index != 0)
			points[index] = ((((points[index] + points[0]) * .5f) + points[index]) * .5f);

		Math::VectorTransform(points[index], matrix[bone], pointsTransformed[index]);
	}

	Draw3DBox(pointsTransformed, color);
}


static void DrawBox(Color color, int x, int y, int w, int h, C_BaseEntity* entity) {

	if (Settings::ESP::Boxes::type == BoxType::FRAME_2D)
	{
		int VertLine = (int)(w * 0.33f);
		int HorzLine = (int)(h * 0.33f);
		int squareLine = min(VertLine, HorzLine);

		// top-left corner / color
		Draw::Rectangle(x, y, x + squareLine, y + 1, color);
		Draw::Rectangle(x, y, x + 1, y + squareLine, color);

		// top-left corner / outer outline
		Draw::Rectangle(x - 1, y - 1, x + squareLine, y, Color(10, 10, 10, 190));
		Draw::Rectangle(x - 1, y, x, y + squareLine, Color(10, 10, 10, 190));

		// top-left corner / inner outline
		Draw::Rectangle(x + 1, y + 1, x + squareLine, y + 2, Color(10, 10, 10, 190));
		Draw::Rectangle(x + 1, y + 2, x + 2, y + squareLine, Color(10, 10, 10, 190));

		// top-left corner / missing edges
		Draw::Rectangle(x + squareLine, y - 1, x + squareLine + 1, y + 2, Color(10, 10, 10, 190));
		Draw::Rectangle(x - 1, y + squareLine, x + 2, y + squareLine + 1, Color(10, 10, 10, 190));


		// top-right corner / color
		Draw::Rectangle(x + w - squareLine, y, x + w, y + 1, color);
		Draw::Rectangle(x + w - 1, y, x + w, y + squareLine, color);

		// top-right corner / outer outline
		Draw::Rectangle(x + w - squareLine, y - 1, x + w + 1, y, Color(10, 10, 10, 190));
		Draw::Rectangle(x + w, y, x + w + 1, y + squareLine, Color(10, 10, 10, 190));

		// top-right corner / inner outline
		Draw::Rectangle(x + w - squareLine, y + 1, x + w - 1, y + 2, Color(10, 10, 10, 190));
		Draw::Rectangle(x + w - 2, y + 2, x + w - 1, y + squareLine, Color(10, 10, 10, 190));

		// top-right corner / missing edges
		Draw::Rectangle(x + w - squareLine - 1, y - 1, x + w - squareLine, y + 2, Color(10, 10, 10, 190));
		Draw::Rectangle(x + w - 2, y + squareLine, x + w + 1, y + squareLine + 1, Color(10, 10, 10, 190));


		// bottom-left corner / color
		Draw::Rectangle(x, y + h - 1, x + squareLine, y + h, color);
		Draw::Rectangle(x, y + h - squareLine, x + 1, y + h, color);

		// bottom-left corner / outer outline
		Draw::Rectangle(x - 1, y + h, x + squareLine, y + h + 1, Color(10, 10, 10, 190));
		Draw::Rectangle(x - 1, y + h - squareLine, x, y + h, Color(10, 10, 10, 190));

		// bottom-left corner / inner outline
		Draw::Rectangle(x + 1, y + h - 2, x + squareLine, y + h - 1, Color(10, 10, 10, 190));
		Draw::Rectangle(x + 1, y + h - squareLine, x + 2, y + h - 2, Color(10, 10, 10, 190));

		// bottom-left corner / missing edges
		Draw::Rectangle(x + squareLine, y + h - 2, x + squareLine + 1, y + h + 1, Color(10, 10, 10, 190));
		Draw::Rectangle(x - 1, y + h - squareLine - 1, x + 2, y + h - squareLine, Color(10, 10, 10, 190));


		// bottom-right corner / color
		Draw::Rectangle(x + w - squareLine, y + h - 1, x + w, y + h, color);
		Draw::Rectangle(x + w - 1, y + h - squareLine, x + w, y + h, color);

		// bottom-right corner / outer outline
		Draw::Rectangle(x + w - squareLine, y + h, x + w + 1, y + h + 1, Color(10, 10, 10, 190));
		Draw::Rectangle(x + w, y + h - squareLine, x + w + 1, y + h + 1, Color(10, 10, 10, 190));

		// bottom-right corner / inner outline
		Draw::Rectangle(x + w - squareLine, y + h - 2, x + w - 1, y + h - 1, Color(10, 10, 10, 190));
		Draw::Rectangle(x + w - 2, y + h - squareLine, x + w - 1, y + h - 2, Color(10, 10, 10, 190));

		// bottom-right corner / missing edges
		Draw::Rectangle(x + w - squareLine, y + h - 2, x + w - squareLine + 1, y + h + 1, Color(10, 10, 10, 190));
		Draw::Rectangle(x + w - 2, y + h - squareLine - 1, x + w + 1, y + h - squareLine, Color(10, 10, 10, 190));
	}
	else if (Settings::ESP::Boxes::type == BoxType::FLAT_2D)
	{
		int VertLine = (int)(w * 0.33f);
		int HorzLine = (int)(h * 0.33f);
		int squareLine = min(VertLine, HorzLine);

		// top-left corner / color
		Draw::Rectangle(x, y, x + squareLine, y + 1, color);
		Draw::Rectangle(x, y, x + 1, y + squareLine, color);

		// top-left corner / outer outline
		//Draw::Rectangle(x - 1, y - 1, x + squareLine, y, Color(10, 10, 10, 190));
		//Draw::Rectangle(x - 1, y, x, y + squareLine, Color(10, 10, 10, 190));

		// top-left corner / inner outline
		//Draw::Rectangle(x + 1, y + 1, x + squareLine, y + 2, Color(10, 10, 10, 190));
		//Draw::Rectangle(x + 1, y + 2, x + 2, y + squareLine, Color(10, 10, 10, 190));

		// top-left corner / missing edges
		Draw::Rectangle(x + squareLine, y - 1, x + squareLine + 1, y + 2, Color(10, 10, 10, 190));
		Draw::Rectangle(x - 1, y + squareLine, x + 2, y + squareLine + 1, Color(10, 10, 10, 190));


		// top-right corner / color
		Draw::Rectangle(x + w - squareLine, y, x + w, y + 1, color);
		Draw::Rectangle(x + w - 1, y, x + w, y + squareLine, color);

		// top-right corner / outer outline
		//Draw::Rectangle(x + w - squareLine, y - 1, x + w + 1, y, Color(10, 10, 10, 190));
		//Draw::Rectangle(x + w, y, x + w + 1, y + squareLine, Color(10, 10, 10, 190));

		// top-right corner / inner outline
		//Draw::Rectangle(x + w - squareLine, y + 1, x + w - 1, y + 2, Color(10, 10, 10, 190));
		//Draw::Rectangle(x + w - 2, y + 2, x + w - 1, y + squareLine, Color(10, 10, 10, 190));

		// top-right corner / missing edges
		Draw::Rectangle(x + w - squareLine - 1, y - 1, x + w - squareLine, y + 2, Color(10, 10, 10, 190));
		Draw::Rectangle(x + w - 2, y + squareLine, x + w + 1, y + squareLine + 1, Color(10, 10, 10, 190));


		// bottom-left corner / color
		Draw::Rectangle(x, y + h - 1, x + squareLine, y + h, color);
		Draw::Rectangle(x, y + h - squareLine, x + 1, y + h, color);

		// bottom-left corner / outer outline
		//Draw::Rectangle(x - 1, y + h, x + squareLine, y + h + 1, Color(10, 10, 10, 190));
		//Draw::Rectangle(x - 1, y + h - squareLine, x, y + h, Color(10, 10, 10, 190));

		// bottom-left corner / inner outline
		//Draw::Rectangle(x + 1, y + h - 2, x + squareLine, y + h - 1, Color(10, 10, 10, 190));
		//Draw::Rectangle(x + 1, y + h - squareLine, x + 2, y + h - 2, Color(10, 10, 10, 190));

		// bottom-left corner / missing edges
		Draw::Rectangle(x + squareLine, y + h - 2, x + squareLine + 1, y + h + 1, Color(10, 10, 10, 190));
		Draw::Rectangle(x - 1, y + h - squareLine - 1, x + 2, y + h - squareLine, Color(10, 10, 10, 190));


		// bottom-right corner / color
		Draw::Rectangle(x + w - squareLine, y + h - 1, x + w, y + h, color);
		Draw::Rectangle(x + w - 1, y + h - squareLine, x + w, y + h, color);

		// bottom-right corner / outer outline
		//Draw::Rectangle(x + w - squareLine, y + h, x + w + 1, y + h + 1, Color(10, 10, 10, 190));
		//Draw::Rectangle(x + w, y + h - squareLine, x + w + 1, y + h + 1, Color(10, 10, 10, 190));

		// bottom-right corner / inner outline
		//Draw::Rectangle(x + w - squareLine, y + h - 2, x + w - 1, y + h - 1, Color(10, 10, 10, 190));
		//Draw::Rectangle(x + w - 2, y + h - squareLine, x + w - 1, y + h - 2, Color(10, 10, 10, 190));

		// bottom-right corner / missing edges
		Draw::Rectangle(x + w - squareLine, y + h - 2, x + w - squareLine + 1, y + h + 1, Color(10, 10, 10, 190));
		Draw::Rectangle(x + w - 2, y + h - squareLine - 1, x + w + 1, y + h - squareLine, Color(10, 10, 10, 190));

		Draw::FilledRectangle(x, y, x + w, y + h, Color(color.r, color.g, color.b, 21));

	}
	else if (Settings::ESP::Boxes::type == BoxType::BOX_3D)
	{
		Vector vOrigin = entity->GetVecOrigin();
		Vector min = entity->GetCollideable()->OBBMins() + vOrigin;
		Vector max = entity->GetCollideable()->OBBMaxs() + vOrigin;

		Vector points[] = { Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(min.x, min.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(max.x, max.y, max.z),
			Vector(max.x, min.y, max.z) };

		int edges[12][2] = { { 0, 1 },{ 1, 2 },{ 2, 3 },{ 3, 0 },
		{ 4, 5 },{ 5, 6 },{ 6, 7 },{ 7, 4 },
		{ 0, 4 },{ 1, 5 },{ 2, 6 },{ 3, 7 }, };

		for (auto it : edges)
		{
			Vector p1, p2;
			if (pDebugOverlay->ScreenPosition(points[it[0]], p1) || pDebugOverlay->ScreenPosition(points[it[1]], p2))
				return;
			Draw::Line(Vector2D(p1.x, p1.y), Vector2D(p2.x, p2.y), color);
		}
	}
	else if (Settings::ESP::Boxes::type == BoxType::HITBOX)
	{
		matrix3x4a_t matrix[MAXSTUDIOBONES];

		if (!entity->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, pGlobalVars->curtime))
			return;

		studiohdr_t* pStudioModel = pModelInfo->GetStudioModel(entity->GetModel());
		if (!pStudioModel)
			return;

		mstudiohitboxset_t* set = pStudioModel->pHitboxSet(entity->GetHitboxSet());
		if (!set)
			return;

		for (int i = 0; i < set->numhitboxes; i++)
		{
			mstudiobbox_t* hitbox = set->pHitbox(i);
			if (!hitbox)
				return;

			Vector vMaxUntransformed = hitbox->bbmax;
			Vector vMinUntransformed = hitbox->bbmin;

			if (hitbox->radius != -1.f)
			{
				vMinUntransformed -= Vector(hitbox->radius, hitbox->radius, hitbox->radius);
				vMaxUntransformed += Vector(hitbox->radius, hitbox->radius, hitbox->radius);
			}

			//Drawpill crashes w2s works fine.
			DrawHitbox(matrix, vMinUntransformed, vMaxUntransformed, hitbox->bone, color);

		}
	}
}


static void DrawEntity(C_BaseEntity* entity, const char* string, Color color)
{
	int x, y, w, h;
	if (GetBox(entity, x, y, w, h))
	{
		DrawBox(color, x, y, w, h, entity);

		Vector2D nameSize = Draw::GetTextSize(string, esp_font);
		Draw::Text((int)(x + (w / 2) - (nameSize.x / 2)), y + h + 2, string, esp_font, Color(255, 255, 255, 255));
	}
}



static void DrawBomb(C_BaseCombatWeapon* bomb)
{
	if (!(*pGameRules) || !(*pGameRules)->IsBombDropped())
		return;

	DrawEntity(bomb, "Bomb", Color::FromImColor(Settings::ESP::bombColor.Color()));
}

static void DrawPlantedBomb(C_PlantedC4* bomb)
{
	if (!(*pGameRules) || !(*pGameRules)->IsBombPlanted())
		return;

	ImColor color = bomb->GetBombDefuser() != -1 || bomb->IsBombDefused() ? Settings::ESP::bombDefusingColor.Color() : Settings::ESP::bombColor.Color();

	float bombTimer = bomb->GetBombTime() - pGlobalVars->curtime;
	std::stringstream displayText;
	if (bomb->IsBombDefused() || !bomb->IsBombTicking() || bombTimer <= 0.f)
	{
		displayText << "Bomb";
	}
	else
	{
		C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
		Vector vecOrigin = bomb->GetVecOrigin();

		float flDistance = localplayer->GetEyePosition().DistTo(vecOrigin);

		float a = 450.7f;
		float b = 75.68f;
		float c = 789.2f;
		float d = ((flDistance - b) / c);
		float flDamage = a * expf(-d * d);

		float damage = max((int)ceilf(GetArmourHealth(flDamage, localplayer->GetArmor())), 0);

		displayText << "Bomb: " << std::fixed << std::showpoint << std::setprecision(1) << bombTimer << ", damage: " << (int)damage;
	}

	DrawEntity(bomb, displayText.str().c_str(), Color::FromImColor(color));
}

static void DrawDefuseKit(C_BaseEntity* defuser)
{
	DrawEntity(defuser, "Defuser", Color::FromImColor(Settings::ESP::defuserColor.Color()));
}

static void DrawDroppedWeapons(C_BaseCombatWeapon* weapon)
{
	Vector vOrig = weapon->GetVecOrigin();
	int owner = weapon->GetOwner();

	if (owner > -1 || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
		return;

	std::string modelName = Util::Items::GetItemDisplayName(*weapon->GetItemDefinitionIndex());

	if (weapon->GetAmmo() > 0)
	{
		modelName += " | ";
		modelName += std::to_string(weapon->GetAmmo());

	}

	DrawEntity(weapon, modelName.c_str(), Color::FromImColor(Settings::ESP::weaponColor.Color()));
}

static void DrawHostage(C_BaseEntity* hostage)
{
	DrawEntity(hostage, "Hostage", Color::FromImColor(Settings::ESP::hostageColor.Color()));
}

static void DrawChicken(C_BaseEntity* chicken)
{
	DrawEntity(chicken, "Chicken", Color::FromImColor(Settings::ESP::chickenColor.Color()));
}

static void DrawFish(C_BaseEntity* fish)
{
	DrawEntity(fish, "Fish", Color::FromImColor(Settings::ESP::fishColor.Color()));
}

static void DrawThrowable(C_BaseEntity* throwable, ClientClass* client)
{
	model_t* nadeModel = throwable->GetModel();

	if (!nadeModel)
		return;

	studiohdr_t* hdr = pModelInfo->GetStudioModel(nadeModel);

	if (!hdr)
		return;

	if (!strstr(hdr->name, XorStr("thrown")) && !strstr(hdr->name, XorStr("dropped")))
		return;

	ImColor nadeColor = ImColor(255, 255, 255, 255);
	std::string nadeName = XorStr("Unknown Grenade");

	IMaterial* mats[32];
	pModelInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

	for (int i = 0; i < hdr->numtextures; i++) {
		IMaterial* mat = mats[i];
		if (!mat)
			continue;


		if (strstr(mat->GetName(), XorStr("flashbang"))) {
			nadeName = XorStr("Flashbang");
			nadeColor = Settings::ESP::flashbangColor.Color();
			break;
		}
		else if (strstr(mat->GetName(), XorStr("m67_grenade")) ||
			strstr(mat->GetName(), XorStr("hegrenade"))) {
			nadeName = XorStr("HE Grenade");
			nadeColor = Settings::ESP::grenadeColor.Color();
			break;
		}
		else if (strstr(mat->GetName(), XorStr("smoke"))) {
			nadeName = XorStr("Smoke");
			nadeColor = Settings::ESP::smokeColor.Color();
			break;
		}
		else if (strstr(mat->GetName(), XorStr("decoy"))) {
			nadeName = XorStr("Decoy");
			nadeColor = Settings::ESP::decoyColor.Color();
			break;
		}
		else if (strstr(mat->GetName(), XorStr("incendiary")) ||
			strstr(mat->GetName(), XorStr("molotov"))) {
			nadeName = XorStr("Molotov");
			nadeColor = Settings::ESP::molotovColor.Color();
			break;
		}
	}

	DrawEntity(throwable, nadeName.c_str(), Color::FromImColor(nadeColor));
}

static void DrawDirection(const Vector& origin) //monarch is the NIGGA
{
	/*constexpr float radius = 360.0f;
	int width, height;
	pEngine->GetScreenSize(width, height);

	Vector vRealAngles;
	pEngine->GetViewAngles(vRealAngles);

	Vector vForward, vRight, vUp(0.0f, 0.0f, 1.0f);

	Math::AngleVectors(vRealAngles, &vForward, NULL, NULL);

	vForward.z = 0.0f;
	Math::NormalizeAngles(vForward);
	CrossProduct(vUp, vForward, vRight);



	float flFront = VecDotProduct(origin, vForward);
	float flSide = VecDotProduct(origin, vRight);
	float flXPosition = radius * -flSide;
	float flYPosition = radius * -flFront;

	float rotation = G::UserCmd->viewangles.y + 180;

	rotation = atan2(flXPosition, flYPosition) + M_PI;
	rotation *= 180.0f / M_PI;

	float flYawRadians = -(rotation)* M_PI / 180.0f;
	float flCosYaw = cos(flYawRadians);
	float flSinYaw = sin(flYawRadians);

	flXPosition = (int)((width / 2.0f) + (radius * flSinYaw));
	flYPosition = (int)((height / 2.0f) - (radius * flCosYaw));
*/

//	int width, height;
//	pEngine->GetScreenSize(width, height);
//
//	float flRadius = 360.0f;
//	Vector vRealAngles;
//	pEngine->GetViewAngles(vRealAngles);
//
//	Vector vForward, vRight, vUp(0.0f, 0.0f, 1.0f);
//
//	Math::AngleVectors(vRealAngles, vForward);
//
//	vForward.z = 0.0f;
//	Math::NormalizeVector(vForward);
//	vRight = CrossProduct(vUp, vForward);
//
//	float flFront = VecDotProduct(origin, vForward);
//	float flSide = VecDotProduct(origin, vRight);
//	float flXPosition = flRadius * -flSide;
//	float flYPosition = flRadius * -flFront;
//
//	float flRotation = G::UserCmd->viewangles.y + 180;
//
////	flRotation = atan2(flXPosition, flYPosition) + PI;
////	flRotation *= 180.f / PI;
//
////	float flYawRadians = -(flRotation)* PI / 180.f;
//	float flCosYaw = cos(flYawRadians);
//	float flSinYaw = sin(flYawRadians);
//
//	flXPosition = (int)((width / 2.0f) + (flRadius * flSinYaw));
//	flYPosition = (int)((height / 2.0f) - (flRadius * flCosYaw));
//
//
//	Draw::OutlinedCircle(flXPosition, flYPosition, 5, 25, Color(255, 0, 0, 255));
//	

	//RENDER::DrawFilledCircle(flXPosition, flYPosition, 10, 50, CColor(255, 0, 255, 120));
}

static void DrawSkeleton(C_BasePlayer* player)
{
	studiohdr_t* pStudioModel = pModelInfo->GetStudioModel(player->GetModel());
	if (!pStudioModel)
		return;

	static matrix3x4_t pBoneToWorldOut[128];
	if (player->SetupBones(pBoneToWorldOut, 128, 256, 0))
	{
		for (int i = 0; i < pStudioModel->numbones; i++)
		{
			mstudiobone_t* pBone = pStudioModel->pBone(i);
			if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
				continue;

			Vector vBonePos1;
			if (pDebugOverlay->ScreenPosition(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
				continue;

			Vector vBonePos2;
			if (pDebugOverlay->ScreenPosition(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
				continue;

			Draw::Line(Vector2D(vBonePos1.x, vBonePos1.y), Vector2D(vBonePos2.x, vBonePos2.y), Color::FromImColor(Settings::ESP::Skeleton::color.Color()));
		}
	}
}


static void DrawBulletTrace(C_BasePlayer* player)
{
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	Math::AngleVectors(*player->GetEyeAngles(), forward);
	filter.pSkip = player;
	src3D = player->GetEyePosition();
	dst3D = src3D + (forward * 8192);

	ray.Init(src3D, dst3D);

	pTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (pDebugOverlay->ScreenPosition(src3D, src) || pDebugOverlay->ScreenPosition(tr.endpos, dst))
		return;

	Draw::Line((int)(src.x), (int)(src.y), (int)(dst.x), (int)(dst.y), Color::FromImColor(ESP::GetESPPlayerColor(player, true)));
	Draw::FilledRectangle((int)(dst.x - 3), (int)(dst.y - 3), 6, 6, Color::FromImColor(ESP::GetESPPlayerColor(player, false)));
}



static void DrawBacktrackIndicator(C_BasePlayer* player) {
	// Like footprints but for the head xDDDDD D DD D D D DD D D
//	Vector headprints2D[Backtracking::MAX_QUEUE_SIZE + 1];
	Vector headprints2D[Backtracking::MAX_QUEUE_SIZE + 1];
	for (unsigned int i = 1; i < Backtracking::lagRecords[player->GetIndex()].size(); i++) {
		pDebugOverlay->ScreenPosition(Vector(Backtracking::lagRecords[player->GetIndex()][i].headPos.x,
			Backtracking::lagRecords[player->GetIndex()][i].headPos.y,
			Backtracking::lagRecords[player->GetIndex()][i].headPos.z),
			headprints2D[i]);


		studiohdr_t* pStudioModel = pModelInfo->GetStudioModel(player->GetModel());
		if (!pStudioModel)
			return;

		//static matrix3x4_t pBoneToWorldOut[128];
		if (player->SetupBones(Backtracking::lagRecords[player->GetIndex()][i].pBoneToWorldOut, 128, 256, 0))
		{
			for (int i = 0; i < pStudioModel->numbones; i++)
			{
				mstudiobone_t* pBone = pStudioModel->pBone(i);
				if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
					continue;

				Vector vBonePos1;
				if (pDebugOverlay->ScreenPosition(Vector(Backtracking::lagRecords[player->GetIndex()][i].pBoneToWorldOut[i][0][3], Backtracking::lagRecords[player->GetIndex()][i].pBoneToWorldOut[i][1][3], Backtracking::lagRecords[player->GetIndex()][i].pBoneToWorldOut[i][2][3]), vBonePos1))
					continue;

				Vector vBonePos2;
				if (pDebugOverlay->ScreenPosition(Vector(Backtracking::lagRecords[player->GetIndex()][i].pBoneToWorldOut[pBone->parent][0][3], Backtracking::lagRecords[player->GetIndex()][i].pBoneToWorldOut[pBone->parent][1][3], Backtracking::lagRecords[player->GetIndex()][i].pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
					continue;

				Draw::Line(Vector2D(vBonePos1.x, vBonePos1.y), Vector2D(vBonePos2.x, vBonePos2.y), Color::FromImColor(Settings::ESP::Skeleton::color.Color()));

			}
		}
	}
}


static void DrawTracer(C_BasePlayer* player)
{
	Vector src3D, src;
	src3D = player->GetVecOrigin() - Vector(0, 0, 0);

	if (pDebugOverlay->ScreenPosition(src3D, src))
		return;

	int ScreenWidth, ScreenHeight;
	pEngine->GetScreenSize(ScreenWidth, ScreenHeight);

	int x = (int)(ScreenWidth * 0.5f);
	int y = 0;

	if (Settings::ESP::Tracers::type == TracerType::CURSOR)
		y = (int)(ScreenHeight * 0.5f);
	else if (Settings::ESP::Tracers::type == TracerType::BOTTOM)
		y = ScreenHeight;

	bool bIsVisible = Entity::IsVisible(player, Bone::BONE_HEAD, 180.f, Settings::ESP::Filters::smokeCheck);
	Draw::Line((int)(src.x), (int)(src.y), x, y, Color::FromImColor(ESP::GetESPPlayerColor(player, bIsVisible)));
}

static void DrawHeaddot(C_BasePlayer* player)
{
	Vector head2D;
	Vector head3D = player->GetBonePosition((int)Bone::BONE_HEAD);
	if (pDebugOverlay->ScreenPosition(Vector(head3D.x, head3D.y, head3D.z), head2D))
		return;

	bool bIsVisible = false;
	if (Settings::ESP::Filters::visibilityCheck || Settings::ESP::Filters::legit)
		bIsVisible = Entity::IsVisible(player, Bone::BONE_HEAD, 180.f, Settings::ESP::Filters::smokeCheck);

	Draw::FilledCircle(Vector2D(head2D.x, head2D.y), 10, Settings::ESP::HeadDot::size, Color::FromImColor(ESP::GetESPPlayerColor(player, bIsVisible)));
}

static void CollectFootstep(int iEntIndex, const char* pSample) {
	if (strstr(pSample, XorStr("player/footsteps")) == NULL && strstr(pSample, XorStr("player/land")) == NULL)
		return;

	if (iEntIndex == pEngine->GetLocalPlayer())
		return;

	Footstep footstep;
//	footstep.entityId = iEntIndex;
//	footstep.position = pEntityList->GetClientEntity(iEntIndex)->GetVecOrigin();
//	footstep.expiration = Util::GetEpochTime() + Settings::ESP::Sounds::time;

	footsteps.push_back(footstep);
}


static void DrawSounds()
{
	for (unsigned int i = 0; i < footsteps.size(); i++) {
		long diff = footsteps[i].expiration - Util::GetEpochTime();

		if (diff <= 0) {
			footsteps.erase(footsteps.begin() + i);
			continue;
		}

		Vector pos2d;

		if (pDebugOverlay->ScreenPosition(footsteps[i].position, pos2d))
			continue;

		C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
		if (!localplayer)
			continue;

		C_BasePlayer* player = (C_BasePlayer*)pEntityList->GetClientEntity(footsteps[i].entityId);
		if (!player)
			continue;

		if (player->GetTeam() != localplayer->GetTeam() && !Settings::ESP::Filters::enemies)
			continue;

		if (player->GetTeam() == localplayer->GetTeam() && !Settings::ESP::Filters::allies)
			continue;

		bool bIsVisible = false;
		if (Settings::ESP::Filters::visibilityCheck || Settings::ESP::Filters::legit)
			bIsVisible = Entity::IsVisible(player, Bone::BONE_HEAD, 180.f,
				Settings::ESP::Filters::smokeCheck);

		float percent = (float)diff / (float)Settings::ESP::Sounds::time;

		Color playerColor = Color::FromImColor(ESP::GetESPPlayerColor(player, bIsVisible));
		playerColor.a =
			min(powf(percent * 2, 0.6f), 1.f) * playerColor.a; // fades out alpha when its below 0.5

		float circleRadius = fabs(percent - 1.f) * 42.f;
		float points = circleRadius * 0.75f;

		Draw::Circle3D(footsteps[i].position, points, circleRadius, playerColor);
	}
}

static void DrawAimbotSpot() {
	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer || !localplayer->GetAlive()) {
		Settings::ESP::AutoAim::target = { 0,0,0 };
		return;
	}
	if (!Settings::Aimbot::AutoAim::enabled || !Settings::Aimbot::enabled) {
		Settings::ESP::AutoAim::target = { 0,0,0 };
		return;
	}
	if (Settings::ESP::AutoAim::target.IsZero())
		return;
	Vector spot2D;
	if (pDebugOverlay->ScreenPosition(Settings::ESP::AutoAim::target, spot2D))
		return;
	int width, height;
	pEngine->GetScreenSize(width, height);
	Draw::Line(Vector2D(width / 2, height / 2), Vector2D(spot2D.x, spot2D.y), Color(45, 235, 60));
	Draw::Circle(Vector2D(width / 2, height / 2), 12, 2.0f, Color(45, 235, 60));
	Draw::Circle(Vector2D(spot2D.x, spot2D.y), 12, 2.0f, Color(45, 235, 60));
}
static void DrawBoneMap(C_BasePlayer* player) {
	static HFont boneMapFont = Draw::CreateFont(XorStr("Andale Mono"), 10, (int)FontFlags::FONTFLAG_DROPSHADOW);
	static Vector bone2D;
	static Vector bone3D;
	studiohdr_t* pStudioModel = pModelInfo->GetStudioModel(player->GetModel());

	for (int i = 1; i < pStudioModel->numbones; i++) {
		bone3D = player->GetBonePosition(i);
		if (pDebugOverlay->ScreenPosition(bone3D, bone2D))
			continue;
		if (Settings::ESP::BoneMap::justDrawDots) {
			Draw::FilledCircle(Vector2D(bone2D.x, bone2D.y), 6, 2.0f, Color(255, 10, 255, 255));
		}
		else {
			char buffer[4];
			printf(buffer, 4, "%d", i);
			Draw::Text(Vector2D(bone2D.x, bone2D.y), buffer, boneMapFont, Color(255, 0, 255, 255));
		}
	}
	if (!Settings::ESP::BoneMap::justDrawDots) {
		IEngineClient::player_info_t entityInformation;
		pEngine->GetPlayerInfo(player->GetIndex(), &entityInformation);
		//	pCvar->ConsoleDPrintf(XorStr("(%s)-ModelName: %s, numBones: %d\n"), entityInformation.name, pStudioModel->name, pStudioModel->numbones);
		//XorCompileTime::w_printf(XorStr("(%s)-ModelName: %s, numBones: %d\n"), entityInformation.name, pStudioModel->name, pStudioModel->numbones);
	}
}


static void DrawAutoWall(C_BasePlayer *player)
{
	const std::map<int, int> *modelType = Util::GetModelTypeBoneMap(player);

	static HFont autowallFont = Draw::CreateFont("Andale Mono", 8, (int)FontFlags::FONTFLAG_DROPSHADOW);
	/*
	Vector bone2D;
	Vector bone3D = player->GetBonePosition((int)Bone::BONE_HEAD);
	if( debugOverlay->ScreenPosition(Vector(bone3D.x, bone3D.y, bone3D.z), bone2D))
	return;

	Autowall::FireBulletData data;
	float damage = Autowall::GetDamage(bone3D, !Settings::Aimbot::friendly, data);
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << damage;
	std::string output = stream.str();

	Draw::Text(Vector2D(bone2D.x, bone2D.y), output.c_str(), autowallFont, Color(255, 0, 255, 255)); // hot pink
	*/

	static int len = 31;
	for (int i = 0; i < len; i++)
	{
		int boneIndex = (*modelType).at(i);
		if (boneIndex == (int)Bone::INVALID)
			continue;
		Vector bone2D;
		Vector bone3D = player->GetBonePosition(boneIndex);
		if (pDebugOverlay->ScreenPosition(Vector(bone3D.x, bone3D.y, bone3D.z), bone2D))
			continue;

		Autowall::FireBulletData data;
		float damage = Autowall::GetDamage(bone3D, !Settings::Aimbot::friendly, data);
		std::stringstream stream;
		stream << std::fixed << std::setprecision(0) << damage;
		std::string output = stream.str();

		//Draw::Text(Vector2D(bone2D.x, bone2D.y), output.c_str(), autowallFont, Color::FromImColor(GetESPPlayerColor(player, true))); // for color from config
		Draw::Text(Vector2D(bone2D.x, bone2D.y), output.c_str(), autowallFont, Color(255, 0, 255, 255)); // hot pink
	}
	matrix3x4_t matrix[128];

	if (!player->SetupBones(matrix, 128, 0x100, 0.f))
		return;
	model_t *pModel = player->GetModel();
	if (!pModel)
		return;

	studiohdr_t *hdr = pModelInfo->GetStudioModel(pModel);
	if (!hdr)
		return;

	mstudiobbox_t *bbox = hdr->pHitbox((int)Hitbox::HITBOX_HEAD, 0); // bounding box
	if (!bbox)
		return;

	Vector mins, maxs;
	Math::VectorTransform(bbox->bbmin, matrix[bbox->bone], mins);
	Math::VectorTransform(bbox->bbmax, matrix[bbox->bone], maxs);

	Vector center = (mins + maxs) * 0.5f;

	// 0 - center, 1 - forehead, 2 - skullcap, 3 - upperleftear, 4 - upperrightear, 5 - uppernose, 6 - upperbackofhead
	// 7 - leftear, 8 - rightear, 9 - nose, 10 - backofhead
	Vector headPoints[11] = { center, center, center, center, center, center, center, center, center, center, center };
	headPoints[1].z += bbox->radius * 0.60f;
	headPoints[2].z += bbox->radius * 1.25f;
	headPoints[3].x += bbox->radius * 0.80f;
	headPoints[3].z += bbox->radius * 0.60f;
	headPoints[4].x -= bbox->radius * 0.80f;
	headPoints[4].z += bbox->radius * 0.90f;
	headPoints[5].y += bbox->radius * 0.80f;
	headPoints[5].z += bbox->radius * 0.90f;
	headPoints[6].y -= bbox->radius * 0.80f;
	headPoints[6].z += bbox->radius * 0.90f;
	headPoints[7].x += bbox->radius * 0.80f;
	headPoints[8].x -= bbox->radius * 0.80f;
	headPoints[9].y += bbox->radius * 0.80f;
	headPoints[10].y -= bbox->radius * 0.80f;


	Autowall::FireBulletData data;
	for (int i = 0; i < 11; i++)
	{
		float damage = Autowall::GetDamage(headPoints[i], !Settings::Aimbot::friendly, data);
		std::stringstream stream;
		stream << std::fixed << std::setprecision(0) << damage;
		std::string output = stream.str();

		Vector string2D;
		pDebugOverlay->ScreenPosition(headPoints[i], string2D);
		Draw::Text(Vector2D(string2D.x, string2D.y), output.c_str(), autowallFont, Color(255, 0, 255, 255));
	}
}


static void DrawFOVCrosshair()
{
	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer->GetAlive())
		return;

	int width, height;
	pEngine->GetScreenSize(width, height);

	float radius;
	if (Settings::Aimbot::AutoAim::realDistance)
	{
		Vector src3D, dst3D, forward;
		trace_t tr;
		Ray_t ray;
		CTraceFilter filter;

		Vector angles = viewanglesBackup;
		Math::AngleVectors(angles, forward);
		filter.pSkip = localplayer;
		src3D = localplayer->GetEyePosition();
		dst3D = src3D + (forward * 8192);

		ray.Init(src3D, dst3D);
		pTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		Vector leftViewAngles = Vector(angles.x, angles.y - 90.f, 0.f);
		Math::NormalizeAngles(leftViewAngles);
		Math::AngleVectors(leftViewAngles, forward);
		forward *= Settings::Aimbot::AutoAim::fov * 5.f;

		Vector maxAimAt = tr.endpos + forward;

		Vector max2D;
		if (pDebugOverlay->ScreenPosition(maxAimAt, max2D))
			return;

		radius = fabsf(width / 2 - max2D.x);
	}
	else
	{
		float aimbotFov = Settings::Aimbot::AutoAim::fov;
		float fov = 90;// OverrideView::currentFOV;////////////////
		radius = tanf(DEG2RAD(aimbotFov) / 2) / tanf(DEG2RAD(fov) / 2) * width;
	}

	if (Settings::ESP::FOVCrosshair::filled)
		Draw::FilledCircle(Vector2D(width / 2, height / 2), 20, radius, Color::FromImColor(Settings::ESP::FOVCrosshair::color.Color()));
	else
		Draw::Circle(Vector2D(width / 2, height / 2), 20, radius, Color::FromImColor(Settings::ESP::FOVCrosshair::color.Color()));
}

static void DrawGlow()
{
	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer)
		return;

	for (int i = 0; i < pGlowManager->m_GlowObjectDefinitions.Count(); i++)
	{
		GlowObjectDefinition_t& glow_object = pGlowManager->m_GlowObjectDefinitions[i];

		if (glow_object.IsUnused() || !glow_object.m_pEntity)
			continue;

		ImColor color;
		ClientClass* client = glow_object.m_pEntity->GetClientClass();
		bool shouldGlow = true;

		if (client->m_ClassID == EClassIds::ccsplayer)
		{
			C_BasePlayer* player = (C_BasePlayer*)glow_object.m_pEntity;

			if (player->GetDormant() || !player->GetAlive())
				continue;

			if (player == localplayer)
			{
				color = Settings::ESP::Glow::localplayerColor.Color(player);
			}
			else
			{
				if (glow_object.m_pEntity->GetTeam() != localplayer->GetTeam())
				{
					if (Entity::IsVisible(player, Bone::BONE_HEAD))
						color = Settings::ESP::Glow::enemyVisibleColor.Color(player);
					else
						color = Settings::ESP::Glow::enemyColor.Color(player);
				}
				else
					color = Settings::ESP::Glow::allyColor.Color(player);
			}
		}
		else if (client->m_ClassID != EClassIds::cbaseweaponworldmodel &&
			(strstr(client->m_pNetworkName, "Weapon") || client->m_ClassID == EClassIds::cdeagle || client->m_ClassID == EClassIds::cak47))
		{
			color = Settings::ESP::Glow::weaponColor.Color();
		}
		else if (client->m_ClassID == EClassIds::cbasecsgrenadeprojectile || client->m_ClassID == EClassIds::cdecoyprojectile ||
			client->m_ClassID == EClassIds::cmolotovprojectile || client->m_ClassID == EClassIds::csmokegrenadeprojectile)
		{
			color = Settings::ESP::Glow::grenadeColor.Color();
		}
		else if (client->m_ClassID == EClassIds::cbaseanimating)
		{
			color = Settings::ESP::Glow::defuserColor.Color();

			if (localplayer->HasDefuser() || localplayer->GetTeam() == TeamID::TEAM_TERRORIST)
				shouldGlow = false;
		}
		else if (client->m_ClassID == EClassIds::cchicken)
		{
			color = Settings::ESP::Glow::chickenColor.Color();

			*reinterpret_cast<C_Chicken*>(glow_object.m_pEntity)->GetShouldGlow() = shouldGlow;
		}

		shouldGlow = shouldGlow && color.Value.w > 0;

		glow_object.m_flGlowColor[0] = color.Value.x;
		glow_object.m_flGlowColor[1] = color.Value.y;
		glow_object.m_flGlowColor[2] = color.Value.z;
		glow_object.m_flGlowAlpha = shouldGlow ? color.Value.w : 1.0f;
		glow_object.m_flBloomAmount = 1.0f;
		glow_object.m_bRenderWhenOccluded = shouldGlow;
		glow_object.m_bRenderWhenUnoccluded = false;
	}
}


static void DrawScope()
{
	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer)
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
	if (!activeWeapon)
		return;

	if (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SG556 || *activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_AUG)
		return;

	int width, height;
	pEngine->GetScreenSize(width, height);

	Draw::Line(0, height * 0.5, width, height * 0.5, Color(0, 0, 0, 255));
	Draw::Line(width * 0.5, 0, width * 0.5, height, Color(0, 0, 0, 255));

}



static void DrawSpread()
{
	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer)
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
	if (!activeWeapon)
		return;

	int width, height;
	pEngine->GetScreenSize(width, height);

	if (Settings::ESP::Spread::enabled)
	{
		float cone = activeWeapon->GetInaccuracy();

		if (cone > 0.0f) {
			float radius = (cone * height) / 1.5f;
			Draw::Rectangle(Vector2D(((width / 2) - radius), (height / 2) - radius + 1), Vector2D((width / 2) + radius + 1, (height / 2) + radius + 2), Color::FromImColor(Settings::ESP::FOVCrosshair::color.Color()));
		}
	}
	if (Settings::ESP::Spread::spreadLimit)
	{
		float cone = Settings::Aimbot::SpreadLimit::value;
		if (cone > 0.0f) {

			float radius;
			if (Settings::Aimbot::SpreadLimit::distanceBased)
			{
				Vector src3D, dst3D, forward;
				trace_t tr;
				Ray_t ray;
				CTraceFilter filter;

				Vector angles = viewanglesBackup;
				Math::AngleVectors(angles, forward);
				filter.pSkip = localplayer;
				src3D = localplayer->GetEyePosition();
				dst3D = src3D + (forward * 8192);

				ray.Init(src3D, dst3D);
				pTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

				float dX = tr.endpos.x - src3D.x;
				float dY = tr.endpos.y - src3D.y;
				float dZ = tr.endpos.z - src3D.z;
				float dist = cbrt((dX * dX) + (dY * dY) + (dZ * dZ));

				radius = cone * height / 1.5f * 100.0f / dist;
			}
			else
			{
				radius = cone * height / 1.5f;
			}

			Draw::Rectangle(Vector2D(((width / 2) - radius), (height / 2) - radius + 1), Vector2D((width / 2) + radius + 1, (height / 2) + radius + 2), Color::FromImColor(Settings::ESP::Spread::spreadLimitColor.Color()));
		}
	}
}

static void DrawPlayer(int index, C_BasePlayer* player, IEngineClient::player_info_t player_info) {
	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer)
		return;

	if (player == localplayer && !Settings::ESP::Filters::localplayer)
		return;

	if (player->GetTeam() != localplayer->GetTeam() && !Settings::ESP::Filters::enemies)
		return;

	if (player != localplayer && player->GetTeam() == localplayer->GetTeam() && !Settings::ESP::Filters::allies)
		return;


	bool bIsVisible = false;
	if (Settings::ESP::Filters::visibilityCheck || Settings::ESP::Filters::legit)
	{
		bIsVisible = Entity::IsVisible(player, Bone::BONE_HEAD, 180.f, Settings::ESP::Filters::smokeCheck);
		if (!bIsVisible && Settings::ESP::Filters::legit)
			return;
	}

	//crash gebeurt hierna...

	ImColor playerColor = ESP::GetESPPlayerColor(player, bIsVisible);

	static Vector2D textSize = Draw::GetTextSize("Hi", esp_font);

	int x, y, w, h;
	if (!GetBox(player, x, y, w, h))
		return;

	if (Settings::ESP::Boxes::enabled)
		DrawBox(Color::FromImColor(playerColor), x, y, w, h, player);

	int boxSpacing = Settings::ESP::Boxes::enabled ? 3 : 0;
	Vector2D barsSpacing = Vector2D(0, 0);

	// draw bars
	if (Settings::ESP::Bars::enabled)
	{
		Color barColor;

		// clamp it to 100
		int HealthValue = max(0, min(player->GetHealth(), 100));
		float HealthPerc = HealthValue / 100.f;

		int barx = x;
		int bary = y;
		int barw = w;
		int barh = h;

		if (Settings::ESP::Bars::colorType == BarColorType::HEALTH_BASED)
			barColor = Util::GetHealthColor(HealthValue);
		else if (Settings::ESP::Bars::colorType == BarColorType::STATIC_COLOR)
			barColor = Color::FromImColor(playerColor);

		if (Settings::ESP::Bars::type == BarType::VERTICAL)
		{
			barw = 4; // outline(1px) + bar(2px) + outline(1px) = 6px;
			barx -= barw + boxSpacing; // spacing(1px) + outline(1px) + bar(2px) + outline (1px) = 8 px

			Draw::Rectangle(barx - 1, bary - 1, barx + barw + 1, bary + barh + 1, Color(255, 255, 255, 170));
			Draw::FilledRectangle(barx, bary, barx + barw, bary + barh, Color(10, 10, 10, 255));

			if (HealthPerc > 0)
				Draw::FilledRectangle(barx + 1, bary + (barh * (1.f - HealthPerc)) + 1, barx + barw - 1, bary + barh - 1, barColor);

			barsSpacing.x += barw;
		}
		else if (Settings::ESP::Bars::type == BarType::VERTICAL_RIGHT)
		{
			barx += barw + boxSpacing; // spacing(1px) + outline(1px) + bar(2px) + outline (1px) = 8 px
			barw = 4; // outline(1px) + bar(2px) + outline(1px) = 6px;

			Draw::Rectangle(barx - 1, bary - 1, barx + barw + 1, bary + barh + 1, Color(255, 255, 255, 170));
			Draw::FilledRectangle(barx, bary, barx + barw, bary + barh, Color(10, 10, 10, 255));

			if (HealthPerc > 0)
				Draw::FilledRectangle(barx + 1, bary + (barh * (1.f - HealthPerc)) + 1, barx + barw - 1, bary + barh - 1, barColor);

			barsSpacing.x += barw;
		}
		else if (Settings::ESP::Bars::type == BarType::HORIZONTAL)
		{
			bary += barh + boxSpacing; // player box(?px) + spacing(1px) + outline(1px) + bar(2px) + outline (1px) = 5 px
			barh = 4; // outline(1px) + bar(2px) + outline(1px) = 4px;

			Draw::Rectangle(barx - 1, bary - 1, barx + barw + 1, bary + barh + 1, Color(255, 255, 255, 170));
			Draw::FilledRectangle(barx, bary, barx + barw, bary + barh, Color(10, 10, 10, 255));

			if (HealthPerc > 0)
			{
				barw *= HealthPerc;
				Draw::Rectangle(barx + 1, bary + 1, barx + barw - 1, bary + barh - 1, barColor);
			}
			barsSpacing.y += barh;
		}
		else if (Settings::ESP::Bars::type == BarType::HORIZONTAL_UP)
		{
			barh = 4; // outline(1px) + bar(2px) + outline(1px) = 4px;
			bary -= barh + boxSpacing; // spacing(1px) + outline(1px) + bar(2px) + outline (1px) = 5 px

			Draw::Rectangle(barx - 1, bary - 1, barx + barw + 1, bary + barh + 1, Color(255, 255, 255, 170));
			Draw::FilledRectangle(barx, bary, barx + barw, bary + barh, Color(10, 10, 10, 255));

			if (HealthPerc > 0)
			{
				barw *= HealthPerc;
				Draw::Rectangle(barx + 1, bary + 1, barx + barw - 1, bary + barh - 1, barColor);
			}
			barsSpacing.y += barh;
		}
		else if (Settings::ESP::Bars::type == BarType::INTERWEBZ)
		{
			// pasted from ayyware and broken ( bg is 1px off to the left ) idc
			bary += barh + boxSpacing;
			barh = 4;

			float Width = (w * HealthPerc);
			barw = (int)(Width);

			Vertex_t Verts[4];
			Verts[0].Init(Vector2D(barx, bary));
			Verts[1].Init(Vector2D(barx + w + 5, bary));
			Verts[2].Init(Vector2D(barx + w, bary + 5));
			Verts[3].Init(Vector2D(barx - 5, bary + 5));

			Draw::TexturedPolygon(4, Verts, Color(10, 10, 10, 255));
			Draw::PolyLine(Verts, 4, Color(255, 255, 255, 170));

			Vertex_t Verts2[4];
			Verts2[0].Init(Vector2D(barx + 1, bary + 1));
			Verts2[1].Init(Vector2D(barx + barw + 5, bary + 1));
			Verts2[2].Init(Vector2D(barx + barw, bary + 5));
			Verts2[3].Init(Vector2D(barx - 4, bary + 5));

			Draw::TexturedPolygon(4, Verts2, barColor);

			Verts2[0].Init(Vector2D(barx + 1, bary + 1));
			Verts2[1].Init(Vector2D(barx + barw + 2, bary + 1));
			Verts2[2].Init(Vector2D(barx + barw, bary + 2));
			Verts2[3].Init(Vector2D(barx - 2, bary + 2));

			Draw::TexturedPolygon(4, Verts2, Color(255, 255, 255, 40));

			barsSpacing.y += barh;
		}
	}

	// draw name
	int multiplier = 1;
	int nameOffset = (int)(Settings::ESP::Bars::type == BarType::HORIZONTAL_UP ? boxSpacing + barsSpacing.y : 0);

	if (Settings::ESP::Info::name || Settings::ESP::Info::clan)
	{
		std::string displayString;

		if (Settings::ESP::Info::clan)
			displayString += std::string((*csPlayerResource)->GetClan(index));

		if (Settings::ESP::Info::clan && Settings::ESP::Info::name)
			displayString += " ";

		if (Settings::ESP::Info::name)
			displayString += player_info.name;

		Vector2D nameSize = Draw::GetTextSize(displayString.c_str(), esp_font);
		Draw::Text((int)(x + (w / 2) - (nameSize.x / 2)), (int)(y - textSize.y - nameOffset), displayString.c_str(), esp_font, Color(255, 255, 255));
		multiplier++;
	}

	// draw steamid
	if (Settings::ESP::Info::steamId)
	{
		Vector2D rankSize = Draw::GetTextSize(player_info.guid, esp_font);
		Draw::Text((int)(x + (w / 2) - (rankSize.x / 2)), (int)(y - (textSize.y * multiplier) - nameOffset), player_info.guid, esp_font, Color(255, 255, 255, 255));
		multiplier++;
	}

	// draw rank
	if (Settings::ESP::Info::rank)
	{
		int rank = *(*csPlayerResource)->GetCompetitiveRanking(index);

		if (rank >= 0 && rank < 19)
		{
			Vector2D rankSize = Draw::GetTextSize(ESP::ranks[rank], esp_font);
			Draw::Text((int)(x + (w / 2) - (rankSize.x / 2)), (int)(y - (textSize.y * multiplier) - nameOffset), ESP::ranks[rank], esp_font, Color(255, 255, 255, 255));
		}
	}

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(player->GetActiveWeapon());

	// health
	if (Settings::ESP::Info::health)
	{
		std::string buf = std::to_string(player->GetHealth()) + " HP";
		Draw::Text(x + w + boxSpacing, (int)(y + h - textSize.y), buf.c_str(), esp_font, Color(255, 255, 255));
	}

	// weapon
	if (Settings::ESP::Info::weapon && activeWeapon)
	{
		std::string modelName = Util::Items::GetItemDisplayName(*activeWeapon->GetItemDefinitionIndex());
		int offset = (int)(Settings::ESP::Bars::type == BarType::HORIZONTAL || Settings::ESP::Bars::type == BarType::INTERWEBZ ? boxSpacing + barsSpacing.y + 1 : 0);

		Vector2D weaponTextSize = Draw::GetTextSize(modelName.c_str(), esp_font);

		Draw::Text((int)(x + (w / 2) - (weaponTextSize.x / 2)), y + h + offset, modelName.c_str(), esp_font, Color(255, 255, 255));
	}

	if (Settings::ESP::Info::distance)
	{
	/*	Vector Origin = player->GetVecOrigin();
		Origin.z -= 5.0f;

		std::string buf = "Distance: " + std::to_string(fabsf(localplayer->GetNetworkOrigin().Length2D() - Origin.Length2D()));

		Draw::Text(x + w + boxSpacing, (int)(y + h - textSize.y - 10), buf.c_str(), esp_font, Color(255, 255, 255));*/
	}

	// draw info
	std::vector<std::string> stringsToShow;

	if (Settings::ESP::Info::scoped && player->IsScoped())
		stringsToShow.push_back("Scoped");

	if (Settings::ESP::Info::money)
	{
		char money[6];
		sprintf(money, "$%d", player->GetMoney());
		stringsToShow.push_back(money);
	}

	if (Settings::ESP::Info::reloading && activeWeapon && activeWeapon->GetInReload())
		stringsToShow.push_back("Reloading");

	if (Settings::ESP::Info::flashed && player->GetFlashBangTime() - pGlobalVars->curtime > 2.0f)
		stringsToShow.push_back("Flashed");

	if (Settings::ESP::Info::planting && Entity::IsPlanting(player))
		stringsToShow.push_back("Planting");

	if (Settings::ESP::Info::planting && index == (*csPlayerResource)->GetPlayerC4())
		stringsToShow.push_back("Bomb Carrier");

	if (Settings::ESP::Info::hasDefuser && player->HasDefuser())
		stringsToShow.push_back("Defuse kit");

	if (Settings::ESP::Info::defusing && player->IsDefusing())
		stringsToShow.push_back("Defusing");

	if (Settings::ESP::Info::grabbingHostage && player->IsGrabbingHostage())
		stringsToShow.push_back("Hostage Carrier");

	if (Settings::ESP::Info::rescuing && player->IsRescuing())
		stringsToShow.push_back("Rescuing");

	if (Settings::Debug::AnimLayers::draw) {
		CUtlVector<AnimationLayer> *layers = player->GetAnimOverlay();
		for (int i = 0; i <= layers->Count(); i++) {
			stringsToShow.push_back(Util::GetActivityName(player->GetSequenceActivity(layers->operator[](i).m_nSequence)));
		}
	}

	if (Settings::Resolver::enabled && localplayer->GetAlive() && Resolver::resolvingId == player->GetIndex() && Settings::ESP::Info::lby) {
		if (Settings::Resolver::LagComp) {
			if (Resolver::lbyUpdated)
				stringsToShow.push_back(XorStr("LBY Updated"));
			else if (Backtracking::backtrackingLby)
				stringsToShow.push_back(XorStr("LBY Backtracked"));
			else if (!Backtracking::backtrackingLby && !Resolver::lbyUpdated)
				stringsToShow.push_back(XorStr("Can't backtrack LBY"));
		}
		else {
			if (Resolver::lbyUpdated)
				stringsToShow.push_back(XorStr("LBY Updated"));
			else
				stringsToShow.push_back(XorStr("LBY Not Updated"));
		}
	}


	if (Settings::Resolver::enabled && localplayer->GetAlive() && Settings::ESP::Info::resolveInfo && Resolver::resolvingId == player->GetIndex()) {
		std::string lH = std::to_string(Math::ResNormalizeYaw(Resolver::lastHitAng[player->GetIndex()]));
		std::string aF = std::to_string(Math::ResNormalizeYaw(Resolver::angForce[player->GetIndex()]));
		stringsToShow.push_back(XorStr("Last hit : ") + lH);
		stringsToShow.push_back(XorStr("Resolving  : ") + aF);
	}

	if (Settings::ESP::Info::location)
		stringsToShow.push_back(player->GetLastPlaceName());


	int i = 0;
	for (auto Text : stringsToShow)
	{
		Draw::Text(x + w + boxSpacing, (int)(y + (i * (textSize.y + 2))), Text.c_str(), esp_font, Color(255, 255, 255));
		i++;
	}


	if (Settings::ESP::PlayerIndicator::enabled)
		DrawDirection(player->GetVecOrigin());

	if (Settings::ESP::Skeleton::enabled)
		DrawSkeleton(player);

	if (Settings::ESP::SkeletonBacktrack::enabled)
		DrawBacktrackIndicator(player);

	if (Settings::ESP::BulletTracers::enabled)
		DrawBulletTrace(player);

	if (Settings::ESP::Tracers::enabled && player != localplayer)
		DrawTracer(player);

	if (Settings::ESP::HeadDot::enabled)
		DrawHeaddot(player);

	if (Settings::ESP::AutoWall::debugView)
		DrawAutoWall(player);

	if (Settings::ESP::BoneMap::draw)
		DrawBoneMap(player);

	if (Settings::ESP::AutoAim::drawTarget)
		DrawAimbotSpot();

}

bool ESP::PrePaintTraverse(unsigned int VGUIPanel, bool force_repaint, bool allow_force) {
	if (Settings::ESP::enabled && Settings::NoScopeBorder::enabled &&
		strcmp("HudZoom", pPanel->GetName(VGUIPanel)))
		return false;

	return true;
}

void ESP::Paint()
{
	if (!Settings::ESP::enabled && !pInputSystem->IsButtonDown(Settings::ESP::key))
		return;

	if (!pEngine->IsInGame())
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer)
		return;

	for (int i = 1; i < pEntityList->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* entity = (C_BaseEntity*)pEntityList->GetClientEntity(i);
		if (!entity)
			continue;

		ClientClass* client = entity->GetClientClass();

		if (client->m_ClassID == EClassIds::ccsplayer && (Settings::ESP::Filters::enemies || Settings::ESP::Filters::allies || (Settings::ESP::Filters::localplayer && Settings::ThirdPerson::enabled)))
		{
			C_BasePlayer* player = (C_BasePlayer*)entity;

			if (player->GetDormant() || !player->GetAlive())
				continue;

			IEngineClient::player_info_t playerInfo;
			if (pEngine->GetPlayerInfo(i, &playerInfo))
				DrawPlayer(i, player, playerInfo);
		}
		if ((client->m_ClassID != EClassIds::cbaseweaponworldmodel && (strstr(client->m_pNetworkName, "Weapon") || client->m_ClassID == EClassIds::cdeagle || client->m_ClassID == EClassIds::cak47)) && Settings::ESP::Filters::weapons)
		{
			C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*)entity;
			DrawDroppedWeapons(weapon);
		}
		else if (client->m_ClassID == EClassIds::cc4 && Settings::ESP::Filters::bomb)
		{
			C_BaseCombatWeapon* bomb = (C_BaseCombatWeapon*)entity;
			DrawBomb(bomb);
		}
		else if (client->m_ClassID == EClassIds::cplantedc4 && Settings::ESP::Filters::bomb)
		{
			C_PlantedC4* pC4 = (C_PlantedC4*)entity;
			DrawPlantedBomb(pC4);
		}
		else if (client->m_ClassID == EClassIds::chostage && Settings::ESP::Filters::hostages)
		{
			DrawHostage(entity);
		}
		else if (client->m_ClassID == EClassIds::cbaseanimating && Settings::ESP::Filters::defusers)
		{
			DrawDefuseKit(entity);
		}
		else if (client->m_ClassID == EClassIds::cchicken && Settings::ESP::Filters::chickens)
		{
			DrawChicken(entity);
		}
		else if (client->m_ClassID == EClassIds::cfish && Settings::ESP::Filters::fishes)
		{
			DrawFish(entity);
		}
		else if (Settings::ESP::Filters::throwables && strstr(client->m_pNetworkName, "Projectile"))
		{
			DrawThrowable(entity, client);
		}
	}

	if (Settings::ESP::Sounds::enabled)
		DrawSounds();

	if (Settings::ESP::FOVCrosshair::enabled)
		DrawFOVCrosshair();

	if (Settings::NoScopeBorder::enabled && localplayer->IsScoped())
		DrawScope();

	if (Settings::ESP::Spread::enabled || Settings::ESP::Spread::spreadLimit)
		DrawSpread();
}


//void ESP::DrawModelExecute(ClientFrameStage_t stage)
//{
//	if (stage != ClientFrameStage_t::FRAME_RENDER_START)
//		return;
//
//	if (Settings::ScreenshotCleaner::enabled && pEngine->IsTakingScreenshot())
//		return;
//
//	if (!Settings::ESP::enabled)
//		return;
//
//	if (!pEngine->IsInGame())
//		return;
//
//	if (Settings::ESP::Glow::enabled)
//		DrawGlow();
//}

void ESP::DrawModelExecute(IMatRenderContext* matctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!Settings::ESP::enabled)
		return;

	if (!pEngine->IsInGame())
		return;

	if (Settings::ESP::Glow::enabled)
		DrawGlow();
}


void ESP::EmitSound(int iEntIndex, const char* pSample) {
	if (Settings::ESP::Sounds::enabled)
		CollectFootstep(iEntIndex, pSample);
}



void ESP::CreateMove(CUserCmd* cmd)
{
	if (!Settings::ESP::enabled)
		return;

	viewanglesBackup = cmd->viewangles;
}