#pragma once
#include "LibTransmit/Server.h"
#include "LibXML/LibXML.h"
#include <boost/thread/mutex.hpp>

#define MODULE_CONTROL	"Control"

class CMControl :
	public SVS::IModule
{
public:
	CMControl(void);
	~CMControl(void);

	virtual const SVS::SVSSTRING GetName() const;
	virtual SVS::ResponseCode Execute(const SVS::SVSSTRING& sMethod, SVS::CData& oParams, SVS::CData& oResult);

public:
	void Login(SVS::CData& oParams, SVS::CData& oResult);

private:
	boost::mutex	m_oMutexFunc;
};
