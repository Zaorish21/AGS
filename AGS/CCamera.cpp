#include "CCamera.h"

CCamera::CCamera(void)
{
	std::ifstream file("Camera.txt");
	if (!file.is_open())
	{
		radius = 40;
		theta = radians(5.0);
		fi = 0;
		Center = vec3(0, 0, 0);
	}
	else
	{
		file >> radius >> theta >> fi >>Center.x>>Center.y>>Center.z;
		file.close();
	}
	Eye.x = radius * sin(theta)* cos(fi);
	Eye.y = radius * cos(theta);
	Eye.z = radius * sin(theta)* sin(fi);
	Up = vec3(0, 1, 0);

	ViewMatrix = lookAt(Eye + Center, Center, Up);
}

mat4 CCamera::GetViewMatrix()
{
	return ViewMatrix;
}

void CCamera::MoveOXZ(float dForward, float dRight)
{
	dForward = dForward * Speed;
	dRight = dRight * Speed;
	vec3 VForward = normalize(Center - (Eye + Center));
	vec3 DeltaF = vec3(VForward.x * dForward, 0, VForward.z * dForward);
	vec3 DeltaR = normalize(cross(VForward, Up));
	DeltaR = vec3(DeltaR.x * dRight, 0, DeltaR.z * dRight);
	std::cout << DeltaR.x << ' ' << DeltaR.y << ' ' << DeltaR.z<<std::endl;
	Center = Center + DeltaF + DeltaR;

	ViewMatrix = lookAt(Eye + Center, Center, Up);
}

void CCamera::Zoom(float dR)
{
	if ((0 > dR) & (radius < 40)) radius = radius - dR;
	if ((0 < dR) & (radius > 3)) radius = radius - dR;
	Eye.x = radius * sin(theta) * cos(fi);
	Eye.y = radius * cos(theta);
	Eye.z = radius * sin(theta) * sin(fi);
	ViewMatrix = lookAt(Eye + Center, Center, Up);
}

void CCamera::Rotate(float dHorizAngle, float dVertAngle)
{
	bool rButton = GetAsyncKeyState(VK_RBUTTON);
	if (rButton)
	{
		if ((dVertAngle > 0) & (theta < THETA_MAX)) theta += dVertAngle / 360;
		if ((dVertAngle < 0) & (theta > THETA_MIN)) theta += dVertAngle / 360;
		
		fi += dHorizAngle / 360;
		if (fi > 2 * PI) fi -= 2 * PI;
		if (fi < 0) fi += 2 * PI;

		if (theta < THETA_MIN) theta = THETA_MIN;
		if (theta > THETA_MAX) theta = THETA_MAX;

		Eye.x = radius * sin(theta) * cos(fi);
		Eye.y = radius * cos(theta);
		Eye.z = radius * sin(theta) * sin(fi);
		ViewMatrix = lookAt(Eye + Center, Center, Up);
	}
}

void CCamera::SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
	ProjectionMatrix = perspective(fovy, aspect, zNear, zFar);
}

mat4 CCamera::GetProjectionMatrix(void)
{
	return ProjectionMatrix;
}

void CCamera::Saving(void)
{
	std::ofstream file("Camera.txt");
	file << radius << ' ' << theta << ' ' << fi << ' ' << Center.x << ' ' << Center.y << ' ' << Center.z;
	file.close();
}