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
 * $Id: SQLDisconnect.c,v 1.2 2002/05/14 23:01:05 dbox Exp $
 *
 * $Log: SQLDisconnect.c,v $
 * Revision 1.2  2002/05/14 23:01:05  dbox
 * added a bunch of error checking and some 'constructors' for the
 * environment handles
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.12  2000/05/23 09:18:42  tom
 * Fixed double disconnect
 *
 * Revision 1.11  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.10  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.9  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.8  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.7  2000/04/28 08:39:45  tom
 * Tidy up
 *
 * Revision 1.6  2000/04/26 15:28:33  tom
 * Changed to account for new handle definitions
 *
 * Revision 1.5  2000/04/26 10:06:28  tom
 * Connection handle ORAFREE implemented fully
 *
 * Revision 1.4  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.3  2000/04/19 15:24:36  tom
 * First functional checkin
 *
 * Revision 1.2  2000/04/13 15:34:00  tom
 * Broke out Oracle specific functions
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLDisconnect.c,v $ $Revision: 1.2 $";

SQLRETURN SQL_API SQLDisconnect(
    SQLHDBC        ConnectionHandle )
{
    hDbc_T *dbc=(hDbc_T*)ConnectionHandle;
    SQLRETURN status;
    assert(IS_VALID(dbc));
    if(!dbc || HANDLE_TYPE(dbc)!=SQL_HANDLE_DBC)
        return SQL_INVALID_HANDLE;
#ifdef ENABLE_TRACE
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)dbc,0,"");
#endif
    ood_clear_diag((hgeneric*)dbc);

    THREAD_MUTEX_LOCK(dbc);
    status=ood_driver_disconnect(dbc);
    THREAD_MUTEX_UNLOCK(dbc);
    return status;
}
