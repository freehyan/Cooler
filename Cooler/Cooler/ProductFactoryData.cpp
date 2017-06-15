#include "ProductFactoryData.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include "ProductFactory.h"

using namespace Cooler;

CProductFactoryData::~CProductFactoryData(void)
{
}

//***********************************************************
//FUNCTION:
CBaseProduct* CProductFactoryData::createProduct(const std::string& vSig)
{
	std::string Sig = boost::algorithm::to_upper_copy(vSig);
	if (!__isProductFactoryRegistered(Sig)) return nullptr;

	CBaseProduct *pProduct = m_ProductFactoryMap[Sig]->_createProductV();
	_ASSERT(pProduct);
	pProduct->__setProductCreationSig(Sig);
	return pProduct;
}

//***********************************************************
//FUNCTION:
CBaseProduct* CProductFactoryData::getOrCreateProduct(const std::string& vSig)
{
	std::string Sig = boost::algorithm::to_upper_copy(vSig);
	if (!__isProductFactoryRegistered(Sig)) return nullptr;

	CBaseProduct *pProduct = nullptr;
	if (m_ProductIndexedBySig.count(Sig) == 0)
	{
		pProduct = createProduct(vSig);
		_ASSERT(pProduct);
		m_ProductIndexedBySig[Sig] = pProduct;
	}
	else
	{
		pProduct = m_ProductIndexedBySig[Sig];
	}

	return pProduct;
}

//***********************************************************
//FUNCTION:
void CProductFactoryData::registerProductFactory(CProductFactory<CBaseProduct> *vProductFactory, const std::string& vSig)
{
	m_ProductFactoryMap[boost::algorithm::to_upper_copy(vSig)] = vProductFactory;
}

//***********************************************************
//FUNCTION:
bool CProductFactoryData::__isProductFactoryRegistered(const std::string& vSig) const
{
	return m_ProductFactoryMap.count(vSig) > 0;
}