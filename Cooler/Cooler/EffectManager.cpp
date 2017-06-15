#include "EffectManager.h"
#include "GraphicsCommon.h"
#include "GraphicsConfig.h"
#include "ProductFactory.h"
#include "Effect.h"

using namespace Cooler;
CEffectManager::CEffectManager()
{
}

CEffectManager::~CEffectManager()
{

}

//***********************************************************
//FUNCTION::
bool Cooler::CEffectManager::createAllEffect(const CGraphicsConfig* vEffectConfig, std::map<std::string, CEffect*>& voEffectMap)
{
	_ASSERT(vEffectConfig);
	for (unsigned int i = 0; i < vEffectConfig->getSubConfigNum(); i++)
	{
		CGraphicsConfig* pConfig = vEffectConfig->fetchSubConfigAt(i);
		std::string EffectSig = pConfig->getName();
		CEffect* pEffect = dynamic_cast<CEffect*>(CProductFactoryData::getInstance()->createProduct(EffectSig));
		_ASSERTE(pEffect);
		if (!pEffect->initEffectWithConfig(pConfig)) continue;
		voEffectMap.insert(std::make_pair(EffectSig, pEffect));
	}

	return true;
}
