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
 * $Id: SQLDescribeParam.c,v 1.2 2002/05/14 23:01:05 dbox Exp $
 *
 * $Log: SQLDescribeParam.c,v $
 * Revision 1.2  2002/05/14 23:01:05  dbox
 * added a bunch of error checking and some 'constructors' for the
 * environment handles
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.9  2000/06/06 10:24:14  tom
 * Initial Implementation
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
 * Revision 1.2  2000/04/19 15:24:31  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLDescribeParam.c,v $ $Revision: 1.2 $";

SQLRETURN SQL_API SQLDescribeParam(
    SQLHSTMT            StatementHandle,
    SQLUSMALLINT        ParameterNumber,
    SQLSMALLINT            *DataTypePtr,
    SQLUINTEGER            *ParameterSizePtr,
    SQLSMALLINT            *DecimalDigitsPtr,
    SQLSMALLINT            *NullablePtr )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
	ap_T *ap;
  
	assert(IS_VALID(stmt));

#ifdef ENABLE_TRACE
    SQLRETURN status=SQL_SUCCESS;
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"");
#endif

    ood_clear_diag((hgeneric*)stmt);
    ood_mutex_lock_stmt(stmt);

	if(ParameterNumber>stmt->current_ap->num_recs)
	{
		ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_07009,ParameterNumber,
                "",ERROR_MESSAGE_07009,
                __LINE__,0,"",ERROR_STATE_07009,__FILE__,__LINE__);
        ood_mutex_unlock_stmt(stmt);
#ifdef ENABLE_TRACE
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                (SQLHANDLE)NULL,SQL_ERROR,"");
#endif
		return SQL_ERROR;
	}
	assert(IS_VALID(stmt->current_ap));
	ap=&stmt->current_ap->recs.ap[ParameterNumber];
	assert(IS_VALID(ap));
	if(!ap)
	  return SQL_ERROR;

	if(DataTypePtr){
	  if(!ap->concise_type)
	    return SQL_ERROR;
	  *DataTypePtr=ap->concise_type;
	}
	if(ParameterSizePtr)
		*ParameterSizePtr=ap->length;

	if(DecimalDigitsPtr)
		*DecimalDigitsPtr=ap->precision;

	if(NullablePtr)
		*NullablePtr=SQL_NULLABLE_UNKNOWN;

    ood_mutex_unlock_stmt(stmt);

#ifdef ENABLE_TRACE
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
#endif
    return SQL_SUCCESS;
}
