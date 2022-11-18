/************************************************************************/
/*
* LibXML.h
* Copyright (c) 2009, Skyvis
* All rights reserved.
*
* @version		V1.00
* @author		huchou
* @date			2009-03-03
*
*
*************************************************************************/

#ifndef __SKYVIS_INCLUDE_LIBXML_H__
#define __SKYVIS_INCLUDE_LIBXML_H__

#include "xmlParser.h"
#include "Defines.h"

namespace SVS {

class CData
{
public:
	/**
	* ��ֵ����������
	*/
	typedef enum DataType {
		eDataTypeNull = 0x00,
		eDataTypeBool,
		eDataTypeLong8,
		eDataTypeLong16,
		eDataTypeLong32,
		eDataTypeLong64,
		eDataTypeFloat,
		eDataTypeDouble,
		eDataTypeString,
		eDataTypeVector,
		eDataTypeMap,
		eDataTypeBinary,
		eDataTypeSize	// �������͸���
	} DataType;

	/**
	* �����ַ����͵������������ֵ�͵���������
	* @result ������ֵ�͵���������
	*/
	static DataType GetType( const SVSSTRING sType);

	/**
	* ������ֵ�͵�����������ַ����͵���������
	* @result �����ַ����͵���������
	*/
	static SVSSTRING GetTypeString( const DataType enumType);
		// 

public:
	CData();
	~CData();

	// ���ݵĹ��캯��
	CData(const SVSBOOL &bValue);
	CData(const SVSLONG8 &lValue);
	CData(const SVSLONG16 &lValue);
	CData(const SVSLONG32 &lValue);
	CData(const SVSLONG64 &lValue);
	CData(const SVSFLOAT &fValue);
	CData(const SVSDOUBLE &dValue);
	CData(const SVSSTRING &sValue);
	CData(const SVSVECTOR &listValue);
	CData(const SVSMAP &mapValue);
	CData(const SVSBINARY &binValue);
	CData(const CData &oData);

	// ���ݵĸ�ֵ����
	CData& operator= (const SVSBOOL &bValue);
	CData& operator= (const SVSLONG8 &lValue);
	CData& operator= (const SVSLONG16 &lValue);
	CData& operator= (const SVSLONG32 &lValue);
	CData& operator= (const SVSLONG64 &lValue);
	CData& operator= (const SVSFLOAT &fValue);
	CData& operator= (const SVSDOUBLE &dValue);
	CData& operator= (const SVSSTRING &sValue);
	CData& operator= (const SVSVECTOR &listValue);
	CData& operator= (const SVSMAP &mapValue);
	CData& operator= (const SVSBINARY &binValue);
	CData& operator= (const CData &oData);

	/**
	* �õ���������
	*/
	DataType GetType() const { return m_enumType; }

	/**
	* �ͷ���Դ
	*/
	void Release();

	/**
	* ������������һ���¶���
	* @result ���ذ���ͬ�����ݵ��¶���
	*/
	CData Clone() const;

public:
	// �������ݵ�ָ��
	SVSBOOL * PBool()
	{
		CheckType(m_enumType, eDataTypeBool);
		return (SVSBOOL *)m_pData;
	}
	SVSLONG8 * PLong8()
	{
		CheckType(m_enumType, eDataTypeLong8);
		return (SVSLONG8 *)m_pData;
	}
	SVSLONG16 * PLong16()
	{
		CheckType(m_enumType, eDataTypeLong16);
		return (SVSLONG16 *)m_pData;
	}
	SVSLONG32 * PLong32()
	{
		CheckType(m_enumType, eDataTypeLong32);
		return (SVSLONG32 *)m_pData;
	}
	SVSLONG64 * PLong64()
	{
		CheckType(m_enumType, eDataTypeLong64);
		return (SVSLONG64 *)m_pData;
	}
	SVSFLOAT * PFloat()
	{
		CheckType(m_enumType, eDataTypeFloat);
		return (SVSFLOAT *)m_pData;
	}
	SVSDOUBLE * PDouble()
	{
		CheckType(m_enumType, eDataTypeDouble);
		return (SVSDOUBLE *)m_pData;
	}
	SVSSTRING * PString()
	{
		CheckType(m_enumType, eDataTypeString);
		return (SVSSTRING *)m_pData;
	}
	SVSVECTOR * PVector()
	{
		CheckType(m_enumType, eDataTypeVector);
		return (SVSVECTOR *)m_pData;
	}
	SVSMAP * PMap()
	{
		CheckType(m_enumType, eDataTypeMap);
		return (SVSMAP *)m_pData;
	}
	SVSBINARY * PBinary()
	{
		CheckType(m_enumType, eDataTypeBinary);
		return (SVSBINARY *)m_pData;
	}

	// �������
	const SVSBOOL * PBool() const
	{
		CheckType(m_enumType, eDataTypeBool);
		return (SVSBOOL *)m_pData;
	}
	const SVSLONG8 * PLong8() const
	{
		CheckType(m_enumType, eDataTypeLong8);
		return (SVSLONG8 *)m_pData;
	}
	const SVSLONG16 * PLong16() const
	{
		CheckType(m_enumType, eDataTypeLong16);
		return (SVSLONG16 *)m_pData;
	}
	const SVSLONG32 * PLong32() const
	{
		CheckType(m_enumType, eDataTypeLong32);
		return (SVSLONG32 *)m_pData;
	}
	const SVSLONG64 * PLong64() const
	{
		CheckType(m_enumType, eDataTypeLong64);
		return (SVSLONG64 *)m_pData;
	}
	const SVSFLOAT * PFloat() const
	{
		CheckType(m_enumType, eDataTypeFloat);
		return (SVSFLOAT *)m_pData;
	}
	const SVSDOUBLE * PDouble() const
	{
		CheckType(m_enumType, eDataTypeDouble);
		return (SVSDOUBLE *)m_pData;
	}
	const SVSSTRING * PString() const
	{
		CheckType(m_enumType, eDataTypeString);
		return (SVSSTRING *)m_pData;
	}
	const SVSVECTOR * PVector() const
	{
		CheckType(m_enumType, eDataTypeVector);
		return (SVSVECTOR *)m_pData;
	}
	const SVSMAP * PMap() const
	{
		CheckType(m_enumType, eDataTypeMap);
		return (SVSMAP *)m_pData;
	}
	const SVSBINARY * PBinary() const
	{
		CheckType(m_enumType, eDataTypeBinary);
		return (SVSBINARY *)m_pData;
	}

public:
	//�������ݵ�����
	SVSBOOL & RefBool()
	{
		CheckType(m_enumType, eDataTypeBool);
		return *(SVSBOOL *)m_pData;
	}
	SVSLONG8 & RefLong8()
	{
		CheckType(m_enumType, eDataTypeLong8);
		return *(SVSLONG8 *)m_pData;
	}
	SVSLONG16 & RefLong16()
	{
		CheckType(m_enumType, eDataTypeLong16);
		return *(SVSLONG16 *)m_pData;
	}
	SVSLONG32 & RefLong32()
	{
		CheckType(m_enumType, eDataTypeLong32);
		return *(SVSLONG32 *)m_pData;
	}
	SVSLONG64 & RefLong64()
	{
		CheckType(m_enumType, eDataTypeLong64);
		return *(SVSLONG64 *)m_pData;
	}
	SVSFLOAT & RefFloat()
	{
		CheckType(m_enumType, eDataTypeFloat);
		return *(SVSFLOAT *)m_pData;
	}
	SVSDOUBLE & RefDouble()
	{
		CheckType(m_enumType, eDataTypeDouble);
		return *(SVSDOUBLE *)m_pData;
	}
	SVSSTRING & RefString()
	{
		CheckType(m_enumType, eDataTypeString);
		return *(SVSSTRING *)m_pData;
	}
	SVSVECTOR &RefVector()
	{
		CheckType(m_enumType, eDataTypeVector);
		return *(SVSVECTOR *)m_pData;
	}
	SVSMAP & RefMap()
	{
		CheckType(m_enumType, eDataTypeMap);
		return *(SVSMAP *)m_pData;
	}
	SVSBINARY & RefBinary()
	{
		CheckType(m_enumType, eDataTypeBinary);
		return *(SVSBINARY *)m_pData;
	}

	// �������
	const SVSBOOL & RefBool() const
	{
		CheckType(m_enumType, eDataTypeBool);
		return *(SVSBOOL *)m_pData;
	}
	const SVSLONG8 & RefLong8() const
	{
		CheckType(m_enumType, eDataTypeLong8);
		return *(SVSLONG8 *)m_pData;
	}
	const SVSLONG16 & RefLong16() const
	{
		CheckType(m_enumType, eDataTypeLong16);
		return *(SVSLONG16 *)m_pData;
	}
	const SVSLONG32 & RefLong32() const
	{
		CheckType(m_enumType, eDataTypeLong32);
		return *(SVSLONG32 *)m_pData;
	}
	const SVSLONG64 & RefLong64() const
	{
		CheckType(m_enumType, eDataTypeLong64);
		return *(SVSLONG64 *)m_pData;
	}
	const SVSFLOAT & RefFloat() const
	{
		CheckType(m_enumType, eDataTypeFloat);
		return *(SVSFLOAT *)m_pData;
	}
	const SVSDOUBLE & RefDouble() const
	{
		CheckType(m_enumType, eDataTypeDouble);
		return *(SVSDOUBLE *)m_pData;
	}
	const SVSSTRING & RefString() const
	{
		CheckType(m_enumType, eDataTypeString);
		return *(SVSSTRING *)m_pData;
	}
	const SVSVECTOR &RefVector() const
	{
		CheckType(m_enumType, eDataTypeVector);
		return *(SVSVECTOR *)m_pData;
	}
	const SVSMAP & RefMap() const
	{
		CheckType(m_enumType, eDataTypeMap);
		return *(SVSMAP *)m_pData;
	}
	const SVSBINARY & RefBinary() const
	{
		CheckType(m_enumType, eDataTypeBinary);
		return *(SVSBINARY *)m_pData;
	}

private:
	/**
	* ���Զ�������ת�����Զ���XML
	* @param oData ������������
	* @param newNode �����õ���XML���ڵ�
	* @result �ɹ�����SVSTRUE�����򷵻�SVSFALSE
	*/
	static SVSBOOL CustomDataCustomXMLTree(const CData& oData, XMLNode& newNode);

	/**
	* ���Զ���XML��ת�����Զ�������������
	* @param oNode �������������ݵ�XML���ڵ�
	* @param oData �����õ�������
	* @result �ɹ�����SVSTRUE�����򷵻�SVSFALSE
	*/
	static SVSBOOL CustomXMLTree2CustomData(const XMLNode& oNode, CData& oData);

public:
	/**
	* ������ת�����Զ���XML��ʽ���浽�ļ���
	* @param oData ������������
	* @param sFile �����õ���XML�ı����ŵ��ļ�·��
	* @result �ɹ�����SVSTRUE�����򷵻�SVSFALSE
	*/
	static SVSBOOL SaveFile(CData& oData, SVSSTRING sFile);
		// 

	/**
	* ������ת�����Զ���XML��ʽ���ַ����������XMLͷ<?xml version="1.0" encoding="gb2312"?>
	* @param oData ������������
	* @param sXMLString �����õ���XML�ı�
	* @result �ɹ�����SVSTRUE�����򷵻�SVSFALSE
	*/
	static SVSBOOL ToCustomXMLString(const CData& oData, SVSSTRING& sXMLString);

	/**
	* ���ļ��н����Զ���XML��ʽ�õ�����
	* @param oData �����õ�������
	* @param sFile �������İ���XML�ı����ļ�·��
	* @result �ɹ�����SVSTRUE�����򷵻�SVSFALSE
	*/
	static SVSBOOL FromFile(CData& oData, SVSSTRING sFile);
	
	/**
	* ���Զ���XML��ʽ���ַ��������õ�����
	* @param oData �����õ�������
	* @param sXMLString ��������XML�ı�
	* @result �ɹ�����SVSTRUE�����򷵻�SVSFALSE
	*/
	static SVSBOOL FromCustomString(CData& oData, const SVSSTRING& sXMLString);

private:
	/**
	* �������������Ƿ�һ�£���һ���׳��쳣
	* @param enumType1 ��������1
	* @param enumType2 ��������2
	*/
	void CheckType(DataType enumType1, DataType enumType2) const 
	{
		if (enumType1 != enumType2)
		{
			throw std::runtime_error("type unmatch");
		}
	}

private:
	mutable DataType	m_enumType;		// ��������
	mutable void *		m_pData;		// ָ�������׵�ַ��ָ��
	mutable SVSLONG32 *	m_pRefCount;	// ָ���������ô�����ָ��
};

} // namespace SVS
#endif // __SKYVIS_INCLUDE_LIBXML_H__
