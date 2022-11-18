/**
* Connection.h	-	这是一个连接类的头文件。
*/
#ifndef __INCLUDE_SKYVIS_CONNECTION_H__
#define __INCLUDE_SKYVIS_CONNECTION_H__

#include "TransmitDefines.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "RequestHandler.h"

namespace SVS
{

//////////////////////////////////////////////////////////////////////////
/**
* 接收客户端发送的数据的缓冲的大小
*/
#define SKYVIS_BUFFER_SIZE		1524*1024	

//////////////////////////////////////////////////////////////////////////

class CConnectionManager;

/**
* 连接类，对应一个客户端连接
*/
class CConnection
	: private boost::noncopyable
{
public:
	/// Construct a connection with the given io_service.
	explicit CConnection(boost::asio::io_service& io_service,
		CConnectionManager& oManager, ICommandHandler * pCmdHandler);

	/**
	* 获得与该连接关联的socket
	* @result 关联的socket
	*/
	boost::asio::ip::tcp::socket& Socket();

	/**
	* 启动该连接的第一次异步操作
	*/
	void Start();

	/**
	* 停止该连接所有的异步操作
	*/
	void Stop();

	/**
	* 主动关闭
	*/
	void Shutdown();

private:
	/**
	* 处理读操作的函数
	* @param e 异步读操作的错误变量
	* @param lSizeTransferred  异步读操作接收的字节数
	*/
	void OnRead(const boost::system::error_code& e,
		std::size_t lSizeTransferred);

	/**
	* 处理写操作的函数
	* @param binResponse 返回给客户端的响应
	*/
	bool Write(SVSBINARY& binResponse);

private:
	/**
	* 该连接关联的socket
	*/
	boost::asio::ip::tcp::socket m_oSocket;

	/**
	* 管理该连接的Manager的引用
	*/
	CConnectionManager& m_oManager;

	/**
	* 处理该连接接收数据的处理对象
	*/
	CRequestHandler m_oRequestHandler;

	/**
	* 存放接收的二进制数据的缓冲
	*/
	boost::array<char, SKYVIS_BUFFER_SIZE> m_oBuffer;
};

typedef CConnection * CConnectionPtr;

} // namespace SVS

#endif // __INCLUDE_SKYVIS_CONNECTION_H__
