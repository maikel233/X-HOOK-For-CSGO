#pragma once


namespace ReportBot {
	void run() noexcept;
	void resetReportbot() noexcept;
	bool submitReport(const char* xuid, const char* report);
};