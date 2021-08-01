#pragma once

#include "../../ProfileChanger/Messages.h"
#include "../../ProtoUtils/Protobuffs.h"
#include "../../ProtoUtils/ProtoWriter.h"

class ProfileChanger
{
public:
	bool Presend(uint32_t& unMsgType, void* pubData, uint32_t& cubData);
	void RenderWindow(bool contentOnly) noexcept;
};
inline ProfileChanger* pProfileChanger;