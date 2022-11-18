/************************************************************************/
/*
* Logger.h
* a cross-platform log tool。
*
* Copyright (c) 2009, Skyvis
* All rights reserved.
*
* @version		V1.00
* @author		huchou
* @date			2009-04-20
*
* Example:
* #include "Logger.h"
* using namespace SVS::log;
* 
SVS::log::CLogger SVS::log::theLogger;
* int main(int argc, char* argv[])
* {
* 	// Only log to console
* 	theLogger.Create(eLog_DEBUG);
* 	or
* 	// Log to console and file
* 	theLogger.Create("B:\\", "test.log", eLog_DEBUG);
* 
* 	LogDebug("debug");
* 	LogInfo("info");
* 	LogWarning("warning");
* 	LogError("error");
* 	return 0;
* }
*************************************************************************/
#ifndef __SKYVIS_INCLUDE_LOGGER_H__
#define __SKYVIS_INCLUDE_LOGGER_H__

#include <time.h>
#include <assert.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sstream>
#include <boost/thread/mutex.hpp>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#pragma comment(lib, "Kernel32.lib")

#endif // _WIN32


namespace SVS
{
	namespace log
	{

#ifndef ARRSIZE
#define ARRSIZE(x)	(sizeof(x)/sizeof(x[0]))
#endif

		enum LogLevel {
			eLog_DEBUG = 0,
			eLog_INFO,
			eLog_WARNING,
			eLog_ERROR,
		};

		//////////////////////////////////////////////////////////////////////////
		// CLogger

		class CLogger
		{
		public:
			CLogger() 
				: m_fp(NULL)
				, m_tStarted(0)
				, m_sLogDir("")
				, m_sFileName("")
				, m_uBytesWritten(0)
				, m_uMaxFileSize((unsigned int)-1)
				, m_bOpened(false)
				, m_bLog2File(false)
				, m_eLogLevel(eLog_INFO)
			{

			}
			~CLogger()
			{
				Close();
			}

			bool IsOpen() const { return m_bOpened; }
			bool IsLog2File() const { return m_bLog2File; }
			const std::string& GeLogDir() const { return m_sLogDir; }
			const std::string& GetFileName() const { return m_sFileName; }
			const LogLevel GetLogLevel() const { return m_eLogLevel; }

			bool Create(LogLevel eLogLevel = eLog_INFO, unsigned int uMaxFileSize = 1024*1024)
			{
				boost::mutex::scoped_lock lock(m_oMutexFunc);

				Close();
				m_bLog2File = false;
				m_eLogLevel = eLogLevel;
				SetMaxFileSize(uMaxFileSize);
				return Open();
			}

			bool Create(const char * pszLogDir, const char * pszFileName, 
				LogLevel eLogLevel = eLog_INFO, unsigned int uMaxFileSize = 1024*1024)
			{
				boost::mutex::scoped_lock lock(m_oMutexFunc);

				Close();
				m_bLog2File = true;
				m_sLogDir = pszLogDir;
				m_sFileName = pszFileName;
				m_eLogLevel = eLogLevel;
				SetMaxFileSize(uMaxFileSize);
				return Open();
			}

			bool Log(LogLevel eLogLevel, std::string& sMsg)
			{
				boost::mutex::scoped_lock lock(m_oMutexFunc);

				if (!m_bOpened || eLogLevel < m_eLogLevel)
					return false;

#ifdef _WIN32
				printf(sMsg.c_str());
				OutputDebugString(sMsg.c_str());
#else
				printf(sMsg.c_str());
#endif // _WIN32

				if (!m_bLog2File)
					return true;

				if (m_fp == NULL)
					return false;

				size_t uWritten = fwrite(sMsg.c_str(), 1, sMsg.size(), m_fp);
				bool bResult = !ferror(m_fp);
				m_uBytesWritten += uWritten;

				if (m_uBytesWritten >= m_uMaxFileSize)
					StartNewLogFile();
				else
					fflush(m_fp);

				return bResult;
			}

			/**
			* 设置日志大小限制
			* @param lMByte 日志最大大小，单位MByte
			*/
			void SetLogLimitSize(unsigned int lMByte)
			{
				boost::mutex::scoped_lock lock(m_oMutexFunc);

				SetMaxFileSize(lMByte * 1024 * 1024);
			}

		protected:
			bool Open()
			{
				if (m_bOpened)
					return true;

				if (!m_bLog2File)
				{
					m_bOpened = true;
					return true;
				}

				std::ostringstream os;
#ifdef _WIN32
				os << m_sLogDir << "\\" << m_sFileName;
#else
				os << m_sLogDir << "/" << m_sFileName;
#endif // _WIN32
				m_fp = fopen(os.str().c_str(), "a+t");
				if (m_fp != NULL)
				{
					m_tStarted = time(NULL);
					fseek(m_fp, 0, SEEK_END);
					m_uBytesWritten = ftell (m_fp);
				}
				return m_bOpened = (m_fp != NULL);
			}

			bool Close()
			{
				if (!m_bOpened)
					return true;

				bool bResult = true;
				if (m_bLog2File)
				{
					bResult = (fclose(m_fp) == 0);
				}
				m_fp = NULL;
				m_tStarted = 0;
				m_uBytesWritten = 0;
				m_bOpened = false;
				return bResult;
			}

			void StartNewLogFile()
			{
				if (!m_bLog2File)
					return;

				time_t tStarted = m_tStarted;
				Close();

				char szDateLogStarted[20];
				strftime(szDateLogStarted, ARRSIZE(szDateLogStarted), "%Y.%m.%d %H.%M.%S", localtime(&tStarted));

				std::ostringstream oos,nos;
				std::string newFileName;
				size_t offset = m_sFileName.rfind(".");
#ifdef _WIN32
				oos<< m_sLogDir << "\\" << m_sFileName;
				nos<< m_sLogDir << "\\" << m_sFileName.substr(0, offset) 
					<< " - " << szDateLogStarted << m_sFileName.substr(offset, m_sFileName.size());
#else
				oos<< m_sLogDir << "/" << m_sFileName;
				nos<< m_sLogDir << "/" << m_sFileName.substr(0, offset) 
					<< " - " << szDateLogStarted << m_sFileName.substr(offset, m_sFileName.size());
#endif // _WIN32
				if (rename(oos.str().c_str(), nos.str().c_str()) != 0)
					remove(oos.str().c_str());

				Open();
			}

			void SetMaxFileSize(unsigned int uMaxFileSize)
			{
				if (uMaxFileSize == 0)
					uMaxFileSize = (unsigned int)-1;
				else if (uMaxFileSize < 0x10000)
					uMaxFileSize = 0x10000;
				m_uMaxFileSize = uMaxFileSize;
			}

		protected:
			FILE* m_fp;
			bool m_bOpened;
			bool m_bLog2File;
			LogLevel m_eLogLevel;		// log level limit
			time_t m_tStarted;
			std::string m_sLogDir;
			std::string m_sFileName;
			unsigned int m_uBytesWritten;
			unsigned int m_uMaxFileSize;
			boost::mutex m_oMutexFunc;
		};

		extern CLogger theLogger;
		//////////////////////////////////////////////////////////////////////////

		static void MakeMessage(std::string& str, unsigned int uLevel,
			const char * sModuleName, const char * sFuncName, long lLine,
			const char* fmt, const va_list& ap)
		{
			time_t rawtime;
			time ( &rawtime );
			char buffer[20];
			strftime(buffer, ARRSIZE(buffer), "%Y-%m-%d %H:%M:%S", localtime ( &rawtime ));

			std::ostringstream os;
			os << buffer << " ";
			switch (uLevel)
			{
			case eLog_ERROR:
				os << "ERROR\t";
				break;
			case eLog_WARNING:
				os << "WARNING\t";
				break;
			case eLog_INFO:
				os << "INFO\t";
				break;
			case eLog_DEBUG:
				os << "DEBUG\t";
				break;
			}
			os << " - " << sModuleName << "(" << lLine << ") [" << sFuncName << "]:\t" << fmt << "\n";

#define MAX_MESSAGE_LENGTH 1060 
			char message[MAX_MESSAGE_LENGTH];

			vsnprintf(message, MAX_MESSAGE_LENGTH, os.str().c_str(), ap);

#undef MAX_MESSAGE_LENGTH

			str = message;
		}

		static void LogV(LogLevel uLevel, const char * sModuleName, long lLine,
			const char * sFuncName, const char * pszFmt, ...)
		{
			assert(pszFmt != NULL);
			va_list args;
			va_start(args, pszFmt);

			std::string message;
			MakeMessage(message, uLevel, sModuleName, sFuncName, lLine, pszFmt, args);

			va_end(args);

			theLogger.Log(uLevel, message);
		}

		//////////////////////////////////////////////////////////////////////////

#define LogDebug(fmt, ...) { LogV(eLog_DEBUG, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__); }
#define LogInfo(fmt, ...) { LogV(eLog_INFO, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__); }
#define LogWarning(fmt, ...) { LogV(eLog_WARNING, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__); }
#define LogError(fmt, ...) { LogV(eLog_ERROR, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__); }

		//////////////////////////////////////////////////////////////////////////
	}
}

//////////////////////////////////////////////////////////////////////////

#if defined(_WIN32) && defined(_MSC_VER) && (_MSC_VER <= 1200)
#pragma inline_depth()
#endif

//////////////////////////////////////////////////////////////////////////

#endif // __SKYVIS_INCLUDE_LOGGER_H__