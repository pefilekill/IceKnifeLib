
#include "CDbControl.h"
#include "CPubFuncUtil.h"


//打开指定的数据库文件
sqlite3* CDbControl::OpenDb(string strDbFilePath)
{
	sqlite3*  sqDb = NULL;
	if (strDbFilePath.length() > 0)
	{
		sqlite3_open( strDbFilePath.c_str(), &sqDb);
	}
	return sqDb;
}
//
void CDbControl::CloseDb(sqlite3*  &sqDb)
{
	if (sqDb != NULL)
		sqlite3_close(sqDb);
	sqDb = NULL;
}
//查看表是否存在
bool CDbControl::IsTableExistInDb(sqlite3 *sqDb, string strTableName)
{
    char szQuery[1024] = {0};
    sprintf(szQuery, "SELECT count(*) FROM sqlite_master WHERE type=\"table\" AND name=\"%s\"", strTableName.c_str());
	sqlite3_stmt *pstmt;
	sqlite3_prepare(sqDb, szQuery, strlen(szQuery), &pstmt, NULL);
	sqlite3_step(pstmt);
	int count=sqlite3_column_int(pstmt,0);
	sqlite3_finalize(pstmt);
	if(count > 0)
		return true;
	return false;
}
//
bool CDbControl::IsValueExistInDb(sqlite3 *sqDb, string strTableName, string strColumName, string strValue)
{
    char szQuery[1024] = {0};
    sprintf(szQuery, "SELECT count(*) FROM %s WHERE  %s=\"%s\"",
					(strTableName.c_str()),
					(strColumName.c_str()),
					(strValue.c_str()));
	sqlite3_stmt *pstmt;
	sqlite3_prepare(sqDb, szQuery, strlen(szQuery), &pstmt, NULL);
	sqlite3_step(pstmt);
	int count=sqlite3_column_int(pstmt,0);
	sqlite3_finalize(pstmt);

	if(count > 0)
		return true;
	return false;
}
//是否存在多个
bool CDbControl::IsSomeValueExistInDb(sqlite3 *sqDb, string strTableName, string strColumName, string strValue)
{
    char szQuery[1024] = {0};
    sprintf(szQuery, "SELECT count(*) FROM %s WHERE  %s=\"%s\"",
		(strTableName.c_str()),
		(strColumName.c_str()),
		(strValue.c_str()));
	sqlite3_stmt *pstmt;
	sqlite3_prepare(sqDb, szQuery, strlen(szQuery), &pstmt, NULL);
	sqlite3_step(pstmt);
	int count=sqlite3_column_int(pstmt,0);
	sqlite3_finalize(pstmt);

	if(count > 1)
		return true;
	return false;
}


//查看表是否存在
int CDbControl::GetRowCountOfTable(sqlite3 *sqDb, string strTableName)
{
    char szQuery[1024] = {0};
    sprintf(szQuery, "SELECT count(*) FROM \"%s\"", (strTableName.c_str()));
	// 	char sql_query[128]={0};
	// 	sprintf_s(sql_query,128, "SELECT count(*) FROM sqlite_master WHERE type=\"table\" AND name=\"%s\"", strTableName.c_str());
	sqlite3_stmt *pstmt;
	sqlite3_prepare(sqDb, szQuery, strlen(szQuery), &pstmt, NULL);
	sqlite3_step(pstmt);
	int count=sqlite3_column_int(pstmt,0);
	sqlite3_finalize(pstmt);
	return count;
}
