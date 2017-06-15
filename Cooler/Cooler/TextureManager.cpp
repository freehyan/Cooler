#include "TextureManager.h"
#include <glm/glm.hpp>
#include "GraphicsCommon.h"
#include "GraphicsConfig.h"
#include "GraphicsInterface.h"
#include "Texture.h"

using namespace Cooler;

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
}

//***********************************************************
//FUNCTION:
Cooler::IResource* Cooler::CTextureManager::__createResouceV(const CGraphicsConfig* vConfig) const
{
	_ASSERTE(glGetError() == GL_NO_ERROR);
	_ASSERTE(vConfig);
	CTexture* pTexture = new CTexture;
	_ASSERTE(pTexture);

	std::vector<std::string> TextureWrap = vConfig->getAttribute<std::vector<std::string>>(CONFIG_KEYWORD::TEXTURE_WRAP);
	GLint TextureWrapS = interpretString2GraphicsEnum<GLint>(TextureWrap[0]);
	GLint TextureWrapT = interpretString2GraphicsEnum<GLint>(TextureWrap[1]);

	std::vector<std::string> TextureFilter = vConfig->getAttribute<std::vector<std::string>>(CONFIG_KEYWORD::TEXTURE_FILTER);
	GLint TextureFilterMin = interpretString2GraphicsEnum<GLint>(TextureFilter[0]);
	GLint TextureFilterMax = interpretString2GraphicsEnum<GLint>(TextureFilter[1]);
	
	if (vConfig->isAttributeExisted(CONFIG_KEYWORD::TEXTURE_PATH))
	{
		pTexture->loadImage(vConfig->getAttribute<std::string>(CONFIG_KEYWORD::TEXTURE_PATH), TextureFilterMin, TextureFilterMax, TextureWrapS, TextureWrapT);
	}
	_ASSERTE(glGetError() == GL_NO_ERROR);
	return pTexture;
}