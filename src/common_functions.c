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
 * $Id: common_functions.c,v 1.2 2002/05/16 19:27:26 dbox Exp $
 *
 * $Log: common_functions.c,v $
 * Revision 1.2  2002/05/16 19:27:26  dbox
 * new test to beat the crap out of SQLGetInfo.c
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.10  2000/07/20 08:18:20  tom
 * *** empty log message ***
 *
 * Revision 1.9  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.8  2000/06/05 15:49:29  tom
 * Deadlock warning debugging code
 *
 * Revision 1.7  2000/05/11 13:17:32  tom
 * *** empty log message ***
 *
 * Revision 1.6  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.5  2000/05/04 14:56:32  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.4  2000/05/03 16:01:44  tom
 * Tracing functions added
 *
 * Revision 1.3  2000/05/02 16:24:58  tom
 * resolved deadlock and a few tidy-ups
 *
 * Revision 1.2  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.1  2000/04/26 14:30:43  tom
 *
 * New generic functions
 *
 ******************************************************************************/

static char const rcsid[]= "$RCSfile: common_functions.c,v $ $Revision: 1.2 $";

/*
 * SQL type display size
 *
 * returns the diplay size of a column based on the SQL type and the data
 * length
 */
#include "common.h"
const char * oci_var_type(SQLSMALLINT sqltype)
{
  switch(sqltype){
  case SQLT_CHR: return "SQLT_CHR"; 
  case SQLT_NUM: return "SQLT_NUM"; 
  case SQLT_INT: return "SQLT_INT"; 
  case SQLT_FLT: return "SQLT_FLT"; 
  case SQLT_STR: return "SQLT_STR"; 
  case SQLT_VNU: return "SQLT_VNU"; 
  case SQLT_PDN: return "SQLT_PDN"; 
  case SQLT_LNG: return "SQLT_LNG"; 
  case SQLT_VCS: return "SQLT_VCS"; 
  case SQLT_NON: return "SQLT_NON"; 
  case SQLT_RID: return "SQLT_RID"; 
  case SQLT_DAT: return "SQLT_DAT"; 
  case SQLT_VBI: return "SQLT_VBI"; 
  case SQLT_BIN: return "SQLT_BIN"; 
  case SQLT_LBI: return "SQLT_LBI"; 
  case SQLT_UIN: return "SQLT_UIN"; 
  case SQLT_SLS: return "SQLT_SLS"; 
  case SQLT_LVC: return "SQLT_LVC"; 
  case SQLT_LVB: return "SQLT_LVB"; 
  case SQLT_AFC: return "SQLT_AFC"; 
  case SQLT_AVC: return "SQLT_AVC"; 
  case SQLT_CUR: return "SQLT_CUR"; 
  case SQLT_RDD: return "SQLT_RDD"; 
  case SQLT_LAB: return "SQLT_LAB"; 
  case SQLT_OSL: return "SQLT_OSL"; 
  case SQLT_NTY: return "SQLT_NTY"; 
  case SQLT_REF: return "SQLT_REF"; 
  case SQLT_CLOB: return "SQLT_CLOB"; 
  case SQLT_BLOB: return "SQLT_BLOB"; 
  case SQLT_BFILEE: return "SQLT_BFILEE"; 
  case SQLT_CFILEE: return "SQLT_CFILEE"; 
  case SQLT_RSET: return "SQLT_RSET"; 
  case SQLT_NCO: return "SQLT_NCO"; 
  case SQLT_VST: return "SQLT_VST"; 
  case SQLT_ODT: return "SQLT_ODT"; 
    /* case SQLT_FILE: return "SQLT_FILE"; */
    /*case SQLT_CFILE: return "SQLT_CFILE"; */
    /*case SQLT_BFILE: return "SQLT_BFILE"; */
  default: return "UNKNOWN_OCI_TYPE";
}
}


const char * odbc_var_type(SQLSMALLINT sqltype)
{
  switch(sqltype){
  case SQL_UNKNOWN_TYPE: return "SQL_UNKNOWN_TYPE";        
  case SQL_CHAR    : return "SQL_CHAR";         
  case SQL_NUMERIC  : return "SQL_NUMERIC";        
  case SQL_DECIMAL  : return "SQL_DECIMAL";        
  case SQL_INTEGER   : return "SQL_INTEGER";       
  case SQL_SMALLINT  : return "SQL_SMALLINT";       
  case SQL_FLOAT    : return "SQL_FLOAT";        
  case SQL_REAL     : return "SQL_REAL";        
  case SQL_DOUBLE    : return "SQL_DOUBLE";       
  case SQL_DATETIME   : return "SQL_DATETIME";      
  case SQL_VARCHAR     : return "SQL_VARCHAR";
  default: return "OUT_OF_RANGE_NO_IDEA"; 
  }
}





SQLINTEGER sqltype_display_size(SQLSMALLINT sqltype,int length)
{
    switch(sqltype)
    {
        case SQL_C_CHAR:
            return length-1;

        case SQL_BIT:
            return 1;

        case SQL_TINYINT:
            return 4;

        case SQL_SMALLINT:
        case SQL_C_SSHORT:
            return 6;
        case SQL_C_USHORT:
            return 5;

        case SQL_INTEGER:
        case SQL_C_SLONG:
            return 11;
        case SQL_C_ULONG:
            return 10;

        case SQL_BIGINT:
            return 20;

        case SQL_REAL:
            return 13;

        case SQL_FLOAT:
        case SQL_DOUBLE:
            return 23;

        case SQL_TYPE_DATE:
        case SQL_C_DATE:
            return 10;

        case SQL_TYPE_TIME:
        case SQL_C_TIME:
            return 8;

        case SQL_TYPE_TIMESTAMP:
        case SQL_C_TIMESTAMP:
            return 21;

        default :
            return length;
    }
}

/*
 * Pthread lock statement
 *
 * Locks the statement handle and the four current descriptors
 */
void ood_mutex_lock_stmt(hStmt_T *stmt)
{
    THREAD_MUTEX_LOCK(stmt);
    THREAD_MUTEX_LOCK(stmt->current_ap);
    THREAD_MUTEX_LOCK(stmt->current_ip);
    THREAD_MUTEX_LOCK(stmt->current_ar);
    THREAD_MUTEX_LOCK(stmt->current_ir);
}

/* thread unlock statement
 *
 * unlocks the statement and the four current descriptors
 */
void ood_mutex_unlock_stmt(hStmt_T *stmt)
{
    THREAD_MUTEX_UNLOCK(stmt->current_ap);
    THREAD_MUTEX_UNLOCK(stmt->current_ip);
    THREAD_MUTEX_UNLOCK(stmt->current_ar);
    THREAD_MUTEX_UNLOCK(stmt->current_ir);
    THREAD_MUTEX_UNLOCK(stmt);
}

/* thread mutex init
 *
 * Initialise thread
 */

int ood_mutex_init(hgeneric* handle)
{
#if defined(HAVE_LIBPTHREAD)
    return pthread_mutex_init(HANDLE_MUTEX_PTR(handle),NULL);
#elif defined(WIN32)
    return (int) (HANDLE_MUTEX(handle)=(int*)CreateMutex(NULL,FALSE,NULL));
#else
    return 1;
#endif
}

int ood_mutex_destroy(hgeneric* handle)
{
#if defined(HAVE_LIBPTHREAD)
    return pthread_mutex_destroy(HANDLE_MUTEX_PTR(handle));
#elif defined(WIN32)
    return ReleaseMutex(HANDLE_MUTEX_PTR(handle));
#else
    return 1;
#endif
}

/*
 * Altertate fetch :- fetch no data
 */
sword ood_alt_fetch_no_data(hStmt_T* stmt)
{
	return OCI_NO_DATA;
}


#ifdef ENABLE_TRACE
/*
 * _log_message
 *
 * The nitty gritty of log_message
 */
static void _log_message_imp(char *tracefilename,char *file,int line, 
        int mask, SQLHANDLE handle, SQLRETURN ret, char* fmt, va_list ap)
{
    FILE *tracefile;
    char *left; /*left hand side of each format pair */
    if(NULL==(tracefile=fopen(tracefilename,"a")))
    {
        return;
    }
#if !defined(WIN32)
    fprintf(tracefile, "%s[%d]%s[%04d]\n",ERROR_PRODUCT,
            (int)getpid(),file,line);
#else
    fprintf(tracefile, "%s-%s[%04d]\n",ERROR_PRODUCT,
            file,line);
#endif

    if(mask==TRACE_FUNCTION_ENTRY)
    {
        fputs("                Entry:",tracefile);
        switch(HANDLE_TYPE(handle))
        {
            case SQL_HANDLE_STMT:
            fputs("[SQL_HANDLE_STMT]",tracefile);
            break;

            case SQL_HANDLE_DBC:
            fputs("[SQL_HANDLE_DBC]",tracefile);
            break;

            case SQL_HANDLE_DESC:
            fputs("[SQL_HANDLE_DESC]",tracefile);
            break;

            case SQL_HANDLE_ENV:
            fputs("[SQL_HANDLE_ENV]",tracefile);
            break;
            
            default:
            fprintf(tracefile,"[UNKOWN=%d]",HANDLE_TYPE(handle));
            break;
        }
        fprintf(tracefile,"[0x%.8lx]\n",(long)handle);
    }
    else if(mask==TRACE_FUNCTION_EXIT)
    {
        fprintf(tracefile,"                Exit: ");
        switch(ret)
        {
            case SQL_SUCCESS:
            fputs("[SQL_SUCCESS]\n",tracefile);
            break;

            case SQL_NO_DATA:
            fputs("[SQL_NO_DATA]\n",tracefile);
            break;

            case SQL_ERROR:
            fputs( "[SQL_ERROR]\n",tracefile);
            break;

            case SQL_INVALID_HANDLE:
            fputs("[SQL_INVALID_HANDLE]\n",tracefile);
            break;

            case SQL_STILL_EXECUTING:
            fputs("[SQL_STILL_EXECUTING]\n",tracefile);
            break;

            case SQL_NEED_DATA:
            fputs("[SQL_NEED_DATA]\n",tracefile);
            break;

            case SQL_SUCCESS_WITH_INFO:
            fputs("[SQL_SUCESS_WITH_INFO]\n",tracefile);
            break;

            default:
            fprintf(tracefile, "[UNKNOWN=%d]", ret );
            break;
        }
    }
    else
    {
        fputs("                Unknown:\n",tracefile);
    }

    if ( !fmt )
    {
        fputs("<NULL FORMAT>\n",tracefile);
        return;
    }

    while ( *fmt )
    {
        left=va_arg(ap,char *);
        if(left)
            fprintf(tracefile,"                %s = ",left);
        else
            fprintf(tracefile,"                     ");
        switch ( *fmt )
        {
              case 'h':            /* a handle */
              case 'p':            /* a pointer */
              {
                void *h = va_arg( ap, void * );
                fprintf(tracefile, "%p\n", h );
                break;
              }
    
              case 's':            /* C string */
              {
                char * ptr = va_arg( ap, char * );

                if ( ptr )
                {
                    fprintf(tracefile, "\"%s\"\n", ptr );
                }
                else
                {
                    fprintf(tracefile, "<NULLPTR>\n" );
                }
                break;
              }

              case 'I':            /* a short */
              {
              short i = va_arg( ap, short );

                fprintf(tracefile, "%d\n", i );
                break;
              }

              case 'U':            /* a unsigned short */
              {
              unsigned short i = va_arg( ap, unsigned short );

                fprintf(tracefile, "%u\n", i );
                break;
              }

              case 'i':            /* a int */
              {
                int i = va_arg( ap, int );

                fprintf(tracefile, "%d\n", i );
                break;
              }

              case 'u':            /* a unsigned int */
              {
                unsigned int i = va_arg( ap, unsigned int );

                fprintf(tracefile, "%u\n", i );
                break;
              }
        }/*switch(*fmt)*/

        if(*fmt)
            fmt++;
    }/*while(*fmt)*/
    fputc('\n',tracefile);
    fclose(tracefile);
}

/*
 * log_message
 *
 * Make a trace entry. This fn is deliberately small to try to keep performance
 * up when tracing is off.
 *
 * Operation is a little odd. There is a format string much like printf,
 * but each format specifier represents a pair of arguments, ie
 * ood_log_message(..."sh","SQL",stmt->sqltext,"Statement",stmt);
 * would produce
 *            SQL = SELECT * FROM ....
 *            Statement = 0x43565432
 *
 *  Format specifiers are...
 *
 * h            Print a handle.
 * p            Print a pointer.
 * i            Print a small integer.
 * u            Print a unsigned small integer.
 * I            Print a integer.
 * U            Print a unsigned integer.
 * s            A null terminated string.
 */
void ood_log_message(hDbc_T* dbc,char *file,int line, int mask, SQLHANDLE handle,
        SQLRETURN ret, char *fmt, ... )
{
    va_list ap;
	/*
	 * This is a common place for deadlocks...
	 */
#ifdef UNIX_DEBUG
	int i=0;
	while(EBUSY==pthread_mutex_trylock( &((hgeneric*)dbc)->mutex ))
	{
		fprintf(stderr,"ood_log_message -> dbc [0x%.8lx] on pid %d locked\n",
				(long)dbc,getpid());
		sleep(1);
		if(i++>10)
			abort();
	}
#else
    THREAD_MUTEX_LOCK(dbc);
#endif
    if(dbc->trace!=SQL_OPT_TRACE_ON)
    {
        THREAD_MUTEX_UNLOCK(dbc);
        return;
    }
    va_start( ap, fmt );
    _log_message_imp((char*)dbc->tracefile,file,line,mask,handle,ret,fmt,ap);
    va_end( ap );
    THREAD_MUTEX_UNLOCK(dbc);
    return;
}

void* ora_malloc(size_t size)
{
  void * ptr = malloc(size);
  if(ptr==NULL){
    fprintf(stderr,"Out Of Memory Error line %d file %s\n",__LINE__,__FILE__);
    exit(-1);
  }
  return ptr;
}

void* ora_realloc(void *ptr, size_t size);
{  
  void * ptr2 = realloc(ptr,size);
  if(ptr==NULL){
    fprintf(stderr,"Out Of Memory Error line %d file %s\n",__LINE__,__FILE__);
    exit(-1);
  }
  return ptr2;
}

#endif
