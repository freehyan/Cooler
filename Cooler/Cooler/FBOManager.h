#pragma once
#include "ResourceManager.h"

namespace Cooler
{
	class CFBOManager : public IResourceManager
	{
	public:
		CFBOManager();
		virtual ~CFBOManager();

	private:
		virtual IResource* __createResouceV(const CGraphicsConfig* vConfig) const override;
	};
}