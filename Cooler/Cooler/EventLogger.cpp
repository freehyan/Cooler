#include "EventLogger.h"
#include <sys/timeb.h>
#include <boost/format.hpp>
#include "GraphicsInterface.h"
#include "GraphicsMicro.h"
#include "OutputToScreen.h"

using namespace Cooler;

CEventLogger::CEventLogger(void) : m_EventOutputLevel(INFO_LEVEL)
{
	registerOutputTarget(OUTPUT_EVENT_2_FILE);
	registerOutputTarget(OUTPUT_EVENT_2_SCREEN);
	__outputAppStartEvent();

#ifdef _DEBUG
	m_BreakOnWarningHint = false;
#endif
}

CEventLogger::~CEventLogger(void)
{
	for (auto t : m_OutputTargets)
		_SAFE_DELETE(t.second);
}

//***********************************************************
//FUNCTION:
void CEventLogger::outputEvent(const std::string& vEvent, EEventLevel vLevel/*=INFO_LEVEL*/)
{
	if (vLevel < m_EventOutputLevel) return;

	std::string EventString = __assembleEventString(vEvent);
	for (std::map<std::string, COutputTarget*>::iterator itr = m_OutputTargets.begin(); itr != m_OutputTargets.end(); itr++)
	{
		if (itr->second->_isOutputEventV(vLevel)) itr->second->writeEvent(EventString);
	}
}

//***********************************************************
//FUNCTION:
void CEventLogger::outputWarning(const std::string& vSource, const std::string& vFile, int vLine, const std::string& vDescription)
{
	std::string Event = _BOOST_STR4("The application throws a warning:\n    Description: %1%\n    Source: %2%\n    File: %3%(%4%)",
		vDescription, vSource, vFile, vLine);

	outputEvent(Event, WARNING_LEVEL);

#ifdef _DEBUG
	if (m_BreakOnWarningHint) _ASSERT(false);
#endif
}

//***********************************************************
//FUNCTION:
std::string CEventLogger::__assembleEventString(const std::string& vEvent) const
{
	return _BOOST_STR2("%1% :\n  %2%\n", __assembleTimeString(), vEvent);
}

//***********************************************************
//FUNCTION:
std::string CEventLogger::__assembleTimeString() const
{
	struct _timeb Timebuffer;
	char Timeline[26];
	char TimeString[128];

	_ftime64_s(&Timebuffer);

	ctime_s(Timeline, 26, &(Timebuffer.time));
	for (int i = 25; i >= 0; i--)
	{
		if (Timeline[i] == '\n')
		{
			Timeline[i] = 0;
			break;
		}
	}
	sprintf_s(TimeString, 128, "[%.19s.%hu %s]", Timeline, Timebuffer.millitm, &Timeline[20]);
	return std::string(TimeString);
}

//***********************************************************
//FUNCTION:
void CEventLogger::registerOutputTarget(const std::string& vTargetSig)
{
	COutputTarget *pTarget = dynamic_cast<COutputTarget*>(getOrCreateProduct(vTargetSig));
	if (pTarget)
	{
		m_OutputTargets[pTarget->getProductCreationSig()] = pTarget;
	}
}

//***********************************************************
//FUNCTION:
void CEventLogger::__outputAppStartEvent()
{
	std::string EventString = std::string("\n\n/*************************************************************************************************************/\n");
	EventString += std::string("/*****************************Application starts at ") + __assembleTimeString();
	EventString += std::string("****************************/\n");

	for (std::map<std::string, COutputTarget*>::iterator itr = m_OutputTargets.begin(); itr != m_OutputTargets.end(); itr++)
	{
		if (!dynamic_cast<COutputToScreen*>(itr->second)) itr->second->writeEvent(EventString);
	}
}

//***********************************************************
//FUNCTION:
void CEventLogger::__outputAppEndEvent()
{
	std::string EventString = std::string("\n\n/**************************Application ends at ") + __assembleTimeString();
	EventString += std::string("*************************/\n");
	EventString += std::string("/*******************************************************************************************************************/\n");

	for (std::map<std::string, COutputTarget*>::iterator itr = m_OutputTargets.begin(); itr != m_OutputTargets.end(); itr++)
	{
		if (!dynamic_cast<COutputToScreen*>(itr->second)) itr->second->writeEvent(EventString);
	}
}
