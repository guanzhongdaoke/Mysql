#pragma once


#include <string>
#include <Include/Logger/LogFile.h>
#include <Include/Logger/LogManager.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>


// MYSQL Statement操作处理操作(简单文本模式)
class MYSQLSTMTHelper
{
protected:
	bool m_bValid = false;
protected:
	bool m_bNextResultSet = false;
	sql::Statement* m_pMYSQLStatement = nullptr;

public:
	MYSQLSTMTHelper(sql::Statement* pMYSQLStatement);
	~MYSQLSTMTHelper();

public:
	// 执行SQL处理
	void DoSQL(const char* sql);
	// 主动清空，一般情况下不需要，因为析构函数就会自动清空
	// 如果连续两次以上执行MYSQLHelper，就需要手动Clear
	void Clear();

public:
	// 获取一个结果集
	// 如果有多个select返回语句，则需要调用多次
	std::unique_ptr<sql::ResultSet> GetResultSet();
};

// MYSQL PreparedStatement操作处理操作(预处理二进制模式)
class MYSQLPreparedSTMTHelper
{
protected:
	bool m_bUpdateOpt = false;// 是否为更新操作
protected:
	int m_nCursor = 1;// 参数游标
	bool m_bNextResultSet = false;
	std::unique_ptr<sql::PreparedStatement> m_pMYSQLPreStatement = nullptr;

public:
	MYSQLPreparedSTMTHelper(sql::Connection* pConnection, const char* sql);
	~MYSQLPreparedSTMTHelper();

public:
	// 设置参数
	// 小心使用，设置Blob比较诡异，必须确保在DB操作完成之前value不会被释放
	void SetBlob(std::istream* value);
	// 设置参数
	void SetBoolean(bool value);
	// 设置参数
	void SetDouble(double value);
	// 设置参数
	void SetInt(int32_t value);
	// 设置参数
	void SetUInt(uint32_t value);
	// 设置参数
	void SetInt64(int64_t value);
	// 设置参数
	void SetNull(int sqlType);
	// 设置参数
	void SetString(const sql::SQLString& value);

public:
	// 执行SQL处理
	void DoSQL();
	// 主动清空，一般情况下不需要，因为析构函数就会自动清空
	// 如果连续两次以上执行MYSQLHelper，就需要手动Clear
	void Clear();

public:
	// 获取一个结果集
	// 如果有多个select返回语句，则需要调用多次
	std::unique_ptr<sql::ResultSet> GetResultSet();
};

// MYSQL PreparedStatement操作处理操作(预处理二进制模式)
// 理论上，可以自己提前缓存PreparedStatement，然后多次使用
// 这样子就不用每次创建了
class MYSQLCachePreparedSTMTHelper
{
protected:
	bool m_bUpdateOpt = false;// 是否为更新操作
protected:
	int m_nCursor = 1;// 参数游标
	bool m_bNextResultSet = false;
	sql::PreparedStatement* m_pMYSQLPreStatement = nullptr;

public:
	MYSQLCachePreparedSTMTHelper(sql::PreparedStatement* pMYSQLPreStatement);
	~MYSQLCachePreparedSTMTHelper();

public:
	// 设置参数
	// 小心使用，设置Blob比较诡异，必须确保在DB操作完成之前value不会被释放
	void SetBlob(std::istream* value);
	// 设置参数
	void SetBoolean(bool value);
	// 设置参数
	void SetDouble(double value);
	// 设置参数
	void SetInt(int32_t value);
	// 设置参数
	void SetUInt(uint32_t value);
	// 设置参数
	void SetInt64(int64_t value);
	// 设置参数
	void SetNull(int sqlType);
	// 设置参数
	void SetString(const sql::SQLString& value);

public:
	// 执行SQL处理
	void DoSQL();
	// 主动清空，一般情况下不需要，因为析构函数就会自动清空
	// 如果连续两次以上执行MYSQLHelper，就需要手动Clear
	void Clear();

public:
	// 获取一个结果集
	// 如果有多个select返回语句，则需要调用多次
	std::unique_ptr<sql::ResultSet> GetResultSet();
};

// MYSQL Statement返回值处理操作
class MYSQLSTMTResultHelper
{
protected:
	int m_nCursorIndex = 1;
	sql::ResultSet* m_pMYSQLResultSet = nullptr;

public:
	MYSQLSTMTResultHelper(sql::ResultSet* pResultSet);
	~MYSQLSTMTResultHelper();

public:
	// 获取Blob字段
	void GetBlob(sql::SQLString& value);
	// 获取Blob字段
	void GetBlob(char* pBuffer, int nBufferLen);
	// 获取参数
	void GetBoolean(bool& value);
	// 获取参数
	void GetDouble(double& value);
	// 获取参数
	void GetInt(int32_t& value);
	// 获取参数
	void GetUInt(uint32_t& value);
	// 获取参数
	void GetInt64(int64_t& value);
	// 获取参数
	void GetString(std::string& value);
	// 跳过
	void Next();

public:
	// 获取Blob字段
	void GetBlob(int nCursorIndex, sql::SQLString& value);
	// 获取Blob字段
	void GetBlob(int nCursorIndex, char* pBuffer, int nBufferLen);
	// 获取参数
	void GetBoolean(int nCursorIndex, bool& value);
	// 获取参数
	void GetDouble(int nCursorIndex, double& value);
	// 获取参数
	void GetInt(int nCursorIndex, int32_t& value);
	// 获取参数
	void GetUInt(int nCursorIndex, uint32_t& value);
	// 获取参数
	void GetInt64(int nCursorIndex, int64_t& value);
	// 获取参数
	void GetString(int nCursorIndex, std::string& value);
};
