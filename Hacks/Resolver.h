#pragma once

#include "../Features.h"
#include <random>

#define TIME_TO_TICKS( dt )        ( (int)( 0.5f + (float)(dt) / pGlobalVars->interval_per_tick ) )
#define TICKS_TO_TIME( t )        ( pGlobalVars->interval_per_tick * ( t ) )

class CResolveInfo {

public:

	//std::deque<CTickRecord> m_sRecords;
	//std::deque<CTickRecord> old_m_sRecords;
	bool m_bEnemyShot; //priority
	bool m_bLowerBodyYawChanged;
	bool m_bBacktrackThisTick;

};

namespace Resolver {

	extern std::vector<int64_t> playerAngleLogs;
	extern std::array<CResolveInfo, 32> m_arrInfos;

	extern std::map<int, int> shotsMiss;

	extern std::map<int, float> angForce;
	extern std::map<int, float> lastHitAng;
	extern std::map<int, const char*> angForceTxt;
	extern std::map<int, float> lby;

	extern int resolvingId;

	extern bool lbyUpdated;

	extern bool shouldBaim;

	//void Override();

	//void Hug(C_BasePlayer* Circlebian);

	//void FrameStageNotify(ClientFrameStage_t stage);

	//void PostFrameStageNotify(ClientFrameStage_t stage);

	//void FireGameEvent(IGameEvent* event);

	//void CreateMove(CUserCmd* cmd);

	////CTickRecord GetShotRecord(C_BasePlayer*);

	//bool HasStaticRealAngle(int index, float tolerance = 15.f);

	/*bool HasStaticRealAngle(const std::deque<CTickRecord>& l, float tolerance = 15.f);

	bool HasStaticYawDifference(const std::deque<CTickRecord>& l, float tolerance = 15.f);

	bool HasSteadyDifference(const std::deque<CTickRecord>& l, float tolerance = 15.f);

	int GetDifferentDeltas(const std::deque<CTickRecord>& l, float tolerance = 15.f);

	int GetDifferentLBYs(const std::deque<CTickRecord>& l, float tolerance = 15.f);

	float GetLBYByComparingTicks(const std::deque<CTickRecord>& l);

	float GetDeltaByComparingTicks(const std::deque<CTickRecord>& l);

	bool DeltaKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance = 15.f);

	bool LBYKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance = 15.f);*/

	/*bool IsEntityMoving(C_BasePlayer* Circlebian);

	bool& LowerBodyYawChanged(C_BasePlayer* Circlebian);

	void StoreVars(C_BasePlayer* Circlebian);

	void StoreVars(C_BasePlayer* Circlebian, Vector ang, float lby, float simtime, float tick);

	bool& BacktrackThisTick(C_BasePlayer* Circlebian);*/

//	bool IsAdjustingBalance(C_BasePlayer *player, CTickRecord &record, AnimationLayer *layer);
}

const inline float GetDelta(float a, float b) {
	return abs(Math::ClampYaw(a - b));
}

//const inline float LBYDelta(const CTickRecord& v) {
//	return v.m_angEyeAngles.y - v.m_flLowerBodyYawTarget;
//}

const inline bool IsDifferent(float a, float b, float tolerance = 10.f) {
	return (GetDelta(a, b) > tolerance);
}
