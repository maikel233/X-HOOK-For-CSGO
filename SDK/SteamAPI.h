#pragma once
#include <iostream>
#include <cstdint>

#include "Inconstructible.h"
#include "VirtualMethod.h"

class SteamFriends {
public:
	INCONSTRUCTIBLE(SteamFriends)

		VIRTUAL_METHOD(int, getSmallFriendAvatar, 34, (std::uint64_t steamID), (this, steamID))
		VIRTUAL_METHOD(const char*, getPersonaName, 0, (), (this))
};

class SteamUtils {
public:
	INCONSTRUCTIBLE(SteamUtils)

	VIRTUAL_METHOD(bool, getImageRGBA, 6, (int image, std::uint8_t* buff, int buffSize), (this, image, buff, buffSize))
};

struct SteamAPIContext {
	void* steamClient;
	void* steamUser;
	SteamFriends* steamFriends;
	SteamUtils* steamUtils;
};

template<typename FuncType>
__forceinline static FuncType CallVFunction(void* ppClass, int index)
{
	int* pVTable = *(int**)ppClass;
	int dwAddress = pVTable[index];
	return (FuncType)(dwAddress);
}

enum EGCResult
{
	k_EGCResultOK = 0,
	k_EGCResultNoMessage = 1,           // There is no message in the queue
	k_EGCResultBufferTooSmall = 2,      // The buffer is too small for the requested message
	k_EGCResultNotLoggedOn = 3,         // The client is not logged onto Steam
	k_EGCResultInvalidMessage = 4,      // Something was wrong with the message being sent with SendMessage
};

class ISteamGameCoordinator
{
public:
	virtual EGCResult GCSendMessage(int unMsgType, const void* pubData, int cubData) = 0;
	virtual bool IsMessageAvailable(int* pcubMsgSize) = 0;
	virtual EGCResult GCRetrieveMessage(int* punMsgType, void* pubDest, int cubDest, int* pcubMsgSize) = 0;
};
//
