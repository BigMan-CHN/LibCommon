/**
* TransmitClient.h	-	����һ��ִ�з������������õĿͻ������ͷ�ļ���
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
* �������ݵĻ����С
*/
#define SKYVIS_BUFFER_SIZE			(1524 * 1024)

//////////////////////////////////////////////////////////////////////////

/**
* ִ�з������������õĿͻ����ࡣ
*/
class CTransmitClient
{
public:
	/**
	* @param lTimeout ���ӷ�������ʱ���ƣ���λ����
	*/
	CTransmitClient(unsigned long lTimeout = 3000);
	~CTransmitClient(void);

	/**
	* ���ó�ʱ����
	* @param lTimeout ���ӷ�������ʱ���ƣ���λ����
	*/
	void SetTimeout(unsigned long lTimeout);

	/**
	* ���ӵ�ָ���ķ�����
	* @param sIP ��������ַ
	* @param nPort �������˿�
	* @result ���ӳɹ�����true��ʧ�ܷ���false
	*/
	bool Connect(std::string sIP, unsigned int nPort,unsigned int proto = 1);

	/**
	* �ڷ������ϵ���ָ��ģ���ָ�������ĺ���
	* @param sModule ���õĺ�������ģ�������
	* @param sMethod ���õĺ�������
	* @param oParams �������õĺ����Ĳ�����Map����
	* @param oResult �������÷��ؽ����Map����
	* @param lTimeout ִ�г�ʱʱ������λ���룬���Ϊ0����ʹ��ȫ��Ĭ�ϳ�ʱʱ��
	* @result ���óɹ�����eResponseCodeSuccess�����򷵻ش������
	*/
#if 0
	ResponseCode Execute(std::string sModule, std::string sMethod,
		const CData& oParams, CData& oResult, unsigned long lTimeout = 0);
#endif 

	//add fwb 20201027
	ResponseCode Execute(std::string sModule, std::string sMethod,
		const CData& oParams, const char *cCmd, std::size_t bytes, CData& oResult, char *cResult, unsigned long lTimeout = 0);

	/**
	* �Ƿ������ӷ�����
	* @result �����ӷ���true�����򷵻�false
	*/
	bool IsConnect();

	/**
	* �ر�����
	*/
    void Close();

	ResponseCode p_code;

private:
	/**
	* ִ��ʵ�����ӵ��ڲ�����
	*/
	bool Connect();
	/**
	* ִ��ʵ�ʹر����ӵ��ڲ�����
	*/
	void CloseConnect();
	/**
	* �����첽���ӵķ���
	*/
	void OnConnect(const boost::system::error_code& err);
	/**
	* ִ��ʵ�ʷ��͵��ڲ�����
	* @param pData ָ��Ҫ���͵����ݵ�ָ��
	* @param len Ҫ���͵����ݵĳ���
	*/
	bool Send(const char * pData, std::size_t bytes);
	/**
	* ִ��ʵ�ʽ��յ��ڲ�����
	* @param lTimeout �ȴ����ճ�ʱʱ������λ����
	*/
	bool Receive(unsigned long lTimeout);
	/**
	* �����첽���յķ���
	*/
	void OnReceive(const boost::system::error_code& err, std::size_t bytes);
	/**
	* ���ӳ�ʱ����ķ���
	*/
	void OnExecuteTimeout();

private:
	/**
	* ����XML��ʽ�����������ִ�в�����ִ�н��
	* @param oRequest ��������ģ�麯��������MAP����
	* @result ִ�гɹ����ط���eResponseCodeSuccess�����򷵻ش������
	*/
	ResponseCode SendXMlCmdRequest(const CData& oRequest, const char *cCmd, std::size_t bytes);

	/**
	* ���շ��������ص�ִ�н��
	* @param oResult ����ִ�н����MAP����
	* @param lTimeout �ȴ����ճ�ʱʱ������λ����
	* @result ִ�гɹ����ط���eResponseCodeSuccess�����򷵻ش������
	*/
	ResponseCode RecvXMLCmdResponse(CData& oResult,char *cResult,unsigned long lTimeout);
	/**
	* �������������ص�����
	*/
	RequestParseResult Parse(const char * pBuffer, std::size_t bytes);

private:
	// ����ִ����
	boost::mutex					_funcMutex;
	// boost���asio�������
	boost::asio::io_service			_ioService;
	// boost���socket����
	boost::asio::ip::tcp::socket	_socket;

	// ��������ַ
	SVSSTRING						_ip;
	// �������˿�
	unsigned int					_port;

	// �첽ִ����
	boost::mutex					_dealMutex;
	// ��ʱʱ��
	unsigned long					_limitTime;
	// ��ʱ��ʱ��
	boost::asio::deadline_timer		_timer;
	// �Ƿ�ִ�гɹ�
	bool							_executed;
	// �Ƿ��Ѿ��������첽��ʱ
	bool							_done;

	// ��Ž��յĶ��������ݵĻ���
	unsigned int                    _bufferBytes;
	boost::array<char, SKYVIS_BUFFER_SIZE> _buffer;
	boost::array<char, SKYVIS_BUFFER_SIZE> _recvBuffer;
	// �Ƿ��ѽ�������ͷ
	bool							_hasHeader;
	// �ѽ��յ����ݵĴ�С
	std::size_t						_recved;
	std::size_t						_recvedLen;
	// ���յ�����İ�ͷ
	TransmitHeader					_responseHeader;
	// ���յ����ݵĴ�ű���
	SVSBINARY						_response;
	unsigned int                    conntype; //1:ipv4 2:ipv6
};

} // namespace SVS

#endif // __INCLUDE_SKYVIS_TRANSMITCLIENT_H__
