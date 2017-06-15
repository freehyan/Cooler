#pragma once
#include <string>
#include "BaseProduct.h"
#include "GraphicsExport.h"

namespace Cooler
{
	class CGraphicsConfig;

	class COOLER_COMMON_DLL_EXPORT IResource : public CBaseProduct
	{
	public:
		IResource();
		virtual	~IResource();

	protected:
		virtual bool _onResourceCreatedV(const CGraphicsConfig* vConfig) { return true; }

	private:
	//	std::string m_ResourceName;

	friend class IResourceManager;
	};
}
