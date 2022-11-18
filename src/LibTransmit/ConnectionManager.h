/**
* ConnectionManager.h	-	这是一个连接管理类的头文件。
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
* 管理服务打开的所有连接的类
*/
class CConnectionManager
  : private boost::noncopyable
{
public:
	/**
	* 增加新的连接并启动
	* @param 新的连接的指针
	*/
	void Start(CConnectionPtr pConn);

	/**
	* 停止传入的连接
	* @param 需要停止的连接的指针
	*/
	void Stop(CConnectionPtr pConn);

	/**
	* 停止所有的连接
	*/
	void StopAll();

private:
	std::set<CConnectionPtr> m_oConns;	// 管理的连接集合
	boost::mutex m_mutexFunc;
};

} // namespace SVS

#endif // __INCLUDE_SKYVIS_CONNECTION_MANAGER_H__
