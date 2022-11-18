/**
* RequestHandler.h	-	����һ�������������ݲ���������ͷ�ļ���
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
*�����������ݲ��������
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
	* ����XML�������Ӧ
	* @param eType �������������
	* @param oResult �������ؽ��������ΪMAP���Զ������
	* @param binResponse �������ɵĶ��������ݵĴ洢����
	* @result 
	*/
	void FormResponse(const TransmitType eType, const SVS::CData& oResult,
		SVSBINARY& binResponse);

	/**
	* ���ɸ���״̬����Ӧ
	* @param eType �������������
	* @param eCode �������ͻ��˵���Ϣcode
	* @param binResponse �������ɵĶ��������ݵĴ洢����
	* @result 
	*/
	void FormResponse(const TransmitType eType, const ResponseCode eCode,
		SVSBINARY& binResponse);

	/**
	* ��������������
	* @param binResponse ����ͻ��˴���Ľ��յ�XMl���ö���������
	* @param sClientIP �ͻ��˵������ַ
	* @param lClientPort �ͻ��˵Ķ˿�
	* @result 
	*/
	void HandleXMLCmdRequest(SVSBINARY& binResponse, SVSSTRING sClientIP, SVSLONG16 lClientPort);

	/**
	* �����ļ��ϴ�����
	* @param binResponse ����ͻ��˴���Ľ��յ��ļ��Ķ���������
	* @result 
	*/
	void HandleFileRequest(SVSBINARY& binResponse);


private:
	ICommandHandler * m_pCmdHandler;		// XML������ô�������ָ��
	std::string		m_sFileDir;				// �ͻ����ϴ��ļ��Ĵ�Ÿ�·��

	bool			m_bHeader;				// �Ƿ��ǵ�һ�ν��գ����յİ�ͷ
	TransmitHeader	m_oRequestHeader;	// ���յ�����İ�ͷ
	SVSLONG32	m_lRecved;				// �ѽ��յ����ݵĴ�С
	SVSBINARY	m_binRequest;			// ���յ����ݵĴ�ű���
};

} // namespace SVS

#endif // __INCLUDE_SKYVIS_REQUESTHANDLER_H__
