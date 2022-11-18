#include "MControl.h"

using namespace std;
using namespace SVS;

CMControl::CMControl(void)
{
}

CMControl::~CMControl(void)
{
}

const SVS::SVSSTRING CMControl::GetName() const
{
	return MODULE_CONTROL;
}

ResponseCode CMControl::Execute(const SVSSTRING& sMethod, SVS::CData& oParams, SVS::CData& oResult)
{
	boost::mutex::scoped_lock oScopedLock(m_oMutexFunc);

	if (sMethod == "Login")
	{
		Login(oParams, oResult);
		return SVS::eResponseCodeSuccess;
	}
	else
	{
		return SVS::eResponseCodeNoMethod;
	}

	return SVS::eResponseCodeSuccess;
}

void CMControl::Login( SVS::CData& oParams, SVS::CData& oResult )
{
	SVSSTRING sName = oParams.RefMap()["sName"].RefString();
	SVSSTRING sPass = oParams.RefMap()["sPass"].RefString();
	SVSSTRING sAddr = oParams.RefMap()[SKYVIS_TRANSMIT_CLIENTADDR].RefString();
	SVSLONG16 lPort = oParams.RefMap()[SKYVIS_TRANSMIT_CLIENTPORT].RefLong16();

	std::cout << "CMControl received a client(" << sAddr << ":" << lPort 
		<< ") call : Test()" << std::endl;

	oResult = SVSMAP();
	if (sName == "skyvis" && sPass == "123")
	{
		oResult.RefMap()[SKYVIS_TRANSMIT_RESULT] = SVSTRUE;
	}
	else
	{
		oResult.RefMap()[SKYVIS_TRANSMIT_RESULT] = SVSFALSE;
	}
}