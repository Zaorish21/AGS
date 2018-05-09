#include "CCubeTexture.h"

void CCubeTexture::Load(std::string FileName)
{
	ILuint TextureID = ilGenImage();
	ilBindImage(TextureID);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &TexIndex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TexIndex);

	std::string CubeTexturesSide[6];
	CubeTexturesSide[0] = FileName + "_Back.png";
	CubeTexturesSide[1] = FileName + "_Front.png";
	CubeTexturesSide[2] = FileName + "_Left.png";
	CubeTexturesSide[3] = FileName + "_Right.png";
	CubeTexturesSide[4] = FileName + "_Up.png";
	CubeTexturesSide[5] = FileName + "_Down.png";

	wchar_t Unicode_String[256];
	wsprintf(Unicode_String, L"%S", CubeTexturesSide[0].c_str());
	bool rezult = ilLoadImage(Unicode_String);

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);
	float *Data = new float[width*height*4];
	
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_FLOAT, Data);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, Data);
	
	wsprintf(Unicode_String, L"%S", CubeTexturesSide[1].c_str());
	rezult = ilLoadImage(Unicode_String);
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_FLOAT, Data);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, Data);

	wsprintf(Unicode_String, L"%S", CubeTexturesSide[2].c_str());
	rezult = ilLoadImage(Unicode_String);
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_FLOAT, Data);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, Data);

	wsprintf(Unicode_String, L"%S", CubeTexturesSide[3].c_str());
	rezult = ilLoadImage(Unicode_String);
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_FLOAT, Data);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, Data);

	wsprintf(Unicode_String, L"%S", CubeTexturesSide[4].c_str());
	rezult = ilLoadImage(Unicode_String);
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_FLOAT, Data);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, Data);

	wsprintf(Unicode_String, L"%S", CubeTexturesSide[5].c_str());
	rezult = ilLoadImage(Unicode_String);
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_FLOAT, Data);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, Data);
	
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CCubeTexture::Apply(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP,TexIndex);
}