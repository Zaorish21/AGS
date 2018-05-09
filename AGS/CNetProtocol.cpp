#include "CNetProtocol.h"

int CNetProtocol::transID = 0;
bool CNetProtocol::init(std::string ip, short port)
{
	// первый объект
	WSADATA wsa;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : %d\n", WSAGetLastError());
	}
	else {
		printf("OK\n");
	}

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		printf("Could not create socket : %d\n", WSAGetLastError());
	}
	else {
		printf("Socket created.\n");
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip.c_str());
	server.sin_port = htons(port);

	int connect_error = connect(s, (struct sockaddr *)&server, sizeof(server));
	if (connect_error == SOCKET_ERROR) {
		printf("Connection failed : %d\n", WSAGetLastError());
		connected = false;
		return false;
	}
	else {
		printf("Connection is established.\n");
		connected = true;
		return true;
	};
}

std::string CNetProtocol::getMessage()
{

	SNetworkHader header;
	header.transactionID = transID++;
	header.frameNumber = 1;
	header.frameCount = 1;
	header.dataLen = 1;
	header.funcID = 0;

	int send_bytes_count = send(s, (const char*)&header, sizeof(header), 0);
	if (send_bytes_count < sizeof(header)) {
		printf("Data was not send: %d.\n", WSAGetLastError());
	}

	int receive_data_count = recv(s, (char*)&header, sizeof(header), 0);
	if (receive_data_count <= 0) {
		return std::string("Data was not received");
	}
	else {
		char *str = new char[header.dataLen - 1];
		recv(s, str, header.dataLen - 1, 0);
		std::string temp(str);
		delete[] str;
		return temp;
	}
}

std::vector<SGameObjectDescription> CNetProtocol::getDemoScene()
{
	SNetworkHader header;
	std::vector<SGameObjectDescription> DescVec;
	header.transactionID = ++transID;
	header.frameNumber = 1;
	header.frameCount = 1;
	header.dataLen = 1;
	header.funcID = 1;
	int send_bytes_count = send(s, (const char*)&header, sizeof(header), 0);
	if (send_bytes_count < sizeof(header)) {
		printf("Data was not send: %d.\n", WSAGetLastError());
	}
	int receive_data_count = recv(s, (char*)&header, sizeof(header), 0);
	if (receive_data_count <= 0) {
		printf("Data was not received: %d.\n", WSAGetLastError());
	}
	else {
		int count;
		waitingToRead(sizeof(int));
		recv(s, reinterpret_cast<char*>(&count), sizeof(int), 0);
		SGameObjectDescription Description;
		for (int i = 0; i < count; i++)
		{
			waitingToRead(sizeof(SGameObjectDescription));
			recv(s, reinterpret_cast<char*>(&Description), sizeof(Description), 0);
			DescVec.push_back(Description);
		}
	}
	return DescVec;
}

std::vector<SGameObjectDescription> CNetProtocol::getNearlyObjects(float x, float y, float z, float r)
{
	SNetworkHader header1;
	SNetworkHader2 header
	{
		transID++,
		1,
		1,
		17,
		2,
		x,
		y,
		z,
		r
	};
	std::vector<SGameObjectDescription> DescVec;
	int send_bytes_count = send(s, (const char*)&header, sizeof(header), 0);
	if (send_bytes_count < sizeof(header)) {
		printf("Data was not send: %d.\n", WSAGetLastError());
	}
	int receive_data_count = recv(s, (char*)&header1, sizeof(header1), 0);
	if (receive_data_count <= 0) {
		printf("Data was not received: %d.\n", WSAGetLastError());
	}
	else {
		int count;
		waitingToRead(sizeof(int));
		recv(s, reinterpret_cast<char*>(&count), sizeof(int), 0);
		SGameObjectDescription Description;
		for (int i = 0; i < count; i++)
		{
			waitingToRead(sizeof(SGameObjectDescription));
			recv(s, reinterpret_cast<char*>(&Description), sizeof(Description), 0);
			DescVec.push_back(Description);
		}
	}
	return DescVec;
}

void CNetProtocol::waitingToRead(int count)
{
	u_long bytesToRead = 0;
	while (bytesToRead < count)
	{
		ioctlsocket(s, FIONREAD, &bytesToRead);
	}
}