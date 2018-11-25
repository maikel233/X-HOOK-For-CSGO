#pragma once

class ConVar
{
public:
	// got this from TGF aswell, 
	// ignore the typedef that doesn't really follow my other styles
	// i just chose to change the names to original when I made this class
	int GetInt()
	{
		typedef int(__thiscall *o_GetInt)(void*);
		return getvfunc<o_GetInt>(this, 13)(this);
	}
	float GetFloat()
	{
		typedef float(__thiscall *o_GetFloat)(void*);
		return getvfunc<o_GetFloat>(this, 12)(this);
	}
	void SetValue(const char* value)
	{
		typedef void(__thiscall* original)(void*, const char*);
		return getvfunc<original>(this, 14)(this, value);
	}

	void SetValue(float value)
	{
		typedef void(__thiscall* original)(void*, float);
		return getvfunc<original>(this, 15)(this, value);
	}

	void SetValue(int value)
	{
		typedef void(__thiscall* original)(void*, int);
		return getvfunc<original>(this, 16)(this, value);
	}

	void SetValue(Color value)
	{
		typedef void(__thiscall* original)(void*, Color);
		return getvfunc<original>(this, 17)(this, value);
	}

	void InternalSetString(const char* str)
	{

	}

	char* GetName()
	{
		typedef char*(__thiscall* original)(void*);
		return getvfunc<original>(this, 5)(this);
	}

	char* GetDefault()
	{
		return pszDefaultValue;
	}

	char pad_0x0000[0x4]; //0x0000
	ConVar* pNext; //0x0004 
	__int32 bRegistered; //0x0008 
	char* pszName; //0x000C 
	char* pszHelpString; //0x0010 
	__int32 nFlags; //0x0014 
	char pad_0x0018[0x4]; //0x0018
	ConVar* pParent; //0x001C 
	char* pszDefaultValue; //0x0020 
	char* strString; //0x0024 
	__int32 StringLength; //0x0028 
	float fValue; //0x002C 
	__int32 nValue; //0x0030 
	__int32 bHasMin; //0x0034 
	float fMinVal; //0x0038 
	__int32 bHasMax; //0x003C 
	float fMaxVal; //0x0040 
	void* fnChangeCallback; //0x0044 
};



class SpoofedConvar {
public:
	SpoofedConvar();
	SpoofedConvar(const char* szCVar);
	SpoofedConvar(ConVar* pCVar);

	~SpoofedConvar();

	bool           IsSpoofed();
	void           Spoof();

	void           SetFlags(int flags);
	int            GetFlags();

	void           SetBool(bool bValue);
	void           SetInt(int iValue);
	void           SetFloat(float flValue);
	void           SetString(const char* szValue);

private:
	ConVar* m_pOriginalCVar = NULL;
	ConVar* m_pDummyCVar = NULL;

	char m_szDummyName[128];
	char m_szDummyValue[128];
	char m_szOriginalName[128];
	char m_szOriginalValue[128];
	int m_iOriginalFlags;
};
class IAppSystemm
{
public:
	virtual ~IAppSystemm()
	{
	}

	virtual void func0() = 0;
	virtual void func1() = 0;
	virtual void func2() = 0;
	virtual void func3() = 0;
	virtual void func4() = 0;
	virtual void func5() = 0;
	virtual void func6() = 0;
	virtual void func7() = 0;
	virtual void func8() = 0;
	virtual void func9() = 0;
};

struct CVarDLLIdentifier_t;

// Not really ideal but there's already a Color struct used by the GUI.
struct ColorRGBA {
	ColorRGBA(unsigned char Red, unsigned char Green, unsigned char Blue, unsigned char Alpha = 255) {
		RGBA[0] = Red;
		RGBA[1] = Green;
		RGBA[2] = Blue;
		RGBA[3] = Alpha;
	}

	unsigned char RGBA[4];
};

class ICvar : public IAppSystemm
{
public:
	virtual void			func10() = 0;
	virtual void			RegisterConCommand(ConVar *pCommandBase) = 0;
	virtual void			UnregisterConCommand(ConVar *pCommandBase) = 0;
	virtual void			func13() = 0;
	virtual ConVar			*FindVar(const char *var_name) = 0;
	//	virtual void            ConsoleColorPrintf(const Color& clr, const char *pFormat, ...) const = 0;
	virtual void			func15() = 0;
	virtual void			func16() = 0;
	virtual void			func17() = 0;
	virtual void			func18() = 0;
	virtual void			func19() = 0;
	virtual void			func20() = 0;


	template< typename... Values >
	void ConsoleColorPrintf(const ColorRGBA& MsgColor, const char* szMsgFormat, Values... Parameters) {
		typedef void(*oConsoleColorPrintf)(void*, const ColorRGBA&, const char*, ...);
		return getvfunc<oConsoleColorPrintf>(this, 25)(this, MsgColor, szMsgFormat, Parameters...);
	}
	template <typename... Values>
	void ConsoleDPrintf(const char* szMsgFormat, Values... Parameters)
	{
		typedef void(*oConsoleDPrintf)(void*, const char*, ...);
		return getvfunc<oConsoleDPrintf>(this, 27)(this, szMsgFormat, Parameters...);
	}
};

