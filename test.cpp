void test()
{

	char sqltext[40960] = "";
	sprintf(sqltext, "select count(*) from `t_char` where `charname`='%s'", "test");
	MYSQLSTMTHelper mysqlHelper(m_pMYSQLStatement);
	mysqlHelper.DoSQL(sqltext);

	// ѡ��ȡ���ؽ��
	std::unique_ptr<sql::ResultSet> pMYSQLResultSet = mysqlHelper.GetResultSet();
	while (pMYSQLResultSet && pMYSQLResultSet->next())
	{
		rRetMsg->mCount = pMYSQLResultSet->getInt(1);// ���
	}

}

void test2()
{

	// �����Ƕ����ƣ�Ҳ�������ַ���
	std::string pbstructstring = "";

	MYSQLPreparedSTMTHelper mysqlHelper(m_pMYSQLConnection, SQL::SAVECHAR3);
	BlobType commonData((char*)pbstructstring.c_str(), pbstructstring.size());
	mysqlHelper.SetBlob(&commonData);
	mysqlHelper.SetInt64(m_CharGuid.GetValue());
	mysqlHelper.DoSQL();

}

