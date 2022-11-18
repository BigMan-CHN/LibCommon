#include "LibXML.h"
#include <stdarg.h>
#include <sstream>
#include <cstring>

namespace SVS
{

const SVSSTRING sDataType[12] = {
	"NULL",
	"BOOL",
	"LONG8",
	"LONG16",
	"LONG32",
	"LONG64",
	"FLOAT",
	"DOUBLE",
	"STRING",
	"VECTOR",
	"MAP",
	"BINARY"
}; // 字符串型数据类型

//////////////////////////////////////////////////////////////////////////

CData::DataType CData::GetType( const SVSSTRING sType )
{
	for (SVSLONG16 i = 1; i < eDataTypeSize; i++)
	{
		if (sType == sDataType[i])
		{
			return (DataType)i;
		}
	}

	return eDataTypeNull;
}

SVSSTRING CData::GetTypeString( const DataType enumType )
{
	return sDataType[enumType];
}

//////////////////////////////////////////////////////////////////////////

CData::CData()
	: m_enumType(eDataTypeNull)
	, m_pData(NULL)
	, m_pRefCount(NULL)
{

}

CData::~CData()
{
	Release();
}


//////////////////////////////////////////////////////////////////////////
// 构造函数

CData::CData( const SVSBOOL &bValue )
	: m_enumType(eDataTypeBool)
{
	m_pData		= new SVSBOOL(bValue);
	m_pRefCount	= new SVSLONG32(1);
}

CData::CData( const SVSLONG8 &lValue )
	: m_enumType(eDataTypeLong8)
{
	m_pData = new SVSLONG8(lValue);
	m_pRefCount	= new SVSLONG32(1);
}

CData::CData( const SVSLONG16 &lValue )
	: m_enumType(eDataTypeLong16)
{
	m_pData = new SVSLONG16(lValue);
	m_pRefCount	= new SVSLONG32(1);
}

CData::CData( const SVSLONG32 &lValue )
	: m_enumType(eDataTypeLong32)
{
	m_pData = new SVSLONG32(lValue);
	m_pRefCount	= new SVSLONG32(1);
}

CData::CData( const SVSLONG64 &lValue )
	: m_enumType(eDataTypeLong64)
{
	m_pData = new SVSLONG64(lValue);
	m_pRefCount	= new SVSLONG32(1);
}

CData::CData( const SVSFLOAT &fValue )
	: m_enumType(eDataTypeFloat)
{
	m_pData = new SVSFLOAT(fValue);
	m_pRefCount	= new SVSLONG32(1);
}

CData::CData( const SVSDOUBLE &dValue )
	: m_enumType(eDataTypeDouble)
{
	m_pData = new SVSDOUBLE(dValue);
	m_pRefCount	= new SVSLONG32(1);
}

CData::CData( const SVSSTRING &sValue )
	: m_enumType(eDataTypeString)
{
	m_pData = new SVSSTRING(sValue);
	m_pRefCount	= new SVSLONG32(1);
}

CData::CData( const SVSVECTOR &listValue )
	: m_enumType(eDataTypeVector)
{
	m_pData = new SVSVECTOR(listValue);
	m_pRefCount	= new SVSLONG32(1);
}

CData::CData( const SVSMAP &mapValue )
	: m_enumType(eDataTypeMap)
{
	m_pData = new SVSMAP(mapValue);
	m_pRefCount	= new SVSLONG32(1);
}

CData::CData( const SVSBINARY &binValue )
	: m_enumType(eDataTypeBinary)
{
	m_pData = new SVSBINARY(binValue);
	m_pRefCount	= new SVSLONG32(1);
}

CData::CData( const CData &oData )
	: m_enumType(oData.m_enumType)
	, m_pData(oData.m_pData)
	, m_pRefCount(oData.m_pRefCount)
{
	if (m_pRefCount != NULL)
	{
		(*m_pRefCount)++;
	}
}

//////////////////////////////////////////////////////////////////////////
// 赋值函数

CData& CData::operator= ( const SVSBOOL &bValue )
{
	Release();
	m_pData		= new SVSBOOL(bValue);
	m_enumType	= eDataTypeBool;
	m_pRefCount	= new SVSLONG32(1);

	return * this;
}

CData& CData::operator= ( const SVSLONG8 &lValue )
{
	Release();
	m_pData		= new SVSLONG8(lValue);
	m_enumType	= eDataTypeLong8;
	m_pRefCount	= new SVSLONG32(1);

	return * this;
}

CData& CData::operator= ( const SVSLONG16 &lValue )
{
	Release();
	m_pData		= new SVSLONG16(lValue);
	m_enumType	= eDataTypeLong16;
	m_pRefCount	= new SVSLONG32(1);

	return * this;
}

CData& CData::operator= ( const SVSLONG32 &lValue )
{
	Release();
	m_pData		= new SVSLONG32(lValue);
	m_enumType	= eDataTypeLong32;
	m_pRefCount	= new SVSLONG32(1);

	return * this;
}

CData& CData::operator= ( const SVSLONG64 &lValue )
{
	Release();
	m_pData		= new SVSLONG64(lValue);
	m_enumType	= eDataTypeLong64;
	m_pRefCount	= new SVSLONG32(1);

	return * this;
}

CData& CData::operator= ( const SVSFLOAT &fValue )
{
	Release();
	m_pData		= new SVSFLOAT(fValue);
	m_enumType	= eDataTypeFloat;
	m_pRefCount	= new SVSLONG32(1);

	return * this;
}

CData& CData::operator= ( const SVSDOUBLE &dValue )
{
	Release();
	m_pData		= new SVSDOUBLE(dValue);
	m_enumType	= eDataTypeDouble;
	m_pRefCount	= new SVSLONG32(1);

	return * this;
}

CData& CData::operator= ( const SVSSTRING &sValue )
{
	Release();
	m_pData		= new SVSSTRING(sValue);
	m_enumType	= eDataTypeString;
	m_pRefCount	= new SVSLONG32(1);

	return * this;
}

CData& CData::operator= ( const SVSVECTOR &listValue )
{
	Release();
	m_pData		= new SVSVECTOR(listValue);
	m_enumType	= eDataTypeVector;
	m_pRefCount	= new SVSLONG32(1);

	return * this;
}

CData& CData::operator= ( const SVSMAP &mapValue )
{
	Release();
	m_pData		= new SVSMAP(mapValue);
	m_enumType	= eDataTypeMap;
	m_pRefCount	= new SVSLONG32(1);

	return * this;
}

CData& CData::operator= ( const SVSBINARY &binValue )
{
	Release();
	m_pData		= new SVSBINARY(binValue);
	m_enumType	= eDataTypeBinary;
	m_pRefCount	= new SVSLONG32(1);

	return * this;
}

CData& CData::operator= ( const CData &oData )
{
	if ( this == &oData)
	{
		return * this;
	}

	Release();

	m_enumType	= oData.m_enumType;
	m_pData		= oData.m_pData;
	m_pRefCount	= oData.m_pRefCount;

	if (m_pRefCount != NULL)
	{
		(*m_pRefCount)++;
	}

	return * this;
}

//////////////////////////////////////////////////////////////////////////
// 释放资源

void CData::Release()
{
	if(m_enumType == eDataTypeNull) return;

	SVSLONG32 lRefCount = --(*m_pRefCount);
	if (lRefCount == 0)
	{
		switch (m_enumType)
		{
		case eDataTypeBool:
			delete (SVSBOOL *)m_pData;
			break;
		case eDataTypeLong8:
			delete (SVSLONG8 *)m_pData;
			break;
		case eDataTypeLong16:
			delete (SVSLONG16 *)m_pData;
			break;
		case eDataTypeLong32:
			delete (SVSLONG32 *)m_pData;
			break;
		case eDataTypeLong64:
			delete (SVSLONG64 *)m_pData;
			break;
		case eDataTypeFloat:
			delete (SVSFLOAT *)m_pData;
			break;
		case eDataTypeDouble:
			delete (SVSDOUBLE *)m_pData;
			break;
		case eDataTypeString:
			delete (SVSSTRING *)m_pData;
			break;
		case eDataTypeVector:
			delete (SVSVECTOR *)m_pData;
			break;
		case eDataTypeMap:
			delete (SVSMAP *)m_pData;
			break;
		case eDataTypeBinary:
			delete (SVSBINARY *)m_pData;
			break;
		}

		delete m_pRefCount;
	}

	m_pData = NULL;
	m_pRefCount = NULL;
	m_enumType = eDataTypeNull;
}

//////////////////////////////////////////////////////////////////////////
// 复制数据生成一个新对象

CData CData::Clone() const
{
	CData oNewData;
	switch (m_enumType)
	{
	case eDataTypeBool:
		oNewData = RefBool();
		break;
	case eDataTypeLong8:
		oNewData = RefLong8();
		break;
	case eDataTypeLong16:
		oNewData = RefLong16();
		break;
	case eDataTypeLong32:
		oNewData = RefLong32();
		break;
	case eDataTypeLong64:
		oNewData = RefLong64();
		break;
	case eDataTypeFloat:
		oNewData = RefFloat();
		break;
	case eDataTypeDouble:
		oNewData = RefDouble();
		break;
	case eDataTypeString:
		oNewData = RefString();
		break;
	case eDataTypeBinary:
		oNewData = RefBinary();
		break;
	case eDataTypeVector:
		{
			oNewData = SVSVECTOR();
			for(SVSVECTOR::const_iterator it = RefVector().begin();
				it != RefVector().end(); it++)
			{
				oNewData.RefVector().push_back(it->Clone());
			}
		}
		break;
	case eDataTypeMap:
		{
			oNewData = SVSMAP();
			for(SVSMAP::const_iterator it = RefMap().begin();
				it != RefMap().end();it++)
			{
				oNewData.RefMap().insert(std::make_pair(it->first.c_str(),
					it->second.Clone()));
			}
		}
		break;
	default:
		throw std::runtime_error("invalid type");
	}

	return oNewData;
}

//////////////////////////////////////////////////////////////////////////

SVSBOOL CData::SaveFile( CData& oData, SVSSTRING sFile )
{
	if (oData.m_enumType != eDataTypeMap)
	{
		return SVSFALSE;
	}
	
	SVSSTRING sXMLString;
	if (!ToCustomXMLString(oData, sXMLString))
	{
		return SVSFALSE;
	}
	
	std::ostringstream os;
	os << "<?xml version=\"1.0\" encoding=\"gb2312\"?>" << sXMLString;

	XMLResults xe;
	XMLNode xMainNode = XMLNode::parseString(os.str().c_str(), NULL, &xe);
	
	return xMainNode.writeToFile(sFile.c_str()) == eXMLErrorNone ? 1 : 0;
}

SVSBOOL CData::ToCustomXMLString( const CData& oData, SVSSTRING& sValue )
{
	if (oData.m_enumType != eDataTypeMap)
	{
		return SVSFALSE;
	}

	XMLNode oRootNode = XMLNode::createXMLTopNode(SKYVIS_XML_ROOT, false);

	CustomDataCustomXMLTree(oData, oRootNode);

	char* sXml = oRootNode.createXMLString(false);
	sValue = sXml;
	free(sXml);

	return SVSTRUE;
}

SVSBOOL CData::CustomDataCustomXMLTree( const CData& oData, XMLNode& newNode )
{
	switch (oData.m_enumType)
	{
	case eDataTypeBool:
		{
			std::ostringstream os;
			os << (SVSSUCCEEDED(oData.RefBool()) ? 1 : 0);
			newNode.addAttribute("type", "BOOL");
			newNode.addText(os.str().c_str());
		}
		break;
	case eDataTypeLong8:
		{
			std::ostringstream os;
			os << oData.RefLong8();
			newNode.addAttribute("type", "LONG8");
			newNode.addText(os.str().c_str());
		}
		break;
	case eDataTypeLong16:
		{
			std::ostringstream os;
			os << oData.RefLong16();
			newNode.addAttribute("type", "LONG16");
			newNode.addText(os.str().c_str());
		}
		break;
	case eDataTypeLong32:
		{
			std::ostringstream os;
			os << oData.RefLong32();
			newNode.addAttribute("type", "LONG32");
			newNode.addText(os.str().c_str());
		}
		break;
	case eDataTypeLong64:
		{
			std::ostringstream os;
			os << oData.RefLong64();
			newNode.addAttribute("type", "LONG64");
			newNode.addText(os.str().c_str());
		}
		break;
	case eDataTypeFloat:
		{
			std::ostringstream os;
			os << oData.RefFloat();
			newNode.addAttribute("type", "FLOAT");
			newNode.addText(os.str().c_str());
		}
		break;
	case eDataTypeDouble:
		{
			std::ostringstream os;
			os << oData.RefDouble();
			newNode.addAttribute("type", "DOUBLE");
			newNode.addText(os.str().c_str());
		}
		break;
	case eDataTypeString:
		{
			newNode.addAttribute("type", "STRING");
			newNode.addClear(oData.RefString().c_str());
		}
		break;
	case eDataTypeBinary:
		{
			// base64 encode
			newNode.addAttribute("type", "BINARY");
			XMLParserBase64Tool oBase64Tool;
			SVSSTRING sText = oBase64Tool.encode(&oData.RefBinary()[0], oData.RefBinary().size());
			newNode.addText(sText.c_str());
		}
		break;
	case eDataTypeVector:
		{
			newNode.addAttribute("type", "VECTOR");
			for(int i = 0; i < oData.RefVector().size(); i++)
			{
				std::ostringstream os;
				os << "A" << i; // XML节点名称不能为数字
				XMLNode childNode = newNode.addChild(os.str().c_str());
				CustomDataCustomXMLTree(oData.RefVector()[i], childNode);
			}
		}
		break;
	case eDataTypeMap:
		{
			newNode.addAttribute("type", "MAP");
			for(SVSMAP::const_iterator it = oData.RefMap().begin();
				it != oData.RefMap().end(); it++)
			{
				if(it->first.c_str()!=NULL)
				{
					XMLNode childNode = newNode.addChild(it->first.c_str());
					CustomDataCustomXMLTree(it->second, childNode);
				}
				
			}
		}
		break;
	default:
		return SVSFALSE;
	}

	return SVSTRUE;
}

//////////////////////////////////////////////////////////////////////////

SVSBOOL CData::FromFile( CData& oData, SVSSTRING sFile )
{
	oData = SVSMAP();
	
	try
	{
		XMLNode oRootNode = XMLNode::openFileHelper(sFile.c_str(), SKYVIS_XML_ROOT);
		if (oRootNode.isEmpty())
		{
			return SVSFALSE;
		}
		else if (eDataTypeMap != CData::GetType(oRootNode.getAttribute("type")))
		{
			return SVSFALSE;
		}
		else if (oRootNode.nChildNode() == 0)
		{
			return SVSTRUE;
		}

		return CustomXMLTree2CustomData(oRootNode, oData);
	}
	catch (...)
	{
	}

	return SVSFALSE;
}

SVSBOOL CData::FromCustomString( CData& oData, const SVSSTRING& sXMLString )
{
	oData = SVSMAP();

	try
	{
		XMLResults xe;
		XMLNode oRootNode = XMLNode::parseString(sXMLString.c_str(), NULL, &xe);
		if (oRootNode.isEmpty() || xe.error != eXMLErrorNone)
		{
			return SVSFALSE;
		}
		else if (eDataTypeMap != CData::GetType(oRootNode.getAttribute("type")))
		{
			return SVSFALSE;
		}

		return CustomXMLTree2CustomData(oRootNode, oData);
	}
	catch (...)
	{
	}

	return SVSFALSE;
}

SVSBOOL CData::CustomXMLTree2CustomData( const XMLNode& oNode, CData& oData )
{
	switch (CData::GetType(oNode.getAttribute("type")))
	{
	case eDataTypeBool:
		{
			if (oNode.getText() != NULL)
				oData = (strcmp(oNode.getText(), "0") != 0 ? SVSTRUE : SVSFALSE);
			else
				oData = SVSFALSE;
		}
		break;
	case eDataTypeLong8:
		{
			if (oNode.getText() != NULL)
			{
				std::istringstream is;
				is.str(oNode.getText());
				SVSLONG8 lValue;
				is >> lValue;
				oData = lValue;
			}
			else
				oData = (SVSLONG8)0;
		}
		break;
	case eDataTypeLong16:
		{
			if (oNode.getText() != NULL)
			{
				std::istringstream is;
				is.str(oNode.getText());
				SVSLONG16 lValue;
				is >> lValue;
				oData = lValue;
			}
			else
				oData = (SVSLONG16)0;
		}
		break;
	case eDataTypeLong32:
		{
			if (oNode.getText() != NULL)
			{
				std::istringstream is;
				is.str(oNode.getText());
				SVSLONG32 lValue;
				is >> lValue;
				oData = lValue;
			}
			else
				oData = (SVSLONG32)0;
		}
		break;
	case eDataTypeLong64:
		{
			if (oNode.getText() != NULL)
			{
				std::istringstream is;
				is.str(oNode.getText());
				SVSLONG64 lValue;
				is >> lValue;
				oData = lValue;
			}
			else
				oData = (SVSLONG64)0;
		}
		break;
	case eDataTypeFloat:
		{
			if (oNode.getText() != NULL)
			{
				std::istringstream is;
				is.str(oNode.getText());
				SVSFLOAT fValue;
				is >> fValue;
				oData = fValue;
			}
			else
				oData = (SVSFLOAT)0;
		}
		break;
	case eDataTypeDouble:
		{
			if (oNode.getText() != NULL)
			{
				std::istringstream is;
				is.str(oNode.getText());
				SVSDOUBLE dValue;
				is >> dValue;
				oData = dValue;
			}
			else
				oData = (SVSDOUBLE)0;
		}
		break;
	case eDataTypeString:
		{
			XMLClear xc = oNode.getClear();
			if (xc.lpszValue != NULL)
			{
				oData = SVSSTRING(xc.lpszValue);
			}
			else
			{
				oData = "";
			}
		}
		break;
	case eDataTypeBinary:
		{
			if (oNode.getText() != NULL)
			{
				// base64 encode
				XMLError oError;
				int lLen;
				XMLParserBase64Tool oBase64Tool;
				unsigned char* szValue = oBase64Tool.decode(oNode.getText(), &lLen, &oError);
				SVSBINARY oBin(lLen);
				memcpy(&oBin[0], szValue, lLen);
				oData = oBin;
			}
			else
				oData = SVSBINARY();
		}
		break;
	case eDataTypeVector:
		{
			SVSLONG32 lNodeCount = oNode.nChildNode();
			oData = SVSVECTOR(lNodeCount);
			for (SVSLONG32 i = 0; i < lNodeCount; i++)
			{
				CData oChildData;
				CustomXMLTree2CustomData(oNode.getChildNode(i), oChildData);
				oData.RefVector()[atol(&oNode.getChildNode(i).getName()[1])] = oChildData;
			}
		}
		break;
	case eDataTypeMap:
		{
			oData = SVSMAP();
			SVSLONG32 lNodeCount = oNode.nChildNode();
			for (SVSLONG32 i = 0; i < lNodeCount; i++)
			{
				CData oChildData;
				CustomXMLTree2CustomData(oNode.getChildNode(i), oChildData);
				oData.RefMap()[oNode.getChildNode(i).getName()] = oChildData;
			}
		}
		break;
	default:
		//throw std::runtime_error("invalid type");
		return SVSFALSE;
	}

	return SVSTRUE;
}

} // namespace SVS