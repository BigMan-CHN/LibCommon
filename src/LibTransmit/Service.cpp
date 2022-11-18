#include "Service.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
//#include "Logger.h"

//using namespace SVS::log;

namespace SVS
{

CService::CService( ICommandHandler * pCmdHandler, std::size_t threadPoolSize, const std::string sFileDir)
	: m_ioService()
	, m_oAcceptor(m_ioService)
	, m_oManager()
	, m_pCmdHandler(pCmdHandler)
	, m_pConn(new CConnection(m_ioService, m_oManager, pCmdHandler))
	, m_uPoolSize(threadPoolSize)
{
}

CService::~CService(void)
{
	if (m_pConn)
	{
		delete m_pConn;
		m_pConn = NULL;
	}
}

bool CService::Start( const int& nPort, const std::string& sAddress )
{
	boost::asio::ip::tcp::endpoint endpoint;

	if (sAddress != "")
	{
		endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(sAddress),
			nPort);
	}
	else
	{
		endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(),
			nPort);
	}

	boost::system::error_code ec;
	m_oAcceptor.open(endpoint.protocol(), ec);
	if (ec)
	{
        ///LogError("Open %s:%d failed.", sAddress.c_str(), nPort);
		return false;
	}
	m_oAcceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_oAcceptor.bind(endpoint, ec);
	if (ec)
	{
        ///LogError("Bind %s:%d failed.", sAddress.c_str(), nPort);
		return false;
	}
	m_oAcceptor.listen(boost::asio::socket_base::max_connections, ec);
	if (ec)
	{
        ///LogError("Listen %s:%d failed.", sAddress.c_str(), nPort);
		return false;
	}
	m_oAcceptor.async_accept(m_pConn->Socket(),
		boost::bind(&CService::OnAccept, this, boost::asio::placeholders::error));

	return true;
}

void CService::Stop()
{
	m_ioService.post(boost::bind(&CService::OnStop, this));
}

void CService::Run()
{
	// Create a pool of threads to run all of the io_services.
	boost::thread_group pools;
	for (std::size_t i = 0; i < m_uPoolSize; ++i)
	{
		pools.create_thread(boost::bind(&boost::asio::io_service::run, &m_ioService));
	}

	pools.join_all();
}

void CService::OnAccept( const boost::system::error_code& e )
{
    ///LogDebug("Receive a connect.");

	if (!e)
	{
		m_oManager.Start(m_pConn);
		m_pConn = new CConnection(m_ioService, m_oManager, m_pCmdHandler);
		m_oAcceptor.async_accept(m_pConn->Socket(),
			boost::bind(&CService::OnAccept, this,
			boost::asio::placeholders::error));
        ///LogDebug("Continue to listen.");

		return;
	}
	
	if (e == boost::asio::error::connection_aborted)
	{
        ///LogDebug("Client aborted.");
	}
	else if (e == boost::asio::error::operation_aborted)
	{
        ///LogDebug("Server aborted.");
	}
	else
	{
        ///LogError("Accept error %s.", e.message().c_str());
	}
    ///LogWarning("Exit listen.");
}

void CService::OnStop()
{
	m_oAcceptor.close();
	m_oManager.StopAll();
    ///LogInfo("Service is stopped!");
}
} // namespace SVS
