#include "stdio.h"
char *search_str26(int n)
{
    char *enum_kv[167]={

            "NUSMV_STATEMENTS_SYMBOL_FIRST = NUSMV_CORE_SYMBOL_FIRST",


            "TRANS", /* 101 */
            "INIT",
            "INVAR",
            "ASSIGN",
            "FAIRNESS",
            "JUSTICE",
            "COMPASSION",
            "SPEC",
            "LTLSPEC",
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            "CTLGSPEC",/* 110 */
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            "PSLSPEC", /* 110 */
            "INVARSPEC",
            "COMPUTE",
            "DEFINE",
            "ISA",
            "GOTO",
            "CONSTRAINT",
            "MODULE",
            "PROCESS",
            "MODTYPE",
            "LAMBDA", /* 120 */
            "CONSTANTS",

            /* To be moved elsewhere */
            "PRED",
            "ATTIME",
            "PREDS_LIST",
            "MIRROR",
            /* End to be moved elsewhere */

            "DEFINE_PROPERTY",

            "SYNTAX_ERROR",

            "NUSMV_STATEMENTS_SYMBOL_LAST",
            /* ---------------------------------------------------------------------- */


            /* ---------------------------------------------------------------------- */
            "NUSMV_EXPR_SYMBOL_FIRST",
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            "EGX",
            "EGG",
            "EGU",
            "EGF",
            "AGX",
            "AGG",
            "AGU",
            "AGF",
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

            "FAILURE",
            "CONTEXT",//131
            "EU",
            "AU",
            "EBU",
            "ABU",
            "MINU",
            "MAXU",
            "VAR",
            "FROZENVAR",
            "IVAR", /* 140 */
            "BOOLEAN",
            "ARRAY",
            "SCALAR",
            "CONS",
            "BDD", /* 145 */
            "SEMI",
            "EQDEF",
            "TWODOTS",
            "FALSEEXP",
            "TRUEEXP", /* 150 */
            "SELF",
            "CASE",
            "COLON",
            "IFTHENELSE",
            "SIMPWFF", /* 155 */
            "NEXTWFF",
            "LTLWFF",
            "CTLWFF",
            "COMPWFF",
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            "CTLGWFF",
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            "ATOM", /* 160 */
            "NUMBER",//161
            "COMMA",
            "IMPLIES",
            "IFF",
            "OR",
            "XOR",
            "XNOR",
            "AND",
            "NOT",//169
            "EX",
            "AX",
            "EF",//172
            "AF",
            "EG",
            "AG",
            "SINCE",
            "UNTIL",
            "TRIGGERED",
            "RELEASES",
            "EBF", /* 180 */
            "EBG",
            "ABF",
            "ABG",
            "OP_NEXT",
            "OP_GLOBAL",
            "OP_FUTURE",
            "OP_PREC",
            "OP_NOTPRECNOT",
            "OP_HISTORICAL",
            "OP_ONCE",
            "EQUAL",//191
            "NOTEQUAL",
            "LT",
            "GT",
            "LE",
            "GE",
            "UNION",
            "SETIN",
            "MOD",
            "PLUS", /* 200 */
            "MINUS",
            "TIMES",
            "DIVIDE",
            "UMINUS",
            "NEXT", /* 205 */
            "SMALLINIT",
            "DOT",
            "BIT",
            "RANGE",
            "UNSIGNED_WORD", /* identifies expressions and types */
            "SIGNED_WORD",   /* identifies types */
            "INTEGER",
            "REAL",
            "CONTINUOUS",

            "NUMBER_UNSIGNED_WORD", /* identifies constants */
            "NUMBER_SIGNED_WORD",   /* identifies constants */
            "NUMBER_FRAC",
            "NUMBER_REAL",
            "NUMBER_EXP",
            "LSHIFT",
            "RSHIFT", /* 220 */
            "LROTATE",
            "RROTATE",
            "BIT_SELECTION",
            "CONCATENATION",
            "CAST_BOOL",
            "CAST_WORD1",
            "CAST_SIGNED",
            "CAST_UNSIGNED",
            "EXTEND", /* 230 extend the width of a word (signed or unsigned) */
            "WORDARRAY",
            "WAREAD",
            "WAWRITE",
            "UWCONST", /* these are removed by the flattener, so */
            "SWCONST", /* after flattening they do not exist anywhere */
            "WRESIZE",
            "WSIZEOF",
            "CAST_TOINT",

            "COMPID", /* Used for comparing properties ids */

            "ARRAY_TYPE", /* 240 Used to represent generic array type */
            "ARRAY_DEF",  /* Define obtained via Matrix-Define */

            "NFUNCTION",
            "NFUNCTION_TYPE",
            "FUN",

            "COUNT",
            "FLOOR",

            "ITYPE", /* used to represent an internal type as read from the parser */

            "WORDARRAY_TYPE", /* Represent generic array type with word indices */
            "INTERNAL_ARRAY_STRUCT", /* Internal representation of the array structure
                    for processing the write function of arrays */
            "CONST_ARRAY", /* 250 Constant array */
            "INTARRAY",
            "INTARRAY_TYPE", /* Represent generic array type with integer indices */
            "TYPEOF", /* function for returning the type of variable */

            "CAST_TO_UNSIGNED_WORD", /* integer to bv conversion */

            "NUSMV_EXPR_SYMBOL_LAST",
            /* ---------------------------------------------------------------------- */

            "NUSMV_CORE_SYMBOL_LAST",
    };

    if(n>=100&&n<=266)
    {
        return enum_kv[n-100];
    }
    else
    {
        return "";
    }

}

char *search_str24(int n)
{
    char *enum_kv[167]={
            "NUSMV_STATEMENTS_SYMBOL_FIRST = NUSMV_CORE_SYMBOL_FIRST",


            "TRANS", /* 101 */
            "INIT",
            "INVAR",
            "ASSIGN",
            "FAIRNESS",
            "JUSTICE",
            "COMPASSION",
            "SPEC",
            "LTLSPEC",
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            "CTLGSPEC",/* 110 */
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            "PSLSPEC", /* 110 */
            "INVARSPEC",
            "COMPUTE",
            "DEFINE",
            "ISA",
            "GOTO",
            "CONSTRAINT",
            "MODULE",
            "PROCESS",
            "MODTYPE",
            "LAMBDA", /* 120 */
            "CONSTANTS",

            /* To be moved elsewhere */
            "PRED",
            "PREDS_LIST",
            "MIRROR",


            "NUSMV_STATEMENTS_SYMBOL_LAST",
            /* ---------------------------------------------------------------------- */


            /* ---------------------------------------------------------------------- */
            "NUSMV_EXPR_SYMBOL_FIRST",

            "FAILURE",
            "CONTEXT",//131
            "EU",
            "AU",
            "EBU",
            "ABU",
            "MINU",
            "MAXU",
            "VAR",
            "IVAR", /* 140 */
            "BOOLEAN",
            "ARRAY",
            "OF",
            "SCALAR",
            "CONS",
            "BDD", /* 145 */
            "SEMI",
            "LP",
            "RP",
            "LB",
            "RB",//150
            "LCB",
            "RCB",
            "EQDEF",
            "TWODOTS",
            "FALSEEXP",
            "TRUEEXP", /* 150 */
            "SELF",
            "CASE",
            "ESAC",
            "COLON",
            "IFTHENELSE",
            "INCONTEXT",
            "SIMPWFF", /* 155 */
            "LTLWFF",
            "CTLWFF",
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            "CTLGWFF",
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            "COMPWFF",
            "ATOM", /* 160 */
            "NUMBER",//161
            "COMMA",
            "IMPLIES",
            "IFF",
            "OR",
            "XOR",
            "XNOR",
            "AND",
            "NOT",//169
            "EX",
            "AX",
            "EF",//172
            "AF",
            "EG",
            "AG",
            "EE",
            "AA",
            "SINCE",
            "UNTIL",
            "TRIGGERED",
            "RELEASES",
            "EBF", /* 180 */
            "EBG",
            "ABF",
            "ABG",
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            "EGX",
            "EGG",
            "EGU",
            "EGF",
            "AGX",
            "AGG",
            "AGU",
            "AGF",
            /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            "BUNTIL",
            "MMIN",
            "MMAX",
            "OP_NEXT",
            "OP_GLOBAL",
            "OP_FUTURE",
            "OP_PREC",
            "OP_NOTPRECNOT",
            "OP_HISTORICAL",
            "OP_ONCE",
            "EQUAL",//191
            "NOTEQUAL",
            "LT",
            "GT",
            "LE",
            "GE",
            "UNION",
            "SETIN",
            "MOD",
            "PLUS", /* 200 */
            "MINUS",
            "TIMES",
            "DIVIDE",
            "UMINUS",
            "NEXT", /* 205 */
            "SMALLINIT",
            "DOT",
            "BIT",
            "RANGE",
            "WORD", /* identifies expressions and types */
            "INTEGER",
            "REAL",

            "NUMBER_WORD", /* identifies constants */
            "NUMBER_FRAC",
            "NUMBER_REAL",
            "NUMBER_EXP",
            "LSHIFT",
            "RSHIFT", /* 220 */
            "LROTATE",
            "RROTATE",
            "BIT_SELECTION",
            "CONCATENATION",
            "CAST_BOOL",
            "CAST_WORD1",
            /* 230 extend the width of a word (signed or unsigned) */
            "WORDARRAY",
            "WAREAD",
            "WAWRITE",

            /* Mathsat symbols */
            "SLT", /* signed less then */
            "SLE", /* signed less or equal */
            "SGT", /* signed greater then */
            "SGE", /* signed greater or equal */
            "SIGN_EXTEND", /* sign extend */

            "NUSMV_EXPR_SYMBOL_LAST",
            /* ---------------------------------------------------------------------- */

            "NUSMV_CORE_SYMBOL_LAST",
    };

    if(n>=100&&n<=266)
    {
        return enum_kv[n-100];
    }
    else
    {
        return "";
    }

}

int main()
{
    int a26,a24;
    while(scanf("%d%d",&a26,&a24)!=EOF) {
        printf("%s\n", search_str26(a26));
        printf("%s\n", search_str24(a24));
    }

    return 0;
}