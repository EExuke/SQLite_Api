/**************************************************************************** **
 * Copyright (C) 2001-2020 Inhand Networks, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : Sqlite_db.h
 *     FIRST CREATION DATE    : 2021/02/02
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2021/02/02
 *     FILE DESCRIPTION       : API exported to external moduels
** ************************************************************************** */
#ifndef _SQLITE_DB_H_
#define _SQLITE_DB_H_

/*------------------Macro Definition-------------------------*/
/* data base file path */
#define DB_FILE_PATH          "./Sq_test.db"
#define PUB_MSG_TABLE_NAME    "test_TB"
#define MAX_SQL_CMD_LEN       256

#define CREATE_TABL_SCHEMA_FMT    \
	"CREATE TABLE %s ("  \
	"ID INT PRIMARY KEY     NOT NULL," \
	"NAME           TEXT    NOT NULL," \
	"AGE            INT     NOT NULL," \
	"ADDRESS        CHAR(50)," \
	"SALARY         REAL );"

#define INSERT_CMD_FMT    \
	"INSERT INTO %s (%s) VALUES (%s);" \
	"SELECT * FROM %s;"

#define UPDATE_CMD_FMT    \
	"UPDATE %s set %s='%s' where ID=%d; " \
	"SELECT * FROM %s;"

/*------------------End of Macro Definition------------------*/
static sqlite3 *gl_db;

/*------------------API Definition---------------------------*/
static int callback(void *data, int argc, char **argv, char **azColName);
sqlite3 *get_db_handle(void);
int db_open(void);
int db_create_table(const char *table);
int db_table_insert(char *table, const char *column_names, const char *row_values);
int db_table_get_key_value(char *table, const char *primary_key, const char *key, char *buff);
int db_table_update(const char *table, const char *column_names, const char *row_values, const int id);
int db_table_delete_key(char *table, const char *key, const char *value);
int db_close(void);
/*------------------End of API Definition--------------------*/

#endif /* End of _SQLITE_DB_H_ */

