/******************************************************************************

    File name:
        ModelChecking.c

    Package name:
        graded/mc

    Synopsis:
        Main graded-CTL model checking algorithm.

    Description:
        Main function for the model checking of graded-CTL formulas.
        
    Author:
        Alessandro Ferrante

******************************************************************************/

#include <nusmv/core/dd/DDMgr_private.h>
#include "ModelChecking.h"


static char rcsid[] UTIL_UNUSED = "$Id: ModelChecking.c,v 1.0.3 2008/12/05 08:55:58 nusmv Exp $";



/*---------------------------------------------------------------------------*/
/* Variable declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Static function prototypes                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Definition of exported functions                                          */
/*---------------------------------------------------------------------------*/

/******************************************************************************

    Function name:
        GradedMc_checkGradedCtlSpec
        
    Parameters:
        Prop_ptr    property        The formula to be verified
        
    Result:
        void
        
    Description:
        The main function to make the model checking.
*******************************************************************************/
void GradedMc_checkGradedCtlSpec(NuSMVEnv_ptr env,Prop_ptr prop) {
	bdd_ptr constraints, fair, initial, states;
	node_ptr exp;
	bdd_ptr tmp1, tmp2;
	BddFsm_ptr fsm=BDD_FSM(NULL);
	BddEnc_ptr enc;
	DDMgr_ptr dd;
	Expr_ptr spec = Prop_get_expr_core(prop);
	StreamMgr_ptr streams;
	OStream_ptr outstream;
    treeNode_ptr albero;
    FILE *fp;
    int j;
    int nTracce = 1;
    char** argv[4]={"dump_fsm","-o","/mnt/d/WSL/NuSMV-2.6.0-MultiCE/NuSMV/test/fsm.dot","-t"};
    //-iItr
	cycleInf_ptr cicli;
	const ErrorMgr_ptr errmgr =
			ERROR_MGR(NuSMVEnv_get_value(env, ENV_ERROR_MANAGER));

	const OptsHandler_ptr opts =
			OPTS_HANDLER(NuSMVEnv_get_value(env, ENV_OPTS_HANDLER));
	streams = STREAM_MGR(NuSMVEnv_get_value(env, ENV_STREAM_MANAGER));
    outstream = StreamMgr_get_output_ostream(streams);


//	const NuSMVEnv_ptr env = Be_Manager_GetEnvironment(manager);


	if (opt_verbose_level_gt(opts, 0)) {
		Logger_ptr logger = LOGGER(NuSMVEnv_get_value(env, ENV_LOGGER));
		Logger_log(logger, "evaluating ");
		print_spec(Logger_get_ostream(logger), prop, get_prop_print_method(opts));
		Logger_log(logger, "\n");

//
//		StreamMgr_print_error(streams,  "evaluating ");
//		print_spec(nusmv_stderr, prop);
//		StreamMgr_print_error(streams,  "\n");
	}
	
	if (opt_cone_of_influence(opts) == true) {
		Prop_apply_coi_for_bdd(env,prop);
	}

    /*Recruits the properties of the fsm*/

//	BddFsm_ptr fsm = Prop_get_bdd_fsm(original);
//	if (NULL == fsm) {
//		if (opt_cone_of_influence(opts)) {
//			Prop_apply_coi_for_bdd(env, original);
//			fsm =  Prop_get_bdd_fsm(original);
//		}

//	sexp_fsm = SEXP_FSM(NuSMVEnv_get_value(env, ENV_SEXP_FSM));
//	SEXP_FSM_CHECK_INSTANCE(sexp_fsm);

	fsm = Prop_compute_ground_bdd_fsm(env, prop);
	BDD_FSM_CHECK_INSTANCE(fsm);

//	fsm = Prop_get_bdd_fsm(prop);
//	if (fsm == BDD_FSM(NULL)) {
//		Prop_set_fsm_to_master(prop);
//		fsm = Prop_get_bdd_fsm(prop);
	//	BDD_FSM_CHECK_INSTANCE(fsm);
//	}
	enc = BddFsm_get_bdd_encoding(fsm);
	dd = BddEnc_get_dd_manager(enc);
	constraints = BddFsm_get_state_constraints(fsm);
	fair = BddFsm_get_fair_states(fsm);
	initial = BddFsm_get_init(fsm);
	
	if (bdd_is_false(dd, fair))
		ErrorMgr_warning_fsm_fairness_empty(errmgr);


  fp = fopen("/mnt/d/WSL/NuSMV-2.6.0-MultiCE/NuSMV/test/spec.dot", "w");


              if (fp == NULL)
                  printf("fail to open the file! \n");
              else {
                  printf("The file is open! \n");
                  bst_print_dot(spec, fp);
               //   test_bdd(dd->dd);
                  //Compile_print_fsm_stats(env, fsm, outstream, true);


//                  dump_fsm -o test1.dot -iItr

//mask=65535
//                  (ptruint) ((mask & (ptrint) scan) /
//                             sizeof(DdNode)),
//                          (ptruint) ((mask & (ptrint) cuddE(scan)) /
//                                     sizeof(DdNode)));
                  CommandDumpFsm(env,4,argv);
                  fclose(fp);
              }



	/* Evaluates the spec */
	tmp1 = GradedMc_evalGradedCtlSpec(fsm, enc, spec, Nil);
    printf("**************S0**************\n");
    BddEnc_print_set_of_states(BddFsm_get_bdd_encoding(fsm), tmp1, false, true,(VPFBEFNNV) NULL,outstream,NULL);
    printf("**************S0**************\n");

//    BddEnc_print_set_of_states(enc, tmp1, false,true, (VPFNNF) NULL,
//                               outstream,NULL);


//tmp1为满足specification的状态，如果！tmp1余初始状态集合的交集费控，则说明从初始状态开始存在不满足的路径，模型检验失败
	tmp2 = bdd_not(dd, tmp1);
    printf("**************！S0**************\n");
    BddEnc_print_set_of_states(BddFsm_get_bdd_encoding(fsm), tmp2, false, true,(VPFBEFNNV) NULL,outstream,NULL);
    printf("**************！S0**************\n");
	bdd_free(dd, tmp1);
    
    bdd_and_accumulate(dd, &tmp2, constraints);
	bdd_and_accumulate(dd, &tmp2, fair);
	bdd_and_accumulate(dd, &tmp2, initial);

    printf("**************initial**************\n");
    BddEnc_print_set_of_states(BddFsm_get_bdd_encoding(fsm), initial, false, true,(VPFBEFNNV) NULL,outstream,NULL);
    printf("**************initial**************\n");
//
//    BddEnc_print_set_of_states(enc, initial , false,true, (VPFNNF) NULL,
//                               outstream,NULL);
//    BddEnc_print_set_of_states(enc, tmp2 , false,true, (VPFNNF) NULL,
//                               outstream,NULL);
	 states =BddEnc_apply_state_frozen_vars_mask_bdd(enc, tmp2);
//    states = BddEnc_apply_state_vars_mask_bdd(enc, tmp2);
    //bdd_free(dd, tmp2);
		
	/* Prints out the result, if not true explain. */
//	fprintf(nusmv_stdout, "-- ");
//	print_spec(nusmv_stdout, prop);
	StreamMgr_print_output(streams,  "-- ");
	print_spec(StreamMgr_get_output_ostream(streams),
			   prop, get_prop_print_method(opts));


	if (bdd_is_false(dd, states)) {
		StreamMgr_print_output(streams,   "is true\n");
//		fprintf(nusmv_stdout, "is true\n");
		Prop_set_status(prop, Prop_True);
	} else {
		int tr = 0;
		int i = 0;
		
//		fprintf(nusmv_stdout, "is false\n");
		StreamMgr_print_output(streams,   "is false\n");
		Prop_set_status(prop, Prop_False);

		//return;
//		printf("mau prima\n");
//		dd_printminterm(dd, states);

		tmp1 = GradedUtils_bddPickOneState(enc, states);

//		printf("stato selezionato\n");
//		dd_printminterm(dd, tmp1);
//		printf("mau dopo\n");
		
		 nTracce = 1;
		 albero = GradedMc_explainGraded(fsm, enc, tmp1, spec, Nil);
		
		
		GradedUtils_countTraces(fsm, enc, albero, &nTracce);
		
		 cicli = ALLOC(cycleInf, nTracce);

		for(j=0; j<nTracce; j++){
		  cicli[j].nStato = 0;
		}
		GradedUtils_findCycle(fsm, enc, albero, cicli);
		
		GradedUtils_stampaPaths(fsm, enc, albero, cicli);
		return;

//		if (opt_counter_examples(opts)) {
//			Trace_ptr trace;
//			node_ptr* counter;
//			char* trace_title = NULL;
//			char* trace_title_postfix = " Counterexample";
//			bool nullRes = false;
//
//			tmp1 = BddEnc_pick_one_state(enc, states);
//
//			counter = (node_ptr*)GradedMc_explainGraded(fsm, enc,
//                                cons((node_ptr)bdd_dup(tmp1), Nil), spec, Nil);
//
//			if (counter[0] == Nil) {
//				counter = ALLOC(node_ptr, 1);
//				counter[0] = cons((node_ptr)bdd_dup(tmp1), Nil);
//				counter[1] = Nil;
//				nullRes = true;
//			}
//
//			while(counter[i] != Nil) {
//				if (nullRes)
//					exp =  counter[i];
//				else
//					exp =  reverse(counter[i]);
//
//				if (exp == Nil) {
//					/* The counterexample consists of one initial state */
//					exp = cons((node_ptr)bdd_dup(tmp1), Nil);
//					nullRes = true;
//				}
//
//				trace_title = ALLOC(char, Prop_get_type_as_string(prop) +
//						strlen(trace_title_postfix) + 1);
//				nusmv_assert(trace_title != (char*) NULL);
//				strcpy(trace_title, Prop_get_type_as_string(prop));
//				strcat(trace_title, trace_title_postfix);
//
//				trace = Trace_create_from_state_input_list(enc, trace_title,
//						TRACE_TYPE_CNTEXAMPLE, exp);
//				FREE(trace_title);
//
//				tr = TraceManager_register_trace(global_trace_manager, trace);
//				Prop_set_trace(prop, Trace_get_id(trace));
//
//				fprintf(nusmv_stdout, "-- as demonstrated by the following execution sequence\n");
//
//				TraceManager_execute_plugin(global_trace_manager,
//						TraceManager_get_default_plugin(global_trace_manager), tr);
//
//				if (!nullRes){
//					exp =  reverse(exp);
//					nullRes = false;
//				}
//
//				/*walk_dd(dd_manager, bdd_free, explanation);
//				free_list(explanation);*/
//				i++;
//			}
//			bdd_free(dd, tmp1);
//			i = 0;
//			while (counter[i] != Nil)
//				walk_dd(dd, bdd_free, counter[i++]);
//		}
	}
	
	bdd_free(dd, tmp2);
	//bdd_free(dd, tmp1);
	bdd_free(dd, initial);
	bdd_free(dd, constraints);
	bdd_free(dd, fair);
}
