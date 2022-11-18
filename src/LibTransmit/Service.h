/**
* Service.h	-	这是一个服务类的头文件。
*/
#ifndef __INCLUDE_SKYVIS_SERVICE_H__
#define __INCLUDE_SKYVIS_SERVICE_H__

#pragma once

#include "TransmitDefines.h"
#include <boost/asio.hpp>
#include "LibXML.h"
#include "Connection.h"
#include "ConnectionManager.h"
#include "RequestHandler.h"

namespace SVS
{

/**
* XML函数调度接口
*/
class ICommandHandler
{
public:
	/**
	* 服务会调用此方法将函数调用的所有参数传递给处理类对象
	* @param sModule XML函数调度的模块名称
	* @param sMethod XML函数调度的函数名称
	* @param oParams 包含函数参数的类型为MAP的自定义对象
	* @param oResult 包含函数调用结果的类型为MAP的自定义对象
	* @result 返回的调用结果，成功返回eResponseCodeSuccess，否则返回错误代码
	*/
	virtual ResponseCode Call(const SVSSTRING& sModule, const SVSSTRING& sMethod,
		SVS::CData& oParams, SVS::CData& oResult) = 0;

	ResponseCode p_code;
};

/**
* 服务类，调度所有客户端连接的服务
*/
class CService
{
public:
	/**
	* 默认构造函数
	* @param pCmdHandler XML函数调度接口指针
	* @param threadPoolSize 运行ioservice的线程池中线程的数目
	* @param sFileDir 上传的文件临时存放的根路径
	*/
	CService(ICommandHandler * pCmdHandler, std::size_t threadPoolSize, 
		const std::string sFileDir = "");
	~CService(void);

	/**
	* 启动服务
	* @param nPort 监听端口
	* @param sAddress 监听地址，默认本机所有地址
	* @result 成功返回true，否则返回false
	*/
	bool Start(const int& nPort, const std::string& sAddress = "");

	/**
	* 停止服务
	*/
	void Stop();

	/**
	* 运行服务，Start成功后需要运行此函数执行，然后服务进入循环等待状态
	*/
	void Run();

protected:
	/**
	* 处理Accept响应的函数
	* @param e 错误变量
	*/
	void OnAccept(const boost::system::error_code& e);

	/**
	* 处理停止Accept的函数
	*/
	void OnStop();

private:
	boost::asio::io_service m_ioService;		// boost的asio服务对象
	boost::asio::ip::tcp::acceptor m_oAcceptor;	// boost的服务监听Acceptor对象
	CConnectionManager m_oManager;				// 连接管理类对象
	CConnectionPtr m_pConn;						// 新连接对象
	ICommandHandler * m_pCmdHandler;			// XML函数调用对象指针
	std::size_t m_uPoolSize;					// 运行ioservice的线程池中线程的数目
};

} // namespace SVS

#endif // __INCLUDE_SKYVIS_SERVICE_H__
