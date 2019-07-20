/**CHeaderFile*****************************************************************

  FileName    [Explain.h]

  PackageName [graded/mc]

  Synopsis    [Fair graded-CTL model checking algorithms. External header file.]

  Description [Fair graded-CTL model checking algorithms. External header file.]

  Author      [Alessandro Ferrante]
******************************************************************************/

#ifndef __GRADEDMC_EXPLAIN_H__
#define __GRADEDMC_EXPLAIN_H__

#include "nusmv/core/dd/dd.h"
#include "nusmv/core/enc/enc.h"
#include "nusmv/core/enc/operators.h"
#include "nusmv/core/fsm/bdd/BddFsm.h"
#include "nusmv/core/fsm/bdd/FairnessList.h"
#include "nusmv/core/mc/mc.h"
#include "nusmv/core/mc/mcInt.h"
#include "nusmv/core/parser/symbols.h"
#include "nusmv/core/prop/prop.h"
#include "nusmv/core/trace/Trace.h"
#include "nusmv/core/trace/TraceMgr.h"
#include "nusmv/core/utils/error.h"
#include "nusmv/core/utils/utils.h"
#include "nusmv/core/utils/utils_io.h"
#include "nusmv/core/node/NodeMgr.h"

#include "nusmv/core/graded/mc/Eval.h"
#include "nusmv/core/graded/mc/GradedCtl.h"
#include "nusmv/core/graded/mc/ModelChecking.h"
#include "nusmv/core/graded/utils/GradedUtils.h"

/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/

static treeNode_ptr GradedMc_explainRecurGraded (BddFsm_ptr, BddEnc_ptr, BddStates, node_ptr, node_ptr, boolean, boolean);
treeNode_ptr GradedMc_explainGraded(BddFsm_ptr fsm, BddEnc_ptr enc, BddStates radice,
                                    node_ptr spec_formula, node_ptr context);

#endif
