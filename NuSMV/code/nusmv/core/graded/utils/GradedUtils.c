#include <nusmv/core/dd/DDMgr_private.h>
#include <nusmv/core/utils/StreamMgr.h>
#include <nusmv/core/mc/mc.h>
#include "GradedUtils.h"
#include "../../../../../../cudd-2.4.1.1/cudd/cuddInt.h"

/******************************************************************************
    Utilities for bdd and add
******************************************************************************/

node_ptr GradedUtils_nodePlus(NuSMVEnv_ptr env,node_ptr n1, node_ptr n2, node_ptr k)
{ 
    nusmv_ptrint n1_int, n2_int, k_int;
//	BddEnc_ptr enc = BddFsm_get_bdd_encoding(fsm);
//	const NuSMVEnv_ptr env = EnvObject_get_environment(ENV_OBJECT(enc));
	const NodeMgr_ptr nodemgr =
			NODE_MGR(NuSMVEnv_get_value(env, ENV_NODE_MGR));
	const ExprMgr_ptr exprs = EXPR_MGR(NuSMVEnv_get_value(env, ENV_EXPR_MANAGER));
         
    if (node_get_type(n1) == FAILURE) return n1; /* error in previous expr */
    if (node_get_type(n2) == FAILURE) return n2; /* error in previous expr */
    if (node_get_type(k) == FAILURE) return k; /* error in previous expr */
    
    n1_int = NODE_TO_INT(car(n1));
    n2_int = NODE_TO_INT(car(n2));
    k_int = NODE_TO_INT(car(k));
    
    if (n1_int > (k_int - n2_int))
	    return find_node(nodemgr,NUMBER, NODE_FROM_INT(k_int), Nil);

    return find_node(nodemgr,NUMBER, NODE_FROM_INT(n1_int + n2_int), Nil);

}

node_ptr GradedUtils_nodeTimes(NuSMVEnv_ptr env,node_ptr n1, node_ptr n2, node_ptr k)
{ 
    nusmv_ptrint n1_int, n2_int, k_int;
	const NodeMgr_ptr nodemgr =
			NODE_MGR(NuSMVEnv_get_value(env, ENV_NODE_MGR));

    if (node_get_type(n1) == FAILURE) return n1; /* error in previous expr */
    if (node_get_type(n2) == FAILURE) return n2; /* error in previous expr */
    if (node_get_type(k) == FAILURE) return k; /* error in previous expr */

    n1_int = NODE_TO_INT(car(n1));
    n2_int = NODE_TO_INT(car(n2));
    k_int = NODE_TO_INT(car(k));
    
    if ((n1_int>0 && n2_int>0) && (n1_int> k_int / n2_int ))
	    return find_node(nodemgr,NUMBER,  NODE_FROM_INT(k_int), Nil);

    return find_node(nodemgr,NUMBER, NODE_FROM_INT(n1_int * n2_int), Nil);
}


DdNode * GradedUtils_addApply(DDMgr_ptr dd, ptruint op, DdNode *f, DdNode *g, int k)
{
	DdNode *res;
	const NuSMVEnv_ptr env = EnvObject_get_environment(ENV_OBJECT(dd));
	const NodeMgr_ptr nodemgr =
			NODE_MGR(NuSMVEnv_get_value(env, ENV_NODE_MGR));


	add_ptr add_k = add_leaf(dd, find_node(nodemgr,NUMBER, NODE_FROM_INT(k), Nil));
	do {
		dd->dd->reordered = 0;
		res = GradedUtils_addApplyRecur(dd, op, f, g, add_k);
	} while (dd->dd->reordered == 1);
	
		//common_error(res, "GradedUtils_addApply: result = NULL");
	Cudd_Ref(res);
	
	return(res);
} 


DdNode * GradedUtils_addApplyRecur(DDMgr_ptr dd, ptruint op, DdNode *f, DdNode *g, DdNode *k)
{
	DdNode *res,
	*fv, *fvn, *gv, *gvn,
	*T, *E;
	unsigned int ford, gord;
	unsigned int index;
    const NuSMVEnv_ptr env = EnvObject_get_environment(ENV_OBJECT(dd));

    /*
	Check for terminals. If it's the case then "op" is applied to
	the operands f and gmay swap f and g.
    */
	if(cuddIsConstant(f) && cuddIsConstant(g)){
		CUDD_VALUE_TYPE res_n;

		if (op == DD_ADD_NODE_PLUS_TAG)
			res_n = GradedUtils_nodePlus(env,cuddV(f), cuddV(g), cuddV(k));
		else
			res_n = GradedUtils_nodeTimes(env,cuddV(f), cuddV(g), cuddV(k));
		if (res_n == NULL) return(NULL);
		
		return(cuddUniqueConst(dd->dd,res_n));
	}

	/* Check cache */
	//res = cuddCacheLookup(dd,(ptruint)(op),f,g,k);
	res = GradedUtils_cacheLookup3(dd->dd, (ptruint)(op), f, g, k);
	if (res != NULL) return(res);

	/* Recursive Step */
	
	ford = Cudd_ReadPerm(dd->dd,f->index);
	gord = Cudd_ReadPerm(dd->dd,g->index);
	if (ford <= gord) {
		index = f->index;
		fv = cuddT(f);
		fvn = cuddE(f);
	} else {
		index = g->index;
		fv = fvn = f;
	}
	if (gord <= ford) {
		gv = cuddT(g);
		gvn = cuddE(g);
	} else {
		gv = gvn = g;
	}

	T = GradedUtils_addApplyRecur(dd, op, fv, gv, k);
	if (T == NULL) return(NULL);
	cuddRef(T);

	E = GradedUtils_addApplyRecur(dd, op, fvn, gvn, k);
	if (E == NULL) {
		Cudd_RecursiveDeref(dd->dd,T);
		return(NULL);
	}
	cuddRef(E);

	/* Necessary to have ROBDD */
	res = (T == E) ? T : cuddUniqueInter(dd->dd,(int)index,T,E);
	if (res == NULL) {
		Cudd_RecursiveDeref(dd->dd, T);
		Cudd_RecursiveDeref(dd->dd, E);
		return(NULL);
	}
	cuddDeref(T);
	cuddDeref(E);

	/* Store result */
	//cuddCacheInsert(dd,(ptruint)(op),f,g,k,res);
	GradedUtils_cacheInsert3((ptruint)(op), f, g, k, res);


	return(res);

}



DdNode * GradedUtils_addAbstract(DDMgr_ptr dd, add_ptr a, bdd_ptr b, int k)
{
	DdNode * cube;
	DdNode *res;
	const NuSMVEnv_ptr env = EnvObject_get_environment(ENV_OBJECT(dd));
	const NodeMgr_ptr nodemgr =
			NODE_MGR(NuSMVEnv_get_value(env, ENV_NODE_MGR));
	add_ptr add_k = add_leaf(dd, find_node(nodemgr,NUMBER, NODE_FROM_INT(k), Nil));
  
	cube = Cudd_BddToAdd(dd->dd, (DdNode *)b);
	//common_error(cube, "add_exist_abstract: cube = NULL");
	Cudd_Ref(cube);
	/**********************************************************/
	

	do {
		dd->dd->reordered = 0;
		res = GradedUtils_addAbstractRecur(dd, DD_ADD_NODE_PLUS_TAG, (DdNode *)a, cube, add_k);
	} while (dd->dd->reordered == 1);
	
	//common_error(res, "add_exist_abstract: result = NULL");
	
	Cudd_Ref(res);
	
	return(res);
}

DdNode * GradedUtils_addAbstractRecur(DDMgr_ptr  manager, ptruint op, DdNode * f, DdNode * cube, DdNode *k)
{
	DdNode	*T, *E, *res, *res1, *res2, *zero;

	statLine(manager->dd);
	zero = DD_FALSE(manager->dd);

	/* Cube is guaranteed to be a cube at this point. */	
	if (f == zero || cuddIsConstant(cube)) {  
		return(f);
	}

	/* Abstract a variable that does not appear in f */
	if (Cudd_ReadPerm(manager->dd,f->index) > Cudd_ReadPerm(manager->dd,cube->index)) {
		res1 = GradedUtils_addAbstractRecur(manager, op, f, cuddT(cube), k);
		if (res1 == NULL) return(NULL);
		cuddRef(res1);
	/* Use the "internal" procedure to be alerted in case of
		** dynamic reordering. If dynamic reordering occurs, we
		** have to abort the entire abstraction.
	*/
		res = GradedUtils_addApplyRecur(manager, op, res1, res1, k);
		if (res == NULL) {
			Cudd_RecursiveDeref(manager->dd,res1);
			return(NULL);
		}
		cuddRef(res);
		Cudd_RecursiveDeref(manager->dd,res1);
		cuddDeref(res);
		return(res);
	}

	if ((res = GradedUtils_cacheLookup3(manager->dd, (ptruint)(op), f, cube, k)) != NULL) {
		return(res);
	}

	T = cuddT(f);
	E = cuddE(f);

	/* If the two indices are the same, so are their levels. */
	if (f->index == cube->index) {
		res1 = GradedUtils_addAbstractRecur(manager, op, T, cuddT(cube), k);
		if (res1 == NULL) return(NULL);
		cuddRef(res1);
		res2 = GradedUtils_addAbstractRecur(manager, op, E, cuddT(cube), k);
		if (res2 == NULL) {
			Cudd_RecursiveDeref(manager->dd,res1);
			return(NULL);
		}
		cuddRef(res2);
		res = GradedUtils_addApplyRecur(manager, op, res1, res2, k);
		if (res == NULL) {
			Cudd_RecursiveDeref(manager->dd,res1);
			Cudd_RecursiveDeref(manager->dd,res2);
			return(NULL);
		}
		cuddRef(res);
		Cudd_RecursiveDeref(manager->dd,res1);
		Cudd_RecursiveDeref(manager->dd,res2);
		//cuddCacheInsert(manager, (ptruint)(op), f, cube, k, res);
		GradedUtils_cacheInsert3((ptruint)(op), f, cube, k, res);

		cuddDeref(res);
		return(res);
	} else { /* if (cuddI(manager,f->index) < cuddI(manager,cube->index)) */
		res1 = GradedUtils_addAbstractRecur(manager, op, T, cube, k);
		if (res1 == NULL) return(NULL);
		cuddRef(res1);
		res2 = GradedUtils_addAbstractRecur(manager, op, E, cube, k);
		if (res2 == NULL) {
			Cudd_RecursiveDeref(manager->dd,res1);
			return(NULL);
		}
		cuddRef(res2);
		res = cuddUniqueInter(manager->dd, (int) f->index, res1, res2);
		if (res == NULL) {
			Cudd_RecursiveDeref(manager->dd,res1);
			Cudd_RecursiveDeref(manager->dd,res2);
			return(NULL);
		}
		cuddDeref(res1);
		cuddDeref(res2);
		//cuddCacheInsert(manager, (ptruint)(op), f, cube, k, res);
		GradedUtils_cacheInsert3((ptruint)(op), f, cube, k, res);

		return(res);
	}	    

}

int GradedUtils_bddIsSubset(DDMgr_ptr dd, bdd_ptr a, bdd_ptr b) {
	bdd_ptr not_b = bdd_not(dd, b);
	bdd_ptr intersection = bdd_and(dd, a, not_b);
	int res = bdd_is_false(dd, intersection);
	
	bdd_free(dd, not_b);
	bdd_free(dd, intersection);
	
	return res;
}

bdd_ptr GradedUtils_bddMinus(DDMgr_ptr dd, bdd_ptr a, bdd_ptr b) {
    bdd_ptr tmp1 = bdd_not(dd, b);
    bdd_ptr result = bdd_and(dd, a, tmp1);
    
    bdd_free(dd, tmp1);
    
    return result;
}


//non �pi utilizzata andrebbe eliminata
add_ptr GradedUtils_addComposition(DDMgr_ptr dd, add_ptr a, add_ptr b, int i) {
    const NuSMVEnv_ptr env = EnvObject_get_environment(ENV_OBJECT(dd));
    const NodeMgr_ptr nodemgr =
            NODE_MGR(NuSMVEnv_get_value(env, ENV_NODE_MGR));
	add_ptr add_i = add_leaf(dd, find_node(nodemgr,NUMBER, NODE_FROM_INT(i), Nil));
	add_ptr tmp = add_apply(dd, node_times, a, add_i);
	add_ptr result = add_apply(dd, node_plus, tmp, b);
	
	add_free(dd, add_i);
	add_free(dd, tmp);
	
	return result;
}

void GradedUtils_addSum(DDMgr_ptr dd, add_ptr* a, add_ptr b, int k) {
	add_ptr tmp = GradedUtils_addApply(dd, DD_ADD_NODE_PLUS_TAG, *a, b, k);
	
    add_free(dd, *a);
			
    *a = add_dup(tmp);
    add_free(dd, tmp);
}

/**Function****************************************************************************
    Utilities for fsm transitions
******************************************************************************/

bdd_ptr GradedUtils_andTransition(BddFsm_ptr fsm, bdd_ptr states) {
	BddEnc_ptr enc = BddFsm_get_bdd_encoding(fsm);
	DDMgr_ptr dd = BddEnc_get_dd_manager(enc);
	BddTrans_ptr transition = BddFsm_get_trans(fsm);
	ClusterList_ptr cluster_list = BddTrans_get_backward(transition);
	ClusterListIterator_ptr iterator = ClusterList_begin(cluster_list);
	
	bdd_ptr result = bdd_dup(states);
	
	while (!ClusterListIterator_is_end(iterator)) {
		Cluster_ptr cluster = ClusterList_get_cluster(cluster_list, iterator);
		bdd_ptr trans = Cluster_get_trans(cluster);		
		bdd_and_accumulate(dd, &result, trans);
		bdd_free(dd, trans);
		iterator = ClusterListIterator_next(iterator);
	}
	
	return result;
}

bdd_ptr GradedUtils_getCleanTransition(DDMgr_ptr dd,BddFsm_ptr fsm, Cluster_ptr cluster) {
    //Cluster_get_quantification_state_input
    //Returns a pointer to the list of variables (both state and input vars) to be quantified.
    //Returns a pointer to the list of variables to be quantified respect to the transition relation inside the cluster. Returned bdd is referenced.
    bdd_ptr state_input_vars = Cluster_get_quantification_state_input(cluster);

    //Returns a pointer to the list of variables (state vars only) to be quantified.
    //Returned value is referenced
    bdd_ptr state_vars = Cluster_get_quantification_state(cluster);
    //forsome:state_vars是一些元素的合取范式，例如x0 x1 x2的合取范式， bdd some表示 存在x0，x1，x2 使得state_input_vars






    bdd_ptr input_vars = bdd_forsome(dd, state_input_vars, state_vars);

    
    bdd_ptr transition = Cluster_get_trans(cluster);
    
    bdd_ptr result = bdd_forsome(dd, transition, input_vars);

    /*ssssss*/
//    BddEnc_ptr enc = BddFsm_get_bdd_encoding(fsm);
//    SymbTable_ptr st = BaseEnc_get_symb_table(BASE_ENC(enc));
//    const NuSMVEnv_ptr env = EnvObject_get_environment(ENV_OBJECT(enc));
//    const StreamMgr_ptr streams =
//            STREAM_MGR(NuSMVEnv_get_value(env, ENV_STREAM_MANAGER));
//    const OStream_ptr outstream = StreamMgr_get_output_ostream(streams);
//
//
//    array_t* ls = SymbTable_get_class_layer_names(st,(const char*) NULL);
//    NodeList_ptr sym=SymbTable_get_layers_sf_i_symbols(st, ls);
//    node_ptr valueList = BddEnc_assign_symbols(enc, state_vars,sym ,false, (bdd_ptr*)NULL);
//    node_ptr valueList1 = BddEnc_assign_symbols(enc, input_vars,sym ,false, (bdd_ptr*)NULL);
//
//    BddEnc_print_vars_in_cube(enc,state_vars,valueList,outstream);
//    BddEnc_print_vars_in_cube(enc,input_vars,valueList1,outstream);

/*ssssss*/


    
    bdd_free(dd, state_input_vars);
    bdd_free(dd, state_vars);
    bdd_free(dd, input_vars);
    bdd_free(dd, transition);
    
    return result;
}


/**Function****************************************************************************
    Name:
        GradedUtils_euGetIntermediateSets
        
    Parameters:
        BddFsm_ptr  fsm     Kripke structure to verify
        BddStates   f       The states where the subformula \psi_1 is true
        BddStates   g       The states where the subformula \psi_2 is true
        int         k       The grading constant
        
    Result:
        BddStates *          The list of the sets [E^{>j} \psi_1 U \psi_2] with 0<=j<=k
        
    Description:
        Given the sets f and g where \psi_1 and \psi_2 are true
        returns list of the sets [E^{>j} \psi_1 U \psi_2] with 0<=j<=k
        
*********************************************************************************/
BddStates * GradedUtils_euGetIntermediateSets(BddFsm_ptr fsm, BddStates f, BddStates g, int k) {
	BddStates *lista = ALLOC(BddStates, k+1);

	BddEnc_ptr enc = BddFsm_get_bdd_encoding(fsm);
	DDMgr_ptr dd = BddEnc_get_dd_manager(enc);

	add_ptr count_paths, adding_paths;
	bdd_ptr contributors, contributors_tmp, old_contributors;
	int i;

	count_paths = add_false(dd);
	
	contributors_tmp = (bdd_ptr) eu(fsm, f, g);
	contributors = BddEnc_apply_state_frozen_vars_mask_bdd(enc, contributors_tmp);
	bdd_free(dd, contributors_tmp);

	for (i=1; i<=k; i++) {
		old_contributors = bdd_dup(contributors);
		adding_paths = GradedUtils_fsmCountSuccessors(fsm, contributors, k+1);
		bdd_free(dd, contributors);
		GradedUtils_addSum(dd, &count_paths, adding_paths, k+1);
		add_free(dd, adding_paths);
		contributors = add_to_bdd_strict_threshold(dd, count_paths, i);
		GradedUtils_applyReachableAndMask(fsm, &contributors);
		bdd_and_accumulate(dd, &contributors, f);
		GradedUtils_fixPoint(fsm, &contributors, f);
		
		lista[i-1] = GradedUtils_bddMinus(dd, old_contributors, contributors);
		
		bdd_free(dd, old_contributors);
	}
	//if(k>0)
		lista[i-1] = contributors;
	/*else
		lista[0] = contributors;*/
	bdd_free(dd, contributors);
	add_free(dd, count_paths);
	
	return lista;

}

/**Function****************************************************************************
    Name:
        GradedUtils_egGetIntermediateSets
        
    Parameters:
        BddFsm_ptr  fsm     Kripke structure to verify
        BddStates   f       The states where the subformula \psi is true
        int         k       The grading constant
        
    Result:
        BddStates *         The list of the sets [E^{>j} G \psi] with 0<=j<=k
        
    Description:
        Given the set f where \psi is true
        returns list of the sets [E^{>j} G \psi] with 0<=j<=k
        
*********************************************************************************/
BddStates * GradedUtils_egGetIntermediateSets(BddFsm_ptr fsm, BddStates f, int k) {
	BddStates *lista = ALLOC(BddStates, k+1);

	BddEnc_ptr enc = BddFsm_get_bdd_encoding(fsm);
	DDMgr_ptr dd = BddEnc_get_dd_manager(enc);

	add_ptr count_paths, adding_paths;
	bdd_ptr contributors, contributors_tmp, old_contributors;
	int i;

	count_paths = add_false(dd);
	
	contributors_tmp = (bdd_ptr) eg(fsm, f);
	contributors = BddEnc_apply_state_frozen_vars_mask_bdd(enc, contributors_tmp);
	bdd_free(dd, contributors_tmp);

	for (i=1; i<=k; i++) {
		old_contributors = bdd_dup(contributors);
		adding_paths = GradedUtils_fsmCountSuccessors(fsm, contributors, k+1);
		bdd_free(dd, contributors);
		GradedUtils_addSum(dd, &count_paths, adding_paths, k+1);
		add_free(dd, adding_paths);
		contributors = add_to_bdd_strict_threshold(dd, count_paths, i);
		GradedUtils_applyReachableAndMask(fsm, &contributors);
		bdd_and_accumulate(dd, &contributors, f);
		GradedUtils_fixPoint(fsm, &contributors, f);
		
		lista[i-1] = GradedUtils_bddMinus(dd, old_contributors, contributors);
		
		bdd_free(dd, old_contributors);
	}
	//if(k>0)
	lista[i-1] = contributors;
	/*else
	lista[0] = contributors;*/
	bdd_free(dd, contributors);
	add_free(dd, count_paths);
	
	return lista;

}

/**Function****************************************************************************
    Name:
        GradedUtils_node_ptrToTreeNode_ptr
        
    Parameters:
        BddFsm_ptr  fsm     	Kripke structure to verify
        BddEnc_ptr  enc     	The coding of the Kripke
        node_ptr    path    	The path representing an execution trace
        BddInputs   inputRoot	The input for the path's root
        
    Result:
        treeNode_ptr        The Tree representing the same execution trace of path.
        
    Description:
        Given an execution trace "path" returns the same execution trace as a Tree.
        
*********************************************************************************/
treeNode_ptr GradedUtils_node_ptrToTreeNode_ptr(BddFsm_ptr fsm, BddEnc_ptr enc, node_ptr path, BddInputs inputRoot)
{
	DDMgr_ptr dd = BddEnc_get_dd_manager(enc);
	treeNode_ptr res, nodo;
	node_ptr exp = reverse(path);
    adjList_ptr figli;

    if (path == NODE_PTR(0)) {
		/* Trivially false */
		bdd_ptr one = bdd_true(dd);
		res = TreeUtils_treeNodeCreate(one, NIL(DdNode));
		bdd_free(dd, one);
	}
	else {
		res = TreeUtils_treeNodeCreate(BDD_STATES(car(exp)), inputRoot);
		nodo = res;
		exp = cdr(exp);
		while (exp != NODE_PTR(0)) {
			bdd_ptr input;
			bdd_ptr state;
			
			input = BDD_INPUTS(car(exp));
			exp = cdr(exp);
			state = BDD_STATES(car(exp)); 
                    
			TreeUtils_concat(nodo, TreeUtils_treeNodeCreate(state, input));
			 figli  = TreeUtils_getListaFigli(nodo);
			nodo = TreeUtils_getNodo(figli);
       
			exp = cdr(exp);
		}
	}
	return res;
}


void GradedUtils_stampaPathsRecur(BddFsm_ptr fsm, BddEnc_ptr enc, treeNode_ptr albero, int nStato, int* nTraccia,
								  NodeList_ptr nodo_stampa, NodeList_ptr input_stampa, cycleInf_ptr cicli, node_ptr pathParziale)
{
	bdd_ptr stato, input;
	adjList_ptr figli;
	const NuSMVEnv_ptr env = EnvObject_get_environment(ENV_OBJECT(enc));
	const NodeMgr_ptr nodemgr =
			NODE_MGR(NuSMVEnv_get_value(env, ENV_NODE_MGR));
	StreamMgr_ptr streams;
    OStream_ptr errstream;
    node_ptr path;



    streams = STREAM_MGR(NuSMVEnv_get_value(env, ENV_STREAM_MANAGER));
	 errstream = StreamMgr_get_error_ostream(streams);

	if (albero == NIL(treeNode))
	{
		printf("It's impossible to get the nex state\n");
		return ;
	}
	
	stato = TreeUtils_getStato(albero);
	input = TreeUtils_getInput(albero);
	figli = TreeUtils_getListaFigli(albero);
	

	if(pathParziale != Nil)
	  path = cons(nodemgr,(node_ptr) bdd_dup(stato),
			cons(nodemgr,(node_ptr) bdd_dup(input), pathParziale));
	else
	  path = cons(nodemgr,(node_ptr) bdd_dup(stato), Nil);
	//Stampa
	
	
	if (cicli[*nTraccia-1].stato == stato && cicli[*nTraccia-1].nStato > nStato)
	  printf("Loop starts here\n");
	else if (cicli[*nTraccia-1].nStato == nStato && !cicli[*nTraccia-1].sink)
	  printf("End loop\n");

	printf("-> Input: %d.%d <-\n", (*nTraccia), nStato);
	if(input != ((bdd_ptr) 0)) {
	  BddEnc_print_bdd_begin(enc, input_stampa, 1);
	  BddEnc_print_bdd(enc, input, (VPFBEFNNV) NULL, errstream, NULL);
	  BddEnc_print_bdd_end(enc);
	}

	printf("-> State: %d.%d <-\n", (*nTraccia), nStato);
	BddEnc_print_bdd(enc, stato, (VPFBEFNNV) NULL, errstream, NULL);
	
	nStato++;
	while  (figli != NIL(adjList))
	{
		treeNode_ptr figlio = TreeUtils_getNodo(figli);
		GradedUtils_stampaPathsRecur(fsm, enc, figlio, nStato, nTraccia, nodo_stampa, input_stampa, cicli, path);
		
		figli = TreeUtils_getNext(figli);
		if (figli != NIL(adjList))
		{
			//Quando si trova un non sink cycle occorre stampare una sola traccia. Tutte le altre sottotracce
			//differiscono solo per il numero di volte che si percorre il ciclo.
			if (cicli[*nTraccia-1].nStato != 0 && cicli[*nTraccia-1].nStato <=nStato && !cicli[*nTraccia-1].sink)
			  return;
		  
			(*nTraccia)++;
			
			printf("\n");
			BddEnc_print_bdd_end(enc);
			BddEnc_print_bdd_begin(enc, nodo_stampa, true);
			
			if (cicli[*nTraccia-1].nStato>0)
			{
			  node_ptr copiaPath = copy_list(nodemgr,path);
			  treeNode_ptr newRoot = (treeNode_ptr) GradedUtils_node_ptrToTreeNode_ptr(fsm, enc, copiaPath, NIL(DdNode));
			  
			  GradedUtils_stampaPathsRecur(fsm, enc, newRoot, 1, nTraccia, nodo_stampa, input_stampa, cicli, Nil);
			}
			
		}
	}
}

/**Function****************************************************************************
    Name:
        GradedUtils_stampaPaths
        
    Parameters:
        BddFsm_ptr  	fsm     Kripke structure to verify
        BddEnc_ptr  	enc     The coding of the Kripke
        treeNode_ptr    albero  The tree of the paths representing the execution traces to print
        
    Result:
        boolean		return 1 if the print operation is succesfully executed, 0 otherwise.
        
    Description:
        Given the tree of execution traces "albero" print it.
        
*********************************************************************************/
boolean GradedUtils_stampaPaths(BddFsm_ptr fsm, BddEnc_ptr enc, treeNode_ptr albero, cycleInf_ptr cicli)
{
    int nTraccia;
    NodeList_ptr svars;
    NodeList_ptr i_symbs;

//    node_ptr nodo_stampa;
//    node_ptr input_stampa;
    SymbTable_ptr st;
    array_t* layers;
    char* name;
    boolean found;
    node_ptr pathParziale;
    int idx;


    printf("Trace Type: Counterexample\n");
	if (albero == NIL(treeNode))
	{
		printf("Counterexample not computed\n");
		return 0;
	}
	
	 nTraccia = 1;
	 layers = array_alloc(const char*, 1);

	 st = BaseEnc_get_symb_table(BASE_ENC(enc));

	 found = false;
	
	arrayForEachItem(const char*, layers, idx, name){
		if (name == (const char*) NULL) {
			array_insert(const char*, layers, idx, util_strsav((char*) MODEL_LAYER_NAME));
			found = true;
			break;
		}
	}
	
	if(!found)
		array_insert_last(const char*, layers, util_strsav((char*) MODEL_LAYER_NAME));

//    SymbTable_get_layers_sf_i_symbols(
//            BaseEnc_get_symb_table(BASE_ENC(self)), layer_names);
	 svars = SymbTable_get_layers_sf_symbols(st, layers);
	 i_symbs = SymbTable_get_layers_sf_i_symbols(st, layers);
	
	 //nodo_stampa = NodeList_to_node_ptr(svars);
	// input_stampa = NodeList_to_node_ptr(i_symbs);

	BddEnc_print_bdd_begin(enc, svars, true);
	 pathParziale = Nil;
	GradedUtils_stampaPathsRecur(fsm, enc, albero, 1, &nTraccia, svars, i_symbs, cicli, pathParziale);
	BddEnc_print_bdd_end(enc);
	
	NodeList_destroy(svars);
}



void GradedUtils_findCycleRecur(BddFsm_ptr fsm, BddEnc_ptr enc, DDMgr_ptr dd, treeNode_ptr albero,
        bdd_ptr visitedStates, int* nTraccia, int nStato, cycleInf_ptr cicli)
{
	bdd_ptr stato;
	adjList_ptr figli;
    bdd_ptr oldVisited;

    if (albero == NIL(treeNode))
	{
		printf("It's impossible to get the nex state\n");
		return ;
	}
	
	stato = TreeUtils_getStato(albero);
	figli = TreeUtils_getListaFigli(albero);
	
     oldVisited = bdd_dup(visitedStates);

    bdd_or_accumulate(dd, &visitedStates, stato);
	
    if (bdd_is_true(dd, (bdd_ptr) GradedMc_iff(fsm, oldVisited, visitedStates)))
    {
       cicli[*nTraccia-1].nStato = nStato;
       cicli[*nTraccia-1].stato = stato;
       if (figli == NIL(adjList))
       {
          //sink cycle
          cicli[*nTraccia-1].sink = true;
       }
       else
       {
          //non sink cycle
	  
          cicli[*nTraccia-1].sink = false;
       }
    }
    //bdd_free(dd, oldVisited);
    
    nStato++;
	while  (figli != NIL(adjList))
	{
		treeNode_ptr figlio = TreeUtils_getNodo(figli);
		GradedUtils_findCycleRecur(fsm, enc, dd, figlio, visitedStates, nTraccia, nStato, cicli);
		
		figli = TreeUtils_getNext(figli);
		if (figli != NIL(adjList))
		{
			(*nTraccia)++;
		}
	}
}


/**Function****************************************************************************
    Name:
        GradedUtils_findCycles
        
    Parameters:
        BddFsm_ptr  	fsm     Kripke structure to verify
        BddEnc_ptr  	enc     The coding of the Kripke
        treeNode_ptr    albero  The tree of the paths representing the execution traces where 
        we need to search a cycle.
        
    Result:
        boolean		return 1 if it find a cycle, 0 otherwise.
        
    Description:
        Given the tree of execution traces "albero", search for a cycle in it.
        
*********************************************************************************/
void GradedUtils_findCycle(BddFsm_ptr fsm, BddEnc_ptr enc, treeNode_ptr albero, cycleInf_ptr cicli)
{
    DDMgr_ptr dd = BddEnc_get_dd_manager(enc);
    bdd_ptr visitedStates;
    int nTraccia;

    if (albero == NIL(treeNode))
	{
		printf("Counterexample not computed\n");
		return;
	}
	 visitedStates = bdd_false(dd);
	 nTraccia = 1;
	
	GradedUtils_findCycleRecur(fsm, enc, dd, albero, visitedStates, &nTraccia, 1, cicli);
	//bdd_free(dd, visitedStates);
	return;
}



void GradedUtils_countTraces(BddFsm_ptr fsm, BddEnc_ptr enc, treeNode_ptr albero, int* nTraccia)
{
	adjList_ptr figli;
	
	if (albero == NIL(treeNode))
	{		
		return ;
	}
	
	figli = TreeUtils_getListaFigli(albero);
  
	while  (figli != NIL(adjList))
	{
		treeNode_ptr figlio = TreeUtils_getNodo(figli);
		GradedUtils_countTraces(fsm, enc, figlio, nTraccia);
		
		figli = TreeUtils_getNext(figli);
		if (figli != NIL(adjList))
		{
			(*nTraccia)++;
		}
	}
}



/**Function****************************************************************************
    Utilities for fsm states
******************************************************************************/

/*

Nome: GradedUtils_fsmCountSuccessors

Descrizione:
	Data una macchina a stati finiti con insieme di stati S e funzione di transizione T ed un sottoinsieme X di S, questa funzione calcola
	il numero di successori che ogni stato di S ha in X secondo la funzione di transizione T.

Lista dei parametri:
	BddFsm_ptr	fsm		Macchina a stati finiti di cui contare i successori
	bdd_ptr		states		L'insieme degli stati in cui contare i successori

Tipo di ritorno:
	add_ptr		Un add che rappresenta il numero di successori che ogni stato della fsm ha nell'insieme states


English version：

Given a finite state machine with set of states S and transition function T and a subset X of S, this function calculates
the number of successors that each state of S has in X according to the transition function T.

List of parameters:
BddFsm_ptr fsm Finite state machine to count successors
bdd_ptr states The set of states in which to count successors

Return type:
add_ptr An ADD that represents the number of successors that each state of the fsm has in the set states
计算状态集S中每个状态的后继数目（这些后继要求在X中）
 S输入fsm, X是states
*/

add_ptr GradedUtils_fsmCountSuccessors(BddFsm_ptr fsm, bdd_ptr states, int k) {
	BddEnc_ptr enc = BddFsm_get_bdd_encoding(fsm);
	DDMgr_ptr dd = BddEnc_get_dd_manager(enc);
	BddTrans_ptr trans = BddFsm_get_trans(fsm);	
	ClusterList_ptr cluster_list = BddTrans_get_backward(trans);

    const NuSMVEnv_ptr env = EnvObject_get_environment(ENV_OBJECT(enc));
    const StreamMgr_ptr streams =
            STREAM_MGR(NuSMVEnv_get_value(env, ENV_STREAM_MANAGER));
    const OStream_ptr outstream = StreamMgr_get_output_ostream(streams);
	//反向搜索的关系迁移
    bdd_ptr sum_variable;
	
	bdd_ptr primed_states;
    bdd_ptr states_add;
	add_ptr transition, result;
	ClusterListIterator_ptr iter;
	Cluster_ptr cluster;
	int index;
	DdNode* result_max, * state_vars_primed_max,* transition_max,* tmp_result_max;



	int i=0;


	primed_states = BddEnc_state_var_to_next_state_var(enc, states);
//	states_add = Cudd_BddToAdd(dd->dd, states);
   // write_dd(dd->dd, states_add, "/mnt/d/WSL/NuSMV-2.6.0-MultiCE/NuSMV/test/states.dot");  /*Write the resulting cascade dd to a file*/
//    write_dd(dd->dd, primed_states, "/mnt/d/WSL/NuSMV-2.6.0-MultiCE/NuSMV/test/primed_states.dot");  /*Write the resulting cascade dd to a file*/
//    print_add_dot(enc,states_add,"/mnt/d/WSL/NuSMV-2.6.0-MultiCE/NuSMV/test/add.dot");
	result = bdd_to_add(dd, states);



	/*******Print_test*******************************************/
	//print_add_dot(enc,states_add,"/mnt/d/WSL/NuSMV-2.6.0-MultiCE/NuSMV/test/rt.dot");
	print_add_dot(enc,result,"/mnt/d/WSL/NuSMV-2.6.0-MultiCE/NuSMV/test/nrt.dot");
    //write_dd(dd->dd, result, "/mnt/d/WSL/NuSMV-2.6.0-MultiCE/NuSMV/test/result.dot");  /*Write the resulting cascade dd to a file*/
	dd_printminterm(dd, primed_states);
	/************************************************************/




//    return result;
//	printf("/***********states （GradedUtils_fsmCountSuccessors）**********/\n");
//	BddEnc_print_set_of_states(enc, states, false,true, (VPFNNF) NULL,outstream,NULL);
//	printf("/***********states （GradedUtils_fsmCountSuccessors）**********/\n");
//
//    printf("/***********result （GradedUtils_fsmCountSuccessors）**********/\n");
//    BddEnc_print_set_of_states(enc, primed_states, false,true, (VPFNNF) NULL,outstream,NULL);
//    printf("/***********result （GradedUtils_fsmCountSuccessors）**********/\n");

    bdd_free(dd, primed_states);

    sum_variable = bdd_true(dd);
	iter = ClusterList_begin(cluster_list);
	index=0;
	//遍历transition  relation
	while ( ! ClusterListIterator_is_end(iter) ) {
		cluster = ClusterList_get_cluster(cluster_list, iter);

		{
			bdd_ptr tmp1 = GradedUtils_getCleanTransition(dd,fsm, cluster);
			//GradedUtils_applyReachableAndMaskToBackTransitions(fsm, &tmp1);
			transition = bdd_to_add(dd, tmp1);
			bdd_free(dd, tmp1);
		}

		{
			//add_ptr tmpb = GradedUtils_addApply(dd, DD_ADD_NODE_TIMES_TAG, result, transition, k);

			bdd_ptr cur_var = Cluster_get_quantification_state(cluster);
			bdd_ptr state_vars_primed = bdd_forsome(dd, cur_var, sum_variable);
            add_ptr tmp_result;
			//sum_variable=true则返回cur_var

			//add_ptr tmp_result = GradedUtils_addAbstract(dd, tmpb, state_vars_primed, k);

            result_max=Cudd_addFindMax(dd->dd,(DdNode *)result);
            transition_max=Cudd_addFindMax(dd->dd,(DdNode *)transition);
            state_vars_primed_max=Cudd_addFindMax(dd->dd,(DdNode *)bdd_to_add(dd,state_vars_primed));

            printf("result%d:%d\n",index,cuddV(result_max)->left.inttype);
            printf("transition%d:%d\n",index,cuddV(transition_max)->left.inttype);
            printf("state_vars_primed%d:%d\n",index,cuddV(state_vars_primed_max)->left.inttype);


            tmp_result = GradedUtils_AddAndAbstract(dd, result, transition, state_vars_primed, k);

			tmp_result_max=Cudd_addFindMax(dd->dd,(DdNode *)tmp_result);
			printf("tmp_result%d:%d\n",index,cuddV(tmp_result_max)->left.inttype);

			add_free(dd, result);
			result = add_dup(tmp_result);

			bdd_and_accumulate(dd, &sum_variable, state_vars_primed);
			//交操作，sum刚开始是true所以结果是state_vars_primed，然后不断地取交集

			add_free(dd, tmp_result);
			bdd_free(dd, cur_var);
			bdd_free(dd, state_vars_primed);
			add_free(dd, transition);
		}

		iter = ClusterListIterator_next(iter);
	    index++;
	} /* iteration */
	
	/*bdd_ptr primed_vars = BddEnc_get_next_state_vars_cube(enc);
	result = GradedUtils_addAbstract(dd, result, primed_vars, k);*/
	
	bdd_free(dd, sum_variable);
	return result;
}


void GradedUtils_fixPoint(BddFsm_ptr fsm, bdd_ptr* states, bdd_ptr valid) {
	BddTrans_ptr transition = BddFsm_get_trans(fsm);
	BddEnc_ptr enc = BddFsm_get_bdd_encoding(fsm);
	DDMgr_ptr dd = BddEnc_get_dd_manager(enc);
	
	BddStates backward_tmp,backward;
	bdd_ptr actual = bdd_dup(*states);
	
	while (1) {
		//backward_tmp = BddFsm_get_backward_image(fsm, actual);
		backward_tmp = (BddStates) ex(fsm, actual);
		backward = BddEnc_apply_state_frozen_vars_mask_bdd(enc,backward_tmp);
		bdd_free(dd, backward_tmp);
		bdd_and_accumulate(dd, &backward, valid);
		
		if (GradedUtils_bddIsSubset(dd, backward, actual)) {
			bdd_free(dd, backward);
			bdd_free(dd, *states);
			*states = bdd_dup(actual);
			bdd_free(dd, actual);
			return;
		} else {
			bdd_or_accumulate(dd, &actual, backward);
			bdd_free(dd, backward);
		}
	}
}

void GradedUtils_applyReachableAndMask(BddFsm_ptr fsm, bdd_ptr* states) {
	BddEnc_ptr enc = BddFsm_get_bdd_encoding(fsm);
	DDMgr_ptr dd = BddEnc_get_dd_manager(enc);
	BddStates reachable;
	bdd_ptr tmp;
	
	tmp = BddFsm_get_reachable_states(fsm);
	reachable = BddEnc_apply_state_frozen_vars_mask_bdd(enc, tmp);
	bdd_free(dd, tmp);
	
	bdd_and_accumulate(dd, states, reachable);
	
	tmp = BddEnc_apply_state_frozen_vars_mask_bdd(enc, *states);
	
	/*Modifica effettuata da Maurizio Memoli*/
	
	bdd_free(dd, *states);
	
	/****************************************/
	*states = bdd_dup(tmp);
	
	bdd_free(dd, tmp);
	bdd_free(dd, reachable);
}

void GradedUtils_applyReachableAndMaskToBackTransitions(BddFsm_ptr fsm, bdd_ptr* transition) {
	BddEnc_ptr enc = BddFsm_get_bdd_encoding(fsm);
	DDMgr_ptr dd = BddEnc_get_dd_manager(enc);
	
	{
		//bdd_ptr tmp = BddFsm_get_reachable_states(fsm);
		//bdd_ptr reachable = BddEnc_apply_state_vars_mask_bdd(enc, tmp);
		bdd_ptr reachable = BddFsm_get_reachable_states(fsm);
		//bdd_ptr primed_reachable = BddEnc_state_var_to_next_state_var(enc, reachable);
		//bdd_free(dd, tmp);

		bdd_and_accumulate(dd, transition, reachable);
		//bdd_and_accumulate(dd, transition, primed_reachable);
	
		bdd_free(dd, reachable);
		//bdd_free(dd, primed_reachable);
	}
	
	{
		bdd_ptr mask = BddEnc_get_state_frozen_vars_mask_bdd(enc);
		//bdd_ptr primed_mask = BddEnc_state_var_to_next_state_var(enc, mask);
		
		bdd_and_accumulate(dd, transition, mask);
		//bdd_and_accumulate(dd, transition, primed_mask);
		
		bdd_free(dd, mask);
		//bdd_free(dd, primed_mask);
	}
}

BddStates GradedUtils_getKBackwardImage(const BddFsm_ptr fsm, BddStates states, int k) {
	BddEnc_ptr enc = BddFsm_get_bdd_encoding(fsm);
	DDMgr_ptr  dd = BddEnc_get_dd_manager(enc);
   	BddStates invar = BddFsm_get_state_constraints(fsm);
	bdd_ptr result = bdd_false(dd);
	
	BDD_FSM_CHECK_INSTANCE(fsm);
	
	//printf("Numero di stati in states: %f\n", BddEnc_count_states_of_bdd(enc, states));
	{
		/*bdd_ptr tmp1 = BddFsm_get_init(fsm);
		bdd_ptr tmp2 = BddFsm_get_forward_image(fsm, tmp1);
		bdd_ptr tmp3 = bdd_and(dd, tmp2, states);*/
		
		//printf("Numero di stati ininiziali: %f\n", BddEnc_count_states_of_bdd(enc, tmp1));
		//printf("Numero di stati successivi dello stato iniziale: %f\n", BddEnc_count_states_of_bdd(enc, tmp2));
		/*dd_printminterm(dd, tmp2);
		BddEnc_print_set_of_states(enc, tmp2, 1, stdout);*/
		//printf("Numero di stati in tmp3: %f\n", BddEnc_count_states_of_bdd(enc, tmp3));
		/*dd_printminterm(dd, tmp3);
		BddEnc_print_set_of_states(enc, tmp3, 1, stdout);*/
		/*bdd_free(dd, tmp1);
		bdd_free(dd, tmp2);
		bdd_free(dd, tmp3);*/
	}
	
	{
		bdd_ptr tmp1 = bdd_and(dd, states, invar);
		bdd_ptr tmp2 = BddEnc_apply_state_frozen_vars_mask_bdd(enc, tmp1);
		add_ptr tmp3 = GradedUtils_fsmCountSuccessors(fsm, tmp2, k+1);
	
        	result = add_to_bdd_strict_threshold(dd, tmp3, k);
        
		bdd_free(dd, tmp1);
		bdd_free(dd, tmp2);
		add_free(dd, tmp3);
	}
		
	bdd_and_accumulate(dd, &result, invar);
	bdd_free(dd, invar);
	
	return (result);
}


/**Function********************************************************************

  Synopsis [Picks one on-set minterm deterministically from the given DD.]

  Description [Picks one on-set minterm deterministically from the
  given DD. The minterm is in terms of vars. Builds a BDD for the
  minterm and returns a pointer to it if successful; NULL
  otherwise. There are two reasons why the procedure may fail: It may
  run out of memory; or the function f may be the constant 0.]

  SideEffects [None]

  SeeAlso     []

******************************************************************************/
DdNode *
GradedUtils_bddPickOneMintermNR(
DDMgr_ptr dd,
DdNode *f,
DdNode **vars,
int n)
{
    char *string;
    int i, size;
    int *indices;
    int result;
    DdNode *zero, *old, *new;

    size = dd->dd->size;
    string = ALLOC(char, size);
    if (string == NULL)
	return(NULL);
    indices = ALLOC(int,n);
    if (indices == NULL) {
	FREE(string);
	return(NULL);
    }

    for (i = 0; i < n; i++) {
        indices[i] = vars[i]->index;
    }

    result = Cudd_bddPickOneCubeNR(dd->dd,f,string);
    if (result == 0) {
	FREE(string);
	FREE(indices);
	return(NULL);
    }

    /*
      Don't cares always set to 0.
      A cube is represented as an array of literals, which are integers in
      {0, 1, 2}; 0 represents a complemented literal, 1 represents an
      uncomplemented literal, and 2 stands for don't care.
    */
    for (i = 0; i < n; i++) {
      if (string[indices[i]] == 2) {
        /* For dont care we choose false */
        string[indices[i]] = 0;
      }
    }

    /* Build result BDD. */
    old = Cudd_ReadTrue(dd->dd);
    cuddRef(old);
    zero = Cudd_Not(Cudd_ReadTrue(dd->dd));

    for (i = 0; i < n; i++) {
	if (string[indices[i]] == 0) {
	    new = Cudd_bddIte(dd->dd,old,Cudd_Not(vars[i]),zero);
	} else {
	    new = Cudd_bddIte(dd->dd,old,vars[i],zero);
	}
	if (new == NULL) {
	    FREE(string);
	    FREE(indices);
	    Cudd_RecursiveDeref(dd->dd,old);
	    return(NULL);
	}
	cuddRef(new);
	Cudd_RecursiveDeref(dd->dd,old);
	old = new;
    }

    /* Test. */
    //if (Cudd_bddLeq(dd,old,f)) {
	cuddDeref(old);
    /*} else {
	Cudd_RecursiveDeref(dd,old);
	old = NULL;
    }*/

    FREE(string);
    FREE(indices);
    return(old);

}  /* end of GradedUtils_bddPickOneMintermNR */

/**Function********************************************************************

  Synopsis [Picks one on-set minterm deterministically from the given BDD.]

  Description [Picks one on-set minterm deterministically from the
  given DD. The minterm is in terms of vars. Builds a BDD for the
  minterm and returns a pointer to it if successful; a failure is
  generated otherwise. There are two reasons why the procedure may fail: It may
  run out of memory; or the function fn may be the constant 0. The
  result is referenced.]

  SideEffects []

******************************************************************************/
bdd_ptr GradedUtils_bddPickOneState(const BddEnc_ptr enc, bdd_ptr fn)
{
  DdNode * result;
  DdNode ** vars;
  DDMgr_ptr  dd = BddEnc_get_dd_manager(enc);
    int n;


  if (bdd_is_false(dd, fn)) {
    Cudd_Ref(fn);
    return(fn);
  }
  else {
    vars = array_fetch_p(bdd_ptr, enc->minterm_state_vars, 0);
     n = enc->minterm_state_vars_dim;
    result = GradedUtils_bddPickOneMintermNR(dd, (DdNode *)fn, vars, n);
    //common_error(result, "GradedUtils_bddPickOneMintermNR: result = NULL");
    Cudd_Ref(result);
    return((bdd_ptr)result);
  }
}

/**Function********************************************************************

  Synopsis [Takes the * of two ADDs and simultaneously abstracts the
  variables in cube.]

  Description [Takes the * of two ADDs and simultaneously abstracts
  the variables in cube. The variables are existentially abstracted.
  Returns a pointer to the result is successful; NULL otherwise.]

  SideEffects [None]

  SeeAlso     [GradedUtils_AddAndAbstract]

******************************************************************************/

DdNode *
GradedUtils_AddAndAbstractRecur(
  DDMgr_ptr  manager,
  DdNode * f,
  DdNode * g,
  DdNode * cube,
  DdNode * k)
{
    const NuSMVEnv_ptr env = EnvObject_get_environment(ENV_OBJECT(manager));
    const NodeMgr_ptr nodemgr =
            NODE_MGR(NuSMVEnv_get_value(env, ENV_NODE_MGR));
    DdNode *ft, *fe,*gt, *ge;
    DdNode *r, *t, *e;
    unsigned int topf, topg, topcube, top, index;

    statLine(manager->dd);
	
	
	
	
    topf = Cudd_ReadPerm(manager->dd, f->index);
    topg = Cudd_ReadPerm(manager->dd, g->index);
    top = ddMin(topf, topg);
    topcube = Cudd_ReadPerm(manager->dd, cube->index);

    if (topcube < top) {

	/*cube = cuddT(cube);
	if (cuddIsConstant(cube)) {
	    return(GradedUtils_addApplyRecur(manager, DD_ADD_NODE_TIMES_TAG, f, g, k));
	}

	topcube = Cudd_ReadPerm(manager, cube->index);*/
	add_ptr res1 = GradedUtils_AddAndAbstractRecur(manager, f, g, cuddT(cube), k);
	if (res1 == NULL) return NULL;
	cuddRef(res1);
	
	r = GradedUtils_addApplyRecur(manager, DD_ADD_NODE_PLUS_TAG, res1, res1, k);
	if (r == NULL) {
	  Cudd_RecursiveDeref(manager->dd, res1);
	  return NULL;
	}
	cuddRef(r);
	Cudd_RecursiveDeref(manager->dd, res1);
	cuddDeref(r);
	return r;
    }
    
    /*
	Check for terminals. If it's the case then the TIMES operator is applied to
	the operands f and g.    
	*/
	if(cuddIsConstant(f) && cuddIsConstant(g)){
		CUDD_VALUE_TYPE res_n;
		res_n = GradedUtils_nodeTimes(env,cuddV(f), cuddV(g), cuddV(k));
		if (res_n == NULL) return(NULL);
		
		return(cuddUniqueConst(manager->dd,res_n));
	}
    
	/* Check cache */
	r = GradedUtils_cacheLookup4(manager->dd, (ptruint)(DD_ADD_NODE_ANDABSTRACT_TAG),f,g,cube,k);
	//r = cuddCacheLookup(manager,(ptruint)(DD_ADD_NODE_ANDABSTRACT_TAG),f,g,cube);
	if (r != NULL) return(r);
    
    
    /* Now, topcube >= top. */


    if (topf == top) {
	index = f->index;
	ft = cuddT(f);
	fe = cuddE(f);
    } else {
	index = g->index;
	ft = fe = f;
    }

    if (topg == top) {
	gt = cuddT(g);
	ge = cuddE(g);
    } else {
	gt = ge = g;
    }
	


    if (topcube == top) {	/* quantify */
		DdNode *Cube = cuddT(cube);
	
		t = GradedUtils_AddAndAbstractRecur(manager, ft, gt, Cube, k);
		if (t==NULL) return NULL;
		cuddRef(t);
		
		e = GradedUtils_AddAndAbstractRecur(manager, fe, ge, Cube, k);
		if (e==NULL) {
			Cudd_RecursiveDeref(manager->dd, t);
			return(NULL);
		}
		
		cuddRef(e);
		r = GradedUtils_addApplyRecur(manager, DD_ADD_NODE_PLUS_TAG, t, e, k);
		if (r == NULL) {
			Cudd_RecursiveDeref(manager->dd,t);
			Cudd_RecursiveDeref(manager->dd,e);
			return(NULL);
		}
		cuddRef(r);
		Cudd_RecursiveDeref(manager->dd,t);
		Cudd_RecursiveDeref(manager->dd,e);
		GradedUtils_cacheInsert4((ptruint)(DD_ADD_NODE_ANDABSTRACT_TAG), f, g, cube, k, r);
		//cuddCacheInsert(manager,(ptruint)(DD_ADD_NODE_ANDABSTRACT_TAG),f,g,cube,r);
		cuddDeref(r);
		return(r);
	
    } else {
	
		t = GradedUtils_AddAndAbstractRecur(manager, ft, gt, cube, k);
		if (t == NULL) return(NULL);
		cuddRef(t);
		e = GradedUtils_AddAndAbstractRecur(manager, fe, ge, cube, k);
		if (e == NULL) {
			Cudd_RecursiveDeref(manager->dd,t);
			return(NULL);
		}
		cuddRef(e);
		r = cuddUniqueInter(manager->dd, (int) index, t, e);
		if (r == NULL) {
			Cudd_RecursiveDeref(manager->dd,t);
			Cudd_RecursiveDeref(manager->dd,e);
			return(NULL);
		}
		cuddDeref(t);
		cuddDeref(e);
		GradedUtils_cacheInsert4((ptruint)(DD_ADD_NODE_ANDABSTRACT_TAG), f, g, cube, k, r);
		//cuddCacheInsert(manager,(ptruint)(DD_ADD_NODE_ANDABSTRACT_TAG),f,g,cube,r);
		return(r);
	
    }


}


/**Function********************************************************************

  Synopsis [Takes the * of two ADDs and simultaneously abstracts the
  variables in cube.]

  Description [Takes the * of two ADDs and simultaneously abstracts
  the variables in cube. The variables are existentially abstracted.
  Returns a pointer to the result is successful; NULL otherwise.]

  SideEffects [None]

  SeeAlso     [GradedUtils_AddAndAbstractRecur]

******************************************************************************/
DdNode * GradedUtils_AddAndAbstract(
  DDMgr_ptr  manager,
  DdNode * f,
  DdNode * g,
  DdNode * cube,
  int k
)
{
    const NuSMVEnv_ptr env = EnvObject_get_environment(ENV_OBJECT(manager));
    const NodeMgr_ptr nodemgr =
            NODE_MGR(NuSMVEnv_get_value(env, ENV_NODE_MGR));
    DdNode *res;
    add_ptr add_k = add_leaf(manager, find_node(nodemgr,NUMBER, NODE_FROM_INT(k), Nil));
    do {
	manager->dd->reordered = 0;
	res = GradedUtils_AddAndAbstractRecur(manager, f, g, cube, add_k);
    } while (manager->dd->reordered == 1);
    cuddRef(res);
    return(res);

} /* end of GradedUtils_AddAndAbstract */
