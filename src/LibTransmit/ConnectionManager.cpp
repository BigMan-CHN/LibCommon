#include "ConnectionManager.h"
#include <algorithm>
#include <boost/bind.hpp>
//#include "Logger.h"

//using namespace SVS::log;

namespace SVS
{

void CConnectionManager::Start(CConnectionPtr pConn)
{
	boost::mutex::scoped_lock lock(m_mutexFunc);

	m_oConns.insert(pConn);
	pConn->Start();
    ///LogDebug("Start connect.");
}

void CConnectionManager::Stop(CConnectionPtr pConn)
{
	boost::mutex::scoped_lock lock(m_mutexFunc);

	std::set<CConnectionPtr>::iterator it = m_oConns.find(pConn);
	if (it != m_oConns.end())
	{
		m_oConns.erase(it);
		pConn->Stop();
		delete pConn;
        ///LogDebug("Stop connect.");
	}
}

void CConnectionManager::StopAll()
{
	boost::mutex::scoped_lock lock(m_mutexFunc);

	std::for_each(m_oConns.begin(), m_oConns.end(),
		boost::bind(&CConnection::Shutdown, _1));

	for (std::set<CConnectionPtr>::iterator it = m_oConns.begin();
		it != m_oConns.end(); it++)
	{
		CConnectionPtr pConn = *it;
		delete pConn;
		pConn = NULL;
	}

	m_oConns.clear();
}

} // namespace SVS
