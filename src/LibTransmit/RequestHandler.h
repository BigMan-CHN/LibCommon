/**
* RequestHandler.h	-	这是一个解析传输数据并处理的类的头文件。
*/

#ifndef __INCLUDE_SKYVIS_REQUESTHANDLER_H__
#define __INCLUDE_SKYVIS_REQUESTHANDLER_H__

#pragma once

#include "TransmitDefines.h"
#include "Defines.h"
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

namespace SVS
{

class ICommandHandler;

/**
*解析传输数据并处理的类
*/
class CRequestHandler
{
public:
	CRequestHandler(ICommandHandler * pCmdHandler, const std::string sFileDir = "");
	~CRequestHandler(void);

	RequestParseResult Parse(const char * pBuffer, std::size_t lSize);
	void HandleRequest(SVSBINARY& binResponse, SVSSTRING sClientIP, SVSLONG16 lClientPort);

protected:
	/**
	* 生成XML命令的响应
	* @param eType 传输的数据类型
	* @param oResult 包含返回结果的类型为MAP的自定义对象
	* @param binResponse 包含生成的二进制数据的存储对象
	* @result 
	*/
	void FormResponse(const TransmitType eType, const SVS::CData& oResult,
		SVSBINARY& binResponse);

	/**
	* 生成各种状态的响应
	* @param eType 传输的数据类型
	* @param eCode 反馈给客户端的消息code
	* @param binResponse 包含生成的二进制数据的存储对象
	* @result 
	*/
	void FormResponse(const TransmitType eType, const ResponseCode eCode,
		SVSBINARY& binResponse);

	/**
	* 处理控制命令调用
	* @param binResponse 处理客户端传入的接收的XMl调用二进制数据
	* @param sClientIP 客户端的网络地址
	* @param lClientPort 客户端的端口
	* @result 
	*/
	void HandleXMLCmdRequest(SVSBINARY& binResponse, SVSSTRING sClientIP, SVSLONG16 lClientPort);

	/**
	* 处理文件上传调用
	* @param binResponse 处理客户端传入的接收的文件的二进制数据
	* @result 
	*/
	void HandleFileRequest(SVSBINARY& binResponse);


private:
	ICommandHandler * m_pCmdHandler;		// XML命令调用处理对象的指针
	std::string		m_sFileDir;				// 客户端上传文件的存放根路径

	bool			m_bHeader;				// 是否是第一次接收，接收的包头
	TransmitHeader	m_oRequestHeader;	// 接收的请求的包头
	SVSLONG32	m_lRecved;				// 已接收的数据的大小
	SVSBINARY	m_binRequest;			// 接收的数据的存放变量
};

} // namespace SVS

#endif // __INCLUDE_SKYVIS_REQUESTHANDLER_H__
