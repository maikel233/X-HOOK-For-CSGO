#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "HWID.h"
#include "IRC.h"

#include <windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include "Menu.h"


bool Settings::IRC::Enabled = false;
char Settings::IRC::Message[255] = "";

int Time;
bool init;
bool bSendMessage;
std::vector<std::string>Logs;
#define IM_ARRAYSIZE(_ARR)      ((int)(sizeof(_ARR)/sizeof(*_ARR)))
static char User[256] = "";

void IRC::RenderWindow()
{

	if (!Settings::IRC::Enabled)
		return;

	/*

	IRC::Thread();

	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiSetCond_FirstUseEver);

	if (ImGui::Begin(XorStr("IRC"), &Settings::IRC::Enabled, ImGuiWindowFlags_ShowBorders))
	{

		if (g_pSecurity->User == "User")
		{
			ImGui::Image(UsericoTexture, ImVec2(16, 16));
			ImGui::SameLine();
			ImGui::Text("Username:");
			ImGui::SameLine();
			ImGui::InputText("", User, IM_ARRAYSIZE(User), ImGuiInputTextFlags_CharsNoBlank);
			ImGui::SameLine();
			if (ImGui::Button(XorStr("Set Username"))) {
				g_pSecurity->User = User;

			}
		}

		
		ImGui::Columns(1, XorStr("X-HOOK - IRC"));
		ImGui::Separator();
		ImGui::Text(XorStr("Chat"));
		ImGui::NextColumn();
		ImGui::Separator();

		for (auto i = 0; i < Logs.size(); i++)
		{
			if (i >= Logs.size())
				continue;

			ImGui::Text(Logs[i].c_str());
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::Separator();

		ImGui::InputText(XorStr("Message"), Settings::IRC::Message, sizeof(Settings::IRC::Message)); ImGui::SameLine();
		if (ImGui::IsWindowFocused() && Settings::IRC::Enabled && GetAsyncKeyState(VK_RETURN) & 1)
			bSendMessage = true;
		if (ImGui::Button(XorStr("Send")))
			bSendMessage = true;

		ImGui::SameLine();

		if (ImGui::Button(XorStr("Refresh")))
			init = false;

		ImGui::SetScrollY(ImGui::GetScrollMaxY());
	}
	*/
	ImGui::End();
}
void IRC::Thread()
{
	std::string Break;
	std::stringstream Log;
	std::stringstream oLog;

	if (bSendMessage)
	{
		SendResponse(Settings::IRC::Message);
		strcpy(Settings::IRC::Message, "");
		bSendMessage = false;
	}
	if (Time > 3000)
	{
		Time = 0;
		Log.str(GetResponse().c_str());
	}
	if (Log.str() != oLog.str())
	{
		if (Logs.size() > 0)
			Logs.clear();
		while (getline(Log, Break, '|'))
		{
			Logs.push_back(Break.c_str());
		}
		oLog.str() = Log.str();
	}
	Time++;
}

void find_and_replace(std::string& source, std::string const& find, std::string const& replace)
{
	for (std::string::size_type i = 0; (i = source.find(find, i)) != std::string::npos;)
	{
		source.replace(i, find.length(), replace);
		i += replace.length();
	}
}
bool IRC::SendResponse(std::string msg)
{
	std::string Message;
	Message.append(XorStr("/loader/lso/chat/Write.php?action=post"));

	Message.append(XorStr("&name="));
	Message.append(g_pSecurity->User);
	Message.append("&usermsg=");
	Message.append(msg.c_str());
	Message.append("\"");
	find_and_replace(Message, " ", "%20");
	g_Winsock.InitWinsock();
	g_Winsock.Connect(XorStr("x-hook.xyz")); 
	std::string cstrResponse;
	g_Winsock.SendHttpGet(Message.c_str(), cstrResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();
	return false;
}

std::string IRC::GetResponse()
{
	std::string Message;
	
	Message.append(XorStr("/loader/lso/chat/data.txt")); // Read.php?action=post  // Read file shows <br> lines
	find_and_replace(Message, "cancer", "%20");
	//Message.append(XorStr("/index.php"));
	g_Winsock.InitWinsock();

	g_Winsock.Connect(XorStr("x-hook.xyz")); // Note VPS is needed Website has shared hosting not possible (:
	std::string cstrResponse;
	g_Winsock.SendHttpGet(Message.c_str(), cstrResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();

	if (cstrResponse.size() > 0)
		return cstrResponse;
	return std::string(XorStr("Failed to Receive response from IRC"));
}
CWinsock g_Winsock;

CWinsock::CWinsock(VOID)
{
	m_ConnectedSocket = INVALID_SOCKET;
}

CWinsock::~CWinsock(VOID)
{

}

VOID CWinsock::InitWinsock(VOID)
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ExitProcess(0);
	}

}

VOID CWinsock::Connect(const char* pszServerUrl)
{
	m_pszConnectedUrl = pszServerUrl;

	addrinfo hints, *pAddrInfo;

	SecureZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	bool bPort;

	if (getaddrinfo(pszServerUrl, XorStr("80"), &hints, &pAddrInfo) != 0)
	{
		bPort = false;
	}
	if ((m_ConnectedSocket = socket(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol)) == INVALID_SOCKET)
	{
		freeaddrinfo(pAddrInfo);
		CleanUp();
		ExitProcess(0);
	}

	if (connect(m_ConnectedSocket, pAddrInfo->ai_addr, (INT)pAddrInfo->ai_addrlen) != 0)
	{
		freeaddrinfo(pAddrInfo);
		Disconnect();
		CleanUp();
		ExitProcess(0);
	}

	freeaddrinfo(pAddrInfo);
}



VOID CWinsock::SendHttpGet(const char* pszFileUrl, std::string &sDestBuffer)
{
	CString sMsg = XorStr("GET ");
	sMsg += pszFileUrl;
	sMsg += XorStr(" HTTP/1.0\r\nHost: "); // 1.0 instead of 1.1 because Chunked_transfer_encoding
	sMsg += m_pszConnectedUrl;
	sMsg += XorStr("\r\n\r\n");

	CHAR szRecvBuffer[512];

	if (send(m_ConnectedSocket, sMsg, strlen(sMsg), 0) == SOCKET_ERROR)
	{
		Disconnect();
		WSACleanup();
		ExitProcess(0);
	}

	INT iBytesReceived = 0;
	do
	{
		iBytesReceived = recv(m_ConnectedSocket, szRecvBuffer, sizeof(szRecvBuffer), 0);

		sDestBuffer.append(szRecvBuffer, iBytesReceived);

	} while (iBytesReceived > 0);
	//sprintf("%s", sDestBuffer);
	for (int i = 0; i < sDestBuffer.size(); i++)
	{
		if (sDestBuffer[i] == '\r' && sDestBuffer[i + 1] == '\n' && sDestBuffer[i + 2] == '\r' && sDestBuffer[i + 3] == '\n')
			sDestBuffer.erase(0, i + 4);
	}

}

VOID CWinsock::Disconnect(VOID)
{
	if (m_ConnectedSocket != INVALID_SOCKET)
	{
		closesocket(m_ConnectedSocket);
		m_ConnectedSocket = INVALID_SOCKET;
	}

}

VOID CWinsock::CleanUp(VOID)
{
	WSACleanup();
}