///*
// * resources.h
// *
// *  Created on: 2013-9-24
// *      Author: liyongfeng
// */
//
//#ifndef RESOURCES_H_
//#define RESOURCES_H_
//#include <boost/serialization/serialization.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//
//class Resources {//cdcdcd
//public:
//	Resources(){}
//	Resources(const unsigned int& cpus, const unsigned int& mem)
//	{
//		free_cpus = cpus;
//		max_cpus = cpus;
//		free_mem = mem;
//		max_mem = mem;
//	}
//	Resources(const Resources& that)
//	{
//		free_cpus = that.free_cpus;
//		max_cpus = that.max_cpus;
//		free_mem = that.free_mem;
//		max_mem = that.max_mem;
//	}
//	virtual ~Resources(){}
//
//	// Override some operators(+=, -=, ...)
//	Resources& operator += (const Resources& that)
//	{
//		(*this).free_cpus += that.free_cpus;
//		(*this).free_mem += that.free_mem;
//
//		return(*this);
//	}
//	Resources& operator -=(const Resources& that)
//	{
//		(*this).free_cpus -= that.free_cpus;
//		(*this).free_cpus -= that.free_mem;
//		return(*this);
//	}
//
//
//private:
//	unsigned int free_cpus;
//	unsigned int max_cpus;
//	unsigned int free_mem;
//	unsigned int max_mem;
//
//	// serialization
//	friend class boost::serialization::access;
//	template<class Archive>
//	void serialize(Archive & ar, const unsigned int version)
//	{
//		ar & free_cpus;
//		ar & max_cpus;
//		ar & free_mem;
//		ar & max_mem;
//	}
//};
//
//#endif /* RESOURCES_H_ */
