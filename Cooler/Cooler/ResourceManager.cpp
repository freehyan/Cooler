#include "ResourceManager.h"
#include "GraphicsMicro.h"
#include "GraphicsConfig.h"
#include "Resource.h"

using namespace Cooler;

IResourceManager::IResourceManager()
{
}

IResourceManager::~IResourceManager()
{
	for (auto resource : m_Key2ResourceMap)
		_SAFE_DELETE(resource.second);
}

//***********************************************************
//FUNCTION:
Cooler::IResource* Cooler::IResourceManager::createResource(const CGraphicsConfig* vConfig)
{
	_ASSERTE(vConfig);

	std::string ResouceKey = vConfig->getType();
	_ASSERTE(glGetError() == GL_NO_ERROR);
	IResource* pResouce = __createResouceV(vConfig);
	_ASSERTE(pResouce);
	m_Key2ResourceMap[vConfig->getName()] = pResouce;
//	pResouce->_onResourceCreatedV(vConfig);

	return pResouce;
}

//***********************************************************
//FUNCTION::
Cooler::IResource* Cooler::IResourceManager::fetchResource(const std::string& vResourceKey)
{
	_ASSERTE(!vResourceKey.empty());
	if (m_Key2ResourceMap.find(vResourceKey) != m_Key2ResourceMap.end())
		return m_Key2ResourceMap[vResourceKey];
	else
		return nullptr;
}