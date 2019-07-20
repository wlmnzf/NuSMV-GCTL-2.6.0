/**CHeaderFile*****************************************************************

  FileName    [Eval.h]

  PackageName [graded/mc]

  Synopsis    [Fair graded-CTL model checking algorithms. External header file.]

  Description [Fair graded-CTL model checking algorithms. External header file.]

  Author      [Alessandro Ferrante]
******************************************************************************/

#ifndef __GRADEDMC_EVAL_H__
#define __GRADEDMC_EVAL_H__

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

#include "nusmv/core/graded/mc/Explain.h"
#include "nusmv/core/graded/mc/GradedCtl.h"
#include "nusmv/core/graded/mc/ModelChecking.h"
#include "nusmv/core/graded/utils/GradedUtils.h"

/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/

bdd_ptr GradedMc_evalGradedCtlSpec (BddFsm_ptr, BddEnc_ptr, node_ptr, node_ptr);

#endif
