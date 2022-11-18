// test.cpp : Defines the entry point for the console application.
//

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/program_options.hpp>
#include "Logger.h"

using namespace SVS::log;
SVS::log::CLogger SVS::log::theLogger;

//////////////////////////////////////////////////////////////////////////

#include "LibXML/LibXML.h"

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "LibXMLD.lib")
#else
#pragma comment(lib, "LibXML.lib")
#endif // _DEBUG
#endif // _WIN32

//////////////////////////////////////////////////////////////////////////

#include "LibTransmit/Server.h"
#include "LibTransmit/TransmitClient.h"
#include "MControl.h"

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "LibTransmitD.lib")
#else
#pragma comment(lib, "LibTransmit.lib")
#endif // _DEBUG
#endif // _WIN32

//////////////////////////////////////////////////////////////////////////


using namespace std;
using namespace SVS;

#define PORT 5333//SERVICE_DEFAULT_PORT

// 测试LibXML库

void testSaveFile(SVS::SVSSTRING sFile)
{
	CData oRoot = SVSMAP();
	SVSMAP listVariant;
	listVariant["boolVariant"] = SVSTRUE;
	listVariant["long8Variant"] = (SVSLONG8)8;
	listVariant["long16Variant"] = (SVSLONG16)16;
	listVariant["long32Variant"] = (SVSLONG32)32;
	listVariant["long64Variant"] = (SVSLONG64)64;
	listVariant["floatVariant"] = (SVSFLOAT)8.0;
	listVariant["doubleVariant"] = (SVSDOUBLE)16.0;
	listVariant["stringVariant"] = "string";

	SVSBINARY oBin;
	oBin.push_back('a');
	oBin.push_back('b');
	oBin.push_back('c');
	oBin.push_back('d');
	SVSMAP info;
	info["cpu"] = (SVSSTRING)"1.8g";
	info["memory"] = (SVSDOUBLE)2048;
	info["binary"] = oBin;

	SVSVECTOR arr;
	arr.push_back(info);
	arr.push_back(info);

	oRoot.RefMap()["listVariant"] = listVariant;
	oRoot.RefMap()["array"] = arr;

	CData::SaveFile(oRoot, sFile);
}

void testParseFile(SVS::SVSSTRING sFile)
{
	CData oRoot;
	if (SVSSUCCEEDED(CData::FromFile(oRoot, sFile)))
	{
		SVSMAP listVariant = oRoot.RefMap()["listVariant"].RefMap();
		printf("boolVariant:\t%s\n", SVSSUCCEEDED(listVariant["boolVariant"].RefBool()) ? "true" : "false");
		printf("long8Variant:\t%d\n", listVariant["long8Variant"].RefLong8());
		printf("long16Variant:\t%d\n", listVariant["long16Variant"].RefLong16());
		printf("long32Variant:\t%d\n", listVariant["long32Variant"].RefLong32());
		printf("long64Variant:\t%d\n", listVariant["long64Variant"].RefLong64());
		printf("floatVariant:\t%f\n", listVariant["floatVariant"].RefFloat());
		printf("doubleVariant:\t%f\n", listVariant["doubleVariant"].RefDouble());
		printf("stringVariant:\t%s\n", listVariant["stringVariant"].RefString().c_str());

		SVSVECTOR arr = oRoot.RefMap()["array"].RefVector();
		printf("size of array: %d\n", arr.size());

		printf("cpu:\t %s \n", arr[0].RefMap()["cpu"].RefString().c_str());
		printf("memory:\t %f \n", arr[0].RefMap()["memory"].RefDouble());

		SVSBINARY oBin = arr[0].RefMap()["binary"].RefBinary();
		printf("binary:\n", oBin.size());
		for (SVSLONG32 i = 0; i < oBin.size(); i++)
		{
			printf("%c", oBin[i]);
		}
		printf("\n");
	}
}

//////////////////////////////////////////////////////////////////////////
// 测试LibTransmit库

#ifdef _WIN32

boost::function0<void> console_ctrl_function;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		console_ctrl_function();
		return TRUE;
	default:
		return FALSE;
	}
}

void testService(std::size_t lThreadPoolSize)
{
	theLogger.Create(eLog_DEBUG);
	CServer oServer(lThreadPoolSize);
	oServer.Register(new CMControl);

	// Set console control handler to allow server to be stopped.
	console_ctrl_function = boost::bind(&CServer::Stop, &oServer);
	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

	if (oServer.Start(PORT))
	{
		printf("server start success.\n");
		oServer.Run();
	}
	oServer.UnRegisterAll();
}

#else

#include <pthread.h>
#include <signal.h>

void testService(std::size_t lThreadPoolSize)
{
	try
    {   printf("AAAAAAAAAAAAAAAAAAAAA");
		// Run server in background thread.
		CServer oServer(lThreadPoolSize);
		oServer.Register(new CMControl);
        printf("BBBBBBBBBBBBBBBBBBBBBB");
		if (oServer.Start(PORT))
        {   printf("CCCCCCCCCCCCCCCCCCC");
			// Block all signals for background thread.
			sigset_t new_mask;
			sigfillset(&new_mask);
			sigset_t old_mask;
			pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

			boost::thread t(boost::bind(&CServer::Run, &oServer));

			// Restore previous signals.
			pthread_sigmask(SIG_SETMASK, &old_mask, 0);

			// Wait for signal indicating time to shut down.
			sigset_t wait_mask;
			sigemptyset(&wait_mask);
			sigaddset(&wait_mask, SIGINT);
			sigaddset(&wait_mask, SIGQUIT);
			sigaddset(&wait_mask, SIGTERM);
			pthread_sigmask(SIG_BLOCK, &wait_mask, 0);
			int sig = 0;
			sigwait(&wait_mask, &sig);

			oServer.Stop();
			t.join();
		}
		// Stop the server.
		oServer.UnRegisterAll();
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}
}

#endif // _WIN32

void testClient(std::string sServiceIP)
{
	theLogger.Create("B:", "test.log", eLog_DEBUG);
	CTransmitClient client(10000);
	if (client.Connect(sServiceIP, PORT))
	{
		CData oParams = SVSMAP();
		oParams.RefMap()["sName"] = (SVSSTRING)"skyvis";
		oParams.RefMap()["sPass"] = (SVSSTRING)"123";

		CData oResult;
		ResponseCode eCode = client.Execute(MODULE_CONTROL, "Login", oParams, oResult);
		if (eResponseCodeSuccess == eCode)
		{
			SVSBOOL bLogin = oResult.RefMap()[SKYVIS_TRANSMIT_RESULT].RefBool();
			if (SVSSUCCEEDED(bLogin))
			{
				cout << "Login succ!" << endl;
			}
			else
			{
				cout << "Login failed! Unmatched user or pass." << endl;
			}
		}
		else
		{
			cout << "Login Execute faile! Error: " << GetResponseError(eCode) << endl;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void testConfig()
{
	CTransmitClient client;
	if (client.Connect("192.168.1.60", PORT))
	{
		CData oParams = SVSMAP();
		CData oResult;
		ResponseCode eCode = client.Execute("Config", "GetConfig", oParams, oResult);
		if (eResponseCodeSuccess == eCode)
		{
			CData::SaveFile(oResult, "B:\\config.xml");
		}
		else
		{
			cout << "GetConfig Execute faile! Error: " << GetResponseError(eCode) << endl;
		}
	}
}

namespace po = boost::program_options;
int main(int argc, char* argv[])
{
	try
	{
		int opt;
		string sServiceIP;
		po::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "produce help message")
			("Service,S", po::value<int>(&opt), 
				"test XML call service. - thread pool size")
			("Client,C", po::value<string>(&sServiceIP), 
				"test XML call client. - service ip")
			("ParseFile,p", "test parsing XML from file.")
			("SaveFile,s", "test saving xml to file.")
			("Config,c", "test mps config to file.")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    

		if (vm.count("help")) {  
			cout << desc << "\n";
			return 0;
		}

		if (vm.count("SaveFile"))
		{
			// 测试保存成文件
			testSaveFile("B:\\a.xml");
		}
		else if (vm.count("ParseFile"))
		{
			// 测试从文件中读取数据
			testParseFile("B:\\a.xml");
		}
		else if (vm.count("Service"))
		{
			testService(opt);
		}
		else if (vm.count("Client"))
		{
			testClient(sServiceIP);
		}
		else if (vm.count("Config"))
		{
			testConfig();
		}
		else
		{
			cout << desc << "\n";
		}
	}
	catch(exception& e) {
		cerr << e.what() << "\n";
	}
	catch(...) {
		cerr << "Exception of unknown type!\n";
	}

	return 0;
}

