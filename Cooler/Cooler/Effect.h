#pragma once
#include <map>
#include <boost/any.hpp>
#include "GraphicsExport.h"
#include "BaseProduct.h"

namespace Cooler
{
	class CGraphicsConfig;
	class CShader;
	class CScene;
	class CInputTransformer;

	class COOLER_COMMON_DLL_EXPORT CEffect : public CBaseProduct
	{
	public:
		enum EEffectState
		{
			EFFECT_OFF = 0,
			EFFECT_ON
		};

	public:
		CEffect();
		virtual ~CEffect();

		void loadEffect();
		void enableEffect();
		void disableEffect();
		
		bool initEffectWithConfig(const CGraphicsConfig* vEffectConfig);
		
		CGraphicsConfig* fetchEffectConfig() const { return m_pEffectConfig; }

	protected:
		virtual void _renderEffectV() {}
		virtual void _initEffectV()   {}
		virtual void _destoryEffectV(){}

		bool _enableShader(const std::string& vShaderName);
		bool _disableShader(const std::string& vShaderName);

		void _enableTexture(const std::string& vTextueName, unsigned int vTexUnitId);
		void _disableTexture(const std::string& vTextueName, unsigned int vTexUnitId);

		void _updateShaderUniform(const std::string& vUniformName, boost::any vValue);
		void _updateShaderUniformfv(const std::string& vUniformName, unsigned int vCount, float* vVlaue);

		unsigned int _getShaderId(const std::string& vShaderName);

	private:
		bool __initEffectShader(const CGraphicsConfig* vShaderConfig);

		EEffectState m_EffectState;
		std::string m_EffectSig;

		std::map<std::string, CShader*> m_ShaderMap;
		CShader* m_pCurrentShader;
		CGraphicsConfig* m_pEffectConfig;

	friend class IRender;
	friend class CEffectManager;
	};
}