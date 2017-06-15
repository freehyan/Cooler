#pragma once
#include <boost/algorithm/string.hpp>
#include "GraphicsExport.h"

namespace Cooler
{
	class COOLER_COMMON_DLL_EXPORT CBaseProduct
	{
	public:
		CBaseProduct() {}
		virtual ~CBaseProduct() {}

		const std::string& getProductCreationSig() const { return m_ProductCreationSig; }

	private:
		void __setProductCreationSig(const std::string& vSig) { m_ProductCreationSig = boost::to_upper_copy(vSig);}

		std::string m_ProductCreationSig;

	friend class CProductFactoryData;
	};
}
