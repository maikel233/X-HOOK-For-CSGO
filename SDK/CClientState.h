#pragma once

#include "VirtualMethod.h"

class CBaseHudChat
{
public:
	void ChatPrintf(int iPlayerIndex, int iFilter, const char* fmt, ...)
	{
		getvfunc<void(__stdcall*)(void*, int, int, const char*, ...)>(this, 26)(this, iPlayerIndex, iFilter, fmt);
	}
};


class ECommandMsgBoxSlot;

class CGameUI {
public:
	virtual void Initialize(void * (*)(char const*, int *)) = 0;
	virtual void PostInit(void) = 0;
	virtual void Connect(void * (*)(char const*, int *)) = 0;
	virtual void Start(void) = 0;
	virtual void Shutdown(void) = 0;
	virtual void RunFrame(void) = 0;
	virtual void OnGameUIActivated(void) = 0;
	virtual void OnGameUIHidden(void) = 0;
	virtual void OLD_OnConnectToServer(char const*, int, int) = 0;
	virtual void OnDisconnectFromServer_OLD(unsigned char, char const*) = 0;
	virtual void OnLevelLoadingStarted(char const*, bool) = 0;
	virtual void OnLevelLoadingFinished(bool, char const*, char const*) = 0;
	virtual void StartLoadingScreenForCommand(char const*) = 0;
	virtual void StartLoadingScreenForKeyValues(KeyValues *) = 0;
	virtual void UpdateProgressBar(float, char const*, bool) = 0;
	virtual void SetShowProgressText(bool) = 0;
	virtual void UpdateSecondaryProgressBar(float, wchar_t const*) = 0;
	virtual void SetProgressLevelName(char const*) = 0;
	virtual void ShowMessageDialog(unsigned int, IVPanel *) = 0;
	virtual void ShowMessageDialog(char const*, char const*) = 0;
	virtual void CreateCommandMsgBox(char const*, char const*, bool, bool, char const*, char const*, char const*, char const*) = 0;
	virtual void CreateCommandMsgBoxInSlot(ECommandMsgBoxSlot, char const*, char const*, bool, bool, char const*, char const*, char const*, char const*) = 0;
	virtual void SetLoadingBackgroundDialog(unsigned long long) = 0;
	virtual void OnConnectToServer2(char const*, int, int, int) = 0;
	virtual void SetProgressOnStart(void) = 0;
	virtual void OnDisconnectFromServer(unsigned char) = 0;
	virtual void NeedConnectionProblemWaitScreen(void) = 0;
	virtual void ShowPasswordUI(char const*) = 0;
	virtual void LoadingProgressWantsIsolatedRender(bool) = 0;
	virtual bool IsPlayingFullScreenVideo(void) = 0;
	virtual bool IsTransitionEffectEnabled(void) = 0;
	virtual bool IsInLevel(void) = 0;
	virtual void RestoreTopLevelMenu(void) = 0;
	virtual void StartProgressBar(void) = 0;
	virtual void ContinueProgressBar(float, bool) = 0;
	virtual void StopProgressBar(bool, char const*, char const*) = 0;
	virtual void SetProgressBarStatusText(char const*, bool) = 0;
	virtual void SetSecondaryProgressBar(float) = 0;
	virtual void SetSecondaryProgressBarText(wchar_t const*) = 0;
};



class NetworkChannel {
public:
	VIRTUAL_METHOD(float, getLatency, 9, (int flow), (this, flow))
	std::byte pad[24];
	int OutSequenceNr;
	int InSequenceNr;
	int OutSequenceNrAck;
	int OutReliableState;
	int InReliableState;
	int chokedPackets;
};

//class INetChannel : public INetChannelInfo
//{
//public:
//	virtual	~INetChannel(void) {};
//
//	virtual void	SetDataRate(float rate) = 0;
//	virtual bool	RegisterMessage(void *msg) = 0;
//	virtual bool	StartStreaming(unsigned int challengeNr) = 0;
//	virtual void	ResetStreaming(void) = 0;
//	virtual void	SetTimeout(float seconds) = 0;
//	virtual void	SetDemoRecorder(void *recorder) = 0;
//	virtual void	SetChallengeNr(unsigned int chnr) = 0;
//
//	virtual void	Reset(void) = 0;
//	virtual void	Clear(void) = 0;
//	virtual void	Shutdown(const char *reason) = 0;
//
//	virtual void	ProcessPlayback(void) = 0;
//	virtual bool	ProcessStream(void) = 0;
//	virtual void	ProcessPacket(struct netpacket_s* packet, bool bHasHeader) = 0;
//
//	virtual bool	SendNetMsg(bool bForceReliable = false, bool bVoice = false) = 0;
//#ifdef POSIX
//	FORCEINLINE bool SendNetMsg(INetMessage const &msg, bool bForceReliable = false, bool bVoice = false) { return SendNetMsg(*((INetMessage *)&msg), bForceReliable, bVoice); }
//#endif
//	virtual bool	SendData(bf_write &msg, bool bReliable = true) = 0;
//	virtual bool	SendFile(const char *filename, unsigned int transferID) = 0;
//	virtual void	DenyFile(const char *filename, unsigned int transferID) = 0;
//	virtual void	RequestFile_OLD(const char *filename, unsigned int transferID) = 0;	// get rid of this function when we version the 
//	virtual void	SetChoked(void) = 0;
//	virtual int		SendDatagram(bf_write *data) = 0;
//	virtual bool	Transmit(bool onlyReliable = false) = 0;
//
//	virtual void	GetRemoteAddress(void) const = 0;
//	virtual void *GetMsgHandler(void) const = 0;
//	virtual int				GetDropNumber(void) const = 0;
//	virtual int				GetSocket(void) const = 0;
//	virtual unsigned int	GetChallengeNr(void) const = 0;
//	virtual void			GetSequenceData(int &nOutSequenceNr, int &nInSequenceNr, int &nOutSequenceNrAck) = 0;
//	virtual void			SetSequenceData(int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck) = 0;
//
//	virtual void	UpdateMessageStats(int msggroup, int bits) = 0;
//	virtual bool	CanPacket(void) const = 0;
//	virtual bool	IsOverflowed(void) const = 0;
//	virtual bool	IsTimedOut(void) const = 0;
//	virtual bool	HasPendingReliableData(void) = 0;
//
//	virtual void	SetFileTransmissionMode(bool bBackgroundMode) = 0;
//	virtual void	SetCompressionMode(bool bUseCompression) = 0;
//	virtual unsigned int RequestFile(const char *filename) = 0;
//	virtual float	GetTimeSinceLastReceived(void) const = 0;	// get time since last received packet in seconds
//
//	virtual void	SetMaxBufferSize(bool bReliable, int nBytes, bool bVoice = false) = 0;
//
//	virtual bool	IsNull() const = 0;
//	virtual int		GetNumBitsWritten(bool bReliable) = 0;
//	virtual void	SetInterpolationAmount(float flInterpolationAmount) = 0;
//	virtual void	SetRemoteFramerate(float flFrameTime, float flFrameTimeStdDeviation) = 0;
//
//	// Max # of payload bytes before we must split/fragment the packet
//	virtual void	SetMaxRoutablePayloadSize(int nSplitSize) = 0;
//	virtual int		GetMaxRoutablePayloadSize() = 0;
//
//	virtual int		GetProtocolVersion() = 0;
//};
class CClientState //: public CBaseHudChat
{
public:
	std::byte		pad0[0x9C];
	class NetworkChannel* m_pNetChannel;
	uint32_t	challenge_nr;
	std::byte		pad1[0x64];
	uint32_t	m_nSignonState;
	std::byte		pad2[0x8];
	float		m_flNextCmdTime;
	uint32_t	server_count;
	uint32_t	current_sequence;
	std::byte		pad3[0x54];
	uint32_t	m_nDeltaTick;
	bool		paused;
	std::byte		pad4[0x7];
	uint32_t	m_nViewEntity;
	uint32_t	player_slot;
	char m_szLevelName[260]; 
	char m_szLevelNameShort[80];
	char		group_name[80];
	std::byte		pad5[0x5C];
	uint32_t	max_players;
	std::byte		pad6[0x4984];
	float		last_server_tick_time;
	bool		in_simulation;
	std::byte		pad7[0x3];
	uint32_t	old_tick_count;
	float		tick_remainder;
	float		frame_time;
	int		lastoutgoingcommand;
	int		chokedcommands;
	int		last_command_ack;
	int		command_ack;
	int		m_nSoundSequence;
	void ForceFullUpdate()
	{
		m_nDeltaTick;
	}
	std::byte		pad8[0x50];
	Vector3		view_angles;
	//std::byte		pad9[0xD0];
	//EventInfo* pEvents;

}; //Size: 0x4CC5

class CSGameRulesProxy
{
public:
	
		bool IsValveDS()
		{
			return *(bool*)((uintptr_t)this + offsets.DT_CSGameRulesProxy.m_bIsValveDS);
		}

		bool IsBombDropped()
		{
			return *(bool*)((uintptr_t)this + offsets.DT_CSGameRulesProxy.m_bBombDropped);
		}

		bool IsBombPlanted()
		{
			return *(bool*)((uintptr_t)this + offsets.DT_CSGameRulesProxy.m_bBombPlanted);
		}

		bool IsFreezeTime()
		{
			return *(bool*)((uintptr_t)this + offsets.DT_CSGameRulesProxy.m_bFreezePeriod);
		}

		int IsCurrentRound()
		{
			return *(int*)((uintptr_t)this + offsets.DT_CSGameRulesProxy.m_totalRoundsPlayed);
		}
};