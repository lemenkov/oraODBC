
/*      test following functions:                                  */

/*              SQLAllocHandle()                                   */
/*              SQLAllocStmt()                                     */
/*              SQLColAttribute() -very cursory check-             */      
/*              SQLConnect()                                       */      
/*              SQLDisconnect()                                    */
/*              SQLExecDirect()                                    */
/*              SQLFreeHandle()                                    */
/*              SQLNumResultCols()                                 */
/*              SQLNumDescribeCol()                                */
/*              SQLSetEnvAttr()                                    */


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
    #define MAX_LEN 255
    SQLRETURN   rc = SQL_SUCCESS;
    SQLCHAR     twoTask[MAX_LEN];
    SQLCHAR     *userName = "scott";
    SQLCHAR     *pswd = "tiger";
    SQLHANDLE    EnvHandle;
    SQLHANDLE    ConHandle;
    SQLHANDLE   StmtHandle;
    SQLCHAR  SQLStmt[MAX_LEN];

    int an_int;
    SQLUSMALLINT num_cols;
    float a_float;
    SQLCHAR buf1[MAX_LEN];
    SQLCHAR buf2[MAX_LEN];
   
    SQLUSMALLINT col;
    SQLSMALLINT col_len;
    SQLSMALLINT type;
    SQLUINTEGER sz;
    SQLSMALLINT scale;
    SQLSMALLINT can_null;


    
    if(getenv("TWO_TASK") && strlen((const char*)getenv("TWO_TASK"))<MAX_LEN)
      sprintf(twoTask,"%s",getenv("TWO_TASK"));
    else{
      fprintf(stderr,"Error: TWO_TASK env variable must be set\n");
      exit(-1);
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
    VERBOSE("allocing handle\n");

    rc = SQLAllocStmt(ConHandle, &StmtHandle);
    assert(rc == SQL_SUCCESS);

 
    sprintf(SQLStmt,"select an_int, a_float, a_string from some_types");
  
    VERBOSE("executing %s\n",SQLStmt);
  
    rc = SQLExecDirect(StmtHandle, SQLStmt, SQL_NTS);
    assert(rc == SQL_SUCCESS);

    rc = SQLNumResultCols(StmtHandle,&num_cols);
    assert(rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO);
    assert(num_cols==3);


    for(col=1; col<=num_cols; col++)
      {
	rc = SQLDescribeCol(StmtHandle,col,buf1,MAX_LEN,&col_len,
			    &type,&sz,&scale,&can_null);

	assert(rc==SQL_SUCCESS);

	VERBOSE("col=%d name:%s len=%d type=%d size=%d scale=%d nullable=%d\n"
		,col,buf1,col_len,type,sz,scale,can_null);

	rc = SQLColAttribute(StmtHandle, col, SQL_DESC_NAME,
			      buf2, sizeof(buf2), &type, NULL);

	assert(rc==SQL_SUCCESS);
	assert(strcmp(buf1,buf2)==0);


      }



    rc = SQLBindCol(StmtHandle, 1, SQL_C_SLONG, 
             &an_int, sizeof(an_int), NULL);
    assert(rc == SQL_SUCCESS);

    rc = SQLBindCol(StmtHandle, 2, SQL_C_FLOAT, 
             &a_float, sizeof(a_float), NULL);
    assert(rc == SQL_SUCCESS);

    rc = SQLBindCol(StmtHandle, 3, SQL_C_CHAR, 
            (SQLPOINTER) buf1, sizeof(buf1), NULL);
    assert(rc == SQL_SUCCESS);


    do{
      rc = SQLFetch(StmtHandle);
      VERBOSE("an_int=%d a_float=%f a_string=%s\n",an_int,a_float,buf1);
    }while(rc==SQL_SUCCESS);

    assert(rc==SQL_NO_DATA);

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
