/*
 * NodeTracker.h
 *
 *  Created on: Nov 3, 2013
 *      Author: wangli
 *
 *  NodeTrakcer allocates a unique Id for each node.
 */

#ifndef NODETRACKER_H_
#define NODETRACKER_H_
#include <string>
#include "../hashmap.hpp"
typedef std::string NodeIP;//TODO: may use ip + port to support multiple instances on a single node.
typedef int NodeID;
class NodeTracker {
public:

	static NodeTracker* getInstance();
	virtual ~NodeTracker();
	int RegisterNode(std::string node_name);
	std::string getNodeIP(NodeID)const;
private:
	NodeTracker();
	hashmap<NodeIP,NodeID> ip_to_id_;
	unsigned allocate_cur_;
	static NodeTracker* instance_;
};

#endif /* NODETRACKER_H_ */
