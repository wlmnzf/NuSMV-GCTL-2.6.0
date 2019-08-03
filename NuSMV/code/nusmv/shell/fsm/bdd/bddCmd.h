/* ---------------------------------------------------------------------------


  This file is part of the ``bdd'' package.
  %COPYRIGHT%
  

-----------------------------------------------------------------------------*/

/*!
  \author Michele Dorigatti
  \brief Module header file for bdd shell commands

  Module header file for bdd shell commands

*/


#ifndef __NUSMV_SHELL_FSM_BDD_BDD_CMD_H__
#define __NUSMV_SHELL_FSM_BDD_BDD_CMD_H__

#include "nusmv/core/cinit/NuSMVEnv.h"
/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Type declarations                                                         */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Structure declarations                                                    */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Variable declarations                                                     */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Macro declarations                                                        */
/*---------------------------------------------------------------------------*/


/**AutomaticStart*************************************************************/

/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/



int CommandDumpFsm(NuSMVEnv_ptr env, int argc, char** argv);



/*!
  \brief Initializes the BddFsm package.

  Initializes the BddFsm package.
*/
void Bdd_Init(NuSMVEnv_ptr env);

/*!
  \brief Quit the BddFsm package

  Quit the BddFsm package
*/
void Bdd_End(NuSMVEnv_ptr env);

/**AutomaticEnd***************************************************************/

#endif /* __NUSMV_SHELL_FSM_BDD_BDD_CMD_H__ */
