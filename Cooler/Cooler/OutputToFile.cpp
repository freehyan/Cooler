#include "OutputToFile.h"
#include "ProductFactory.h"

using namespace Cooler;
CProductFactory<COutputToFile> Creator(OUTPUT_EVENT_2_FILE);

COutputToFile::COutputToFile() : m_LogFileName("Events.log"), m_BufferMaxSize(5000)
{
	m_Buffer.reserve(m_BufferMaxSize);
	m_LogFile.open(m_LogFileName.c_str(), std::ios::out | std::ios::app);
	_ASSERT(m_LogFile.is_open());

#ifdef _DEBUG
	m_BufferMaxSize = 0;
#endif
}

COutputToFile::~COutputToFile()
{
	if (!m_Buffer.empty() && m_LogFile.is_open())
	{
		m_LogFile << m_Buffer;
		_ASSERT(!m_LogFile.bad());
		m_LogFile.close();
	}
}

//*********************************************************************************
//FUNCTION:
void COutputToFile::_outputEventV(const std::string& vEvent)
{
	boost::mutex::scoped_lock Lock(m_Mutex);

	m_Buffer.append(vEvent);
	m_Buffer.append(1, '\n');
	if (m_Buffer.size() > m_BufferMaxSize)
	{
		m_LogFile << m_Buffer;
		m_LogFile.flush();
		_ASSERT(!m_LogFile.bad());
		m_Buffer.clear();
	}
}