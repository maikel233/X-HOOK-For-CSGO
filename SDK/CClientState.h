#pragma once
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



class INetChannelInfo
{
public:
	__int32 vtable; //0x0000 
	//Netmsgbinder* msgbinder1; //0x0004 
	//Netmsgbinder* msgbinder2; //0x0008 
	//Netmsgbinder* msgbinder3; //0x000C 
//	Netmsgbinder* msgbinder4; //0x0010 
	unsigned char m_bProcessingMessages; //0x0014 
	unsigned char m_bShouldDelete; //0x0015 
	char pad_0x0016[0x2]; //0x0016
	__int32 m_nOutSequenceNr; //0x0018 
	__int32 m_nInSequenceNr; //0x001C 
	__int32 m_nOutSequenceNrAck; //0x0020 
	__int32 m_nOutReliableState; //0x0024 
	__int32 m_nInReliableState; //0x0028 
	__int32 m_nChokedPackets; //0x002C 

	//bf_write m_StreamReliable; //0x0030 
	/*CUtlMemory*/ char m_ReliableDataBuffer[12]; //0x0048 
	//bf_write m_StreamUnreliable; //0x0054 
	/*CUtlMemory*/ char m_UnreliableDataBuffer[12]; //0x006C 
	//bf_write m_StreamVoice; //0x0078 
	/*CUtlMemory*/char m_VoiceDataBuffer[12]; //0x0090 
	__int32 m_Socket; //0x009C 
	__int32 m_StreamSocket; //0x00A0 
	__int32 m_MaxReliablePayloadSize; //0x00A4 
	char pad_0x00A8[0x4]; //0x00A8
//	netadr_t remote_address; //0x00AC 
	char dylanpadding[84]; //padding added by dylan
	float last_received; //0x00B8  //dylan found 0x10c
						 //char pad_0x00BC[0x4]; //0x00BC
	double /*float*/ connect_time; //0x00C0 //dylan found 0x110
								   //char pad_0x00C4[0x4]; //0x00C4
	__int32 m_Rate; //0x00C8  //dylan found 0x118
	/*float*/double m_fClearTime; //0x00CC  //dylan found 0x120
	char pad_0x00D0[0x8]; //0x00D0
	char m_WaitingList[48];
	//CUtlVector m_WaitingList[0]; //0x00D8 
	//CUtlVector m_WaitingList[1]; //0x00EC 
	//char pad_0x0100[0x4120]; //0x0100
	char pad_0x0100[0x40F0]; //dylan changed
	__int32 m_PacketDrop; //0x4220  //dylan found 0x4250
	char m_Name[32]; //0x4224 
	__int32 m_ChallengeNr; //0x4244 
	float m_Timeout; //0x4248 //dylan found 0x4278
//	INetChannelHandler* m_MessageHandler; //0x424C 
	/*CUtlVector*/char m_NetMessages[14]; //0x4250 
	__int32 dylanUnknown;
	void* m_pDemoRecorder; //0x4264 
	__int32 m_nQueuedPackets; //0x4268  //dylan found 0x4298
	float m_flInterpolationAmount; //0x426C //dylan found 0x429c
	float m_flRemoteFrameTime; //0x4270 //dylan found 0x42a0
	float m_flRemoteFrameTimeStdDeviation; //0x4274  //dylan found 0x42a4
	float m_flRemoteFrameTimeUnknown; //dylan found 0x42a8
	__int32 m_nMaxRoutablePayloadSize; //0x4278  //dylan found 0x42ac
	__int32 m_nSplitPacketSequence; //0x427C  //dylan found 0x42b0
	char pad_0x4280[0x14]; //0x4280


	bool Transmit(bool a);

	virtual const char  *GetName(void) const = 0;	// get channel name
	virtual const char  *GetAddress(void) const = 0; // get channel IP address as string
	virtual float		GetTime(void) const = 0;	// current net time
	virtual float		GetTimeConnected(void) const = 0;	// get connection time in seconds
	virtual int			GetBufferSize(void) const = 0;	// netchannel packet history size
	virtual int			GetDataRate(void) const = 0; // send data rate in byte/sec

	virtual bool		IsLoopback(void) const = 0;	// true if loopback channel
	virtual bool		IsTimingOut(void) const = 0;	// true if timing out
	virtual bool		IsPlayback(void) const = 0;	// true if demo playback

	virtual float		GetLatency(int flow) const = 0;	 // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency(int flow) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss(int flow) const = 0;	 // avg packet loss[0..1]
	virtual float		GetAvgChoke(int flow) const = 0;	 // avg packet choke[0..1]
	virtual float		GetAvgData(int flow) const = 0;	 // data flow in bytes/sec
	virtual float		GetAvgPackets(int flow) const = 0; // avg packets/sec
	virtual int			GetTotalData(int flow) const = 0;	 // total flow in/out in bytes
	virtual int			GetSequenceNr(int flow) const = 0;	// last send seq number
	virtual bool		IsValidPacket(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime(int flow, int frame_number) const = 0; // time when packet was send
	virtual int			GetPacketBytes(int flow, int frame_number, int group) const = 0; // group size of this packet
	virtual bool		GetStreamProgress(int flow, int *received, int *total) const = 0;  // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived(void) const = 0;	// get time since last recieved packet in seconds
	virtual	float		GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void		GetPacketResponseLatency(int flow, int frame_number, int *pnLatencyMsecs, int *pnChoke) const = 0;
	virtual void		GetRemoteFramerate(float *pflFrameTime, float *pflFrameTimeStdDeviation) const = 0;

	virtual float		GetTimeoutSeconds() const = 0;
}; //Size: 0x431A
class INetChannel : public INetChannelInfo
{
public:
	virtual	~INetChannel(void) {};

	virtual void	SetDataRate(float rate) = 0;
	virtual bool	RegisterMessage(void *msg) = 0;
	virtual bool	StartStreaming(unsigned int challengeNr) = 0;
	virtual void	ResetStreaming(void) = 0;
	virtual void	SetTimeout(float seconds) = 0;
	virtual void	SetDemoRecorder(void *recorder) = 0;
	virtual void	SetChallengeNr(unsigned int chnr) = 0;

	virtual void	Reset(void) = 0;
	virtual void	Clear(void) = 0;
	virtual void	Shutdown(const char *reason) = 0;

	virtual void	ProcessPlayback(void) = 0;
	virtual bool	ProcessStream(void) = 0;
	virtual void	ProcessPacket(struct netpacket_s* packet, bool bHasHeader) = 0;

	virtual bool	SendNetMsg(bool bForceReliable = false, bool bVoice = false) = 0;
#ifdef POSIX
	FORCEINLINE bool SendNetMsg(INetMessage const &msg, bool bForceReliable = false, bool bVoice = false) { return SendNetMsg(*((INetMessage *)&msg), bForceReliable, bVoice); }
#endif
	virtual bool	SendData(bf_write &msg, bool bReliable = true) = 0;
	virtual bool	SendFile(const char *filename, unsigned int transferID) = 0;
	virtual void	DenyFile(const char *filename, unsigned int transferID) = 0;
	virtual void	RequestFile_OLD(const char *filename, unsigned int transferID) = 0;	// get rid of this function when we version the 
	virtual void	SetChoked(void) = 0;
	virtual int		SendDatagram(bf_write *data) = 0;
	virtual bool	Transmit(bool onlyReliable = false) = 0;

	virtual void	GetRemoteAddress(void) const = 0;
	virtual void *GetMsgHandler(void) const = 0;
	virtual int				GetDropNumber(void) const = 0;
	virtual int				GetSocket(void) const = 0;
	virtual unsigned int	GetChallengeNr(void) const = 0;
	virtual void			GetSequenceData(int &nOutSequenceNr, int &nInSequenceNr, int &nOutSequenceNrAck) = 0;
	virtual void			SetSequenceData(int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck) = 0;

	virtual void	UpdateMessageStats(int msggroup, int bits) = 0;
	virtual bool	CanPacket(void) const = 0;
	virtual bool	IsOverflowed(void) const = 0;
	virtual bool	IsTimedOut(void) const = 0;
	virtual bool	HasPendingReliableData(void) = 0;

	virtual void	SetFileTransmissionMode(bool bBackgroundMode) = 0;
	virtual void	SetCompressionMode(bool bUseCompression) = 0;
	virtual unsigned int RequestFile(const char *filename) = 0;
	virtual float	GetTimeSinceLastReceived(void) const = 0;	// get time since last received packet in seconds

	virtual void	SetMaxBufferSize(bool bReliable, int nBytes, bool bVoice = false) = 0;

	virtual bool	IsNull() const = 0;
	virtual int		GetNumBitsWritten(bool bReliable) = 0;
	virtual void	SetInterpolationAmount(float flInterpolationAmount) = 0;
	virtual void	SetRemoteFramerate(float flFrameTime, float flFrameTimeStdDeviation) = 0;

	// Max # of payload bytes before we must split/fragment the packet
	virtual void	SetMaxRoutablePayloadSize(int nSplitSize) = 0;
	virtual int		GetMaxRoutablePayloadSize() = 0;

	virtual int		GetProtocolVersion() = 0;
};
class CClientState //: public CBaseHudChat
{
public:
	char pad_0000[156]; //0x0000
	class INetChannel* m_pNetChannel; //0x0094
	//char pad_0098[16]; //0x0098
	char pad_00A0[116]; //0x00A0
	//unsigned int m_nRetryNumber; //0x00A8
	//char pad_00AC[84]; //0x00AC
	//int m_nSignonState; //0x0100
	//char pad_0104[8]; //0x0104
	float m_flNextCmdTime; //0x010C
	int m_nServerCount; //0x0110
	int m_nCurrentSequence; //0x0114
	char pad_0120[76]; //0x0120
	int m_nServerTick; //0x0163
	int m_nClientTick; //0x0167
	int m_nDeltaTick; //0x016B
	bool paused; //0x0178
	char pad_0179[15]; //0x0179
	//int m_nViewEntity; //0x0173
	//char pad_0177[8]; //0x0177
	char m_szLevelName[260]; //0x017F
	char m_szLevelNameShort[40]; //0x0283
//	char pad_02AB[18940]; //0x02AB
//	char pad_0x2DA0; //0x4CA7
	char pad_02B4[212]; //0x02B4
	uint32_t max_players; //0x0388
	char pad_038C[18844]; //0x038C
	int lastoutgoingcommand; //0x4CA8
	int chokedcommands; //0x4CAC
	int last_command_ack; //0x4CB0
	int command_ack; //0x4CB4
	int m_nSoundSequence; //0x4CB8
	char pad_4D38[80]; //0x4D38
	bool ishltv; //0x4CC4
	CBaseHudChat			*m_pChatElement;

	void ForceFullUpdate()
	{
		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x174) = -1;
	}

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