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
 * $Id: common_functions.h,v 1.1 2002/02/11 19:48:07 dbox Exp $
 *
 * $Log: common_functions.h,v $
 * Revision 1.1  2002/02/11 19:48:07  dbox
 * Initial revision
 *
 * Revision 1.5  2000/05/11 13:17:32  tom
 * *** empty log message ***
 *
 * Revision 1.4  2000/05/04 14:56:32  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.3  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.2  2000/04/27 13:43:47  tom
 * Added #define to stop double inclusion
 *
 * Revision 1.1  2000/04/26 14:30:43  tom
 *
 * New generic functions
 *
 ******************************************************************************/
#ifndef _ORACOMMON_FUNCTIONS_H
#define _ORACOMMON_FUNCTIONS_H
#include <stdlib.h>

#define ORAFREE(aptr) do { if(aptr) { free (aptr); aptr=NULL; } } while(0)
#define ORAMALLOC(size)  ora_malloc(size)
#define ORAREALLOC(ptr, size) ora_realloc(ptr,size)

void* ora_malloc(size_t num);
void* ora_realloc(void *ptr, size_t size);

SQLINTEGER sqltype_display_size(SQLSMALLINT,int);
void ood_mutex_lock_stmt(hStmt_T *stmt);
void ood_mutex_unlock_stmt(hStmt_T *stmt);
int ood_mutex_init(hgeneric* handle);
int ood_mutex_destroy(hgeneric* handle);
sword ood_alt_fetch_no_data(hStmt_T* stmt);
#ifdef ENABLE_TRACE
void ood_log_message(hDbc_T* dbc,char *file,int line, int mask, 
		SQLHANDLE handle, SQLRETURN ret, char *fmt, ... );
#endif
#endif
