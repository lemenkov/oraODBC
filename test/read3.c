/* test reads data from some_numeric_types table inserted by insert3.c
 *
 * author: Dennis Box, dbox@fnal.gov
 * $Id: read3.c,v 1.8 2003/01/17 23:10:42 dbox Exp $
 */

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

    int anInteger_i,anInt_i,aSmallInt_i;
    float aDecimal83_f,aNumeric94_f,aFloat_f,aFloat9_f,aReal_f;
    SQLSMALLINT numCols_si, p1_si;
    SQLINTEGER p2_i;
    SQLCHAR buf1[MAX_LEN];
    SQLCHAR buf2[MAX_LEN];
   
    SQLUSMALLINT i_usi;
    SQLSMALLINT len_si,tp_si;
    SQLINTEGER type_i;
    SQLSMALLINT sz_si;
    SQLUINTEGER sz_ui;
    SQLSMALLINT scale_si;
    SQLSMALLINT nullable_si;

    

    GET_LOGIN_VARS();
      
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

    rc = SQLNumResultCols(StmtHandle,&numCols_si);
    assert(rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO);
    assert(numCols_si==8);


    rc = SQLColAttribute(StmtHandle, 1, SQL_DESC_COUNT,
			      NULL, NULL, NULL, &p2_i);
    assert(rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO);

    VERBOSE("SQLNumResultCols says  %d SQLColAttribute says %d\n",numCols_si,p2_i);

    assert(p2_i == 8);

    rc = SQLNumParams(StmtHandle,&p1_si);
    assert(rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO);

    assert(p1_si == 0);

    for(i_usi=1; i_usi<=numCols_si; i_usi++)
      {
	rc = SQLDescribeCol(StmtHandle, 
			    i_usi,
			    buf1,
			    sizeof(buf1),
			    &len_si,
			    &tp_si,
			    &sz_ui,
			    &scale_si,
			    &nullable_si);

	assert(rc==SQL_SUCCESS); 
 
	VERBOSE("SQLDescribeCol col=%d name:%s len=%d type_i=%d size=%d scale_si=%d nullable=%d\n"
		,i_usi,buf1,len_si,type_i,sz_ui,scale_si,nullable_si);
	/*	
	if(i_usi<3) assert(type_i==SQL_C_SLONG);
	if(i_usi==6)assert(type_i==SQL_C_DOUBLE);
	if(i_usi==8)assert(type_i==SQL_C_FLOAT);
	if(i_usi==4||i_usi==5||i_usi==7)  assert(type_i==SQL_C_NUMERIC);
	*/
	

	rc = SQLColAttribute(StmtHandle, i_usi, SQL_DESC_NAME,
			      buf2, sizeof(buf2), &sz_si, NULL);

	assert(rc==SQL_SUCCESS);
	assert(strcmp(buf1,buf2)==0);

	type_i=0;
	rc = SQLColAttribute(StmtHandle, i_usi, SQL_DESC_TYPE,
			      NULL, NULL,NULL,(SQLPOINTER)&type_i);
	assert(rc==SQL_SUCCESS);
	VERBOSE("SQLColAttribute col %d type %d \n", i_usi, type_i );
	if(i_usi<3) assert(type_i==SQL_C_SLONG);
	if(i_usi==6)assert(type_i==SQL_C_DOUBLE);
	if(i_usi==8)assert(type_i==SQL_C_FLOAT);
	if(i_usi==4||i_usi==5||i_usi==7)  assert(type_i==SQL_C_NUMERIC);

        type_i=0;
        rc = SQLColAttribute(StmtHandle, i_usi, SQL_DESC_CONCISE_TYPE,
                              NULL, NULL,NULL,(SQLPOINTER)&type_i);
        assert(rc==SQL_SUCCESS);
	VERBOSE("SQLColAttribute col %d concise type %d \n", i_usi, type_i );
        if(i_usi<3) assert(type_i==SQL_C_NUMERIC);
        if(i_usi==6)assert(type_i==SQL_C_DOUBLE);
        if(i_usi==8)assert(type_i==SQL_C_FLOAT);
        if(i_usi==4||i_usi==5||i_usi==7)  assert(type_i==SQL_C_NUMERIC);


      }




    /*SQLGetData does not seem to work for SQL_NUMBERIC or SQL_DOUBLE */

    while(SQLFetch(StmtHandle)==SQL_SUCCESS){

      rc = SQLGetData(StmtHandle, 1, SQL_C_SLONG, 
		      &anInteger_i, sizeof(anInteger_i), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 2, SQL_C_SLONG, 
		      &anInt_i, sizeof(anInt_i), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 3, SQL_C_SLONG, 
		      &aSmallInt_i, sizeof(aSmallInt_i), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 4, SQL_C_FLOAT, 
		      &aDecimal83_f, sizeof(aDecimal83_f), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 5, SQL_C_FLOAT, 
		      &aNumeric94_f, sizeof(aNumeric94_f), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 6, SQL_C_FLOAT, 
		      &aFloat_f, sizeof(aFloat_f), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      
      rc = SQLGetData(StmtHandle, 7, SQL_C_FLOAT, 
		      &aFloat9_f, sizeof(aFloat9_f), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      
      rc = SQLGetData(StmtHandle, 8, SQL_C_FLOAT, 
		      &aReal_f, sizeof(aReal_f), NULL);
      assert( rc == SQL_SUCCESS || rc == SQL_NO_DATA );
      

      VERBOSE("anInteger_i=%d, anInt_i=%d, aSmallInt_i=%d, aDecimal83_f=%f, aNumeric94_f=%f, aFloat_f=%f, aFloat9_f=%f, aReal_f=%f\n",
               anInteger_i,anInt_i,aSmallInt_i,aDecimal83_f,
	       aNumeric94_f,aFloat_f,aFloat9_f,aReal_f);
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
