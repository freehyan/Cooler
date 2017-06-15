#pragma once
#include <gl/glew.h>
#include "Resource.h"

namespace Cooler
{
	class CTexture : public IResource
	{
	public:
		CTexture();
		virtual ~CTexture();

		void setupTexture(GLenum vTarget, GLuint vWidth, GLuint vHeight, GLint vInternalFormat, GLenum vFormat, GLenum vType, GLfloat vWrapS, GLfloat vWrapT, GLfloat vFilterMin, GLfloat vFilterMax);
		bool loadImage(const std::string& vFile, GLint vFilterMin, GLint vFilterMax, GLint vWrapS, GLint vWrapT);
		void useTexture() const;
		void banTexture() const;

		GLuint getTextureId() const { return m_TextureObj; }

	private:
		GLuint m_TextureObj;
		GLenum m_TexTarget;

		GLuint64 m_TextureHandle;

	friend class CTexture;
	};
}