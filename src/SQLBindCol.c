/********************************************************************************
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
 * $Id: SQLBindCol.c,v 1.1 2002/02/11 19:48:06 dbox Exp $
 *
 * $Log: SQLBindCol.c,v $
 * Revision 1.1  2002/02/11 19:48:06  dbox
 * Initial revision
 *
 * Revision 1.15  2000/07/21 10:01:54  tom
 * Added LOB flag
 *
 * Revision 1.14  2000/06/05 15:56:44  tom
 * Changes due to initial bound params implementation
 *
 * Revision 1.13  2000/05/23 16:04:56  tom
 * Fixed possible data type return problem
 *
 * Revision 1.12  2000/05/17 15:17:08  tom
 * Made slightly more robust
 *
 * Revision 1.11  2000/05/15 08:58:37  tom
 * Release 0.0.2
 *
 * Revision 1.10  2000/05/10 12:42:44  tom
 * Various updates
 *
 * Revision 1.9  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.8  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.7  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.6  2000/04/28 08:39:45  tom
 * Tidy up
 *
 * Revision 1.5  2000/04/26 15:27:09  tom
 *
 * Now updates the default copy fn in the IR
 *
 * Revision 1.4  2000/04/26 10:12:58  tom
 *
 *
 * First functional implementation
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 *
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:23:21  tom
 *
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 *
 * Added files
 *
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLBindCol.c,v $ $Revision: 1.1 $";

SQLRETURN SQL_API SQLBindCol(
    SQLHSTMT        StatementHandle,
    SQLUSMALLINT    ColumnNumber,
    SQLSMALLINT        TargetType,
    SQLPOINTER        TargetValuePtr,
    SQLINTEGER        BufferLength,
    SQLINTEGER        *StrLen_or_IndPtr )
{
    hStmt_T *stmt=(hStmt_T*)StatementHandle;
    SQLRETURN status=SQL_SUCCESS;

#ifdef ENABLE_TRACE
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)StatementHandle,0,"iii",
			"ColumnNumber",ColumnNumber,
			"TargetType",TargetType,
			"BufferLength",BufferLength);
#endif
    ood_clear_diag((hgeneric*)(hgeneric*)stmt);
    ood_mutex_lock_stmt(stmt);

    /*
     * We may not have allocated our row descriptors yet... so make sure
     */
    if(SQL_SUCCESS!=ood_alloc_col_desc(stmt,ColumnNumber,
                stmt->current_ir,stmt->current_ar))
    {
        status=SQL_ERROR;
    }
    else
    {

        /*
         * Common stuff
         */
        stmt->current_ar->recs.ar[ColumnNumber].concise_type=stmt->current_ar->recs.ar[ColumnNumber].data_type=TargetType==SQL_C_DEFAULT?SQL_C_CHAR:TargetType;
        stmt->current_ar->recs.ar[ColumnNumber].bind_indicator=StrLen_or_IndPtr;
        stmt->current_ar->recs.ar[ColumnNumber].buffer_length=BufferLength;
        stmt->current_ar->recs.ar[ColumnNumber].data_ptr=TargetValuePtr;
        stmt->current_ir->recs.ir[ColumnNumber].default_copy=
			ood_fn_default_copy(
					stmt->current_ir->recs.ir[ColumnNumber].data_type,
					TargetType);
        stmt->current_ar->bound_col_flag++;
        stmt->current_ar->lob_col_flag++;
    }
#ifdef ENABLE_TRACE
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
#endif
    ood_mutex_unlock_stmt(stmt);
#ifdef UNIX_DEBUG
    for(ColumnNumber=1;ColumnNumber<=stmt->current_ir->num_recs;ColumnNumber++)
    {
        fprintf(stderr,"%s %d col num [%d] fn[%.8lx]\n",
                __FILE__,__LINE__,
                stmt->current_ir->recs.ir[ColumnNumber].col_num,
                (long)stmt->current_ir->recs.ir[ColumnNumber].default_copy);
    }
#endif
    return SQL_SUCCESS;
}
