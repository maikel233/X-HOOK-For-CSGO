#pragma once

#include "../SDK/SDK.h"
#include "../Utils/Draw.h"
#include "../Utils/Math.h"
#include "../Settings.h"
enum Act {

	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP
};

namespace GrenadePrediction {

	void Setup(Vector& vecSrc, Vector& vecThrow, Vector viewangles);

	void Simulate(ViewSetup* setup);

	int Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval);

	bool CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval);

	void TraceHull(Vector& src, Vector& end, trace_t& tr);

	void AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground);

	void PushEntity(Vector& src, const Vector& move, trace_t& tr);

	void ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval);

	int PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce);

	// Hooks

	void CreateMove(CUserCmd* cmd);

	void OverrideView(ViewSetup* setup);

	void Paint();

}
