///*
// * master.cpp
// *
// *  Created on: 2013-9-24
// *      Author: liyongfeng
// */
//
//#include "master.h"
//#include "scheduler.h"
//#include "rdtsc.h"
//#include "foreach.hpp"
//
//Master::Master(Scheduler* _scheduler)
//		      : scheduler(_scheduler)
//{}
//
//void Master::initizlize()
//{
//
//}
//void Master::registerSlave(const UPID& slavePID, const Resources& resources)
//{
//	// Check if this slave is already registered.
//	foreachvalue(Slave* slave, slaves)
//	{
//		if(slave->pid == slavePID)
//		{
//			cout << "Slave " << slavePID << " is already registered, resending acknowledge." << endl;
//			// Sending other message.
//			return;
//		}
//	}
//	// First registration.
//	double currentTime = getSecond(initTime);
//	Slave* slave = new Slave(slavePID, newSlaveID(), resources, currentTime);
//	addSlave(slave);
//}
//
//void Master::addSlave(Slave* slave, bool reregister)
//{
//	cout << "Adding slave " << slave->pid << endl;
//	slaves[slave->slaveid] = slave;
//}
//
//vector<Slave*> Master::getActiveSlaves() const
//{
//	vector<Slave*> result;
//	foreachvalue(Slave* slave, slaves)
//	{
//		if(slave->active)
//		{
//			result.push_back(slave);
//		}
//	}
//	return result;
//}
//
//
//
//hashmap<string, TableDescriptor*> Master::getRequiredTables(const vector<string>& tableNames) const
//{
//	hashmap<string, TableDescriptor*> result;
//	foreach(string& name, tableNames)
//	{
//		result[name] = tables[name];
//	}
//	return result;
//}
//
//// Actors
//Master::HeartBeatActor::HeartBeatActor(Theron::Framework& framework, const char *const name, Master* _master)
//					: Theron::Actor(framework, name),
//					  master(_master)
//{
//	RegisterHandler(this, &Master::HeartBeatActor::Handle);
//}
//
//void Master::HeartBeatActor::Handle(const Message4K &message, const Theron::Address from)
//{
//	cout << "HeartBeat has been received!" << endl;
//	// TODO:For indicating this slave is active, and can be send queries to do.
//	// Include SlaveID and current remaining memory.
//}
//
//Master::RegisterSlaveResourcesActor::RegisterSlaveResourcesActor(Theron::Framework& framework, const char *const name, Master* _master)
//					: Theron::Actor(framework, name),
//					  master(_master)
//{
//	RegisterHandler(this, &Master::RegisterSlaveResourcesActor::Handle);
//}
//
//void Master::RegisterSlaveResourcesActor::Handle(const Message4K& message, const Theron::Address from)
//{
//	cour << "RegisterSlaveResourcesMessage has been received!" << endl;
//	// Process message.
//}
