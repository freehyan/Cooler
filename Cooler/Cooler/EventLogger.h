#pragma once
#include "Singleton.h"
#include "GraphicsMicro.h"
#include "GraphicsCommon.h"

namespace Cooler
{
	class COutputTarget;

	class CEventLogger : public CSingleton<CEventLogger>
	{
	public:
		~CEventLogger(void);

		void registerOutputTarget(const std::string& vTargetSig);
		void outputWarning(const std::string& vSource, const std::string& vFile, int vLine, const std::string& vDescription);
		void outputEvent(const std::string& vEvent, EEventLevel vLevel = INFO_LEVEL);
		void setEventOutputLevel(EEventLevel vLevel) { m_EventOutputLevel = vLevel; }

#ifdef _DEBUG
		void setBreakOnWarningHint() { m_BreakOnWarningHint = true; }
#endif

	private:
		CEventLogger(void);

		void __outputAppStartEvent();
		void __outputAppEndEvent();

		std::string __assembleTimeString() const;
		std::string __assembleEventString(const std::string& vEvent) const;

		std::map<std::string, COutputTarget*> m_OutputTargets;
		EEventLevel m_EventOutputLevel;
#ifdef _DEBUG
		bool        m_BreakOnWarningHint;
#endif

		friend class CSingleton<CEventLogger>;
	};
}