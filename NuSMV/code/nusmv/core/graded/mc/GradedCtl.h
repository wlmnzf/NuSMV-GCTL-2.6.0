/**CHeaderFile*****************************************************************

  FileName    [GradedCtl.h]

  PackageName [graded/mc]

  Synopsis    [Fair graded-CTL model checking algorithms. External header file.]

  Description [Fair graded-CTL model checking algorithms. External header file.]

  Author      [Alessandro Ferrante]
******************************************************************************/

#ifndef __GRADEDMC_GRADEDCTL_H__
#define __GRADEDMC_GRADEDCTL_H__

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
#include "cudd/cuddInt.h"

#include "nusmv/core/graded/mc/Eval.h"
#include "nusmv/core/graded/mc/Explain.h"
#include "nusmv/core/graded/utils/GradedUtils.h"

/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/

BddStates GradedMc_ex (BddFsm_ptr, BddStates, int);

BddStates GradedMc_eu (BddFsm_ptr, BddStates, BddStates, int);

BddStates GradedMc_eg (BddFsm_ptr, BddStates, int);

BddStates GradedMc_ef (BddFsm_ptr, BddStates, int);

BddStates GradedMc_au (BddFsm_ptr, BddStates, BddStates, int);

BddStates GradedMc_ag (BddFsm_ptr, BddStates, int);

BddStates GradedMc_af (BddFsm_ptr, BddStates, int);

BddStates GradedMc_iff(BddFsm_ptr fsm, BddStates f, BddStates g);


#endif /* __MC_H__ */
