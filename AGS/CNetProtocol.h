#pragma once
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <vector>
#pragma comment(lib,"ws2_32.lib")

#include "Data.h"

class CNetProtocol
{
	SOCKET s;
	bool connected;
	static 	int transID;
public:
	bool init(std::string ip, short port);
	std::string getMessage();
	std::vector<SGameObjectDescription> getDemoScene();
	std::vector<SGameObjectDescription> getNearlyObjects(float x, float y, float z, float r);
private:
	void waitingToRead(int count);
};

