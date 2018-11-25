#pragma once

enum MotionBlurMode_t
{
	MOTION_BLUR_DISABLE = 1,
	MOTION_BLUR_GAME = 2,			// Game uses real-time inter-frame data
	MOTION_BLUR_SFM = 3				// Use SFM data passed in CViewSetup structure
};

class CViewSetup {
public:
	int x;
	int oldX;
	int y;
	int oldY;
	int width;
	int oldWidth;
	int height;
	int oldHeight;

	bool m_bOrtho;
	float m_OrthoLeft;
	float m_OrthoTop;
	float m_OrthoRight;
	float m_OrthoBottom;

private:
	char pad1[0x7C];

public:
	float fov;
	float fovViewmodel;
	Vector origin;
	Vector angles;

	float zNear;
	float zFar;
	float zNearViewmodel;
	float zFarViewmodel;

	float m_flAspectRatio;
	float m_flNearBlurDepth;
	float m_flNearFocusDepth;
	float m_flFarFocusDepth;
	float m_flFarBlurDepth;
	float m_flNearBlurRadius;
	float m_flFarBlurRadius;
	int m_nDoFQuality;
	MotionBlurMode_t m_nMotionBlurMode;

	float m_flShutterTime;
	Vector m_vShutterOpenPosition;
	Vector m_shutterOpenAngles;
	Vector m_vShutterClosePosition;
	Vector m_shutterCloseAngles;

	float m_flOffCenterTop;
	float m_flOffCenterBottom;
	float m_flOffCenterLeft;
	float m_flOffCenterRight;

	bool m_bOffCenter : 1;
	bool m_bRenderToSubrectOfLargerScreen : 1;
	bool m_bDoBloomAndToneMapping : 1;
	bool m_bDoDepthOfField : 1;
	bool m_bHDRTarget : 1;
	bool m_bDrawWorldNormal : 1;
	bool m_bCullFrontFaces : 1;
	bool m_bCacheFullSceneState : 1;
	bool m_bRenderFlashlightDepthTranslucents : 1;
private:
	char pad2[0x40];
};

class CViewRender
{
public:
	void RenderView(CViewSetup &view, CViewSetup &hud, int nClearFlags, int whatToDraw)
	{
		typedef void(__thiscall*oGetAllClasses)(void*, CViewSetup&, CViewSetup&, int, int);
		return getvfunc<oGetAllClasses>(this, 6)(this, view, hud, nClearFlags, whatToDraw);
	}
};

class ViewSetup {
public:
	int x;
	int unscaled_x;
	int y;
	int unscaled_y;
	int width;
	int unscaled_width;
	int height;
	int unscaled_height;
	int stereo_eye;
	char _pad0x24[0x78];
	bool ortho;
	float ortho_left;
	float ortho_top;
	float ortho_right;
	float ortho_bottom;
	float fov;
	float fov_viewmodel;
	Vector origin;
	Vector angles;
	float z_near;
	float z_far;
	float z_near_viewmodel;
	float z_far_viewmodel;
	bool render_to_subrect_of_larger_screen;
	float aspect_ratio;
	bool off_center;
	float off_center_top;
	float off_center_bottom;
	float off_center_left;
	float off_center_right;
	bool do_bloom_and_tone_mapping;
	bool cache_full_scene_state;
	bool view_to_projection_override;
	//Matrix view_to_projection;
};


