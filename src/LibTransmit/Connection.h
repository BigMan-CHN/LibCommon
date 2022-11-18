/**
* Connection.h	-	����һ���������ͷ�ļ���
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
* ���տͻ��˷��͵����ݵĻ���Ĵ�С
*/
#define SKYVIS_BUFFER_SIZE		1524*1024	

//////////////////////////////////////////////////////////////////////////

class CConnectionManager;

/**
* �����࣬��Ӧһ���ͻ�������
*/
class CConnection
	: private boost::noncopyable
{
public:
	/// Construct a connection with the given io_service.
	explicit CConnection(boost::asio::io_service& io_service,
		CConnectionManager& oManager, ICommandHandler * pCmdHandler);

	/**
	* ���������ӹ�����socket
	* @result ������socket
	*/
	boost::asio::ip::tcp::socket& Socket();

	/**
	* ���������ӵĵ�һ���첽����
	*/
	void Start();

	/**
	* ֹͣ���������е��첽����
	*/
	void Stop();

	/**
	* �����ر�
	*/
	void Shutdown();

private:
	/**
	* ����������ĺ���
	* @param e �첽�������Ĵ������
	* @param lSizeTransferred  �첽���������յ��ֽ���
	*/
	void OnRead(const boost::system::error_code& e,
		std::size_t lSizeTransferred);

	/**
	* ����д�����ĺ���
	* @param binResponse ���ظ��ͻ��˵���Ӧ
	*/
	bool Write(SVSBINARY& binResponse);

private:
	/**
	* �����ӹ�����socket
	*/
	boost::asio::ip::tcp::socket m_oSocket;

	/**
	* ��������ӵ�Manager������
	*/
	CConnectionManager& m_oManager;

	/**
	* ��������ӽ������ݵĴ������
	*/
	CRequestHandler m_oRequestHandler;

	/**
	* ��Ž��յĶ��������ݵĻ���
	*/
	boost::array<char, SKYVIS_BUFFER_SIZE> m_oBuffer;
};

typedef CConnection * CConnectionPtr;

} // namespace SVS

#endif // __INCLUDE_SKYVIS_CONNECTION_H__
