#include "util.h"
#include "../settings.h"
#include "Xorstr.hpp"
#include "Memmem.h"
#include <iomanip>



void Util::Log(std::string s)
{
}

int Util::RandomInt(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

float Util::RandomFloat(float min, float max)
{
	typedef float(*RandomFloat_t)(float, float);
	static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");
	return m_RandomFloat(min, max);
}

float Util::rand_FloatRange(float a, float b)
{
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

std::string Util::ReplaceString(std::string subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;

	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}

	return subject;
}

void Util::StdReplaceStr(std::string& replaceIn, const std::string& replace, const std::string& replaceWith)
{
	size_t const span = replace.size();
	size_t const step = replaceWith.size();
	size_t index = 0;

	while (true)
	{
		index = replaceIn.find(replace, index);

		if (index == std::string::npos)
			break;

		replaceIn.replace(index, span, replaceWith);
		index += step;
	}
}

const char* Util::PadStringRight(std::string text, size_t value)
{
	text.insert(text.length(), value - text.length(), ' ');

	return text.c_str();
}

bool Util::Contains(const std::string &word, const std::string &sentence) {
	if (word == "" || sentence == "")
		return true;

	return sentence.find(word) != std::string::npos;
}

std::string Util::ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	return str;
}

std::string Util::ToUpper(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);

	return str;
}


//conversion of LPCWSTR to std::string
std::string Util::wstringTostring(const std::wstring &wstr)
{
	std::string strTo;
	char *szTo = new char[wstr.length() + 1];
	szTo[wstr.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	strTo = szTo;
	delete[] szTo;
	return strTo;
}


std::wstring Util::stringTowstring(const std::string& s) {
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
	//return NULL;
}


long Util::GetEpochTime()
{
	using namespace std::chrono;
	milliseconds ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);

	return ms.count();
}

ImColor Util::GetRainbowColor(float speed)
{
	speed = 0.002f * speed;
	long now = Util::GetEpochTime();
	float hue = (now % (int)(1.0f / speed)) * speed;

	return ImColor::HSV(hue, 1.0f, 1.0f);
}

Color Util::GetHealthColor(int hp)
{
	return Color(
		min(510 * (100 - hp) / 100, 255),
		min(510 * hp / 100, 255),
		25
	);
}

Color Util::GetHealthColor(C_BasePlayer* player)
{
	return Color(
		min(510 * (100 - player->GetHealth()) / 100, 255),
		min(510 * player->GetHealth() / 100, 255),
		25
	);
}


bool Util::IsBadReadPtr(PVOID pPointer)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };

	if (VirtualQuery(pPointer, &mbi, sizeof(mbi)))
	{
		DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);

		bool ret = !(mbi.Protect & mask);

		if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
			ret = true;

		return ret;
	}

	return true;
}


const std::map<int, int> * Util::GetModelTypeBoneMap(C_BasePlayer* player)
{
	studiohdr_t* pStudioModel = pModelInfo->GetStudioModel(player->GetModel());

	switch (pStudioModel->numbones) {
	case 84: // Anarchists or Leet Krew
		if (memchr(pStudioModel->name, 'h', sizeof(pStudioModel->name)) != NULL)// Anarchist
		{
			return &BoneMapT_Anarchist;
		}
		else // Leet Krew
		{
			return &BoneMapT_Leet;
		}
	case 85: // Leet Krew 2
		return &BoneMapT_Leet2;
	case 86: // Balkan, Phoenix, and Separatists
		if (memchr(pStudioModel->name, 'h', sizeof(pStudioModel->name)) != NULL) // Phoenix
		{
			return &BoneMapT_Phoenix;
		}
		else if (memmem(pStudioModel->name, sizeof(pStudioModel->name), "ba", 2) != NULL) // balkan
		{
			return &BoneMapT_Balkan;
		}
		else // Separatist
		{
			return &BoneMapT_Separatist;
		}
	case 89: // FBI, GSG, and SEALS
		if (memchr(pStudioModel->name, 'f', sizeof(pStudioModel->name)) != NULL) // FBI
		{
			return &BoneMapCT_FBI;
		}
		else if (memmem(pStudioModel->name, sizeof(pStudioModel->name), "sg", 2) != NULL) // GSG
		{
			return &BoneMapCT_GSG;
		}
		else // Seals
		{
			return &BoneMapCT_Seals;
		}
	case 91: // SWAT, Professionals
		if (memchr(pStudioModel->name, 'w', sizeof(pStudioModel->name)) != NULL) // SWAT
		{
			return &BoneMapCT_SWAT;
		}
		else // Professionals
		{
			return &BoneMapT_Professional;
		}
	case 93: // GIGN
		return &BoneMapCT_GIGN;
	case 94: // IDF
		return &BoneMapCT_IDF;
	case 98: // FBI or SAS
		if (memchr(pStudioModel->name, 'f', sizeof(pStudioModel->name)) != NULL) // FBI
		{
			return &BoneMapCT_FBI;
		}
		else {
			return &BoneMapCT_SAS;
		}
	default:
	//	pCvar->ConsoleDPrintf(XorStr("(Util::GetModelTypeBoneMap)- Warning. Model type \"%s\" Unknown. Using Generic boneMap\n"), pStudioModel->name);
		XorCompileTime::w_printf(XorStr("(Util::GetModelTypeBoneMap)- Warning. Model type \"%s\" Unknown. Using Generic boneMap\n"), pStudioModel->name);
	}
}

ModelType Util::GetModelTypeID(C_BasePlayer* player)
{
	studiohdr_t* pStudioModel = pModelInfo->GetStudioModel(player->GetModel());

	switch (pStudioModel->numbones) {
	case 84: // Anarchists or Leet Krew
		if (memchr(pStudioModel->name, 'h', sizeof(pStudioModel->name)) != NULL)// Anarchist
		{
			return ModelType::ANARCHIST;
		}
		else // Leet Krew
		{
			return ModelType::LEETKREW;
		}
	case 85:
		return ModelType::LEETKREW2;
	case 86: // Balkan, Phoenix, and Separatists
		if (memchr(pStudioModel->name, 'h', sizeof(pStudioModel->name)) != NULL) // Phoenix
		{
			return ModelType::PHOENIX;
		}
		else if (memmem(pStudioModel->name, sizeof(pStudioModel->name), "ba", 2) != NULL) // balkan
		{
			return ModelType::BALKAN;
		}
		else // Separatist
		{
			return ModelType::SEPARATIST;
		}
	case 89: // FBI, GSG, and SEALS
		if (memchr(pStudioModel->name, 'f', sizeof(pStudioModel->name)) != NULL) // FBI
		{
			return ModelType::FBI;
		}
		else if (memmem(pStudioModel->name, sizeof(pStudioModel->name), "sg", 2) != NULL) // GSG
		{
			return ModelType::GSG;
		}
		else // Seals
		{
			return ModelType::SEALS;
		}
	case 91: // SWAT, Professionals
		if (memchr(pStudioModel->name, 'w', sizeof(pStudioModel->name)) != NULL) // SWAT
		{
			return ModelType::SWAT;
		}
		else // Professionals
		{
			return ModelType::PROFESSIONAL;
		}
	case 93: // GIGN
		return ModelType::GIGN;
	case 94: // IDF
		return ModelType::IDF;
	case 98: // SAS
		return ModelType::SAS;
	default:
		return ModelType::UNKNOWN;
	}
}

void Util::SetConsoleState(bool bVis)
{
	HWND Hide;
	AllocConsole();
	Hide = FindWindowA(nullptr, XorStr("XHOOK - CSGO"));
	ShowWindow(Hide, bVis);
}

void Util::HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void Util::ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

bool Util::DebugConsole(char* strConName)
{
	FILE* g_pFileBuf;
	if (!AttachConsole(ATTACH_PARENT_PROCESS))
		if (!AllocConsole())
			return false;
	if (!SetConsoleTitleA(strConName))
		return false;
	auto iErrorCode = freopen_s(&g_pFileBuf, "CONOUT$", "w", stdout);
	if (iErrorCode != NULL)
		return false;
	return true;
}

bool Util::SetTextColor(DWORD dwFore, DWORD dwBack)
{
	auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	return SetConsoleTextAttribute(hConsole, dwFore | dwBack << 4);
}


std::string Util::GetTimestamp()
{
	std::time_t t = std::time(nullptr);
	std::tm tm;
	localtime_s(&tm, &t);
	std::locale loc(std::cout.getloc());

	std::basic_stringstream<char> ss;
	ss.imbue(loc);
	ss << std::put_time(&tm, "[%H:%M:%S %Y]"); // "[%A %b %e %H:%M:%S %Y]");

	return ss.str();
}
//
//const char* Util::GetActivityName(int actNum) {
//	if (actNum == -1) {
//		return "ACT_INVALID";
//	}
//	return ActivityStrings[actNum];
//}
