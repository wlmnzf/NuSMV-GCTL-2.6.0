/******************************************************************************

    File name:
        ModelChecking.h
        
    Package name:
        graded/mc
    
    Synopsis:
        Main graded-CTL model checking algorithm. External header file.
        
    Description:
        Main function for model checking of graded-CTL formulas.

    Author:
        Alessandro Ferrante

******************************************************************************/

#ifndef __GRADEDMC_MODELCHECKING_H__
#define __GRADEDMC_MODELCHECKING_H__

/*****************************************************************************/
/* Libraries                                                                 */
/*****************************************************************************/

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
#include "nusmv/core/utils/StreamMgr.h"
#include "nusmv/core/utils/Logger.h"
#include "nusmv/core/node/NodeMgr.h"
#include "nusmv/core/utils/ErrorMgr.h"
#include "nusmv/core/node/NodeMgr.h"
#include "nusmv/shell/fsm/bdd/bddCmd.h"

#include "nusmv/core/graded/mc/Eval.h"
#include "nusmv/core/graded/mc/Explain.h"
#include "nusmv/core/graded/utils/GradedUtils.h"
#include "nusmv/core/mc/printinfo.h"

/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/

extern void GradedMc_checkGradedCtlSpec(NuSMVEnv_ptr,Prop_ptr);

#endif
