#pragma once

#include "SDK/SDK.h"

namespace Math {
	void inline SinCos(float radians, float* sine, float* cosine);

	void AngleVectors(const Vector& angles, Vector& forward);

	void NormalizeAngles(Vector& angle);

	float ResNormalizeYaw(float yaw);

	void ClampAngles(Vector& angle);

	void ClampY(int& y);

	float RoundFloat(float f);

	void CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove);

	float GetFov(const Vector& viewAngle, const Vector& aimAngle);

	float DotProduct(Vector& v1, const float* v2);

	void AngleMatrix(const Vector& angles, matrix3x4_t& matrix);

	void VectorAngles(const Vector& forward, Vector& angles);

	void VectorTransform(Vector& in1, const matrix3x4_t& in2, Vector& out);

	Vector CalcAngle(Vector src, Vector dst);

	float float_rand(float min, float max);

	void CalcAngleYawOnly(Vector src, Vector dst, Vector& angles);

	void Normalize(Vector& vIn, Vector& vOut);

	void NormalizePitch(float& pitch);

	void NormalizeVector(Vector& vec);

	void NormalizeYaw(float& yaw);

	float NormalizeYaw2(float value);

	void VectorAngles2(Vector& vecForward, Vector& vecAngles);

	void AngleVectors2(Vector& Vectors, Vector& vecForward);
	
	
	void AngleVectors3(const Vector angles, Vector& forward, Vector& right, Vector& up);
	void AngleVectors3(const Vector &angles, Vector* forward);
	void AngleVectors3(const Vector &angles, Vector *forward, Vector *right, Vector *up);

	void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);

	float ClampYaw(float val);
}

