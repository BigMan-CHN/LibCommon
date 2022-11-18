/************************************************************************/
/*
* Defines.h
* 定义所有通用数据
*
* Copyright (c) 2009, Skyvis
* All rights reserved.
*
* @version		V1.00
* @author		huchou
* @date			2009-03-03
*
*
*************************************************************************/
#ifndef __SKYVIS_INCLUDE_DEFINES_H__
#define __SKYVIS_INCLUDE_DEFINES_H__

#include <stdexcept>
#include <map>
#include <vector>
#include <string>

namespace SVS {

	class CData;		// user-defined complex type

	//////////////////////////////////////////////////////////////////////////
	//自定义数据类型
	//////////////////////////////////////////////////////////////////////////
	typedef signed char						SVSBOOL;	// bool,0 and ~0
	typedef unsigned char					SVSLONG8;	// 8-bit unsigned integer
	typedef unsigned short int				SVSLONG16;	// 16-bit unsigned integer
	typedef unsigned int					SVSLONG32;	// 32-bit unsigned integer
	typedef unsigned long long				SVSLONG64;	// 64-bit unsigned integer
	typedef float							SVSFLOAT;	// float type
	typedef double							SVSDOUBLE;	// double type
	typedef std::string						SVSSTRING;	// string
	typedef std::vector<CData>				SVSVECTOR;	// array
	typedef std::map<SVSSTRING, CData>		SVSMAP;		// map
	typedef std::vector<unsigned char>		SVSBINARY;	// binary data

#ifndef SVSTRUE
#define SVSTRUE		((SVSBOOL)1)
#endif

#ifndef SVSFALSE
#define SVSFALSE	((SVSBOOL)0)
#endif

#define SVSSUCCEEDED(x)		(x != SVSFALSE)
#define SVSFAILED(x)		(x == SVSFALSE)

#define SKYVIS_XML_ROOT		"Root"

} // namespace SVS

#endif // __SKYVIS_INCLUDE_DEFINES_H__