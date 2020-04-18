#pragma once

#include "sdk/SDK.h"
#include "interfaces.h"

/*#define PI 3.14159265358979323846f

inline Vector angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}
inline float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

namespace SDK
{
	struct lbyRecords
	{
		int tick_count;
		float lby;
		Vector headPosition;
	};

	struct backtrackData
	{
		float simtime;
		Vector hitboxPos;
	};

	class BackTracks
	{
		int latest_tick;
		
		void UpdateRecord(int i);
	public:
		float GetLerpTime();
		bool IsTickValid(int tick);
		lbyRecords records[64];
		bool RunLBYBackTrack(int i, CUserCmd* cmd, Vector& aimPoint);
		void legitBackTrack(CUserCmd * cmd);
		void Update(int tick_count);
	};
}

extern SDK::backtrackData headPositions[64][12];

extern SDK::BackTracks* backtrackin*///g;