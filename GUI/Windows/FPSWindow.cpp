#include "FPSWindow.h"
//#include "Draw.h"
#include "../../Settings.h"
#include "../../Utils/XorStr.hpp"
#include <iomanip>
bool Settings::ESP::Info::FPS = false;
std::string CurrentFPS;

ColorVar Settings::UI::Watermark::color = ImColor(26, 104, 173, 255);
bool Settings::UI::Watermark::displayIngame = false;
bool Settings::UI::Watermark::showwatermark = true;
static std::atomic_int netOutgoingLatency;

static void updateNetLatency() noexcept
{
    if (const auto networkChannel = pEngine->GetNetChannelInfo())
        netOutgoingLatency = (std::max)(static_cast<int>(networkChannel->getLatency(0) * 1000.0f), 0);
    else
        netOutgoingLatency = 0;
}

void FPSWindow::update()
{
    static int lastFrame;
    if (lastFrame == pGlobalVars->framecount)
        return;
    lastFrame = pGlobalVars->framecount;

    updateNetLatency();
}

int FPSWindow::getNetOutgoingLatency() noexcept
{
    return netOutgoingLatency;
}

void FPSWindow::Info()
{

    if (Settings::HWID::LoggedIn)
    {

        if (!Settings::UI::Watermark::showwatermark)
            return;

        //FPS
        static auto fps = 1.0f;
        fps = 0.9f * fps + 0.1f * pGlobalVars->absoluteframetime;

        //PING
        auto ping = FPSWindow::getNetOutgoingLatency();

        //TIME
        time_t t = std::time(nullptr);
        std::ostringstream time;
        time << std::put_time(std::localtime(&t), ("%H:%M:%S"));

        std::ostringstream format;
        format << "XHOOK.XYZ"
            << " | " << (fps != 0.0f ? static_cast<int>(1 / fps) : 0) << " fps"
            << " | " << (ping) << " ms"
            << " | " << (time.str().data());


        float windowWidth = ImGui::CalcTextSize(format.str().c_str()).x + 16.f;
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - windowWidth, 0), ImGuiSetCond_Always);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
        if (ImGui::Begin("Watermark", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar /*| ImGuiWindowFlags_ShowBorders*/ | ImGuiWindowFlags_NoTitleBar))
        {
            ImGui::Text("%s", format.str().c_str());
            ImGui::End();
        }
        ImGui::PopStyleVar();
    }
}