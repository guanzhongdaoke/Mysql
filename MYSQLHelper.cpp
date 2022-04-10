#include <Include/DB/MYSQLHelper.h>
#include <Include/DB/DBDefine.h>
#include <Include/Common/Utils.h>
#include <Include/ThreadContext/ThreadContext.h>
#include <Include/ThreadContext/ThreadContextManager.h>
#include <memory.h>
#include <iostream>
#include <string>



MYSQLSTMTHelper::MYSQLSTMTHelper(sql::Statement* pMYSQLStatement)
{
	m_bValid = false;
	m_pMYSQLStatement = pMYSQLStatement;
}

MYSQLSTMTHelper::~MYSQLSTMTHelper()
{
	Clear();
	m_pMYSQLStatement = nullptr;
	m_bValid = false;
}

void MYSQLSTMTHelper::DoSQL(const char* sql)
{
	if (sql == nullptr)
	{
		ErrorEx("MYSQLSTMTHelper::DoSQL NULL");
	}

	sql::SQLString val = sql;

	// 保存在当前线程上下文
	// 一般有异常方便获取
	if (strlen(sql) >= 512)
	{// 安全操作，只接取前一段数值，防止把日志写爆了
		ThreadContext::Get()->m_LastestDBSQLString = val.substr(0, 512);
		ThreadContext::Get()->m_LastestDBSQLString += "...";
	}
	else
	{
		ThreadContext::Get()->m_LastestDBSQLString = sql;
	}
	m_pMYSQLStatement->execute(val);
	m_bNextResultSet = false;
	m_bValid = true;
}

void MYSQLSTMTHelper::Clear()
{
	if (m_pMYSQLStatement == nullptr)
		return;
	if (!m_bValid)
		return;

	if (!m_bNextResultSet)
	{
		m_bNextResultSet = true;
		sql::ResultSet* pTmp = m_pMYSQLStatement->getResultSet();
		if (pTmp)
			delete pTmp;
	}

	while (m_pMYSQLStatement->getMoreResults())
	{
		//std::unique_ptr<sql::ResultSet> pMYSQLResultSet(m_pMYSQLPreStatement->getResultSet());
		//while (pMYSQLResultSet->next())
		//{
		//	;
		//}
		;
	}

	m_bNextResultSet = false;
	m_bValid = false;
}

std::unique_ptr<sql::ResultSet> MYSQLSTMTHelper::GetResultSet()
{
	if (!m_bValid)
		return nullptr;

	if (!m_bNextResultSet)
	{
		m_bNextResultSet = true;
		std::unique_ptr<sql::ResultSet> pMYSQLResultSet(m_pMYSQLStatement->getResultSet());
		return pMYSQLResultSet;
	}

	if (!m_pMYSQLStatement->getMoreResults())
		return nullptr;

	std::unique_ptr<sql::ResultSet> pMYSQLResultSet(m_pMYSQLStatement->getResultSet());
	return pMYSQLResultSet;
}


MYSQLPreparedSTMTHelper::MYSQLPreparedSTMTHelper(sql::Connection* pConnection, const char* sqlName)
{
	m_nCursor = 1;

	// 保存在当前线程上下文
	// 一般有异常方便获取
	ThreadContext::Get()->m_LastestDBSQLString = sqlName;
	if (strlen(sqlName) >= 512)
	{// 安全操作，只接取前一段数值，防止把日志写爆了
		ThreadContext::Get()->m_LastestDBSQLString = ThreadContext::Get()->m_LastestDBSQLString.substr(0, 512);
		ThreadContext::Get()->m_LastestDBSQLString += "...";
	}

	sql::SQLString val = sqlName;
	m_pMYSQLPreStatement = std::unique_ptr<sql::PreparedStatement>(pConnection->prepareStatement(val));
}

MYSQLPreparedSTMTHelper::~MYSQLPreparedSTMTHelper()
{
	Clear();
}

void MYSQLPreparedSTMTHelper::SetBlob(std::istream* value)
{
	m_pMYSQLPreStatement->setBlob(m_nCursor++, value);
}

void MYSQLPreparedSTMTHelper::SetBoolean(bool value)
{
	m_pMYSQLPreStatement->setBoolean(m_nCursor++, value);
}

void MYSQLPreparedSTMTHelper::SetDouble(double value)
{
	m_pMYSQLPreStatement->setDouble(m_nCursor++, value);
}

void MYSQLPreparedSTMTHelper::SetInt(int32_t value)
{
	m_pMYSQLPreStatement->setInt(m_nCursor++, value);
}

void MYSQLPreparedSTMTHelper::SetUInt(uint32_t value)
{
	m_pMYSQLPreStatement->setUInt(m_nCursor++, value);
}

void MYSQLPreparedSTMTHelper::SetInt64(int64_t value)
{
	m_pMYSQLPreStatement->setInt64(m_nCursor++, value);
}

void MYSQLPreparedSTMTHelper::SetNull(int sqlType)
{
	m_pMYSQLPreStatement->setNull(m_nCursor++, sqlType);
}

void MYSQLPreparedSTMTHelper::SetString(const sql::SQLString& value)
{
	m_pMYSQLPreStatement->setString(m_nCursor++, value);
}

void MYSQLPreparedSTMTHelper::DoSQL()
{
	m_bUpdateOpt = m_pMYSQLPreStatement->execute();
	m_bNextResultSet = false;
}

void MYSQLPreparedSTMTHelper::Clear()
{
	m_nCursor = 1;
	if (m_pMYSQLPreStatement == nullptr)
		return;

	if (!m_bNextResultSet)
	{
		m_bNextResultSet = true;
		sql::ResultSet* pTmp = m_pMYSQLPreStatement->getResultSet();
		if (pTmp)
			delete pTmp;
	}

	while (m_pMYSQLPreStatement->getMoreResults())
	{
		//std::unique_ptr<sql::ResultSet> pMYSQLResultSet(m_pMYSQLPreStatement->getResultSet());
		//while (pMYSQLResultSet->next())
		//{
		//	;
		//}
		;
	}
	m_pMYSQLPreStatement = nullptr;
	m_bNextResultSet = false;
}

std::unique_ptr<sql::ResultSet> MYSQLPreparedSTMTHelper::GetResultSet()
{
	if (!m_bUpdateOpt)
		return nullptr;
	if (m_pMYSQLPreStatement == nullptr)
		return nullptr;

	if (!m_bNextResultSet)
	{
		m_bNextResultSet = true;
		std::unique_ptr<sql::ResultSet> pMYSQLResultSet(m_pMYSQLPreStatement->getResultSet());
		return pMYSQLResultSet;
	}

	if (!m_pMYSQLPreStatement->getMoreResults())
		return nullptr;

	std::unique_ptr<sql::ResultSet> pMYSQLResultSet(m_pMYSQLPreStatement->getResultSet());
	return pMYSQLResultSet;
}

MYSQLCachePreparedSTMTHelper::MYSQLCachePreparedSTMTHelper(sql::PreparedStatement* pMYSQLPreStatement)
{
	m_nCursor = 1;

	m_pMYSQLPreStatement = pMYSQLPreStatement;
}

MYSQLCachePreparedSTMTHelper::~MYSQLCachePreparedSTMTHelper()
{
	Clear();
}

void MYSQLCachePreparedSTMTHelper::SetBlob(std::istream* value)
{
	m_pMYSQLPreStatement->setBlob(m_nCursor++, value);
}

void MYSQLCachePreparedSTMTHelper::SetBoolean(bool value)
{
	m_pMYSQLPreStatement->setBoolean(m_nCursor++, value);
}

void MYSQLCachePreparedSTMTHelper::SetDouble(double value)
{
	m_pMYSQLPreStatement->setDouble(m_nCursor++, value);
}

void MYSQLCachePreparedSTMTHelper::SetInt(int32_t value)
{
	m_pMYSQLPreStatement->setInt(m_nCursor++, value);
}

void MYSQLCachePreparedSTMTHelper::SetUInt(uint32_t value)
{
	m_pMYSQLPreStatement->setUInt(m_nCursor++, value);
}

void MYSQLCachePreparedSTMTHelper::SetInt64(int64_t value)
{
	m_pMYSQLPreStatement->setInt64(m_nCursor++, value);
}

void MYSQLCachePreparedSTMTHelper::SetNull(int sqlType)
{
	m_pMYSQLPreStatement->setNull(m_nCursor++, sqlType);
}

void MYSQLCachePreparedSTMTHelper::SetString(const sql::SQLString& value)
{
	m_pMYSQLPreStatement->setString(m_nCursor++, value);
}

void MYSQLCachePreparedSTMTHelper::DoSQL()
{
	m_bUpdateOpt = m_pMYSQLPreStatement->execute();
	m_bNextResultSet = false;
}

void MYSQLCachePreparedSTMTHelper::Clear()
{
	m_nCursor = 1;
	if (m_pMYSQLPreStatement == nullptr)
		return;

	if (!m_bNextResultSet)
	{
		m_bNextResultSet = true;
		sql::ResultSet* pTmp = m_pMYSQLPreStatement->getResultSet();
		if (pTmp)
			delete pTmp;
	}

	while (m_pMYSQLPreStatement->getMoreResults())
	{
		//std::unique_ptr<sql::ResultSet> pMYSQLResultSet(m_pMYSQLPreStatement->getResultSet());
		//while (pMYSQLResultSet->next())
		//{
		//	;
		//}
		;
	}
	//m_pMYSQLPreStatement = nullptr;
	m_bNextResultSet = false;
}

std::unique_ptr<sql::ResultSet> MYSQLCachePreparedSTMTHelper::GetResultSet()
{
	if (!m_bUpdateOpt)
		return nullptr;
	if (m_pMYSQLPreStatement == nullptr)
		return nullptr;

	if (!m_bNextResultSet)
	{
		m_bNextResultSet = true;
		std::unique_ptr<sql::ResultSet> pMYSQLResultSet(m_pMYSQLPreStatement->getResultSet());
		return pMYSQLResultSet;
	}

	if (!m_pMYSQLPreStatement->getMoreResults())
		return nullptr;

	std::unique_ptr<sql::ResultSet> pMYSQLResultSet(m_pMYSQLPreStatement->getResultSet());
	return pMYSQLResultSet;
}



MYSQLSTMTResultHelper::MYSQLSTMTResultHelper(sql::ResultSet* pResultSet)
{
	m_nCursorIndex = 1;
	m_pMYSQLResultSet = pResultSet;
}

MYSQLSTMTResultHelper::~MYSQLSTMTResultHelper()
{
	m_pMYSQLResultSet = nullptr;
}

void MYSQLSTMTResultHelper::GetBlob(sql::SQLString& value)
{
	value = m_pMYSQLResultSet->getString(m_nCursorIndex);
	m_nCursorIndex++;
}

void MYSQLSTMTResultHelper::GetBlob(char* pBuffer, int nBufferLen)
{
	sql::SQLString tempString = m_pMYSQLResultSet->getString(m_nCursorIndex);
	m_nCursorIndex++;
	
	std::size_t len = tempString->size();
	if (len == 0)
		return;

	if (len >= nBufferLen)
	{
		memcpy(pBuffer, tempString->c_str(), nBufferLen);
	}
	else
	{
		memcpy(pBuffer, tempString->c_str(), len);
	}
}

void MYSQLSTMTResultHelper::GetBoolean(bool& value)
{
	value = m_pMYSQLResultSet->getInt(m_nCursorIndex)==1;
	m_nCursorIndex++;
}

void MYSQLSTMTResultHelper::GetDouble(double& value)
{
	value = m_pMYSQLResultSet->getDouble(m_nCursorIndex);
	m_nCursorIndex++;
}

void MYSQLSTMTResultHelper::GetInt(int32_t& value)
{
	value = m_pMYSQLResultSet->getInt(m_nCursorIndex);
	m_nCursorIndex++;
}

void MYSQLSTMTResultHelper::GetUInt(uint32_t& value)
{
	value = m_pMYSQLResultSet->getUInt(m_nCursorIndex);
	m_nCursorIndex++;
}

void MYSQLSTMTResultHelper::GetInt64(int64_t& value)
{
	value = m_pMYSQLResultSet->getInt64(m_nCursorIndex);
	m_nCursorIndex++;
}

void MYSQLSTMTResultHelper::GetString(std::string& value)
{
	value = m_pMYSQLResultSet->getString(m_nCursorIndex);
	m_nCursorIndex++;
}

void MYSQLSTMTResultHelper::Next()
{
	m_nCursorIndex++;
}

void MYSQLSTMTResultHelper::GetBlob(int nCursorIndex, sql::SQLString& value)
{
	value = m_pMYSQLResultSet->getString(nCursorIndex);
}

void MYSQLSTMTResultHelper::GetBlob(int nCursorIndex, char* pBuffer, int nBufferLen)
{
	sql::SQLString tempString = m_pMYSQLResultSet->getString(nCursorIndex);
	std::size_t len = tempString->size();
	if (len == 0)
		return;

	if (len >= nBufferLen)
	{
		memcpy(pBuffer, tempString->c_str(), nBufferLen);
	}
	else
	{
		memcpy(pBuffer, tempString->c_str(), len);
	}
}

void MYSQLSTMTResultHelper::GetBoolean(int nCursorIndex, bool& value)
{
	value = m_pMYSQLResultSet->getInt(nCursorIndex) == 1;
}

void MYSQLSTMTResultHelper::GetDouble(int nCursorIndex, double& value)
{
	value = m_pMYSQLResultSet->getDouble(nCursorIndex);
}

void MYSQLSTMTResultHelper::GetInt(int nCursorIndex, int32_t& value)
{
	value = m_pMYSQLResultSet->getInt(nCursorIndex);
}

void MYSQLSTMTResultHelper::GetUInt(int nCursorIndex, uint32_t& value)
{
	value = m_pMYSQLResultSet->getUInt(nCursorIndex);
}

void MYSQLSTMTResultHelper::GetInt64(int nCursorIndex, int64_t& value)
{
	value = m_pMYSQLResultSet->getInt64(nCursorIndex);
}

void MYSQLSTMTResultHelper::GetString(int nCursorIndex, std::string& value)
{
	value = m_pMYSQLResultSet->getString(nCursorIndex);
}

