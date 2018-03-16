#include "Simulation.h"

void CameraSimulation(double simulationTimePassed)
{
	bool Forward = GetAsyncKeyState(VK_UP);
	bool Back = GetAsyncKeyState(VK_DOWN);
	bool Left = GetAsyncKeyState(VK_LEFT);
	bool Right = GetAsyncKeyState(VK_RIGHT);

	float dForward = (int(Forward) - int(Back))*simulationTimePassed;
	float dRigth = (int(Right) - int(Left))*simulationTimePassed;

	GetCursorPos(&newPossition);
	float dHorizAngle = oldPossition.x - newPossition.x;
	float dVertAngle = oldPossition.y - newPossition.y;
	oldPossition = newPossition;

	Camera.MoveOXZ(dForward, dRigth);
	Camera.Rotate(dHorizAngle, dVertAngle);
}
