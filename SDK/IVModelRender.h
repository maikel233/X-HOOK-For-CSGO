#pragma once

typedef unsigned short ModelInstanceHandle_t;

struct model_t
{
	char name[255];
};

struct ModelRenderInfo_t
{
	Vector origin;
	Vector angles;
	char pad[0x4]; // <- new
	void *pRenderable;
	model_t* pModel;
	const matrix3x4_t* pModelToWorld;
	const matrix3x4_t* pLightingOffset;
	const Vector *pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;
	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

class IMatRenderContext;



struct DrawModelState_t;
typedef float matrix3x4[3][4];


class IVModelRender
{
public:
	void ForcedMaterialOverride(IMaterial* mat)
	{
		typedef void(__thiscall* oForcedMaterialOverride)(void*, IMaterial*, int, int);
		return getvfunc<oForcedMaterialOverride>(this, 1)(this, mat, 0, 0);
	}

	void DrawModelExecute(void* ctx, void* state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld = NULL) {
		typedef void(*oDrawModelExecute)(void*, void* ctx, void* state, const ModelRenderInfo_t& pInfo,
			matrix3x4_t* pCustomBoneToWorld);
		return getvfunc<oDrawModelExecute>(this, 21)(this, ctx, state, pInfo, pCustomBoneToWorld);
	}

	/*void DrawModelExecute(IMatRenderContext* matctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4* pCustomBoneToWorld)
	{
		typedef void(__thiscall* DrawModelExecuteFn)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4*);
		getvfunc<DrawModelExecuteFn>(this, 21)(this, matctx, state, pInfo, pCustomBoneToWorld);
	}*/
};


