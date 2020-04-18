#include "Shared.h"
#include "XorStr.hpp"
#include "IRC.h"
#pragma comment(lib,"Wininet.lib")
#pragma comment(lib,"gdiplus.lib")

std::string base64_encode(unsigned char const*, unsigned int len);
std::string base64_decode(std::string const& s);



struct Info_t
{
	char* Name;
	DWORD Volume;
	DWORD Mac;
	DWORD Extra;
	char* User;
};
enum Info_e
{
	KEVINPC,
	JesseLaptop,
	Nils,
	Maikel,
	Mark,
	LAPTOPDMUMTDG5,
	Evocloud,
	DoggyLaptop,
	MaikelLAP,
};
class cSecurity
{
public:
	cSecurity();
	char* User;
	bool RunSecurityChecks();
	bool Trampoline(bool, int);
	void Thread();
	bool SendHWID = false;
	std::string Message;
	bool SendResponse(std::string);
	std::string GetResponseHWID();
	std::string GetResponsePASS();
	bool pwmatch(char* Pass);

	void timerstart();

	int UniqueId();

	void verify(int id);

	bool isVerified();
private:
	inline bool Authenticate();
	inline bool Detour(bool, int);
	inline bool BackroundWorker(bool);
	static inline void InvokeBSOD();
	static inline const char* getMachineName();
	static inline uint16_t getCpuHash();
	static inline uint16_t getVolumeHash();
	static inline void getMacHash(uint16_t& mac1, uint16_t& mac2);
	static inline uint16_t hashMacAddress(PIP_ADAPTER_INFO info);
	//decs
	uint16_t buf1, buf2;


	//std::string GetResponse();
};
extern cSecurity* g_pSecurity;


//class CWinsock
//{
//public:
//	CWinsock(VOID);
//	~CWinsock(VOID);
//
//	VOID InitWinsock(VOID);
//	VOID Connect(const char* pszServerUrl);
//	VOID SendHttpGet(const char* pszFileUrl, std::string &sDestBuffer);
//	VOID Disconnect(VOID);
//	VOID CleanUp(VOID);
//private:
//	SOCKET m_ConnectedSocket;
//	const char* m_pszConnectedUrl;
//
//}; extern CWinsock g_Winso