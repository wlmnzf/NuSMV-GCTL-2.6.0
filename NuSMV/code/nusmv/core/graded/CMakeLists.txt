set(SRCS

mc/Eval.c
mc/Explain.c
mc/GradedCtl.c
mc/ModelChecking.c

utils/GradedUtils.c
utils/TreeUtils.c

)

nusmv_add_pkg_lib(${SRCS})

nusmv_add_pkg_headers(
utils/TreeUtils.h
utils/GradedUtils.h

mc/Eval.h
mc/Explain.h
mc/ModelChecking.h
mc/GradedCtl.h

  )
