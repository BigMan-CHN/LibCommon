#include "Connection.h"
#include <vector>
#include <boost/bind.hpp>
#include "ConnectionManager.h"
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include "Service.h"
//#include "Logger.h"

//using namespace SVS::log;

namespace SVS
{

CConnection::CConnection(boost::asio::io_service& ioService,
						 CConnectionManager& manager, ICommandHandler * pCmdHandler)
						 : m_oSocket(ioService)
						 , m_oManager(manager)
						 , m_oRequestHandler(CRequestHandler(pCmdHandler))
{
}

boost::asio::ip::tcp::socket& CConnection::Socket()
{
	return m_oSocket;
}

void CConnection::Start()
{
	boost::asio::socket_base::keep_alive option(true);
	m_oSocket.set_option(option);

	m_oSocket.async_read_some(boost::asio::buffer(m_oBuffer),
		boost::bind(&CConnection::OnRead, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void CConnection::Stop()
{
	m_oSocket.close();
}

void CConnection::Shutdown()
{
	boost::system::error_code ignored_ec;
	m_oSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
}

//////////////////////////////////////////////////////////////////////////

void CConnection::OnRead(const boost::system::error_code& e,
						 std::size_t lSizeTransferred)
{
	if (!e)
	{
		try
		{
			RequestParseResult eResult = m_oRequestHandler.Parse(m_oBuffer.data(), lSizeTransferred);

			switch (eResult)
			{
			case eRequestParseContinue:
				{
					m_oSocket.async_read_some(boost::asio::buffer(m_oBuffer),
						boost::bind(&CConnection::OnRead, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
				}
				return;
			case eRequestParseEnd:
				{
					SVSBINARY binResponse;

					m_oRequestHandler.HandleRequest(binResponse, m_oSocket.remote_endpoint().address().to_string(),
						m_oSocket.remote_endpoint().port());

					if (Write(binResponse))
					{
						// 保持长连接，准备处理下一次请求
						m_oSocket.async_read_some(boost::asio::buffer(m_oBuffer),
							boost::bind(&CConnection::OnRead, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
					}
					else
					{
                        ///LogError("Write。。。。。。。。。。。。.error");
						m_oManager.Stop(this);
					}
				}
				return;
			default:
                  {}
                //LogError("Parse request error.");
			}	
		}
		catch (...)
		{
            ///LogError("Throw an exception.");
		}
	}

	if(e == boost::asio::error::eof)
	{
        ///LogDebug("Client closed this connect.");
	}
	else
	{
        ///LogWarning("OnRead error %s.", e.message().c_str());
	}

	if (e != boost::asio::error::operation_aborted)
	{
		m_oManager.Stop(this);
        ///LogDebug("Stop connect success.");
	}
}

bool CConnection::Write( SVSBINARY& binResponse )
{
	if (m_oSocket.is_open())
	{
		size_t len = binResponse.size();
		unsigned char * data = &binResponse[0];
		int status = 0;
		size_t lSended = 0;
		size_t lSize = 4096;
		boost::system::error_code ec;

		while ((len - lSended) > lSize)
		{
			m_oSocket.write_some(boost::asio::buffer(data+lSended, lSize), ec);
			if (ec)
			{
                ///LogError("Write data to client failed. - %s", ec.message().c_str());
				return false;
			}
			lSended += lSize;
			boost::xtime xt2;
            boost::xtime_get(&xt2, boost::TIME_UTC_);
			xt2.nsec += 15*1000000;         // Sleep for 1 second
			boost::thread::sleep(xt2);
		}
		m_oSocket.write_some(boost::asio::buffer(data+lSended, len - lSended), ec);
		if (ec)
		{
            ///LogError("Write data to client failed. - %s", ec.message().c_str());
			return false;
		}
	}

	return true;
}

} // namespace SVS
