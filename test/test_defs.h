
/* bunch of definitions common to all of the tests in this subdirectory
 * author: Dennis Box, dbox@fnal.gov
 * $Id: test_defs.h,v 1.5 2002/05/31 19:55:00 dbox Exp $
 */

#ifndef _TEST_DEFS__H
#define _TEST_DEFS__H
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#ifdef WIN32
#include <windows.h>
#endif

    
#define MAX_LEN 100
SQLRETURN   rc = SQL_SUCCESS;
SQLCHAR     twoTask[MAX_LEN];
SQLCHAR     driverStr[MAX_LEN];
SQLCHAR     userName[MAX_LEN];
SQLCHAR     pswd[MAX_LEN];
SQLHANDLE    EnvHandle;
SQLHANDLE    ConHandle;
HSTMT    StmtHandle;
SQLCHAR  SQLStmt[255];



#define VERBOSE if(getenv("VERBOSE")!=NULL)printf

#define T_ASSERT( x , y ) if (!x) printf("%s\n",y); assert(x);
#define T_ASSERT2( x , y , z ) if (!x) printf("%s %s\n",y,z); assert(x);
#define T_ASSERT3( x , y , z , a ) if (!x) printf("%s %s %s\n",y,z,a); assert(x);
#define T_ASSERT4( x , y , z , a , b  ) if (!x) printf("%s %s %s %s \n",y,z,a,b); assert(x);
#define T_ASSERT5( x , y , z , a, b, c ) if (!x) printf("%s %s %s %s %s\n",y,z,a, b, c); assert(x);
#define T_ASSERT6( x , y , z , a, b, c ,d) if (!x) printf("%s %s %s %s %s %s\n",y,z,a, b, c,d); assert(x);
#endif


#define GET_LOGIN_VARS()     if(getenv("TWO_TASK") && strlen((const char*)getenv("TWO_TASK"))<MAX_LEN)\
      sprintf(twoTask,"%s",getenv("TWO_TASK"));\
    else{\
      fprintf(stderr,"Error: TWO_TASK env variable must be set\n");\
      exit(-1);\
    }\
    if(getenv("ORACLE_USER") && strlen((const char*)getenv("ORACLE_USER"))<MAX_LEN)\
      sprintf(userName,"%s",getenv("ORACLE_USER"));\
    else{\
      fprintf(stderr,"Error: ORACLE_USER env variable must be set\n");\
      exit(-1);\
    }\
    if(getenv("ORACLE_PSWD") && strlen((const char*)getenv("ORACLE_PSWD"))<MAX_LEN)\
      sprintf(pswd,"%s",getenv("ORACLE_PSWD"));\
    else{\
      fprintf(stderr,"Error: ORACLE_PSWD env variable must be set\n");\
      exit(-1);\
    }
