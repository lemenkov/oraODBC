/*******************************************************************************
 *
 * Copyright (c) 2000 Easysoft Ltd
 *
 * The contents of this file are subject to the Easysoft Public License 
 * Version 1.0 (the "License"); you may not use this file except in compliance 
 * with the License. 
 *
 * You may obtain a copy of the License at http://www.easysoft.org/EPL.html 
 *
 * Software distributed under the License is distributed on an "AS IS" basis, 
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for 
 * the specific language governing rights and limitations under the License. 
 *
 * The Original Code was created by Easysoft Limited and its successors. 
 *
 * Contributor(s): Tom Fosdick (Easysoft) 
 *
 *******************************************************************************
 *
 * $Id: SQLGetInfo.c,v 1.2 2002/04/03 01:21:44 dbox Exp $
 *
 * $Log: SQLGetInfo.c,v $
 * Revision 1.2  2002/04/03 01:21:44  dbox
 * reports that it supports transactions
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.10  2000/07/21 10:10:51  tom
 * Forward only cursor attributes added
 *
 * Revision 1.9  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.8  2000/07/07 08:07:26  tom
 * lots more debugging
 * A few more info sections implemented
 *
 * Revision 1.7  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.6  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.5  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.4  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:27:37  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLGetInfo.c,v $ $Revision: 1.2 $";

SQLRETURN SQL_API SQLGetInfo(
    SQLHDBC                ConnectionHandle,
    SQLUSMALLINT        InfoType,
    SQLPOINTER            InfoValuePtr,
    SQLSMALLINT            BufferLength,
    SQLSMALLINT            *StringLengthPtr )
{
    hDbc_T* dbc=(hDbc_T*)ConnectionHandle;
    SQLRETURN status=SQL_SUCCESS;
#ifdef UNIX_DEBUG
	char inftxt[80]="MISSED";
#endif
#ifdef ENABLE_TRACE
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)dbc,0,"i","InfoType",InfoType);
#endif
    ood_clear_diag((hgeneric*)dbc);

    switch(InfoType)
    {
        case SQL_ACCESSIBLE_PROCEDURES:
        case SQL_ACCESSIBLE_TABLES:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_ACCESSIBLE_TABLES");
#endif
            strcpy(InfoValuePtr,"N");
            break;
        
        case SQL_ACTIVE_ENVIRONMENTS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_ACTIVE_ENVIRONMENTS");
#endif
            *(SQLUSMALLINT*)InfoValuePtr=0;
            break;

        case SQL_AGGREGATE_FUNCTIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_AGGREGATE_FUNCTIONS");
#endif
            *(SQLUINTEGER*)InfoValuePtr=SQL_AF_ALL|
                SQL_AF_AVG|SQL_AF_COUNT|SQL_AF_DISTINCT|
                SQL_AF_MAX|SQL_AF_MIN|SQL_AF_SUM;
            break;

        case SQL_ALTER_DOMAIN: /* TODO */
        case SQL_ALTER_TABLE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_ALTER_TABLE");
#endif
            break;

        case SQL_ASYNC_MODE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_ASYNC_MODE");
#endif
            *(SQLUINTEGER*)InfoValuePtr=SQL_AM_NONE;
            break;

        case SQL_BATCH_ROW_COUNT: /* TODO */
        case SQL_BATCH_SUPPORT:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_BATCH_SUPPORT");
#endif
            break;

        case SQL_BOOKMARK_PERSISTENCE: /* TODO :- check this */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_BOOKMARK_PERSISTENCE");
#endif
            *(SQLUINTEGER*)InfoValuePtr=SQL_BP_OTHER_HSTMT
                |SQL_BP_UPDATE;
            break;

        case SQL_CATALOG_LOCATION:
        case SQL_CATALOG_USAGE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CATALOG_USAGE");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0; /* No catatlogs */
            break;

        case SQL_CATALOG_NAME:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CATALOG_NAME");
#endif
            strcpy(InfoValuePtr,"N");
            break;

        case SQL_CATALOG_NAME_SEPARATOR:
        case SQL_CATALOG_TERM:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CATALOG_TERM");
#endif
            *(SQLCHAR*)InfoValuePtr='\0';
            break;

        case SQL_COLLATION_SEQ: /* TODO :- find out what this is */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_COLLATION_SEQ");
#endif
            *(SQLCHAR*)InfoValuePtr='\0';
            break;

        case SQL_COLUMN_ALIAS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_COLUMN_ALIAS");
#endif
            strcpy(InfoValuePtr,"Y");
            break;

        case SQL_CONCAT_NULL_BEHAVIOR:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONCAT_NULL_BEHAVIOR");
#endif
            *(SQLUINTEGER*)InfoValuePtr=SQL_CB_NON_NULL;
            break;


        /*
         * Conversions. 
         */
        case SQL_CONVERT_BIGINT:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_BIGINT");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_BIGINT|
                SQL_CVT_NUMERIC;
            break;

        case SQL_CONVERT_BINARY:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_BINARY");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_BINARY;
            break;

        case SQL_CONVERT_BIT:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_BIT");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_BIT;
            break;

        case SQL_CONVERT_CHAR:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_CHAR");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_CHAR|
                SQL_CVT_VARCHAR;
            break;

        case SQL_CONVERT_DATE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_DATE");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_DATE|
                SQL_CVT_TIMESTAMP|
                SQL_CVT_CHAR|
                SQL_CVT_VARCHAR;
            break;

        case SQL_CONVERT_DECIMAL: /* TODO :- don't do these yet */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_DECIMAL");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                0;
            break;

        case SQL_CONVERT_DOUBLE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_DOUBLE");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_DOUBLE|
                SQL_CVT_NUMERIC|
                SQL_CVT_CHAR|
                SQL_CVT_VARCHAR;
            break;

        case SQL_CONVERT_FLOAT:
        case SQL_CONVERT_REAL:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_REAL");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_REAL|
                SQL_CVT_FLOAT|
                SQL_CVT_DOUBLE|
                SQL_CVT_NUMERIC|
                SQL_CVT_CHAR|
                SQL_CVT_VARCHAR;
            break;

        case SQL_CONVERT_INTEGER:
        case SQL_CONVERT_SMALLINT:
        case SQL_CONVERT_TINYINT:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_TINYINT");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_CHAR|
                SQL_CVT_VARCHAR|
                SQL_CVT_INTEGER|
                SQL_CVT_BIGINT|
                SQL_CVT_SMALLINT|
                SQL_CVT_NUMERIC;
            break;

        case SQL_CONVERT_INTERVAL_YEAR_MONTH:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_INTERVAL_YEAR_MONTH");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                0;
            break;
        case SQL_CONVERT_INTERVAL_DAY_TIME:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_INTERVAL_DAY_TIME");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                0;
            break;

        case SQL_CONVERT_LONGVARBINARY:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_LONGVARBINARY");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_LONGVARBINARY;
            break;

        case SQL_CONVERT_LONGVARCHAR:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_LONGVARCHAR");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_LONGVARCHAR;
            break;

        case SQL_CONVERT_NUMERIC:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_NUMERIC");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_CHAR|
                SQL_CVT_VARCHAR|
                SQL_CVT_NUMERIC;
            break;


        case SQL_CONVERT_TIME:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_TIME");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_TIME|
                SQL_CVT_TIMESTAMP|
                SQL_CVT_CHAR|
                SQL_CVT_VARCHAR;
            break;

        case SQL_CONVERT_TIMESTAMP:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_TIMESTAMP");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_TIMESTAMP|
                SQL_CVT_CHAR|
                SQL_CVT_VARCHAR;
            break;

            break;
        case SQL_CONVERT_VARBINARY:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_VARBINARY");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_VARBINARY;
            break;

        case SQL_CONVERT_VARCHAR:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_VARCHAR");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CVT_CHAR|
                SQL_CVT_VARCHAR;
            break;

        case SQL_CONVERT_FUNCTIONS: /* TODO :- find this out */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CONVERT_FUNCTIONS");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_CORRELATION_NAME: /* TODO :- make certain of this */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CORRELATION_NAME");
#endif
            *(SQLUSMALLINT*)InfoValuePtr=SQL_CN_ANY;
            break;

        case SQL_CREATE_ASSERTION: /* TODO  :- make certain of this */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CREATE_ASSERTION");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_CREATE_CHARACTER_SET: 
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CREATE_CHARACTER_SET");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0; 
            break;

        case SQL_CREATE_COLLATION:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CREATE_COLLATION");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_CREATE_DOMAIN:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CREATE_DOMAIN");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_CREATE_SCHEMA: /* CREATE USER != CREATE SCHEMA */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CREATE_SCHEMA");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_CREATE_TABLE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CREATE_TABLE");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CT_CREATE_TABLE|
                SQL_CT_TABLE_CONSTRAINT|
                SQL_CT_CONSTRAINT_NAME_DEFINITION|
                SQL_CT_COMMIT_DELETE|
                SQL_CT_GLOBAL_TEMPORARY|
                SQL_CT_COLUMN_CONSTRAINT|
                SQL_CT_COLUMN_DEFAULT|
                SQL_CT_CONSTRAINT_INITIALLY_IMMEDIATE| /* guess */
                SQL_CT_CONSTRAINT_NON_DEFERRABLE; /* guess */
            break;

        case SQL_CREATE_VIEW:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CREATE_VIEW");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_CV_CREATE_VIEW|
                SQL_CV_CHECK_OPTION|
                SQL_CV_CASCADED; /*guess*/
            break;

        case SQL_CURSOR_COMMIT_BEHAVIOR:
        case SQL_CURSOR_ROLLBACK_BEHAVIOR:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CURSOR_ROLLBACK_BEHAVIOR");
#endif
            *(SQLUSMALLINT*)InfoValuePtr=SQL_CB_DELETE;
            break;

        case SQL_CURSOR_SENSITIVITY:

#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_CURSOR_SENSITIVITY");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_UNSPECIFIED;
            break;

        case SQL_DATA_SOURCE_NAME: /*TODO this is not always strcitly correct */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DATA_SOURCE_NAME");
#endif
            THREAD_MUTEX_LOCK(dbc);
            ood_bounded_strcpy(InfoValuePtr,(char*)dbc->DSN,BufferLength);
            if(StringLengthPtr)
                *StringLengthPtr=strlen(dbc->DSN);
            THREAD_MUTEX_UNLOCK(dbc);
            break;

        case SQL_DATA_SOURCE_READ_ONLY:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DATA_SOURCE_READ_ONLY");
#endif
            strcpy(InfoValuePtr,"N");
            break;

        case SQL_DATABASE_NAME:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DATABASE_NAME");
#endif
            THREAD_MUTEX_LOCK(dbc);
            ood_bounded_strcpy(InfoValuePtr,(char*)dbc->DB,BufferLength);
            if(StringLengthPtr)
                *StringLengthPtr=strlen(dbc->DB);
            THREAD_MUTEX_UNLOCK(dbc);
            break;

        case SQL_DATETIME_LITERALS: 
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DATETIME_LITERALS");
#endif
            /*
             * TODO At the moment Oracle supports a superset of the types 
             * supported by this driver 
             */
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_DL_SQL92_DATE|
                SQL_DL_SQL92_TIME|
                SQL_DL_SQL92_TIMESTAMP;
            break;

        case SQL_DBMS_VER: /* TODO :- actually implement this */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DBMS_VER");
#endif
            *(SQLCHAR*)InfoValuePtr='\0';
            if(StringLengthPtr)
                *StringLengthPtr=0;
            break;

        case SQL_DBMS_NAME:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DBMS_NAME");
#endif
            ood_bounded_strcpy(InfoValuePtr,
                    ERROR_PRODUCT,BufferLength);
            if(StringLengthPtr)
                *StringLengthPtr=strlen(ERROR_PRODUCT);
            break;

        case SQL_DDL_INDEX:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DDL_INDEX");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_DI_CREATE_INDEX|
                SQL_DI_DROP_INDEX;
            break;

        case SQL_DEFAULT_TXN_ISOLATION:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DEFAULT_TXN_ISOLATION");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_TXN_SERIALIZABLE;
            break;

        case SQL_DESCRIBE_PARAMETER: /* When we actually implement it! */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DESCRIBE_PARAMETER");
#endif
            strcpy(InfoValuePtr,"Y");
            break;

        /* SQL_DM_VER, SQL_DRIVER_HDBC, SQL_DRIVER_HENV,
         * SQL_DRIVER_HDESC, SQL_DRIVER_HLIB, SQL_DRIVER_HSTMT are DM fns */

		case SQL_DRIVER_NAME:
#ifdef UNIX_DEBUG
		strcpy(inftxt,"SQL_DRIVER_NAME");
#endif
			/* TODO this is a hack! */
#ifdef WIN32
            strcpy(InfoValuePtr,"oracle.dll");
#else
            strcpy(InfoValuePtr,"liboraodbc.so");
#endif
            if(StringLengthPtr)
                *StringLengthPtr=strlen((const char*)InfoValuePtr);
			break;

        case SQL_DRIVER_ODBC_VER:
        case SQL_ODBC_VER:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DRIVER_ODBC_VER");
#endif
            ood_bounded_strcpy(InfoValuePtr,"03.51",BufferLength);
            if(StringLengthPtr)
                *StringLengthPtr=strlen((const char*)InfoValuePtr);
            break;

        case SQL_DRIVER_VER:
            {
                /*
                 * This is annoying as the library version string is
                 * not easily convertable into the correct format 
                 */
                char tmp[128];
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DRIVER_VER");
#endif
#ifdef LIB_VERSION
                sprintf(tmp,"00.00.0000 %s",LIB_VERSION);
#else
                sprintf(tmp,"00.00.0000");
#endif
                ood_bounded_strcpy(InfoValuePtr,tmp,BufferLength);
                if(StringLengthPtr)
                    *StringLengthPtr=strlen(InfoValuePtr);
            }
            break;

        case SQL_DROP_ASSERTION:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DROP_ASSERTION");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_DROP_CHARACTER_SET:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DROP_CHARACTER_SET");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_DROP_COLLATION:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DROP_COLLATION");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_DROP_DOMAIN:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DROP_DOMAIN");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_DROP_SCHEMA: /* drop user != drop schema */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DROP_SCHEMA");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_DROP_TABLE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DROP_TABLE");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_DT_DROP_TABLE|
                SQL_DT_CASCADE|
                SQL_DT_RESTRICT;
            break;

        case SQL_DROP_VIEW:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DROP_VIEW");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_DV_DROP_VIEW;
            break;

            /* ODBC Ref Vol2 Page 777 */
        case SQL_DYNAMIC_CURSOR_ATTRIBUTES1:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DYNAMIC_CURSOR_ATTRIBUTES1");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_DYNAMIC_CURSOR_ATTRIBUTES2:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_DYNAMIC_CURSOR_ATTRIBUTES2");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_EXPRESSIONS_IN_ORDERBY:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_EXPRESSIONS_IN_ORDERBY");
#endif
            strcpy(InfoValuePtr,"Y");
            break;

        case SQL_FILE_USAGE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_FILE_USAGE");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_FILE_NOT_SUPPORTED;
            break;

        case SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1: /*TODO*/
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1");
#endif
            *(SQLUINTEGER*)InfoValuePtr=SQL_CA1_NEXT;
            break;

        case SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
				SQL_CA2_READ_ONLY_CONCURRENCY|
				SQL_CA2_CRC_APPROXIMATE;
            break;

        case SQL_GETDATA_EXTENSIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_GETDATA_EXTENSIONS");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_GD_ANY_COLUMN|
                SQL_GD_ANY_ORDER|
                /*SQL_GD_BLOCK|*/ /*when implemented! */
                SQL_GD_BOUND;
            break;

        case SQL_GROUP_BY:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_GROUP_BY");
#endif
            *(SQLUSMALLINT*)InfoValuePtr=
                SQL_GB_GROUP_BY_EQUALS_SELECT;
            break;

        case SQL_IDENTIFIER_CASE: /* TODO check */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_IDENTIFIER_CASE");
#endif
            *(SQLUSMALLINT*)InfoValuePtr=SQL_IC_UPPER;
            break;

        case SQL_IDENTIFIER_QUOTE_CHAR:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_IDENTIFIER_QUOTE_CHAR");
#endif
            strcpy(InfoValuePtr,"\"");
            if(StringLengthPtr)
                *StringLengthPtr=1;
            break;

        case SQL_INDEX_KEYWORDS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_INDEX_KEYWORDS");
#endif
            *(SQLUINTEGER*)InfoValuePtr=SQL_IK_ALL;
            break;

        case SQL_INFO_SCHEMA_VIEWS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_INFO_SCHEMA_VIEWS");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_INSERT_STATEMENT:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_INSERT_STATEMENT");
#endif
            *(SQLUINTEGER*)InfoValuePtr=
                SQL_IS_INSERT_LITERALS|
                SQL_IS_INSERT_SEARCHED;
            break;

        case SQL_INTEGRITY:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_INTEGRITY");
#endif
            strcpy(InfoValuePtr,"Y");
            break;

        case SQL_KEYSET_CURSOR_ATTRIBUTES1: /* TODO :- implement this! */
        case SQL_KEYSET_CURSOR_ATTRIBUTES2:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_KEYSET_CURSOR_ATTRIBUTES2");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_KEYWORDS: /* TODO :- implement this! */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_KEYWORDS");
#endif
            *(SQLCHAR*)InfoValuePtr='\0';
            break;

        case SQL_LIKE_ESCAPE_CLAUSE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_LIKE_ESCAPE_CLAUSE");
#endif
            strcpy(InfoValuePtr,"Y");
            break;

        case SQL_MAX_ASYNC_CONCURRENT_STATEMENTS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_ASYNC_CONCURRENT_STATEMENTS");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_MAX_BINARY_LITERAL_LEN: /* TODO check */
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_BINARY_LITERAL_LEN");
#endif
            *(SQLUINTEGER*)InfoValuePtr=0;
            break;

        case SQL_MAX_CATALOG_NAME_LEN:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_CATALOG_NAME_LEN");
#endif
            *(SQLUSMALLINT*)InfoValuePtr=0;
            break;

        case SQL_MAX_CHAR_LITERAL_LEN: /* TODO guess */
        case SQL_MAX_COLUMN_NAME_LEN:
        case SQL_MAX_SCHEMA_NAME_LEN:
        case SQL_MAX_USER_NAME_LEN:
        case SQL_MAX_TABLE_NAME_LEN:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_TABLE_NAME_LEN");
#endif
            *(SQLUSMALLINT*)InfoValuePtr=30;
            break;

        case SQL_MAX_COLUMNS_IN_GROUP_BY:
        case SQL_MAX_COLUMNS_IN_ORDER_BY:
        case SQL_MAX_COLUMNS_IN_SELECT:
        case SQL_MAX_COLUMNS_IN_TABLE:
        case SQL_MAX_TABLES_IN_SELECT:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_TABLES_IN_SELECT");
#endif
            *(SQLUSMALLINT*)InfoValuePtr=1000;
            break;

        /* TODO :- the rest of these! - see the ODBC 3 Book Vol2 page 791 */
        case SQL_MAX_CONCURRENT_ACTIVITIES:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_CONCURRENT_ACTIVITIES");
		break;
#endif
        case SQL_MAX_CURSOR_NAME_LEN:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_CURSOR_NAME_LEN");
		break;
#endif
        case SQL_MAX_DRIVER_CONNECTIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_DRIVER_CONNECTIONS");
		break;
#endif
        case SQL_MAX_IDENTIFIER_LEN:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_IDENTIFIER_LEN");
		break;
#endif
        case SQL_MAX_INDEX_SIZE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_INDEX_SIZE");
		break;
#endif
        case SQL_MAX_PROCEDURE_NAME_LEN:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_PROCEDURE_NAME_LEN");
		break;
#endif
        case SQL_MAX_ROW_SIZE_INCLUDES_LONG:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_ROW_SIZE_INCLUDES_LONG");
		break;
#endif
        case SQL_MAX_STATEMENT_LEN:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MAX_STATEMENT_LEN");
		break;
#endif
        case SQL_MULT_RESULT_SETS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MULT_RESULT_SETS");
		break;
#endif
        case SQL_MULTIPLE_ACTIVE_TXN:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_MULTIPLE_ACTIVE_TXN");
		break;
#endif
        case SQL_NEED_LONG_DATA_LEN:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_NEED_LONG_DATA_LEN");
		break;
#endif
        case SQL_NON_NULLABLE_COLUMNS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_NON_NULLABLE_COLUMNS");
		break;
#endif
        case SQL_NULL_COLLATION:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_NULL_COLLATION");
		break;
#endif
        case SQL_NUMERIC_FUNCTIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_NUMERIC_FUNCTIONS");
		break;
#endif
        case SQL_ODBC_INTERFACE_CONFORMANCE:
		case SQL_ODBC_API_CONFORMANCE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_ODBC_INTERFACE_CONFORMANCE");
		break;
#endif
        case SQL_OJ_CAPABILITIES:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_OJ_CAPABILITIES");
		break;
#endif
        case SQL_ORDER_BY_COLUMNS_IN_SELECT:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_ORDER_BY_COLUMNS_IN_SELECT");
		break;
#endif
        case SQL_PARAM_ARRAY_ROW_COUNTS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_PARAM_ARRAY_ROW_COUNTS");
		break;
#endif
        case SQL_PARAM_ARRAY_SELECTS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_PARAM_ARRAY_SELECTS");
		break;
#endif
        case SQL_PROCEDURE_TERM:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_PROCEDURE_TERM");
		break;
#endif
        case SQL_PROCEDURES:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_PROCEDURES");
		break;
#endif
        case SQL_QUOTED_IDENTIFIER_CASE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_QUOTED_IDENTIFIER_CASE");
		break;
#endif
        case SQL_ROW_UPDATES:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_ROW_UPDATES");
		break;
#endif
        case SQL_SCHEMA_TERM:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SCHEMA_TERM");
		break;
#endif
        case SQL_SCHEMA_USAGE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SCHEMA_USAGE");
		break;
#endif
        case SQL_SCROLL_OPTIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SCROLL_OPTIONS");
		break;
#endif
        case SQL_SEARCH_PATTERN_ESCAPE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SEARCH_PATTERN_ESCAPE");
		break;
#endif
        case SQL_SERVER_NAME:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SERVER_NAME");
		break;
#endif
        case SQL_SPECIAL_CHARACTERS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SPECIAL_CHARACTERS");
		break;
#endif
        case SQL_SQL_CONFORMANCE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SQL_CONFORMANCE");
		break;
#endif
        case SQL_SQL92_DATETIME_FUNCTIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SQL92_DATETIME_FUNCTIONS");
		break;
#endif
        case SQL_SQL92_FOREIGN_KEY_DELETE_RULE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SQL92_FOREIGN_KEY_DELETE_RULE");
		break;
#endif
        case SQL_SQL92_GRANT:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SQL92_GRANT");
		break;
#endif
        case SQL_SQL92_NUMERIC_VALUE_FUNCTIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SQL92_NUMERIC_VALUE_FUNCTIONS");
		break;
#endif
        case SQL_SQL92_PREDICATES:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SQL92_PREDICATES");
		break;
#endif
        case SQL_SQL92_RELATIONAL_JOIN_OPERATORS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SQL92_RELATIONAL_JOIN_OPERATORS");
		break;
#endif
        case SQL_SQL92_REVOKE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SQL92_REVOKE");
		break;
#endif
        case SQL_SQL92_ROW_VALUE_CONSTRUCTOR:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SQL92_ROW_VALUE_CONSTRUCTOR");
		break;
#endif
        case SQL_SQL92_STRING_FUNCTIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SQL92_STRING_FUNCTIONS");
		break;
#endif
        case SQL_SQL92_VALUE_EXPRESSIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SQL92_VALUE_EXPRESSIONS");
		break;
#endif
        case SQL_STANDARD_CLI_CONFORMANCE:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_STANDARD_CLI_CONFORMANCE");
		break;
#endif
        case SQL_STATIC_CURSOR_ATTRIBUTES1:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_STATIC_CURSOR_ATTRIBUTES1");
		break;
#endif
        case SQL_STATIC_CURSOR_ATTRIBUTES2:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_STATIC_CURSOR_ATTRIBUTES2");
		break;
#endif
        case SQL_STRING_FUNCTIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_STRING_FUNCTIONS");
		break;
#endif
        case SQL_SUBQUERIES:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SUBQUERIES");
		break;
#endif
        case SQL_SYSTEM_FUNCTIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_SYSTEM_FUNCTIONS");
		break;
#endif
        case SQL_TABLE_TERM:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_TABLE_TERM");
		break;
#endif
        case SQL_TIMEDATE_ADD_INTERVALS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_TIMEDATE_ADD_INTERVALS");
		break;
#endif
        case SQL_TIMEDATE_DIFF_INTERVALS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_TIMEDATE_DIFF_INTERVALS");
		break;
#endif
        case SQL_TIMEDATE_FUNCTIONS:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_TIMEDATE_FUNCTIONS");
		break;
#endif


        case SQL_TXN_CAPABLE: 
	  *(SQLUSMALLINT*)InfoValuePtr=SQL_TXN_CAPABLE;
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_TXN_CAPABLE");
#endif	
	break;



        case SQL_TXN_ISOLATION_OPTION:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_TXN_ISOLATION_OPTION");
		break;
#endif
        case SQL_UNION:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_UNION");
		break;
#endif
        case SQL_USER_NAME:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_USER_NAME");
		break;
#endif
        case SQL_XOPEN_CLI_YEAR:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"SQL_XOPEN_CLI_YEAR");
		break;
#endif
        default:
#ifdef UNIX_DEBUG
        strcpy(inftxt,"default");
#endif
            *(SQLUSMALLINT*)InfoValuePtr=0;
			status=SQL_SUCCESS;
            break;
    }

#ifdef ENABLE_TRACE
#ifdef UNIX_DEBUG
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"is","*InfoValuePtr",
            *(SQLUINTEGER*)InfoValuePtr,
			"Info Type",inftxt);
#else
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"i","*InfoValuePtr",
            *(SQLUINTEGER*)InfoValuePtr);
#endif
#endif
    return status;
}
