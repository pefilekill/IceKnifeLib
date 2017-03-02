
// 全部和数据库操作相关的操作都在这里
#pragma once
#include <string>
#include <sqlite/sqlite3.h>

using namespace std;


class CDbControl
{
public:
	//打开指定的数据库文件
	static sqlite3*		OpenDb(string strDbFilePath);
	//关闭数据库
	static void			CloseDb();
	//关闭数据库
	static void			CloseDb(sqlite3*  &sqDb);
	//查看表是否存在
 	static bool			IsTableExistInDb(sqlite3 *sqDb, string strTableName);
	//查看值是否存在
	static bool			IsValueExistInDb(sqlite3 *sqDb, string strTableName, string strColumName, string strValue);
	//是否存在多个
	static bool			IsSomeValueExistInDb(sqlite3 *sqDb, string strTableName, string strColumName, string strValue);
	//获取表的所有行数
	static int			GetRowCountOfTable(sqlite3 *sqDb, string strTableName);


protected:
private:
};



