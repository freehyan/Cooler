#pragma once
#include <map>
#include <string>
#include "GraphicsExport.h"
#include "Singleton.h"

namespace Cooler
{
	class CBaseProduct;

	template<typename TDerivedProduct>
	class CProductFactory;

	class COOLER_COMMON_DLL_EXPORT CProductFactoryData : public CSingleton<CProductFactoryData>
	{
	public:
		~CProductFactoryData(void);

		CBaseProduct* createProduct(const std::string& vSig);
		CBaseProduct* getOrCreateProduct(const std::string& vSig);

		void registerProductFactory(CProductFactory<CBaseProduct> *vProductFactory, const std::string& vSig);

	protected:
		CProductFactoryData(void) {}

	private:
		bool __isProductFactoryRegistered(const std::string& vSig) const;

		std::map<std::string, CBaseProduct*>				  m_ProductIndexedBySig;
		std::map<std::string, CProductFactory<CBaseProduct>*> m_ProductFactoryMap;

		friend class CSingleton<CProductFactoryData>;
	};

}