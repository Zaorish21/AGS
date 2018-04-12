#pragma once


#include "CScene.h"

extern CScene		Scene;
extern LARGE_INTEGER oldValue, newValue, frequency;

extern double simulationTimePassed;
extern int FPScount;
extern int FPS;
extern double seconds;
extern char currentState[100];