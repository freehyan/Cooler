#include "FBO.h"
#include "GraphicsInterface.h"
#include "GraphicsMicro.h"

using namespace Cooler;

CFBO::CFBO() : m_FBO(0), m_DepthTex(0), m_Width(0), m_Height(0)
{
}

CFBO::~CFBO()
{
	if (m_FBO != 0) glDeleteFramebuffers(1, &m_FBO);
	for (unsigned int i=0; i<m_TexBuffers.size(); i++)
		glDeleteTextures(1, &m_TexBuffers[i]);
	glDeleteTextures(1, &m_DepthTex);
}

//***********************************************************
//FUNCTION:
void Cooler::CFBO::createFBO(unsigned int vWidth, unsigned int vHeight, bool vEnableStencil /*= false*/)
{
	m_Width = vWidth;
	m_Height = vHeight;
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	glGenTextures(1, &m_DepthTex);
	glBindTexture(GL_TEXTURE_2D, m_DepthTex);

	if (!vEnableStencil)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, vWidth, vHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, vWidth, vHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTex, 0);

	__checkFramebufferStatus();

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//***********************************************************
//FUNCTION:
void Cooler::CFBO::attachTex2FBO()
{
	__checkFramebufferStatus();
	__drawFBOBuffers();
}

//***********************************************************
//FUNCTION:
void Cooler::CFBO::openFBO() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

//***********************************************************
//FUNCTION:
void Cooler::CFBO::closeFBO() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//***********************************************************
//FUNCTION:
void Cooler::CFBO::bindFBOTex(GLenum vTextureUnit, const std::string& vTexName)
{
// 	glActiveTexture(vTextureUnit);
// 	glEnable(GL_TEXTURE_2D);
// 	GLuint TexId = getFBOTexId(vTexName);
// 	_ASSERTE(TexId);
// 	glBindTexture(GL_TEXTURE_2D, TexId);
}

//***********************************************************
//FUNCTION:
// GLuint Cooler::CFBO::getFBOTexId(const std::string& vTexName) const
// {
// // 	auto Itr = m_TexBuffers.find(vTexName);
// // 	if (Itr != m_TexBuffers.end())
// // 		return Itr->second;
// // 	else
// 		return 0;
// }

//***********************************************************
//FUNCTION:
void Cooler::CFBO::_generateTexture(GLuint& vTextureID, GLint vInternalFormat, GLenum vFormat)
{
	glGenTextures(1, &vTextureID);
	glBindTexture(GL_TEXTURE_2D, vTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, vInternalFormat, m_Width, m_Height, 0, vFormat, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_TexBuffers.push_back(vTextureID);
}

//***********************************************************
//FUNCTION:
bool Cooler::CFBO::__checkFramebufferStatus()
{
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (Status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		return true;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		Cooler::outputWarning(__EXCEPTION_SITE__, "[ERROR] Frame buffer incomplete: Attachment is NOT complete.");
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		Cooler::outputWarning(__EXCEPTION_SITE__, "[ERROR] Frame buffer incomplete: No image is attached to FBO.");
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		Cooler::outputWarning(__EXCEPTION_SITE__, "[ERROR] Frame buffer incomplete: Draw buffer.");
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		Cooler::outputWarning(__EXCEPTION_SITE__, "[ERROR] Frame buffer incomplete: Read buffer.");
		return false;

	case GL_FRAMEBUFFER_UNSUPPORTED:
		Cooler::outputWarning(__EXCEPTION_SITE__, "[ERROR] Frame buffer incomplete: Unsupported by FBO implementation.");
		return false;

	default:
		Cooler::outputWarning(__EXCEPTION_SITE__, "[ERROR] Frame buffer incomplete: Unknown error.");
		return false;
	}
}

//***********************************************************
//FUNCTION:
void Cooler::CFBO::__drawFBOBuffers()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	GLenum DrawBuffers[10] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7, GL_COLOR_ATTACHMENT8,
		GL_COLOR_ATTACHMENT9 };
	unsigned int TexBufferCount = m_TexBuffers.size();

	for (unsigned int i = 0; i < TexBufferCount; i++)
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_TexBuffers[i], 0);

	glDrawBuffers(TexBufferCount, DrawBuffers);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
