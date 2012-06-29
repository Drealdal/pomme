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
#include "pomme_rpcparse.h"
#include "pomme_rpcgen.h"
#include <libxml/parser.h>
int pomme_parse_param(xmlNodePtr node, pomme_param_t *param)
{
    assert(NULL != node );
    assert(NULL != param );
    if( xmlStrcmp(node->name, BAD_CAST(RPC_PARAM)) != 0 )
    {
	debug("wrong label for params:%d",node->line);
	return -1;
    }

    if( xmlHasProp(node, BAD_CAST(RPC_PARAM_NAME)))
    {
	param->name = (char *)xmlGetProp(node,BAD_CAST(RPC_PARAM_NAME));
    }else{
	debug("No name attr for param at line:%d",node->line);
	return -1;
    }

    if( xmlHasProp(node, BAD_CAST(RPC_PARAM_TYPE)))
    { 
	param->type.name = (char *)xmlGetProp(node, BAD_CAST(RPC_PARAM_TYPE));
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
    int ret ;
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
	*params = malloc(count*sizeof(pomme_param_t));
	if(*params == NULL )
	{
	    debug("Malloc Error");
	    return -1;
	}
    }	

    curNode = node->xmlChildrenNode;
    count = 0;
    while( curNode != NULL )
    {
	if(  pomme_parse_param(curNode,
		       	*params+count)  < 0 )
	{
	    ret = -1;
	}
	curNode= curNode->next;
	count++;
    }
    return count;
}

/**
 * <return type=""></return>
 */
int pomme_parse_return(xmlNodePtr node, pomme_param_t *param)
{
    assert(NULL != node );
    assert(NULL != param );
    if( xmlStrcmp(node->name, BAD_CAST(RPC_FUNCTION_RETURN)) != 0 )
    {
	debug("wrong label for params:%d",node->line);
	return -1;
    }


    if( xmlHasProp(node, BAD_CAST(RPC_FUNCTION_RETURN_TYPE)))
    { 

	param->type.name = (char *)xmlGetProp(node, BAD_CAST(RPC_FUNCTION_RETURN_TYPE));
	param->type.len = -1;

    }else{
	param->type.name = RPC_NORET;
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
	debug("Wrong lable(%s) for function at %d",node->name, node->line);
	return -1;
    }

    if(xmlHasProp(node, BAD_CAST(RPC_FUNCTION_NAME)))
    {
	func->name = (char *)xmlGetProp(node, BAD_CAST(RPC_FUNCTION_NAME));
    }else{
	debug("Function Must have names");
	return -1;
    }
    // TODO
    xmlNodePtr curNode = node->xmlChildrenNode;
    xmlNodePtr params = NULL , rparam = NULL;
    while( curNode != NULL )
    {
	if( (params != NULL) && ( rparam != NULL ))
	{
	    debug("Only one params and one return node allows");
	    ret = -1;
	    break;
	}
	if( xmlStrcmp(curNode->name,BAD_CAST(RPC_PARAMS)) == 0 )
	{
	    params = curNode; 
	}else if (xmlStrcmp(curNode->name,
		    BAD_CAST(RPC_FUNCTION_RETURN )) == 0 ) 
	{
	    rparam = curNode;
	}else{
	    debug("Not accept label(%s) at line %d",curNode->name, curNode->line);
	    ret = -1;
	}
	curNode = curNode->next;
    }
    ret = pomme_parse_params(params, &func->params);
    if( ret < 0 )
    {
	debug("parse params for function(%s) failure",func->name);
    }else{
	func->argnum = ret;
    }
    if(pomme_parse_return(rparam, &func->rparam) < 0  )
    {
	ret = -1;
    }	
    return ret;
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
    int ret, i = 0;
    assert( node != NULL );

    if( xmlStrcmp(node->name, BAD_CAST(RPC_FUNCTIONS) ) != 0 )
    {
	debug("Not accept label(%s), (%s) is expected",node->name, RPC_FUNCTIONS);
	return -1;
    }

    xmlNodePtr curNode = node->xmlChildrenNode;
    int count = 0;
    while( curNode != NULL )
    {
	count++;
	curNode = curNode->next;
    }

    *funcs = malloc(count * sizeof(funcgen_t));
    if( *funcs == NULL )
    {
	debug("Malloc Memory Error");
	return -1;
    }
    curNode = node->xmlChildrenNode;
    for( i = 0 ; i < count; i++)
    {
	if( pomme_parse_function(curNode, *funcs + i ) < 0 )
	{
	    ret = -1;
	}
	curNode = curNode->next;
    }

    return count;
}
/*
 * <server name="meta_server">
 * 	<functions>
 * 	</functions>
 * <server>
 * fileGenerated: pomme_meta_server.h
 *                pomme_meta_server.c
 *                pomme_meta_server_imp.h
 *                pomme_meta_server_imp.c
 *                pomme_meta_server_client.h
 *                pomme_meta_server_client.c
 *                pomme_meta_server_const.h
 * 
 */
int pomme_parse_server(xmlNodePtr node, rpcgen_t *server)
{
     int ret = 0;
     assert( node != NULL );
     assert( server != NULL );
     if( (xmlStrcmp(node->name, BAD_CAST(RPC_SERVER))) != 0 )
     {
	 debug("not accept label(%s) , expecting(%s)",node->name, RPC_SERVER);
	 return -1;
     }

    if( xmlHasProp(node, BAD_CAST(RPC_NAME)))
    {
	server->name = (char *)xmlGetProp(node, BAD_CAST(RPC_NAME));
	debug("Server Name:%s",server->name);
    }else{
	debug("No name attr for param at line:%d",node->line);
	return -1;
    }
    
    if( xmlHasProp(node, BAD_CAST(RPC_SERVER_PREFIX)))
    {
	server->prefix = (char *)xmlGetProp(node,BAD_CAST(RPC_SERVER_PREFIX));
    }else{
	debug("No name prefix attr for param at line:%d",node->line);
    }

    xmlNodePtr curNode = node->xmlChildrenNode;
    xmlNodePtr funcs = NULL;

    while( curNode != NULL )
    {
	if( funcs != NULL )
	{
	    debug("Too much children");
	    break;
	}
	if( xmlStrcmp(curNode->name, BAD_CAST(RPC_FUNCTIONS)) == 0  )
	{
	    funcs = curNode;
	}
	curNode = curNode->next;
    }

    if( funcs == NULL )
    {
	debug("No functions is defined in for server(%s)",node->name);
	ret = -1;
    }else{
	if ( (ret = pomme_parse_functions(funcs,
			&server->funcs)) < 0 )
	{
	    debug("Parse functions fail for server(%s)",node->name);
	}else{
	    server->funcnum = ret;
	    ret = 0;
	}
    }
    return ret ;
}
int pomme_parse_init(xmlNodePtr proot,rpcgen_t **server)
{
    int ret;
    int count = 1;
    xmlNodePtr curNode = proot;

    *server = malloc(sizeof(rpcgen_t));
    if( *server == NULL )
    {
	debug("Mem problem");
	exit(-1);
    }
    if( ( ret = pomme_parse_server(curNode, 
		    *server) ) < 0)
    {
	exit(-1);
    }
    return count;
}
