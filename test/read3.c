
/*      test following functions:                                  */

/*              SQLAllocHandle()                                   */
/*              SQLAllocStmt()                                     */
/*              SQLColAttribute() -very cursory check-             */      
/*              SQLConnect()                                       */      
/*              SQLDisconnect()                                    */
/*              SQLExecDirect()                                    */
/*              SQLGetData()                                       */
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

    int anInteger,anInt,aSmallInt;
    float aDecimal83,aNumeric94,aFloat,aFloat9,aReal;
    SQLUSMALLINT num_cols;
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

 
    sprintf(SQLStmt,"select anInteger,anInt,aSmallInt,aDecimal83,\
                     aNumeric94,aFloat,aFloat9,aReal from some_numeric_types");
  
    VERBOSE("executing %s\n",SQLStmt);
  
    rc = SQLExecDirect(StmtHandle, SQLStmt, SQL_NTS);
    assert(rc == SQL_SUCCESS);

    rc = SQLNumResultCols(StmtHandle,&num_cols);
    assert(rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO);
    assert(num_cols==8);


    for(col=1; col<=num_cols; col++)
      {
	rc = SQLDescribeCol(StmtHandle,col,buf1,MAX_LEN,&col_len,
			    &type,&sz,&scale,&can_null);

	assert(rc==SQL_SUCCESS);

	VERBOSE("col=%d name:%s len=%d type=%d size=%d scale=%d nullable=%d\n"
		,col,buf1,col_len,type,sz,scale,can_null);

	if(col<3) assert(type==SQL_C_SLONG);
	if(col==6)assert(type==SQL_C_DOUBLE);
	if(col==8)assert(type==SQL_C_FLOAT);
	if(col==4||col==5||col==7)  assert(type==SQL_C_NUMERIC);
	

	rc = SQLColAttribute(StmtHandle, col, SQL_DESC_NAME,
			      buf2, sizeof(buf2), &type, NULL);

	assert(rc==SQL_SUCCESS);
	assert(strcmp(buf1,buf2)==0);

	type=0;
	rc = SQLColAttribute(StmtHandle, col, SQL_DESC_TYPE,
			      NULL, NULL,NULL,(SQLPOINTER)&type);
	assert(rc==SQL_SUCCESS);
	if(col<3) assert(type==SQL_C_SLONG);
	if(col==6)assert(type==SQL_C_DOUBLE);
	if(col==8)assert(type==SQL_C_FLOAT);
	if(col==4||col==5||col==7)  assert(type==SQL_C_NUMERIC);

      }




    /*SQLGetData does not seem to work for SQL_NUMBERIC or SQL_DOUBLE */

    while(SQLFetch(StmtHandle)==SQL_SUCCESS){

      rc = SQLGetData(StmtHandle, 1, SQL_C_SLONG, 
		      &anInteger, sizeof(anInteger), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 2, SQL_C_SLONG, 
		      &anInt, sizeof(anInt), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 3, SQL_C_SLONG, 
		      &aSmallInt, sizeof(aSmallInt), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 4, SQL_C_FLOAT, 
		      &aDecimal83, sizeof(aDecimal83), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 5, SQL_C_FLOAT, 
		      &aNumeric94, sizeof(aNumeric94), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 6, SQL_C_FLOAT, 
		      &aFloat, sizeof(aFloat), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      
      rc = SQLGetData(StmtHandle, 7, SQL_C_FLOAT, 
		      &aFloat9, sizeof(aFloat9), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 8, SQL_C_FLOAT, 
		      &aReal, sizeof(aReal), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      

      VERBOSE("anInteger=%d, anInt=%d, aSmallInt=%d, aDecimal83=%f, aNumeric94=%f, aFloat=%f, aFloat9=%f, aReal=%f\n",
               anInteger,anInt,aSmallInt,aDecimal83,
	       aNumeric94,aFloat,aFloat9,aReal);
    }
    
    
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
