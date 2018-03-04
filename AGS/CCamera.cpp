#include "CCamera.h"

CCamera::CCamera(void)
{
	vec3 Eye = vec3(0.0, 5.0, 10.0);
	vec3 Center = vec3(0, 0, 0);
	vec3 Up = vec3(0, 1.0, 0);
	ViewMatrix = lookAt(Eye, Center, Up);
}

mat4 CCamera::GetViewMatrix()
{
	return ViewMatrix;
}

void CCamera::SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
	ProjectionMatrix = perspective(fovy, aspect, zNear, zFar);
}

mat4 CCamera::GetProjectionMatrix(void)
{
	return ProjectionMatrix;
}