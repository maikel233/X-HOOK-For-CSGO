#pragma once
template<typename Function>Function getvfunc(void*_VMT, int Index) // credits @ I dont remember =(
{
	void***_TVMT = (void***)_VMT;
	void**VMT = *_TVMT;
	void*_Address = VMT[Index];
	return(Function)(_Address);
}

inline constexpr void* getvfuncSafe(void* _VMT, size_t index)
{
	return (*static_cast<void***>(_VMT))[index];
}

