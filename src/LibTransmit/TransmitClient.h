/**
* TransmitClient.h	-	这是一个执行服务器函数调用的客户端类的头文件。
*/

#ifndef __INCLUDE_SKYVIS_TRANSMITCLIENT_H__
#define __INCLUDE_SKYVIS_TRANSMITCLIENT_H__

#pragma once

#include "TransmitDefines.h"
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread/condition.hpp>
#include "LibXML/LibXML.h"

namespace SVS
{

//////////////////////////////////////////////////////////////////////////
/**
* 接收数据的缓冲大小
*/
#define SKYVIS_BUFFER_SIZE			(1524 * 1024)

//////////////////////////////////////////////////////////////////////////

/**
* 执行服务器函数调用的客户端类。
*/
class CTransmitClient
{
public:
	/**
	* @param lTimeout 连接服务器超时限制，单位毫秒
	*/
	CTransmitClient(unsigned long lTimeout = 3000);
	~CTransmitClient(void);

	/**
	* 设置超时长短
	* @param lTimeout 连接服务器超时限制，单位毫秒
	*/
	void SetTimeout(unsigned long lTimeout);

	/**
	* 连接到指定的服务器
	* @param sIP 服务器地址
	* @param nPort 服务器端口
	* @result 连接成功返回true，失败返回false
	*/
	bool Connect(std::string sIP, unsigned int nPort,unsigned int proto = 1);

	/**
	* 在服务器上调用指定模块的指定方法的函数
	* @param sModule 调用的函数所在模块的名称
	* @param sMethod 调用的函数名称
	* @param oParams 包含调用的函数的参数的Map对象
	* @param oResult 包含调用返回结果的Map对象
	* @param lTimeout 执行超时时长，单位毫秒，如果为0，则使用全局默认超时时长
	* @result 调用成功返回eResponseCodeSuccess，否则返回错误代码
	*/
#if 0
	ResponseCode Execute(std::string sModule, std::string sMethod,
		const CData& oParams, CData& oResult, unsigned long lTimeout = 0);
#endif 

	//add fwb 20201027
	ResponseCode Execute(std::string sModule, std::string sMethod,
		const CData& oParams, const char *cCmd, std::size_t bytes, CData& oResult, char *cResult, unsigned long lTimeout = 0);

	/**
	* 是否已连接服务器
	* @result 已连接返回true，否则返回false
	*/
	bool IsConnect();

	/**
	* 关闭连接
	*/
    void Close();

	ResponseCode p_code;

private:
	/**
	* 执行实际连接的内部函数
	*/
	bool Connect();
	/**
	* 执行实际关闭连接的内部函数
	*/
	void CloseConnect();
	/**
	* 处理异步连接的方法
	*/
	void OnConnect(const boost::system::error_code& err);
	/**
	* 执行实际发送的内部函数
	* @param pData 指向要发送的数据的指针
	* @param len 要发送的数据的长度
	*/
	bool Send(const char * pData, std::size_t bytes);
	/**
	* 执行实际接收的内部函数
	* @param lTimeout 等待接收超时时长，单位毫秒
	*/
	bool Receive(unsigned long lTimeout);
	/**
	* 处理异步接收的方法
	*/
	void OnReceive(const boost::system::error_code& err, std::size_t bytes);
	/**
	* 连接超时处理的方法
	*/
	void OnExecuteTimeout();

private:
	/**
	* 发送XML格式命令给服务器执行并接受执行结果
	* @param oRequest 包含调用模块函数参数的MAP对象
	* @result 执行成功返回返回eResponseCodeSuccess，否则返回错误代码
	*/
	ResponseCode SendXMlCmdRequest(const CData& oRequest, const char *cCmd, std::size_t bytes);

	/**
	* 接收服务器返回的执行结果
	* @param oResult 包含执行结果的MAP对象
	* @param lTimeout 等待接收超时时长，单位毫秒
	* @result 执行成功返回返回eResponseCodeSuccess，否则返回错误代码
	*/
	ResponseCode RecvXMLCmdResponse(CData& oResult,char *cResult,unsigned long lTimeout);
	/**
	* 解析服务器返回的数据
	*/
	RequestParseResult Parse(const char * pBuffer, std::size_t bytes);

private:
	// 函数执行锁
	boost::mutex					_funcMutex;
	// boost库的asio服务对象
	boost::asio::io_service			_ioService;
	// boost库的socket对象
	boost::asio::ip::tcp::socket	_socket;

	// 服务器地址
	SVSSTRING						_ip;
	// 服务器端口
	unsigned int					_port;

	// 异步执行锁
	boost::mutex					_dealMutex;
	// 超时时长
	unsigned long					_limitTime;
	// 超时定时器
	boost::asio::deadline_timer		_timer;
	// 是否执行成功
	bool							_executed;
	// 是否已经处理了异步或超时
	bool							_done;

	// 存放接收的二进制数据的缓冲
	unsigned int                    _bufferBytes;
	boost::array<char, SKYVIS_BUFFER_SIZE> _buffer;
	boost::array<char, SKYVIS_BUFFER_SIZE> _recvBuffer;
	// 是否已接收数据头
	bool							_hasHeader;
	// 已接收的数据的大小
	std::size_t						_recved;
	std::size_t						_recvedLen;
	// 接收的请求的包头
	TransmitHeader					_responseHeader;
	// 接收的数据的存放变量
	SVSBINARY						_response;
	unsigned int                    conntype; //1:ipv4 2:ipv6
};

} // namespace SVS

#endif // __INCLUDE_SKYVIS_TRANSMITCLIENT_H__
