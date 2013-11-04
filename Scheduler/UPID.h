///*
// * UPID.h
// *
// *  Created on: 2013-8-17
// *      Author: imdb
// */
//
//#ifndef UPID_H_
//#define UPID_H_
//
//#include <boost/serialization/serialization.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <iostream>
//using namespace std;
//struct UPID
//{
//	UPID(){}
//	UPID(const UPID& that)
//        : ip(that.ip),
//          port(that.port) {}
//	UPID(string ip_, string port_)
//        : ip(ip_),
//          port(port_) {}
//
//  /*If you want use UPID as key of map data structure,
//   * you should implement the method in this way,
//   * namely override operator < .
//   */
//	friend bool operator < (const struct UPID& lpid, const struct UPID& rpid);
//
//	bool operator == (const UPID& that) const
//    {
//		if (this != &that)
////		{
//			return (ip == that.ip && port == that.port);
//		}
//
//		return true;
//    }
//
//	bool operator != (const UPID& that) const
//	{
//		return !(this->operator == (that));
//	}
//
//	friend inline ostream& operator << (ostream& out, const UPID& that)
//	{
//		out<<that.ip<<": "<<that.port;
//		return out;
//	}
//	void print()
//	{
//		cout<<ip<<" "<<port<<endl;
//	}
//
//	string ip;
//	string port;
//
//private://serialization
//	friend class boost::serialization::access;
//	template<class Archive>
//	void serialize(Archive & ar, const unsigned int version)
//	{
//		ar & ip;
//		ar & port;
//	}
//};
//
//inline bool operator < (const struct UPID& lpid, const struct UPID& rpid)
//{
//	return (lpid.ip < rpid.ip || (lpid.ip == rpid.ip && lpid.port < rpid.port));
//}
//
//#endif /* UPID_H_ */
