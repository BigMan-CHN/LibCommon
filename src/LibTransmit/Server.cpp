#include "Server.h"
#include <iostream>
//#include "Logger.h"

//using namespace SVS::log;

namespace SVS
{

CServer::CServer(SVSLONG16 lThreadPoolSize, SVSSTRING sFileDir)
	: m_oService(this, lThreadPoolSize, sFileDir)
	, m_bRunning(false)
{
}

CServer::~CServer(void)
{
}

bool CServer::Register( IModule * pModule )
{
	assert(pModule);

	MODULES::iterator it = m_oModules.find(pModule->GetName());
	if (it != m_oModules.end())
	{
		return false;
	}

	m_oModules[pModule->GetName()] = pModule;
	return true;
}

void CServer::UnRegisterAll()
{
	assert(!m_bRunning);

	for (MODULES::iterator it = m_oModules.begin(); it != m_oModules.end(); it++)
	{
		delete it->second;
	}
	m_oModules.clear();
}

bool CServer::Start(const int& nPort, const std::string& sAddress)
{
	assert(!m_bRunning);

	try
	{
		m_bRunning = m_oService.Start(nPort, sAddress);
	}
	catch (std::exception& e)
	{
        ///LogError("Start error: %s.", e.what());
	}

	return m_bRunning;
}

void CServer::Stop()
{
	if (m_bRunning)
	{
		m_oService.Stop();
		m_bRunning = false;
        ///LogInfo("Transmit Server is stopped!");
	}
}

void CServer::Run()
{
	assert(m_bRunning);

	m_oService.Run();
}

//////////////////////////////////////////////////////////////////////////

ResponseCode CServer::Call( const SVS::SVSSTRING& sModule, const SVS::SVSSTRING& sMethod,
						   SVS::CData& oParams, SVS::CData& oResult )
{
	//LogDebug("Server is ready to Call [%s] - %s.",sModule.c_str(), sMethod.c_str());
	MODULES::iterator it = m_oModules.find(sModule);
	if (it != m_oModules.end())
	{
		ResponseCode code = it->second->Execute(sMethod, oParams, oResult);
		//LogInfo("Server finished to Call [%s] - %s.",sModule.c_str(), sMethod.c_str());
		p_code=code;
		return code;
	}
	else
	{
		p_code=eResponseCodeNoModule;
        ///LogWarning("No module [%s]in server - %s.", sModule.c_str(), sMethod.c_str());
		return eResponseCodeNoModule;
	}
}

}
