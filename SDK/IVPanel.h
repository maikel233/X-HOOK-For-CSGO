#pragma once

class IVPanel
{
public:
	const char* GetName(int Panel)
	{
		typedef const char*(__thiscall* OriginalFn)(void*, int);
		return getvfunc<OriginalFn>(this, 36)(this, Panel);
	}
};