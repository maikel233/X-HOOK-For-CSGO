#pragma once
struct MoveData { byte Data[256]; };
class IGameMovement
{
public:
	virtual			~IGameMovement(void) {}

	virtual void	ProcessMovement(IClientEntity *pPlayer, MoveData *pMove) = 0;
	virtual void	Reset(void) = 0;
	virtual void	StartTrackPredictionErrors(IClientEntity *pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(IClientEntity *pPlayer) = 0;
	virtual void	DiffPrint(char const *fmt, ...) = 0;

	virtual Vector const&	GetPlayerMins(bool ducked) const = 0;
	virtual Vector const&	GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const& GetPlayerViewOffset(bool ducked) const = 0;

	virtual bool			IsMovingPlayerStuck(void) const = 0;
	virtual C_BaseEntity*	GetMovingPlayer(void) const = 0;
	virtual void			UnblockPusher(IClientEntity* pPlayer, IClientEntity *pPusher) = 0;

	virtual void SetupMovementBounds(MoveData *pMove) = 0;
};

class IMoveHelper
{
public:
	void SetHost(IClientEntity* pPlayer)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, IClientEntity* pPlayer);
		getvfunc< OriginalFn >(this, 1)(this, pPlayer);
	}
};

enum PLAYER_ANIM;

//-----------------------------------------------------------------------------
// Implementation of the movehelper on the server
//-----------------------------------------------------------------------------
#define abstract_class class
abstract_class IMoveHelperServer : public IMoveHelper
{
	//public:
	//virtual void SetHost(CBasePlayer *host) = 0; //dylan removed
};

class CMoveHelperServer : public IMoveHelperServer
{
public:
	CMoveHelperServer(void);
	virtual ~CMoveHelperServer();

	// Methods associated with a particular entity
	virtual	char const*		GetName(IClientEntity* handle) const;

	// Touch list...
	virtual void	ResetTouchList(void);
	virtual bool	AddToTouched(const trace_t &tr, const Vector& impactvelocity);
	virtual void	ProcessImpacts(void);

	virtual bool	PlayerFallingDamage(void);
	virtual void	PlayerSetAnimation(PLAYER_ANIM eAnim);

	// Numbered line printf
	virtual void	Con_NPrintf(int idx, char const* fmt, ...);

	// These have separate server vs client impementations
	virtual void	StartSound(const Vector& origin, int channel, char const* sample, float volume, soundlevel_t soundlevel, int fFlags, int pitch);
	virtual void	StartSound(const Vector& origin, const char *soundname);

	virtual void	PlaybackEventFull(int flags, int clientindex, unsigned short eventindex, float delay, Vector& origin, Vector& angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
	virtual IPhysicsSurfaceProps *GetSurfaceProps(void);

	//void DylanMissingFunction(void);

	//void			SetHost(CBasePlayer *host);

	virtual bool IsWorldEntity(const CBaseHandle &handle);

private:
	C_BasePlayer*	m_pHostPlayer;

	// results, tallied on client and server, but only used by server to run SV_Impact.
	// we store off our velocity in the trace_t structure so that we can determine results
	// of shoving boxes etc. around.
	struct touchlist_t
	{
		Vector	deltavelocity;
		trace_t trace;
	};

	//CUtlVector<touchlist_t>	m_TouchList;
};


class IPrediction {
public:
	virtual void UnknownVirtual0() = 0;
	virtual void UnknownVirtual1() = 0;
	virtual void UnknownVirtual2() = 0;
	virtual void UnknownVirtual3() = 0;
	virtual void UnknownVirtual4() = 0;
	virtual void UnknownVirtual5() = 0;
	virtual void UnknownVirtual6() = 0;
	virtual void UnknownVirtual7() = 0;
	virtual void UnknownVirtual8() = 0;
	virtual void UnknownVirtual9() = 0;
	virtual void UnknownVirtual10() = 0;
	virtual void UnknownVirtual11() = 0;
	virtual void UnknownVirtual12() = 0;
	virtual void UnknownVirtual13() = 0;
	virtual void UnknownVirtual14() = 0;
	virtual void UnknownVirtual15() = 0;
	virtual void UnknownVirtual16() = 0;
	virtual void UnknownVirtual17() = 0;
	virtual void UnknownVirtual18() = 0;
	virtual void UnknownVirtual19() = 0;
	//virtual void SetupMove(IClientEntity* player, CUserCmd* ucmd, void* pHelper, MoveData* move) = 0; //20
	//virtual void FinishMove(IClientEntity* player, CUserCmd* ucmd, MoveData* move) = 0;


	bool InPrediction() {
		typedef bool(__thiscall* oInPrediction)(void*);
		return getvfunc<oInPrediction>(this, 14)(this);
	}

	void RunCommand(IClientEntity* player, CUserCmd *ucmd, IMoveHelper *moveHelper) {
		typedef void(__thiscall* oRunCommand)(void*, IClientEntity*, CUserCmd*, IMoveHelper*);
		return getvfunc<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
	}

	void SetupMove(IClientEntity* player, CUserCmd *ucmd, IMoveHelper *moveHelper, void* pMoveData) {
		typedef void(__thiscall* oSetupMove)(void*, IClientEntity*, CUserCmd*, IMoveHelper*, void*);
		return getvfunc<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
	}

	void FinishMove(IClientEntity* player, CUserCmd *ucmd, void*pMoveData) {
		typedef void(__thiscall* oFinishMove)(void*, IClientEntity*, CUserCmd*, void*);
		return getvfunc<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
	}
};