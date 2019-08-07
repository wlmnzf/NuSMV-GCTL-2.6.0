//
// Created by William on 2019/4/5.
//
#include "nusmv/core/mc/printinfo.h"


char *myitoa(int num,char *str,int radix)
{
    /* 索引表 */
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned unum; /* 中间变量 */
    int i=0,j,k;
    char temp;
    /* 确定unum的值 */
    if(radix==10&&num<0) /* 十进制负数 */
    {
        unum=(unsigned)-num;
        str[i++]='-';
    }
    else unum=(unsigned)num; /* 其它情况 */
    /* 逆序 */
    do
    {
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
    }while(unum);
    str[i]='\0';
    /* 转换 */
    if(str[0]=='-') k=1; /* 十进制负数 */
    else k=0;
    /* 将原来的“/2”改为“/2.0”，保证当num在16~255之间，radix等于16时，也能得到正确结果 */

    for(j=k;j<=(i-k-1)/2.0;j++)
    {
        temp=str[j];
        str[j]=str[i-j-1];
        str[i-j-1]=temp;
    }
    return str;
}

char *search_str(int n)
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


//TODO:boolean,array类型的暂未支持，目前支持number类型的
static int ind;



void bst_print_dot_aux(struct node* node, FILE* stream,char* last_str,int index)
{
    static int nullcount = 0;
    char *inttype=NULL;
    char strtype[1000];
    char buffer[100];
    char total_str[2000];
    int span=0;
    node_val *nv;
    strcpy(total_str,"");


    switch(node->type) {

//        case CONTEXT:
//        case AND:
//        case OR:
//        case XOR:
//        case XNOR:
//        case NOT:
//        case IMPLIES:
//        case IFF:
//        case EX:
//        case AX:
//        case EF:
//        case AG:
//        case AF:
//        case EG:
//        case EU:
//        case AU:
//        case EBU:
//        case ABU:
//        case EBF:
//        case ABF:
//        case EBG:
//        case ABG:
        //


        case ATOM:
//             nv=(node_val*)(node->left.nodetype);
            strcpy(strtype,node->left.strtype->text);
            fprintf(stream, "   \"%d : %s\" -> \"%d : %s\";\n",index, last_str,++ind, strtype);
            break;
        case NUMBER:
//            nv=(node_val*)(node->left.nodetype);
            inttype=myitoa(node->left.inttype,buffer,10);
            fprintf(stream, "   \"%d : %s\" -> \"%d : 【%s】\";\n", index,last_str,++ind, inttype);
            break;

//
//        case FAILURE:
//        case TRUEEXP:
//        case FALSEEXP:
//        case SELF:
//        case BOOLEAN:
//        case DOT:
//        case ARRAY:
//        case NUMBER_UNSIGNED_WORD:
//        case UWCONST:
//        case SWCONST:
//        case NUMBER_SIGNED_WORD:
//        case NUMBER_FRAC:
//        case NUMBER_REAL:
//        case NUMBER_EXP:
//        case BIT:break;

        default:
            if(node->type<100||node->type>256)  break;

            if ((node->left).nodetype)
            {
                inttype=search_str((node->left).nodetype->type);
                fprintf(stream, "   \"%d : %s\" -> \"%d : %s\";\n",index, last_str, ind+1,inttype);
                bst_print_dot_aux((node->left).nodetype, stream,inttype,++ind);

            }


            if ( (node->right).nodetype )
            {

                inttype=search_str((node->right).nodetype->type);

                fprintf(stream, "   \"%d : %s\" -> \"%d : %s\";\n", index,last_str,ind+1, inttype);
                bst_print_dot_aux((node->right).nodetype, stream,inttype,++ind);
//                ind++;
            }

            break;

    }


}

void bst_print_dot(struct node * tree, FILE* stream)
{
    int index=0;
    ind=0;
    fprintf(stream, "digraph Tree {\n");
    fprintf(stream, "    node [fontname=\"Arial\"];\n");

    if (!tree)
        fprintf(stream, "\n");
    else if (!(tree->left).nodetype && !(tree->right).nodetype && !(tree->left).strtype && !(tree->right).strtype) {
        fprintf(stream, "%d : Root:%s;\n",index, search_str(tree->type));
    }
    else {
        bst_print_dot_aux(tree, stream,search_str(tree->type),index);
    }

    fprintf(stream, "}\n");
}





void print_dd (DdManager *gbm, DdNode *dd, int n, int pr )
{
    printf("DdManager nodes: %ld | ", Cudd_ReadNodeCount(gbm)); /*Reports the number of live nodes in BDDs and ADDs*/
    printf("DdManager vars: %d | ", Cudd_ReadSize(gbm) ); /*Returns the number of BDD variables in existence*/
    printf("DdManager reorderings: %d | ", Cudd_ReadReorderings(gbm) ); /*Returns the number of times reordering has occurred*/
    printf("DdManager memory: %ld \n", Cudd_ReadMemoryInUse(gbm) ); /*Returns the memory in use by the manager measured in bytes*/
    Cudd_PrintDebug(gbm, dd, n, pr);  // Prints to the standard output a DD and its statistics: number of nodes, number of leaves, number of minterms.
}


/**
 * Writes a dot file representing the argument DDs
 * @param the node object
 */
void write_dd (DdManager *gbm, DdNode *dd, char* filename)
{
    DdNode **ddnodearray = (DdNode**)malloc(sizeof(DdNode*)); // initialize the function array
    FILE *outfile; // output file pointer for .dot file
    outfile = fopen(filename,"w");
    if (outfile == NULL) {
        printf("fail to write bdd! \n");
        return;
    }
    else
        printf("Dumping the bdd! \n");


    ddnodearray[0] = dd;
    Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile); // dump the function to .dot file
    free(ddnodearray);
    fclose (outfile); // close the file */
}


void test_bdd(DdManager *manager)
{

    DdNode * f;
    DdNode * x[4];
    DdNode * y[4];
    int i;
    DdNode * tmp1;
    DdNode * tmp2;
    FILE * fp;
    char * names[8] = { "x1", "x2", "x3", "x4", "y1", "y2", "y3", "y4" };
    int order[8];
    DdNode **ddnodearray;

    /* Initialize the bdd manager with default options */
    //manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);



    /* each new variable is put at the new of the current order */
    for(i=0;i<4;i++) {
        x[i] = Cudd_bddNewVar(manager);
    }

    for(i=0;i<4;i++) {
        y[i] = Cudd_bddNewVar(manager);
    }


    /* ordering = x1 < x2 < x3 < x4 < y1 < y2 < y3 < y4 */

    f = Cudd_ReadTrue(manager);
    Cudd_Ref(f);                      /* Explicit Reference */

    for(i=0;i<4;i++) {

        tmp1 = Cudd_bddXnor(manager,x[i],y[i]);        /* x[i] <=> y[i] */
        Cudd_Ref(tmp1);

        tmp2 = Cudd_bddAnd(manager,f,tmp1);
        Cudd_Ref(tmp2);

        Cudd_RecursiveDeref(manager,f);                 /* Explicit Dereference */
        Cudd_RecursiveDeref(manager,tmp1);

        f = tmp2;
    }

    /* dumping bdd in dot format */
    ddnodearray = (DdNode**)malloc(sizeof(DdNode*)); // initialize the function array
    ddnodearray[0] = f;
    fp = fopen("/mnt/d/WSL/NuSMV-2.6.0-MultiCE/NuSMV/test/f1.dot","w");
    Cudd_DumpDot(manager,1,ddnodearray,NULL,NULL,fp);
    fclose(fp);

    printf("Number of Nodes = %d\n",Cudd_DagSize(f));


    /* xi and yi are put together in order */
    for(i=0;i<4;i= i++) {
        order[2* i] = i;
        order[2* i + 1] = i + 4;
    }

    /* shuffle to new order */
    Cudd_ShuffleHeap(manager,order);


    /* dumping bdd in dot format */
    fp = fopen("/mnt/d/WSL/NuSMV-2.6.0-MultiCE/NuSMV/test/f2.dot","w");
    Cudd_DumpDot(manager,1,&f,NULL,NULL,fp);
    fclose(fp);


    printf("Number of Nodes After Shuffling = %d\n",Cudd_DagSize(f));

    Cudd_RecursiveDeref(manager,f);
    Cudd_Quit(manager);    /* Release Manager */

}


int print_add_dot(BddEnc_ptr enc,add_ptr add,char * filename)
{
    AddArray_ptr addarray;
    const char** labels;
    FILE *fp;
    int res;

    addarray = AddArray_create(1);
    AddArray_set_n(addarray, 0, add);
    labels = ALLOC(const char*, 1);
    labels[0]=util_strsav("result");

    fp = fopen(filename, "w");

    if (fp == NULL)
        printf("fail to open the file! \n");
    else
        printf("The file is open! \n");


    res = BddEnc_dump_addarray_dot(enc, addarray, NULL, fp);
}