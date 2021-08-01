#pragma once

class CBaseHandle;
typedef unsigned long CBaseHandle2;
class IClientEntityList
{
public:
	virtual IClientNetworkable* GetClientNetworkable(int entindex) = 0;
	virtual IClientNetworkable* GetClientNetworkableFromHandle(CBaseHandle handle) = 0;
	virtual IClientUnknown* GetClientUnknownFromHandle(CBaseHandle handle) = 0;
	// FIXME: GetClientEntity should return an IClientEntity pointer.
	virtual C_BaseEntity* GetClientEntity(int entindex) = 0;
	// FIXME: GetClientEntityFromHandle should accept a CBaseHandle.
	virtual IClientEntity* GetClientEntityFromHandle(DWORD handle) = 0;
	//	virtual IClientEntity* GetClientEntityFromHandle(void* handle) = 0;
	virtual int NumberOfEntities(bool include_non_networkable) = 0;
	virtual int GetHighestEntityIndex(void) = 0;
	virtual void SetMaxEntities(int max_entities) = 0;
	virtual int GetMaxEntities() = 0;
	DWORD GetClientEntityFromHandle2(DWORD Entity) { typedef DWORD(__thiscall* OriginalFn)(void*, DWORD); return getvfunc<OriginalFn>(this, 4)(this, Entity); }
	//void* GetClientEntity(int index) {
	//	using original_fn = void* (__thiscall*)(IClientEntityList*, int);
	//	return (*(original_fn**)this)[3](this, index);
	//}
	//void* GetClientEntityFromHandle(uintptr_t handle) {
	//	using original_fn = void* (__thiscall*)(IClientEntityList*, uintptr_t);
	//	return (*(original_fn**)this)[4](this, handle);
	//}
	//int GetHighestEntityIndex() {
	//	using original_fn = int(__thiscall*)(IClientEntityList*);
	//	return (*(original_fn**)this)[6](this);
	//}
};