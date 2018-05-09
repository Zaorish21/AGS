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
	// номер транзакции
	unsigned short transactionID;
	// номер пакета среди группы пакетов одного запроса/ответа
	unsigned short frameNumber;
	// всего пакетов в группе пакетов одного запроса/ответа
	unsigned short frameCount;
	// длина оставшейся части данных текущего пакета (включая funcID)
	unsigned int dataLen;
	// код запроса/ответа в соответствии с протоколом
	unsigned char funcID;
};

struct SNetworkHader2 {
	// номер транзакции
	unsigned short transactionID;
	// номер пакета среди группы пакетов одного запроса/ответа
	unsigned short frameNumber;
	// всего пакетов в группе пакетов одного запроса/ответа
	unsigned short frameCount;
	// длина оставшейся части данных текущего пакета (включая funcID)
	unsigned int dataLen;
	// код запроса/ответа в соответствии с протоколом
	unsigned char funcID;
	float x;
	float y;
	float z;
	float r;
};

#pragma pack(push,1)
struct SGameObjectDescription {
	// идентификатор объекта
	unsigned int ObjectID;
	// тип объекта
	unsigned char ObjectType;
	// позиция объекта (x, y, z)
	float x, y, z;
	// угол поворота в градусах по часовой стрелке вокруг оси oY
	float Yaw;
	// имя модели (в файле Resources.xml)
	char Model[16];
	// зарезервированные поля
	float AABB[3];
	char Reserved[47];
};