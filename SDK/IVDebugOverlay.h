#pragma once

class IVDebugOverlay
{
public:

		// Linux +1 for every index.

		/*	CIVDebugOverlay::AddEntityTextOverlay(int, int, float, int, int, int, int, char const*, ...)
		1	CIVDebugOverlay::AddBoxOverlay(Vector const&, Vector const&, Vector const&, QAngle const&, int, int, int, int, float)
		2	CIVDebugOverlay::AddSphereOverlay(Vector const&, float, int, int, int, int, int, int, float)
		3	CIVDebugOverlay::AddTriangleOverlay(Vector const&, Vector const&, Vector const&, int, int, int, int, bool, float)
		4	CIVDebugOverlay::AddLineOverlay(Vector const&, Vector const&, int, int, int, bool, float)
		5	CIVDebugOverlay::AddTextOverlay(Vector const&, float, char const*, ...)
		6	CIVDebugOverlay::AddTextOverlay(Vector const&, int, float, char const*, ...)
		7	CIVDebugOverlay::AddScreenTextOverlay(float, float, float, int, int, int, int, char const*)
		8	CIVDebugOverlay::AddSweptBoxOverlay(Vector const&, Vector const&, Vector const&, Vector const&, QAngle const&, int, int, int, int, float)
		9	CIVDebugOverlay::AddGridOverlay(Vector const&)
		10	CIVDebugOverlay::AddCoordFrameOverlay(matrix3x4_t const&, float, int(*)[3])
		11	CIVDebugOverlay::ScreenPosition(Vector const&, Vector&)
		12	CIVDebugOverlay::ScreenPosition(float, float, Vector &)
		13	CIVDebugOverlay::GetFirst(void)
		14	CIVDebugOverlay::GetNext(OverlayText_t *)
		15	CIVDebugOverlay::ClearDeadOverlays(void)
		16	CIVDebugOverlay::ClearAllOverlays(void)
		17	CIVDebugOverlay::AddTextOverlayRGB(Vector const&, int, float, float, float, float, float, char const*, ...)
		18	CIVDebugOverlay::AddTextOverlayRGB(Vector const&, int, float, int, int, int, int, char const*, ...)
		19	CIVDebugOverlay::AddLineOverlayAlpha(Vector const&, Vector const&, int, int, int, int, bool, float)
		20	CIVDebugOverlay::AddBoxOverlay2(Vector const&, Vector const&, Vector const&, QAngle const&, Color const&, Color const&, float)
		21	CIVDebugOverlay::AddLineOverlay(Vector const&, Vector const&, int, int, int, int, float, float)
		22	CIVDebugOverlay::PurgeTextOverlays(void)
		23	CIVDebugOverlay::AddCapsuleOverlay(Vector const&, Vector const&, float const&, int, int, int, int, float)*/


	virtual void AddSphereOverlay(const Vector& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration = 0) = 0;



	bool ScreenPosition(const Vector& vIn, Vector& vOut)
	{
		typedef bool(__thiscall* oScreenPosition)(void*, const Vector&, Vector&);
		return getvfunc<oScreenPosition>(this, 13)(this, vIn, vOut);
	}

	void DrawPill(const Vector& mins, const Vector& max, float& diameter, int r, int g, int b, int a, float duration)
	{
		typedef void(*OriginalFn)(void*, const Vector&, const Vector&, float&, int, int, int, int, float);
		return getvfunc<OriginalFn>(this, 24)(this, mins, max, diameter, r, g, b, a, duration);
	}


	void AddCapsuleOverlay(Vector& mins, Vector& maxs, float pillradius, int r, int g, int b, int a, float duration) 
	{
	typedef void(*OriginalFn)(void*, Vector&, Vector&, float&, int, int, int, int, float);
	return getvfunc<OriginalFn>(this, 24)(this, mins, maxs, pillradius, r, g, b, a, duration);
	}
};