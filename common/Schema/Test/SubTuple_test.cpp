/*
 * SubTuple_test.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: wangli
 */
#include "../TupleConvertor.h"
#include "../SchemaFix.h"
#pragma pack(4)
struct Tuple{
	 int32_t a1;
	 char name[14];
	 uint64_t a2;
	 int32_t a3;
};
#pragma pack()
static int sub_tuple_test(){
	printf("Hello li\n");
	column_type a(t_int);
	column_type b(t_u_long);
	column_type c(t_int);
	column_type d(t_string,14);
	std::vector<column_type> columns;
	columns.push_back(a);
	columns.push_back(d);
	columns.push_back(b);
	columns.push_back(c);
	Schema* schema=new SchemaFix(columns);

	Tuple t;
	t.a1=10;
	t.a2=11;
	t.a3=12;
	strcpy(t.name,"wangli");
//	t.name="wangli";
	printf("struct size =%d\n",sizeof(t));
	printf("schema size=%d\n",schema->getTupleMaxSize());
	std::vector<unsigned> index;
	index.push_back(1);
	index.push_back(3);
	Schema* schema_new=schema->getSubSchema(index);

	SubTuple st(schema,schema_new,index);

	void* new_tuple=malloc(schema_new->getTupleMaxSize());		//newmalloc
	void* orignal_tuple=&t;
	st.getSubTuple(orignal_tuple,new_tuple);
	schema->displayTuple(orignal_tuple);
	schema_new->displayTuple(new_tuple);

}

