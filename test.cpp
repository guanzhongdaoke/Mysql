void test()
{

	char sqltext[40960] = "";
	sprintf(sqltext, "select count(*) from `t_char` where `charname`='%s'", "test");
	MYSQLSTMTHelper mysqlHelper(m_pMYSQLStatement);
	mysqlHelper.DoSQL(sqltext);

	// 选获取返回结果
	std::unique_ptr<sql::ResultSet> pMYSQLResultSet = mysqlHelper.GetResultSet();
	while (pMYSQLResultSet && pMYSQLResultSet->next())
	{
		rRetMsg->mCount = pMYSQLResultSet->getInt(1);// 结果
	}

}

void test2()
{

	// 可以是二进制，也可以是字符串
	std::string pbstructstring = "";

	MYSQLPreparedSTMTHelper mysqlHelper(m_pMYSQLConnection, SQL::SAVECHAR3);
	BlobType commonData((char*)pbstructstring.c_str(), pbstructstring.size());
	mysqlHelper.SetBlob(&commonData);
	mysqlHelper.SetInt64(m_CharGuid.GetValue());
	mysqlHelper.DoSQL();

}

