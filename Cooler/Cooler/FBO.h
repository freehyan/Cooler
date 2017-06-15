#pragma once
#include <vector>
#include <gl/glew.h>
#include "Resource.h"
#include "GraphicsExport.h"

namespace Cooler
{
	class COOLER_COMMON_DLL_EXPORT CFBO : public IResource
	{
	public:
		CFBO();
		virtual ~CFBO();

		virtual void addTextureV(const std::string& vTexName, GLint vInternalFormat = GL_RGBA32F, GLenum vFormat = GL_RGBA) = 0;

		void createFBO(unsigned int vWidth, unsigned int vHeight, bool vEnableStencil = false);
		void attachTex2FBO();

		void openFBO() const;
		void closeFBO() const;

		void bindFBOTex(GLenum vTextureUnit, const std::string& vTexName);
		GLuint getFBOTexId(const std::string& vTexName) const;
		inline GLuint getFBODepth() const { return m_DepthTex; }

	protected:
		void _generateTexture(GLuint& vTextureID, GLint vInternalFormat, GLenum vFormat);
	
	private:
		bool __checkFramebufferStatus();
		void __drawFBOBuffers();

		GLuint m_Width, m_Height;
		GLuint m_FBO, m_DepthTex;
		std::vector<GLuint> m_TexBuffers;
	};
}