#pragma once

#include <windows.h>
#include "stdio.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace glm;

const float PI = 3.14;
const float THETA_MAX = radians(85.0);
const float THETA_MIN = radians(5.0);

class CCamera
{
	
	// ������� ��������
	mat4 ProjectionMatrix;
	// ������� ����������
	mat4 ViewMatrix;

	vec3 Eye ;
	vec3 Center;
	vec3 Up;

	float radius;
	float fi;
	float theta;
	float Speed = 5.0;

public:
	// ����������� �� ���������
	CCamera(void);

	// ��������� ������� ��������
	void SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
	// ��������� ������� ��������
	mat4 GetProjectionMatrix(void);
	// �������� ������� ������ (����������)
	mat4 GetViewMatrix(void);
	// ����������� ������ � ����� ���������� � �������������� ��������� oXZ
	void MoveOXZ(float dForward, float dRight);
	// �������� � �������������� � ������������ ���������
	void Rotate(float dHorizAngle, float dVertAngle);
	// �����������/��������
	void GetUp();
	void Zoom(float dR);
	void Saving();
};
