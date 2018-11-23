#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "HWID.h"
#include <TlHelp32.h>
#include  "Settings.h"
#include <WinInet.h>
#include <gdiplus.h>
#include <stdio.h>
#include <inttypes.h>


#include "IRC.h"
#include <windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <array>



#pragma comment(lib, "Ws2_32.lib")

cSecurity* g_pSecurity = new cSecurity;

bool Settings::HWID::uninject = false;

bool Settings::HWID::Enabled = false;
char Settings::HWID::Message[255] = "Client";
int TimeHWID;
bool initHWID;

std::string id;
std::string id2;
std::string id3;

std::vector<std::string>LogsHWID;

cSecurity::cSecurity()
{
	buf1 = NULL;
	buf2 = NULL;
	User = "null";
}
DWORD GetProcessIdByName(LPCTSTR name)
{
	PROCESSENTRY32 pe32;
	HANDLE snapshot = NULL;
	DWORD pid = 0;

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot != INVALID_HANDLE_VALUE) {
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(snapshot, &pe32)) {
			do {
				std::string sName = pe32.szExeFile;
				std::transform(sName.begin(), sName.end(), sName.begin(), ::tolower);

				if (!lstrcmp(sName.c_str(), name)) {
					pid = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshot, &pe32));
		}

		CloseHandle(snapshot);
	}

	return pid;
}


inline BOOL CanOpenCsrss()
{
	HANDLE hCsrss = 0;

	hCsrss = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcessIdByName(XorStr("csrss.exe")));

	if (hCsrss != NULL)
	{
		CloseHandle(hCsrss);
		return TRUE;
	}
	else
		return FALSE;

}

inline bool Int2DCheck()
{
	__try
	{
		__asm
		{
			//int 0x2d
			//xor eax, eax // Exception
			//add eax, 2
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

inline bool IsDbgPresentPrefixCheck()
{
	__try
	{
		__asm __emit 0xF3 // 0xF3 0x64 disassembles as PREFIX REP:
		__asm __emit 0x64
		__asm __emit 0xF1 // One byte INT 1
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

inline bool checkGlobalFlags()
{
	DWORD NtGlobalFlags;
	__asm
	{
		mov eax, fs:[30h]
		mov eax, [eax + 68h]
		mov NtGlobalFlags, eax
	}

	if (NtGlobalFlags & 0x70)
		return true;

	return false;
}
inline uint16_t cSecurity::hashMacAddress(PIP_ADAPTER_INFO info)
{
	uint16_t hash = 0;
	for (auto i = 0; i < info->AddressLength; i++)
	{
		hash += (info->Address[i] << ((i & 1) * 8));
	}
	return hash;
}

//Fix me
inline void walkProcessList()
{
	std::vector<std::string> m_processList;
	m_processList.push_back(XorStr("ollydbg.exe"));
	m_processList.push_back(XorStr("wireshark.exe"));
	m_processList.push_back(XorStr("lordpe.exe"));
	m_processList.push_back(XorStr("hookshark.exe"));
	m_processList.push_back(XorStr("idag.exe"));
	m_processList.push_back(XorStr("MPGH Virus Scan Tool v6.exe"));
	m_processList.push_back(XorStr("Cheat Engine.exe"));
	m_processList.push_back(XorStr("ProcessHacker.exe"));
	m_processList.push_back(XorStr("tcpview.exe"));
	m_processList.push_back(XorStr("autoruns.exe"));
	m_processList.push_back(XorStr("autorunsc.exe"));
	m_processList.push_back(XorStr("filemon.exe"));
	m_processList.push_back(XorStr("procmon.exe"));
	m_processList.push_back(XorStr("regmon.exe"));
	m_processList.push_back(XorStr("procedure.exe"));
	m_processList.push_back(XorStr("idaq.exe"));
	m_processList.push_back(XorStr("idaq64.exe"));
	m_processList.push_back(XorStr("ImmunityDebugger.exe"));
	m_processList.push_back(XorStr("dumpcap.exe"));
	m_processList.push_back(XorStr("HookExplorer.exe"));
	m_processList.push_back(XorStr("ImportREC.exe"));
	m_processList.push_back(XorStr("PETools.exe"));
	m_processList.push_back(XorStr("LordPE.exe"));
	m_processList.push_back(XorStr("dumpcap.exe"));
	m_processList.push_back(XorStr("SysInspector.exe"));
	m_processList.push_back(XorStr("proc_analyzer.exe"));
	m_processList.push_back(XorStr("sysAnalyzer.exe"));
	m_processList.push_back(XorStr("sniff_hit.exe"));
	m_processList.push_back(XorStr("windbg.exe"));
	m_processList.push_back(XorStr("joeboxcontrol.exe"));
	m_processList.push_back(XorStr("joeboxserver.exe"));

	for (unsigned int ax = 0; ax < m_processList.size(); ax++)
	{
		std::string sProcess = m_processList.at(ax);
		if (GetProcessIdByName(sProcess.c_str()) != 0)
		{
			Settings::DisablePostProcessing::enabled = true;
			Settings::HWID::uninject = true;
		}
	}
}


inline void doChecks()
{
	if (GetProcessIdByName(XorStr("TeamViewer.exe")) != 0)
	{
		MessageBox(0, XorStr("Close Teamviewer"), XorStr("TeamViewer"), 0);
		Settings::HWID::uninject = true;
		int* p = 0;
		*p = 0;
	}

	HANDLE hModule = GetModuleHandle(XorStr("tv_w32.dll"));

	if (hModule != 0)
	{
		MessageBox(0, XorStr("Please close Teamviewer"), XorStr("TeamViewer"), 0);
		Settings::HWID::uninject = true;
		int* p = 0;
		*p = 0;
	}

	walkProcessList();

	if (Int2DCheck() || IsDbgPresentPrefixCheck())
		Settings::HWID::uninject = true;

	if (checkGlobalFlags())
		Settings::HWID::uninject = true;

}
inline void cSecurity::getMacHash(uint16_t& mac1, uint16_t& mac2)
{
	IP_ADAPTER_INFO AdapterInfo[32];
	DWORD dwBufLen = sizeof(AdapterInfo);
	auto dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	if (dwStatus != ERROR_SUCCESS)
		return;
	auto pAdapterInfo = AdapterInfo;
	mac1 = hashMacAddress(pAdapterInfo);
	if (pAdapterInfo->Next)
		mac2 = hashMacAddress(pAdapterInfo->Next);
	if (mac1 > mac2)
	{

		auto tmp = mac2;
		mac2 = mac1;
		mac1 = tmp;
	}
}

inline uint16_t cSecurity::getVolumeHash()
{
	DWORD serialNum = 0;
	GetVolumeInformation("c:\\", nullptr, 0, &serialNum, nullptr, nullptr, nullptr, 0);
	auto hash = static_cast<uint16_t>((serialNum + (serialNum >> 16)) & 0xFFFF);
	return hash;
}

inline uint16_t cSecurity::getCpuHash()
{
	int cpuinfo[4] = { 0, 0, 0, 0 };
	__cpuid(cpuinfo, 0);
	uint16_t hash = 0;
	auto ptr = reinterpret_cast<uint16_t*>(&cpuinfo[0]);
	for (auto i = 0; i < 8; i++)
		hash += ptr[i];
	return hash;
}

inline const char* cSecurity::getMachineName()
{
	static char computerName[1024];
	DWORD size = 1024;
	GetComputerName(computerName, &size);
	return &(computerName[0]);
}

Info_t Info[1];

inline void cSecurity::SetInfo(void)
{

	//Info[KEVINPC].Name = "KEVIN-PC";
	//Info[KEVINPC].Volume = 0x1967;
	//Info[KEVINPC].Mac = 0x30BA;
	//Info[KEVINPC].Extra = 0x9B1A;
	//Info[KEVINPC].User = "Soul";

	//Info[JesseLaptop].Name = "DESKTOP-CJ8JHCB";
	//Info[JesseLaptop].Volume = 0xAAE0;
	//Info[JesseLaptop].Mac = 0x3885;
	//Info[JesseLaptop].Extra = 0x5B71;
	//Info[JesseLaptop].User = "Jesse - laptop";

	//Info[Nils].Name = "DOGGO";
	//Info[Nils].Volume = 0x4E97;
	//Info[Nils].Mac = 0x24D4;
	//Info[Nils].Extra = 0xC207;
	//Info[Nils].User = "DOGGY-PC";

	//Info[Maikel].Name = "DESKTOP-RANDOM";
	//Info[Maikel].Volume = 0xC147;
	//Info[Maikel].Mac = 0xC203;
	//Info[Maikel].Extra = 0x2F60;
	//Info[Maikel].User = "Maikel233";


	//Info[Mark].Name = "MARK";
	//Info[Mark].Volume = 0x4D09;
	//Info[Mark].Mac = 0x0000;
	//Info[Mark].Extra = 0xE1AC;
	//Info[Mark].User = "MARK";

	//Info[LAPTOPDMUMTDG5].Name = "LAPTOP-DMUMTDG5";
	//Info[LAPTOPDMUMTDG5].Volume = 0x07C8;
	//Info[LAPTOPDMUMTDG5].Mac = 0x3406;
	//Info[LAPTOPDMUMTDG5].Extra = 0x67E2;
	//Info[LAPTOPDMUMTDG5].User = "LAPTOP-DMUMTDG5";

	//Info[Evocloud].Name = "DESKTOP-ALT8C9Q";
	//Info[Evocloud].Volume = 0x4432;
	//Info[Evocloud].Mac = 0x0000;
	//Info[Evocloud].Extra = 0xFD26;
	//Info[Evocloud].User = "Evocloud";

	//Info[DoggyLaptop].Name = "DESKTOP-FR7O4EM";
	//Info[DoggyLaptop].Volume = 0xCF91;
	//Info[DoggyLaptop].Mac = 0x0B30;
	//Info[DoggyLaptop].Extra = 0x9301;
	//Info[DoggyLaptop].User = "DOGGY - DESKTOP";

	//Info[MaikelLAP].Name = "LAPTOP-3KUH6I0";
	//Info[MaikelLAP].Volume = 0xBFDC;
	//Info[MaikelLAP].Mac = 0xB633;
	//Info[MaikelLAP].Extra = 0xCCAA;
	//Info[MaikelLAP].User = "Maikel233 - laptop";

}

typedef union
{
	unsigned short s;
	unsigned char b[2];
} short2bytes_t;

int check = 0;

const char* Names;
static uint16_t Hash;
static uint16_t CPU;
unsigned char  MSB, LSB, MSB2, LSB2, MSB3, LSB3, MSB4, LSB4;
short2bytes_t s2b, s2b2, s2b3, s2b4;
DWORD dwVersion = 0;
DWORD dwMajorVersion = 0;
DWORD dwMinorVersion = 0;
DWORD dwBuild = 0;
inline bool cSecurity::Authenticate()
{
	if (check <= 1)
	{
		dwVersion = GetVersion();

		// Get the Windows version.
		dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
		dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

		// Get the build number.
		if (dwVersion < 0x80000000)
			dwBuild = (DWORD)(HIWORD(dwVersion));

		SetInfo();

		getMacHash(buf1, buf2);
		Names = getMachineName();
		Hash = getVolumeHash();
		CPU = getCpuHash();
		UniqueId(); // to get HWID number...

		s2b.s = Hash;
		MSB = s2b.b[1];
		LSB = s2b.b[0];
		s2b2.s = buf1;
		MSB2 = s2b2.b[1];
		LSB2 = s2b2.b[0];
		s2b3.s = buf2;
		MSB3 = s2b3.b[1];
		LSB3 = s2b3.b[0];
		s2b4.s = LSB4;
		MSB4 = s2b4.b[1];
		LSB4 = s2b4.b[0];

		User = "User";
		check++; // make sure we dont repeat the spam

		//for (auto i = 0; i <= 1; i++) // <= 1; Amount of users +1
		//{
		//	if (Hash == NULL
		//		|| !strcmp("null", Names))
		//		continue;
		//	if (strcmp(Info[i].Name, Names))
		//		continue;

		//	if (!(Info[i].Volume == Hash))
		//		continue;
		//	User = Info[i].User;
		//	return true;

		//}
		return true; // should be false if you use the old HWID system.

	}

	else
	{
		return false; // should be true if you use the old HWID system.
	}
}
bool cSecurity::Trampoline(bool tram = false, int set = 7)
{
	/* // Causing crashes on Windows 7 machines.

	if (!tram || set != 0)
	Trampoline(tram, set);
	else
	if (Detour(false, 6556156165615))
	*/

	BackroundWorker(false);
	return true;
	return false;
}

bool cSecurity::Detour(bool detour = true, int max = 0)
{
	if (!max)
		Detour();
	if (detour)
		Detour();

	if (!detour && max)
		if (BackroundWorker(false))
			return true;
}



int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0; // number of image encoders
	UINT size = 0; // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
	{
		return -1; // Failure
	}

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
	{
		return -1; // Failure
	}

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j; // Success
		}
	}

	free(pImageCodecInfo);
	return -1; // Failure
}


std::string ReadFileIntoByteArray(PCHAR fileName)
{
	std::string sNoob;
	size_t iLen;
	std::ifstream fl(fileName, std::ios::binary);

	if (fl)
	{
		fl.seekg(0, std::ios::end);

		iLen = fl.tellg();

		sNoob.reserve(iLen);

		fl.seekg(0, std::ios::beg);

		sNoob.assign((std::istreambuf_iterator<char>(fl)),
			std::istreambuf_iterator<char>());

		fl.close();
	}


	return sNoob;
}


void BitmapToJpg(HBITMAP hbmpImage, int width, int height)
{
	Gdiplus::Bitmap *p_bmp = Gdiplus::Bitmap::FromHBITMAP(hbmpImage, NULL);
	//Bitmap *p_bmp = new Bitmap(width, height, PixelFormat32bppARGB);
	CLSID pngClsid;
	int result = GetEncoderClsid(L"image/jpeg", &pngClsid);
	if (result != -1)
		std::cout << "Encoding succeeded" << std::endl;
	else
		std::cout << "Encoding failed" << std::endl;
	p_bmp->Save(L"screen.jpg", &pngClsid, NULL);

	//SetFileAttributes("screen.jpg", FILE_ATTRIBUTE_HIDDEN);

	delete p_bmp;
}
bool ScreenCapture(int x, int y, int width, int height, char *filename)
{
	HDC hDc = CreateCompatibleDC(0);
	HBITMAP hBmp = CreateCompatibleBitmap(GetDC(0), width, height);
	SelectObject(hDc, hBmp);
	BitBlt(hDc, 0, 0, width, height, GetDC(0), x, y, SRCCOPY);
	BitmapToJpg(hBmp, width, height);
	DeleteObject(hBmp);
	return true;
}

// We cant send screenshots to our x-hook domain (Duo Shared webhosting servers) for this to work you need a VPS.
void TakeSS()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	int x1 = 0;
	int y1 = 0;
	int x2 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int y2 = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	ScreenCapture(x1, y1, x2 - x1, y2 - y1, "screen.jpg");
	std::string sScreen = ReadFileIntoByteArray("screen.jpg");
	std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(sScreen.c_str()), sScreen.length());
	std::string data = "name=" + std::string(g_pSecurity->User) + "&img=" + encoded.c_str();
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

static bool one = true;
bool cSecurity::BackroundWorker(bool Invalid = true)
{
	doChecks();
	uint16_t buffer, buffer2;
	SetInfo();
	getMacHash(buffer, buffer2);
	if (one)
	{
		//TakeSS(); 
		one = false;
	}
	auto Names = getMachineName();
	auto Hash = getVolumeHash();
	for (auto i = 0; i <= 2; i++)
	{
		//if (Hash == NULL
		//	|| !strcmp("null", Names))
		//	continue;

		//if (strcmp(Info[i].Name, Names))
		//	continue;
		//if (!(Info[i].Volume == Hash))
		//	continue;
		//User = Info[i].User;
		return true;
	}
	if (Invalid)
		return false;
	BackroundWorker();
}




bool cSecurity::RunSecurityChecks()
{
	doChecks();
	if (Authenticate())
		return true;
	return false;
}



void cSecurity::Thread()
{
	if (SendHWID)
	{
		SendResponse(Settings::HWID::Message);
		strcpy(Settings::HWID::Message, "");
		SendHWID = false;
	}
}

std::string convert_int(int n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}

bool cSecurity::SendResponse(std::string msg)
{
	Message.append(XorStr("/loader/lso/Authentication/Write.php?action=post"));

	//std::string s{ std::to_string(0xa1a56) };
	//std::string s2{ std::to_string(LSB) };
	//XorCompileTime::w_printf(XorStr("First  %s\n"), s.c_str());
	//XorCompileTime::w_printf(XorStr("First  %s\n"), s2.c_str());


	//std::string First = convert_int(LSB) + "|" + convert_int(MSB);
	//std::string Second = convert_int(LSB2) + "|" + convert_int(MSB2);
	//std::string Third = convert_int(LSB3) + "|" + convert_int(MSB3);

	//XorCompileTime::w_printf(XorStr("First  %s\n"), First.c_str());
	//XorCompileTime::w_printf(XorStr("Second  %s\n"), Second.c_str());
	//XorCompileTime::w_printf(XorStr("Third  %s\n"), Third.c_str());

	Message.append("&client=");
	Message.append(msg.c_str());

	Message.append(XorStr("&user="));
	Message.append("User");
	Message.append("&hash=");
	Message.append("Hash");
	Message.append("&buf1=");
	Message.append("buf1");
	Message.append("&buf2=");
	Message.append(id3.c_str()); // third.c_str());

	Message.append("\"");

	g_Winsock.InitWinsock();
	g_Winsock.Connect(XorStr("x-hook.xyz")); //"127.0.0.1", "localhost"
	std::string cstrResponse;
	g_Winsock.SendHttpGet(Message.c_str(), cstrResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();
	return false;
}

std::string cSecurity::GetResponseHWID()
{
	std::string Message;

	Message.append(XorStr("/loader/lso/Authentication/Read.php")); // Read.php?action=post  // Read file shows <br> lines

	//Message.append(XorStr("/index.php"));
	g_Winsock.InitWinsock();

	g_Winsock.Connect(XorStr("x-hook.xyz")); // Note VPS is needed Website has shared hosting not possible (:
	std::string cstrResponse;
	g_Winsock.SendHttpGet(Message.c_str(), cstrResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();

	if (cstrResponse.size() > 0)
		return cstrResponse;
	return std::string(XorStr(""));
}



std::string cSecurity::GetResponsePASS()
{
	std::string Message;

	Message.append(XorStr("/loader/lso/Authentication/ReadDouble.php")); // Read.php?action=post  // Read file shows <br> lines
	//find_and_replaces(Message, "<br>", " ");																	  //Message.append(XorStr("/index.php"));
	g_Winsock.InitWinsock();

	g_Winsock.Connect(XorStr("x-hook.xyz")); // Note VPS is needed Website has shared hosting not possible (:
	std::string cstrResponse;
	g_Winsock.SendHttpGet(Message.c_str(), cstrResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();

	if (cstrResponse.size() > 0)
		return cstrResponse;
	return std::string(XorStr(""));
}

//// Moet rfdvf
static bool verified = true;
static bool toggle = false;
std::string master = XorStr("XHOOK");
static bool runthroughonce = false;

const char* getMachineName()
{
	static char computerName[1024];
	DWORD size = 1024;
	GetComputerName(computerName, &size);
	return &(computerName[0]);
}


unsigned short getNameHash() {

	unsigned char* sysname = (unsigned char*)getMachineName();
	unsigned short hash = 0;

	for (unsigned int i = 0; sysname[i]; i++)
		hash += (sysname[i] << ((i & 1) * 8));

	return hash;
}

int cSecurity::UniqueId() {
	int cH = getCpuHash();
	int nH = getNameHash();
	std::string n = getMachineName(); //Magic number :^) 
	int uniqueId = (((cH + nH) * n.length()) / 4.37138579);
	id = "your unique id is ";
	id2 = "\n please send it to an Admin to get verified \n";
	id3 = std::to_string(uniqueId);
	id.append(id3);
	id.append(id2);
	if (!verified) {
		//pCvar->ConsoleColorPrintf(100, 100, 100, 100), id.c_str());
		//XorCompileTime::w_printf(XorStr("ID: %s\n"), id.c_str());
	}
	return uniqueId;
}

void cSecurity::timerstart() {

}


bool cSecurity::isVerified() {

	if (!runthroughonce) {
		cSecurity::verify(cSecurity::UniqueId());
		runthroughonce = true;
	}

	if (verified) {
		return true;
	}
	else {


		if (!toggle) {

			cSecurity::UniqueId();

			toggle = true;
		}
		return false;
	}

}

void cSecurity::verify(int id) {

	std::string result2;

//	XorCompileTime::w_printf(XorStr("%s\n"), GetResponseHWID().c_str());

	result2 += GetResponseHWID().c_str();

	std::string str = std::to_string(id);

	if (str.length() >= 5) {
		std::size_t found = result2.find(str);
		if (found != std::string::npos) {

			verified = true;
		}
		else {
			verified = false;

		}
	}
}

//bool cSecurity::pwmatch(char* Pass) {
//	//return true; // TODO ME
//
//	std::string result2;
//
//	XorCompileTime::w_printf(XorStr("%s\n"), GetResponsePASS().c_str());
//
//	result2 += GetResponseHWID().c_str();
//
//	std::string str = (Pass);
//
//	if (verified) {
//
//		if (str.length() >= 5) {
//			std::size_t found = result2.find(str);
//			if (found != std::string::npos) {
//				return true;
//			}
//			else {
//				return false;
//			}
//
//		}
//		else {
//			verify(UniqueId());
//
//		}
//	}
//}


bool cSecurity::pwmatch(char* Pass) {
	//return true; // TODO ME

//	XorCompileTime::w_printf(XorStr("Pass %s\n"), Pass);
	char line[328];
	std::string result;

	if (verified) {

		result += GetResponsePASS().c_str();
	//	XorCompileTime::w_printf(XorStr("%s\n"), result.c_str());

		std::string str1(Pass);

		if (str1.length() <= 3)
		{
			return false;
		}
		
		else if (result.find(str1) != std::string::npos) {
				return true;
		}

		else
		{
			return false;
		}

	}
	else {
		return false;
	}
}