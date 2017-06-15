#include "ResourceExplorer.h"
#include "GraphicsCommon.h"
#include "GraphicsConfig.h"
#include "GraphicsMicro.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "ModelManager.h"
#include "FBOManager.h"

using namespace Cooler;

CResourceExplorer::CResourceExplorer()
{
}

CResourceExplorer::~CResourceExplorer()
{
	for (auto resourceManager : m_ResourceManagerSet)
		_SAFE_DELETE(resourceManager.second);
}

//***********************************************************
//FUNCTION:
void Cooler::CResourceExplorer::createAllResourceManager()
{
	m_ResourceManagerSet[CONFIG_KEYWORD::TEXTURE] = new CTextureManager;
	m_ResourceManagerSet[CONFIG_KEYWORD::FRAMEBUFFER] = new CFBOManager;
	m_ResourceManagerSet[CONFIG_KEYWORD::MODEL] = new CModelManager;
	m_ResourceManagerSet[CONFIG_KEYWORD::CAMERA] = new CCameraManager;
}

//***********************************************************
//FUNCTION:
void Cooler::CResourceExplorer::createResource(const CGraphicsConfig* vConfig) const
{
	_ASSERTE(vConfig);
	
	for (unsigned int i = 0; i < vConfig->getSubConfigNum(); i++)
	{
		CGraphicsConfig* pSubConfig = vConfig->fetchSubConfigAt(i);
		_ASSERTE(pSubConfig);
		auto Itr = m_ResourceManagerSet.find(pSubConfig->getType());
		if (Itr != m_ResourceManagerSet.end())
		{
			_ASSERTE(glGetError() == GL_NO_ERROR);
			Itr->second->createResource(pSubConfig);
		}
	}
}

//***********************************************************
//FUNCTION:
Cooler::IResourceManager* Cooler::CResourceExplorer::findResourceManager(const std::string& vResourceType)
{
	return (m_ResourceManagerSet.count(vResourceType) == 0) ? nullptr : m_ResourceManagerSet[vResourceType];
}
