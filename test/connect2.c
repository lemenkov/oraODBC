/*      test following functions:                                  */
/*              SQLAllocHandle()                                   */
/*              SQLConnect()                                       */      
/*              SQLDisconnect()                                    */
/*              SQLFreeHandle()                                    */
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
    #define MAX_LEN 100
    SQLRETURN   rc = SQL_SUCCESS;
    SQLCHAR     twoTask[MAX_LEN];
    SQLCHAR     *userName = "scott";
    SQLCHAR     *pswd = "tiger";
    SQLCHAR     driverStr[MAX_LEN];
    SQLHANDLE    EnvHandle;
    SQLHANDLE    ConHandle;
   

   
    
    if(getenv("TWO_TASK") && strlen((const char*)getenv("TWO_TASK"))<MAX_LEN)
      sprintf(twoTask,"%s",getenv("TWO_TASK"));
    else{
      fprintf(stderr,"Error: TWO_TASK env variable must be set\n");
      exit(-1);
    }



  
    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &EnvHandle);
    assert(rc == SQL_SUCCESS);
    assert(EnvHandle != (SQLHANDLE)NULL);


   
    rc = SQLSetEnvAttr(EnvHandle, SQL_ATTR_ODBC_VERSION, 
		       (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_UINTEGER);

    assert(rc == SQL_SUCCESS);
    
    rc = SQLAllocHandle(SQL_HANDLE_DBC, EnvHandle, &ConHandle);
    assert(ConHandle != (SQLHANDLE)NULL);
    assert(rc == SQL_SUCCESS);

    sprintf(driverStr,"DRIVER=\"%s\"; DB=\"%s\"; DSN=\"%s\"; UID=\"%s\"; PWD=\"%s\";",
	    "oracle",twoTask,twoTask,userName,pswd);
    VERBOSE("connecting with string '%s'\n",driverStr);
    rc = SQLDriverConnect(ConHandle, NULL, driverStr,
			  SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
   

    T_ASSERT2(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO,
	    " failed to login with this string:",driverStr );

    VERBOSE("Connected to  database %s\n",twoTask);

   
    rc = SQLDisconnect(ConHandle);
    assert(rc == SQL_SUCCESS);
    VERBOSE("disconnected from  database\n");
    
   
    assert (ConHandle != (SQLHANDLE)NULL);
    rc = SQLFreeHandle(SQL_HANDLE_DBC, ConHandle);
    assert(rc == SQL_SUCCESS);
   

    assert (EnvHandle != (SQLHANDLE)NULL);
    rc = SQLFreeHandle(SQL_HANDLE_ENV, EnvHandle);
    assert(rc == SQL_SUCCESS);
   

    return(rc);
}
