/**
 * @brief Enumeration for the http multipart types.
 * @since_tizen 3.0
 */
typedef enum {
	    HTTP_MULTIPART_CONTENTS,
	    HTTP_MULTIPART_FILE,
} http_formdata_type_e;

/**
 * @brief Adds the multipart/form-data.
 * @since_tizen 3.0
 * @remarks    It is used with #HTTP_METHOD_POST
 * @param[in]  http_transaction	The http transaction handle
 * @param[in]  multipart_type	The type of form-data
 * @param[in]  part_name	The name of part
 * @param[in]  value	The value of part \n
 *						It will be file path or content value
 * @param[in]  content_type		The content type of value (e.g text/html, image/jpeg, etc) \n
 *								It will be #NULL
 * @return 0 on success, otherwise negative error value
 * @retval  #HTTP_ERROR_NONE  Successful
 * @retval  #HTTP_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #HTTP_ERROR_INVALID_OPERATION  Invalid operation
 * @retval  #HTTP_ERROR_NOT_SUPPORTED  Not Supported
 */
int http_transaction_request_add_multipart(http_transaction_h http_transaction,
		http_formdata_type_e multipart_type, const char *part_name, const char *value, const char *content_type);
