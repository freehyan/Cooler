#pragma once
#include "Singleton.h"

namespace Cooler
{
	class CGraphicsConfig;
	class CEffect;

	class CEffectManager : public CSingleton<CEffectManager>
	{
	public:
		virtual ~CEffectManager(void);

		bool createAllEffect(const CGraphicsConfig* vEffectConfig, std::map<std::string, CEffect*>& voEffectMap);

	private:
		CEffectManager(void);

	private:
		friend class CSingleton<CEffectManager>;
	};
}