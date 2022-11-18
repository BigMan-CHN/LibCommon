#include "TransmitClient.h"
//#include "Logger.h"

using namespace boost::asio;
using boost::asio::ip::tcp;
//using namespace SVS::log;

namespace SVS
{

CTransmitClient::CTransmitClient(unsigned long lTimeout)
	: _ioService()
	, _timer(_ioService)
	, _socket(_ioService)
	, _executed(false)
	, _done(false)
	, _limitTime(lTimeout)
	, _ip("")
	, _port(0)
	, _hasHeader(false)
	, _recved(0)
	, p_code(eResponseCodeErrorTransmit)
	, conntype(1)
{
}

CTransmitClient::~CTransmitClient(void)
{
	CloseConnect();
}

void CTransmitClient::SetTimeout(unsigned long lTimeout)
{
	//boost::mutex::scoped_lock scopedLock(_funcMutex);

	_limitTime = lTimeout;
}

bool CTransmitClient::Connect( std::string sIP, unsigned int nPort,unsigned int proto)
{
    ///LogDebug("In Connect(...).");
	//boost::mutex::scoped_lock scopedLock(_funcMutex);

    ///LogDebug("Be ready to Connect(...).");
	_ip = sIP;
	_port = nPort;
	conntype=proto;


	return Connect();
}

bool CTransmitClient::IsConnect()
{
    ///LogDebug("In IsConnect.");
	//boost::mutex::scoped_lock scopedLock(_funcMutex);

    ///LogDebug("Be ready to Connect.");
	if (!_socket.is_open())
	{
        ///LogWarning("Is not Connected.");
		return false;
	}
/*
	
		TransmitHeader header;
		ResetTransmitHeader(header);
		header.eType = (SVSLONG16)eRequestTypeConnect;
		header.lHeaderSize = 0;
	
		if (boost::asio::write(_socket, boost::asio::buffer(&header, SKYVIS_HEADER_SIZE)) != SKYVIS_HEADER_SIZE)
		{
			return false;
			}*/
    ///LogDebug("Is Connected.");
	return true;
}

void CTransmitClient::Close()
{
	//boost::mutex::scoped_lock scopedLock(_funcMutex);

	CloseConnect();
}

ResponseCode CTransmitClient::Execute( std::string sModule, std::string sMethod, 
				       const CData& oParams,const char *cCmd, std::size_t bytes, 
                                       //CData& oResult,unsigned long lTimeout )
                                       CData& oResult,char *cResult, unsigned long lTimeout )
{
	p_code=eResponseCodeErrorTransmit;
	//LogInfo("Execute..................");
	//boost::mutex::scoped_lock scopedLock(_funcMutex);
	//  LogInfo("Execute22222.");
	if (!_socket.is_open())
	{
		///LogInfo("Socket is disconnected, try to reconnect.");
		if (!Connect())
		{
			///LogError("Reconnect socket failed.");
			return eResponseCodeNoConnected;
		}
		///LogInfo("Reconnect socket success.");
	}

	CData oRequest = SVSMAP();
	oRequest.RefMap()[SKYVIS_TRANSMIT_MODULE] = (SVSSTRING)sModule;
	oRequest.RefMap()[SKYVIS_TRANSMIT_METHOD] = (SVSSTRING)sMethod;
	oRequest.RefMap()[SKYVIS_TRANSMIT_PARAMS] = oParams;

	ResponseCode eCode = SendXMlCmdRequest(oRequest, cCmd, bytes);
	if (eResponseCodeSuccess != eCode)
	{
        //LogError("SendXMlCmdRequest failed.");
		p_code=eCode;
		return eCode;
	}

	eCode = RecvXMLCmdResponse(oResult, cResult, lTimeout == 0 ? _limitTime : lTimeout);
	//eCode = RecvXMLCmdResponse(oResult, lTimeout == 0 ? _limitTime : lTimeout);

	p_code=eCode;

	return eCode;
}

//////////////////////////////////////////////////////////////////////////

bool CTransmitClient::Connect()
{
	///LogDebug("In Connect().");
	try
	{
		//LogInfo("Be ready to call CloseConnect().");
		CloseConnect();
		//LogInfo("Finish to call CloseConnect().");

		_executed = false;
		_done = false;
		_ioService.reset();



		if (conntype==1)
		{
			_socket.async_connect(
				tcp::endpoint(boost::asio::ip::address_v4::from_string(_ip), _port),
				boost::bind(&CTransmitClient::OnConnect, this,
				boost::asio::placeholders::error));
		} 
		else
		{
			_socket.async_connect(
				tcp::endpoint(boost::asio::ip::address_v6::from_string(_ip), _port),
				boost::bind(&CTransmitClient::OnConnect, this,
				boost::asio::placeholders::error));
		}
		
		_timer.expires_from_now(boost::posix_time::millisec(_limitTime));
		_timer.async_wait(
			boost::bind(&CTransmitClient::OnExecuteTimeout, this));
		_ioService.run();

		if (_executed)
		{
			//LogDebug("Be ready to set_option.");
#if 1
			boost::asio::ip::tcp::socket::reuse_address ra(true);
			_socket.set_option(ra);

			boost::system::error_code err_code;
			_socket.set_option(boost::asio::ip::tcp::no_delay(true), err_code);  //设置为数据立即发送，避免延迟；有副作用，导致网络传输效率降低
			_socket.set_option(boost::asio::socket_base::linger(true, 0), err_code); //设置socket关闭时，立即关闭
			_socket.set_option(boost::asio::socket_base::keep_alive(true));
			//boost::asio::socket_base::keep_alive option(true);
			//_socket.set_option(option);
#endif 
		}
	}
	catch (...)
	{
	}
    ///LogDebug("Out Connect().");

	return _executed;
}

void CTransmitClient::CloseConnect()
{
    ///LogDebug("In CloseConnect().");
	if (_socket.is_open())
	{
        ///LogDebug("Be ready to call close socket.");
		_socket.close();
        ///LogDebug("Finish to call close socket.");
	}
    ///LogDebug("Out CloseConnect().");
}

void CTransmitClient::OnConnect( const boost::system::error_code& err )
{
	///LogDebug("In OnConnect().");
	boost::mutex::scoped_lock scopedLock(_dealMutex);

	if (_done)
	{
        ///LogInfo("Out OnConnect(Has been done).");
		return;
	}

	_done = true;
	_timer.cancel();
	if (err)
	{
		_executed = false;
		///LogError("%s", err.message().c_str());
	}
	else
	{
		_executed = true;
		///LogDebug("OnConnect success.");
	}
	///LogDebug("Out OnConnect().");
}

bool CTransmitClient::Send(const char * pData, std::size_t bytes)
{
	try
	{
		_executed = false;
		boost::system::error_code ec;
		size_t sendbytes = boost::asio::write(_socket, boost::asio::buffer(pData, bytes), boost::asio::transfer_all(), ec);
		if (ec)
		{
			///LogError("Send data to server failed, %d/%d bytes(%s).", sendbytes, bytes,
			///ec.message().c_str());
		}
		else
		{
			_executed = true;
		}
	}
	catch (...)
	{
		//LogError("Send data to server throw an exception.");
	}
	return _executed;
}

bool CTransmitClient::Receive(unsigned long lTimeout)
{
	try
	{
		_executed = false;
		_done = false;
		_ioService.reset();

		_hasHeader = false;
		_socket.async_read_some(
			boost::asio::buffer(_buffer),
			boost::bind(&CTransmitClient::OnReceive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

		_timer.expires_from_now(boost::posix_time::millisec(lTimeout));
		_timer.async_wait(
			boost::bind(&CTransmitClient::OnExecuteTimeout, this));
		_ioService.run();
	}
	catch (...)
	{
        ///LogError("Receive() throw an exception.");
	}

	return _executed;
}

void CTransmitClient::OnReceive( const boost::system::error_code& err, std::size_t bytes )
{
	boost::mutex::scoped_lock scopedLock(_dealMutex);

	if (_done)
	{
		///LogInfo("Out OnReceive(Has been done).");
		return;
	}

	if (err)
	{
		///LogWarning("OnReceive failed.");
		///LogError("%s", err.message().c_str());
		_socket.close();
		_done = true;
		_executed = false;
		_timer.cancel();
		_hasHeader = false;
		return;
	}

#if 0
        int length = (_buffer[2] << 8) + _buffer[3] + 4;
        printf("bytes %d len %d\n", bytes, length);
	for (int i = 0; i < bytes; i++)
	{
	    printf("%c", _buffer[i]);
	}

	_bufferBytes = bytes;
	_done = true;
	_executed = true;
	_timer.cancel();
	_hasHeader = false;
#endif 

#if 1
	RequestParseResult eResult = Parse(_buffer.data(), bytes);

	if (eResult == eRequestParseContinue)
	{
		//LogDebug("Continue to receive data from server.");
		_socket.async_read_some(
			boost::asio::buffer(_buffer),
			boost::bind(&CTransmitClient::OnReceive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	else if (eResult == eRequestParseEnd)
	{
		//LogDebug("Receive data from server success.");
		_done = true;
		_executed = true;
		_timer.cancel();
		_hasHeader = false;
	}
	else
	{
		///LogWarning("Parse xml error!");
		_done = true;
		_executed = false;
		_timer.cancel();
		_hasHeader = false;
	}
#endif 
}

void CTransmitClient::OnExecuteTimeout()
{
	//LogWarning("In OnExecuteTimeout().");
	boost::mutex::scoped_lock scopedLock(_dealMutex);

	if (_done)
	{
		//LogWarning("Out OnExecuteTimeout(Has been done).");
		return;
	}

    ///LogWarning("---OnExecuteTimeout---");
	_done = true;
	_executed = false;
	_socket.close();
    ///LogWarning("Out OnExecuteTimeout().");
}

//////////////////////////////////////////////////////////////////////////

ResponseCode CTransmitClient::SendXMlCmdRequest( const CData& oRequest , const char *cCmd, std::size_t bytes)
{
	if (!_socket.is_open())
	{
		///LogDebug("Socket is disconnected.");
		return eResponseCodeNoConnected;
	}
#if 0
        for (int i = 0; i < bytes; i++)
		printf("%c\n", cCmd[i]);
        printf("\n"); 
#endif 
	//printf("cCmdLength[%d]\n", bytes);
	if (!Send(cCmd, bytes))
	{
		///LogError("Send.............8192");
		return eResponseCodeErrorSend;
	}

#if 0
	SVSSTRING sRequest;
	try
	{
		if (!CData::ToCustomXMLString(oRequest, sRequest))
		{
            ///LogError("Convert xml data to xml string failed.");
			return eResponseCodeErrorLocalDataConvert;
		}
	}
	catch(...)
	{
        ///LogError("ToCustomXMLString error.");
		return eResponseCodeErrorSend;
	}

	TransmitHeader header;
	ResetTransmitHeader(header);
	header.eType = eTransmitTypeXMLCommand;
	header.lHeaderSize = sRequest.size();

	SVSLONG32 lRequestSize = SKYVIS_HEADER_SIZE + header.lHeaderSize;
	SVSBINARY binRequest;
	binRequest.resize(lRequestSize);
	memcpy((char *)&binRequest[0], (char *)&header, SKYVIS_HEADER_SIZE);
	memcpy((char *)&binRequest[SKYVIS_HEADER_SIZE], sRequest.c_str(), header.lHeaderSize);

	SVSLONG32  lRequestStart=0;

	//LogInfo("lRequestSize......%d",lRequestSize);

	while (lRequestSize>SKYVIS_BUFFER_SIZE)
	{
		if (!Send((char *)&binRequest[lRequestStart], SKYVIS_BUFFER_SIZE))
		{
            ///LogError("Send.............8192");
			return eResponseCodeErrorSend;
		}
		lRequestSize=lRequestSize-SKYVIS_BUFFER_SIZE;
		lRequestStart=lRequestStart+SKYVIS_BUFFER_SIZE;
	}

	//LogInfo("lRequestSize......%d",lRequestSize);

	if (!Send((char *)&binRequest[lRequestStart], lRequestSize))
	{
        ///LogError("Send......<<<<<<<<<<<<.......8192");
		return eResponseCodeErrorSend;
	}
#endif 

	return eResponseCodeSuccess;
}

ResponseCode CTransmitClient::RecvXMLCmdResponse( CData& oResult, char *cResult, unsigned long lTimeout )
//ResponseCode CTransmitClient::RecvXMLCmdResponse( CData& oResult, unsigned long lTimeout )
{
	try
	{
		if (!Receive(lTimeout))
		{
			//printf("Receive failed.\n");
			//LogError("RecvXMLCmdResponse Receive failed.");
			return eResponseCodeErrorRecv;
		}

#if 0
		printf("\nRecvXMLCmdResponse length %d\n", length);
		for (int i = 0; i < length; i++)
		{
			if (i < 4)
			{
				printf("%d", _buffer[i]);
			}
			else 
			{
				printf("%d", _buffer[i]);
			}
		}
#endif 
		int length;
		int CmdNum = _recvBuffer[1] & 0x0f;
		//printf("RecvXMLCmdResponse CmdNum %d\n", CmdNum);
		if(CmdNum == 7)
		{
			length = (_recvBuffer[2] << 16) + (_recvBuffer[3] << 8) + _recvBuffer[4] + 5;
			 //printf("RecvXMLCmdResponse length1 %d\n", length);
		}
		else
		{
			length = (_recvBuffer[2] << 8) + _recvBuffer[3] + 4;
			//printf("RecvXMLCmdResponse length3 %d\n", length);
		}
		//int length = (_recvBuffer[2] << 8) + _recvBuffer[3] + 4;
        
        memcpy(cResult, _recvBuffer.data(), length);
        if (cResult == NULL)
		{
			return eResponseCodeErrorRecv;
		}
	}
	catch(...)
	{
		//LogWarning("RecvXMLCmdResponse throw an exception.");
		return eResponseCodeErrorRecv;
	}

	return eResponseCodeSuccess;
}

SVS::RequestParseResult CTransmitClient::Parse( const char * pBuffer, std::size_t bytes )
{
	try
	{
		if (!_hasHeader)
		{
			_hasHeader = true;
			_recved = 0;
			int CmdNum = pBuffer[1] & 0x0f;
			//printf(" ##Parse CmdNum %d\n", CmdNum);
			if(CmdNum == 7)
			{
				_recvedLen = (pBuffer[2] << 16) + (pBuffer[3] << 8) + pBuffer[4] + 5;
				//printf("_recvedLen1 %d  bytes %d\n", _recvedLen, bytes);
			}else{
				_recvedLen = (pBuffer[2] << 8) + pBuffer[3] + 4;
                //printf("_recvedLen3 %d  bytes %d\n", _recvedLen, bytes);
			}
			
			//memcpy(_recvBuffer.data(), pBuffer, bytes);
			//_recved = bytes;
			//return eRequestParseEnd;
			//濡傛灉鎺ユ敹鐨勬暟鎹暱搴﹁秴杩囨帴鏀剁紦鍐插尯鐨勬渶澶ч暱搴﹀垯涓婃姤鏁呴殰
			if(_recvedLen > SKYVIS_BUFFER_SIZE)
			{
				printf("fuction: CTransmitClient::Parse()  _recvedLen3 %d  bytes %d\n", _recvedLen, bytes);
				return eRequestParseError;
			}
			
			if (bytes < _recvedLen)
			{
				memcpy(_recvBuffer.data(), pBuffer, bytes);
				_recved = bytes;
			}
			else if (_recvedLen == bytes)
			{
				memcpy(_recvBuffer.data(), pBuffer, _recvedLen);
				return eRequestParseEnd;
			}
		}
		else
		{
                        int readLen = _recved + bytes;
                        //printf("1-- rLen %d bytes %d clen %d\n", _recvedLen, bytes,readLen);
			if (readLen <_recvedLen)
			{
				memcpy(_recvBuffer.data() + _recved, pBuffer, bytes);
				_recved += bytes;
			}
			else if(readLen == _recvedLen)
			{
				memcpy(_recvBuffer.data() + _recved, pBuffer, _recvedLen - _recved);
				return eRequestParseEnd;
			}
		}

	}
	catch (...)
	{
		///LogWarning("Parse data error.");
		return eRequestParseError;
	}

	//LogInfo("_recved......%d",_recved);
	return eRequestParseContinue;
}
} // namespace SVS
