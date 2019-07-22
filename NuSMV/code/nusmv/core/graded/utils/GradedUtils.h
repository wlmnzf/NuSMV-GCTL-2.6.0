#ifndef _GRADEDUTILS_

#define _GRADEDUTILS_

#include "nusmv/core/dd/dd.h"
#include "nusmv/core/enc/operators.h"
#include "nusmv/core/fsm/bdd/BddFsm.h"
#include "nusmv/core/parser/symbols.h"
#include "cudd/cuddInt.h"
#include "cudd/cudd.h"
#include "TreeUtils.h"
#include "nusmv/core/be/beInt.h"
#include "nusmv/core/compile/compile.h"
#include "nusmv/core/compile/symb_table/SymbTable.h"
#include "nusmv/core/enc/bdd/BddEnc_private.h"
#include "nusmv/core/graded/mc/GradedCtl.h"

typedef struct cycleInf *cycleInf_ptr;

typedef struct cycleInf{
       int nStato;
       bdd_ptr stato;
       boolean sink;
} cycleInf;


FILE* nusm_stdout;

add_ptr GradedUtils_addComposition(DDMgr_ptr, add_ptr, add_ptr, int);

add_ptr GradedUtils_fsmCountSuccessors(BddFsm_ptr, bdd_ptr, int);

void GradedUtils_fixPoint(BddFsm_ptr, bdd_ptr*, bdd_ptr);

int GradedUtils_isSubset(DDMgr_ptr, bdd_ptr, bdd_ptr);

void GradedUtils_applyReachableAndMask(BddFsm_ptr, bdd_ptr *);

void  GradedUtils_applyReachableAndMaskToTransitions(BddFsm_ptr, bdd_ptr*);

bdd_ptr GradedUtils_andTransition(BddFsm_ptr, bdd_ptr);

void GradedUtils_addSum(DDMgr_ptr, add_ptr*, add_ptr, int);

BddStates GradedUtils_getKBackwardImage(const BddFsm_ptr, BddStates, int);

treeNode_ptr GradedUtils_node_ptrToTreeNode_ptr(BddFsm_ptr, BddEnc_ptr, node_ptr, BddInputs);

DdNode * GradedUtils_addApplyRecur(DDMgr_ptr , ptruint, DdNode *, DdNode *, DdNode * );

DdNode * GradedUtils_addApply(DDMgr_ptr , ptruint, DdNode *, DdNode *, int);

DdNode * GradedUtils_addAbstract(DDMgr_ptr, add_ptr, bdd_ptr, int);

DdNode * GradedUtils_addAbstractRecur(DDMgr_ptr , ptruint, DdNode *, DdNode *, DdNode *);

DdNode * GradedUtils_AddAndAbstract(DDMgr_ptr ,  DdNode *, DdNode *, DdNode *, int);

bdd_ptr GradedUtils_bddPickOneState(const BddEnc_ptr enc, bdd_ptr fn);

void GradedUtils_countTraces(BddFsm_ptr fsm, BddEnc_ptr enc, treeNode_ptr albero, int* nTraccia);

void GradedUtils_findCycle(BddFsm_ptr fsm, BddEnc_ptr enc, treeNode_ptr albero, cycleInf_ptr cicli);

boolean GradedUtils_stampaPaths(BddFsm_ptr fsm, BddEnc_ptr enc, treeNode_ptr albero, cycleInf_ptr cicli);

BddStates * GradedUtils_euGetIntermediateSets(BddFsm_ptr fsm, BddStates f, BddStates g, int k);

BddStates * GradedUtils_egGetIntermediateSets(BddFsm_ptr fsm, BddStates f, int k);

#endif
