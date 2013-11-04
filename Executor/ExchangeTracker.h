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
#include "../AdaptiveEndPoint.h"
#include "../Logging.h"
#include "../Message.h"
class ExchangeTracker {
public:
	ExchangeTracker();
	virtual ~ExchangeTracker();
	bool RegisterExchange(unsigned long long int exchange_id, std::string port);
	void LogoutExchange(const unsigned long long int &exchange_id);
	int AskForSocketConnectionInfo(unsigned long long int exchange_id,std::string target_ip);
private:
	Theron::EndPoint* endpoint;
	Theron::Framework* framework;
	Theron::Actor* actor;
	std::map<unsigned long long int,std::string> id_to_port;
	Logging* logging_;


	/////////////////////////////////////////////////////////////
	/**
	 * RegisterActor
	 */
	friend class RegisterActor;
	class ExchangeTrackerActor:public Theron::Actor{
	public:
		ExchangeTrackerActor(ExchangeTracker* et,Theron::Framework* framework, const char* Name);
	private:
		void AskForConnectionInfo(const unsigned long long int &exchange_id, const Theron::Address from);

	private:
		ExchangeTracker* et;


	};
};

#endif /* EXCHANGETRACKER_H_ */
