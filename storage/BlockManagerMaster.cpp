/*
 * BlockManagerMaster.cpp
 *
 *  Created on: 2013-10-15
 *      Author: casa
 */

#include <sstream>

#include "BlockManagerMaster.h"
#include "../Environment.h"
#include "../common/Message.h"
#include "../utility/print_tool.h"

#include "caf/io/all.hpp"
#include "caf/all.hpp"
#include "../node_manager/base_node.h"
using caf::after;
using caf::io::remote_actor;
using claims::BindingAtom;
using claims::OkAtom;
using claims::UnBindingAtom;
BlockManagerMaster *BlockManagerMaster::master_ = 0;

BlockManagerMaster::BlockManagerMaster() { master_ = this; }

BlockManagerMaster::~BlockManagerMaster() { master_ = 0; }

void BlockManagerMaster::initialize() { abi_ = AllBlockInfo::getInstance(); }

/*
 * send message to specified node to set chunk number in partition
 * whose id is partition_id.
 *
 * this function is used in two way:
 *    1) bind a projection to a node:
 *      add new partition info like chunk number in the node
 *      chunk number is not changed
 *    2) update the chunk number in a node: chunk number changed
 */
bool BlockManagerMaster::SendBindingMessage(
    const PartitionID &partition_id, const unsigned &number_of_chunks,
    const StorageLevel &desirable_storage_level, const NodeID &target) const {
  caf::scoped_actor self;
  auto target_node_addr =
      Environment::getInstance()->get_master_node()->GetNodeAddrFromId(target);
  try {
    auto target_actor =
        remote_actor(target_node_addr.first.c_str(), target_node_addr.second);
    self->sync_send(target_actor, BindingAtom::value, partition_id,
                    number_of_chunks, desirable_storage_level)
        .await(

            [=](OkAtom) {
              LOG(INFO) << "sending binding message is OK!!" << endl;
            },
            after(std::chrono::seconds(30)) >>
                [=]() {
                  LOG(WARNING) << "sending binding message, but timeout 30s!!"
                               << endl;
                  return false;
                }

            );
  } catch (caf::network_error &e) {
    LOG(WARNING) << "cann't connect to node ( " << target_node_addr.first
                 << " , " << target_node_addr.second << " )";
    return false;
  }
  return true;
}

/*
 * As opposed to SendBindingMessage, except this method isn't used in updating
 * chunk number
 */
bool BlockManagerMaster::SendUnbindingMessage(const PartitionID &partition_id,
                                              NodeID &target) const {
  caf::scoped_actor self;
  auto target_node_addr =
      Environment::getInstance()->get_master_node()->GetNodeAddrFromId(target);
  auto target_actor =
      remote_actor(target_node_addr.first.c_str(), target_node_addr.second);
  self->sync_send(target_actor, UnBindingAtom::value, partition_id).await(
      [=](OkAtom) { LOG(INFO) << "sending unbinding message is OK!!" << endl; },
      after(std::chrono::seconds(30)) >>
          [=]() {
            LOG(WARNING) << "sending unbinding message, but timeout 30s!!"
                         << endl;
            return false;
          }

      );
  return true;
}
