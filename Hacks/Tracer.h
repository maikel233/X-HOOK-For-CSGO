#pragma once

#include "../Features.h"
#include "../settings.h"
#include <map>
#include <string>
#include <vector>
namespace TracerEffect
{
	void RestoreTracers();
	//Hooks
	void CreateMove(CUserCmd* cmd);
}