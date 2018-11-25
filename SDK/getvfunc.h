#pragma once
template<typename Function>Function getvfunc(void*_VMT, int Index) // credits @ I dont remember =(
{
	void***_TVMT = (void***)_VMT;
	void**VMT = *_TVMT;
	void*_Address = VMT[Index];
	return(Function)(_Address);
}