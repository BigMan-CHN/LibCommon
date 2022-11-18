/**
* TransmitDefines.h	-	这是传输库自定义的头文件。
*/

#ifndef __INCLUDE_SKYVIS_TRANSMITDEFINES_H__
#define __INCLUDE_SKYVIS_TRANSMITDEFINES_H__

#include "LibXML/Defines.h"
#include <cstring>

// 自定义命名空间，skyvis缩写
namespace SVS {

/**
* 服务默认端口
*/
//const unsigned int SERVICE_DEFAULT_PORT = 5231;
const unsigned int SERVICE_DEFAULT_PORT = 6180;

/**
* 传输类型
*/
typedef enum TransmitType {
	eTransmitTypeConnect = 0x0001,	// 连接测试
	eTransmitTypeXMLCommand,		// XML函数调用
	eTransmitTypeFile,				// 文件上传
} TransmitType;

/**
* 传输的预定义字段名称
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
* 传输包头标识
*/
#define SKYVIS_HEADER_FLAG			"SKYVIS"

/**
* 传输包头标识的长度
*/
const SVSLONG16 SKYVIS_FLAG_SIZE = 6;

/**
* 传输包头
*/
typedef struct TransmitHeader
{
	char			szMark[SKYVIS_FLAG_SIZE];	// 传输包头标识，“SKYVIS”
	SVS::SVSLONG16	eType;						// 2字节传输类型
	SVS::SVSLONG32	lHeaderSize;				// 4字节数据长度
} TransmitHeader;

/**
* 传输包头的长度
*/
const SVSLONG16 SKYVIS_HEADER_SIZE = sizeof(TransmitHeader);

/**
* 初始化包头的方法
*/
static void ResetTransmitHeader(TransmitHeader& h)
{ 
	memcpy(&h, SKYVIS_HEADER_FLAG, SKYVIS_FLAG_SIZE);
	h.eType = eTransmitTypeXMLCommand;
	h.lHeaderSize = 0;
};

/**
* 自定义服务反馈给客户端的消息code
*/
typedef enum ResponseCode
{
	eResponseCodeSuccess = 0,				// 成功
	eResponseCodeNoModule,					// 不存在此模块
	eResponseCodeNoMethod,					// 不存在此方法
	eResponseCodeNoConnected,				// 尚没有连接到服务器
	eResponseCodeErrorParam,				// 缺少参数或参数类型不正确
	eResponseCodeErrorParamType,			// 参数类型不正确
	eResponseCodeErrorServiceDataConvert,	// 服务端数据转换出错
	eResponseCodeErrorLocalDataConvert,		// 本地数据转换出错
	eResponseCodeErrorSend,					// 发送数据到服务器出错
	eResponseCodeErrorRecv,					// 接收服务器数据出错
	eResponseCodeErrorTransmit,				// 传输出错
} ResponseCode;

static SVSSTRING GetResponseError(ResponseCode& e)
{
	switch (e)
	{
	case eResponseCodeSuccess:
		return "";
	case eResponseCodeNoModule:
		return "不存在此模块";
	case eResponseCodeNoMethod:
		return "不存在此方法";
	case eResponseCodeNoConnected:
		return "尚没有连接到服务器";
	case eResponseCodeErrorParamType:
		return "参数类型不正确";
	case eResponseCodeErrorParam:
		return "缺少参数或参数类型不正确";
	case eResponseCodeErrorServiceDataConvert:
		return "服务端数据转换出错";
	case eResponseCodeErrorLocalDataConvert:
		return "本地数据转换出错";
	case eResponseCodeErrorSend:
		return "发送数据到服务器出错";
	case eResponseCodeErrorRecv:
		return "接收服务器数据出错";
	case eResponseCodeErrorTransmit:
		return "传输出错";
	default:
		return "No such error.";
	}	
}

/**
* 解析客户端请求传输的数据的返回类型
*/
typedef enum RequestParseResult {
	eRequestParseOK = 0,			// 正常无错误
	eRequestParseEnd,				// 数据接收完毕
	eRequestParseContinue,			// 继续接收数据
	eRequestParseError,				// 接收的二进制数据不正确，包头不正确或者数据长度不正确
} RequestParseResult;

} // namespace SVS


#endif // __INCLUDE_SKYVIS_TRANSMITDEFINES_H__
