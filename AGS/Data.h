#pragma once
#include <cstdio>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CShader.h"
#include "CCamera.h"
#include "CMesh.h"
#include "ÑResourceManager.h"

extern CShader		Shader;
extern CCamera		Camera;
extern CMesh		Mesh;

extern LARGE_INTEGER oldValue, newValue, frequency;
extern POINT newPossition, oldPossition;
extern double simulationTimePassed;

extern int MeshId[3];

void MeshesInit();
void ShaderInit();