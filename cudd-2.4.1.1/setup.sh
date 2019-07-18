#! /bin/sh

CREATE="ln -f -s"

HEADERS="cudd/cudd.h cudd/cuddInt.h dddmp/dddmp.h mtr/mtr.h st/st.h \
    util/util.h ../epd/epd.h obj/cuddObj.hh mnemosyne/mnemosyne.h"

LIBS="cudd/libcudd.a mtr/libmtr.a st/libst.a util/libutil.a"
INCLUDE_DIR=include/cudd

printf "Setting up cudd...  "
if [ ! -d $INCLUDE_DIR ]; then
    mkdir -p $INCLUDE_DIR
fi
cd $INCLUDE_DIR
for file in $HEADERS; do
    if [ ! -h `basename $file` ] && [ -f ../../$file ]; then 
	$CREATE ../../$file .
    fi
done 
cd -


if [ ! -d lib ]; then 
    mkdir lib
fi
cd lib
for file in $LIBS; do
    if [ ! -h `basename $file` ] && [ -f ../$file ]; then 
	$CREATE ../$file .
    fi
done 
if [ -f libutil.a ]; then
    mv libutil.a libcudd_util.a
fi
cd ..

printf "Done\n"
