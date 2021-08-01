#pragma once

void Info();

namespace FPSWindow
{
	void Info();
	int getNetOutgoingLatency() noexcept;
	void update();
};