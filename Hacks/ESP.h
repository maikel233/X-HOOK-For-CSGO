#pragma once
#include "../interfaces.h"

#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)
#define FLOAT32_NAN_BITS     (uint32)0x7FC00000	// not a number!
#define FLOAT32_NAN          BitsToFloat( FLOAT32_NAN_BITS )
#define VEC_T_NAN FLOAT32_NAN
#define DECL_ALIGN(x)			__declspec( align( x ) )
#define ALIGN4 DECL_ALIGN(4)
#define ALIGN8 DECL_ALIGN(8)
#define ALIGN16 DECL_ALIGN(16)
#define ALIGN32 DECL_ALIGN(32)
#define ALIGN128 DECL_ALIGN(128)
typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;

// intp is an integer that can accomodate a pointer
// (ie, sizeof(intp) >= sizeof(int) && sizeof(intp) >= sizeof(void *)
typedef intptr_t intp;
typedef uintptr_t uintp;

inline float BitsToFloat(uint32 i)
{
	union Convertor_t
	{
		float f;
		unsigned long ul;
	} tmp;
	tmp.ul = i;
	return tmp.f;
}


class ALIGN16 matrix3x4a_t : public matrix3x4_t
{
public:
	/*
	matrix3x4a_t() { if (((size_t)Base()) % 16 != 0) { Error( "matrix3x4a_t missaligned" ); } }
	*/
	matrix3x4a_t& operator=(const matrix3x4_t& src)
	{
		memcpy(Base(), src.Base(), sizeof(float) * 3 * 4);
		return *this;
	};
};

namespace ESP
{
	extern const char* ranks[];

	ImColor GetESPPlayerColor(C_BasePlayer* player, bool visible);
	//void DrawModelExecute(ClientFrameStage_t stage);
	void DrawModelExecute(IMatRenderContext* matctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	bool PrePaintTraverse(unsigned int VGUIPanel, bool force_repaint, bool allow_force);
	void Paint();
	void EmitSound(int iEntIndex, const char* pSample);
	void CreateMove(CUserCmd* cmd);
}