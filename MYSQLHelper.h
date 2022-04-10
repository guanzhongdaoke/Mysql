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


// MYSQL Statement�����������(���ı�ģʽ)
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
	// ִ��SQL����
	void DoSQL(const char* sql);
	// ������գ�һ������²���Ҫ����Ϊ���������ͻ��Զ����
	// ���������������ִ��MYSQLHelper������Ҫ�ֶ�Clear
	void Clear();

public:
	// ��ȡһ�������
	// ����ж��select������䣬����Ҫ���ö��
	std::unique_ptr<sql::ResultSet> GetResultSet();
};

// MYSQL PreparedStatement�����������(Ԥ���������ģʽ)
class MYSQLPreparedSTMTHelper
{
protected:
	bool m_bUpdateOpt = false;// �Ƿ�Ϊ���²���
protected:
	int m_nCursor = 1;// �����α�
	bool m_bNextResultSet = false;
	std::unique_ptr<sql::PreparedStatement> m_pMYSQLPreStatement = nullptr;

public:
	MYSQLPreparedSTMTHelper(sql::Connection* pConnection, const char* sql);
	~MYSQLPreparedSTMTHelper();

public:
	// ���ò���
	// С��ʹ�ã�����Blob�ȽϹ��죬����ȷ����DB�������֮ǰvalue���ᱻ�ͷ�
	void SetBlob(std::istream* value);
	// ���ò���
	void SetBoolean(bool value);
	// ���ò���
	void SetDouble(double value);
	// ���ò���
	void SetInt(int32_t value);
	// ���ò���
	void SetUInt(uint32_t value);
	// ���ò���
	void SetInt64(int64_t value);
	// ���ò���
	void SetNull(int sqlType);
	// ���ò���
	void SetString(const sql::SQLString& value);

public:
	// ִ��SQL����
	void DoSQL();
	// ������գ�һ������²���Ҫ����Ϊ���������ͻ��Զ����
	// ���������������ִ��MYSQLHelper������Ҫ�ֶ�Clear
	void Clear();

public:
	// ��ȡһ�������
	// ����ж��select������䣬����Ҫ���ö��
	std::unique_ptr<sql::ResultSet> GetResultSet();
};

// MYSQL PreparedStatement�����������(Ԥ���������ģʽ)
// �����ϣ������Լ���ǰ����PreparedStatement��Ȼ����ʹ��
// �����ӾͲ���ÿ�δ�����
class MYSQLCachePreparedSTMTHelper
{
protected:
	bool m_bUpdateOpt = false;// �Ƿ�Ϊ���²���
protected:
	int m_nCursor = 1;// �����α�
	bool m_bNextResultSet = false;
	sql::PreparedStatement* m_pMYSQLPreStatement = nullptr;

public:
	MYSQLCachePreparedSTMTHelper(sql::PreparedStatement* pMYSQLPreStatement);
	~MYSQLCachePreparedSTMTHelper();

public:
	// ���ò���
	// С��ʹ�ã�����Blob�ȽϹ��죬����ȷ����DB�������֮ǰvalue���ᱻ�ͷ�
	void SetBlob(std::istream* value);
	// ���ò���
	void SetBoolean(bool value);
	// ���ò���
	void SetDouble(double value);
	// ���ò���
	void SetInt(int32_t value);
	// ���ò���
	void SetUInt(uint32_t value);
	// ���ò���
	void SetInt64(int64_t value);
	// ���ò���
	void SetNull(int sqlType);
	// ���ò���
	void SetString(const sql::SQLString& value);

public:
	// ִ��SQL����
	void DoSQL();
	// ������գ�һ������²���Ҫ����Ϊ���������ͻ��Զ����
	// ���������������ִ��MYSQLHelper������Ҫ�ֶ�Clear
	void Clear();

public:
	// ��ȡһ�������
	// ����ж��select������䣬����Ҫ���ö��
	std::unique_ptr<sql::ResultSet> GetResultSet();
};

// MYSQL Statement����ֵ�������
class MYSQLSTMTResultHelper
{
protected:
	int m_nCursorIndex = 1;
	sql::ResultSet* m_pMYSQLResultSet = nullptr;

public:
	MYSQLSTMTResultHelper(sql::ResultSet* pResultSet);
	~MYSQLSTMTResultHelper();

public:
	// ��ȡBlob�ֶ�
	void GetBlob(sql::SQLString& value);
	// ��ȡBlob�ֶ�
	void GetBlob(char* pBuffer, int nBufferLen);
	// ��ȡ����
	void GetBoolean(bool& value);
	// ��ȡ����
	void GetDouble(double& value);
	// ��ȡ����
	void GetInt(int32_t& value);
	// ��ȡ����
	void GetUInt(uint32_t& value);
	// ��ȡ����
	void GetInt64(int64_t& value);
	// ��ȡ����
	void GetString(std::string& value);
	// ����
	void Next();

public:
	// ��ȡBlob�ֶ�
	void GetBlob(int nCursorIndex, sql::SQLString& value);
	// ��ȡBlob�ֶ�
	void GetBlob(int nCursorIndex, char* pBuffer, int nBufferLen);
	// ��ȡ����
	void GetBoolean(int nCursorIndex, bool& value);
	// ��ȡ����
	void GetDouble(int nCursorIndex, double& value);
	// ��ȡ����
	void GetInt(int nCursorIndex, int32_t& value);
	// ��ȡ����
	void GetUInt(int nCursorIndex, uint32_t& value);
	// ��ȡ����
	void GetInt64(int nCursorIndex, int64_t& value);
	// ��ȡ����
	void GetString(int nCursorIndex, std::string& value);
};
