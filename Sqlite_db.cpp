/**************************************************************************** **
 * Copyright (C) 2001-2020 Inhand Networks, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : Sqlite_db.cpp
 *     FIRST CREATION DATE    : 2021/02/02
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2021/02/02
 *     FILE DESCRIPTION       :
** ************************************************************************** */

#include <iostream>
#include <sqlite3.h>

#include "Sqlite_db.h"
#include "/home/xuke/bin/dbg.h"

using namespace std;

static int callback(void *data, int argc, char **argv, char **azColName)
{
	int i;

	if (data) {
		fprintf(stdout, "%s: \n", (const char*)data);
	}

	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");

	return 0;
}

sqlite3 *get_db_handle(void)
{
	return gl_db;
}

int db_open(void)
{
	int rc;
	char *zErrMsg = 0;

	rc = sqlite3_open(DB_FILE_PATH, &gl_db);
	if (rc) {
		fprintf(stderr, "Can't open database:%s, error:%s\n", DB_FILE_PATH, sqlite3_errmsg(gl_db));
		sqlite3_close(gl_db);
		return -1;
	}

	db_create_table(PUB_MSG_TABLE_NAME);

	return 0;
}

int db_create_table(const char *table)
{
	char *zErrMsg = 0;
	int  rc;
	char sql[MAX_SQL_CMD_LEN];

	sqlite3 *db = get_db_handle();
	if(!db) {
		printf("db of %s cannot be found", table);
		return -1;
	}

	/* Create SQL statement */
	snprintf(sql, sizeof(sql), CREATE_TABL_SCHEMA_FMT, table);

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Table created successfully\n");
	}

   return 0;
}

int db_table_insert(char *table, const char *column_names, const char *row_values)
{
	char *zErrMsg = 0;
	int rc;
	char sql[MAX_SQL_CMD_LEN];
	const char *data = "Callback function called";

	sqlite3 *db = get_db_handle();
	if(!db) {
		printf("db of %s cannot be found", table);
		return -1;
	}

	/* Create SQL statement */
	snprintf(sql, sizeof(sql), INSERT_CMD_FMT, table, column_names, row_values, table);

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Records created successfully\n");
	}

	return 0;
}

int db_table_get_key_value(char *table, const char *primary_key, const char *key, char *buff)
{
	char *zErrMsg = 0;
	int rc;
	char sql[MAX_SQL_CMD_LEN];
	const char *data = "Callback function called";

	sqlite3 *db = get_db_handle();
	if(!db) {
		printf("db of %s cannot be found", table);
		return -1;
	}

	/* SELECT * FROM <table> WHERE <primary key>=<value> */
	snprintf(sql, sizeof(sql), "SELECT * FROM %s WHERE %s=%s;", table, primary_key, key);

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Operation done successfully\n\n");
	}

	return 0;
}

int db_table_update(char *table, const char *column_names, const char *row_values, const int id)
{
	char *zErrMsg = 0;
	int rc;
	char sql[MAX_SQL_CMD_LEN];
	const char* data = "Callback function called";

	sqlite3 *db = get_db_handle();
	if(!db) {
		printf("db of %s cannot be found", table);
		return -1;
	}

	/* Create merged SQL statement */
	snprintf(sql, sizeof(sql), UPDATE_CMD_FMT, table, column_names, row_values, id, table);

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return 0;
}

int db_table_delete_key(char *table, const char *key, const char *value)
{
	char *zErrMsg = 0;
	int rc;
	char sql[MAX_SQL_CMD_LEN];
	const char* data = "Callback function called";

	sqlite3 *db = get_db_handle();
	if(!db) {
		printf("db of %s cannot be found", table);
		return -1;
	}

	/* Create merged SQL statement */
	snprintf(sql, sizeof(sql), "DELETE FROM %s WHERE %s='%s'; SELECT * from %s;", table, key, value, table);

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return 0;
}

int db_close(void)
{
	sqlite3_close(gl_db);

	return 0;
}

/****************************************************************************
 *  Function Name : main
 *  Description   : The Main Function to test this Sqlite_db API.
 *  Input(s)      : argc - The numbers of input value.
 *                : argv - The pointer to input specific parameters.
 *  Output(s)     : NULL
 *  Returns       : 0
 ****************************************************************************/
int main(int argc, const char *argv[])
{
	string column_names = "ID, NAME, AGE, ADDRESS, SALARY";
	string entry_values;
	char table[] = PUB_MSG_TABLE_NAME;

	db_open();
	
	/* InhandTag: insert */
	my_debug_msg("------[INSERT 1]:");
	entry_values = "1, 'xuke', 23, 'ADDRESS 1', 1.00";
	db_table_insert(table, column_names.c_str(), entry_values.c_str());
	my_debug_msg("------[INSERT 2]:");
	entry_values = "2, 'EExu', 24, 'address 2', 5.00";
	db_table_insert(table, column_names.c_str(), entry_values.c_str());

	/* InhandTag: get */
	my_debug_msg("------[SELECT]:");
	db_table_get_key_value(table, "ID", "1", 0);
	db_table_get_key_value(table, "ID", "2", 0);

	/* InhandTag: change */
	my_debug_msg("------[UPDATE 1]:");
	db_table_update(table, "NAME", "EExuke", 1);
	my_debug_msg("------[UPDATE 2]:");
	db_table_update(table, "ADDRESS", "ADDRESS 2", 2);

	/* InhandTag: delete */
	my_debug_msg("------[DELETE]:");
	db_table_delete_key(table, "NAME", "EExu");

	db_close();

	return 0;
}

