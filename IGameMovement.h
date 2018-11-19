//#pragma once
//
//class CMoveData
//{
//public:
//	//bool			m_bFirstRunOfFunctions : 1; // these are removed on linux
//	//bool			m_bGameCodeMovedPlayer : 1;
//
//	void*       	m_nPlayerHandle;	// edict index on server, client entity handle on client
//
//	int				m_nImpulseCommand;	// Impulse command issued.
//	Vector			m_vecViewAngles;	// Command view angles (local space)
//	Vector			m_vecAbsViewAngles;	// Command view angles (world space)
//	int				m_nButtons;			// Attack buttons.
//	int				m_nOldButtons;		// From host_client->oldbuttons;
//	float			m_flForwardMove;
//	float			m_flSideMove;
//	float			m_flUpMove;
//
//	float			m_flMaxSpeed;
//	float			m_flClientMaxSpeed;
//
//	// Variables from the player edict (sv_player) or entvars on the client.
//	// These are copied in here before calling and copied out after calling.
//	Vector			m_vecVelocity;		// edict::velocity		// Current movement direction.
//	Vector			m_vecAngles;		// edict::angles
//	Vector			m_vecOldAngles;
//
//	// Output only
//	float			m_outStepHeight;	// how much you climbed this move
//	Vector			m_outWishVel;		// This is where you tried
//	Vector			m_outJumpVel;		// This is your jump velocity
//
//										// Movement constraints	(radius 0 means no constraint)
//	Vector			m_vecConstraintCenter;
//	float			m_flConstraintRadius;
//	float			m_flConstraintWidth;
//	float			m_flConstraintSpeedFactor;
//
//	void			SetAbsOrigin(const Vector &vec);
//	const Vector	&GetAbsOrigin() const;
//
//private:
//	Vector			m_vecAbsOrigin;		// edict::origin
//};
//
//class IGameMovement {
//public:
//	void ProcessMovement(IClientEntity* player, CMoveData* move) {
//		typedef void(*oProcessMovement)(void*, IClientEntity*, CMoveData*);
//		return getvfunc<oProcessMovement>(this, 2)(this, player, move);
//	}
//
//	void StartTrackPredictionErrors(IClientEntity* player) {
//		typedef void(*oStartTrackPredictionErrors)(void*, IClientEntity*);
//		return getvfunc<oStartTrackPredictionErrors>(this, 4)(this, player);
//	}
//
//	void FinishTrackPredictionErrors(IClientEntity* player) {
//		typedef void(*oFinishTrackPredictionErrors)(void*, IClientEntity*);
//		return getvfunc<oFinishTrackPredictionErrors>(this, 5)(this, player);
//	}
//};
//
//class IMoveHelper {
//public:
//	void SetHost(IClientEntity* host) {
//		typedef void(*oSetHost)(void*, IClientEntity*);
//		return getvfunc<oSetHost>(this, 1)(this, host);
//	}
//};