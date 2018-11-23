#include "DDI.h"
#include "Draw.h"
#include "Settings.h"
#include <string>
#include "SpotifyControls.h"
#include <comdef.h>  
#include "XorStr.hpp"

bool Settings::ESP::Info::FPS = false;
std::string CurrentFPS;

ColorVar Settings::UI::Watermark::color = ImColor(26, 104, 173, 255);
bool Settings::UI::Watermark::displayIngame = false;
bool Settings::UI::Watermark::showwatermark = false;
////Spotify
//bool Settings::Spotify::ShowControls = false;
//bool Settings::Spotify::ShowCurrentlyPlaying = false;

static std::string GetTimeString()
{
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%X", time_info);
	return timeString;
}


void Info()
{

	//Draw::Text(1, -1, "X-HOOK.XYZ V1.2 for Counter-Strike Global offensive", esp_font, Color(255, 255, 255, 255));

	//	if (pEngine->IsInGame() && (*pGameRules) && (*pGameRules)->IsValveDS())
	//		Draw::Text(200, -1, "VAC Server", esp_font, Color(0, 255, 0, 255));

	if (Settings::HWID::LoggedIn)
	{

		if (!Settings::UI::Watermark::showwatermark)
			return;

		if (pEngine->IsInGame() && !Settings::UI::Watermark::displayIngame) {
			return;
		}

		//Draw::Text(1, -1, "X-HOOK.XYZ V1.3 for Counter-Strike Global offensive", esp_font, Color(255, 255, 255, 255));
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[40];

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, sizeof(buffer), "%T %Z", timeinfo);
		std::string time(buffer);

		std::string watermark(XorStr("X-HOOK.XYZ | "));
		watermark.append(time);

		Draw::Text(1, -1, watermark.c_str(), esp_font, Color(255, 255, 255, 255));
				
		if (Settings::ESP::Info::FPS)
		{

		}
	}
}

//Spotify
void ShowSpotify::RenderWindow()
{
	//if (!Settings::Spotify::ShowControls)
	//	return;

	//ImGui::SetNextWindowSize(ImVec2(256, 50), ImGuiSetCond_FirstUseEver);

	//if (ImGui::Begin("Spotify controls"), &Settings::ShowSpectators::enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders)
	//{
	//	//
	//	static Spotify* spotify = new Spotify();
	//	if (spotify->_IsAttached())
	//	{
	//		std::wstring curSong = spotify->GetCurrentSong();

	//		if (ImGui::Button("<= Previous song")) {
	//			spotify->Previous();
	//		}
	//		ImGui::SameLine();
	//		if (curSong.find(L"Paused") == std::wstring::npos)
	//		{
	//			if (ImGui::Button("Pause song")) {
	//				spotify->Pause();
	//			}
	//		}
	//		else
	//		{
	//			if (ImGui::Button("Play song")) {
	//				spotify->Pause();
	//			}
	//		}

	//		ImGui::SameLine();
	//		if (ImGui::Button("Next Song =>")) {
	//			spotify->Next();
	//		}

	//		if (curSong.find(L"Paused") == std::wstring::npos)
	//		{
	//			_bstr_t b(curSong.c_str());
	//			const char* c = b;


	//			ImGui::Text("Current song: ");
	//			ImGui::SameLine();
	//			ImGui::Text("%s", c);
	//		}
	//		else
	//		{

	//			ImGui::Separator();
	//			ImGui::Text("Music Paused");
	//		}
	//	} // End of spotify attach

	//	ImGui::End();
	//}
	
}

//Windows Media player?
//VLC?