AC_INIT([Claims], [0.3], [imdb@ecnu])
AM_INIT_AUTOMAKE([-Wall foreign])
AM_PROG_AR
AC_PROG_LIBTOOL
CPPFLAGS="-w -O0 -DTHERON_XS -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS"
AM_CONDITIONAL(OPT_TCMALLOC, true)
CXXFLAGS=${CXXFLAGS="-g  -std=c++11"}
AC_PROG_CXX
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  Catalog/Makefile
  Catalog/Test/Makefile
  Catalog/stat/Makefile 
  Client/Makefile 
  Client/Test/Makefile 
  Client/json/Makefile
  common/Makefile 
  common/Block/Makefile 
  common/Expression/Makefile 
  common/log/Makefile
  common/Schema/Makefile 
  common/Schema/Test/Makefile 
  common/types/Makefile 
  common/types/Test/Makefile 
  common/types/ttmath/Makefile 
  Daemon/Makefile 
  Daemon/Test/Makefile 
  Executor/Makefile 
  Executor/Test/Makefile 
  IndexManager/Makefile
  IndexManager/Test/Makefile
  Loader/Makefile
  Loader/Test/Makefile
  common/serialization/Makefile 
  physical_operator/Makefile
  logical_operator/Makefile 
  Parsetree/Makefile 
  Parsetree/Test/Makefile 
  Resource/Makefile 
  storage/Makefile 
  Test/Makefile
  Test/common/Makefile
  Test/TestSuit/Makefile
  Test/utility/Makefile
  utility/Makefile
  codegen/Makefile 
])
AC_OUTPUT

