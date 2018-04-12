#pragma once

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

// ����� ��� ������ � ���������
class CTexture
{
private:
	// ������ ����������� �������
	GLuint TexIndex;
public:
	// ����� ����������
	static int TexFilterMode;
	// �������� �������� �� �������� �����
	virtual void Load(const char *filename);
	// ���������� ��������
	virtual void Apply(GLenum texUnit = GL_TEXTURE0);
};