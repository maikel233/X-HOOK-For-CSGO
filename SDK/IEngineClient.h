#pragma once



typedef void* FileNameHandle_t;

struct SndInfo_t
{
	int m_nGuid;
	FileNameHandle_t m_filenameHandle;
	int m_nSoundSource;
	int m_nChannel;

	int m_nSpeakerEntity;
	float m_flVolume;
	float m_flLastSpatializedVolume;

	float m_flRadius;
	int m_nPitch;
	Vector *m_pOrigin;
	Vector *m_pDirection;

	bool m_bUpdatePositions;
	bool m_bIsSentence;
	bool m_bDryMix;
	bool m_bSpeaker;
	bool m_bSpecialDSP;
	bool m_bFromServer;
};

class IEngineClient
{
public:
	typedef struct player_info_s
	{
		int64_t __pad0;
		union {
			int64_t xuid;
			struct {
				int xuidlow;
				int xuidhigh;
			};
		};
		char name[128];
		int userid;
		char guid[33];
		unsigned int friendsid;
		char friendsname[128];
		bool fakeplayer;
		bool ishltv;
		unsigned int customfiles[4];
		unsigned char filesdownloaded;
	} player_info_t;

	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* oGetScreenSize)(void*, int&, int&);
		return getvfunc<oGetScreenSize>(this, 5)(this, width, height);
	}

	bool GetPlayerInfo(int iIndex, player_info_t *pInfo)
	{
		typedef bool(__thiscall* oGetPlayerInfo)(void*, int, player_info_t*);
		return getvfunc<oGetPlayerInfo>(this, 8)(this, iIndex, pInfo);
	}

	int GetPlayerForUserID(int UserID)
	{
		typedef int(__thiscall*oGetPlayerForUserID)(void*, int);
		return getvfunc<oGetPlayerForUserID>(this, 9)(this, UserID);
	}

	int GetLocalPlayer()
	{
		typedef int(__thiscall*oGetLocalPlayer)(void*);
		return getvfunc<oGetLocalPlayer>(this, 12)(this);
	}

	float Time()
	{
		typedef float(__thiscall* Fn)(PVOID);
		return getvfunc<Fn>(this, 14)(this);

	}


	VIRTUAL_METHOD(void, GetViewAngles, 18, (Vector& angles), (this, std::ref(angles)))
	VIRTUAL_METHOD(void, SetViewAngles, 19, (const Vector& angles), (this, std::cref(angles)))

	VIRTUAL_METHOD(int, GetMaxClients, 20, (), (this))
	VIRTUAL_METHOD(bool, IsInGame, 26, (), (this))
	VIRTUAL_METHOD(bool, isConnnected, 27, (), (this))
	VIRTUAL_METHOD(NetworkChannel*, GetNetChannelInfo, 78, (), (this))

	const char *GetGameDirectory()
	{
		typedef const char *(__thiscall* oIsInGame)(void*);
		return getvfunc<oIsInGame>(this, 35)(this);
	}
	const VMatrix& WorldToScreenMatrix() {
		typedef VMatrix&(__thiscall* OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 37)(this);
	}

	bool IsTakingScreenshot()
	{
		typedef bool(__thiscall* oIsTakingScreenshot)(void*);
		return getvfunc<oIsTakingScreenshot>(this, 92)(this);
	}

	unsigned int GetEngineBuildNumber()
	{
		typedef unsigned int(__thiscall* OriginalFn)(PVOID);
		return getvfunc< OriginalFn >(this, 104)(this);
	}

	void ExecuteClientCmd(const char* szCmdString)
	{
		typedef void(__thiscall* oExecuteClientCmd)(void*, const char*);
		return getvfunc<oExecuteClientCmd>(this, 108)(this, szCmdString);
	}

	void ClientCmd_Unrestricted(char  const* cmd)
	{
		typedef void(__thiscall* ofunc)(PVOID, const char*, char);
		return getvfunc<ofunc>(this, 114)(this, cmd, 0);
	}
};
enum soundlevel_t
{
	SNDLVL_NONE = 0,

	SNDLVL_20dB = 20,			// rustling leaves
	SNDLVL_25dB = 25,			// whispering
	SNDLVL_30dB = 30,			// library
	SNDLVL_35dB = 35,
	SNDLVL_40dB = 40,
	SNDLVL_45dB = 45,			// refrigerator

	SNDLVL_50dB = 50,	// 3.9	// average home
	SNDLVL_55dB = 55,	// 3.0

	SNDLVL_IDLE = 60,	// 2.0	
	SNDLVL_60dB = 60,	// 2.0	// normal conversation, clothes dryer

	SNDLVL_65dB = 65,	// 1.5	// washing machine, dishwasher
	SNDLVL_STATIC = 66,	// 1.25

	SNDLVL_70dB = 70,	// 1.0	// car, vacuum cleaner, mixer, electric sewing machine

	SNDLVL_NORM = 75,
	SNDLVL_75dB = 75,	// 0.8	// busy traffic

	SNDLVL_80dB = 80,	// 0.7	// mini-bike, alarm clock, noisy restaurant, office tabulator, outboard motor, passing snowmobile
	SNDLVL_TALKING = 80,	// 0.7
	SNDLVL_85dB = 85,	// 0.6	// average factory, electric shaver
	SNDLVL_90dB = 90,	// 0.5	// screaming child, passing motorcycle, convertible ride on frw
	SNDLVL_95dB = 95,
	SNDLVL_100dB = 100,	// 0.4	// subway train, diesel truck, woodworking shop, pneumatic drill, boiler shop, jackhammer
	SNDLVL_105dB = 105,			// helicopter, power mower
	SNDLVL_110dB = 110,			// snowmobile drvrs seat, inboard motorboat, sandblasting
	SNDLVL_120dB = 120,			// auto horn, propeller aircraft
	SNDLVL_130dB = 130,			// air raid siren

	SNDLVL_GUNFIRE = 140,	// 0.27	// THRESHOLD OF PAIN, gunshot, jet engine
	SNDLVL_140dB = 140,	// 0.2

	SNDLVL_150dB = 150,	// 0.2

	SNDLVL_180dB = 180,			// rocket launching

								// NOTE: Valid soundlevel_t values are 0-255.
								//       256-511 are reserved for sounds using goldsrc compatibility attenuation.
};
class IRecipientFilter;


class IEngineSound
{
public:

	void GetActiveSounds(CUtlVector<SndInfo_t>& sndlist)
	{
		typedef void(*oGetActiveSounds)(void*, CUtlVector<SndInfo_t>& sndlist);
		getvfunc<oGetActiveSounds>(this, 18)(this, sndlist);
	}

	////Virtual funcs

	//// Precache a particular sample
	//virtual bool PrecacheSound(const char *pSample, bool bPreload = false, bool bIsUISound = false) = 0;
	//virtual bool IsSoundPrecached(const char *pSample) = 0;
	//virtual void PrefetchSound(const char *pSample) = 0;
	//virtual bool IsLoopingSound(const char *pSample) = 0;

	//// Just loads the file header and checks for duration (not hooked up for .mp3's yet)
	//// Is accessible to server and client though
	//virtual float GetSoundDuration(const char *pSample) = 0;

	//// Pitch of 100 is no pitch shift.  Pitch > 100 up to 255 is a higher pitch, pitch < 100
	//// down to 1 is a lower pitch.   150 to 70 is the realistic range.
	//// EmitSound with pitch != 100 should be used sparingly, as it's not quite as
	//// fast (the pitchshift mixer is not native coded).

	//// NOTE: setting iEntIndex to -1 will cause the sound to be emitted from the local
	//// player (client-side only)
	//virtual int EmitSound1(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
	//	float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
	//	const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1) = 0;

	//virtual int EmitSound2(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
	//	float flVolume, float flAttenuation, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
	//	const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1) = 0;

	//virtual void EmitSentenceByIndex(IRecipientFilter& filter, int iEntIndex, int iChannel, int iSentenceIndex,
	//	float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
	//	const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1) = 0;

	//virtual void StopSound(int iEntIndex, int iChannel, const char *pSample, unsigned int nSoundEntryHash) = 0;
};

