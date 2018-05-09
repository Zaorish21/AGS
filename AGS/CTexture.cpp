#include "CTexture.h"
#include <iostream>
void CTexture::Load(const char * filename)
{
	ILuint TextureID = ilGenImage();
	ilBindImage(TextureID);
	
	wchar_t Unicode_String[256];
	wsprintf(Unicode_String, L"%S", filename);
	bool rezult = ilLoadImage(Unicode_String);

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);

	int size = width * height * 4;
	float *Data = new float[size];

	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_FLOAT, Data);
	glGenTextures(1, &TexIndex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexIndex);
	
	glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, Data);
	glGenerateMipmap(GL_TEXTURE_2D);
	delete[] Data;
	ilDeleteImage(TextureID);
}

void CTexture::Apply(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, TexIndex);
}
