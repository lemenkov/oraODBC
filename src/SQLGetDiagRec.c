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
 ******************************************************************************
 *
 * $Id: SQLGetDiagRec.c,v 1.2 2002/05/14 23:01:06 dbox Exp $
 *
 * $Log: SQLGetDiagRec.c,v $
 * Revision 1.2  2002/05/14 23:01:06  dbox
 * added a bunch of error checking and some 'constructors' for the
 * environment handles
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.7  2000/07/21 10:10:19  tom
 * logging increased, diags now cleared more aggressively
 *
 * Revision 1.6  2000/05/04 15:53:44  tom
 * fixed a cut-and-paste error
 *
 * Revision 1.5  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.4  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.3  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.2  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

static char const rcsid[]= "$RCSfile: SQLGetDiagRec.c,v $ $Revision: 1.2 $";

#include "common.h"

/*
 * Please note this in implemented in diagnostics.h
 */

SQLRETURN SQL_API SQLGetDiagRec(
    SQLSMALLINT            HandleType,
    SQLHANDLE            Handle,
    SQLSMALLINT            RecNumber,
    SQLCHAR                *SqlState,
    SQLINTEGER            *NativeErrorPtr,
    SQLCHAR                *MessageText,
    SQLSMALLINT            BufferLength,
    SQLSMALLINT            *TextLengthPtr )
{
    SQLRETURN status=SQL_SUCCESS;
#ifdef ENABLE_TRACE
    hDbc_T* dbc=NULL;
    switch(HANDLE_TYPE(Handle))
    {
        case SQL_HANDLE_DBC:
            dbc=(hDbc_T*)Handle;
            break;
        case SQL_HANDLE_STMT:
            dbc=((hStmt_T*)Handle)->dbc;
            break;
        case SQL_HANDLE_DESC:
            dbc=((hDesc_T*)Handle)->dbc;
            break;
    }
    if(dbc){
        ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)Handle,0,"II",
			"RecNumber",RecNumber,
			"BufferLength,",BufferLength);
	assert(IS_VALID(dbc));
    }
#endif
    status=_SQLGetDiagRec( HandleType, Handle, RecNumber, SqlState,
            NativeErrorPtr, MessageText, BufferLength, TextLengthPtr );
#ifdef ENABLE_TRACE
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
#endif
	ood_clear_diag((hgeneric*)Handle);
    return status;
}
