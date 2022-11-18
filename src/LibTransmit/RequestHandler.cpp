#include "RequestHandler.h"
#include "LibXML/LibXML.h"
#include "Service.h"
//#include "Logger.h"
#include <boost/thread.hpp>

//using namespace SVS::log;

namespace SVS
{

CRequestHandler::CRequestHandler(ICommandHandler * pCmdHandler,
								 const std::string sFileDir)
								 : m_pCmdHandler(pCmdHandler)
								 , m_sFileDir(sFileDir)
								 , m_bHeader(true)
								 , m_lRecved(0)
{
	ResetTransmitHeader(m_oRequestHeader);
}

CRequestHandler::~CRequestHandler(void)
{
}

RequestParseResult CRequestHandler::Parse( const char * pBuffer, std::size_t lSize )
{
	if (m_bHeader)
	{
		m_bHeader = false;
		m_lRecved = 0;
		ResetTransmitHeader(m_oRequestHeader);

		if (lSize < SKYVIS_HEADER_SIZE)
		{
            ///LogWarning("Header size is invalid!");
			return eRequestParseError;
		}

		memcpy((char *)&m_oRequestHeader, pBuffer, SKYVIS_HEADER_SIZE);

		if (strncmp(m_oRequestHeader.szMark, SKYVIS_HEADER_FLAG, SKYVIS_FLAG_SIZE) != 0)
		{
            ///LogWarning("Header is invalid!");
			return eRequestParseError;
		}

		m_binRequest.clear();
		m_binRequest.resize(m_oRequestHeader.lHeaderSize);
		switch (m_oRequestHeader.eType)
		{
		case eTransmitTypeXMLCommand:
		case eTransmitTypeFile:
			break;
		default:
			return eRequestParseError;
		}
		if (m_oRequestHeader.lHeaderSize<lSize-SKYVIS_HEADER_SIZE)
		{
            ///LogWarning("m_oRequestHeader.lHeaderSize:%d<lSize-SKYVIS_HEADER_SIZE:.%d ....",m_oRequestHeader.lHeaderSize,lSize-SKYVIS_HEADER_SIZE);
			return eRequestParseError;
		} 
		else
		{
			memcpy((char *)&m_binRequest[m_lRecved], pBuffer + SKYVIS_HEADER_SIZE, lSize - SKYVIS_HEADER_SIZE);
			m_lRecved += lSize - SKYVIS_HEADER_SIZE;

		}
		
	}
	else
	{
		SVSLONG32 lRecved_tmp=m_lRecved+lSize;
		
		if (lRecved_tmp>m_oRequestHeader.lHeaderSize)
		{
            ///LogWarning("lRecved_tmp:%d > m_oRequestHeader.lHeaderSize:%d....",lRecved_tmp,m_oRequestHeader.lHeaderSize);
			return eRequestParseError;
		} 
		else
		{
			memcpy((char *)&m_binRequest[m_lRecved], pBuffer, lSize);
			m_lRecved += lSize;
		}
		
	}

	if (m_lRecved == m_oRequestHeader.lHeaderSize)
	{
		return eRequestParseEnd;
	}
	
	return eRequestParseContinue;
}

void CRequestHandler::HandleRequest( SVSBINARY& binResponse, SVSSTRING sClientIP, SVSLONG16 lClientPort )
{
	m_bHeader = true;

	if (m_oRequestHeader.eType == eTransmitTypeConnect)
	{
		FormResponse(eTransmitTypeXMLCommand, eResponseCodeSuccess, binResponse);
	}
	else if (m_oRequestHeader.eType == eTransmitTypeXMLCommand)
	{
		HandleXMLCmdRequest(binResponse, sClientIP, lClientPort);
	}
	else if (m_oRequestHeader.eType == eTransmitTypeFile)
	{
		HandleFileRequest(binResponse);
	}
	else
	{
		FormResponse(eTransmitTypeXMLCommand, eResponseCodeErrorParamType, binResponse);
	}
}

void CRequestHandler::FormResponse( const TransmitType eType, const CData& oResult, SVSBINARY& binResponse )
{
	try
	{
		SVSSTRING sResponse;

		CData oResponseData = SVSMAP();
		oResponseData.RefMap()[SKYVIS_TRANSMIT_RESPONSE] = oResult;
		oResponseData.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = (SVSLONG32)eResponseCodeSuccess;
		CData::ToCustomXMLString(oResponseData, sResponse);

		TransmitHeader header;
		ResetTransmitHeader(header);
		header.eType = (SVSLONG16)eType;
		header.lHeaderSize = sResponse.size();

		binResponse.clear();
		binResponse.resize(SKYVIS_HEADER_SIZE + header.lHeaderSize);

		memcpy((char *)&binResponse[0], (char *)&header, SKYVIS_HEADER_SIZE);
		memcpy((char *)&binResponse[SKYVIS_HEADER_SIZE], sResponse.c_str(), header.lHeaderSize);
	}
	catch (...)
	{
		FormResponse(eTransmitTypeXMLCommand, eResponseCodeErrorParamType, binResponse);
	}
}

void CRequestHandler::FormResponse( const TransmitType eType, const ResponseCode eCode, SVSBINARY& binResponse )
{
	SVSSTRING sResponse;

	CData oResponseData = SVSMAP();
	oResponseData.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = (SVSLONG32)eCode;
	CData::ToCustomXMLString(oResponseData, sResponse);

    ///LogWarning("sResponse.....%s",sResponse.c_str());

	TransmitHeader header;
	ResetTransmitHeader(header);
	header.eType = (SVSLONG16)eType;
	header.lHeaderSize = sResponse.size();

	binResponse.clear();
	binResponse.resize(SKYVIS_HEADER_SIZE + sResponse.size());

    ///LogWarning("sResponse.size():%d....",sResponse.size());

	memcpy((char *)&binResponse[0], (char *)&header, SKYVIS_HEADER_SIZE);
	memcpy((char *)&binResponse[SKYVIS_HEADER_SIZE], sResponse.c_str(), sResponse.size());
}

void CRequestHandler::HandleXMLCmdRequest( SVSBINARY& binResponse, SVSSTRING sClientIP, SVSLONG16 lClientPort )
{
	SVSSTRING sResponse;
	SVSLONG32 lResponseSize;
	CData oResult;
	ResponseCode tCode;
	CData oRequestData;

	SVSSTRING sModule = "";
	SVSSTRING sMethod = "";

	try
	{
		SVSSTRING sRequest((const char *)&m_binRequest[0], m_binRequest.size());
		if (!CData::FromCustomString(oRequestData, sRequest))
		{
			FormResponse(eTransmitTypeXMLCommand, eResponseCodeErrorServiceDataConvert, binResponse);
			return;
		}

		sModule = oRequestData.RefMap()[SKYVIS_TRANSMIT_MODULE].RefString();
		sMethod = oRequestData.RefMap()[SKYVIS_TRANSMIT_METHOD].RefString();
		CData oParams = oRequestData.RefMap()[SKYVIS_TRANSMIT_PARAMS].RefMap();
		oParams.RefMap()[SKYVIS_TRANSMIT_CLIENTADDR] = sClientIP;
		oParams.RefMap()[SKYVIS_TRANSMIT_CLIENTPORT] = lClientPort;

		//LogInfo("Client call Module [%s] - Method [%s].",sModule.c_str(), sMethod.c_str());

		//m_pCmdHandler->p_code=eResponseCodeSuccess;
		boost::thread t(boost::bind(&ICommandHandler::Call, m_pCmdHandler,sModule, sMethod, oParams, boost::ref(oResult)));
		boost::xtime xt;
        boost::xtime_get(&xt, boost::TIME_UTC_);
		xt.sec += 80;
		if (!t.timed_join((boost::system_time)xt))
		{
			//t.interrupt();
            ///LogError("Client call Module [%s] - Method [%s].",sModule.c_str(), sMethod.c_str());
			tCode = eResponseCodeErrorParamType;
		}
		else
		{
			tCode =m_pCmdHandler->p_code;
		}
		
		//tCode = m_pCmdHandler->Call(sModule, sMethod, oParams, oResult);
		//LogInfo("Client call end, return %d.", tCode);
	}
	catch (...)
	{
        ///LogError("Client call Module [%s] - Method [%s].",sModule.c_str(), sMethod.c_str());
		tCode = eResponseCodeErrorParamType;


	}

	if (tCode == eResponseCodeSuccess)
	{
		
		FormResponse(eTransmitTypeXMLCommand, oResult, binResponse);
		
	}
	else
	{
		//LogError("FormResponse(eTransmitTypeXMLCommand, tCode, binResponse)。。。start");
		FormResponse(eTransmitTypeXMLCommand, tCode, binResponse);
		//LogError("FormResponse(eTransmitTypeXMLCommand, tCode, binResponse)。。。end");
	}
}

void CRequestHandler::HandleFileRequest( SVSBINARY& binResponse )
{

}

} // namespace SVS
