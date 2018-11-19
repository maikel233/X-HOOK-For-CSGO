#pragma once

namespace NameStealer
{
	extern int entityId;
	void BeginFrame(float frameTime);
	void FireGameEvent(IGameEvent* event);
};