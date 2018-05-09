#include "Data.h"

LARGE_INTEGER oldValue, newValue, frequency;
bool AABB = true;
char currentState[100];
char *State = (char*)"Frustum culling";

int ObjectCount = 0;
int Objects = 0;
int FPScount = 0;
int FPS = 0;

double seconds = 0;
double simulationTimePassed;


