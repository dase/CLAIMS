AM_CPPFLAGS=-fPIC -fpermissive -DTHERON_XS\
-I${HADOOP_HOME}/src/c++/libhdfs\
-I${JAVA_HOME}/include\
-I${JAVA_HOME}/include/linux \
-I${THERON_HOME}/Include \
-I${BOOST_HOME}/boost/serialization \
-I${BOOST_HOME}/ \
-I${GTEST_HOME}/include

#-L/usr/local/lib \
#-I/usr/local/include
AM_LDFLAGS= -lc -lm -lrt -lpthread -lboost_serialization -lboost_date_time -lboost_system -lconfig++ -lxs -lnuma -lreadline -lhistory -lz -ltinfo -ldl -rdynamic

LDADD =  \
	Test/libtest.a \
	Test/common/libcommon.a \
	Test/TestSuit/libtestsuit.a \
	Test/utility/libutility.a \
	BlockStreamIterator/Serialization/libserialization.a \
	Client/libclient.a \
	Client/Test/libtest.a \
	Daemon/libdaemon.a \
	Daemon/Test/libtest.a \
	Executor/libexecutor.a \
	Executor/Test/libtest.a \
	IndexManager/libindexmanager.a \
	IndexManager/Test/libtest.a \
	Parsetree/libparsetree.a \
	Parsetree/Test/libtest.a \
	LogicalQueryPlan/Test/libtest.a \
	LogicalQueryPlan/liblogicalqueryplan.a \
	storage/libstorage.a \
	BlockStreamIterator/ParallelBlockStreamIterator/Test/libtest.a \
	BlockStreamIterator/ParallelBlockStreamIterator/libparallelblockstreamiterator.a \
	BlockStreamIterator/libblockstreamiterator.a \
	Resource/libresouce.a \
	Catalog/libcatalog.a \
	Catalog/Test/libtest.a \
	Catalog/stat/libstat.a \
	Loader/libloader.a \
	Loader/Test/libtest.a \
	common/libcommon.a \
	common/Block/libblock.a \
	common/Expression/libexpression.a \
	common/Schema/libschema.a \
	common/Schema/Test/libtest.a \
	common/types/libtypes.a \
	common/types/Test/libtest.a \
	common/types/ttmath/libttmath.a \
	utility/libutility.a \
      codegen/libcodegen.a\
	${BOOST_HOME}/stage/lib/libboost_serialization.a \
	${BOOST_HOME}/stage/lib/libboost_serialization.so \
	${BOOST_HOME}/stage/lib/libboost_system.a \
	${BOOST_HOME}/stage/lib/libboost_system.so \
	${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.so \
	${HADOOP_HOME}/c++/Linux-amd64-64/lib/libhdfs.a \
	${JAVA_HOME}/jre/lib/amd64/server/libjvm.so\
	${THERON_HOME}/Lib/libtherond.a \
	${GTEST_HOME}/libgtest.a \
	-lLLVMJIT -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMMCParser -lLLVMCodeGen -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMX86Desc -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMTarget -lLLVMRuntimeDyld -lLLVMExecutionEngine -lLLVMMC -lLLVMObject -lLLVMBitReader -lLLVMCore -lLLVMSupport -lLLVMMCParser



 
include_HEADERS = Config.h \
				Debug.h \
				Environment.h \
				IDsGenerator.h \
				configure.h
				
bin_PROGRAMS =  claimsserver \
				client \
				test
				
client_SOURCES = Client.cpp \
					Environment.cpp \
					Config.cpp \
					IDsGenerator.cpp 

claimsserver_SOURCES = Config.cpp	\
				Environment.cpp \
				IDsGenerator.cpp \
				Server.cpp

test_SOURCES = Test/gtest_main.cpp\
				Environment.cpp \
				IDsGenerator.cpp \
				Config.cpp
				


SUBDIRS= BlockStreamIterator Catalog Client common Daemon Executor IndexManager\
Loader LogicalQueryPlan Parsetree Resource \
storage Test utility codegen

DIST_SUBDIRS = BlockStreamIterator Catalog Client common Daemon Executor IndexManager\
Loader LogicalQueryPlan Parsetree Resource \
storage Test utility codegen
