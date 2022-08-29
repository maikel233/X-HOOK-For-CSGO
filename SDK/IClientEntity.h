#pragma once

#include "../Utils/FindPattern.h"
#include "VirtualMethod.h"
#include "Globalshhh.h"
#include <array>

#define MAX_SHOOT_SOUNDS 16
#define MAX_WEAPON_STRING 80
#define MAX_WEAPON_PREFIX 16
#define MAX_WEAPON_AMMO_NAME 32

#define FLOW_OUTGOING	0		
#define FLOW_INCOMING	1
#define MAX_FLOWS		2

class IWeaponSystem;
class AnimationLayer;
class CBasePlayerAnimState;
class CCSPlayerAnimState;
class CCSGOPlayerAnimState;


enum class LifeState
{
	ALIVE = 0,
	DYING,
	DEAD,
	RESPAWNABLE,
	DISCARDBODY,
};

enum DrawModelFlags_t
{
	STUDIO_NONE = 0x00000000,
	STUDIO_RENDER = 0x00000001,
	STUDIO_VIEWXFORMATTACHMENTS = 0x00000002,
	STUDIO_DRAWTRANSLUCENTSUBMODELS = 0x00000004,
	STUDIO_TWOPASS = 0x00000008,
	STUDIO_STATIC_LIGHTING = 0x00000010,
	STUDIO_WIREFRAME = 0x00000020,
	STUDIO_ITEM_BLINK = 0x00000040,
	STUDIO_NOSHADOWS = 0x00000080,
	STUDIO_WIREFRAME_VCOLLIDE = 0x00000100,
	STUDIO_NOLIGHTING_OR_CUBEMAP = 0x00000200,
	STUDIO_SKIP_FLEXES = 0x00000400,
	STUDIO_DONOTMODIFYSTENCILSTATE = 0x00000800,
	// Not a studio flag, but used to flag model as a non-sorting brush model
	STUDIO_TRANSPARENCY = 0x80000000,
	// Not a studio flag, but used to flag model as using shadow depth material override
	STUDIO_SHADOWDEPTHTEXTURE = 0x40000000,
	// Not a studio flag, but used to flag model as doing custom rendering into shadow texture
	STUDIO_SHADOWTEXTURE = 0x20000000,
	STUDIO_SKIP_DECALS = 0x10000000,
};

enum WeaponSound_t
{
	EMPTY,
	SINGLE,
	SINGLE_NPC,
	WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	TAUNT,
	FAST_RELOAD,

	// Add new shoot sound types here
	REVERSE_THE_NEW_SOUND,

	NUM_SHOOT_SOUND_TYPES,
};

enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};


class ICollideable
{
public:
	virtual void pad0();
	virtual Vector& OBBMins() const;
	virtual Vector& OBBMaxs() const;
	//virtual void pad0();
	//virtual const Vector& OBBMins() const;
	//virtual const Vector& OBBMaxs() const;
};

class AnimationLayer
{
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038x0038


class IHandleEntity
{
public:
	virtual ~IHandleEntity() {};
};

class IClientUnknown : public IHandleEntity {};
class IClientRenderable
{
public:
	virtual ~IClientRenderable() {};

	model_t* GetModel()
	{
		typedef model_t* (__thiscall*oGetModel)(void*);
		return getvfunc<oGetModel>(this, 8)(this);
	}

	int DrawModel(DrawModelFlags_t flags, uint8_t alpha)
	{
		typedef int(__thiscall* origfn)(void*, DrawModelFlags_t, uint8_t);
		return getvfunc<origfn>(this, 9)(this, flags, alpha);
	}

	bool SetupBones(matrix3x4_t* pBoneMatrix, int nMaxBones, int nBoneMask, float flCurTime = 0)
	{
		typedef bool(__thiscall*oSetupBones)(void*, matrix3x4_t*, int, int, float);
		return getvfunc<oSetupBones>(this, 13)(this, pBoneMatrix, nMaxBones, nBoneMask, flCurTime);
	}



};

class IClientNetworkable
{
public:
	/*virtual ~IClientNetworkable() {};*/


	virtual IClientUnknown*  GetIClientUnknown() = 0;
	virtual void             Release() = 0;
//	virtual ClientClass*     GetClientClass() = 0;
	virtual void             NotifyShouldTransmit(int state) = 0;
	virtual void             OnPreDataChanged(int updateType) = 0;
	virtual void             OnDataChanged(int updateType) = 0;
	//virtual void             PreDataUpdate(int updateType) = 0;
	virtual void             PostDataUpdate(int updateType) = 0;
	virtual void             __unkn(void) = 0;
	virtual bool             IsDormant(void) = 0;
	virtual int              EntIndex(void) const = 0;
//	virtual void             ReceiveMessage(int classID, bf_read& msg) = 0;
	virtual void*            GetDataTableBasePtr() = 0;
//	virtual void             SetDestroyedOnRecreateEntities(void) = 0;

	ClientClass* GetClientClass()
	{
		typedef ClientClass* (*oGetClientClass)(void*);
		return getvfunc<oGetClientClass>(this, 2)(this);
	}

	void PreDataUpdate(DataUpdateType_t updateType)
	{
		typedef void(__thiscall*oPreDataUpdate)(void*, DataUpdateType_t);
		return getvfunc<oPreDataUpdate>(this, 6)(this, updateType);
	}

	bool GetDormant()
	{
		typedef bool(__thiscall*oGetDormant)(void*);
		return getvfunc<oGetDormant>(this, 9)(this);
	}

	bool GetDormant2() { return *(bool*)((DWORD)this + 0xE9); }
	int GetIndex()
	{
		typedef int(__thiscall*oGetIndex)(void*);
		return getvfunc<oGetIndex>(this, 10)(this);
	}


	int GetIndex2()
	{
		return *reinterpret_cast<int*>((uintptr_t)this + 0x64);
	}

	TeamID GetTeam()
	{
		return *(TeamID*)((uintptr_t)this + offsets.DT_BaseEntity.m_iTeamNum);
	}

	int GetHealth()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BasePlayer.m_iHealth);
	}

	void SetDestroyedOnRecreateEntities()
	{
		typedef void(__thiscall*oSetDestroyedOnRecreateEntities)(void*);
		return getvfunc<oSetDestroyedOnRecreateEntities>(this, 13)(this);
	}

	float nextAttack()
	{
		return *(float*)((uintptr_t)this + offsets.DT_BaseCombatCharacter.m_flNextAttack);
	}

	float GetSimulationTime()
	{
		return *(float*)((uintptr_t)this + offsets.DT_BaseEntity.m_flSimulationTime);
	}

	Vector GetVelocity()
	{
		return *(Vector*)((uintptr_t)this + offsets.DT_BasePlayer.m_vecVelocity);
	}

	Vector* GetVecOrigin()
	{
		return (Vector*)((uintptr_t)this + offsets.DT_BaseEntity.m_vecOrigin);
	}

	int GetChokedTicks() {
		if (this->GetSimulationTime() > globalsh.OldSimulationTime[this->GetIndex()])
			return fabs(this->GetSimulationTime() - globalsh.OldSimulationTime[this->GetIndex()]);
		return 0;
	}

	Vector getAbsOriginal()
	{
		typedef Vector(__thiscall *o_getAbsOriginal)(void*);
		return getvfunc<o_getAbsOriginal>(this, 10)(this);
	}

	Vector getAbsAngle()
	{
		typedef Vector(__thiscall *o_getAbsAechse)(void*);
		return getvfunc<o_getAbsAechse>(this, 11)(this);
	}

	CUserCmd*& m_pCurrentCommand() {
		static auto current_command = *(uint32_t*)(FindPattern("client.dll", "89 BE ? ? ? ? E8 ? ? ? ? 85 FF") + 2);

		return *(CUserCmd **)((uintptr_t)this + current_command);
	}

	QAngle& GetAngles()
	{
		typedef QAngle&(__thiscall* oGetAbsAngles)(PVOID);
		return getvfunc< oGetAbsAngles >(this, 11)(this);
	}

	Vector GetAbsAngles2() {
		__asm {
			MOV ECX, this;
			MOV EAX, DWORD PTR DS : [ECX];
			CALL DWORD PTR DS : [EAX + 0x2C]
		}
	}


};

class IClientThinkable
{
public:
	virtual ~IClientThinkable() {};
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual ~IClientEntity() {};
};




class C_BaseEntity : public IClientEntity
{
public:



	IClientNetworkable* GetNetworkable()
	{
		return (IClientNetworkable*)((uintptr_t)this + 0x08);
	}

	void SetModelIndex(int model)
	{
		typedef void(__thiscall* oSetModelIndex)(PVOID, int);
		return getvfunc< oSetModelIndex >(this, 75)(this, model);
	}

	int* GetModelIndex()
	{
		return (int*)((uintptr_t)this + offsets.DT_BaseViewModel.m_nModelIndex);
	}

	//NETVAR(float_t, m_flSimulationTime, "CBaseEntity", "m_flSimulationTime");
	
	float GetAnimTime()
	{
		return *(float*)((uintptr_t)this + offsets.DT_BaseEntity.m_flAnimTime);
	}

	float GetSimulationTime()
	{
		return *(float*)((uintptr_t)this + offsets.DT_BaseEntity.m_flSimulationTime);
	}

	float GetCycle()
	{

		return *(float*)((uintptr_t)this + offsets.CBaseAnimating.m_flCycle);
	}
	int GetSequence()
	{
		return *(float*)((uintptr_t)this + offsets.CBaseAnimating.m_nSequence);
	}
	#define BONE_USED_BY_ANYTHING           0x0007FF00
	Vector GetBonePos(int bone)
	{
		matrix3x4_t boneMatrix[MAXSTUDIOBONES];

		if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f)) {
			return Vector(boneMatrix[bone][0][3], boneMatrix[bone][1][3], boneMatrix[bone][2][3]);
		}
		return Vector{};
	}

	float GetFrozen() {
		return *(float*)((uintptr_t)
			this + offsets.DT_ServerAnimationData.m_flFrozen);
	}

	float GetFriction()
	{
		return *reinterpret_cast<float*>(uintptr_t(this + offsets.DT_CSPlayer.m_flFriction));
	}

	float GetPose(int idx)
	{
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets.CBaseAnimating.m_flPoseParameter + sizeof(float) * idx);
	}
	float &GetPose2(int idx)
	{
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offsets.CBaseAnimating.m_flPoseParameter + sizeof(float) * idx);
	}

	TeamID GetTeam()
	{
		return *(TeamID*)((uintptr_t)this + offsets.DT_BaseEntity.m_iTeamNum);
	}




	int GetTeam2()
	{
		return *reinterpret_cast<int*>((DWORD)this + offsets.DT_BaseEntity.m_iTeamNum);
	}

	Vector GetVecOrigin()
	{
		return *(Vector*)((uintptr_t)this + offsets.DT_BaseEntity.m_vecOrigin);
	}


	//VIRTUAL_METHOD(const Vector&, getAbsOrigin, WIN32_LINUX(10, 12), (), (this))
	VIRTUAL_METHOD(const Vector&, getAbsOrigin, WIN32_LINUX(10, 12), (), (this))

	void SetOrigin(Vector origin)
	{
		using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
		static SetAbsOriginFn SetAbsOrigin = (SetAbsOriginFn)FindPattern("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");

		SetAbsOrigin(this, origin);
	}

	void SetAngles(const Vector &angles)
	{
		using SetAbsAnglesFn = void(__thiscall*)(void*, const Vector &angles);
		static SetAbsAnglesFn SetAbsAngles = (SetAbsAnglesFn)FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");

		SetAbsAngles(this, angles);
	}
	void updateClientSideAnimation()
	{
		typedef void(__thiscall *o_updateClientSideAnimation)(void*);
		getvfunc<o_updateClientSideAnimation>(this, 221)(this);
	}


	Vector3 GetVecOrigin2()
	{
		return *(Vector3*)((uintptr_t)this + offsets.DT_BaseEntity.m_vecOrigin);
	}
	MoveType_t GetMoveType()
	{
		return *(MoveType_t*)((uintptr_t)this + offsets.DT_BaseEntity.m_MoveType);
	}

	ICollideable* GetCollideable()
	{
		return (ICollideable*)((uintptr_t)this + offsets.DT_BaseEntity.m_Collision);
	}

	bool* GetSpotted()
	{
		return (bool*)((uintptr_t)this + offsets.DT_BaseEntity.m_bSpotted);
	}

	Vector GetVelocity()
	{
		return *(Vector*)((uintptr_t)this + offsets.DT_BasePlayer.m_vecVelocity);
	}

	int GetHealth()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BasePlayer.m_iHealth);
	}

	int GetHitboxSet()
	{
		return *(int*)((DWORD)this + offsets.DT_BasePlayer.m_nHitboxSet);
	}

	inline int GetUnknownInt()
	{
		return (*(int(__thiscall **)(C_BaseEntity*))(*(DWORD *)this + 0x1E8))(this);  // pated
	}

	inline Vector GetBonePositionBacktrack(int bIndex, matrix3x4_t bMatrix[MAXSTUDIOBONES])
	{
		matrix3x4_t hbox = bMatrix[bIndex];

		return Vector(hbox[0][3], hbox[1][3], hbox[2][3]);
	}


	std::array<float, 24> C_BaseEntity::GetPoseParameter() {
		return *(std::array<float, 24>*) ((uintptr_t)
			this + offsets.DT_BaseAnimating.m_flPoseParameter);
	}

	int C_BaseEntity::GetCollisionGroup()
	{
		return *(int*)((DWORD)this + 0x0470); //  m_CollisionGroup
	}

	int C_BaseEntity::GetNumAnimOverlays()
	{
		return *(int*)((DWORD)this + 0x297C);
	}
	
	
	
};

class CCSPlayerAnimState
{
public:

	virtual ~CCSPlayerAnimState() = 0;
	virtual void Update(float_t eyeYaw, float_t eyePitch) = 0;
};

class CCSGOPlayerAnimState
{
public:

	Vector GetVecVelocity()
	{
		// Only on ground velocity
		return *(Vector*)((uintptr_t)this + 0xC8);
	}

	float GetVelocity()
	{
		return *(float*)((uintptr_t)this + 0xEC);
	}


	char pad_0x0000[0x344]; //0x0000
}; //Size=0x344

//typedef int(*GetSequenceActivityFn)(void*, int);
//extern GetSequenceActivityFn GetSeqActivity;
/* generic game classes */
class C_BasePlayer : public C_BaseEntity
{
public:
	void setAbsOriginal(Vector origin)
	{
		using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
		static SetAbsOriginFn SetAbsOrigin;

		if (!SetAbsOrigin)
			SetAbsOrigin = (SetAbsOriginFn)(FindPattern("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ?")); //\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00", "xxxxxxxxxxxxx??"));

		SetAbsOrigin(this, origin);
	}

	void setAbsAngle(Vector Angle)
	{
		using SetAbsAngleFn = void(__thiscall*)(void*, const Vector &Angle);
		static SetAbsAngleFn SetAbsAngle;

		if (!SetAbsAngle)
			SetAbsAngle = (SetAbsAngleFn)(FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"));// \x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx" ));

		SetAbsAngle(this, Angle);
	}

	void SetOrigin(Vector wantedpos)
	{
		typedef void(__thiscall* SetOriginFn)(void*, const Vector &);
		static SetOriginFn SetOrigin = (SetOriginFn)(FindPattern("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		SetOrigin(this, wantedpos);
	}
	void SetAngle2(Vector wantedang)
	{
		typedef void(__thiscall* SetAngleFn)(void*, const Vector &);
		static SetAngleFn SetAngle = (SetAngleFn)((DWORD)FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		SetAngle(this, wantedang);
	}


	Vector* GetViewPunchAngle()
	{
		return (Vector*)((uintptr_t)this + offsets.DT_BasePlayer.m_viewPunchAngle);
	}

	Vector* GetAimPunchAngle()
	{
		return (Vector*)((uintptr_t)this + offsets.DT_BasePlayer.m_aimPunchAngle);
	}

	Vector GetVecViewOffset()
	{
		return *(Vector*)((uintptr_t)this + offsets.DT_BasePlayer.m_vecViewOffset);
	}

	unsigned int GetTickBase()
	{
		return *(unsigned int*)((uintptr_t)this + offsets.DT_BasePlayer.m_nTickBase);
	}

	Vector GetVelocity()
	{
		return *(Vector*)((uintptr_t)this + offsets.DT_BasePlayer.m_vecVelocity);
	}

	Vector GetNetworkOrigin()
	{
	return *(Vector*)((DWORD)this + 0x00000134);
	}

	float GetMaxSpeed()
	{
		return *(float*)((uintptr_t)this + offsets.DT_BasePlayer.m_flMaxspeed);
	}
	int GetHealth()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BasePlayer.m_iHealth);
	}

	unsigned char GetLifeState()
	{
		return *(unsigned char*)((uintptr_t)this + offsets.DT_BasePlayer.m_lifeState);
	}

	int GetFlags()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BasePlayer.m_fFlags);
	}

	int &GetFlags2()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BasePlayer.m_fFlags);
	}

	ObserverMode_t* GetObserverMode()
	{
		return (ObserverMode_t*)((uintptr_t)this + offsets.DT_BasePlayer.m_iObserverMode);
	}

	ULONG GetObserverTarget()
	{
		return *(PULONG)((DWORD)this + offsets.DT_BasePlayer.m_hObserverTarget);
	}

	ULONG GetViewModel()
	{
		return *(PULONG)((DWORD)this + offsets.DT_BasePlayer.m_hViewModel);
	}

	const char* GetLastPlaceName()
	{
		return (const char*)((uintptr_t)this + offsets.DT_BasePlayer.m_szLastPlaceName);
	}

	int GetShotsFired()
	{
		return *(int*)((uintptr_t)this + offsets.DT_CSPlayer.m_iShotsFired);
	}

	Vector* GetEyeAngles()
	{
		return (Vector*)((uintptr_t)this + offsets.DT_CSPlayer.m_angEyeAngles[0]);
	}


	Vector* GetEyeAnglesPtr()
	{
		return reinterpret_cast<Vector*>((DWORD)this + offsets.DT_CSPlayer.m_angEyeAngles);
	}

	int GetMoney()
	{
		return *(int*)((uintptr_t)this + offsets.DT_CSPlayer.m_iAccount);
	}

	int GetHits()
	{
		return *(int*)((uintptr_t)this + offsets.DT_CSPlayer.m_totalHitsOnServer);
	}

	int GetArmor()
	{
		return *(int*)((uintptr_t)this + offsets.DT_CSPlayer.m_ArmorValue);
	}

	bool HasArmor()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bHasHeavyArmor);
	}

	int HasDefuser()
	{
		return *(int*)((uintptr_t)this + offsets.DT_CSPlayer.m_bHasDefuser);
	}

	bool IsDefusing()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bIsDefusing);
	}

	bool IsGrabbingHostage()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bIsGrabbingHostage);
	}

	bool IsScoped()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bIsScoped);
	}

	bool GetImmune()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bGunGameImmunity);
	}

	bool IsRescuing()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bIsRescuing);
	}

	bool isMoving()
	{
		return *(bool*)((uintptr_t)this + 0x258);
	}


	int HasHelmet()
	{
		return *(int*)((uintptr_t)this + offsets.DT_CSPlayer.m_bHasHelmet);
	}

	float GetFlashBangTime()
	{
		return *(float*)((uintptr_t)this + 0xABE4);
	}

	float GetFlashDuration()
	{
		return *(float*)((uintptr_t)this + offsets.DT_CSPlayer.m_flFlashDuration);
	}

	float* GetFlashMaxAlpha()
	{
		return (float*)((uintptr_t)this + offsets.DT_CSPlayer.m_flFlashMaxAlpha);
	}

	float* GetLowerBodyYawTarget()
	{
		return (float*)((uintptr_t)this + offsets.DT_CSPlayer.m_flLowerBodyYawTarget);
	}

	float GetLowerBodyYawTarget2()
	{
		return *(float*)((DWORD)this + offsets.DT_CSPlayer.m_flLowerBodyYawTarget);
	}

	ULONG GetActiveWeapon()
	{
		return *(PULONG)((DWORD)this + offsets.DT_BaseCombatCharacter.m_hActiveWeapon);
	}

	int GetAmmo()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_iClip1);
	}

	int GetAmmo2()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_iClip1);
	}

	int* GetWeapons()
	{
		return (int*)((uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hMyWeapons);
	}

	int* GetWearables()
	{
		return (int*)((uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hMyWearables);
	}

	bool GetAlive()
	{
		return this->GetHealth() > 0 && this->GetLifeState() == LIFE_ALIVE;
	}

	Vector GetEyePosition()
	{
		return this->GetVecOrigin() + this->GetVecViewOffset();
	}

	void UpdateClientSideAnimation()
	{
		typedef void(__thiscall* Fn)(void*);
		return getvfunc<Fn>(this, 218)(this);
	}

	/*inline Vector GetBonePosition(int boneIndex)
	{
		matrix3x4_t BoneMatrix[MAXSTUDIOBONES];

		if (!this->SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
			return this->GetVecOrigin();

		matrix3x4_t hitbox = BoneMatrix[boneIndex];

		return Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
	}*/


	inline Vector GetBonePosition(int boneIndex)
	{
		if (boneIndex >= 0 && boneIndex < 128)
		{
			matrix3x4_t BoneMatrix[MAXSTUDIOBONES];

			if (this->SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
				return Vector(BoneMatrix[boneIndex][0][3], BoneMatrix[boneIndex][1][3], BoneMatrix[boneIndex][2][3]);
		}

		return this->GetVecOrigin();
	}

	Vector* GetVAngles()
	{
		return (Vector*)((uintptr_t)this + offsets.DT_BasePlayer.deadflag + 0x4);
	}

};

class C_PlantedC4 : public C_BaseEntity
{
public:
	bool IsBombTicking()
	{
		return (bool)((uintptr_t)this + offsets.DT_PlantedC4.m_bBombTicking);
	}

	float GetBombTime()
	{
		return *(float*)((uintptr_t)this + offsets.DT_PlantedC4.m_flC4Blow);
	}

	float GetBombTimeINT()
	{
		return *(int*)((uintptr_t)this + offsets.DT_PlantedC4.m_flC4Blow);
	}

	bool IsBombDefused()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_PlantedC4.m_bBombDefused);
	}

	int GetBombDefuser()
	{
		return *(int*)((uintptr_t)this + offsets.DT_PlantedC4.m_hBombDefuser);
	}
};

class C_EconItemView
{
private:
	using str_32 = char[32];
public:

	int16_t ItemDefinitionIndex()
	{
		return *(int16_t*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_iItemDefinitionIndex);
	}

};




class C_BaseAttributableItem : public C_BaseEntity
{
public:
	ItemDefinitionIndex* GetItemDefinitionIndex()
	{
		return (ItemDefinitionIndex*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_iItemDefinitionIndex);
	}

	unsigned int* GetItemDefinitionIndex2() {
	
		return reinterpret_cast<unsigned int*>(uintptr_t(this) + offsets.DT_BaseAttributableItem.m_iItemDefinitionIndex);
	}
	int* GetItemIDHigh()
	{
		return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_iItemIDHigh);
	}

	int* GetEntityQuality()
	{
		return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_iEntityQuality);
	}

	char* GetCustomName()
	{
		return (char*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_szCustomName);
	}

	int* GetFallbackPaintKit()
	{
		return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_nFallbackPaintKit);
	}
	void SetModelIndex(int model)
	{
		typedef void(__thiscall* oSetModelIndex)(PVOID, int);
		return getvfunc<oSetModelIndex>(this, 245)(this, model);
	}


	void SetModelIndex2(int index)
	{
		typedef void(__thiscall* oSetModelIndex)(void*, int);
		return getvfunc<oSetModelIndex>(this, 245)(this, index);
	}

	int* GetFallbackSeed()
	{
		return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_nFallbackSeed);
	}

	float* GetFallbackWear()
	{
		return (float*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_flFallbackWear);
	}

	int* GetFallbackStatTrak()
	{
		return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_nFallbackStatTrak);
	}

	int* GetAccountID()
	{
		return (int*)((uintptr_t)this + offsets.DT_BaseAttributableItem.m_iAccountID);
	}

	C_EconItemView& m_Item()
	{
		// Cheating. It should be this + m_Item netvar but then the netvars inside C_EconItemView wont work properly.
		// A real fix for this requires a rewrite of the netvar manager
		return *(C_EconItemView*)this;
	}
};

class C_BaseViewModel : public C_BaseEntity
{
public:
	int GetWeapon()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BaseViewModel.m_hWeapon);
	}

	int GetOwner()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BaseViewModel.m_hOwner);
	}
};

class CHudTexture;

class FileWeaponInfo_t
{
public:
	FileWeaponInfo_t();

	// Each game can override this to get whatever values it wants from the script.
	virtual void Parse(KeyValues *pKeyValuesData, const char *szWeaponName);

	bool bParsedScript;
	bool bLoadedHudElements;

	char szClassName[MAX_WEAPON_STRING];
	char szPrintName[MAX_WEAPON_STRING];

	char szViewModel[MAX_WEAPON_STRING];
	char szWorldModel[MAX_WEAPON_STRING];
	char szAmmo1[MAX_WEAPON_AMMO_NAME];
	char szWorldDroppedModel[MAX_WEAPON_STRING];
	char szAnimationPrefix[MAX_WEAPON_PREFIX];
	int iSlot;
	int iPosition;
	int iMaxClip1;
	int iMaxClip2;
	int iDefaultClip1;
	int iDefaultClip2;
	int iWeight;
	int iRumbleEffect;
	bool bAutoSwitchTo;
	bool bAutoSwitchFrom;
	int iFlags;
	char szAmmo2[MAX_WEAPON_AMMO_NAME];
	char szAIAddOn[MAX_WEAPON_STRING];

	// Sound blocks
	char aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];

	int iAmmoType;
	int iAmmo2Type;
	bool m_bMeleeWeapon;

	// This tells if the weapon was built right-handed (defaults to true).
	// This helps cl_righthand make the decision about whether to flip the model or not.
	bool m_bBuiltRightHanded;
	bool m_bAllowFlipping;

	// Sprite data, read from the data file
	int iSpriteCount;
	CHudTexture* iconActive;
	CHudTexture* iconInactive;
	CHudTexture* iconAmmo;
	CHudTexture* iconAmmo2;
	CHudTexture* iconCrosshair;
	CHudTexture* iconAutoaim;
	CHudTexture* iconZoomedCrosshair;
	CHudTexture* iconZoomedAutoaim;
	CHudTexture* iconSmall;
};
#pragma pack(push, 1)
class CCSWeaponInfo : public FileWeaponInfo_t
{
public:
	int8_t pad0[20];
	int32_t GetClipSize;
	int8_t pad1[12];
	int32_t iMaxReservedAmmo; // 0x0024
	int8_t pad2[96];
	char* szHudName;
	char* szWeaponName;
	int8_t pad3[56];
	int32_t GetWeaponType;
	int8_t pad4[4];
	int32_t GetWeaponPrice;
	int32_t iKillAward;
	int8_t pad5[20];
	uint8_t IsFullAuto;
	int8_t pad6[3];
	int32_t GetDamage;
	float_t GetWeaponArmorRatio;
	int32_t iBullets;
	float_t GetPenetration;
	int8_t pad7[12];
	float_t GetRange;
	float_t GetRangeModifier;
	int8_t pad8[16];
	uint8_t bHasSilencer;
	int8_t pad9[15];
	float_t GetSpread;
	float_t flSpreadAlt;
	int8_t pad10[76];
	int32_t iRecoilSeed;
	int8_t pad11[32];



	//char* GetConsoleName()
	//{
	//	return *(char**)((uintptr_t)this + 0x0004);
	//}

	float GetMaxPlayerSpeed()
	{
		return *(float*)((uintptr_t)this + 0x0130);
	}

	float GetMaxPlayerSpeedAlt()
	{
		return *(float*)((uintptr_t)this + 0x0134);
	}

	int GetZoomLevels()
	{
		return *(int*)((uintptr_t)this + 0x1C0); // 0x01BC
	}
	char* GetTracerEffect()
	{
		return *(char**)((uintptr_t)this + 0x01E4); // 0x01E4
	}
	int* GetTracerFrequency()
	{
		return (int*)((uintptr_t)this + 0x01E8); // 0x01E8
	}

};
#pragma pack(pop)
inline IWeaponSystem* pWeaponSystem = nullptr;
class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:

	int GetOwner()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_hOwner);
	}

	int GetAmmo()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_iClip1);
	}

	int GetAmmo2()
	{
		return *(int*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_iClip1);
	}

	float GetNextPrimaryAttack()
	{
		return *(float*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_flNextPrimaryAttack);
	}

	bool GetInReload()
	{
		return *(bool*)((uintptr_t)this + 0x32A5);
	}

	float GetAccuracyPenalty()
	{
		return *(float*)((uintptr_t)this + offsets.DT_WeaponCSBase.m_fAccuracyPenalty);
	}

	float GetPostponeFireReadyTime() {
		return *(float*)((uintptr_t)
			this + offsets.DT_WeaponCSBase.m_flPostponeFireReadyTime);
	}

	//CCSWeaponInfo* GetCSWeaponData()
	//{
	//	typedef CCSWeaponInfo* (__thiscall*oGetCSWpnData)(void*);
	//	return getvfunc<oGetCSWpnData>(this, 460)(this); // OLD 452
	//}
	CCSWeaponInfo* C_BaseCombatWeapon::GetCSWeaponData() {
		return pWeaponSystem->GetWpnData(this->m_Item().ItemDefinitionIndex());
	}

	//updated below

	float GetInaccuracy()
	{
		typedef float(__thiscall*oGetInaccuracy)(void*);
		return getvfunc<oGetInaccuracy>(this, 483)(this);  
	}

	float GetSpread()
	{
		typedef float(__thiscall*oGetSpread)(void*);
		return getvfunc<oGetSpread>(this, 452)(this);  // OLD 446
	}

	void UpdateAccuracyPenalty()
	{
		typedef void(__thiscall*oUpdateAccuracyPenalty)(void*);
		return getvfunc<oUpdateAccuracyPenalty>(this, 484)(this); 
	}

	bool IsGrenade()
	{
		return GetCSWeaponData()->GetWeaponType != CSWeaponType::WEAPONTYPE_GRENADE;
	}

	bool IsGun()
	{
		switch (GetCSWeaponData()->GetWeaponType)
		{
		case CSWeaponType::WEAPONTYPE_C4:
			return false;
		case CSWeaponType::WEAPONTYPE_GRENADE:
			return false;
		case CSWeaponType::WEAPONTYPE_KNIFE:
			return false;
		case CSWeaponType::WEAPONTYPE_UNKNOWN:
			return false;
		default:
			return true;
		}
	}

	bool IsKnife()
	{
		if (this->m_Item().ItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_TASER) return false;
		return GetCSWeaponData()->GetWeaponType == CSWeaponType::WEAPONTYPE_KNIFE;
	}

	bool IsRifle()
	{
		switch (GetCSWeaponData()->GetWeaponType)
		{
		case CSWeaponType::WEAPONTYPE_RIFLE:
			return true;
		case CSWeaponType::WEAPONTYPE_SUBMACHINEGUN:
			return true;
		case CSWeaponType::WEAPONTYPE_SHOTGUN:
			return true;
		case CSWeaponType::WEAPONTYPE_MACHINEGUN:
			return true;
		default:
			return false;
		}
	}

	bool IsPistol()
	{
		switch (GetCSWeaponData()->GetWeaponType)
		{
		case CSWeaponType::WEAPONTYPE_PISTOL:
			return true;
		default:
			return false;
		}
	}
	bool IsSniper()
	{
		switch (GetCSWeaponData()->GetWeaponType)
		{
		case CSWeaponType::WEAPONTYPE_SNIPER_RIFLE:
			return true;
		default:
			return false;
		}
	}

};


class C_WeaponC4 : C_BaseCombatWeapon
{
public:
	bool GetStartedArming()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_WeaponC4.m_bStartedArming);
	}
};

class C_Chicken : C_BaseEntity
{
public:
	bool* GetShouldGlow()
	{
		return (bool*)((uintptr_t)this + offsets.DT_DynamicProp.m_bShouldGlow);
	}
};

class C_BaseCSGrenade : C_BaseCombatWeapon
{
public:
	bool IsHeldByPlayer()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_BaseCSGrenade.m_bIsHeldByPlayer);
	}

	bool GetPinPulled()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_BaseCSGrenade.m_bPinPulled);
	}

	float GetThrowTime()
	{
		return *(float*)((uintptr_t)this + offsets.DT_BaseCSGrenade.m_fThrowTime);
	}

	float GetThrowStrength()
	{
		return *(float*)((uintptr_t)this + offsets.DT_BaseCSGrenade.m_flThrowStrength);
	}
};



