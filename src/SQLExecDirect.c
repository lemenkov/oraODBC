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
 * $Id: SQLExecDirect.c,v 1.4 2004/05/05 18:08:39 dbox Exp $
 *
 * $Log: SQLExecDirect.c,v $
 * Revision 1.4  2004/05/05 18:08:39  dbox
 * change return status of where 1=0 from SQL_NO_DATA to SQL_SUCCESS. The NO_DATA return value leads to table not found errors in Dbi.  The correct thing is to find the problem in Dbi, but minos needs this to work now so consider this a band-aid
 *
 * Revision 1.3  2004/05/04 22:41:51  dbox
 * fixed error return code for bad select statement
 *
 * Revision 1.2  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.10  2000/07/21 10:06:10  tom
 * Fixed prefetch assignment
 *
 * Revision 1.9  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.8  2000/07/07 08:01:32  tom
 * heavier tracing and debugging
 *
 * Revision 1.7  2000/06/05 16:00:19  tom
 * Added SQL parse stage
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
 * Revision 1.2  2000/04/19 15:24:50  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLExecDirect.c,v $ $Revision: 1.4 $";

SQLRETURN SQL_API SQLExecDirect(
    SQLHSTMT        StatementHandle,
    SQLCHAR            *StatementText,
    SQLINTEGER        TextLength )
{
    hStmt_T *stmt=(hStmt_T*)StatementHandle;
    SQLRETURN status;
    SQLRETURN t_stat;

    if(!stmt||HANDLE_TYPE(stmt)!=SQL_HANDLE_STMT)
        return SQL_INVALID_HANDLE;

if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"si",
			"SQL",StatementText,
			"Length",TextLength);
}

    ood_clear_diag((hgeneric*)stmt);

	stmt->sql=ood_lex_parse((char*)StatementText,
			TextLength,(int*)&stmt->current_ap->num_recs);

    ood_mutex_lock_stmt(stmt);

    status=ood_driver_prepare(stmt,(unsigned char*)stmt->sql);

#if defined(ENABLE_TRACE) && defined (UNIX_DEBUG)
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)stmt,status,"s",
			"Status Report",stmt->sql);
#endif

    status|=ood_driver_execute(stmt);
#if defined(ENABLE_TRACE) && defined (UNIX_DEBUG)
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)stmt,status,"",
			"Status Report",stmt->sql);
#endif

    status|=ood_driver_execute_describe(stmt);

	if(stmt->stmt_type==OCI_STMT_SELECT){
		t_stat=stmt->fetch_status=ood_driver_prefetch(stmt);
		if(t_stat==SQL_ERROR)
			status=t_stat;
	}

    ood_mutex_unlock_stmt(stmt);

if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return status;
}
