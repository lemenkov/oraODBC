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
		   Dennis Box (Fermi Nat Accelerator Lab)
                   Stefan.Radman@ctbto.org

		   *
 *******************************************************************************
 *
 * $Id: oracle_functions.c,v 1.20 2003/01/30 21:01:37 dbox Exp $
 * NOTE
 * There is no mutexing in these functions, it is assumed that the mutexing 
 * will be done at a higher level
 ******************************************************************************/

#include "common.h"
#include "ocitrace.h"
#include <sqlext.h>

static char const rcsid[]= "$RCSfile: oracle_functions.c,v $ $Revision: 1.20 $";

/*
 * There is a problem with a lot of libclntsh.so releases... an undefined
 * symbol slpmprodstab. This little hack gets around that, but beware it
 * is not a safe thing to do!
 */
#ifdef EXPORT_SLPMPRODSTAB
int slpmprodstab;
#endif

int epc_exit_handler()
{
  /* in oracle 8.1.6 and 8.1.7 libclntsh.so registers an
   * exit handler function  epc_exit_handler when it is 
   * dynamically loaded  (like an ODBC driver 
   * might do, for instance) but fails to 
   * un-register it when libclntsh.so is unloaded.  This causes a
   * core dump unless this function stub is here to be called.
   *
   * setting DEBUG or VERBOSE env variables allows  it to gloat.
   *
   */
  
  if(debugLevel()>0 || getenv("VERBOSE"))
    printf("neener neener caught the exit handler bug!\n");
  return 0;
}

#ifdef UNIX_DEBUG
/*
 * errcheck
 *
 * This is a debugging function that prints the return code and any error
 * from an OCI call
 */
void errcheck(char *file, int line, sword ret, OCIError *err)
{
  char errtxt[512];
  ub4 errnum;
  
  switch(ret) 
    {
    case OCI_SUCCESS:
      fprintf (stderr,"OCI_SUCCESS %s:%d\n",file,line);
      break;
      
    case OCI_NEED_DATA:
      fprintf(stderr,"OCI_NEED_DATA %s:%d\n",file,line);
      break;
      
    case OCI_ERROR:
      OCIErrorGet((dvoid*)err,1,NULL,&errnum,(text*)errtxt,sizeof(errtxt),
		  OCI_HTYPE_ERROR);
      fprintf(stderr,"ERROR [%s] %s:%d\n",errtxt,file,line);
      break;
      
    case OCI_CONTINUE:
      fprintf(stderr,"OCI_CONTINUE %s:%d\n",file,line);
      break;
      
    case OCI_SUCCESS_WITH_INFO:
      fprintf(stderr,"OCI_SUCCESS_WITH_INFO %s:%d\n",file,line);
      break;
      
    case OCI_NO_DATA:
      fprintf(stderr,"OCI_NO_DATA %s:%d\n",file,line);
      break;
      
    case OCI_INVALID_HANDLE:
      fprintf(stderr,"OCI_INVALID_HANDLE %s:%d\n",file,line);
      break;
      
    case OCI_STILL_EXECUTING:
      fprintf(stderr,"OCI_STILL_EXECUTE %s:%d\n",file,line);
      break;
      
    default:
      fprintf(stderr,"UNKOWN OCI RETURN [%d] %s:%d\n",ret,file,line);
      break;
    }
}
#endif


/*
 * Oracle Error
 *
 * Determines if there has been an error and what to
 * do about it.
 *
 * returns equivalent SQL codes
 */
SQLRETURN ood_driver_error(void *hH, sword ret,char *file, int line)
{
  text txt[512]="";
  sb4 errcodep;
  ub4 handle_type;
  int i=1;
  dvoid *ood_driver_handle=NULL;
  char *server_name;
  switch(ret)
    {
      
    case OCI_ERROR:
#ifdef UNIX_DEBUG
    case OCI_SUCCESS_WITH_INFO:
#endif
      {
	switch(((hgeneric*)hH)->htype)
	  {
	    
	  case SQL_HANDLE_DBC:
	    ood_driver_handle=((hDbc_T*)hH)->oci_err;
	    handle_type=OCI_HTYPE_ERROR;
	    server_name=((hDbc_T*)hH)->DSN;
	    break;
	    
	  case SQL_HANDLE_STMT:
	    ood_driver_handle=((hStmt_T*)hH)->dbc->oci_err;
	    handle_type=OCI_HTYPE_ERROR;
	    server_name=((hStmt_T*)hH)->dbc->DSN;
	    break;
	    
	  case SQL_HANDLE_DESC:
	    ood_driver_handle=((hDesc_T*)hH)->stmt->dbc->oci_err;
	    handle_type=OCI_HTYPE_ERROR;
	    server_name=((hDesc_T*)hH)->stmt->dbc->DSN;
	    break;
	    
	  case SQL_HANDLE_ENV:
	  default:
	    return SQL_ERROR;
	  }
	while(OCI_SUCCESS==OCIErrorGet(ood_driver_handle,
				       i++,NULL,&errcodep,txt,512,handle_type))
	  {
	    switch (errcodep)
	      {
	      case 1005:
		ood_post_diag((hgeneric*)hH,ERROR_ORIGIN_08004,0,NULL,
			      "Server will not accept NULL password",
			      errcodep,0,server_name,ERROR_STATE_08004,
			      file,line);
		break;
		
	      case 12154:
		ood_post_diag(
			      (hgeneric*)hH,ERROR_ORIGIN_08001,0,NULL,
			      "TNS: Could not resolve TARGETDB parameter as service name",
			      errcodep,0,server_name,ERROR_STATE_08001,
			      file,line);
		break;
		
	      case 12545:
		ood_post_diag(
			      (hgeneric*)hH,ERROR_ORIGIN_08001,0,NULL,
			      "TNS: Could not resolve host for given service name",
			      errcodep,0,server_name,ERROR_STATE_08001,
			      file,line);
		break;
		
	      case 12541:
		ood_post_diag((hgeneric*)hH,ERROR_ORIGIN_08001,0,NULL,
			      "TNS: No Listener",
			      errcodep,0,server_name,ERROR_STATE_08001,
			      file,line);
		break;
		
	      case 4043 :
		ood_post_diag((hgeneric*)hH,ERROR_ORIGIN_HY000,0,NULL,
			      "Object not found",
			      errcodep,0,server_name,ERROR_STATE_HY000,
			      file,line);
		break;
		
	      case 1405:
		ood_post_diag((hgeneric*)hH,ERROR_ORIGIN_HY000,0,NULL,
			      "Unexpected NULL Values",
			      errcodep,0,server_name,ERROR_STATE_HY000,
			      file,line);
		break;
		
	      case 1 :
		
	      case 1400 :
		
	      case 1407 :
		
	      case 2290 :
		
	      case 2291 :
		
	      case 2292 :
		ood_post_diag((hgeneric*)hH,ERROR_ORIGIN_23000,0,NULL,
			      ERROR_MESSAGE_23000,
			      errcodep,0,server_name,ERROR_STATE_23000,
			      file,line);
		break;
		
		
	      case 1406 :
		ood_post_diag((hgeneric*)hH,ERROR_ORIGIN_01004,0,NULL,
			      ERROR_MESSAGE_01004,
			      errcodep,0,server_name,ERROR_STATE_01004,
			      file,line);
		break;
		
		
	      case 910 :
		ood_post_diag((hgeneric*)hH,ERROR_ORIGIN_42000,0,NULL,
			      ERROR_MESSAGE_42000,
			      errcodep,0,server_name,ERROR_STATE_42000,
			      file,line);
		break;
		
	      case 3113:
		ood_post_diag((hgeneric*)hH,ERROR_ORIGIN_08S01,0,NULL,
			      ERROR_MESSAGE_08S01,
			      errcodep,0,server_name,ERROR_STATE_08S01,
			      file,line);
		
	      case 600:
	      case 1041:
		ood_post_diag((hgeneric*)hH,ERROR_ORIGIN_08S01,0,NULL,
			      ERROR_MESSAGE_08S01,
			      errcodep,0,server_name,ERROR_STATE_08S01,
			      file,line);
                
	      default :
		ood_post_diag((hgeneric*)hH,ERROR_ORIGIN_HY000,0,NULL,
			      (char*)txt,
			      errcodep,0,server_name,ERROR_STATE_HY000,
			      file,line);
	      }
	    return SQL_ERROR;
	  }
      }
    case OCI_INVALID_HANDLE:
      return SQL_INVALID_HANDLE;
      
#ifndef UNIX_DEBUG
    case OCI_SUCCESS_WITH_INFO:
      return SQL_STILL_EXECUTING;
#endif
      
    case OCI_SUCCESS:
      return SQL_SUCCESS;
      
    default:
      return 0;
    }
}

/*
 * Oracle Connect
 *
 * Connects to Oracle and sets up the handles
 */
SQLRETURN ood_driver_connect(hDbc_T *dbc)
{
  sword ret;
  if(!dbc->oci_env)
    {
#ifdef LIBCLNTSH8
      OCIEnvCreate((OCIEnv**)&dbc->oci_env,OCI_THREADED|OCI_OBJECT,
		   0,0,0,0,0,0);
#else
      OCIInitialize_log_stat(OCI_OBJECT|OCI_THREADED, 0,0,0,0,ret );
      OCIEnvInit_log_stat(&dbc->oci_env,OCI_DEFAULT,0,0,ret);
#endif
    }
  
  ret=OCIHandleAlloc_log_stat(dbc->oci_env,(dvoid**)&dbc->oci_err,
			      OCI_HTYPE_ERROR, 0,0,ret);
  if(ret)
    {
      /*
       * This one has to be done manually as we have no error handle yet
       */
      sb4 errcodep;
      text txt[512];
      OCIErrorGet(dbc->oci_env,
		  1,NULL,&errcodep,txt,512,OCI_HTYPE_ENV);
      THREAD_MUTEX_UNLOCK(dbc);
      ood_post_diag((hgeneric*)dbc,ERROR_ORIGIN_HY000,0,dbc->DB,
                    (char*)txt,
                    errcodep,0,dbc->DSN,ERROR_STATE_HY000,
                    __FILE__,__LINE__);
      THREAD_MUTEX_LOCK(dbc);
      return SQL_ERROR;
    }
  
  ret=OCIHandleAlloc_log_stat(dbc->oci_env,(dvoid**)&dbc->oci_srv,
			      OCI_HTYPE_SERVER,0,0,ret);
  if(ret)
    {
      THREAD_MUTEX_UNLOCK(dbc);
      ood_driver_error(dbc,ret,__FILE__,__LINE__);
      THREAD_MUTEX_LOCK(dbc);
      return SQL_ERROR;
    }
  
  return_to_space(dbc->DB);
  ret=OCIServerAttach_log_stat(dbc->oci_srv,dbc->oci_err,(text*)dbc->DB,
			       strlen(dbc->DB),(ub4)OCI_DEFAULT,ret);
  if(ret)
    {
      THREAD_MUTEX_UNLOCK(dbc);
      ood_driver_error(dbc,ret,__FILE__,__LINE__);
      THREAD_MUTEX_LOCK(dbc);
      return SQL_ERROR;
    }
  else
    {
#ifdef UNIX_DEBUG
      fprintf(stderr,"Connect to [%s] OK\n",dbc->DB);
#endif
      ret=OCIHandleAlloc_log_stat(dbc->oci_env,(dvoid**)&dbc->oci_svc,
				  OCI_HTYPE_SVCCTX,0,0,ret);
      if(ret)
	{
	  THREAD_MUTEX_UNLOCK(dbc);
	  ood_driver_error(dbc,ret,__FILE__,__LINE__);
	  THREAD_MUTEX_LOCK(dbc);
	  return SQL_ERROR;
	}
      else
        {
	  ret=OCIHandleAlloc_log_stat(dbc->oci_env,(dvoid**)&dbc->oci_ses,
				      OCI_HTYPE_SESSION,0,0,ret);
	  if(ret)
	    {
	      THREAD_MUTEX_UNLOCK(dbc);
	      ood_driver_error(dbc,ret,__FILE__,__LINE__);
	      THREAD_MUTEX_LOCK(dbc);
	      return SQL_ERROR;
	    }
        }
      
      ret=OCIAttrSet_log_stat(dbc->oci_svc,OCI_HTYPE_SVCCTX,dbc->oci_srv,0,
			      OCI_ATTR_SERVER,dbc->oci_err,ret);
      
      ret|=OCIAttrSet_log_stat(dbc->oci_ses,OCI_HTYPE_SESSION,dbc->UID,
			       strlen(dbc->UID), OCI_ATTR_USERNAME,
			       dbc->oci_err,ret);
      
      ret|=OCIAttrSet_log_stat(dbc->oci_ses,OCI_HTYPE_SESSION,dbc->PWD,
			       strlen(dbc->PWD), OCI_ATTR_PASSWORD,
			       dbc->oci_err,ret);
      if(ret)
        {
	  THREAD_MUTEX_UNLOCK(dbc);
	  ood_driver_error(dbc,ret,__FILE__,__LINE__);
	  THREAD_MUTEX_LOCK(dbc);
	  return SQL_ERROR;
        }
      ret=OCISessionBegin_log_stat(dbc->oci_svc,dbc->oci_err,dbc->oci_ses,
				   OCI_CRED_RDBMS,OCI_DEFAULT,ret);
#ifdef UNIX_DEBUG
      fprintf(stderr,"Session Begin returns %d\n",ret);
#endif
      if(ret)
        {
	  THREAD_MUTEX_UNLOCK(dbc);
	  ood_driver_error(dbc,ret,__FILE__,__LINE__);
	  THREAD_MUTEX_LOCK(dbc);
	  return SQL_ERROR;
        }
      
      ret=OCIAttrSet_log_stat(dbc->oci_svc,OCI_HTYPE_SVCCTX,dbc->oci_ses,0,
			      OCI_ATTR_SESSION,dbc->oci_err,ret);
      if(ret)
        {
	  THREAD_MUTEX_UNLOCK(dbc);
	  ood_driver_error(dbc,ret,__FILE__,__LINE__);
	  THREAD_MUTEX_LOCK(dbc);
	  return SQL_ERROR;
        }
    }
  assert(IS_VALID(dbc));
  return SQL_SUCCESS;
}

/*
 * Oracle Disconnect
 */
SQLRETURN ood_driver_disconnect(hDbc_T *dbc)
{
  sword ret;
  ret=OCISessionEnd_log_stat(dbc->oci_svc,dbc->oci_err,dbc->oci_ses,0,ret);
  /*ret|=OCIServerDetach(dbc->oci_srv,dbc->oci_err,OCI_DEFAULT);
   */
  if(ret)
    {
      THREAD_MUTEX_UNLOCK(dbc);
      ood_driver_error(dbc,ret,__FILE__,__LINE__);
      THREAD_MUTEX_LOCK(dbc);
      return SQL_ERROR;
    }
  assert(IS_VALID(dbc));
  return SQL_SUCCESS;
}

/* 
 * Oracle Prepare
 *
 * Alter any bound params to Oracle bound params and prepare
 */

SQLRETURN ood_driver_prepare(hStmt_T* stmt,SQLCHAR *sql_in)
{
  sword ret;
  int len;
  /*
   * Allocate the Oracle statement handle 
   */
  
  ret=OCIHandleAlloc_log_stat(stmt->dbc->oci_env,(dvoid**)&stmt->oci_stmt,
			      OCI_HTYPE_STMT,0,0,ret);
  if(ret)
    {
      ood_driver_error(stmt->dbc,ret,__FILE__,__LINE__);
      if(ENABLE_TRACE){
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
			(SQLHANDLE)stmt->dbc,SQL_ERROR,
			"");
      }
      return SQL_ERROR;
    }
  
  
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		  (SQLHANDLE)stmt->dbc,0,"s",
		  "ood_driver_prepare",sql_in);
#endif
  /*filter out trailing blanks and ;'s */
  len=strlen(sql_in);
  while(len>=1 && sql_in[len-1]==';' || sql_in[len-1]==' ')
    {
      sql_in[len-1]=(char)0;
      len--;
    }
  ret=OCIStmtPrepare_log_stat(stmt->oci_stmt,stmt->dbc->oci_err,sql_in,
			      strlen((const char*)sql_in),OCI_NTV_SYNTAX,OCI_DEFAULT,ret);
  if(ret)
    {
      ood_driver_error(stmt,ret,__FILE__,__LINE__);
      if(ret==OCI_ERROR)
	return SQL_ERROR;
      else
	return SQL_SUCCESS_WITH_INFO;
    }
  assert(IS_VALID(stmt));
  assert(IS_VALID(stmt->dbc));
  return SQL_SUCCESS;
}

/*
 * Oracle Execute
 * Does the execute and error handling
 */
SQLRETURN ood_driver_execute(hStmt_T* stmt)
{
  sword ret=0;
  assert(IS_VALID(stmt));
  if(stmt->current_ap->bound_col_flag)
    {
      unsigned int i;
      for(i=1;i<=stmt->current_ap->num_recs;i++)
	{
	  ret|=ood_driver_bind_param(stmt,i);
	}
    }
  
  ret=OCIAttrGet_log_stat(stmt->oci_stmt,
			  OCI_HTYPE_STMT,
			  (dvoid*)&stmt->stmt_type,
			  NULL,
			  OCI_ATTR_STMT_TYPE,
			  stmt->dbc->oci_err,
			  ret);
  
  if(stmt->stmt_type==OCI_STMT_SELECT)
    {
      if(stmt->row_array_size){
	
	ret=OCIStmtExecute_log_stat(stmt->dbc->oci_svc , 
				    stmt->oci_stmt , 
				    stmt->dbc->oci_err , 
				    stmt->row_array_size , 
				    0,0,0,OCI_DESCRIBE_ONLY,ret );
	
	
      }else{
	ret=OCIStmtExecute_log_stat(stmt->dbc->oci_svc ,
				    stmt->oci_stmt ,
				    stmt->dbc->oci_err ,
				    0,0,0,0,OCI_DEFAULT,ret );
      }
    }
  else
    {
      ub4 commit_mode;
      THREAD_MUTEX_LOCK(stmt->dbc);
      commit_mode=stmt->dbc->autocommit;
      THREAD_MUTEX_UNLOCK(stmt->dbc);
      ret=OCIStmtExecute_log_stat(stmt->dbc->oci_svc,stmt->oci_stmt,stmt->dbc->oci_err,
				  1,0,0,0,commit_mode,ret);
    }
  if(ret)
    {
      ood_driver_error(stmt,ret,__FILE__,__LINE__);
      if(ret==OCI_ERROR)
	return SQL_ERROR;
    }
  OCIAttrGet_log_stat(stmt->oci_stmt,OCI_HTYPE_STMT,
		      &stmt->num_result_rows,NULL,
		      OCI_ATTR_ROW_COUNT,stmt->dbc->oci_err,ret);
  return SQL_SUCCESS;
}

SQLRETURN ood_driver_transaction(hDbc_T *dbc, SQLSMALLINT action)
{
  SQLRETURN stat = SQL_SUCCESS;
  sword ret;
  
  
  switch(action){
  case SQL_COMMIT: 
    THREAD_MUTEX_LOCK(dbc);
    ret = OCITransCommit_log_stat((OCISvcCtx *)dbc->oci_svc,
				  (OCIError*)dbc->oci_err,
				  (ub4)OCI_DEFAULT,ret);
    if(ret) {
      ood_driver_error(dbc,ret,__FILE__,__LINE__);
      stat=SQL_ERROR;
    }
    THREAD_MUTEX_UNLOCK(dbc);
    
    
    break;
  case SQL_ROLLBACK:
    THREAD_MUTEX_LOCK(dbc);
    ret = OCITransRollback_log_stat((OCISvcCtx *)dbc->oci_svc,
				    (OCIError*)dbc->oci_err,
				    (ub4)OCI_DEFAULT,ret);
    if(ret){
      ood_driver_error(dbc,ret,__FILE__,__LINE__);
      stat=SQL_ERROR;
    }
    THREAD_MUTEX_UNLOCK(dbc);
    
    break;
  default:
    stat = SQL_ERROR;
  }
  
  assert(IS_VALID(dbc));
  return stat;
}

/*
 * Oracle Execute Describe
 *
 * After the execute, populate the ir descriptor with information about
 * the result set.
 */
SQLRETURN ood_driver_execute_describe(hStmt_T* stmt)
{
  sword ret;
  OCIParam *parm;
  ub4 i;
  text *col_name;
  ub4 siz=SQL_MAX_COLUMN_NAME_LEN;
  ub4 num_cols;
  SQLRETURN status=SQL_SUCCESS;
  sb2 precision;
  sb1 scale;
  ub1 nullable;
  assert(IS_VALID(stmt));
  if(stmt->stmt_type!=OCI_STMT_SELECT)
    return SQL_SUCCESS;
  
  ret=OCIAttrGet_log_stat((dvoid*)stmt->oci_stmt,OCI_HTYPE_STMT,
			  (dvoid**)&num_cols,
			  0,OCI_ATTR_PARAM_COUNT,stmt->dbc->oci_err,ret);
  if(ret)
    {
      ood_driver_error(stmt,ret,__FILE__,__LINE__);
      if(ret==OCI_ERROR)
	return SQL_ERROR;
    }
  
  
  if(SQL_SUCCESS!=
     ood_alloc_col_desc(stmt,num_cols,stmt->current_ir,stmt->current_ar))
    {
      return SQL_ERROR;
    }
  
  /*
   * Populate the descriptor records
   */
  for(i=1;i<=stmt->current_ir->num_recs;i++)
    {
      /*
       * Make sure we can see the parent descriptor
       */
      ret=OCIParamGet_log_stat((dvoid*)stmt->oci_stmt,OCI_HTYPE_STMT,
			       stmt->dbc->oci_err,(dvoid*)&parm,i,ret);
      
      /* IR data type (AR types may be derived from this later) */
      /* 
       * Annoyingly we find that ODBC expects the data type conversion to
       * happen in the driver rather than in the back-end. This means it
       * expects to be able to derive the original return type from the 
       * IRD. We use the IRD to store the type we actually get the data
       * as, so there needs to be an "original type" field to store what it
       * was.
       */
      if(!stmt->current_ir->recs.ir[i].data_type)
	ret|=OCIAttrGet_log_stat((dvoid*)parm,OCI_DTYPE_PARAM,
				 &stmt->current_ir->recs.ir[i].orig_type,
				 0,OCI_ATTR_DATA_TYPE,stmt->dbc->oci_err,ret);
      stmt->current_ir->recs.ir[i].data_type
	=stmt->current_ir->recs.ir[i].orig_type;
      
      /* IR data size */
      if(!stmt->current_ir->recs.ir[i].data_size)
	ret|=OCIAttrGet_log_stat((dvoid*)parm,OCI_DTYPE_PARAM,
				 &stmt->current_ir->recs.ir[i].data_size,
				 0,OCI_ATTR_DATA_SIZE,stmt->dbc->oci_err,ret);
      
      /* AR column name :- can be set elsewhere */
      ret|=OCIAttrGet((dvoid*)parm,OCI_DTYPE_PARAM,
		      &col_name,
		      &siz,OCI_ATTR_NAME,stmt->dbc->oci_err);
      if(ret)
	{
	  ood_driver_error(stmt,ret,__FILE__,__LINE__);
	  if(ret==OCI_ERROR)
	    return SQL_ERROR;
	}
      if(!*stmt->current_ar->recs.ar[i].column_name)
	{
	  siz=siz>ORACLE_MAX_COLUMN_LEN?ORACLE_MAX_COLUMN_LEN:siz;
	  memcpy(stmt->current_ar->recs.ar[i].column_name,col_name,(int)siz);
	  stmt->current_ar->recs.ar[i].column_name[siz]='\0';
	}
      
      /* AR precision */
      ret|=OCIAttrGet((dvoid*)parm,OCI_DTYPE_PARAM,
		      &precision,
		      0,OCI_ATTR_PRECISION,stmt->dbc->oci_err);
      stmt->current_ar->recs.ar[i].precision=(SQLSMALLINT)precision;
      
      /* AR scale (aka decimal digits) */
      ret|=OCIAttrGet((dvoid*)parm,OCI_DTYPE_PARAM,
		      &scale,
		      0,OCI_ATTR_SCALE,stmt->dbc->oci_err);
      /*if(scale!=-127)*/
	stmt->current_ar->recs.ar[i].scale=(SQLSMALLINT)scale;
      
      /* AR nullable */
      ret|=OCIAttrGet((dvoid*)parm,OCI_DTYPE_PARAM,
		      &nullable,
		      0,OCI_ATTR_IS_NULL,stmt->dbc->oci_err);
      if(nullable)
	stmt->current_ar->recs.ar[i].nullable=SQL_NULLABLE;
      else
	stmt->current_ar->recs.ar[i].nullable=SQL_NO_NULLS;
      
      /* setup the IR and a few more essential AR fields */
      status|=ood_driver_setup_fetch_env(&stmt->current_ir->recs.ir[i],
					 &stmt->current_ar->recs.ar[i]);
    }
  
  return status;
}
/*
 * execute_prefetch 
 *
 * Processes the first rows of a query ready for the first fetch
 * (note, nothing to do with Oracle prefetch)
 */
SQLRETURN ood_driver_prefetch(hStmt_T* stmt)
{
  sword ret=OCIStmtExecute(stmt->dbc->oci_svc,stmt->oci_stmt,
			   stmt->dbc->oci_err,(ub4)stmt->row_array_size,
			   0,0,0,OCI_DEFAULT);
  
  OCIAttrGet(stmt->oci_stmt,OCI_HTYPE_STMT,
	     &stmt->num_result_rows,0,OCI_ATTR_ROW_COUNT,
	     stmt->dbc->oci_err);
#ifdef UNIX_DEBUG
  errcheck(__FILE__,__LINE__,ret,stmt->dbc->oci_err);
#endif
  if(ret&&ret!=OCI_SUCCESS_WITH_INFO)
    {
      if(ret==OCI_NO_DATA)
	return SQL_NO_DATA;
      
      ood_driver_error(stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  return SQL_SUCCESS;
}


SQLRETURN ood_ocitype_to_sqltype_imp(hStmt_T* stmt, int colNum)
{
  

  SQLRETURN ret = stmt->current_ar->recs.ar[colNum].concise_type;

  if(!ret) ret = ood_ocitype_to_sqltype( stmt->current_ir->recs.ir[colNum].orig_type);
  

  if (stmt->current_ar->recs.ar[colNum].data_type == SQL_C_FLOAT)
    ret = SQL_C_FLOAT;
  if (stmt->current_ar->recs.ar[colNum].data_type == SQL_C_DOUBLE)
    ret = SQL_C_DOUBLE;

  /* I dont understand yet why this one doesnt work like the others
     if (stmt->current_ar->recs.ar[colNum].data_type == SQL_C_SLONG)
     ret = SQL_C_SLONG;*/


  return ret;
}

/*
 * Driver type to SQL type
 *
 * Returns the SQL type by default associated with the OCI type
 */
SQLRETURN ood_ocitype_to_sqltype(ub2 data_type)
{

  switch(data_type)
    {
      /*varchar*/
    case SQLT_RDD:
    case SQLT_RID:
    case SQLT_STR:
    case SQLT_VST:
    case SQLT_VCS:
    case SQLT_AVC:

      return SQL_VARCHAR;

      /* fixed char */
    case SQLT_CHR:
    case SQLT_AFC:

      return SQL_CHAR;

      /* numbers*/
    case SQLT_INT:
      return SQL_INTEGER;
        
    case SQLT_FLT:
      return SQL_C_DOUBLE;
        
    case SQLT_NUM:
    case SQLT_VNU:
    case SQLT_UIN:
      return SQL_C_NUMERIC;

      /* LONGs, LOBs and binaries*/
    case SQLT_VBI:
    case SQLT_BIN:
      return SQL_C_BINARY;
        
    case SQLT_LNG:
    case SQLT_CLOB:
    case SQLT_LVC:
      return SQL_LONGVARCHAR;

    case SQLT_LVB:
    case SQLT_BLOB:
    case SQLT_FILE:
    case SQLT_LBI:
      return SQL_LONGVARBINARY;

      /*date*/
    case SQLT_DAT:
    case SQLT_ODT:
      return SQL_C_TIMESTAMP;

      /*others*/
    case SQLT_NTY:
    case SQLT_REF:
      return SQL_C_BINARY;

    }
  return SQL_UNKNOWN_TYPE;
}

/*
 * Alloc Param Descriptors
 *
 * (re)allocate param descriptors being careful to initialise them
 * correctly
 */
SQLRETURN ood_alloc_param_desc(hStmt_T *stmt,int rows,
			       hDesc_T *imp,hDesc_T* app)
{    
  int floor=0;
  if((unsigned int)rows<=imp->num_recs)
    {
      return SQL_SUCCESS;
    }
  /* app->num_resc stores the number of recs we think we should have */
  if(app->num_recs>(unsigned)rows)
    rows=app->num_recs;
  if(imp->num_recs)
    {
      /*
       * Need to realloc 
       */
      floor=imp->num_recs;
      imp->recs.ip=ORAREALLOC(imp->recs.ip,
			      sizeof(ip_T)*(rows+1));
      if(!imp->recs.ip)
        {
	  ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_HY001,0,"",
			ERROR_MESSAGE_HY001,__LINE__,0,stmt->dbc->DSN,
			ERROR_STATE_HY001,
			__FILE__,__LINE__);
	  return SQL_ERROR;
        }
        
      app->recs.ap=ORAREALLOC(app->recs.ap, sizeof(ap_T)*(rows+1));
      if(!app->recs.ap)
        {
	  ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_HY001,0,"",
			ERROR_MESSAGE_HY001,__LINE__,0,stmt->dbc->DSN,
			ERROR_STATE_HY001,
			__FILE__,__LINE__);
	  return SQL_ERROR;
        }
    }
  else /* we can malloc */
    {
      imp->recs.ip=ORAMALLOC(sizeof(struct ip_TAG)*(rows+1));
      if(!imp->recs.ip)
        {
	  ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_HY001,0,"",
			ERROR_MESSAGE_HY001,__LINE__,0,stmt->dbc->DSN,
			ERROR_STATE_HY001,
			__FILE__,__LINE__);
	  return SQL_ERROR;
        }
    
      app->recs.ap=ORAMALLOC(sizeof(struct ap_TAG)*(rows+1));
      if(!app->recs.ap)
        {
	  ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_HY001,0,"",
			ERROR_MESSAGE_HY001,__LINE__,0,stmt->dbc->DSN,
			ERROR_STATE_HY001,
			__FILE__,__LINE__);
	  return SQL_ERROR;
        }

    }
  imp->num_recs=rows;
  app->num_recs=rows;
  while(floor<=rows)
    {
      memset(&(imp->recs.ip[floor]),0,sizeof(struct ip_TAG));
      imp->recs.ip[floor].data_type=0;
      imp->recs.ip[floor].orig_type=0;
      imp->recs.ip[floor].data_size=0;
      imp->recs.ip[floor].col_num=floor;
      imp->recs.ip[floor].data_ptr=NULL;
      imp->recs.ip[floor].default_copy=NULL;
      imp->recs.ip[floor].to_string=NULL;
      imp->recs.ip[floor].to_oracle=NULL;
      imp->recs.ip[floor].desc=imp;
      imp->recs.ip[floor].valid_flag=VALID_FLAG_DEFAULT;
      imp->recs.ir[floor].ind_arr=NULL;
      imp->recs.ir[floor].length_arr=NULL;
      imp->recs.ir[floor].rcode_arr=NULL;
      imp->recs.ir[floor].valid_flag=VALID_FLAG_DEFAULT;


      memset(&(app->recs.ap[floor]),0,sizeof(struct ap_TAG));
      app->recs.ap[floor].auto_unique=0;
      app->recs.ap[floor].base_column_name=NULL;
      app->recs.ap[floor].base_table_name=NULL;
      app->recs.ap[floor].case_sensitive=SQL_FALSE;
      app->recs.ap[floor].catalog_name=NULL;
      app->recs.ap[floor].concise_type=SQL_C_DEFAULT;
      app->recs.ap[floor].data_ptr=NULL;
      app->recs.ap[floor].display_size=0;
      app->recs.ap[floor].fixed_prec_scale=0;
      app->recs.ap[floor].bind_indicator=NULL;
      app->recs.ap[floor].column_label=app->recs.ap[floor].column_name;
      app->recs.ap[floor].length=0;
      app->recs.ap[floor].literal_prefix=NULL;
      app->recs.ap[floor].literal_suffix=NULL;
      app->recs.ap[floor].local_type_name=NULL;
      *app->recs.ap[floor].column_name='\0';
      app->recs.ap[floor].nullable=SQL_TRUE;
      app->recs.ap[floor].num_prec_radix=10;
      app->recs.ap[floor].octet_length=0;
      app->recs.ap[floor].precision=0;
      app->recs.ap[floor].scale=0;
      app->recs.ap[floor].schema_name=NULL;
      app->recs.ap[floor].searchable=SQL_TRUE;
      app->recs.ap[floor].table_name=NULL;
      app->recs.ap[floor].data_type=0;
      app->recs.ap[floor].type_name=NULL;
      app->recs.ap[floor].un_signed=SQL_TRUE;
      app->recs.ap[floor].updateable=SQL_TRUE;
      app->recs.ap[floor].valid_flag=VALID_FLAG_DEFAULT;
      floor++;
    }
  return SQL_SUCCESS;
}

static void ood_setup_bookmark(ir_T *ir, ar_T* ar,void *desc)
{
  ir->data_type=SQLT_RDD;
  ir->orig_type=SQLT_RDD;
  ir->data_size=10;
  ir->col_num=0;
  ir->data_ptr=NULL; /* there is no data! */
  ir->default_copy=NULL;
  ir->to_string=NULL;;
  ir->to_oracle=NULL;
  ir->desc=desc;
  ir->ind_arr=NULL;
  ir->length_arr=NULL;
  ir->rcode_arr=NULL;
  ir->locator=NULL;
  ir->posn=1;
  ir->valid_flag=VALID_FLAG_DEFAULT;

  ar->auto_unique=SQL_TRUE;
  ar->base_column_name=NULL;
  ar->base_table_name=NULL;
  ar->case_sensitive=SQL_FALSE;
  ar->catalog_name=NULL;
  ar->concise_type=SQL_C_BOOKMARK;
  ar->data_ptr=NULL;
  ar->display_size=20;
  ar->fixed_prec_scale=0;
  ar->bind_indicator=NULL;
  ar->column_label=ar->column_name;
  ar->length=20;
  ar->literal_prefix=NULL;
  ar->literal_suffix=NULL;
  ar->local_type_name=NULL;
  strcpy((char*)ar->column_name,"ROWID");
  ar->nullable=SQL_FALSE;
  ar->num_prec_radix=10;
  ar->octet_length=20;
  ar->precision=0;
  ar->scale=0;
  ar->schema_name=NULL;
  ar->searchable=SQL_TRUE;
  ar->table_name=NULL;
  ar->data_type=0;
  ar->type_name=NULL;
  ar->un_signed=SQL_TRUE;
  ar->updateable=SQL_FALSE;	
  ar->valid_flag=VALID_FLAG_DEFAULT;

  return;
}

/*
 * Alloc Col Descriptors
 *
 * (re)allocate row descriptors being careful to initialise them
 * correctly
 */

SQLRETURN ood_alloc_col_desc(hStmt_T *stmt,int rows,
			     hDesc_T *imp,hDesc_T* app)
{    
  int floor=0;
  if((unsigned int)rows<=imp->num_recs)
    {
      return SQL_SUCCESS;
    }
  if(imp->num_recs)
    {
      /*
       * Need to realloc 
       */
      floor=imp->num_recs;
      imp->recs.ir=ORAREALLOC(imp->recs.ir,
			      sizeof(ir_T)*(rows+1));
      if(!imp->recs.ir)
        {
	  ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_HY001,0,"",
			ERROR_MESSAGE_HY001,__LINE__,0,stmt->dbc->DSN,
			ERROR_STATE_HY001,
			__FILE__,__LINE__);
	  return SQL_ERROR;
        }
      app->recs.ar=ORAREALLOC(app->recs.ar,
			      sizeof(ar_T)*(rows+1));
      if(!app->recs.ar)
        {
	  ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_HY001,0,"",
			ERROR_MESSAGE_HY001,__LINE__,0,stmt->dbc->DSN,
			ERROR_STATE_HY001,
			__FILE__,__LINE__);
	  return SQL_ERROR;
        }
    }
  else /* we can malloc */
    {
      imp->recs.ir=ORAMALLOC(sizeof(struct ir_TAG)*(rows+1));
      if(!imp->recs.ir)
        {
	  ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_HY001,0,"",
			ERROR_MESSAGE_HY001,__LINE__,0,stmt->dbc->DSN,
			ERROR_STATE_HY001,
			__FILE__,__LINE__);
	  return SQL_ERROR;
        }
      app->recs.ar=ORAMALLOC(sizeof(struct ar_TAG)*(rows+1));
      if(!app->recs.ar)
        {
	  ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_HY001,0,"",
			ERROR_MESSAGE_HY001,__LINE__,0,stmt->dbc->DSN,
			ERROR_STATE_HY001,
			__FILE__,__LINE__);
	  return SQL_ERROR;
        }
    }
  imp->num_recs=rows;
  app->num_recs=rows;
  while(floor<=rows)
    {
      imp->recs.ir[floor].data_type=0;
      imp->recs.ir[floor].orig_type=0;
      imp->recs.ir[floor].data_size=0;
      imp->recs.ir[floor].col_num=floor;
      imp->recs.ir[floor].data_ptr=NULL;
      imp->recs.ir[floor].default_copy=NULL;
      imp->recs.ir[floor].to_string=NULL;
      imp->recs.ir[floor].to_oracle=NULL;
      imp->recs.ir[floor].desc=imp;
      imp->recs.ir[floor].ind_arr=NULL;
      imp->recs.ir[floor].length_arr=NULL;
      imp->recs.ir[floor].rcode_arr=NULL;
      imp->recs.ir[floor].locator=NULL;
      imp->recs.ir[floor].posn=1;
      imp->recs.ir[floor].valid_flag=VALID_FLAG_DEFAULT;


      app->recs.ar[floor].auto_unique=SQL_FALSE;
      app->recs.ar[floor].base_column_name=NULL;
      app->recs.ar[floor].base_table_name=NULL;
      app->recs.ar[floor].case_sensitive=SQL_FALSE;
      app->recs.ar[floor].catalog_name=NULL;
      app->recs.ar[floor].concise_type=SQL_C_DEFAULT;
      app->recs.ar[floor].data_ptr=NULL;
      app->recs.ar[floor].display_size=0;
      app->recs.ar[floor].fixed_prec_scale=0;
      app->recs.ar[floor].bind_indicator=NULL;
      app->recs.ar[floor].column_label=app->recs.ar[floor].column_name;
      app->recs.ar[floor].length=0;
      app->recs.ar[floor].literal_prefix=NULL;
      app->recs.ar[floor].literal_suffix=NULL;
      app->recs.ar[floor].local_type_name=NULL;
      *app->recs.ar[floor].column_name='\0';
      app->recs.ar[floor].nullable=SQL_TRUE;
      app->recs.ar[floor].num_prec_radix=10;
      app->recs.ar[floor].octet_length=0;
      app->recs.ar[floor].precision=0;
      app->recs.ar[floor].scale=0;
      app->recs.ar[floor].schema_name=NULL;
      app->recs.ar[floor].searchable=SQL_TRUE;
      app->recs.ar[floor].table_name=NULL;
      app->recs.ar[floor].data_type=0;
      app->recs.ar[floor].type_name=NULL;
      app->recs.ar[floor].un_signed=SQL_TRUE;
      app->recs.ar[floor].updateable=SQL_TRUE;
      app->recs.ap[floor].valid_flag=VALID_FLAG_DEFAULT;

      floor++;
    }
  /*
   * Set up out bookmark column
   */
  ood_setup_bookmark(&imp->recs.ir[0],&app->recs.ap[0],imp);
  return SQL_SUCCESS;
}

/*
 * Assing IR
 *
 * This function populates the ir record with specific values (such as the
 * ones used in catalog functions)
 * If attributes have already been set they remain in tact.
 */
SQLRETURN ood_assign_ir(ir_T *ir,
			ub2 data_type,
			ub2 data_size,
			sb2 indicator,
			SQLRETURN (*default_copy)(),
			SQLRETURN (*to_string)()
			)
{
  if(!ir->data_type)
    ir->data_type=data_type;
  if(!ir->orig_type)
    ir->orig_type=data_type;
  if(!ir->data_size)
    {
      ir->data_size=data_size;
    }
  if(!ir->ind_arr)
    ir->ind_arr=(void*)((int)indicator);
  if(!ir->default_copy)
    ir->default_copy=default_copy;
  if(!ir->to_string)
    ir->to_string=to_string;
  return SQL_SUCCESS;
}

SQLRETURN ood_driver_define_col(ir_T* ir)
{
  sword ret;
  OCIDefine *dfn;
  if(!ir->data_ptr)
    {
      if(ir->data_type==SQLT_CLOB||ir->data_type==SQLT_BLOB)
	{
	  unsigned i;
	  ir->locator=ORAMALLOC(sizeof(OCILobLocator*)*
				ir->desc->stmt->row_array_size);
	  ir->data_ptr=ORAMALLOC(sizeof(OCILobLocator**)*
				 ir->desc->stmt->row_array_size);
	  for(i=0;i<ir->desc->stmt->row_array_size;i++)
	    {
	      OCIDescriptorAlloc(ir->desc->stmt->dbc->oci_env,
				 (dvoid*)&ir->locator[i],OCI_DTYPE_LOB,0,0);
	      ((OCILobLocator**)ir->data_ptr)[i]=ir->locator[i];
	    }
	  ir->data_size=(ub2)-1;
	  ir->desc->lob_col_flag++;
	}
      else
	{
	  ir->data_ptr=ORAMALLOC(ir->data_size
				 *(ir->desc->stmt->row_array_size));
	  if(!ir->data_ptr)
            {
	      ood_post_diag((hgeneric*)ir->desc->stmt->current_ir,
			    ERROR_ORIGIN_HY001,0,"",
			    ERROR_MESSAGE_HY001,__LINE__,0,ir->desc->stmt->dbc->DSN,
			    ERROR_STATE_HY001,
			    __FILE__,__LINE__);
	      return SQL_ERROR;
            }
	}
    }
  /*#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)*/
  /*ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,*/
  /*(SQLHANDLE)ir->desc->stmt,0,"i"*/
  /*"row_array_size",ir->desc->stmt->row_array_size);*/
  /*#endif*/
  ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
  memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
  ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
  ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
  ret=OCIDefineByPos(ir->desc->stmt->oci_stmt,&dfn,
		     ir->desc->stmt->dbc->oci_err,ir->col_num,
		     ir->data_ptr, ir->data_size,
		     ir->data_type, ir->ind_arr, ir->length_arr,ir->rcode_arr,
		     OCI_DEFAULT);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  return SQL_SUCCESS;
}

SQLRETURN (*ood_fn_default_copy(ub2 drvtype, SQLSMALLINT sqltype))
     (int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*)
{
  switch(drvtype)
    {
      /*varchar*/
    case SQLT_STR:
    case SQLT_VST:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocistr_sqlnts;

      /* fixed char */
    case SQLT_CHR:
    case SQLT_AFC:
    case SQLT_VCS:
    case SQLT_AVC:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocistr_sqlnts;

      /* numbers*/
    case SQLT_INT:
      switch(sqltype)
        {
	case SQL_C_CHAR:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqlnts;

	case SQL_C_SLONG:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqlslong;

	case SQL_C_ULONG:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqlulong;

	case SQL_C_DOUBLE:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqldouble;

	case SQL_C_FLOAT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqlfloat;

	case SQL_C_SSHORT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqlsshort;

	case SQL_C_USHORT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqlushort;

	case SQL_C_SBIGINT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqlsbigint;

	case SQL_C_UBIGINT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqlubigint;

	case SQL_C_UTINYINT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqlutinyint;

	case SQL_C_STINYINT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqlstinyint;

	default:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociint_sqlslong;
        }
      break;
        
    case SQLT_FLT:
      switch(sqltype)
        {
	case SQL_C_CHAR:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociflt_sqlnts;

	case SQL_C_DOUBLE:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociflt_sqldouble;

	case SQL_C_FLOAT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociflt_sqlfloat;

	case SQL_C_NUMERIC:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ociflt_sqlnumeric;
        }
      break;

        
    case SQLT_NUM:
    case SQLT_VNU:
    case SQLT_UIN:
      switch(sqltype)
        {
	case SQL_C_CHAR:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlnts;

	case SQL_C_SLONG:
	case SQL_C_LONG:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlslong;

	case SQL_C_ULONG:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlulong;

	case SQL_C_DOUBLE:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqldouble;

	case SQL_C_FLOAT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlfloat;

	case SQL_C_SSHORT:
	case SQL_C_SHORT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlsshort;

	case SQL_C_USHORT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlushort;

	case SQL_C_SBIGINT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlsbigint;

	case SQL_C_UBIGINT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlubigint;

	case SQL_C_UTINYINT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlutinyint;

	case SQL_C_STINYINT:
	case SQL_C_TINYINT:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlstinyint;

	case SQL_C_NUMERIC: 
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlnumeric;

	default:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocivnu_sqlslong;
        }
      break;


      /*date*/
    case SQLT_DAT:
    case SQLT_ODT:
      switch(sqltype)
        {
	case SQL_C_TIME:
	case SQL_TYPE_TIME:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocidat_sqltime;

	case SQL_C_DATE:
	case SQL_TYPE_DATE:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocidat_sqldate;
            
	case SQL_C_TIMESTAMP:
	case SQL_TYPE_TIMESTAMP:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocidat_sqltimestamp;

	case SQL_C_CHAR:
	default:
	  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	    ocidat_sqlnts;
        }
      break;

    case SQLT_CLOB:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocilob_sqllvc;
        
    case SQLT_BLOB:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocilob_sqllvb;

      /* LONGs, LOBs and binaries*/
    case SQLT_LNG:
    case SQLT_VBI:
    case SQLT_BIN:
    case SQLT_LBI:
    case SQLT_LVC:
    case SQLT_LVB:
    case SQLT_FILE:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocistr_sqlnts;

      /*others*/
    case SQLT_RDD:
    case SQLT_RID:
      if(sqltype==SQL_C_CHAR)
	return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	  ocistr_sqlnts;
      else
	return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	  ocistr_sqlnts;

    case SQLT_NTY:
    case SQLT_REF:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocistr_sqlnts;

    }
  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
    ocistr_sqlnts;
}

SQLRETURN (*drv_type_to_string(ub2 drvtype, SQLSMALLINT sqltype))
     (int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*)
{
  switch(drvtype)
    {
      /*varchar*/
    case SQLT_STR:
    case SQLT_VST:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocistr_sqlnts;

      /* fixed char */
    case SQLT_CHR:
    case SQLT_AFC:
    case SQLT_VCS:
    case SQLT_AVC:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocistr_sqlnts;

      /* numbers*/
    case SQLT_INT:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ociint_sqlnts;
        
    case SQLT_FLT:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ociflt_sqlnts;
        
    case SQLT_NUM:
    case SQLT_VNU:
    case SQLT_UIN:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocivnu_sqlnts;

    case SQLT_CLOB:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocilob_sqllvc;
		
    case SQLT_BLOB:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocilob_sqllvb;

      /* LONGs, LOBs and binaries*/
    case SQLT_LNG:
    case SQLT_VBI:
    case SQLT_BIN:
    case SQLT_LBI:
    case SQLT_LVC:
    case SQLT_LVB:
    case SQLT_FILE:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocistr_sqlnts;

      /*date*/
    case SQLT_DAT:
    case SQLT_ODT:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocidat_sqlnts;

      /*others*/
    case SQLT_RDD:
    case SQLT_NTY:
    case SQLT_REF:
    case SQLT_RID:
      return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
	ocistr_sqlnts;

    }
  return (SQLRETURN(*)(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*))
    ocistr_sqlnts;
}


/*
	 * Setup Fetch Env
	 *
	 * This function populates the default and to_string function pointers in
	 * the IR descriptor and the data type in the AR descriptor if not already set. 
	 * It also sets the data_type field of IR to return the correct object type and
	 * allocates the correct amount of memory for the data.
	 *
	 * Parameters:
	 * ir_T        ir           The IR descriptor to work on
	 * ar_T        ar           The AR descriptor to work on
	 * ub2         ocitype      The Driver type of the data
	 */
SQLRETURN ood_driver_setup_fetch_env(ir_T *ir, ar_T* ar)
{
		
  switch(ir->data_type)
    {
      /*varchar*/
    case SQLT_STR:
    case SQLT_VST:
      ir->data_type=SQLT_STR;
      ir->data_size++;
      ir->to_string=ocistr_sqlnts;
      break;

      /* fixed char */
      /* TODO! :- fixed char, ATM this is translated to NTS */
    case SQLT_CHR:
    case SQLT_AFC:
    case SQLT_VCS:
    case SQLT_AVC:
    case SQLT_LNG: /* TODO this needs implementing properly */
    case SQLT_LVC: /* TODO this needs implementing properly */
      ir->data_type=SQLT_STR;
      ir->data_size++;
      ir->to_string=ocistr_sqlnts;
      break;

      /* numbers*/
    case SQLT_INT:
      ir->data_type=SQLT_INT;
      ir->data_size=sizeof(long);
      ir->to_string=ociint_sqlslong;
      break;
			
    case SQLT_FLT:
      ir->data_type=SQLT_FLT;
      ir->data_size=sizeof(double);
      ir->to_string=ociflt_sqlnts;
      break;
			
      /* VNU etc */
    case SQLT_NUM:
    case SQLT_VNU:
    case SQLT_UIN:
      ir->data_type=SQLT_VNU;
      ir->data_size=1024;
      ir->to_string=ocivnu_sqlnts;


      break;

      /* LONGs, LOBs and binaries*/
      /* TODO everything! */
    case SQLT_VBI:
    case SQLT_BIN:
    case SQLT_LBI:
    case SQLT_LVB:
      ir->data_type=SQLT_BIN;
      ir->to_string=ocistr_sqlnts;
      break;
			
    case SQLT_BLOB:
      ir->data_type=SQLT_BLOB;
      ir->data_size=0;
      ir->to_string=ocilob_sqllvc;
      break;

    case SQLT_CLOB:
      ir->data_type=SQLT_CLOB;
      ir->data_size=0;
      ir->to_string=ocilob_sqllvc;
      break;

    case SQLT_FILE:
      ir->data_type=SQLT_FILE;
      ir->data_size=0;
      ir->to_string=ocistr_sqlnts;
      break;

      /*date*/
    case SQLT_DAT:
    case SQLT_ODT:
      ir->data_type=SQLT_ODT;
      ir->data_size=sizeof(OCIDate);
      ir->to_string=ocidat_sqlnts;
      break;

      /*others*/
    case SQLT_RDD:
    case SQLT_NTY:
    case SQLT_REF:
    case SQLT_RID:
    default:
      ir->data_type=SQLT_STR;
      ir->data_size=127; /* complete guess */
      ir->to_string=ocistr_sqlnts;
    }
  if(!ar->data_type) /* not bound (yet) */
    {
      ar->data_type=ood_ocitype_to_sqltype(ir->data_type);
      /*hack alert!! */

      if(ar->precision!=0 && ar->scale==0){
	ar->data_type=SQL_INTEGER;
      }

      if(ar->scale==-127){
	if(ar->precision==126)
	  ar->data_type=SQL_DOUBLE;
	else
	ar->data_type=SQL_FLOAT;
      }
      
      ar->concise_type=ar->data_type;
      ar->display_size=sqltype_display_size(ar->data_type,ir->data_size);
    }
  if(!ar->octet_length)
    ar->octet_length=ar->length=ir->data_size;
  if(ar->concise_type==SQL_CHAR || ar->concise_type==SQL_VARCHAR  )
    ar->precision = ar->octet_length-1;

  ir->default_copy=ood_fn_default_copy(ir->data_type,ar->data_type);

  return ood_driver_define_col(ir);
}
/*
 * ========================================================================= *
 * Type conversion functions for SQLGetData.
 * ========================================================================= *
 */

/*
 * ocistr_sqlnts =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=- *
 */

SQLRETURN ocistr_sqlnts(int row,ir_T* ir,SQLPOINTER target,SQLINTEGER buflen,
			SQLINTEGER* indi)
{
  int i;
  SQLCHAR* src;
  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
		  (SQLHANDLE)ir->desc->stmt,0,"siihsh",
		  NULL,"ocistr_sqlnts",
		  "row",row,
		  "BufferLength",buflen,
		  "&ir->data_ptr",(long)ir->data_ptr,
		  "src",src,
		  "&Target",(long)target);
#endif
  i=strlen((const char*)src);
  if(i<buflen)
    {
      strcpy((char*)target,(const char*)src);
      if(indi)
	indi[row]=i;
    }
  else
    {
      for(i=0;i<buflen;i++)
	{
	  ((SQLCHAR*)target)[i]=src[i];
	}
      ((SQLCHAR*)target)[i-1]='\0';
      ood_post_diag((hgeneric*)ir->desc->stmt,ERROR_ORIGIN_01004,ir->col_num,"",
		    ERROR_MESSAGE_01004,
                    __LINE__,0,"",ERROR_STATE_01004,
                    __FILE__,__LINE__);
      if(indi)
	indi[row]=buflen;
      return SQL_SUCCESS_WITH_INFO;
    }
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		  (SQLHANDLE)ir->desc->stmt,SQL_SUCCESS,"ss",
		  NULL,"ocistr_sqlnts",
		  "Target",target);
#endif
  return SQL_SUCCESS;
}

/*
 * ocivnu_sqlnts 
 */
 
SQLRETURN ocivnu_sqlnts(int row,ir_T* ir,SQLPOINTER target,SQLINTEGER buflen,
			SQLINTEGER* indi)
{
  /*    return ocistr_sqlnts(ir,target,buflen,indi);*/
  sword ret;
  ub4 len=64;
  char txt[64];
  /*
    SQLCHAR *src;
    src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);
  */
  /*
   * For some reason, OCINumberToText seems to want >48 <64 chars to
   * play with
   *
   * In other news, the 8.0.5 version of the Oracle client doesn't have
   * the "text minimum" number conversion.
   * but the conditional LIBCLNTSH8 does a half-hearted job since this
   * feature was introduced in 8.1.x (seems like 8.1.5)
   * for this case we should determine at runtime what version we deal with
   * or even if this specific conversion works (btw. idef/else was wrong way round)
   */

 ret=OCINumberToText(ir->desc->dbc->oci_err, (OCINumber*)ir->data_ptr,
#ifdef LIBCLNTSH8
		     (unsigned char*)"TM", 2,
#else
		     (unsigned char*)"9999999999999999990.99999999999", 30,
#endif
		     (unsigned char*)"",0,&len,(unsigned char*)txt);
#ifdef UNIX_DEBUG
  errcheck(__FILE__,__LINE__,ret,ir->desc->dbc->oci_err);
#endif
  if(indi)
    {
      *indi=len;
    }
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  ood_bounded_strcpy(target,txt,buflen);
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		  (SQLHANDLE)ir->desc->stmt,0,"sis",
		  NULL,"ocivnu_sqlnts",
		  "BufferLength",buflen,
		  "Target",target);
#endif
  return SQL_SUCCESS;
}
/*
 *  gcivnu_sqlslong
 */
SQLRETURN ocivnu_sqlslong(int row,ir_T* ir,SQLPOINTER target,SQLINTEGER buflen,
			  SQLINTEGER* indi)
{
  SQLCHAR *src;
  sword ret;
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
		  (SQLHANDLE)ir->desc->stmt,0,"si",
		  NULL,"ocivnu_sqlslong",
		  "BufferLength",buflen);
#endif
  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  ret=OCINumberToInt(ir->desc->dbc->oci_err,
		     (OCINumber*)src,sizeof(long),OCI_NUMBER_SIGNED,
		     target);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  if(indi)
    *indi=sizeof(long);
  return SQL_SUCCESS;
}
/*
 *  ocivnu_sqlulong
 */
SQLRETURN ocivnu_sqlulong(int row,ir_T* ir,SQLPOINTER target,SQLINTEGER buflen,
			  SQLINTEGER* indi)
{
  SQLCHAR *src;
  sword ret;
  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  ret=OCINumberToInt(ir->desc->dbc->oci_err,
		     (OCINumber*)src,sizeof(long),OCI_NUMBER_UNSIGNED,
		     target);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  if(indi)
    *indi=sizeof(long);
  return SQL_SUCCESS;
}
/*
 *  ocivnu_sqldouble
 */
SQLRETURN ocivnu_sqldouble(int row,ir_T* ir,SQLPOINTER target,SQLINTEGER buflen,
			   SQLINTEGER* indi)
{
  SQLCHAR *src;
  sword ret;
  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  ret=OCINumberToReal(ir->desc->dbc->oci_err,
		      (OCINumber*)src,sizeof(double),
		      target);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  if(indi)
    *indi=sizeof(double);
  return SQL_SUCCESS;
}
/*
 *  ocivnu_sqlfloat
 */
SQLRETURN ocivnu_sqlfloat(int row,ir_T* ir,SQLPOINTER target,SQLINTEGER buflen,
			  SQLINTEGER* indi)
{
  SQLCHAR *src;
  sword ret;
  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);
  ret=OCINumberToReal(ir->desc->dbc->oci_err,
		      (OCINumber*)src,sizeof(float),
		      target);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  if(indi)
    *indi=sizeof(float);
  return SQL_SUCCESS;
}

/*
 *  ocivnu_sqlsshort
 */
SQLRETURN ocivnu_sqlsshort(int row,ir_T* ir,SQLPOINTER target,SQLINTEGER buflen,
			   SQLINTEGER* indi)
{
  SQLCHAR *src;
  sword ret;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  ret=OCINumberToInt(ir->desc->dbc->oci_err,
		     (OCINumber*)src,sizeof(short),OCI_NUMBER_SIGNED,
		     target);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  if(indi)
    *indi=sizeof(short);
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		  (SQLHANDLE)ir->desc->stmt,0,"sii",
		  NULL,"ocivnu_sqlshort",
		  "BufferLength",buflen,
		  "Target",target);
#endif
  return SQL_SUCCESS;
}
/*
 *  ocivnu_sqlushort
 */
SQLRETURN ocivnu_sqlushort(int row,ir_T* ir,SQLPOINTER target,SQLINTEGER buflen,
			   SQLINTEGER* indi)
{
  SQLCHAR *src;
  sword ret;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  ret=OCINumberToInt(ir->desc->dbc->oci_err,
		     (OCINumber*)src,sizeof(unsigned short),OCI_NUMBER_UNSIGNED,
		     target);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  if(indi)
    *indi=sizeof(unsigned short);
  return SQL_SUCCESS;
}
/*
 *  ocivnu_sqlsbigint
 */
SQLRETURN ocivnu_sqlsbigint(int row,ir_T* ir,SQLPOINTER target,
			    SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;
  sword ret;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  ret=OCINumberToInt(ir->desc->dbc->oci_err,
		     (OCINumber*)src,sizeof(int64_t),OCI_NUMBER_SIGNED,
		     target);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  if(indi)
    *indi=sizeof(int64_t);
  return SQL_SUCCESS;
}
/*
 *  ocivnu_sqlubigint
 */
SQLRETURN ocivnu_sqlubigint(int row,ir_T* ir,SQLPOINTER target,
			    SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;
  sword ret;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);
  ret=OCINumberToInt(ir->desc->dbc->oci_err,
		     (OCINumber*)src,sizeof(int64_t),OCI_NUMBER_UNSIGNED,
		     target);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  if(indi)
    *indi=sizeof(int64_t);
  return SQL_SUCCESS;
}

/*
 *  ocivnu_sqlutinyint
 */
SQLRETURN ocivnu_sqlutinyint(int row,ir_T* ir,SQLPOINTER target,
			     SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;
  sword ret;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  ret=OCINumberToInt(ir->desc->dbc->oci_err,
		     (OCINumber*)src,sizeof(char),OCI_NUMBER_UNSIGNED,
		     target);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  if(indi)
    *indi=sizeof(char);
  return SQL_SUCCESS;
}

/*
 *  ocivnu_sqlstinyint
 */
SQLRETURN ocivnu_sqlstinyint(int row,ir_T* ir,SQLPOINTER target,
			     SQLINTEGER buflen, SQLINTEGER* indi)
{
  /*
    SQLCHAR *src;
    src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);
  */
  sword ret;

  ret=OCINumberToInt(ir->desc->dbc->oci_err,
		     (OCINumber*)ir->data_ptr,sizeof(char),OCI_NUMBER_SIGNED,
		     target);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      return SQL_ERROR;
    }
  if(indi)
    *indi=sizeof(char);
  return SQL_SUCCESS;
}

/*
 *  ocivnu_sqlnumeric
 */
SQLRETURN ocivnu_sqlnumeric(int row,ir_T* ir,SQLPOINTER target,
			    SQLINTEGER buflen, SQLINTEGER* indi)
{
  /* 
   * There is no nice, clean easy way of doing this conversion.
   *
   * This way works but is not particularly fast. 
   * If anyone would like to write some assembler for their platform...
   * The spec for Oracle VARNUM can be found in 
   * http://technet.oracle.com/doc/server.815/a67846/datatype.htm
   */
  unsigned char *src;
  SQL_NUMERIC_STRUCT *num=(SQL_NUMERIC_STRUCT*)target;
  unsigned char *tval,*sval,*end_ptr;
  register unsigned char posn,carry;
  register unsigned char num_length;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  memset(num,0,sizeof(SQL_NUMERIC_STRUCT));

  /* Byte 1 is the length... precision (roughly)*/
  num_length=*((unsigned char*)src++);
  /*printf("num_length [%d]\n",num_length);*/
  if(num_length<=1)
    {
      num->scale=0;
      num->sign=1;
      num->precision=0;
      return(SQL_SUCCESS);
    }

  /* Byte 2 bit 1 (high) is the sign, which is 1 for positive */
  num->sign=(SQLSCHAR)(*((char*)src)&0x80)>>7;

  /* The lower 7 bits of byte 2 are the exponent */
  num->scale=(SQLSCHAR)*(char*)src&0x7F;
  src++;
  num->scale-=64;

  /* Set the tval pointer to be the lowest order byte of the */ 
  /* numeric */
  tval=num->val;

  sval=(unsigned char*)src;
  if(num->sign)
    {
      num_length--;
      num->precision=num_length*2;
      end_ptr=((unsigned char*)src)+num_length;
      while(sval<end_ptr)
        { 
	  *sval=*sval-1;
	  sval++;
        }
      num->scale*=-1;
    }
  else
    {
      num_length-=2;
      num->precision=num_length*2;
      num->scale++;
      end_ptr=((unsigned char*)src)+num_length;
      while(sval<end_ptr)
        { 
	  *sval=101-*sval;
	  sval++;
        }
    }

  /* Scale needs massaging */
  num->scale+=(num_length);
  num->scale*=2;

  /* Time to spill the beans and own up to what's going on */
  /* The way this conversion is done is to successively */
  /* divide the Oracle number by two, taking note of the carry */
  /* at the end of each division. For each successive division the */
  /* value of the carry is power 2 of the previous... so we can mask */
  /* these into to sql_numeric... fun eh? */

  while(src<end_ptr)
    {
      *tval=0;
      for(posn=0;posn<8;posn++)
        {
	  carry=0;
	  for(sval=src;end_ptr>sval;sval++)
            { 
	      if(carry)
		(*sval)+=100;
	      carry=(*sval)&0x01;
	      (*sval)=(*sval)>>1;
            }
	  if(end_ptr>src)
	    {
	      if(!*(unsigned char*)src)
		src++;
	    }
	  *tval|=(carry<<posn);
        }
      tval++;
    }
  return(SQL_SUCCESS);
}

/*
 *  ociint_sqlnts
 */
SQLRETURN ociint_sqlnts(int row,ir_T* ir,SQLPOINTER target,
			SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;
  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);
  if(buflen<2)
    {
      ood_post_diag((hgeneric*)ir->desc->stmt,ERROR_ORIGIN_22003,
		    ir->col_num,"", ERROR_MESSAGE_22003,
		    __LINE__,0,"",ERROR_STATE_22003, __FILE__,__LINE__);
      return SQL_ERROR;
    }
  sprintf( ((char*)target),"%d",*((int*)src));
  if(indi)
    *indi=strlen(target); /* XXX needs to be set properly */
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		  (SQLHANDLE)ir->desc->stmt,0,"sii",
		  NULL,"ocivnu_sqlshort",
		  "BufferLength",buflen,
		  "Target",target);
#endif
  return SQL_SUCCESS;
}

/*
 *  ociint_sqlslong
 */
SQLRETURN ociint_sqlslong(int row,ir_T* ir,SQLPOINTER target,
			  SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  *((long*)target)=*((int*)src);
  if(indi)
    *indi=sizeof(long);
  return SQL_SUCCESS;
}
/*
 *  ociint_sqlulong
 */
SQLRETURN ociint_sqlulong(int row,ir_T* ir,SQLPOINTER target,
			  SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  *((unsigned long*)target)=*((int*)src);
  if(indi)
    *indi=sizeof(unsigned long);
  return SQL_SUCCESS;
}
/*
 *  ociint_sqldouble
 */
SQLRETURN ociint_sqldouble(int row,ir_T* ir,SQLPOINTER target,
			   SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  *((double*)target)=*((int*)src);
  if(indi)
    *indi=sizeof(double);
  return SQL_SUCCESS;
}
/*
 *  ociint_sqlfloat
 */
SQLRETURN ociint_sqlfloat(int row,ir_T* ir,SQLPOINTER target,
			  SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  *((float*)target)=(float)*((int*)src);
  if(indi)
    *indi=sizeof(float);
  return SQL_SUCCESS;
}
/*
 *  ociint_sqlsshort
 */
SQLRETURN ociint_sqlsshort(int row,ir_T* ir,SQLPOINTER target,
			   SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  *((short*)target)=*((int*)src);
  if(indi)
    *indi=sizeof(short);
  return SQL_SUCCESS;
}
/*
 *  ociint_sqlushort
 */
SQLRETURN ociint_sqlushort(int row,ir_T* ir,SQLPOINTER target,
			   SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;
  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  *((unsigned short*)target)=*((int*)src);
  if(indi)
    *indi=sizeof(unsigned short);

  return SQL_SUCCESS;
}
/*
 *  ociint_sqlsbigint
 */
SQLRETURN ociint_sqlsbigint(int row,ir_T* ir,SQLPOINTER target,
			    SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  *((int64_t*)target)=*((int64_t*)src);
  if(indi)
    *indi=sizeof(int64_t);
  return SQL_SUCCESS;
}
/*
 *  ociint_sqlubigint
 */
SQLRETURN ociint_sqlubigint(int row,ir_T* ir,SQLPOINTER target,
			    SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  *((u_int64_t*)target)=*((u_int64_t*)src);
  if(indi)
    *indi=sizeof(u_int64_t);
  return SQL_SUCCESS;
}
/*
 *  ociint_sqlutinyint
 */
SQLRETURN ociint_sqlutinyint(int row,ir_T* ir,SQLPOINTER target,
			     SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;
  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);
  *((unsigned char*)target)=*((unsigned char*)src);
  if(indi)
    *indi=sizeof(unsigned char);
  return SQL_SUCCESS;
}
/*
 *  ociint_sqlstinyint
 */
SQLRETURN ociint_sqlstinyint(int row,ir_T* ir,SQLPOINTER target,
			     SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  *((signed char*)target)=*((signed char*)src);
  if(indi)
    *indi=sizeof(signed char);
  return SQL_SUCCESS;
}
/*
 *  ociflt_sqlnts
 */
SQLRETURN ociflt_sqlnts(int row,ir_T* ir,SQLPOINTER target,SQLINTEGER buflen,
			SQLINTEGER* indi)
{
  SQLCHAR *src;
  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);
  sprintf(((char*)target),"%f",*((double*)src));
  if(indi)
    *indi=strlen(target);
  return SQL_SUCCESS;
}
/*
 *  ociflt_sqldouble
 */
SQLRETURN ociflt_sqldouble(int row,ir_T* ir,SQLPOINTER target,
			   SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  *((double*)target)=*((double*)src);
  if(indi)
    *indi=sizeof(double);
  return SQL_SUCCESS;
}

/*
 *  ociflt_sqlfloat
 */
SQLRETURN ociflt_sqlfloat(int row,ir_T* ir,SQLPOINTER target,SQLINTEGER buflen,
			  SQLINTEGER* indi)
{
  *((float**)target)[row]=(float)*((double**)ir->data_ptr)[row];
  if(indi)
    *indi=sizeof(float);
  return SQL_SUCCESS;
}

/*
 *  ociflt_sqlnumeric
 */
SQLRETURN ociflt_sqlnumeric(int row,ir_T* ir,SQLPOINTER target,
			    SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQL_NUMERIC_STRUCT *num=(SQL_NUMERIC_STRUCT*)target;
  int length, i;
  double tmp,*val=(double*)ir->data_ptr+(ir->data_size*row);
	
  num->precision=15;
  num->scale=0;
  if(*val<(double)0.0)
    {
      num->sign=0;
      *val=-*val;
    }
  else
    {
      num->sign=1;
    }
  for(i=0;i<SQL_MAX_NUMERIC_LEN;num->val[i++]=0);
  length=0;
  while(*val>0.0)
    {
      tmp=fmod(*val,256.0);
      modf(tmp,&tmp);
      *val=*val/256.0;
      modf(*val,val);
      num->val[length++]=(SQLCHAR)tmp;
    }

  if(indi)
    *indi=sizeof(SQL_NUMERIC_STRUCT);
  return SQL_SUCCESS;
}

/*
 *  ocidat_sqlnts
 */
SQLRETURN ocidat_sqlnts(int row,ir_T* ir,SQLPOINTER target,SQLINTEGER buflen,
			SQLINTEGER* indi)
{
  SQLCHAR *src;
  sword ret;
  ub4 buf_siz=(ub4)buflen;
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		  (SQLHANDLE)ir->desc->stmt,0,"sih",
		  NULL,"ocidat_sqlnts",
		  "BufferLength",buflen,
		  "&Target",(long)target);
#endif

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  ret=OCIDateToText(ir->desc->dbc->oci_err,((OCIDate*)src),
		    (unsigned char*)"YYYY-MM-DD HH24:MI:SS",
		    21,NULL,0,&buf_siz,target);
  if(ret)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      if(indi)
	*indi=SQL_NULL_DATA;
      return SQL_ERROR;
    }
  if(indi)
    *indi=strlen(target);
  return SQL_SUCCESS;
}

/*
 *  ocidat_sqltimestamp
 */
SQLRETURN ocidat_sqltimestamp(int row,ir_T* ir,SQLPOINTER target,
			      SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;
  /*typedef struct tagTIMESTAMP_STRUCT*/
  /*{*/
  /*SQLSMALLINT    year;*/
  /*SQLUSMALLINT   month;*/
  /*SQLUSMALLINT   day;*/
  /*SQLUSMALLINT   hour;*/
  /*SQLUSMALLINT   minute;*/
  /*SQLUSMALLINT   second;*/
  /*SQLUINTEGER    fraction;*/
  /*} TIMESTAMP_STRUCT;*/
  SQL_TIMESTAMP_STRUCT *ts=(SQL_TIMESTAMP_STRUCT*)target;
  ub1 h,m,s;
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		  (SQLHANDLE)ir->desc->stmt,0,"sih",
		  NULL,"ocidat_sqltimestamp",
		  "BufferLength",buflen,
		  "&Target",(long)target);
#endif

  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  OCIDateGetDate((OCIDate*)src,(sb2*)&ts->year,
		 (ub1*)&m,(ub1*)&s);
  ts->month=(SQLUSMALLINT)m;
  ts->day=(SQLUSMALLINT)s;
  OCIDateGetTime((OCIDate*)ir->data_ptr,
		 (ub1*)&h,(ub1*)&m,(ub1*)&s);
  ts->hour=(SQLUSMALLINT)h;
  ts->minute=(SQLUSMALLINT)m;
  ts->second=(SQLUSMALLINT)s;

  ts->fraction=0;
  if(indi)
    *indi=sizeof(SQL_TIMESTAMP_STRUCT);
  return SQL_SUCCESS;
}

/*
 *  ocidat_sqltime
 */
SQLRETURN ocidat_sqltime(int row,ir_T* ir,SQLPOINTER target,
			 SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *src;
  /*typedef struct tagTIME_STRUCT*/
  /*{*/
  /*SQLUSMALLINT   hour;*/
  /*SQLUSMALLINT   minute;*/
  /*SQLUSMALLINT   second;*/
  /*} TIME_STRUCT;*/
  SQL_TIME_STRUCT *ts=(SQL_TIME_STRUCT*)target;
  ub1 h,m,s;
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		  (SQLHANDLE)ir->desc->stmt,0,"sih",
		  NULL,"ocidat_sqltime",
		  "BufferLength",buflen,
		  "&Target",(long)target);
#endif
  src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  OCIDateGetTime((OCIDate*)src,
		 (ub1*)&h,(ub1*)&m,(ub1*)&s);
  ts->hour=(SQLUSMALLINT)h;
  ts->minute=(SQLUSMALLINT)m;
  ts->second=(SQLUSMALLINT)s;
  if(indi)
    *indi=sizeof(SQL_TIME_STRUCT);
  return SQL_SUCCESS;
}

/*
 *  ocidat_sqldate
 */
SQLRETURN ocidat_sqldate(int row,ir_T* ir,SQLPOINTER target,
			 SQLINTEGER buflen, SQLINTEGER* indi)
{
  /*	SQLCHAR *src; */
  /*typedef struct tagDATE_STRUCT*/
  /*{*/
  /*SQLSMALLINT    year;*/
  /*SQLUSMALLINT   month;*/
  /*SQLUSMALLINT   day;*/
  /*} DATE_STRUCT;*/
  SQL_DATE_STRUCT *ts=(SQL_DATE_STRUCT*)target;
  ub1 month;
  ub1 day;
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		  (SQLHANDLE)ir->desc->stmt,0,"sih",
		  NULL,"ocidat_sqldate",
		  "BufferLength",buflen,
		  "&Target",(long)target);
#endif
  /*src=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);*/

  OCIDateGetDate((OCIDate*)ir->data_ptr,(sb2*)&ts->year,
		 (ub1*)&month,(ub1*)&day);
  ts->month=(SQLUSMALLINT)month;
  ts->day=(SQLUSMALLINT)day;
  if(indi)
    *indi=sizeof(SQL_DATE_STRUCT);
  return SQL_SUCCESS;
}
/*
 * ocilob_sqllvc
 *
 * Converts a CLOB to a SQL_LONGVARCHAR (or anything it gets passed actually)
 */
SQLRETURN ocilob_sqllvc(int row,ir_T* ir ,SQLPOINTER target,
			SQLINTEGER buflen, SQLINTEGER* indi)
{
  sword ret;
  ub4 amtp;

  memset(target,'\0',buflen);
	
  OCILobGetLength(ir->desc->stmt->dbc->oci_svc,ir->desc->stmt->dbc->oci_err,
		  ir->locator[row],&ir->lobsiz);

  amtp=ir->lobsiz-ir->posn;
  if(amtp>(unsigned)buflen)
    amtp=(unsigned)buflen;

  ret=OCILobRead(ir->desc->stmt->dbc->oci_svc,ir->desc->stmt->dbc->oci_err,
		 ir->locator[row],&amtp,ir->posn,target,
		 (ub4)ir->lobsiz,
		 NULL,NULL,0,
		 SQLCS_IMPLICIT);
  if(ret==OCI_ERROR)
    {
      ood_driver_error(ir->desc->stmt,ret,__FILE__,__LINE__);
      if(indi)
	*indi=SQL_NULL_DATA;
      return SQL_ERROR;
    }
  ir->posn+=amtp;
  if(indi)
    *indi=amtp;
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		  (SQLHANDLE)ir->desc->stmt,SQL_SUCCESS,"ss",
		  NULL,"ocilob_sqllvc",
		  "Target",target);
#endif
  if(ret!=OCI_ERROR)
    {
      if(ir->posn<ir->lobsiz)
	{
	  ood_post_diag((hgeneric*)ir->desc->stmt,ERROR_ORIGIN_01004,0,"",
			ERROR_MESSAGE_01004,
			__LINE__,0,"",ERROR_STATE_01004,
			__FILE__,__LINE__);

	  return SQL_SUCCESS_WITH_INFO;
	}
      else
	return SQL_SUCCESS;
    }
  else
    return SQL_ERROR;
}

SQLRETURN ocilob_sqllvb(int row,ir_T* ir ,SQLPOINTER target,
			SQLINTEGER buflen, SQLINTEGER* indi)
{
  return(ocilob_sqllvc(row,ir,target,buflen,indi));
}
/*
 * ocidty_sqlint
 *
 * Converts an oracle type string into a SQL data type... see SQLColumns.c
 */
SQLRETURN ocidty_sqlint(int row,ir_T* ir ,SQLPOINTER target,
			SQLINTEGER buflen, SQLINTEGER* indi)
{
  SQLCHAR *str;
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ir->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
		  (SQLHANDLE)ir->desc->stmt,0,"sihsh",
		  NULL,"ocidty_sqlint",
		  "BufferLength",buflen,
		  "&ir->data_ptr",(long)ir->data_ptr,
		  "ir->data_ptr",ir->data_ptr,
		  "&Target",(long)target);
#endif

  str=((SQLCHAR*)ir->data_ptr)+(row*ir->data_size);

  if(!strncmp((char*)str,"VARCHAR2",8))
    {
      *((int*)target)=SQL_C_CHAR;
      return SQL_SUCCESS;
    }
  if(!strncmp((char*)str,"CHAR",4))
    {
      *((int*)target)=SQL_C_CHAR;
      return SQL_SUCCESS;
    }
  if(!strncmp((char*)str,"CLOB",4)||!strncmp((char*)str,"LONG ",5))
    {
      *((int*)target)=SQL_LONGVARCHAR;
      return SQL_SUCCESS;
    }
  if(!strncmp((char*)str,"NUMBER",6))
    {
      *((int*)target)=SQL_C_NUMERIC;
      return SQL_SUCCESS;
    }
  if(!strncmp((char*)str,"DATE",4))
    {
      *((int*)target)=SQL_TYPE_TIMESTAMP;
      return SQL_SUCCESS;
    }
            
  *((int*)target)=SQL_C_CHAR;
  if(indi)
    *indi=30; /* XXX needs to be set properly... */
  return SQL_SUCCESS;
}
/*
 * ...and to string
 */
/*
 *  ocidty_sqlnts
 */
SQLRETURN ocidty_sqlnts(int row,ir_T* ir ,SQLPOINTER target,SQLINTEGER buflen,
			SQLINTEGER* indi)
{
  int i;
  ocidty_sqlint(row,ir,&i,sizeof(int),indi);
  sprintf( ((char*)target)+(buflen*row),"%d",i);
  if(indi)
    *indi=30; /* XXX needs to be set properly... */
  return SQL_SUCCESS;
}

sword ood_driver_bind_param(hStmt_T *stmt,int parmnum)
{
  sword ret;
  OCIBind *ocibind;
  ap_T* ap=&stmt->current_ap->recs.ap[parmnum];
  ip_T* ip=&stmt->current_ip->recs.ip[parmnum];
  void* bind_ptr;
  
  assert(IS_VALID(stmt));
  assert(IS_VALID(ap));
  assert(IS_VALID(ip));
  
  ip->col_num=parmnum;
  ip->desc=stmt->current_ip;
  
  /*
   * There are several types where no converswions that Oracle can't do
   * are likely to arise. 
   */
  if(debugLevel()>0){
    printf("ood_driver_bind_param::line %d concise_type=%d %s\n"
	   ,__LINE__,ap->concise_type,odbc_var_type(ap->concise_type));
  }
  
  switch(ap->concise_type)
    {
    case SQL_C_USHORT:
    case SQL_C_SSHORT:
      ip->data_size=sizeof(short);
      ip->data_type=SQLT_INT;
      ip->data_ptr=NULL; /* not malloc'd, we use APD directly */
      bind_ptr=ap->data_ptr;
      break;
    case SQL_INTEGER:
    case SQL_C_ULONG:
    case SQL_C_SLONG:
      ip->data_size=sizeof(long);
      ip->data_type=SQLT_INT;
      ip->data_ptr=NULL; /* not malloc'd, we use APD directly */
      bind_ptr=ap->data_ptr;
      break;
      
    case SQL_C_UBIGINT:
    case SQL_C_SBIGINT:
      ip->data_size=sizeof(SQLBIGINT);
      ip->data_type=SQLT_INT;
      ip->data_ptr=NULL; /* not malloc'd, we use APD directly */
      bind_ptr=ap->data_ptr;
      break;
      
    case SQL_C_DOUBLE:
      ip->data_size=sizeof(double);
      ip->data_type=SQLT_FLT;
      ip->data_ptr=NULL;
      bind_ptr=ap->data_ptr;
      break;
      
    case SQL_C_FLOAT:
      ip->data_size=sizeof(float);
      ip->data_type=SQLT_FLT;
      ip->data_ptr=NULL;
      bind_ptr=ap->data_ptr;
      break;
      
    case SQL_C_BINARY:
      ip->data_size=ap->octet_length;
      ip->data_type=SQLT_BIN;
      ip->data_ptr=NULL;
      bind_ptr=ap->data_ptr;
      break;
      
      
      /*
       * There are however other types that we have to look out for
       */
      
    case SQL_C_TIMESTAMP:
      {
	char datetxt[25];
	SQL_TIMESTAMP_STRUCT *ts=(SQL_TIMESTAMP_STRUCT*)ap->data_ptr;
	ip->data_ptr=(OCIDate*)ORAMALLOC(sizeof(OCIDate));
	ip->data_type=SQLT_ODT;
	ip->data_size=sizeof(OCIDate);
	bind_ptr=ip->data_ptr;
	switch(ap->bind_target_type)
	  {
	  case SQL_TYPE_TIMESTAMP:
	    sprintf(datetxt,"%.4d%.2d%.2d%.2d%.2d%.2d",
		    ts->year,ts->month+1,ts->day,
		    ts->hour,ts->minute,ts->second);
	    ret=OCIDateFromText(stmt->dbc->oci_err,
				(text*)datetxt,14,
				(text*)"YYYYMMDDHH24MISS",16,
				(text*)"",0,(OCIDate*)ip->data_ptr);
	    if(ret)
	      {
		ood_driver_error(stmt,ret,__FILE__,__LINE__);
		return(ret);
	      }
	    break;
	    
	  case SQL_TYPE_TIME:
	    sprintf(datetxt,"%.2d%.2d%.2d",
		    ts->year,ts->month+1,ts->day);
	    ret=OCIDateFromText(stmt->dbc->oci_err,
				(text*)datetxt,6,
				(text*)"HH24MISS",8,
				(text*)"",0,(OCIDate*)ip->data_ptr);
	    if(ret)
	      {
		ood_driver_error(stmt,ret,__FILE__,__LINE__);
		return(ret);
	      }
	    break;
	    
	  case SQL_TYPE_DATE:
	    sprintf(datetxt,"%.4d%.2d%.2d",
		    ts->year,ts->month+1,ts->day);
	    ret=OCIDateFromText(stmt->dbc->oci_err,
				(text*)datetxt,8,
				(text*)"YYYYMMDD",8,
				(text*)"",0,(OCIDate*)ip->data_ptr);
	    if(ret)
	      {
		ood_driver_error(stmt,ret,__FILE__,__LINE__);
		return(ret);
	      }
	    break;
	  }
      }
    case SQL_C_TIME:
      {
	char datetxt[25];
	SQL_TIMESTAMP_STRUCT *ts=(SQL_TIMESTAMP_STRUCT*)ap->data_ptr;
	ip->data_ptr=(OCIDate*)ORAMALLOC(sizeof(OCIDate));
	ip->data_type=SQLT_ODT;
	ip->data_size=sizeof(OCIDate);
	bind_ptr=ip->data_ptr;
	sprintf(datetxt,"%.2d%.2d%.2d",
		ts->year,ts->month+1,ts->day);
	ret=OCIDateFromText(stmt->dbc->oci_err,
			    (text*)datetxt,6,
			    (text*)"HH24MISS",8,
			    (text*)"",0,(OCIDate*)ip->data_ptr);
	if(ret)
	  {
	    ood_driver_error(stmt,ret,__FILE__,__LINE__);
	    return(ret);
	  }
      }
      break;
      
    case SQL_C_DATE:
      {
	char datetxt[25];
	SQL_TIMESTAMP_STRUCT *ts=(SQL_TIMESTAMP_STRUCT*)ap->data_ptr;
	ip->data_ptr=(OCIDate*)ORAMALLOC(sizeof(OCIDate));
	ip->data_type=SQLT_ODT;
	ip->data_size=sizeof(OCIDate);
	bind_ptr=ip->data_ptr;
	sprintf(datetxt,"%.4d%.2d%.2d",
		ts->year,ts->month+1,ts->day);
	ret=OCIDateFromText(stmt->dbc->oci_err,
			    (text*)datetxt,8,
			    (text*)"YYYYMMDD",8,
			    (text*)"",0,(OCIDate*)ip->data_ptr);
	if(ret)
	  {
	    ood_driver_error(stmt,ret,__FILE__,__LINE__);
	    return(ret);
	  }
      }
      break;


     

    case SQL_CHAR:
    case SQL_VARCHAR:
      
      ip->data_type=SQLT_CHR;
      ip->data_ptr=NULL;
      bind_ptr=ap->data_ptr;
      if(strlen(bind_ptr)<ap->octet_length)
	ip->data_size=strlen(bind_ptr);
      else
	ip->data_size=ap->octet_length;
      
      break;
      
     
    default:
      switch(ap->bind_target_type)
	{
	case SQL_TYPE_DATE:
	  {
	    sword ret;
	    ip->data_ptr=(OCIDate*)ORAMALLOC(sizeof(OCIDate));
	    /*
	     * Dates are expected to be in the format 
	     * "YYYY-MM-DD", but they can also be
	     * odbc escaped, ie "{d 'YYYY-..."
	     */
	    if(!strncmp(ap->data_ptr,"{d ",3))
	      {
		char *date_start=strchr(ap->data_ptr,'\'');
		date_start++;
		ret=OCIDateFromText(stmt->dbc->oci_err,
				    (text*)date_start,10,
				    (text*)"YYYY-MM-DD",10,
				    (text*)"",0,(OCIDate*)ip->data_ptr);
		if(ret)
		  {
		    ood_driver_error(stmt,ret,__FILE__,__LINE__);
		    return(ret);
		  }
		
	      }
	    else
	      {
		ret=OCIDateFromText(
				    stmt->dbc->oci_err,
				    (text*)ip->data_ptr,10,
				    (text*)"YYYY-MM-DD",10,
				    (text*)"",0,(OCIDate*)ip->data_ptr);
		if(ret)
		  {
		    ood_driver_error(stmt,ret,__FILE__,__LINE__);
		    return(ret);
		  }
	      }
	    ip->data_type=SQLT_ODT;
	    ip->data_size=sizeof(OCIDate);
	    bind_ptr=ip->data_ptr;
	  }
	  break;
	case SQL_TYPE_TIME:
	  {
	    sword ret;
	    ip->data_ptr=(OCIDate*)ORAMALLOC(sizeof(OCIDate));
	    /*
	     * Times are expected to be in the format 
	     * "HH:MI:SS", but they can also be
	     * odbc escaped, ie "{t 'HH:MI..."
	     */
	    if(!strncmp(ap->data_ptr,"{t ",3))
	      {
		char *date_start=strchr(ap->data_ptr,'\'');
		date_start++;
		ret=OCIDateFromText(stmt->dbc->oci_err,
				    (text*)date_start,8,
				    (text*)"HH24:MI:SS",10,
				    (text*)"",0,(OCIDate*)ip->data_ptr);
		if(ret)
		  {
		    ood_driver_error(stmt,ret,__FILE__,__LINE__);
		    return(ret);
		  }
		
	      }
	    else
	      {
		ret=OCIDateFromText(
				    stmt->dbc->oci_err,
				    (text*)ip->data_ptr,8,
				    (text*)"HH24:MI:SS",10,
				    (text*)"",0,(OCIDate*)ip->data_ptr);
		if(ret)
		  {
		    ood_driver_error(stmt,ret,__FILE__,__LINE__);
		    return(ret);
		  }
	      }
	    ip->data_type=SQLT_ODT;
	    ip->data_size=sizeof(OCIDate);
	    bind_ptr=ip->data_ptr;
	  }
	  break;
	  
	case SQL_TYPE_TIMESTAMP:
	  {
	    sword ret;
	    ip->data_ptr=(OCIDate*)ORAMALLOC(sizeof(OCIDate));
	    /*
	     * Timestamps are expected to be in the format 
	     * "YYYY-MM-DD HH:MI:SS", but they can also be
	     * odbc escaped, ie "{ts 'YYYY-..."
	     */
	    if(!strncmp(ap->data_ptr,"{ts ",4))
	      {
		char *date_start=strchr(ap->data_ptr,'\'');
		date_start++;
		ret=OCIDateFromText(
				    stmt->dbc->oci_err,
				    (text*)date_start,19,
				    (text*)"YYYY-MM-DD HH24:MI:SS",21,
				    (text*)"",0,(OCIDate*)ip->data_ptr);
		if(ret)
		  {
		    ood_driver_error(stmt,ret,__FILE__,__LINE__);
		    return(ret);
		  }
		
	      }
	    else
	      {
		ret=OCIDateFromText(
				    stmt->dbc->oci_err,
				    (text*)ip->data_ptr,19,
				    (text*)"YYYY-MM-DD HH24:MI:SS",21,
				    (text*)"",0,(OCIDate*)ip->data_ptr);
		if(ret)
		  {
		    ood_driver_error(stmt,ret,__FILE__,__LINE__);
		    return(ret);
		  }
	      }
	    ip->data_type=SQLT_ODT;
	    ip->data_size=sizeof(OCIDate);
	    bind_ptr=ip->data_ptr;
	  }
	  break;
	  
	default:
	  /*case SQL_INTEGER:*/
	case SQL_DECIMAL:
	case SQL_NUMERIC:
	case SQL_SMALLINT:
	case SQL_REAL:
	case SQL_FLOAT:
	case SQL_DOUBLE:
	case SQL_TINYINT:
	case SQL_BIGINT:
	case SQL_BIT:
	case SQL_BINARY:
	case SQL_LONGVARCHAR:
	case SQL_VARBINARY:
	case SQL_LONGVARBINARY:
	  ip->data_size=ap->octet_length;
	  ip->data_type=SQLT_CHR;
	  ip->data_ptr=NULL;
	  bind_ptr=ap->data_ptr;
	  break;
	}
      break;
    }
  
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
  ood_log_message(ip->desc->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		  (SQLHANDLE)ip->desc->stmt,0,"siiiiiis",
		  NULL,"ood_driver_bind_parm",
		  "parmnum",parmnum,
		  "ap->octet_length",ap->octet_length,
		  "ip->data_size",ip->data_size,
		  "ap->concise_type",ap->concise_type,
		  "ap->bind_target_type",ap->bind_target_type,
		  "ip->data_type",ip->data_type,
		  "bind_ptr",bind_ptr
		  );
#endif
  if(debugLevel()>0){
    printf("ood_driver_bind_param::OCIBindByPos parm=%d size=%d type=%d %s\n",
	   parmnum,ip->data_size,ip->data_type,oci_var_type(ip->data_type));
  }
  ret=OCIBindByPos(stmt->oci_stmt,&ocibind,stmt->dbc->oci_err,
		   (ub4)parmnum,bind_ptr,
		   ip->data_size , ip->data_type,
		   /*&ap->bind_indicator*/ 0,
		   0,0,0,0,OCI_DEFAULT);
  return(ret);
}
char *
oci_status_name(sword status)
{
  
  switch (status) {
  case OCI_SUCCESS:		return "SUCCESS";
  case OCI_SUCCESS_WITH_INFO:	return "SUCCESS_WITH_INFO";
  case OCI_NEED_DATA:		return "NEED_DATA";
  case OCI_NO_DATA:		return "NO_DATA";
  case OCI_ERROR:		return "ERROR";
  case OCI_INVALID_HANDLE:	return "INVALID_HANDLE";
  case OCI_STILL_EXECUTING:	return "STILL_EXECUTING";
  case OCI_CONTINUE:		return "CONTINUE";
  }
  return "UNKNOWN OCI STATUS";
}


char *
oci_stmt_type_name(int stmt_type)
{
  
  switch (stmt_type) {

  case OCI_STMT_SELECT:	  return "SELECT";
  case OCI_STMT_UPDATE:	  return "UPDATE";
  case OCI_STMT_DELETE:	  return "DELETE";
  case OCI_STMT_INSERT:	  return "INSERT";
  case OCI_STMT_CREATE:	  return "CREATE";
  case OCI_STMT_DROP:	  return "DROP";
  case OCI_STMT_ALTER:	  return "ALTER";
  case OCI_STMT_BEGIN:	  return "BEGIN";
  case OCI_STMT_DECLARE:  return "DECLARE";

  }
  
  return "UNKOWN OCI STMT TYPE";
}


char *
oci_hdtype_name(ub4 hdtype)
{
    
  switch (hdtype) {
    /* Handles */
  case OCI_HTYPE_ENV:                 return "OCI_HTYPE_ENV";
  case OCI_HTYPE_ERROR:               return "OCI_HTYPE_ERROR";
  case OCI_HTYPE_SVCCTX:              return "OCI_HTYPE_SVCCTX";
  case OCI_HTYPE_STMT:                return "OCI_HTYPE_STMT";
  case OCI_HTYPE_BIND:                return "OCI_HTYPE_BIND";
  case OCI_HTYPE_DEFINE:              return "OCI_HTYPE_DEFINE";
  case OCI_HTYPE_DESCRIBE:            return "OCI_HTYPE_DESCRIBE";
  case OCI_HTYPE_SERVER:              return "OCI_HTYPE_SERVER";
  case OCI_HTYPE_SESSION:             return "OCI_HTYPE_SESSION";
  case OCI_HTYPE_TRANS:               return "OCI_HTYPE_TRANS";
  case OCI_HTYPE_COMPLEXOBJECT:       return "OCI_HTYPE_COMPLEXOBJECT";
  case OCI_HTYPE_SECURITY:            return "OCI_HTYPE_SECURITY";
    
    /* Descriptors */
  case OCI_DTYPE_LOB:			return "OCI_DTYPE_LOB";
  case OCI_DTYPE_SNAP:		return "OCI_DTYPE_SNAP";
  case OCI_DTYPE_RSET:		return "OCI_DTYPE_RSET";
  case OCI_DTYPE_PARAM:		return "OCI_DTYPE_PARAM";
  case OCI_DTYPE_ROWID:		return "OCI_DTYPE_ROWID";
  case OCI_DTYPE_COMPLEXOBJECTCOMP:	return "OCI_DTYPE_COMPLEXOBJECTCOMP";
  case OCI_DTYPE_FILE:		return "OCI_DTYPE_FILE";
  }
  return "UNKNOWN HD TYPE";
}
