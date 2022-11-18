/**
* Service.h	-	����һ���������ͷ�ļ���
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
* XML�������Ƚӿ�
*/
class ICommandHandler
{
public:
	/**
	* �������ô˷������������õ����в������ݸ����������
	* @param sModule XML�������ȵ�ģ������
	* @param sMethod XML�������ȵĺ�������
	* @param oParams ������������������ΪMAP���Զ������
	* @param oResult �����������ý��������ΪMAP���Զ������
	* @result ���صĵ��ý�����ɹ�����eResponseCodeSuccess�����򷵻ش������
	*/
	virtual ResponseCode Call(const SVSSTRING& sModule, const SVSSTRING& sMethod,
		SVS::CData& oParams, SVS::CData& oResult) = 0;

	ResponseCode p_code;
};

/**
* �����࣬�������пͻ������ӵķ���
*/
class CService
{
public:
	/**
	* Ĭ�Ϲ��캯��
	* @param pCmdHandler XML�������Ƚӿ�ָ��
	* @param threadPoolSize ����ioservice���̳߳����̵߳���Ŀ
	* @param sFileDir �ϴ����ļ���ʱ��ŵĸ�·��
	*/
	CService(ICommandHandler * pCmdHandler, std::size_t threadPoolSize, 
		const std::string sFileDir = "");
	~CService(void);

	/**
	* ��������
	* @param nPort �����˿�
	* @param sAddress ������ַ��Ĭ�ϱ������е�ַ
	* @result �ɹ�����true�����򷵻�false
	*/
	bool Start(const int& nPort, const std::string& sAddress = "");

	/**
	* ֹͣ����
	*/
	void Stop();

	/**
	* ���з���Start�ɹ�����Ҫ���д˺���ִ�У�Ȼ��������ѭ���ȴ�״̬
	*/
	void Run();

protected:
	/**
	* ����Accept��Ӧ�ĺ���
	* @param e �������
	*/
	void OnAccept(const boost::system::error_code& e);

	/**
	* ����ֹͣAccept�ĺ���
	*/
	void OnStop();

private:
	boost::asio::io_service m_ioService;		// boost��asio�������
	boost::asio::ip::tcp::acceptor m_oAcceptor;	// boost�ķ������Acceptor����
	CConnectionManager m_oManager;				// ���ӹ��������
	CConnectionPtr m_pConn;						// �����Ӷ���
	ICommandHandler * m_pCmdHandler;			// XML�������ö���ָ��
	std::size_t m_uPoolSize;					// ����ioservice���̳߳����̵߳���Ŀ
};

} // namespace SVS

#endif // __INCLUDE_SKYVIS_SERVICE_H__
