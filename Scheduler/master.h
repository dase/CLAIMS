///*
// * master.h
// *
// *  Created on: 2013-9-24
// *      Author: liyongfeng
// */
//
//#ifndef MASTER_H_
//#define MASTER_H_
//
//#include "hashmap.hpp"
//#include "foreach.hpp"
//#include "../Block/synch.h"
//#include "UPID.h"
//#include "table.h"
//#include "resources.h"
//#include "../Message.h"
//
//typedef size_t SlaveID;
//typedef size_t TableID;
//
//// Some forward declarations
//class Scheduler;
//struct Slave;
//
//
//class Master {
//public:
//	Master(Scheduler* _scheduler);
//	virtual ~Master(){}
//	void initizlize();
//    // Actors
//
//	class HeartBeatActor : public Theron::Actor
//	{
//	public:
//		HeartBeatActor(Theron::Framework& framework, const char *const name, Master* _master);
//	private:
//		void Handle(const Message4K &message, const Theron::Address from);
//		Master* master;
//	};
//
//	class RegisterSlaveResourcesActor : public Theron::Actor
//	{
//	public:
//		RegisterSlaveResourcesActor(Theron::Framework& framework, const char *const name, Master* _master);
//	private:
//		void Handle(const Message4K &message, const Theron::Address from);
//		Master* master;
//	};
//
//	class CompleteIteratorTreeActor : public Theron::Actor
//	{
//	public:
//		CompleteIteratorTreeActor(Theron::Framework& framework, const char *const name, Master* _master);
//	private:
//		void Handle(const Message4K &message, const Theron::Address from);
//		Master* master;
//	};
//
//	class ReceiveQueryActor : public Theron::Actor
//	{
//	public:
//		ReceiveQueryActor(Theron::Framework& framework, const char *const name, Master* _master);
//	private:
//		void Handle(const Message4K &message, const Theron::Address from);
//		Master* master;
//	};
//
//	//Some functions about Resources.
//	void registerSlave(const UPID& slavePID, const Resources& resources);
//	void addSlave(Slave* slave, bool reregister = false);
//
//	// Some functions about Catalog.
//	void addTable();
//	void deleteTable(const string& tableName);
//	bool isTableExisted(const string& tableName);
//	inline hashmap<string, TableDescriptor*> getActiveTables() const {return tables;}
//	hashmap<string, TableDescriptor*> getRequiredTables(const vector<string>& tableNames) const;
//	vector<Slave*> getActiveSlaves() const;
//	// Some functions about Query.
//
//	// Other functions
//
//
//private:
//	Scheduler* scheduler;
//
//	hashmap<SlaveID, Slave*> slaves;
//	hashmap<string, TableDescriptor*> tables;
//
//	// For synchronization.
//	Lock lock_table;
//	Lock lock_query;
//
//	// vector<QueryDag*> queries;
//
//	unsigned long long initTime; // Start time or Bench-Time.
//
//};
//struct Slave
//{
//	Slave(const UPID& _pid,
//		  const SlaveID& id,
//		  const Resources& resources,
//		  const double& time)
//	      : pid(_pid),
//	        slaveid(id),
//		    totalResources(resources),
//		    registerTime(time)
//	{
//		 lastHeartBeat = time;
//	     active = true;
//	}
//
//	UPID pid;
//	SlaveID slaveid;
//	Resources totalResources;
//	Resources usedResources;
//	double registerTime;
//	double lastHeartBeat;
//	bool active; // Turn false when slave is removed.
//
//	//TODO: Queries running on this slave.
//};
//#endif /* MASTER_H_ */
