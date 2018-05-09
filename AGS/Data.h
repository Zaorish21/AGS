#pragma once



#include <windows.h>

extern LARGE_INTEGER oldValue, newValue, frequency;
extern bool AABB;
extern double simulationTimePassed;
extern int FPScount;
extern int FPS;
extern int ObjectCount;
extern int Objects;
extern double seconds;
extern char currentState[100];
extern char *state;
#pragma pack(push,1)
struct SNetworkHader {
	// ����� ����������
	unsigned short transactionID;
	// ����� ������ ����� ������ ������� ������ �������/������
	unsigned short frameNumber;
	// ����� ������� � ������ ������� ������ �������/������
	unsigned short frameCount;
	// ����� ���������� ����� ������ �������� ������ (������� funcID)
	unsigned int dataLen;
	// ��� �������/������ � ������������ � ����������
	unsigned char funcID;
};

struct SNetworkHader2 {
	// ����� ����������
	unsigned short transactionID;
	// ����� ������ ����� ������ ������� ������ �������/������
	unsigned short frameNumber;
	// ����� ������� � ������ ������� ������ �������/������
	unsigned short frameCount;
	// ����� ���������� ����� ������ �������� ������ (������� funcID)
	unsigned int dataLen;
	// ��� �������/������ � ������������ � ����������
	unsigned char funcID;
	float x;
	float y;
	float z;
	float r;
};

#pragma pack(push,1)
struct SGameObjectDescription {
	// ������������� �������
	unsigned int ObjectID;
	// ��� �������
	unsigned char ObjectType;
	// ������� ������� (x, y, z)
	float x, y, z;
	// ���� �������� � �������� �� ������� ������� ������ ��� oY
	float Yaw;
	// ��� ������ (� ����� Resources.xml)
	char Model[16];
	// ����������������� ����
	float AABB[3];
	char Reserved[47];
};