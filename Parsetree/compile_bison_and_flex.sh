#! /bin/bash
#how to compile sql.l&sql.ypp:

bison -d sql.ypp
echo "bison completed"
flex -o lex.yy.cpp sql.l
echo "flex completed"
