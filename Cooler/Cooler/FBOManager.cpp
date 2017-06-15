#include "FBOManager.h"
#include <glm/glm.hpp>
#include "GraphicsCommon.h"
#include "GraphicsConfig.h"
#include "GraphicsInterface.h"
#include "FBO.h"

using namespace Cooler;

CFBOManager::CFBOManager()
{
}

CFBOManager::~CFBOManager()
{
}

//***********************************************************
//FUNCTION:
Cooler::IResource* Cooler::CFBOManager::__createResouceV(const CGraphicsConfig* vConfig) const
{
	_ASSERTE(vConfig);

//	std::string FBO_SIG = vConfig->getAttribute<std::string>(CONFIG_KEYWORD::FBO_SIG);
	CFBO* pFBO = dynamic_cast<CFBO*>(getOrCreateProduct(vConfig->getName()));
	_ASSERTE(pFBO);

	glm::vec2 FBOSize = vConfig->getAttribute<glm::vec2>(CONFIG_KEYWORD::FBO_SIZE);
	bool bStencil = vConfig->getAttribute<bool>(CONFIG_KEYWORD::FBO_STENCIL);

	pFBO->createFBO(FBOSize.x, FBOSize.y, bStencil);

	if (vConfig->getSubConfigNum() > 0)
	{
		Cooler::CGraphicsConfig* pConfig = vConfig->fetchSubConfigAt(0);
		if (pConfig->getType() == CONFIG_KEYWORD::FBO_ATTACHED_TEXTURE)
		{
			std::string Str = "FBO_ATTACHED_TEXTURE_";
			for (unsigned int i = 0; i < pConfig->getAttributeNum(); i++)
			{
				std::string FBOTextureStr = Str + std::to_string(i);
				std::vector<std::string> FBOTexture = pConfig->getAttribute<std::vector<std::string>>(FBOTextureStr);

				if (FBOTexture.size() > 1)
				{
					GLint TextureInternelFormat = interpretString2GraphicsEnum<GLint>(FBOTexture[1]);
					GLint TextureFormat = interpretString2GraphicsEnum<GLint>(FBOTexture[2]);
					pFBO->addTextureV(FBOTexture[0], TextureInternelFormat, TextureFormat);
				}
				else
					pFBO->addTextureV(FBOTexture[0]);
			}
		}
	}
	pFBO->attachTex2FBO();
	return pFBO;
}
