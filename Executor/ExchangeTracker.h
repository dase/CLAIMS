/*
 * ExchangeTracker.h
 * ExchangeTracker is the coordinator between the exchanges and
 * their lower-exchangers. The exchanges register their socket
 * port in the open function, then the exchange-lowers ask their
 * upper ExchangeTrackers for the ports so that the socket
 * connection can be done automatically rather than setting the
 * ports manually.
 *  Created on: Aug 11, 2013
 *      Author: wangli
 */

#ifndef EXCHANGETRACKER_H_
#define EXCHANGETRACKER_H_
#include <map>
#include <boost/unordered_map.hpp>
#include <Theron/Theron.h>
#include "../common/Logging.h"
#include "../utility/lock.h"
#include "../common/ids.h"
class ExchangeTracker {
public:
	ExchangeTracker();
	virtual ~ExchangeTracker();
	bool RegisterExchange(ExchangeID exchange_id, std::string port);
	void LogoutExchange(const ExchangeID &exchange_id);
	int AskForSocketConnectionInfo(ExchangeID exchange_id,std::string target_ip);
	void printAllExchangeId()const;
private:
	Theron::EndPoint* endpoint;
	Theron::Framework* framework;
	Theron::Actor* actor;
	boost::unordered_map<ExchangeID,std::string> id_to_port;
	Logging* logging_;
	Lock lock_;


	/////////////////////////////////////////////////////////////
	/**
	 * RegisterActor
	 */
	friend class RegisterActor;
	class ExchangeTrackerActor:public Theron::Actor{
	public:
		ExchangeTrackerActor(ExchangeTracker* et,Theron::Framework* framework, const char* Name);
	private:
		void AskForConnectionInfo(const ExchangeID &exchange_id, const Theron::Address from);

	private:
		ExchangeTracker* et;


	};
};

#endif /* EXCHANGETRACKER_H_ */
