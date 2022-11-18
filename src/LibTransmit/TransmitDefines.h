/**
* TransmitDefines.h	-	���Ǵ�����Զ����ͷ�ļ���
*/

#ifndef __INCLUDE_SKYVIS_TRANSMITDEFINES_H__
#define __INCLUDE_SKYVIS_TRANSMITDEFINES_H__

#include "LibXML/Defines.h"
#include <cstring>

// �Զ��������ռ䣬skyvis��д
namespace SVS {

/**
* ����Ĭ�϶˿�
*/
//const unsigned int SERVICE_DEFAULT_PORT = 5231;
const unsigned int SERVICE_DEFAULT_PORT = 6180;

/**
* ��������
*/
typedef enum TransmitType {
	eTransmitTypeConnect = 0x0001,	// ���Ӳ���
	eTransmitTypeXMLCommand,		// XML��������
	eTransmitTypeFile,				// �ļ��ϴ�
} TransmitType;

/**
* �����Ԥ�����ֶ�����
*/
#define SKYVIS_TRANSMIT_MODULE			"Module"
#define SKYVIS_TRANSMIT_METHOD			"Method"
#define SKYVIS_TRANSMIT_PARAMS			"Params"
#define SKYVIS_TRANSMIT_TRANSMITCODE	"ResponseCode"
#define SKYVIS_TRANSMIT_RESPONSE		"Response"
#define SKYVIS_TRANSMIT_RESULT			"Result"
#define SKYVIS_TRANSMIT_CLIENTADDR		"ClientAddr"
#define SKYVIS_TRANSMIT_CLIENTPORT		"ClientPort"

/**
* �����ͷ��ʶ
*/
#define SKYVIS_HEADER_FLAG			"SKYVIS"

/**
* �����ͷ��ʶ�ĳ���
*/
const SVSLONG16 SKYVIS_FLAG_SIZE = 6;

/**
* �����ͷ
*/
typedef struct TransmitHeader
{
	char			szMark[SKYVIS_FLAG_SIZE];	// �����ͷ��ʶ����SKYVIS��
	SVS::SVSLONG16	eType;						// 2�ֽڴ�������
	SVS::SVSLONG32	lHeaderSize;				// 4�ֽ����ݳ���
} TransmitHeader;

/**
* �����ͷ�ĳ���
*/
const SVSLONG16 SKYVIS_HEADER_SIZE = sizeof(TransmitHeader);

/**
* ��ʼ����ͷ�ķ���
*/
static void ResetTransmitHeader(TransmitHeader& h)
{ 
	memcpy(&h, SKYVIS_HEADER_FLAG, SKYVIS_FLAG_SIZE);
	h.eType = eTransmitTypeXMLCommand;
	h.lHeaderSize = 0;
};

/**
* �Զ�����������ͻ��˵���Ϣcode
*/
typedef enum ResponseCode
{
	eResponseCodeSuccess = 0,				// �ɹ�
	eResponseCodeNoModule,					// �����ڴ�ģ��
	eResponseCodeNoMethod,					// �����ڴ˷���
	eResponseCodeNoConnected,				// ��û�����ӵ�������
	eResponseCodeErrorParam,				// ȱ�ٲ�����������Ͳ���ȷ
	eResponseCodeErrorParamType,			// �������Ͳ���ȷ
	eResponseCodeErrorServiceDataConvert,	// ���������ת������
	eResponseCodeErrorLocalDataConvert,		// ��������ת������
	eResponseCodeErrorSend,					// �������ݵ�����������
	eResponseCodeErrorRecv,					// ���շ��������ݳ���
	eResponseCodeErrorTransmit,				// �������
} ResponseCode;

static SVSSTRING GetResponseError(ResponseCode& e)
{
	switch (e)
	{
	case eResponseCodeSuccess:
		return "";
	case eResponseCodeNoModule:
		return "�����ڴ�ģ��";
	case eResponseCodeNoMethod:
		return "�����ڴ˷���";
	case eResponseCodeNoConnected:
		return "��û�����ӵ�������";
	case eResponseCodeErrorParamType:
		return "�������Ͳ���ȷ";
	case eResponseCodeErrorParam:
		return "ȱ�ٲ�����������Ͳ���ȷ";
	case eResponseCodeErrorServiceDataConvert:
		return "���������ת������";
	case eResponseCodeErrorLocalDataConvert:
		return "��������ת������";
	case eResponseCodeErrorSend:
		return "�������ݵ�����������";
	case eResponseCodeErrorRecv:
		return "���շ��������ݳ���";
	case eResponseCodeErrorTransmit:
		return "�������";
	default:
		return "No such error.";
	}	
}

/**
* �����ͻ�������������ݵķ�������
*/
typedef enum RequestParseResult {
	eRequestParseOK = 0,			// �����޴���
	eRequestParseEnd,				// ���ݽ������
	eRequestParseContinue,			// ������������
	eRequestParseError,				// ���յĶ��������ݲ���ȷ����ͷ����ȷ�������ݳ��Ȳ���ȷ
} RequestParseResult;

} // namespace SVS


#endif // __INCLUDE_SKYVIS_TRANSMITDEFINES_H__
