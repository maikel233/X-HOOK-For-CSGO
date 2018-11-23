#pragma once

namespace FOVChanger
{
	void OverrideView(ViewSetup* pSetup);
	void GetViewModelFOV(float& fov);
};

namespace OverrideView
{
	extern float currentFOV;
}
