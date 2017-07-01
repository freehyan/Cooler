#pragma once
#include <string>
#include <map>
#include <functional>
#include "GraphicsExport.h"
#include "Singleton.h"
#include "PlatformContext.h"

namespace Cooler
{
	class CGraphicsConfig;
	class CDisplayWindow;
	class CEffect;
	class CInputTransformer;
	class CAntTweakBar;

	class IRender : public CSingleton<IRender>, public CInputCallbacks
	{
	public:
		IRender();
		~IRender();

		void init();
		void run();

		CInputTransformer* fetchInputTransformer() const;
		CDisplayWindow*    fetchDisplayWindow() const;
		CAntTweakBar* fetchAntTweakBar() const;

		// Do not override these virtuals - overide the "handle" ones above
		bool pointerInput(SInputDeviceType::Enum device, SPointerActionType::Enum action, uint32_t modifiers, int32_t count, CPointerEvent* points, int64_t timestamp = 0); // we have base impl.
		bool keyInput(uint32_t code, SKeyActionType::Enum action);

	protected:
//		virtual void _initRenderV() {}
//		virtual void _renderV()		{}

		bool _renderEffect(const std::string& vEffectSig);

	private:
		void __loadConfig();
		void __createResources();
		bool __initEnvironment();
		bool __initAllEffect();
		void __destroy();
		void __updateWindowSize();

	private:
		double m_DeltaTime, m_LastTime;

		CDisplayWindow*    m_pDisplayWindow;
		CAntTweakBar*      m_pAntTweakBar;
		CInputTransformer* m_pInputTransformer;

		std::map<std::string, CEffect*>         m_EffectMap;
		std::map<std::string, CGraphicsConfig*> m_ConfigSet;

	friend class CSingleton<IRender>;
	};
}