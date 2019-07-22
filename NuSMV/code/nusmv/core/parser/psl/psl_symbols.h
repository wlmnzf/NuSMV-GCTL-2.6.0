#ifndef __NUSMV_CORE_PARSER_PSL_PSL_SYMBOLS_H__

/*!
  \brief \todo Missing synopsis

  \todo Missing description
*/
#define __NUSMV_CORE_PARSER_PSL_PSL_SYMBOLS_H__

enum PSL_SYMBOLS_TAG {
  NUSMV_PSL_SYMBOL_FIRST = 1000, /* Do not touch this */
  /* ---------------------------------------------------------------------- */

  PSL_SERE,
  PSL_SERECONCAT,
  PSL_SEREFUSION,
  PSL_SERECOMPOUND,
  PSL_SEREREPEATED,
  PSL_CONCATENATION,

  PSL_REPLPROP,
  PSL_INF,
  PSL_FORALL,
  PSL_FORANY,
  PSL_PIPEMINUSGT,
  PSL_DIAMONDMINUSGT, /* Added for internal use only */
  PSL_PIPEEQGT,
  PSL_ALWAYS,
  PSL_NEVER,
  PSL_EVENTUALLYBANG,
  PSL_WITHINBANG,
  PSL_WITHIN,
  PSL_WITHINBANG_,
  PSL_WITHIN_,
  PSL_WHILENOTBANG,
  PSL_WHILENOT,
  PSL_WHILENOTBANG_,
  PSL_WHILENOT_,
  PSL_NEXT_EVENT_ABANG,
  PSL_NEXT_EVENT_A,
  PSL_NEXT_EVENT_EBANG,
  PSL_NEXT_EVENT_E,
  PSL_NEXT_EVENTBANG,
  PSL_NEXT_EVENT,
  PSL_NEXT_ABANG,
  PSL_NEXT_EBANG,
  PSL_NEXT_A,
  PSL_NEXT_E,
  PSL_NEXTBANG,
  PSL_NEXT,
  PSL_BEFOREBANG,
  PSL_BEFORE,
  PSL_BEFOREBANG_,
  PSL_BEFORE_,
  PSL_UNTILBANG,
  PSL_UNTIL,
  PSL_UNTILBANG_,
  PSL_UNTIL_,
  PSL_ABORT,
  PSL_XBANG,
  PSL_X,
  PSL_W,
  PSL_PIPEPIPE,
  PSL_AMPERSANDAMPERSAND,
  PSL_OR,
  PSL_CARET,
  PSL_LBSPLAT,
  PSL_LBEQ,
  PSL_LBMINUSGT,
  PSL_LBPLUSRB,  

  PSL_TILDE, 
  PSL_EQEQ, 
  
  PSL_RANGE, 
  PSL_ITE, 
  
  PSL_WSELECT, /* PSL bit selection */
  /* ---------------------------------------------------------------------- */  
  NUSMV_PSL_SYMBOL_LAST  /* Do not touch this */
};

#endif /* ___NUSMV_CORE_PARSER_PSL_PSL_SYMBOLS_H__ */
