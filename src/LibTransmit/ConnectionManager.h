/**
* ConnectionManager.h	-	����һ�����ӹ������ͷ�ļ���
*/
#ifndef __INCLUDE_SKYVIS_CONNECTION_MANAGER_H__
#define __INCLUDE_SKYVIS_CONNECTION_MANAGER_H__

#include <set>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include "Connection.h"

namespace SVS
{

/**
* �������򿪵��������ӵ���
*/
class CConnectionManager
  : private boost::noncopyable
{
public:
	/**
	* �����µ����Ӳ�����
	* @param �µ����ӵ�ָ��
	*/
	void Start(CConnectionPtr pConn);

	/**
	* ֹͣ���������
	* @param ��Ҫֹͣ�����ӵ�ָ��
	*/
	void Stop(CConnectionPtr pConn);

	/**
	* ֹͣ���е�����
	*/
	void StopAll();

private:
	std::set<CConnectionPtr> m_oConns;	// ��������Ӽ���
	boost::mutex m_mutexFunc;
};

} // namespace SVS

#endif // __INCLUDE_SKYVIS_CONNECTION_MANAGER_H__
