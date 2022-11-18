/**
* Server.h	-	这是一个处理子模块调用的服务类的头文件。
*/
#ifndef __INCLUDE_SKYVIS_ISERVER_H__
#define __INCLUDE_SKYVIS_ISERVER_H__

#include "Service.h"

namespace SVS
{
	/**
	* 处理模块类接口
	*/
	class IModule
	{
	public:
		virtual ~IModule() {}
		virtual const std::string GetName() const = 0;
		virtual SVS::ResponseCode Execute(const SVS::SVSSTRING& sMethod, SVS::CData& oParams, SVS::CData& oResult) = 0;
	};

	typedef std::map<std::string, IModule * >	MODULES;

	class CServer:
		public SVS::ICommandHandler
	{
	public:
        CServer(SVSLONG16 lThreadPoolSize = 5, SVSSTRING sFileDir = "");
		~CServer(void);

		/**
		* 注册处理模块
		* @param pModule 异步读操作的错误变量
		* @result 注册成功返回true，否则返回false
		*/
		bool Register(IModule * pModule);

		/**
		* 注销所有处理模块，在server关闭前使用
		* @param e 异步读操作的错误变量
		* @param lSizeTransferred  异步读操作接收的字节数
		*/
		void UnRegisterAll();

		/**
		* 启动处理服务器
		* @param nPort 监听端口
		* @param sAddress 监听地址，默认本机所有地址
		* @result 成功返回true，否则返回false
		*/
        bool Start(const int& nPort = SERVICE_DEFAULT_PORT, const std::string& sAddress = "0.0.0.0");  //mickeybear 2016.02.26 ADD 0.0.0.0

		/**
		* 运行服务，Start成功后需要运行此函数执行，然后服务进入循环等待状态
		*/
		void Run();

		/**
		* 停止处理服务器
		*/
		virtual void Stop();

		///ResponseCode p_code; //返回值；

	protected:
		/**
		* 服务会调用此方法将函数调用的所有参数传递给处理类对象
		* @param sModule XML函数调度的模块名称
		* @param sMethod XML函数调度的函数名称
		* @param oParams 包含函数参数的类型为MAP的自定义对象
		* @param oResult 包含函数调用结果的类型为MAP的自定义对象
		* @result 返回的调用结果，成功返回eResponseCodeSuccess，否则返回错误代码
		*/
		virtual SVS::ResponseCode Call(const SVS::SVSSTRING& sModule, const SVS::SVSSTRING& sMethod,
			SVS::CData& oParams, SVS::CData& oResult);
		// implement ICommandHandler

	protected:
		/**
		* 存放接收的二进制数据的缓冲
		*/
		MODULES	m_oModules;

		/**
		* 传输服务类对象
		*/
		CService m_oService;

		/**
		* 正在运行标识
		*/ 
		bool m_bRunning;
	};

} // namespace SVS

#endif // __INCLUDE_SKYVIS_ISERVER_H__
