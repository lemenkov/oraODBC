#include "common.h"

#define IS_VALID(x) (x->valid_flag==VALID_FLAG_DEFAULT)
SQLRETURN not_implemented()
{
  fprintf(stderr,"you have called an unimplemented function!\n");
  return SQL_ERROR;
}


SQLRETURN init_hgeneric(hgeneric *t)
{
  t->error_list = (void*)NULL;
  t->htype = int_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;
  return SQL_SUCCESS;
}

ar_T * make_ar_T()

{
  ar_T * t = ORAMALLOC(sizeof(ar_T));
  t->auto_unique = SQLSMALLINT_DEFAULT;
  t->base_column_name = SQLCHAR_DEFAULT;
  t->base_table_name = SQLCHAR_DEFAULT; 
  t->case_sensitive = SQLSMALLINT_DEFAULT;
  t->catalog_name = SQLCHAR_DEFAULT;
  t->concise_type = SQLSMALLINT_DEFAULT;
  t->data_ptr = SQLPOINTER_DEFAULT;
  t->display_size = SQLINTEGER_DEFAULT;
  t->fixed_prec_scale = SQLSMALLINT_DEFAULT;
  t->bind_indicator = SQLINTEGER_DEFAULT;
  t->column_label = SQLCHAR_DEFAULT;
  t->length = SQLINTEGER_DEFAULT;
  t->literal_prefix = SQLCHAR_DEFAULT;
  t->literal_suffix = SQLCHAR_DEFAULT;
  t->local_type_name = SQLCHAR_DEFAULT;
  t->column_name[0] = SQLCHAR_DEFAULT;
  t->nullable = SQLSMALLINT_DEFAULT;
  t->num_prec_radix = SQLINTEGER_DEFAULT;
  t->octet_length = SQLINTEGER_DEFAULT;
  t->precision = SQLSMALLINT_DEFAULT;
  t->scale = SQLSMALLINT_DEFAULT;
  t->schema_name = SQLCHAR_DEFAULT;
  t->searchable = SQLSMALLINT_DEFAULT;
  t->table_name = SQLCHAR_DEFAULT;
  t->data_type = SQLSMALLINT_DEFAULT;
  t->type_name = SQLCHAR_DEFAULT;
  t->un_signed = SQLSMALLINT_DEFAULT;
  t->updateable = SQLSMALLINT_DEFAULT;
  t->buffer_length = SQLINTEGER_DEFAULT;
  t->bind_target_type = SQLSMALLINT_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;

  return t;
};


ir_T * make_ir_T()
{  
  ir_T * t = ORAMALLOC(sizeof(ir_T));

  t->data_type = ub2_DEFAULT;
  t->orig_type = ub2_DEFAULT;
  t->data_size = ub4_DEFAULT;
  t->col_num = int_DEFAULT;
  t->default_copy = FUNCTION_DEFAULT;
  t->to_string =  FUNCTION_DEFAULT;
  t->to_oracle = FUNCTION_DEFAULT;
  t->desc = struct_DEFAULT;
  t->data_ptr =  struct_DEFAULT;
  t->ind_arr = sb2_DEFAULT;
  t->length_arr = ub2_DEFAULT;
  t->rcode_arr = ub2_DEFAULT;
  t->locator = (void*)NULL ;
  t->posn = ub4_DEFAULT;
  t->lobsiz = ub4_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;
  return t;
}


hEnv_T * make_hEnv_T()
{

  hEnv_T * t = ORAMALLOC(sizeof(hEnv_T));
  init_hgeneric(&(t->base_handle));
  t->parent = (SQLHANDLE*)NULL /*SQLHANDLE_DEFAULT*/ ;
  t->odbc_ver = SQLINTEGER_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;
  return t;
}

hDbc_T * make_hDbc_T()
{
  hDbc_T * t = ORAMALLOC(sizeof(hDbc_T));
  init_hgeneric(&(t->base_handle));
  t->UID[0] = SQLCHAR_DEFAULT;
  t->PWD[0] = SQLCHAR_DEFAULT;
  t->DB[0] = SQLCHAR_DEFAULT;
  t->DSN[0] = SQLCHAR_DEFAULT;
  t->oci_env = (OCIEnv*) NULL;
  t->oci_err = (OCIError*)NULL;
  t->oci_srv = (OCIServer*)NULL;
  t->oci_svc = (OCISvcCtx*)NULL;
  t->oci_ses = (OCISession*)NULL;
  t->metadata_id = (SQLUINTEGER)0;
  t->trace = (SQLUINTEGER)0;
  t->tracefile[0] = SQLCHAR_DEFAULT;
  t->autocommit = ub4_DEFAULT;
  t->stmt_list = struct_DEFAULT;
  t->desc_list = struct_DEFAULT;
  t->env = struct_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;

  return t;
}


hDesc_T * make_hDesc_T()
{
  hDesc_T * t = ORAMALLOC(sizeof(hDesc_T));

  init_hgeneric(&(t->base_handle));
  t->num_recs = ub4_DEFAULT;
  t->type = int_DEFAULT;
  t->bound_col_flag = int_DEFAULT;
  t->lob_col_flag = int_DEFAULT;
  t->recs.ap = struct_DEFAULT;
  t->recs.ip = struct_DEFAULT;
  t->recs.ar = struct_DEFAULT;
  t->recs.ir = struct_DEFAULT;
  t->next = struct_DEFAULT;
  t->prev = struct_DEFAULT;
  t->stmt = struct_DEFAULT;
  t->dbc = struct_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;

  return t;

}


hStmt_T * make_hStmt_T()
{
  hStmt_T * t = ORAMALLOC(sizeof(hStmt_T));
  init_hgeneric(&(t->base_handle));
  t->implicit_ap = struct_DEFAULT;
  t->implicit_ip = struct_DEFAULT;
  t->implicit_ar = struct_DEFAULT;
  t->implicit_ir = struct_DEFAULT;
  t->current_ap = struct_DEFAULT;
  t->current_ip = struct_DEFAULT;
  t->current_ar = struct_DEFAULT;
  t->current_ir = struct_DEFAULT;
  t->oci_stmt = (OCIStmt*)NULL;
  t->next = struct_DEFAULT;
  t->prev = struct_DEFAULT;
  t->stmt_type = ub2_DEFAULT;
  t->num_result_rows = int_DEFAULT;
  t->num_fetched_rows = int_DEFAULT;
  t->bookmark = int_DEFAULT;
  t->current_row = int_DEFAULT;
  t->alt_fetch_data = (void*)NULL;
  t->fetch_status = int_DEFAULT;
  t->sql = SQLCHAR_DEFAULT;
  t->dbc = struct_DEFAULT;
  t->row_bind_offset_ptr = SQLPOINTER_DEFAULT;
  t->param_bind_offset_ptr = SQLPOINTER_DEFAULT;
  t->row_array_size = (SQLUINTEGER)NULL;
  t->row_status_ptr = (SQLUSMALLINT)NULL;
  t->row_bind_type = (SQLUINTEGER)NULL;
  t->rows_fetched_ptr = (SQLUINTEGER)NULL;
  t->query_timeout = (SQLUINTEGER)NULL;
  t->valid_flag = VALID_FLAG_DEFAULT;
  return t;
}