///*
// * Expression.h
// *
// *  Created on: 2014-2-23
// *      Author: casa
// */
//
//#ifndef EXPRESSION_H_
//#define EXPRESSION_H_
//
//#include <vector>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
//
//class Expression {
//public:
//	Expression(std::vector<int> map);
//	virtual ~Expression();
//
//	void add_sys_func(int (*func)(int,int),int val){
//		this->func_=func;
//		this->val_=val;
//	}
//
//	/* func() is the function name and the arguments list
//	 * int (*)(int,int) is the return value which is a function point
//	 * */
//	int (*func())(int,int){
//		return func_;
//	}
//	int val_;
//private:
//	/*system func*/
////	void sys_add();
////	void sys_minus();
////	void sys_mul();
////	void sys_div();
//	int (*func_)(int,int);
//
//private:
//	std::vector<int> map_;
//private:
//	friend class boost::serialization::access;
//	template<class Archive>
//	void serialize(Archive & ar, const unsigned int version){
//		ar & boost::serialization::base_object<Expression>(*this) & map_;
//	}
//};
//
//#endif /* EXPRESSION_H_ */
