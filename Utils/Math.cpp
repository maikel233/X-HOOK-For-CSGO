#include "Math.h"
#include "../settings.h"
#include "../Hacks/AntiAim.h"


void inline Math::SinCos(float radians, float* sine, float* cosine) 
    //double __cosr, __sinr;
    //__asm "fsincos" : "=t" (__cosr), "=u" (__sinr) : "0" (radians);

    //*sine = __sinr;
    //*cosine = __cosr;
    {
        __asm
        {
            fld dword ptr[radians]
            fsincos
            mov edx, dword ptr[cosine]
            mov eax, dword ptr[sine]
            fstp dword ptr[edx]
            fstp dword ptr[eax]
        }
    } 


float Math::float_rand(float min,
    float max) // thanks foo - https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c :^)
{
    float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
    return min + scale * (max - min);      /* [min, max] */
}

void Math::AngleVectors(const Vector& angles, Vector& forward) {
    Assert(s_bMathlibInitialized);
    Assert(forward);

    float sp, sy, cp, cy;

    Math::SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
    Math::SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);

    forward.x = cp * cy;
    forward.y = cp * sy;
    forward.z = -sp;
}

float Math::RoundFloat(float f) {
    return static_cast<float>(floor(f * 5 + 0.5) / 5);
}

void Math::NormalizeAngles(Vector& angle) {
    while (angle.x > 89.0f)
        angle.x -= 180.0f;

    while (angle.x < -89.0f)
        angle.x += 180.0f;

    while (angle.y > 180.0f)
        angle.y -= 360.0f;

    while (angle.y < -180.0f)
        angle.y += 360.0f;
}

float Math::ResNormalizeYaw(float yaw) {
    float yaw2 = yaw;

    while (yaw2 > 180.0f)
        yaw2 -= 360.0f;
    while (yaw2 < -180.0f)
        yaw2 += 360.0f;

    return yaw2;
}

void Math::NormalizePitch(float& pitch) {
    while (pitch > 89.0f)
        pitch -= 180.0f;
    while (pitch < -89.0f)
        pitch += 180.0f;
}

void Math::NormalizeYaw(float& yaw) {
    while (yaw > 180.0f)
        yaw -= 360.0f;
    while (yaw < -180.0f)
        yaw += 360.0f;
}

void Math::ClampAngles(Vector& angle) {
    if ((AntiAim::IsStanding() && !Settings::AntiAim::Standing::untrustedAngles) ||
        (AntiAim::IsMoving() && !Settings::AntiAim::Moving::untrustedAngles) ||
        (AntiAim::IsAirborne() && !Settings::AntiAim::Airborne::untrustedAngles)) {
        if (angle.y > 180.0f)
            angle.y = 180.0f;
        else if (angle.y < -180.0f)
            angle.y = -180.0f;


        if (angle.x > 89.0f)
            angle.x = 89.0f;
        else if (angle.x < -89.0f)
            angle.x = -89.0f;
        if (angle.z > 0)
            angle.z = 0;
        else if (angle.z < 0)
            angle.z = 0;

    }
}

void Math::ClampY(int& y) {
    if ((AntiAim::IsStanding() && !Settings::AntiAim::Standing::untrustedAngles) ||
        (AntiAim::IsMoving() && !Settings::AntiAim::Moving::untrustedAngles) ||
        (AntiAim::IsAirborne() && !Settings::AntiAim::Airborne::untrustedAngles)) {
        if (y > 180)
            y = 180;

        if (y < -180)
            y = -180;
    }
}

void Math::CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove) {
    // side/forward move correction
    float deltaView;
    float f1;
    float f2;

    if (vOldAngles.y < 0.f)
        f1 = 360.0f + vOldAngles.y;
    else
        f1 = vOldAngles.y;

    if (pCmd->viewangles.y < 0.0f)
        f2 = 360.0f + pCmd->viewangles.y;
    else
        f2 = pCmd->viewangles.y;

    if (f2 < f1)
        deltaView = abs(f2 - f1);
    else
        deltaView = 360.0f - abs(f1 - f2);

    deltaView = 360.0f - deltaView;

    pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
    if ((pCmd->viewangles.x > 90 && pCmd->viewangles.x < 270) ||
        (pCmd->viewangles.x < -90 && pCmd->viewangles.x > -270))
        pCmd->forwardmove *= -1;
    pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

float Math::GetFov(const Vector& viewAngle, const Vector& aimAngle) {
    Vector delta = aimAngle - viewAngle;
    NormalizeAngles(delta);

    return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

void Math::VectorAngles(const Vector& forward, Vector& angles) {
    if (forward[1] == 0.0f && forward[0] == 0.0f) {
        angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
        angles[1] = 0.0f;  //yaw left/right
    }
    else {
        angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
        angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

        if (angles[1] > 90)
            angles[1] -= 180;
        else if (angles[1] < 90)
            angles[1] += 180;
        else if (angles[1] == 90)
            angles[1] = 0;
    }

    angles[2] = 0.0f;
}

float Math::DotProduct(Vector& v1, const float* v2) {
    return v1.x * v2[0] + v1.y * v2[1] + v1.z * v2[2];
}

void Math::AngleMatrix(const Vector& angles, matrix3x4_t& matrix) {
    float sr, sp, sy, cr, cp, cy;
    Math::SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
    Math::SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
    Math::SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);

    // matrix = (YAW * PITCH) * ROLL
    matrix[0][0] = cp * cy;
    matrix[1][0] = cp * sy;
    matrix[2][0] = -sp;

    float crcy = cr * cy;
    float crsy = cr * sy;
    float srcy = sr * cy;
    float srsy = sr * sy;
    matrix[0][1] = sp * srcy - crsy;
    matrix[1][1] = sp * srsy + crcy;
    matrix[2][1] = sr * cp;

    matrix[0][2] = (sp * crcy + srsy);
    matrix[1][2] = (sp * crsy - srcy);
    matrix[2][2] = cr * cp;

    matrix[0][3] = 0.0f;
    matrix[1][3] = 0.0f;
    matrix[2][3] = 0.0f;
}

void Math::VectorTransform(Vector& in1, const matrix3x4_t& in2, Vector& out) {
    out.x = DotProduct(in1, in2[0]) + in2[0][3];
    out.y = DotProduct(in1, in2[1]) + in2[1][3];
    out.z = DotProduct(in1, in2[2]) + in2[2][3];
}

Vector Math::CalcAngle(Vector src, Vector dst) {
    Vector angles;
    Vector delta = src - dst;

    Math::VectorAngles(delta, angles);

    delta.Normalize();

    return angles;
}

float Math::ClampYaw(float val) {
    while (val < 0) val += 360.0f;
    while (val > 360.0f) val -= 360.0f;
    return val;
}

void Math::CalcAngleYawOnly(Vector src, Vector dst, Vector& angles) {
    Vector delta = src - dst;
    // double hyp = delta.Length2D();
    angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
    //	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
    angles[2] = 0.00;

    if (delta.x >= 0.0)
        angles.y += 180.0f;
}

void Math::Normalize(Vector& vIn, Vector& vOut) {
    float flLen = vIn.Length();
    if (flLen == 0) {
        vOut.Init(0, 0, 1);
        return;
    }
    flLen = 1 / flLen;
    vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

void Math::NormalizeVector(Vector& vec) {
    for (int i = 0; i < 3; ++i) {
        while (vec[i] > 180.f)
            vec[i] -= 360.f;

        while (vec[i] < -180.f)
            vec[i] += 360.f;
    }
    vec[2] = 0.f;
}

void Math::VectorAngles2(Vector& vecForward, Vector& vecAngles) {
    Vector vecView;
    if (vecForward[1] == 0.f && vecForward[0] == 0.f) {
        vecView[0] = 0.f;
        vecView[1] = 0.f;
    }
    else {
        vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

        if (vecView[1] < 0.f)
            vecView[1] += 360.f;

        vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

        vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
    }

    vecAngles[0] = -vecView[0];
    vecAngles[1] = vecView[1];
    vecAngles[2] = 0.f;
}

void Math::AngleVectors2(Vector& qAngles, Vector& vecForward) {
    float sp, sy, cp, cy;
    SinCos((float)(qAngles[1] * (M_PI / 180.f)), &sy, &cy);
    SinCos((float)(qAngles[0] * (M_PI / 180.f)), &sp, &cp);

    vecForward[0] = cp * cy;
    vecForward[1] = cp * sy;
    vecForward[2] = -sp;
}

void Math::AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up) {
    float sr, sp, sy, cr, cp, cy;

    SinCos(DEG2RAD(angles[1]), &sy, &cy);
    SinCos(DEG2RAD(angles[0]), &sp, &cp);
    SinCos(DEG2RAD(angles[2]), &sr, &cr);

    if (forward) {
        forward->x = cp * cy;
        forward->y = cp * sy;
        forward->z = -sp;
    }

    if (right) {
        right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
        right->y = (-1 * sr * sp * sy + -1 * cr * cy);
        right->z = -1 * sr * cp;
    }

    if (up) {
        up->x = (cr * sp * cy + -sr * -sy);
        up->y = (cr * sp * sy + -sr * cy);
        up->z = cr * cp;
    }
}

//
//void inline Math::SinCos(float x, float* s, float* c)
//{
//	__asm
//	{
//		fld dword ptr[x]
//		fsincos
//		mov edx, dword ptr[c]
//		mov eax, dword ptr[s]
//		fstp dword ptr[edx]
//		fstp dword ptr[eax]
//	}
//}
//
//float Math::float_rand(float min,
//	float max) // thanks foo - https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c :^)
//{
//	float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
//	return min + scale * (max - min);      /* [min, max] */
//}
//
//void Math::AngleVectors(const Vector& angles, Vector& forward) {
//	Assert(s_bMathlibInitialized);
//	Assert(forward);
//
//	float sp, sy, cp, cy;
//
//	Math::SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
//	Math::SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
//
//	forward.x = cp * cy;
//	forward.y = cp * sy;
//	forward.z = -sp;
//}
//
//float Math::RoundFloat(float f) {
//	return static_cast<float>(floor(f * 5 + 0.5) / 5);
//}
//
//void Math::NormalizeAngles(Vector& angle) {
//	while (angle.x > 89.0f)
//		angle.x -= 180.0f;
//
//	while (angle.x < -89.0f)
//		angle.x += 180.0f;
//
//	while (angle.y > 180.0f)
//		angle.y -= 360.0f;
//
//	while (angle.y < -180.0f)
//		angle.y += 360.0f;
//}
//
//float Math::ResNormalizeYaw(float yaw) {
//	float yaw2 = yaw;
//
//	while (yaw2 > 180.0f)
//		yaw2 -= 360.0f;
//	while (yaw2 < -180.0f)
//		yaw2 += 360.0f;
//
//	return yaw2;
//}
//
//void Math::NormalizePitch(float& pitch) {
//	while (pitch > 89.0f)
//		pitch -= 180.0f;
//	while (pitch < -89.0f)
//		pitch += 180.0f;
//}
//
//void Math::NormalizeYaw(float& yaw) {
//	while (yaw > 180.0f)
//		yaw -= 360.0f;
//	while (yaw < -180.0f)
//		yaw += 360.0f;
//}
//
//float Math::NormalizeYaw2(float value)
//{
//	while (value > 180)
//		value -= 360.f;
//
//	while (value < -180)
//		value += 360.f;
//	return value;
//}
//
//
//void Math::ClampAngles(Vector& angle) {
//	if ((AntiAim::IsStanding() && !Settings::AntiAim::Standing::untrustedAngles) ||
//		(AntiAim::IsMoving() && !Settings::AntiAim::Moving::untrustedAngles) ||
//		(AntiAim::IsAirborne() && !Settings::AntiAim::Airborne::untrustedAngles)) {
//		if (angle.y > 180.0f)
//			angle.y = 180.0f;
//		else if (angle.y < -180.0f)
//			angle.y = -180.0f;
//
//
//		if (angle.x > 89.0f)
//			angle.x = 89.0f;
//		else if (angle.x < -89.0f)
//			angle.x = -89.0f;
//		if (angle.z > 0)
//			angle.z = 0;
//		else if (angle.z < 0)
//			angle.z = 0;
//
//	}
//}
//
//void Math::ClampY(int& y) {
//	if ((AntiAim::IsStanding() && !Settings::AntiAim::Standing::untrustedAngles) ||
//		(AntiAim::IsMoving() && !Settings::AntiAim::Moving::untrustedAngles) ||
//		(AntiAim::IsAirborne() && !Settings::AntiAim::Airborne::untrustedAngles)) {
//		if (y > 180)
//			y = 180;
//
//		if (y < -180)
//			y = -180;
//	}
//}
//
//void Math::CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove) {
//	// side/forward move correction
//	float deltaView;
//	float f1;
//	float f2;
//
//	if (vOldAngles.y < 0.f)
//		f1 = 360.0f + vOldAngles.y;
//	else
//		f1 = vOldAngles.y;
//
//	if (pCmd->viewangles.y < 0.0f)
//		f2 = 360.0f + pCmd->viewangles.y;
//	else
//		f2 = pCmd->viewangles.y;
//
//	if (f2 < f1)
//		deltaView = abs(f2 - f1);
//	else
//		deltaView = 360.0f - abs(f1 - f2);
//
//	deltaView = 360.0f - deltaView;
//
//	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
//	if ((pCmd->viewangles.x > 90 && pCmd->viewangles.x < 270) ||
//		(pCmd->viewangles.x < -90 && pCmd->viewangles.x > -270))
//		pCmd->forwardmove *= -1;
//	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
//}
//
//float Math::GetFov(const Vector& viewAngle, const Vector& aimAngle) {
//	Vector delta = aimAngle - viewAngle;
//	NormalizeAngles(delta);
//
//	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
//}
//
//void Math::VectorAngles(const Vector& forward, Vector& angles) {
//	if (forward[1] == 0.0f && forward[0] == 0.0f) {
//		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
//		angles[1] = 0.0f;  //yaw left/right
//	}
//	else {
//		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
//		angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;
//
//		if (angles[1] > 90)
//			angles[1] -= 180;
//		else if (angles[1] < 90)
//			angles[1] += 180;
//		else if (angles[1] == 90)
//			angles[1] = 0;
//	}
//
//	angles[2] = 0.0f;
//}
//
//float Math::DotProduct(Vector& v1, const float* v2) {
//	return v1.x * v2[0] + v1.y * v2[1] + v1.z * v2[2];
//}
//
//
//void Math::AngleMatrix(const Vector& angles, matrix3x4_t& matrix) {
//	float sr, sp, sy, cr, cp, cy;
//	Math::SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
//	Math::SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
//	Math::SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);
//
//	// matrix = (YAW * PITCH) * ROLL
//	matrix[0][0] = cp * cy;
//	matrix[1][0] = cp * sy;
//	matrix[2][0] = -sp;
//
//	float crcy = cr * cy;
//	float crsy = cr * sy;
//	float srcy = sr * cy;
//	float srsy = sr * sy;
//	matrix[0][1] = sp * srcy - crsy;
//	matrix[1][1] = sp * srsy + crcy;
//	matrix[2][1] = sr * cp;
//
//	matrix[0][2] = (sp * crcy + srsy);
//	matrix[1][2] = (sp * crsy - srcy);
//	matrix[2][2] = cr * cp;
//
//	matrix[0][3] = 0.0f;
//	matrix[1][3] = 0.0f;
//	matrix[2][3] = 0.0f;
//}
//
//void Math::VectorTransform(Vector& in1, const matrix3x4_t& in2, Vector& out) {
//	out.x = DotProduct(in1, in2[0]) + in2[0][3];
//	out.y = DotProduct(in1, in2[1]) + in2[1][3];
//	out.z = DotProduct(in1, in2[2]) + in2[2][3];
//}
//
//
//Vector Math::CalcAngle(Vector src, Vector dst) {
//	Vector angles;
//	Vector delta = src - dst;
//
//	Math::VectorAngles(delta, angles);
//
//	delta.Normalize();
//
//	return angles;
//}
//
//float Math::ClampYaw(float val) {
//	while (val < 0) val += 360.0f;
//	while (val > 360.0f) val -= 360.0f;
//	return val;
//}
//
//void Math::CalcAngleYawOnly(Vector src, Vector dst, Vector& angles) {
//	Vector delta = src - dst;
//	// double hyp = delta.Length2D();
//	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
//	//	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
//	angles[2] = 0.00;
//
//	if (delta.x >= 0.0)
//		angles.y += 180.0f;
//}
//
//void Math::Normalize(Vector& vIn, Vector& vOut) {
//	float flLen = vIn.Length();
//	if (flLen == 0) {
//		vOut.Init(0, 0, 1);
//		return;
//	}
//	flLen = 1 / flLen;
//	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
//}
//
//void Math::NormalizeVector(Vector& vec) {
//	for (int i = 0; i < 3; ++i) {
//		while (vec[i] > 180.f)
//			vec[i] -= 360.f;
//
//		while (vec[i] < -180.f)
//			vec[i] += 360.f;
//	}
//	vec[2] = 0.f;
//}
//
//void Math::VectorAngles2(Vector& vecForward, Vector& vecAngles) {
//	Vector vecView;
//	if (vecForward[1] == 0.f && vecForward[0] == 0.f) {
//		vecView[0] = 0.f;
//		vecView[1] = 0.f;
//	}
//	else {
//		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;
//
//		if (vecView[1] < 0.f)
//			vecView[1] += 360.f;
//
//		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);
//
//		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
//	}
//
//	vecAngles[0] = -vecView[0];
//	vecAngles[1] = vecView[1];
//	vecAngles[2] = 0.f;
//}
//
//void Math::AngleVectors2(Vector& Vectors, Vector& vecForward) {
//	float sp, sy, cp, cy;
//	SinCos((float)(Vectors[1] * (M_PI / 180.f)), &sy, &cy);
//	SinCos((float)(Vectors[0] * (M_PI / 180.f)), &sp, &cp);
//
//	vecForward[0] = cp * cy;
//	vecForward[1] = cp * sy;
//	vecForward[2] = -sp;
//}
//
//
//
//
//void Math::AngleVectors3(const Vector angles, Vector& forward, Vector& right, Vector& up)
//{
//	float angle;
//	static float sp, sy, cp, cy;
//
//	angle = angles[0] * (M_PI / 180.f);
//	sp = sin(angle);
//	cp = cos(angle);
//
//	angle = angles[1] * (M_PI / 180.f);
//	sy = sin(angle);
//	cy = cos(angle);
//
//
//	forward[0] = cp * cy;
//	forward[1] = cp * sy;
//	forward[2] = -sp;
//
//
//	static float sr, cr;
//
//	angle = angles[2] * (M_PI / 180.f);
//	sr = sin(angle);
//	cr = cos(angle);
//
//
//	right[0] = -1 * sr * sp * cy + -1 * cr * -sy;
//	right[1] = -1 * sr * sp * sy + -1 * cr *cy;
//	right[2] = -1 * sr * cp;
//
//	up[0] = cr * sp *cy + -sr * -sy;
//	up[1] = cr * sp *sy + -sr * cy;
//	up[2] = cr * cp;
//
//
//}
//void Math::AngleVectors3(const Vector &angles, Vector *forward)
//{
//	Assert(s_bMathlibInitialized);
//	Assert(forward);
//
//	float	sp, sy, cp, cy;
//
//	sy = sin(DEG2RAD(angles[1]));
//	cy = cos(DEG2RAD(angles[1]));
//
//	sp = sin(DEG2RAD(angles[0]));
//	cp = cos(DEG2RAD(angles[0]));
//
//	forward->x = cp*cy;
//	forward->y = cp*sy;
//	forward->z = -sp;
//}
//
//void Math::AngleVectors3(const Vector &angles, Vector *forward, Vector *right, Vector *up)
//{
//	auto SinCos = [](float flRadians, float* pflSine, float* pflCosine)
//	{
//	/*	__asm
//		{
//			fld	DWORD PTR[flRadians]
//			fsincos
//			mov edx, DWORD PTR[pflCosine]
//			mov eax, DWORD PTR[pflSine]
//			fstp DWORD PTR[edx]
//			fstp DWORD PTR[eax]
//		}*/
//	};
//	float sr, sp, sy, cr, cp, cy;
//
//	SinCos(DEG2RAD(angles[1]), &sy, &cy);
//	SinCos(DEG2RAD(angles[0]), &sp, &cp);
//	SinCos(DEG2RAD(angles[2]), &sr, &cr);
//
//	if (forward)
//	{
//		forward->x = cp*cy;
//		forward->y = cp*sy;
//		forward->z = -sp;
//	}
//
//	if (right)
//	{
//		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
//		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
//		right->z = -1 * sr*cp;
//	}
//
//	if (up)
//	{
//		up->x = (cr*sp*cy + -sr*-sy);
//		up->y = (cr*sp*sy + -sr*cy);
//		up->z = cr*cp;
//	}
//}
//
//void Math::AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up) {
//	float sr, sp, sy, cr, cp, cy;
//
//	SinCos(DEG2RAD(angles[1]), &sy, &cy);
//	SinCos(DEG2RAD(angles[0]), &sp, &cp);
//	SinCos(DEG2RAD(angles[2]), &sr, &cr);
//
//	if (forward) {
//		forward->x = cp * cy;
//		forward->y = cp * sy;
//		forward->z = -sp;
//	}
//
//	if (right) {
//		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
//		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
//		right->z = -1 * sr * cp;
//	}
//
//	if (up) {
//		up->x = (cr * sp * cy + -sr * -sy);
//		up->y = (cr * sp * sy + -sr * cy);
//		up->z = cr * cp;
//	}
//}
//
////void inline Math::SinCos(float x, float* s, float* c)
////{
////	__asm
////	{
////		fld dword ptr[x]
////		fsincos
////		mov edx, dword ptr[c]
////		mov eax, dword ptr[s]
////		fstp dword ptr[edx]
////		fstp dword ptr[eax]
////	}
////}
////
////float Math::float_rand(float min, float max) // thanks foo - https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c :^)
////{
////	float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
////	return min + scale * (max - min);      /* [min, max] */
////}
////
////float Math::RoundFloat(float f) {
////	return static_cast<float>(floor(f * 5 + 0.5) / 5);
////}
////
////
////void Math::AngleVectors( const Vector& angles, Vector* forward, Vector* right, Vector* up ) {
////    float sr, sp, sy, cr, cp, cy;
////
////    SinCos( DEG2RAD( angles[1] ), &sy, &cy );
////    SinCos( DEG2RAD( angles[0] ), &sp, &cp );
////    SinCos( DEG2RAD( angles[2] ), &sr, &cr );
////
////    if ( forward ) {
////        forward->x = cp * cy;
////        forward->y = cp * sy;
////        forward->z = -sp;
////    }
////
////    if ( right ) {
////        right->x = ( -1 * sr * sp * cy + -1 * cr * -sy );
////        right->y = ( -1 * sr * sp * sy + -1 * cr * cy );
////        right->z = -1 * sr * cp;
////    }
////
////    if ( up ) {
////        up->x = ( cr * sp * cy + -sr * -sy );
////        up->y = ( cr * sp * sy + -sr * cy );
////        up->z = cr * cp;
////    }
////}
////
////void Math::AngleVectors2(Vector& qAngles, Vector& vecForward) {
////	float sp, sy, cp, cy;
////	SinCos((float)(qAngles[1] * (M_PI / 180.f)), &sy, &cy);
////	SinCos((float)(qAngles[0] * (M_PI / 180.f)), &sp, &cp);
////
////	vecForward[0] = cp * cy;
////	vecForward[1] = cp * sy;
////	vecForward[2] = -sp;
////}
////
////Vector Math::AnglesToPixels(Vector SourceAngles, Vector FinalAngles, float sensitivityy, float mpitch, float myaw)
////{
////	Vector delta = SourceAngles - FinalAngles;
////	NormalizeAngles(delta);
////	float xMove = (-delta.x) / (myaw * sensitivityy);
////	float yMove = (delta.y) / (mpitch * sensitivityy);
////
////	return Vector(yMove, xMove, 0.0f);
////}
////
////Vector Math::PixelsDeltaToAnglesDelta(Vector PixelsDelta, float sensitivityy, float mpitch, float myaw)
////{
////	float xMove = (-PixelsDelta.x) * (myaw * sensitivityy);
////	float yMove = (PixelsDelta.y) * (mpitch * sensitivityy);
////
////	return Vector(yMove, xMove, 0.0f);
////}
////
////void Math::NormalizeAngles(Vector& angle)
////{
////	while (angle.x > 89.0f)
////		angle.x -= 180.f;
////
////	while (angle.x < -89.0f)
////		angle.x += 180.f;
////
////	while (angle.y > 180.f)
////		angle.y -= 360.f;
////
////	while (angle.y < -180.f)
////		angle.y += 360.f;
////}
////
////float Math::ResNormalizeYaw(float yaw) {
////	float yaw2 = yaw;
////
////	while (yaw2 > 180.0f)
////		yaw2 -= 360.0f;
////	while (yaw2 < -180.0f)
////		yaw2 += 360.0f;
////
////	return yaw2;
////}
////
////Vector Math::GetHitboxPosition(C_BaseEntity* pEntity, int Hitbox)
////{
////	matrix3x4_t matrix[128];
////
////
////	if (!pEntity->SetupBones(matrix, 128, 0x00000100, pEntity->GetSimulationTime()))
////		return Vector(0, 0, 0);
////
////
////
////	studiohdr_t* hdr = pModelInfo->GetStudioModel(pEntity->GetModel());
////	mstudiohitboxset_t* set = hdr->pHitboxSet(0);
////
////	mstudiobbox_t* hitbox = set->pHitbox(Hitbox);
////
////	if (!hitbox)
////		return Vector(0, 0, 0);
////
////	Vector vMin, vMax, vCenter, sCenter;
////	Math::VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
////	Math::VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
////	vCenter = (vMin + vMax) *0.5f;
////	return vCenter;
////}
////
////void Math::NormalizePitch(float& pitch) {
////	while (pitch > 89.0f)
////		pitch -= 180.0f;
////	while (pitch < -89.0f)
////		pitch += 180.0f;
////}
////
////void Math::NormalizeYaw(float& yaw) {
////	while (yaw > 180.0f)
////		yaw -= 360.0f;
////	while (yaw < -180.0f)
////		yaw += 360.0f;
////}
////
////void Math::ClampAngles(Vector& angle)
////{
////	if (angle.y > 180.0f)
////		angle.y = 180.0f;
////	else if (angle.y < -180.0f)
////		angle.y = -180.0f;
////
////	if (angle.x > 89.0f)
////		angle.x = 89.0f;
////	else if (angle.x < -89.0f)
////		angle.x = -89.0f;
////
////	angle.z = 0;
////}
////
////float Math::ClampYaw(float val)
////{
////	while (val < 0) val += 360.0f;
////	while (val > 360.0f) val -= 360.0f;
////	return val;
////}
////
////
////void Math::CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
////{
////	// side/forward move correction
////	float deltaView;
////	float f1;
////	float f2;
////
////	if (vOldAngles.y < 0.f)
////		f1 = 360.0f + vOldAngles.y;
////	else
////		f1 = vOldAngles.y;
////
////	if (pCmd->viewangles.y < 0.0f)
////		f2 = 360.0f + pCmd->viewangles.y;
////	else
////		f2 = pCmd->viewangles.y;
////
////	if (f2 < f1)
////		deltaView = abs(f2 - f1);
////	else
////		deltaView = 360.0f - abs(f1 - f2);
////
////	deltaView = 360.0f - deltaView;
////
////	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
////	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
////}
////
////void Math::ClampY(int& y) {
////	if ((AntiAim::IsStanding() && !Settings::AntiAim::Standing::untrustedAngles) ||
////		(AntiAim::IsMoving() && !Settings::AntiAim::Moving::untrustedAngles) ||
////		(AntiAim::IsAirborne() && !Settings::AntiAim::Airborne::untrustedAngles)) {
////		if (y > 180)
////			y = 180;
////
////		if (y < -180)
////			y = -180;
////	}
////}
////
////float Math::GetFov(const Vector& viewAngle, const Vector& aimAngle)
////{
////	Vector delta = aimAngle - viewAngle;
////	NormalizeAngles(delta);
////
////	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
////}
////
////void Math::VectorAngles(const Vector& forward, Vector &angles)
////{
////	if (forward[1] == 0.0f && forward[0] == 0.0f)
////	{
////		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
////		angles[1] = 0.0f;  //yaw left/right
////	}
////	else
////	{
////		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
////		angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;
////
////		if (angles[1] > 90)
////			angles[1] -= 180;
////		else if (angles[1] < 90)
////			angles[1] += 180;
////		else if (angles[1] == 90)
////			angles[1] = 0;
////	}
////
////	angles[2] = 0.0f;
////}
////
////float Math::DotProduct(Vector &v1, const float* v2)
////{
////	return v1.x*v2[0] + v1.y*v2[1] + v1.z*v2[2];
////}
////float Math::DotProduct2(const Vector& a, const Vector& b)
////{
////	return (a.x * b.x + a.y * b.y + a.z * b.z);
////}
////void Math::VectorTransform(Vector &in1, const matrix3x4_t& in2, Vector &out)
////{
////	out.x = DotProduct(in1, in2[0]) + in2[0][3];
////	out.y = DotProduct(in1, in2[1]) + in2[1][3];
////	out.z = DotProduct(in1, in2[2]) + in2[2][3];
////}
////
////Vector Math::CalcAngle(Vector src, Vector dst)
////{
////	Vector angles;
////	Vector delta = src - dst;
////
////	Math::VectorAngles(delta, angles);
////
////	delta.Normalize();
////
////	return angles;
////}
////
