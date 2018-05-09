#pragma once

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

#include "CTexture.h"

class CCubeTexture: public CTexture
{
	GLuint TexIndex;
public:
	static int TexFilterMode;
	void Load(std::string Filename);
	void Apply(GLenum texUnit = GL_TEXTURE0);
};

