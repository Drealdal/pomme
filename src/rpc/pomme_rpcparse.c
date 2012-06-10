/*
 * =====================================================================================
 *
 *       Filename:  pomme_rpcparse.c
 *
 *    Description:  paser
 *
 *        Version:  1.0
 *        Created:  06/08/2012 07:24:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
/**
 * @brief pomme_pasre_arg: parse a param node
 *     <param type="" name="">
 *     </param>
 *
 * @param node
 * @param type: return value here
 *
 * @return: ==0 for success, < 0 for failure 
 */
int pomme_pasre_param(xmlNodePtr node, pomme_type_t *param)
{
    assert(NULL != node );
    assert(NULL != type );
    if( xmlStrcmp(node->name, BAD_CAST(RPC_PARAM)) != 0 )
    {
	debug("wrong label for params:%d",node->line);
	return -1;
    }

    if( xmlHasProp(node, BAD_CAST(RPC_PARAM_NAME)))
    {
	param->name = xmlGetProp(node,BAD_CAST(RPC_PARAM_NAME));
    }else{
	debug("No name attr for param at line:%d",node->line);
	return -1;
    }

    if( xmlHasProp(node, BAD_CAST(RPC_PARAM_TYPE)))
    { 
	param->type.name = xmlGetProp(node, BAD_CAST(RPC_PARAM_TYPE));
	param->type.len = -1;
    }else{
	debug("No type for param %s at line %d",param->name,node->line);
	return -1;
    }	

    return 0;
}

/**
 * @brief pomme_parse_params: parse a list of params 
 *
 *   <params>
 *   	<param></param>
 *   	<param></param>
 *   <params>
 *
 * @param node
 * @param params
 *
 * @return 
 */
int pomme_parse_params(xmlNodePtr node, pomme_param_t **params)
{
    int ret = 0;
    assert( node != NULL );
    xmlNodePtr curNode = node->xmlChildrenNode;
    int count = 0;
    while(curNode != NULL )
    {
	count++;
	curNode = curNode->next;
    }
    if( count == 0 )
    {
	*params = NULL;
    }else{
	*param = malloc(count*sizeof(pomme_param_t));
	if(*param == NULL )
	{
	    debug("Malloc Error");
	    return -1;
	}
    }	

    curNode = node->xmlChildreanNode;
    count = 0;
    while( curNode != NULL )
    {
	if(  pomme_parse_param(curNode,
		       	*param+count)  < 0 )
	{
	    ret = -1;
	}
	curNode= curNode->next;
	count++;
    }
    return ret;
}

/**
 * <return type=""></return>
 */
int pomme_parse_return(xmlNodePtr node, pomme_param_t *param)
{
    assert(NULL != node );
    assert(NULL != type );
    if( xmlStrcmp(node->name, BAD_CAST(RPC_FUNCTION_RETURN)) != 0 )
    {
	debug("wrong label for params:%d",node->line);
	return -1;
    }


    if( xmlHasProp(node, BAD_CAST(RPC_FUNCTION_RETURN_TYPE)))
    { 
	param->type.name = xmlGetProp(node, BAD_CAST(RPC_FUNCTION_RETURN_TYPE));
	param->type.len = -1;
    }else{
	debug("No type for param %s at line %d",param->name,node->line);
	return -1;
    }	

    return 0;
}
/*  
 *  <function name="Name">
 *  	<params>
 *  	</params>
 *  	<return type="">
 *  	</return>
 *  </function>
 *  */
int pomme_parse_function(xmlNodePtr node, funcgen_t *func)
{
    int ret = 0;
    assert(node != NULL );
    assert( NULL != func );

    if(xmlStrcmp(node->name , BAD_CAST(RPC_FUNCTION)) != 0 )
    {
	debug("Wrong lable(%s) for function at %d",node->name, node-line);
	return -1;
    }

    if(xmlHasProp(node, BAD_CAST(RPC_FUNCTION_NAME)))
    {
	func->name = xmlGetProp(node, BAD_CAST(RPC_FUNCTION_RETURN_TYPE));
    }else{
	debug("Function Must have names");
	return -1;
    }
    // TODO
    return 0;
}
/*
 * <functions>
 * 	<function name="Name1"></function>
 * 	<function name="Name2"></function>
 * 	.
 * 	.
 * 	
 * 	<function name="NameN"></function>
 * </functions>
 *
 */
int pomme_parse_functions(xmlNodePtr node, funcgen_t **funcs)
{
    int ret = 0;
    return 0;
}
