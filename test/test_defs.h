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
    if(getenv("ORA_USER") && strlen((const char*)getenv("ORA_USER"))<MAX_LEN)\
      sprintf(userName,"%s",getenv("ORA_USER"));\
    else{\
      fprintf(stderr,"Error: ORA_USER env variable must be set\n");\
      exit(-1);\
    }\
    if(getenv("ORA_PWD") && strlen((const char*)getenv("ORA_PWD"))<MAX_LEN)\
      sprintf(pswd,"%s",getenv("ORA_PWD"));\
    else{\
      fprintf(stderr,"Error: ORA_PWD env variable must be set\n");\
      exit(-1);\
    }
