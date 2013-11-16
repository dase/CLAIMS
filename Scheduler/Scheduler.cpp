/*
 * Scheduler.cpp
 *
 *  Created on: 2013-9-26
 *      Author: casa
 */

#include "Scheduler.h"
#include "OperateNode.h"

void FIFOScheduler::init(){
	cout<<"init the fifoscheduler!"<<endl;
}

//TODO:I think the function in the follow is in the scheduler?
void FIFOScheduler::RewriteTree(OperateNode *subtree_root){
	if(subtree_root->getOperateName()=="scan"){
		cout<<"scan operator no need to cut"<<endl;
	}
	else if(subtree_root->getOperateName()=="filter"){
		cout<<"filter operator no need to cut"<<endl;
	}
	else if(subtree_root->getOperateName()=="broadcastjoin"){
		subtree_root->setCut();
		OperateNode *expand=new ExpandNode(subtree_root->getLeftchild(),0);
		OperateNode *exchange_upper=new ExchangeUpperNode(expand,0);
		subtree_root->setLeftchild(exchange_upper);
	}
	else{
		//TODO: other operator
		cout<<"waiting for other operator"<<endl;
	}
	if(subtree_root->hashleftchild())
		RewriteTree(subtree_root->getLeftchild());
	if(subtree_root->hasrightchild())
		RewriteTree(subtree_root->getRightchild());
}

void FIFOScheduler::displayTree(OperateNode *subtree_root){
	if(subtree_root->hashleftchild())
		displayTree(subtree_root->getLeftchild());
	if(subtree_root->hasrightchild())
		displayTree(subtree_root->getRightchild());
	cout<<"tree node: "<<subtree_root->getOperateName()<<endl;
	cout<<"parent is: ";
	if(subtree_root->getParent()!=0){
		cout<<subtree_root->getParent()->getOperateName()<<endl;
	}
	else{
		cout<<endl;
	}
	if(subtree_root->getLeftchild()!=0){
		cout<<"left child: "<<subtree_root->getLeftchild()->getOperateName()<<endl;
	}
	else{
		cout<<"left child: "<<endl;
	}
	if(subtree_root->getRightchild()!=0){
		cout<<"right child: "<<subtree_root->getRightchild()->getOperateName()<<endl;
	}
	else{
		cout<<"right child: "<<endl;
	}
	cout<<endl;
}

void FIFOScheduler::ConstructParent(OperateNode *subtree_root){
	//point to the parent
	if(subtree_root->hashleftchild())
		subtree_root->getLeftchild()->setParent(subtree_root);
	if(subtree_root->hasrightchild())
		subtree_root->getRightchild()->setParent(subtree_root);
	if(subtree_root->hashleftchild())
		ConstructParent(subtree_root->getLeftchild());
	if(subtree_root->hasrightchild())
		ConstructParent(subtree_root->getRightchild());
}

void FIFOScheduler::GenerateTree(OperateNode *tree_root){
	if(tree_root->hashleftchild())
		GenerateTree(tree_root->getLeftchild());
	if(tree_root->hasrightchild())
		GenerateTree(tree_root->getRightchild());
	if(tree_root->getOperateName()=="scan"){
		cout<<"building scan state"<<endl;
		//还有个参数是block_size，这个block_size暂时可以先认为确定
		//TODO: support ip generate by the more powerful catalog
		BlockStreamIteratorNode *temporary_scan=new BlockStreamIteratorNode();
		string ip_path=reinterpret_cast<ScanNode*>(tree_root)->getIppath().front().second;
		Schema *scan_schema=reinterpret_cast<ScanNode*>(tree_root)->getLeftInputSchema();
		ExpandableBlockStreamSingleColumnScan::State scan_state(ip_path,scan_schema,BLOCK_SIZE);
		ExpandableBlockStreamSingleColumnScan *ebssc=new ExpandableBlockStreamSingleColumnScan(scan_state);
		temporary_scan->name="scan";
		temporary_scan->bsib=ebssc;
		temporary_scan->next=current_;
		current_=temporary_scan;
	}
	else if(tree_root->getOperateName()=="filter"){
		cout<<"building filter state"<<endl;

		ExpandableBlockStreamFilter::State filter_state();
	}
	else if(tree_root->getOperateName()=="broadcastjoin"){
		cout<<"building hashjoin_broadcast state"<<endl;
		//需要知道有哪些属性将用于连接，以及在哪些节点上做join，以上就能够将属性信息得到
		//还有就是hashtale的大小，还有就是block的大小//rsourcemanager//memory
		//查看左右子树，就能够得到孩子的iterator了
		BlockStreamIteratorNode *temporary_join=new BlockStreamIteratorNode();
		Schema *join_left_schema=reinterpret_cast<BroadcastJoinNode*>(tree_root)->getLeftInputSchema();
		Schema *join_right_schema=reinterpret_cast<BroadcastJoinNode*>(tree_root)->getRightInputSchema();
		Schema *join_output_schema=reinterpret_cast<BroadcastJoinNode*>(tree_root)->getOutputSchema();
		vector<unsigned> left_index,right_index,left_payload,right_payload;
		left_index=reinterpret_cast<BroadcastJoinNode*>(tree_root)->getLeftIndex();
		right_index=reinterpret_cast<BroadcastJoinNode*>(tree_root)->getRightIndex();
		left_payload=reinterpret_cast<BroadcastJoinNode*>(tree_root)->getLeftPayload();
		right_payload=reinterpret_cast<BroadcastJoinNode*>(tree_root)->getRightPayload();
		OperateNode *left_child_of_join,*right_child_of_join;
		left_child_of_join=tree_root->getLeftchild();
		right_child_of_join=tree_root->getRightchild();
		BlockStreamIteratorNode *current_join=current_->next;
		BlockStreamIteratorNode *left=new BlockStreamIteratorNode();
		BlockStreamIteratorNode *right=new BlockStreamIteratorNode();
		while(current_join!=0){
			if(current_join->name==left_child_of_join->getOperateName()){
				left->bsib=current_join->bsib;
				left->name=current_join->name;
				left->next=current_join->next;
				break;
			}
			if(current_join->name==right_child_of_join->getOperateName()){
				right->bsib=current_join->bsib;
				right->name=current_join->name;
				right->next=current_join->next;
				continue;
			}
			current_join=current_join->next;
		}
		BlockStreamJoinIterator::State join_state;//(left->bsib,right->bsib,join_left_schema,join_right_schema,join_output_schema,left_index,right_index,left_payload,right_payload,N_BUCKETS,BUCKET_SIZE,BLOCK_SIZE);
		BlockStreamJoinIterator *bsji=new BlockStreamJoinIterator(join_state);
		temporary_join->name="broadcastjoin";
		temporary_join->bsib=bsji;
		temporary_join->next=current_;
		current_=temporary_join;
	}
	else if(tree_root->getOperateName()=="exchange"){
		cout<<"building exchange state"<<endl;
		//其实在exchange中，最重要的其实是upper和lower端的ip list
		//左孩子呢，就是沿着左子树的先序遍历的最后一个节点
		BlockStreamIteratorNode *temporary_exchange=new BlockStreamIteratorNode();
		vector<string> used_lowers,upper_ip_List;
		//TODO: find a method to value the exchange_id
		exchange_id++;
		OperateNode *lastPredecessor=findLastNode(tree_root);
		vector<pair<string,string> > ip_path_lower=lastPredecessor->getIppath();
		for(unsigned i=0;i<ip_path_lower.size();i++){
			used_lowers.push_back(ip_path_lower[i].first);
		}
//		cout<<"tree node: "<<lastPredecessor->getOperateName()<<endl;
		OperateNode *nextPredecessor=findLastNode(tree_root->getParent());
		vector<pair<string,string> > ip_path_upper=nextPredecessor->getIppath();
		for(unsigned j=0;j<ip_path_upper.size();j++){
			upper_ip_List.push_back(ip_path_upper[j].first);
		}
//		cout<<"tree node: "<<nextPredecessor->getOperateName()<<endl;
		BlockStreamIteratorNode *exchange_child=current_->next;
		Schema *exchange_schema=reinterpret_cast<ExchangeUpperNode*>(tree_root)->getLeftchild()->getOutputSchema();
		ExpandableBlockStreamBroadcastExchange::State exchange_state(exchange_schema,exchange_child->bsib,BLOCK_SIZE,used_lowers,upper_ip_List,exchange_id);
		ExpandableBlockStreamBroadcastExchange *ebsbe=new ExpandableBlockStreamBroadcastExchange(exchange_state);
		temporary_exchange->name="exchange";
		temporary_exchange->bsib=ebsbe;
		temporary_exchange->next=current_;
		current_=temporary_exchange;
	}
	else if(tree_root->getOperateName()=="expand"){
		cout<<"building expand state"<<endl;
		BlockStreamIteratorNode *temporary_expand=new BlockStreamIteratorNode();
		BlockStreamIteratorNode *expand_child=current_->next;
		Schema *expand_schema=reinterpret_cast<ExpandNode*>(tree_root)->getLeftchild()->getOutputSchema();
		BlockStreamExpander::State expand_state(expand_schema,expand_child->bsib,THREADS_COUNT,BLOCK_SIZE,EXPAND_BUFFER);
		BlockStreamExpander *bse=new BlockStreamExpander(expand_state);
		temporary_expand->name="expand";
		temporary_expand->bsib=bse;
		temporary_expand->next=current_;
		current_=temporary_expand;
	}
	else if(tree_root->getOperateName()=="exchangelower"){
		/*
		 * join所在的那棵子树的最低scan操作就是这个子树所在的节点的IP
		 * 在遍历的时候，join始终是在有数据的节点上做的
		 * 我们如果只是用hashjoin_broadcast的话，就可以
		 * */
		cout<<"---no need to send the ip list---"<<endl;
	}
	else{
		cout<<"something else!"<<endl;
	}
}

OperateNode *FIFOScheduler::findLastNode(OperateNode *subtree_root){
	OperateNode *lastNode=subtree_root;
	if(subtree_root->hasrightchild())
		findLastNode(subtree_root->getRightchild());
	else if(subtree_root->hashleftchild())
		findLastNode(subtree_root->getLeftchild());
	else
		return lastNode;
}
