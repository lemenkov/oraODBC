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
 * $Id: SQLSetStmtAttr.c,v 1.1 2002/02/11 19:48:07 dbox Exp $
 *
 * $Log: SQLSetStmtAttr.c,v $
 * Revision 1.1  2002/02/11 19:48:07  dbox
 * Initial revision
 *
 * Revision 1.12  2000/07/21 10:14:09  tom
 * Lots of logging added for debug, SQL_ATTR_QUERY_TIMEOUT added
 *
 * Revision 1.11  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.10  2000/07/07 08:11:05  tom
 * inital implementation
 *
 * Revision 1.9  2000/06/06 10:23:41  tom
 * Tidy up for possible release 0.0.3
 *
 * Revision 1.7  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.6  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.5  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.4  2000/04/26 10:18:45  tom
 * removed (currently) unneccessary function stub
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:29:17  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLSetStmtAttr.c,v $ $Revision: 1.1 $";


SQLRETURN SQL_API SQLSetStmtAttr(
    SQLHSTMT              StatementHandle,
    SQLINTEGER            Attribute,
    SQLPOINTER            ValuePtr,
    SQLINTEGER            StringLength )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    SQLRETURN status=SQL_SUCCESS;
#ifdef ENABLE_TRACE
    ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"pii",
			"Attribute",Attribute,
			"ValuePtr",ValuePtr,
			"StringLength",StringLength);
#endif
    ood_clear_diag((hgeneric*)stmt);
    ood_mutex_lock_stmt(stmt);

#ifdef UNIX_DEBUG
	signal(SIGSEGV,SIG_DFL);
#endif

	switch(Attribute)
	{
        case SQL_ATTR_METADATA_ID:
#ifdef UNIX_DEBUG
			ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
					(SQLHANDLE)stmt,0,"p",
					"SQL_ATTR_METADATA_ID:",(long)ValuePtr);
#endif
        THREAD_MUTEX_LOCK(stmt->dbc);
        stmt->dbc->metadata_id=(SQLUINTEGER)ValuePtr;
        THREAD_MUTEX_UNLOCK(stmt->dbc);
		break;

		case SQL_ATTR_APP_ROW_DESC:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_APP_ROW_DESC:",(long)ValuePtr);
#endif
        stmt->current_ip=*((SQLPOINTER*)ValuePtr);
		break;

		case SQL_ATTR_APP_PARAM_DESC:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_APP_PARAM_DESC:",(long)ValuePtr);
#endif
        stmt->current_ip=*((SQLPOINTER*)ValuePtr);
		break;

        case SQL_ATTR_IMP_PARAM_DESC:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_IMP_PARAM_DESC:",(long)ValuePtr);
#endif
        stmt->current_ip=*((SQLPOINTER*)ValuePtr);
		break;

        case SQL_ATTR_IMP_ROW_DESC:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_IMP_ROW_DESC:",(long)ValuePtr);
#endif
        stmt->current_ip=*((SQLPOINTER*)ValuePtr);
		break;

		case SQL_ATTR_QUERY_TIMEOUT:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_IMP_ROW_DESC:",(long)ValuePtr);
#endif
		stmt->query_timeout=(SQLUINTEGER)ValuePtr;
		break;



        case SQL_ATTR_PARAM_BIND_OFFSET_PTR:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_PARAM_BIND_OFFSET_PTR:",(long)ValuePtr);
#endif
        stmt->param_bind_offset_ptr=*((SQLPOINTER*)ValuePtr);
	    break;

        case SQL_ATTR_ROW_BIND_OFFSET_PTR:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_ROW_BIND_OFFSET_PTR:",(long)ValuePtr);
#endif
        stmt->row_bind_offset_ptr=*((SQLPOINTER*)ValuePtr);
		break;

        case SQL_ATTR_ROW_ARRAY_SIZE:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_ROW_ARRAY_SIZE:",(long)ValuePtr);
#endif
        stmt->row_array_size=(SQLINTEGER)ValuePtr;
		break;

        case SQL_ATTR_ROW_BIND_TYPE:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_ROW_BIND_TYPE:",*((int*)ValuePtr));
#endif
           if((SQLINTEGER)ValuePtr==SQL_BIND_BY_COLUMN)
		        stmt->row_bind_type=(SQLINTEGER)ValuePtr;
			else
			{
		        ood_post_diag((hgeneric*)stmt->dbc,ERROR_ORIGIN_IM001,0,"",
                        ERROR_MESSAGE_IM001,
                        __LINE__,0,"",ERROR_STATE_IM001,
                        "-"__FILE__,__LINE__);
                status=SQL_ERROR;
			}
		break;

        case SQL_ATTR_ROW_STATUS_PTR:
        stmt->row_status_ptr=(SQLUSMALLINT*)ValuePtr;
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"pi",
			"SQL_ATTR_ROW_STATUS_PTR:",(long)ValuePtr,
			"StringLength",StringLength);
#endif
        break;

        case SQL_ATTR_ROWS_FETCHED_PTR:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_ROWS_FETCHED_PTR:",(long)ValuePtr);
#endif
        stmt->rows_fetched_ptr=(SQLUINTEGER*)ValuePtr;
		break;

        case SQL_ATTR_PARAM_BIND_TYPE:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_PARAM_BIND_TYPE:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_PARAM_OPERATION_PTR:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_PARAM_OPERATION_PTR:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_PARAMS_PROCESSED_PTR:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_PARAMS_PROCESSED_PTR:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_PARAMSET_SIZE:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_PARAMSET_SIZE:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_ROW_OPERATION_PTR:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_ROW_OPERATION_PTR:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_PARAM_STATUS_PTR:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_PARAM_STATUS_PTR:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_ASYNC_ENABLE:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_ASYNC_ENABLE:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_CONCURRENCY:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_CONCURRENCY:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_CURSOR_SCROLLABLE:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_CURSOR_SCROLLABLE:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_CURSOR_SENSITIVITY:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_CURSOR_SENSITIVITY:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_CURSOR_TYPE:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_CURSOR_TYPE:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_ENABLE_AUTO_IPD:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_ENABLE_AUTO_IPD:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_FETCH_BOOKMARK_PTR:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_FETCH_BOOKMARK_PTR:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_KEYSET_SIZE:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_KEYSET_SIZE:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_MAX_LENGTH:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_MAX_LENGTH:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_MAX_ROWS:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_MAX_ROWS:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_NOSCAN:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_NOSCAN:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_SIMULATE_CURSOR:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_SIMULATE_CURSOR:",(long)ValuePtr);
#endif
            /*break;*/

        case SQL_ATTR_USE_BOOKMARKS:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"SQL_ATTR_USE_BOOKMARKS:",(long)ValuePtr);
#endif
            /*break;*/

		default:
#ifdef UNIX_DEBUG
		ood_log_message(stmt->dbc,"-"__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			(SQLHANDLE)stmt,0,"p",
			"default:",(long)ValuePtr);
#endif
		ood_post_diag((hgeneric*)stmt->dbc,ERROR_ORIGIN_IM001,0,"",
                ERROR_MESSAGE_IM001,
                __LINE__,0,"",ERROR_STATE_IM001,
                "-"__FILE__,__LINE__);
        status=SQL_ERROR;
	}

    ood_mutex_unlock_stmt(stmt);
#ifdef ENABLE_TRACE
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
#endif
    return status;
}
