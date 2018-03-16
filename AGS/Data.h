#pragma once
#include <cstdio>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CShader.h"
#include "CCamera.h"

extern CShader		Shader;
extern CCamera		Camera;

extern LARGE_INTEGER oldValue, newValue, frequency;
extern POINT newPossition, oldPossition;
extern double simulationTimePassed;