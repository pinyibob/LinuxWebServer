#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>
#include "sql_connection_pool.h"

using namespace std;

connection_pool::connection_pool()
{
	m_CurConn = 0;
	m_FreeConn = 0;
}

connection_pool *connection_pool::GetInstance()
{
	static connection_pool connPool;
	return &connPool;
}

void connection_pool::init(string url, string User, string PassWord,
	 string DBName, int Port, int MaxConn, int close_log)
{
	m_url = url;
	m_Port = Port;
	m_User = User;
	m_PassWord = PassWord;
	m_DatabaseName = DBName;
	m_close_log = close_log;
	m_MaxConn = MaxConn;

	for (int i = 0; i < MaxConn; i++)
	{
		MYSQL *con = NULL;
		con = mysql_init(con);

		if (con == NULL)
		{
			LOG_ERROR("MySQL init Error");
			exit(1);
		}
		con = mysql_real_connect(con, m_url.c_str(), m_User.c_str(),
		 m_PassWord.c_str(), m_DatabaseName.c_str(), m_Port, NULL, 0);

		if (con == NULL)
		{
			LOG_ERROR("MySQL new conncection Error");
			exit(1);
		}
		connList.push_back(con);
		++m_FreeConn;
	}

	reserve = sem(m_FreeConn);
}

//当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
MYSQL *connection_pool::GetConnection()
{
	MYSQL *con = NULL;

	if (0 == connList.size())
		return NULL;

	reserve.wait();
	lock.lock();

	con = connList.front();
	connList.pop_front();

	//检查链接是否仍然有效
	if(mysql_ping(con) != 0)
	{
		mysql_close(con);
		auto imp = mysql_init(nullptr);
		con = mysql_real_connect(imp, m_url.c_str(), m_User.c_str(),
		 m_PassWord.c_str(), m_DatabaseName.c_str(), m_Port, NULL, 0);
	}

	--m_FreeConn;
	++m_CurConn;

	lock.unlock();

	return con;
}

//释放当前使用的连接
bool connection_pool::ReleaseConnection(MYSQL *con)
{
	if (NULL == con)
		return false;

	lock.lock();

	connList.push_back(con);
	++m_FreeConn;
	--m_CurConn;

	lock.unlock();
	reserve.post();

	return true;
}

//销毁数据库连接池
void connection_pool::DestroyPool()
{
	lock.lock();
	if (connList.size() > 0)
	{
		list<MYSQL *>::iterator it;
		for (it = connList.begin(); it != connList.end(); ++it)
		{
			mysql_close(*it);
		}
		m_CurConn = 0;
		m_FreeConn = 0;
		connList.clear();
	}

	lock.unlock();
}

//当前空闲的连接数
int connection_pool::GetFreeConn() const
{
	return this->m_FreeConn;
}

connection_pool::~connection_pool()
{
	//等到static 静态对象析构
	DestroyPool();
}

connectionRAII::connectionRAII(MYSQL **SQL, connection_pool *connPool){
	*SQL = connPool->GetConnection();
	
	conRAII = *SQL;
	poolRAII = connPool;
}

connectionRAII::~connectionRAII(){
	poolRAII->ReleaseConnection(conRAII);
}