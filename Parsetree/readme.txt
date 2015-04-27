how to compile sql.l&sql.ypp:

bison -d sql.ypp
flex -o lex.yy.cpp sql.l
