#pragma once

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

// КЛАСС ДЛЯ РАБОТЫ С ТЕКСТУРОЙ
class CTexture
{
private:
	// индекс текстурного объекта
	GLuint TexIndex;
public:
	// режим фильтрации
	static int TexFilterMode;
	// загрузка текстуры из внешнего файла
	virtual void Load(const char *filename);
	// применение текстуры
	virtual void Apply(GLenum texUnit = GL_TEXTURE0);
};