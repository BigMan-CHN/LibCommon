/**
* Server.h	-	����һ��������ģ����õķ������ͷ�ļ���
*/
#ifndef __INCLUDE_SKYVIS_ISERVER_H__
#define __INCLUDE_SKYVIS_ISERVER_H__

#include "Service.h"

namespace SVS
{
	/**
	* ����ģ����ӿ�
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
		* ע�ᴦ��ģ��
		* @param pModule �첽�������Ĵ������
		* @result ע��ɹ�����true�����򷵻�false
		*/
		bool Register(IModule * pModule);

		/**
		* ע�����д���ģ�飬��server�ر�ǰʹ��
		* @param e �첽�������Ĵ������
		* @param lSizeTransferred  �첽���������յ��ֽ���
		*/
		void UnRegisterAll();

		/**
		* �������������
		* @param nPort �����˿�
		* @param sAddress ������ַ��Ĭ�ϱ������е�ַ
		* @result �ɹ�����true�����򷵻�false
		*/
        bool Start(const int& nPort = SERVICE_DEFAULT_PORT, const std::string& sAddress = "0.0.0.0");  //mickeybear 2016.02.26 ADD 0.0.0.0

		/**
		* ���з���Start�ɹ�����Ҫ���д˺���ִ�У�Ȼ��������ѭ���ȴ�״̬
		*/
		void Run();

		/**
		* ֹͣ���������
		*/
		virtual void Stop();

		///ResponseCode p_code; //����ֵ��

	protected:
		/**
		* �������ô˷������������õ����в������ݸ����������
		* @param sModule XML�������ȵ�ģ������
		* @param sMethod XML�������ȵĺ�������
		* @param oParams ������������������ΪMAP���Զ������
		* @param oResult �����������ý��������ΪMAP���Զ������
		* @result ���صĵ��ý�����ɹ�����eResponseCodeSuccess�����򷵻ش������
		*/
		virtual SVS::ResponseCode Call(const SVS::SVSSTRING& sModule, const SVS::SVSSTRING& sMethod,
			SVS::CData& oParams, SVS::CData& oResult);
		// implement ICommandHandler

	protected:
		/**
		* ��Ž��յĶ��������ݵĻ���
		*/
		MODULES	m_oModules;

		/**
		* ������������
		*/
		CService m_oService;

		/**
		* �������б�ʶ
		*/ 
		bool m_bRunning;
	};

} // namespace SVS

#endif // __INCLUDE_SKYVIS_ISERVER_H__
