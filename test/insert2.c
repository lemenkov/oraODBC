


/*test array inserts, doesnt seem to work!*/
/*see SQLSetStmtAttr.c line 188 */
#include "test_defs.h"

#include <stdio.h>
#include <assert.h>
#ifdef WIN32
#include <windows.h>
#endif

#include <sql.h>
#include <sqlext.h>



int main()
{
    // Declare The Local Memory Variables
    #define MAX_CHAR_LEN 255
    #define ARRAY_LEN 3
    SQLRETURN   rc = SQL_SUCCESS;
    SQLCHAR     twoTask[MAX_CHAR_LEN];
    SQLCHAR     *userName = "scott";
    SQLCHAR     *pswd = "tiger";
    SQLHANDLE    EnvHandle;
    SQLHANDLE    ConHandle;
    HSTMT    StmtHandle;
    SQLCHAR  SQLStmt[MAX_CHAR_LEN];
    SQLINTEGER   anIntArray[ARRAY_LEN];
    SQLFLOAT   aFloatArray[ARRAY_LEN];
    SQLCHAR   aCharArray[ARRAY_LEN][MAX_CHAR_LEN];
    SQLINTEGER   charInsArray[ARRAY_LEN];
    int i;

    if(getenv("TWO_TASK") && strlen((const char*)getenv("TWO_TASK"))<MAX_CHAR_LEN)
      sprintf(twoTask,"%s",getenv("TWO_TASK"));
    else{
      fprintf(stderr,"Error: TWO_TASK env variable must be set\n");
      exit(-1);
    }
      
    for(i=0;i<ARRAY_LEN;i++){
      anIntArray[i]=i;
      aFloatArray[i]=(float)i+0.5;
      sprintf(aCharArray[i],"int=%d flt=%f",anIntArray[i],aFloatArray[i]);
      charInsArray[i]=SQL_NTS;
    }
    VERBOSE("calling SQLAllocHandle(EnvHandle) \n");

    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &EnvHandle);
    assert(rc == SQL_SUCCESS);
    assert(EnvHandle != (SQLHANDLE)NULL);


   
    rc = SQLSetEnvAttr(EnvHandle, SQL_ATTR_ODBC_VERSION, 
		       (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_UINTEGER);

    assert(rc == SQL_SUCCESS);
        
    VERBOSE("calling SQLAllocHandle(ConHandle) \n");

    rc = SQLAllocHandle(SQL_HANDLE_DBC, EnvHandle, &ConHandle);
    assert(ConHandle != (SQLHANDLE)NULL);
    assert(rc == SQL_SUCCESS);
   
    rc = SQLConnect(ConHandle, twoTask, SQL_NTS, 
		    (SQLCHAR *)userName , SQL_NTS, (SQLCHAR *) pswd, SQL_NTS);
    assert(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO );

    VERBOSE("connected to  database %s\n",twoTask);

   

    rc = SQLAllocStmt(ConHandle, &StmtHandle);
    assert(rc == SQL_SUCCESS);

    /* Set The SQL_ATTR_ROW_BIND_TYPE Statement Attribute To Tell
       The Driver To Use Column-Wise Binding. */
       rc = SQLSetStmtAttr(StmtHandle, SQL_ATTR_PARAM_BIND_TYPE, 
			   SQL_PARAM_BIND_BY_COLUMN, 0);
    assert(rc==SQL_SUCCESS );
  
    /*Tell The Driver That There Are 3 Values For Each Parameter
      (By Setting The SQL_ATTR_PARAMSET_SIZE Statement
      Attribute*/
    rc = SQLParamOptions(StmtHandle,  ARRAY_LEN, 0);
    assert(rc==SQL_SUCCESS);
    
     
    sprintf(SQLStmt,"insert into some_types values( ");
    strcat(SQLStmt," ?, ?, ? ) ");

    VERBOSE("preparing statement %s\n", SQLStmt);


    rc = SQLPrepare(StmtHandle, SQLStmt, SQL_NTS);
    assert(rc == SQL_SUCCESS);
 
    VERBOSE("binding....\n");

    rc = SQLBindParameter(StmtHandle, 1, SQL_PARAM_INPUT, 
			  SQL_C_DEFAULT, SQL_INTEGER, 0, 0, anIntArray, 0,
			  NULL);
    assert(rc == SQL_SUCCESS);

    rc = SQLBindParameter(StmtHandle, 2, SQL_PARAM_INPUT, 
			  SQL_C_DEFAULT, SQL_FLOAT, 0, 0, aFloatArray, 0,
			  NULL);
    assert(rc == SQL_SUCCESS);

    rc = SQLBindParameter(StmtHandle, 3, SQL_PARAM_INPUT, 
			  SQL_C_CHAR, SQL_CHAR, MAX_CHAR_LEN, 
			  0, aCharArray, MAX_CHAR_LEN,
			  charInsArray);
    assert(rc == SQL_SUCCESS);

    VERBOSE("executing....\n");


    rc = SQLExecute(StmtHandle);

 
    assert(rc == SQL_SUCCESS);
    VERBOSE("success: executed statement\n");

    VERBOSE("calling SQLFreeStmt\n");
    if (StmtHandle != NULL)
      rc=SQLFreeStmt(StmtHandle, SQL_DROP);
    assert(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO);


    rc = SQLDisconnect(ConHandle);
    assert(rc == SQL_SUCCESS);
    VERBOSE("disconnected from  database\n");
     

    VERBOSE("calling SQLFreeHandle(ConHandle) \n");

    assert (ConHandle != (SQLHANDLE)NULL);
    rc = SQLFreeHandle(SQL_HANDLE_DBC, ConHandle);
    assert(rc == SQL_SUCCESS);
   
    VERBOSE("calling SQLFreeHandle(EnvHandle) \n");

    assert (EnvHandle != (SQLHANDLE)NULL);
    rc = SQLFreeHandle(SQL_HANDLE_ENV, EnvHandle);
    assert(rc == SQL_SUCCESS);
   

    return(rc);
}
