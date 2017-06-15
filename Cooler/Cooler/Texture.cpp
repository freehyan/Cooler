#include "Texture.h"
#include <SOIL/SOIL.h>

using namespace Cooler;

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
	glDeleteTextures(1, &m_TextureObj);
}

//***********************************************************
//FUNCTION:
void Cooler::CTexture::setupTexture(GLenum vTarget, GLuint vWidth, GLuint vHeight, GLint vInternalFormat, GLenum vFormat, GLenum vType, GLfloat vWrapS, GLfloat vWrapT, GLfloat vFilterMin, GLfloat vFilterMax)
{
	glGenTextures(1, &m_TextureObj);
	glBindTexture(vTarget, m_TextureObj);
	glTexImage2D(vTarget, 0, vInternalFormat, vWidth, vHeight, 0, vFormat, vType, nullptr);
	glTexParameterf(vTarget, GL_TEXTURE_WRAP_S, vWrapS);
	glTexParameterf(vTarget, GL_TEXTURE_WRAP_T, vWrapT);
	glTexParameterf(vTarget, GL_TEXTURE_MIN_FILTER, vFilterMin);
	glTexParameterf(vTarget, GL_TEXTURE_MAG_FILTER, vFilterMax);
	glBindTexture(vTarget, 0);
	m_TexTarget = vTarget;
	_ASSERTE(glGetError() == GL_NO_ERROR);
}

//***********************************************************
//FUNCTION:
bool Cooler::CTexture::loadImage(const std::string& vFile, GLint vFilterMin, GLint vFilterMax, GLint vWrapS, GLint vWrapT)
{
	_ASSERTE(glGetError() == GL_NO_ERROR);
	glGenTextures(1, &m_TextureObj);
	glBindTexture(GL_TEXTURE_2D, m_TextureObj);
	// Set texture wrapping 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, vWrapS);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vWrapT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, vFilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, vFilterMax);
	
	int iWidth = 0, iHeight = 0;
	unsigned char* pImage = SOIL_load_image(vFile.c_str(), &iWidth, &iHeight, 0, SOIL_LOAD_RGB);
	_ASSERTE(pImage);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage);

//	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(pImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_TexTarget = GL_TEXTURE_2D;
	_ASSERTE(glGetError() == GL_NO_ERROR);
	return true;
}

//***********************************************************
//FUNCTION::
void Cooler::CTexture::useTexture() const
{
	glBindTexture(m_TexTarget, m_TextureObj);
}

//***********************************************************
//FUNCTION::
void Cooler::CTexture::banTexture() const
{
	glBindTexture(m_TexTarget, 0);
}