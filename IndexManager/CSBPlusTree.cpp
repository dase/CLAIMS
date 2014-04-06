/*
 * CSBPlusTree.cpp
 *
 *  Created on: 2013年12月27日
 *      Author: SCDONG
 */

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <memory.h>
#include "CSBPlusTree.h"
using namespace std;

template struct data_offset<int>;
template class CCSBNode<int>;
template class CCSBInternalNode<int>;
template class CCSBLeafNode<int>;
template class CCSBNodeGroup<int>;
template class CCSBInternalNodeGroup<int>;
template class CCSBLeafNodeGroup<int>;
template class CSBPlusTree<int>;

void cheak_com(){
	data_offset<int>();
	CCSBNode<int>();
	CCSBInternalNode<int>();
	CCSBLeafNode<int>();
	CCSBNodeGroup<int>();
	CCSBInternalNodeGroup<int>();
	CCSBInternalNodeGroup<int>* internal = new CCSBInternalNodeGroup<int> (1);
	CCSBLeafNodeGroup<int>();
	CCSBLeafNodeGroup<int>* leaf = new CCSBLeafNodeGroup<int> (1);
	data_offset<int>* data = NULL;
	CSBPlusTree<int>* csb_int = new CSBPlusTree<int>();
	csb_int->BulkLoad(data, 0);
	csb_int->printDoubleLinkedList();
	csb_int->printTree();
	csb_int->Search(0);
	csb_int->rangeQuery(0, 0);
	csb_int->rangeQuery(0, FilterIterator::AttributeComparator::EQ, 0, FilterIterator::AttributeComparator::EQ);
	csb_int->Insert(*data);
	csb_int->serialize(0);
	csb_int->deserialize(0);

	CSBPlusTree<short>* csb_short = new CSBPlusTree<short>();
	csb_short->rangeQuery(0, 0);
	CSBPlusTree<unsigned long>* csb_u_long = new CSBPlusTree<unsigned long>();
	csb_u_long->rangeQuery(0, 0);
	CSBPlusTree<float>* csb_float = new CSBPlusTree<float>();
	csb_float->rangeQuery(0, 0);
}


/***********************************  CSBNode  ***********************************/
template <typename T>
CCSBNode<T>::CCSBNode():used_keys(0), p_father(NULL) { }

template <typename T>
CCSBNode<T>::~CCSBNode()
{
	DeleteChildren();
}


/***********************************  CSBInternalNode  ***********************************/
template <typename T>
CCSBInternalNode<T>::CCSBInternalNode()
{
	CCSBNode<T>(NODE_TYPE_INTERNAL);
	for (unsigned i = 0; i < CSB_MAXNUM_KEY; i++)
		node_keys[i] = INVALID;
	p_child_node_group = NULL;
}

template <typename T>
CCSBInternalNode<T>::~CCSBInternalNode()
{
	DeleteChildren();
}

template <typename T>
bool CCSBInternalNode<T>::Insert(T key)
{
	//return false when the node is full
	if (this->getUsedKeys() >= CSB_MAXNUM_KEY)
	{
		return false;
	}

	//determine the position for insert
	int pos = 0;
	for (pos = 0; pos < this->getUsedKeys() && key >= node_keys[pos]; pos++);
	for (int j = this->getUsedKeys(); j > pos; j--)
	{
		node_keys[j] = node_keys[j-1];
	}
	node_keys[pos] = key;
	this->setUsedKeys(this->getUsedKeys()+1);

	return true;
}

template <typename T>
bool CCSBInternalNode<T>::Delete(T value)
{
return false;
}

template <typename T>
T CCSBInternalNode<T>::SplitInsert(CCSBNode<T>* pNode, T key)
{
	if (this->getUsedKeys() != CSB_MAXNUM_KEY)
	{
		cout << "[ERROR: CSBPlusTree.cpp (CCSBInternalNode->SplitInsert)] The unfull internal node can not be splited!\n";
		exit(-1);
	}

	if (key < this->node_keys[CSB_MAXNUM_KEY/2])	//insert into the first node
	{
		for (unsigned i = CSB_MAXNUM_KEY/2; i < CSB_MAXNUM_KEY; i++)
		{
			pNode->setElement(i-CSB_MAXNUM_KEY/2, this->node_keys[i]);
			this->node_keys[i] = INVALID;
		}
		pNode->setUsedKeys(CSB_MAXNUM_KEY-CSB_MAXNUM_KEY/2);
		this->setUsedKeys(CSB_MAXNUM_KEY/2);
		this->Insert(key);
		return pNode->getElement(0)._key;
	}
	else	//insert into pNode
	{
		unsigned tmp = CSB_MAXNUM_KEY-CSB_MAXNUM_KEY/2;
		for (unsigned i = tmp; i < CSB_MAXNUM_KEY; i++)
		{
			pNode->setElement(i-tmp, this->node_keys[i]);
			this->node_keys[i] = INVALID;
		}
		pNode->setUsedKeys(CSB_MAXNUM_KEY-tmp);
		this->setUsedKeys(tmp);
		pNode->Insert(key);
		return pNode->getElement(0)._key;
	}
}

template <typename T>
bool CCSBInternalNode<T>::Combine(CCSBNode<T>* pNode)
{
return false;
}

template <typename T>
void CCSBInternalNode<T>::DeleteChildren()
{

}

template <typename T>
bool CCSBInternalNode<T>::serialize(FILE* filename)
{
	fwrite((void*)(&this->used_keys), sizeof(int), 1, filename);
	fwrite(this->node_keys, sizeof(T), this->used_keys, filename);
	return true;
}

template <typename T>
bool CCSBInternalNode<T>::deserialize(FILE* filename)
{
	fread((void*)(&this->used_keys), sizeof(int), 1, filename);
	fread(this->node_keys, sizeof(T), this->used_keys, filename);
	return true;
}

/***********************************  CSBLeafNode  ***********************************/
template <typename T>
CCSBLeafNode<T>::CCSBLeafNode()
{
	CCSBNode<T>(NODE_TYPE_LEAF);
	data_offset<T> tmp;
	tmp._key = INVALID;
	tmp._block_off = INVALID;
	tmp._tuple_off = INVALID;
	for (unsigned i = 0; i < CSB_MAXNUM_DATA; i++)
		node_datas[i] = tmp;
}

template <typename T>
CCSBLeafNode<T>::~CCSBLeafNode()
{
	DeleteChildren();
}

template <typename T>
bool CCSBLeafNode<T>::Insert(data_offset<T> value)
{
	//return false if the leaf node is full
	if (this->getUsedKeys() >= CSB_MAXNUM_DATA)
		return false;

	//determine the position for insert(for the equals keys insert at the last)
	int pos = 0;
	for (pos = 0; pos < this->getUsedKeys() && value._key >= (node_datas[pos])._key; pos++);
	for (int j = this->getUsedKeys(); j > pos; j--)
		node_datas[j] = node_datas[j-1];
	node_datas[pos] = value;
	this->setUsedKeys(this->getUsedKeys()+1);

	return true;
}

template <typename T>
bool CCSBLeafNode<T>::Delete(T value)
{
return false;
}

template <typename T>
T CCSBLeafNode<T>::SplitInsert(CCSBNode<T>* pNode, data_offset<T> data)
{
	if (this->getUsedKeys() != CSB_MAXNUM_DATA)
	{
		cout << "[ERROR: CSBPlusTree.cpp (CCSBLeafNode->SplitInsert)] The unfull leaf node can not be splited!\n";
		exit(-1);
	}

	if (data._key < this->node_datas[CSB_MAXNUM_DATA/2]._key)	//insert into the first node
	{
		for (unsigned i = CSB_MAXNUM_DATA/2; i < CSB_MAXNUM_DATA; i++)
		{
			pNode->setElement(i-CSB_MAXNUM_DATA/2, this->node_datas[i]);
			this->node_datas[i]._key = INVALID;
			this->node_datas[i]._block_off = INVALID;
			this->node_datas[i]._tuple_off = INVALID;
		}
		pNode->setUsedKeys(CSB_MAXNUM_DATA-CSB_MAXNUM_DATA/2);
		this->setUsedKeys(CSB_MAXNUM_DATA/2);
		this->Insert(data);
		return pNode->getElement(0)._key;
	}
	else	//insert into pNode
	{
		unsigned tmp = CSB_MAXNUM_DATA-CSB_MAXNUM_DATA/2;
		for (unsigned i = tmp; i < CSB_MAXNUM_DATA; i++)
		{
			pNode->setElement(i-tmp, this->node_datas[i]);
			this->node_datas[i]._key = INVALID;
			this->node_datas[i]._block_off = INVALID;
			this->node_datas[i]._tuple_off = INVALID;
		}
		pNode->setUsedKeys(CSB_MAXNUM_DATA-tmp);
		this->setUsedKeys(tmp);
		pNode->Insert(data);
		return pNode->getElement(0)._key;
	}
}

template <typename T>
bool CCSBLeafNode<T>::Combine(CCSBNode<T>* pNode)
{
return false;
}

template <typename T>
void CCSBLeafNode<T>::DeleteChildren()
{

}

template <typename T>
bool CCSBLeafNode<T>::serialize(FILE* filename)
{
	fwrite((void*)(&this->used_keys), sizeof(int), 1, filename);
	fwrite(this->node_datas, sizeof(data_offset<T>), this->used_keys, filename);
	return true;
}

template <typename T>
bool CCSBLeafNode<T>::deserialize(FILE* filename)
{
	fread((void*)(&this->used_keys), sizeof(int), 1, filename);
	fread(this->node_datas, sizeof(data_offset<T>), this->used_keys, filename);
	return true;
}

/***********************************  NodeGroup  ***********************************/


/***********************************  InternalNodeGroup  ***********************************/
template <typename T>
CCSBInternalNodeGroup<T>::CCSBInternalNodeGroup(unsigned n)
{
	internal_nodes = (CCSBNode<T>**) new CCSBInternalNode<T>* [n];
	for (unsigned i = 0; i < n; i++)
		internal_nodes[i] = new CCSBInternalNode<T> ();
	this->setUsedNodes(n);
}

template <typename T>
CCSBInternalNodeGroup<T>::~CCSBInternalNodeGroup()
{

}

template <typename T>
bool CCSBInternalNodeGroup<T>::serialize(FILE* filename)
{
	fwrite((void*)(&this->used_nodes), sizeof(unsigned), 1, filename);
	for (unsigned i = 0; i < this->used_nodes; i++)
		internal_nodes[i]->serialize(filename);
	return true;
}

template <typename T>
bool CCSBInternalNodeGroup<T>::deserialize(FILE* filename)
{
	fread((void*)(&this->used_nodes), sizeof(unsigned), 1, filename);
	internal_nodes = (CCSBNode<T>**) new CCSBInternalNode<T>* [this->used_nodes];
	for (unsigned i = 0; i < this->used_nodes; i++)
	{
		internal_nodes[i] = new CCSBInternalNode<T> ();
		internal_nodes[i]->deserialize(filename);
	}
	return true;
}

/***********************************  LeafNodeGroup  ***********************************/
template <typename T>
CCSBLeafNodeGroup<T>::CCSBLeafNodeGroup(unsigned n)
{
	leaf_nodes = (CCSBNode<T>**) new CCSBLeafNode<T>* [n];
	for (unsigned i = 0; i < n; i++)
		leaf_nodes[i] = new CCSBLeafNode<T> ();
	this->setUsedNodes(n);
	p_header = NULL;
	p_tailer = NULL;
}

template <typename T>
CCSBLeafNodeGroup<T>::~CCSBLeafNodeGroup()
{

}

template <typename T>
bool CCSBLeafNodeGroup<T>::serialize(FILE* filename)
{
	fwrite((void*)(&this->used_nodes), sizeof(unsigned), 1, filename);
	for (unsigned i = 0; i < this->used_nodes; i++)
		leaf_nodes[i]->serialize(filename);
	return true;
}

template <typename T>
bool CCSBLeafNodeGroup<T>::deserialize(FILE* filename)
{
	fread((void*)(&this->used_nodes), sizeof(unsigned), 1, filename);
	leaf_nodes = (CCSBNode<T>**) new CCSBLeafNode<T>* [this->used_nodes];
	for (unsigned i = 0; i < this->used_nodes; i++)
	{
		leaf_nodes[i] = new CCSBLeafNode<T> ();
		leaf_nodes[i]->deserialize(filename);
	}
	return true;
}

/***********************************  CSBPlusTree  ***********************************/
template <typename T>
CSBPlusTree<T>::CSBPlusTree() :csb_root(NULL), csb_depth(0), leaf_header(NULL), leaf_tailer(NULL) {
	// TODO Auto-generated constructor stub
}

template <typename T>
CSBPlusTree<T>::~CSBPlusTree() {
	// TODO Auto-generated destructor stub
	ClearTree();
}

//bulkload the CSB+-Tree index
template <typename T>
void CSBPlusTree<T>::BulkLoad(data_offset<T>* aray, unsigned aray_num)
{
	if (aray_num == 0)
		return;
	//just one CSBNode
	else if (aray_num <= CSB_MAXNUM_DATA)
	{
		this->csb_depth = 1;
		CCSBNodeGroup<T>* leaf = (CCSBNodeGroup<T>*) new CCSBLeafNodeGroup<T>(1);

		for (unsigned i = 0; i < aray_num; i++)
		{
			leaf->getNode(0)->setElement(i, aray[i]);
		}
		leaf->getNode(0)->setUsedKeys(aray_num);
		leaf->getNode(0)->setFather(NULL);

		//set the root and the leaf doubly linked list
		this->csb_root = leaf->getNode(0);
		leaf->setHeaderNG(NULL);
		leaf->setTailerNG(NULL);
		this->leaf_header = leaf;
		this->leaf_tailer = leaf;
////For testing begin: print the only node
//		cout << "\n\n---------------------Leaf Layer---------------------\n";
//		cout << "Just one node. Used keys: " << ((CCSBNode<T>*)(leaf->getNode(0)))->getUsedKeys() << endl;
//		for (unsigned i = 0;i < ((CCSBNode<T>*)(leaf->getNode(0)))->getUsedKeys(); i++)
//		{
//			cout << "Key: " << ((CCSBNode<T>*)leaf->getNode(0))->getElement(i)._key << " ";
//			cout << "Block offset: " << ((CCSBNode<T>*)leaf->getNode(0))->getElement(i)._block_off << " ";
//			cout << "Tuple offset: " << ((CCSBNode<T>*)leaf->getNode(0))->getElement(i)._tuple_off << endl;
//		}
////For testing end

	}
	//more than one layer: bottom up construct
	else
	{
		//allocate the leaf_node_group
		int leaf_node_group_num = ceil((double)(ceil((double)(aray_num)/(double)(CSB_MAXNUM_DATA)))/(double)(CSB_MAXNUM_KEY+1));
		CCSBNodeGroup<T>** leaf = (CCSBNodeGroup<T>**) new CCSBLeafNodeGroup<T>* [leaf_node_group_num];

		//allocate the parent_key_array of the leaf_node_group
		int internal_key_array_num = ceil((double)aray_num/(double)CSB_MAXNUM_DATA)-1;
		T* internal_key_array = new T [internal_key_array_num];
		//建立最底层叶子结点，并返回其索引internalAray
		internal_key_array_num = makeLeafNodeGroup(aray, aray_num,leaf,internal_key_array);


////For testing begin: print the leaf nodes
//		cout << "\n\n---------------------Leaf Layer (depth: " << this->csb_depth << ")---------------------\n";
//		for (int ti = 0; ti < leaf_node_group_num; ti++)
//		{
//			cout << "NodeGroup: " << ti << "\t";
//			cout << "Used nodes: " << leaf[ti]->used_nodes << endl;
//			for (int i = 0; i < leaf[ti]->used_nodes; i++)
//			{
//				cout << "Node: " << i << "\t";
//				cout << "used keys: " << leaf[ti]->leaf_nodes[i]->used_keys << endl;
//				for (int k = 0; k < leaf[ti]->leaf_nodes[i]->used_keys; k++)
//				{
//					cout << "Key: " << ((CCSBNode<T>*)leaf[ti]->getNode(i))->getElement(k)._key << " ";
//					cout << "Block offset: " << ((CCSBNode<T>*)leaf[ti]->getNode(i))->getElement(k)._block_off << " ";
//					cout << "Tuple offset: " << ((CCSBNode<T>*)leaf[ti]->getNode(i))->getElement(k)._tuple_off << endl;
//				}
//				cout << endl;
//			}
//		}
//		cout << "Upper layer index keys: \n";
//		for (int i = 0; i < internal_key_array_num; i++)
//			cout << internal_key_array[i] << " ";
//		cout << endl;
////For tesing end


		//used the internal_key_array to build the internal indexing node;
		//the indexing array is in internal_key_array, internal_key_array_num stands for the number of keys in it
		//set the p_father and p_child_node_group between index nodes
		if (internal_key_array_num <= CSB_MAXNUM_KEY)
		{
			this->csb_depth += 1;
			CCSBNodeGroup<T>* internal = (CCSBNodeGroup<T>*)new CCSBInternalNodeGroup<T>(1);

			for (unsigned i = 0; i < internal_key_array_num; i++)
			{
				internal->getNode(0)->setElement(i, internal_key_array[i]);
			}
			((CCSBNode<T>*)(internal->getNode(0)))->setUsedKeys(internal_key_array_num);
			((CCSBNode<T>*)(internal->getNode(0)))->setFather(NULL);

			//set the parent-child relationship between internal and leaf
			((CCSBNode<T>*)(internal->getNode(0)))->setPointer((CCSBNodeGroup<T>*)(*leaf));
			for (unsigned i = 0; i < leaf[0]->used_nodes; i++)
				((CCSBNode<T>*)(leaf[0]->getNode(i)))->setFather(internal->getNode(0));

			this->csb_root = internal->getNode(0);


////For testing begin: print the root node
//			cout << "\n\n---------------------Root Node (depth: " << this->csb_depth << ")---------------------\n";
//			cout << "Root: " << "\t";
//			cout << "used keys: " << internal->internal_nodes[0]->used_keys << endl;
//			for (int k = 0; k < internal->internal_nodes[0]->used_keys; k++)
//				cout << ((CCSBNode<T>*)(internal->getNode(0)))->getElement(k)._key << " ";
//			cout << endl;
////For testing end

		}
		else
		{
			//allocate upper node_group (equals to the lower_node_groups/branches)
			int internal_node_group_num = ceil((double)leaf_node_group_num/(double)(CSB_MAXNUM_KEY+1));
			CCSBNodeGroup<T>** internal = (CCSBNodeGroup<T>**)new CCSBInternalNodeGroup<T>* [internal_node_group_num];

			//allocate the key_array for the upper internal_key_array
			T* upper_internal_key_array = new T [leaf_node_group_num-1];
			int upper_internal_key_array_num = makeInternalNodeGroup(internal_key_array, internal_key_array_num, internal, leaf_node_group_num, upper_internal_key_array, leaf);


////For testing begin: print the internal level
//			cout << "\n\n---------------------Internal Layer (depth: " << this->csb_depth << ")---------------------\n";
//			for (int ti = 0; ti < internal_node_group_num; ti++)
//			{
//				cout << "NodeGroup: " << ti << "\t";
//				cout << "Used nodes: " << internal[ti]->used_nodes << endl;
//				for (int i = 0; i < internal[ti]->used_nodes; i++)
//				{
//					cout << "Node: " << i << "\t";
//					cout << "Used keys: " << internal[ti]->internal_nodes[i]->used_keys << endl;
//					for (int k = 0; k < internal[ti]->internal_nodes[i]->used_keys; k++)
//					{
//						cout << ((CCSBNode<T>*)(internal[ti]->getNode(i)))->getElement(k)._key << " ";
//					}
//					cout << endl;
//				}
//				cout << endl;
//			}
//			cout << "Upper layer index keys: \n";
//			for (int i = 0; i < upper_internal_key_array_num; i++)
//				cout << upper_internal_key_array[i] << " ";
//			cout << endl;
////For testing end


			//loop construct the internal node groups until reach the root
			while (upper_internal_key_array_num > CSB_MAXNUM_KEY)
			{
				//释放已经建立索引的internalAray数据，将其上层索引in2Aray赋给该指针准备再次建立索引
				delete[] internal_key_array;
				internal_key_array = upper_internal_key_array;
				internal_key_array_num = upper_internal_key_array_num;
				leaf_node_group_num = internal_node_group_num;
				CCSBNodeGroup<T>** leaf = internal;

				//申请足够的中间结点group空间，注：上层索引node的个数即为下层的nodegroup数
				internal_node_group_num = ceil((double)leaf_node_group_num/(double)(CSB_MAXNUM_KEY+1));
				internal = (CCSBNodeGroup<T>**)new CCSBInternalNodeGroup<T>* [internal_node_group_num];

				//申请更上一层内部结点空间，即为本层node数目-1
				upper_internal_key_array = new T [leaf_node_group_num-1];

				upper_internal_key_array_num = makeInternalNodeGroup(internal_key_array, internal_key_array_num, internal, leaf_node_group_num, upper_internal_key_array, leaf);

////For testing begin: print the internal level
//				cout << "\n\n---------------------Internal Layer (depth: " << this->csb_depth << ")---------------------\n";
//				for (int ti = 0; ti < internal_node_group_num; ti++)
//				{
//					cout << "NodeGroup: " << ti << "\t";
//					cout << "Used nodes: " << internal[ti]->used_nodes << endl;
//					for (int i = 0; i < internal[ti]->used_nodes; i++)
//					{
//						cout << "Node: " << i << "\t";
//						cout << "Used keys: " << internal[ti]->internal_nodes[i]->used_keys << endl;
//						for (int k = 0; k < internal[ti]->internal_nodes[i]->used_keys; k++)
//						{
//							cout << ((CCSBNode<T>*)(internal[ti]->getNode(i)))->getElement(k)._key << " ";
//						}
//						cout << endl;
//					}
//					cout << endl;
//				}
//				cout << "Upper layer index keys: \n";
//				for (int i = 0; i < upper_internal_key_array_num; i++)
//					cout << upper_internal_key_array[i] << " ";
//				cout << endl;
////For testing end

			}
			if (upper_internal_key_array_num <= CSB_MAXNUM_KEY)
			{
				//建立根结点

				//释放已经建立索引的internalAray数据，将其上层索引in2Aray赋给该指针准备再次建立索引
				delete[] internal_key_array;
				internal_key_array = upper_internal_key_array;
				internal_key_array_num = upper_internal_key_array_num;
				leaf_node_group_num = internal_node_group_num;
				CCSBNodeGroup<T>** leaf = internal;

				this->csb_depth += 1;
				CCSBInternalNodeGroup<T>* internal = new CCSBInternalNodeGroup<T>(1);

				//将数据填入internalnode中
				for (int i = 0; i < internal_key_array_num; i++)
				{
					internal->internal_nodes[0]->setElement(i, internal_key_array[i]);
				}
				internal->internal_nodes[0]->used_keys = internal_key_array_num;
				internal->internal_nodes[0]->p_father = NULL;

				//设置与叶子层的父子关系
				((CCSBNode<T>*)internal->getNode(0))->setPointer((CCSBNodeGroup<T>*)leaf[0]);
				for (int i = 0; i < leaf[0]->used_nodes; i++)
					((CCSBNode<T>*)(leaf[0]->getNode(i)))->setFather(internal->getNode(0));

				this->csb_root = internal->internal_nodes[0];

////For testing begin: print the root node
//				cout << "\n\n---------------------Root Node (depth: " << this->csb_depth << ")---------------------\n";
//				cout << "Root: " << "\t";
//				cout << "used keys: " << internal->internal_nodes[0]->used_keys << endl;
//				for (int k = 0; k < internal->internal_nodes[0]->used_keys; k++)
//					cout << ((CCSBNode<T>*)(internal->getNode(0)))->getElement(k)._key << " ";
//				cout << endl;
////For testing end
			}
		}
	}
}

//将aray填入leafNodeGroup中，并记录上层索引key
template <typename T>
int CSBPlusTree<T>::makeLeafNodeGroup(data_offset<T>* aray, unsigned aray_num, CCSBNodeGroup<T>** leaf, T* internal_key_array)
{
	//m_Depth记录树高度，建立叶子层其自加1
	csb_depth += 1;

	//计算leafNodeGroup的数目
	int leaf_node_group_num = ceil((double)(ceil((double)(aray_num)/(double)(CSB_MAXNUM_DATA)))/(double)(CSB_MAXNUM_KEY+1));

	int internal_key_array_off = 0;  //记录internalAray下标

	//如果只有一个leafNodeGroup
	if (leaf_node_group_num == 1)
	{
		//判断之中有几个leafNode （arayNo <= CSB_MAXNUM_DATA的情况已单独讨论过，至少有两个leafNode结点）
		int leaf_node_num = ceil((double)aray_num/(double)CSB_MAXNUM_DATA);
		leaf[0] = new CCSBLeafNodeGroup<T>(leaf_node_num);

		//将数据填入leaf[0]中，对前leafNNo-2个结点而言，每个leafNode中均填满CSB_MAXNUM_DATA个数据
		unsigned i = 0, counter = 0;
		while (counter < leaf_node_num-2)
		{
			unsigned k = 0;
			for (k = 0; k < CSB_MAXNUM_DATA && i < aray_num; k++)
			{
				leaf[0]->getNode(counter)->setElement(i%CSB_MAXNUM_DATA, aray[i]);
				if (counter != 0 && k == 0)
					internal_key_array[internal_key_array_off++] = aray[i]._key;
				i += 1;
			}
			leaf[0]->getNode(counter)->setUsedKeys(k);
			counter += 1;
		}
		if (aray_num-(leaf_node_num-1)*CSB_MAXNUM_DATA >= CSB_ORDER_V) //不需平衡最后两个结点
		{
			int k = 0;
			while (counter < leaf_node_num)
			{
				for (k = 0; k < CSB_MAXNUM_DATA && i < aray_num; k++)
				{
					((CCSBNode<T>*)(leaf[0]->getNode(counter)))->setElement(k, aray[i]);
					if (counter != 0 && k == 0)
						internal_key_array[internal_key_array_off++] = aray[i]._key;
					i += 1;
				}
				((CCSBNode<T>*)(leaf[0]->getNode(counter)))->setUsedKeys(k);
				counter += 1;
			}
		}
		else //最后一个结点data数目小于CSB_ORDER_V，需平衡最后两个结点
		{
			//剩余的data数目
			int rest = aray_num-CSB_MAXNUM_DATA*(leaf_node_num-2);
			if (rest%2 ==1)
				((CCSBNode<T>*)(leaf[0]->getNode(counter)))->setUsedKeys(rest/2+1);
			else
				((CCSBNode<T>*)(leaf[0]->getNode(counter)))->setUsedKeys(rest/2);
			((CCSBNode<T>*)(leaf[0]->getNode(counter+1)))->setUsedKeys(rest/2);
			while (counter < leaf_node_num)
			{
				for (int k = 0; k < ((CCSBNode<T>*)(leaf[0]->getNode(counter)))->getUsedKeys(); k++)
				{
					((CCSBNode<T>*)(leaf[0]->getNode(counter)))->setElement(k, aray[i]);
					if (counter != 0 && k == 0)
						internal_key_array[internal_key_array_off++] = aray[i]._key;
					i += 1;
				}
				counter += 1;
			}
		}
	}
	else //至少有两个leafNodeGroup
	{
		unsigned i = 0, counter = 0;  //记录aray下标、leaf下标
		while (counter < leaf_node_group_num-2)  //对前leafNGNo-2个NG而言，每个NG中均填满CSB_MAXNUM_KEY+1个leafNode
		{
			leaf[counter] = new CCSBLeafNodeGroup<T>(CSB_MAXNUM_KEY+1);
			counter += 1;
		}
		int rest = ceil((double)(aray_num-(leaf_node_group_num-1)*(CSB_MAXNUM_KEY+1)*CSB_MAXNUM_DATA)/(double)CSB_MAXNUM_DATA);
		if (rest >= CSB_ORDER_V+1)  //无需平衡最后两个NG
		{
			//倒数第二个NG仍满，最后一个NG中leafNode的数目需要计算
			leaf[counter] = new CCSBLeafNodeGroup<T>(CSB_MAXNUM_KEY+1);
			leaf[counter+1] = new CCSBLeafNodeGroup<T>(rest);
		}
		else //最后一个NG中leafNode数目小于CSB_ORDER_V+1，需平衡最后两个NG
		{
			rest = rest + (CSB_MAXNUM_KEY+1);
			if (rest%2 == 0)
				leaf[counter] = new CCSBLeafNodeGroup<T>(rest/2);
			else
				leaf[counter] = new CCSBLeafNodeGroup<T>(rest/2+1);
			leaf[counter+1] = new CCSBLeafNodeGroup<T>(rest/2);
		}

		//将data填入之前申请好的leafNG中
		for (counter = 0; counter < leaf_node_group_num-1; counter++) //注意，最后一个NG中的最后两个leafNode可能需要平衡
		{
			for (int j = 0; j < leaf[counter]->used_nodes; j++)
			{
				unsigned k = 0;
				for (k = 0; k < CSB_MAXNUM_DATA; k++)
				{
					((CCSBNode<T>*)(leaf[counter]->getNode(j)))->setElement(k, aray[i]);
					if (i != 0 && k == 0)
						internal_key_array[internal_key_array_off++] = aray[i]._key;
					i += 1;
				}
				((CCSBNode<T>*)(leaf[counter]->getNode(j)))->setUsedKeys(k);
			}
		}
		//最后一个NG的填入
		int restdata = aray_num%CSB_MAXNUM_DATA;
		if (restdata == 0)
			restdata = CSB_MAXNUM_DATA;
		//确定每个leafNode中填充的data数目
		int j = 0;
		for (j = 0; j < leaf[counter]->used_nodes-2; j++)
		{
			((CCSBNode<T>*)(leaf[counter]->getNode(j)))->setUsedKeys(CSB_MAXNUM_DATA);
		}
		if (restdata >= CSB_ORDER_V)  //无需平衡
		{
			((CCSBNode<T>*)(leaf[counter]->getNode(j)))->setUsedKeys(CSB_MAXNUM_DATA);
			((CCSBNode<T>*)(leaf[counter]->getNode(j+1)))->setUsedKeys(restdata);
		}
		else  //平衡最后两个leafNode
		{
			restdata += CSB_MAXNUM_DATA;
			if (restdata%2 == 0)
				((CCSBNode<T>*)(leaf[counter]->getNode(j)))->setUsedKeys(restdata/2);
			else
				((CCSBNode<T>*)(leaf[counter]->getNode(j)))->setUsedKeys(restdata/2+1);
			((CCSBNode<T>*)(leaf[counter]->getNode(j+1)))->setUsedKeys(restdata/2);
		}
		for (j = 0; j < leaf[counter]->used_nodes; j++)
		{
			for (unsigned k = 0; k < ((CCSBNode<T>*)(leaf[counter]->getNode(j)))->getUsedKeys(); k++)
			{
				((CCSBNode<T>*)(leaf[counter]->getNode(j)))->setElement(k, aray[i]);
				if (i != 0 && k == 0)
					internal_key_array[internal_key_array_off++] = aray[i]._key;
				i++;
			}
		}

	}

	//set the doubly linked list in the leaf node group
	leaf_header = leaf[0];
	leaf[0]->setHeaderNG(NULL);
	leaf_tailer = leaf[leaf_node_group_num-1];
	leaf[leaf_node_group_num-1]->setTailerNG(NULL);
	for (unsigned i = 0; i < leaf_node_group_num-1; i++)
		leaf[i]->setTailerNG(leaf[i+1]);
	for (unsigned i = 1; i < leaf_node_group_num; i++)
		leaf[i]->setHeaderNG(leaf[i-1]);
	return internal_key_array_off;
}

//建立最底层索引，直接索引leafNodeGroup层，并记录其上层索引key
template <typename T>
int CSBPlusTree<T>::makeInternalNodeGroup(T* aray, int aray_num, CCSBNodeGroup<T>** internal, int leaf_node_group_num, T* upper_key_array, CCSBNodeGroup<T>** leaf)
{
	//m_Depth记录树高度，建立叶子层其自加1
	csb_depth += 1;
	//计算internalNodeGroup数目
	int internal_node_group_num = ceil((double)leaf_node_group_num/(double)(CSB_MAXNUM_KEY+1));
//	internal = (CCSBNodeGroup<T>**)new CCSBInternalNodeGroup<T>* [internal_node_group_num];

	int upper_key_array_off = 0;  //记录in2Aray下标

	int counter = 0;  //用于记录internal的下标

	if (internal_node_group_num >= 2)
	{
		while (counter < internal_node_group_num-2)  //除了最后两个NG外，其他的NG均填满CSB_MAXNUM_KEY+1个node
		{
			internal[counter] = new CCSBInternalNodeGroup<T>(CSB_MAXNUM_KEY+1);
			counter += 1;
		}
		int rest = leaf_node_group_num-(internal_node_group_num-1)*(CSB_MAXNUM_KEY+1);
		if (rest >= CSB_ORDER_V+1)  //不需平衡最后两个NG
		{
			internal[counter] = new CCSBInternalNodeGroup<T>(CSB_MAXNUM_KEY+1);
			internal[counter+1] = new CCSBInternalNodeGroup<T>(rest);
		}
		else //最后一个NG中leafNode数目小于CSB_ORDER_V+1，需平衡最后两个NG
		{
			rest = rest + (CSB_MAXNUM_KEY+1);
			if (rest%2 == 0)
				internal[counter] = new CCSBInternalNodeGroup<T>(rest/2);
			else
				internal[counter] = new CCSBInternalNodeGroup<T>(rest/2+1);
			internal[counter+1] = new CCSBInternalNodeGroup<T>(rest/2);
		}
	}
	else  //只有一个NG
		internal[counter] = new CCSBInternalNodeGroup<T>(leaf_node_group_num);

	//每个internal中包含的node数目申请结束，将internalAray中的数据填入internal中，并生成该层的索引in2Aray
	//其下一层的NG中只有最后两个可能不满，所以该层NG中的所有node只有最后两个可能不满，即最后一个NG要特殊处理
	int i = 0;  //用于记录internalAray的下标
	for (counter = 0; counter < internal_node_group_num-1; counter++)
	{
		for (int j = 0; j < internal[counter]->getUsedNodes(); j++)
		{
			int k = 0;
			for (k = 0; k < CSB_MAXNUM_KEY; k++)
			{
				((CCSBNode<T>*)(internal[counter]->getNode(j)))->setElement(k, aray[i++]);
			}
			((CCSBNode<T>*)(internal[counter]->getNode(j)))->setUsedKeys(k);
			upper_key_array[upper_key_array_off++] = aray[i++];
		}
	}
	//最后一个NG
	int j = 0;
	for (j = 0; j < internal[counter]->getUsedNodes()-2; j++)
	{
		int k = 0;
		for (k = 0; k < CSB_MAXNUM_KEY; k++)
		{
			((CCSBNode<T>*)(internal[counter]->getNode(j)))->setElement(k, aray[i++]);
		}
		((CCSBNode<T>*)(internal[counter]->getNode(j)))->setUsedKeys(k);
		upper_key_array[upper_key_array_off++] = aray[i++];
	}
	//最后两个node中的key数目为leaf中最后两个nodegroup中node数-1
	((CCSBNode<T>*)(internal[counter]->getNode(j)))->setUsedKeys(leaf[leaf_node_group_num-2]->getUsedNodes()-1);
	((CCSBNode<T>*)(internal[counter]->getNode(j+1)))->setUsedKeys(leaf[leaf_node_group_num-1]->getUsedNodes()-1);
	for (; j < internal[counter]->getUsedNodes(); j++)
	{
		int k = 0;
		for (k = 0; k < ((CCSBNode<T>*)(internal[counter]->getNode(j)))->getUsedKeys(); k++)
		{
			((CCSBNode<T>*)(internal[counter]->getNode(j)))->setElement(k, aray[i++]);
		}
		((CCSBNode<T>*)(internal[counter]->getNode(j)))->setUsedKeys(k);
		if (i < aray_num)
			upper_key_array[upper_key_array_off++] = aray[i++];
	}

	//设置父子关系
	int leafi = 0; //用于记录leaf层NG的编号
	for (counter = 0; counter < internal_node_group_num; counter++)
	{
		for (int j = 0; j < internal[counter]->getUsedNodes(); j++)
		{
			((CCSBNode<T>*)(internal[counter]->getNode(j)))->setPointer(leaf[leafi]);

			for (int k = 0; k < leaf[leafi]->getUsedNodes(); k++)
			{
				((CCSBNode<T>*)(leaf[leafi]->getNode(k)))->setFather(internal[counter]->getNode(j));
			}
			leafi += 1;
		}

	}
	return upper_key_array_off;
}

// 查找指定的数据
template <typename T>
map<index_offset, vector<index_offset> > CSBPlusTree<T>::Search(T key)
//vector<search_result*> CSBPlusTree<T>::Search(T key)
{
	map<index_offset, vector<index_offset> > ret;
//	vector<search_result*> ret;
	ret.clear();
	int i = 0;
/*for testing*/	char* sPath = (char*)malloc(1024);
/*for testing*/	memset((void*)sPath, 0, 1024);

//For testing begin
	int offset = 0;
	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "The serach path is:");
		offset+=19;
	}
//For testing end

	CCSBNode<T>* search_node = csb_root;

	//find the leaf node
	for (unsigned depth = 1; depth < this->csb_depth; depth++)
	{
		//find the first search_node.key >= key
		for (i = 0; (key > search_node->getElement(i)._key)&&(i < search_node->used_keys); i++);

//For testing begin
		if (NULL != sPath)
		{
			(void)sprintf(sPath+offset, " %3d -->", search_node->getElement(0)._key);
			offset+=8;
		}
//For testing end

		search_node = (search_node->getPointer())->getNode(i);
	}
	//not found
	if (NULL == search_node)
		return ret;

//For testing begin
	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "%3d", search_node->getElement(0)._key);
		offset+=3;
	}
//For testing end

	//finding the first data in leaf_node whose key = search key
/*for testing*/	bool found = false;
	for (i = 0; (i < search_node->used_keys); i++)
	{
		if (key == search_node->getElement(i)._key)
		{
/*for testing*/			found = true;
			break;
		}
	}

	//collect all tuples whose key equals to search key
	if (search_node->getFather() == NULL)
	{
		for (; i < search_node->getUsedKeys(); i++)
		{
			if (key == search_node->getElement(i)._key)
				ret[search_node->getElement(i)._block_off].push_back(search_node->getElement(i)._tuple_off);
			else
				return ret;
		}
		return ret;
	}
	CCSBNodeGroup<T>* search_node_group = search_node->getFather()->getPointer();
	unsigned j = 0;
	for (j = 0; j < search_node_group->getUsedNodes(); j++)
	{
		if (search_node == search_node_group->getNode(j))
			break;
	}
	while (search_node_group != NULL)
	{
		for (; j < search_node_group->getUsedNodes(); j++)
		{
			for (; i < search_node_group->getNode(j)->getUsedKeys(); i++)
			{
				if (key == search_node_group->getNode(j)->getElement(i)._key)
					ret[search_node_group->getNode(j)->getElement(i)._block_off].push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
				else
					return ret;
			}
			i = 0;
		}
		i = 0;
		j = 0;
		search_node_group = search_node_group->getTailerNG();
	}

//For testing begin
	if (NULL != sPath)
	{
		if (true == found)

			(void)sprintf(sPath+offset, " ,succeeded.");
		else
			(void)sprintf(sPath+offset, " ,failed.");
	}
//For testing end

	return ret;
}

template <typename T>
map<index_offset, vector<index_offset> > CSBPlusTree<T>::rangeQuery(T lower_key, T upper_key)
{
	map<index_offset, vector<index_offset> > ret;
	ret.clear();
	if (lower_key > upper_key)
		return ret;
	int i = 0;
/*for testing*/	char* sPath = (char*)malloc(1024);
/*for testing*/	memset((void*)sPath, 0, 1024);

//For testing begin
	int offset = 0;
	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "The serach path is:");
		offset+=19;
	}
//For testing end

	CCSBNode<T>* search_node = csb_root;

	//find the leaf node
	for (unsigned depth = 1; depth < this->csb_depth; depth++)
	{
		//find the first search_node.key >= lower_key
		for (i = 0; (lower_key > search_node->getElement(i)._key)&&(i < search_node->used_keys); i++);

//For testing begin
		if (NULL != sPath)
		{
			(void)sprintf(sPath+offset, " %3d -->", search_node->getElement(0)._key);
			offset+=8;
		}
//For testing end

		search_node = (search_node->getPointer())->getNode(i);
	}
	//not found
	if (NULL == search_node)
		return ret;

//For testing begin
	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "%3d", search_node->getElement(0)._key);
		offset+=3;
	}
//For testing end

	//finding the first data in leaf_node whose key >= lower_key
/*for testing*/	bool found = false;
	for (i = 0; (i < search_node->used_keys); i++)
	{
		if (lower_key <= search_node->getElement(i)._key)
		{
/*for testing*/			found = true;
			break;
		}
	}

	//collect all tuples whose key is between lower_key and upper_key
	if (search_node->getFather() == NULL)
	{
		for (; i < search_node->getUsedKeys(); i++)
		{
			if ((lower_key <= search_node->getElement(i)._key) && upper_key >= search_node->getElement(i)._key)
				ret[search_node->getElement(i)._block_off].push_back(search_node->getElement(i)._tuple_off);
			else
				return ret;
		}
		return ret;
	}
	CCSBNodeGroup<T>* search_node_group = search_node->getFather()->getPointer();
	unsigned j = 0;
	for (j = 0; j < search_node_group->getUsedNodes(); j++)
	{
		if (search_node == search_node_group->getNode(j))
			break;
	}
	while (search_node_group != NULL)
	{
		for (; j < search_node_group->getUsedNodes(); j++)
		{
			for (; i < search_node_group->getNode(j)->getUsedKeys(); i++)
			{
				if ((lower_key <= search_node_group->getNode(j)->getElement(i)._key) && (upper_key >= search_node_group->getNode(j)->getElement(i)._key))
					ret[search_node_group->getNode(j)->getElement(i)._block_off].push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
				else
					return ret;
			}
			i = 0;
		}
		i = 0;
		j = 0;
		search_node_group = search_node_group->getTailerNG();
	}

//For testing begin
	if (NULL != sPath)
	{
		if (true == found)

			(void)sprintf(sPath+offset, " ,succeeded.");
		else
			(void)sprintf(sPath+offset, " ,failed.");
	}
//For testing end
	return ret;
}

template<typename T>
map<index_offset, vector<index_offset> > CSBPlusTree<T>::rangeQuery(T lower_key, comparison comp_lower, T upper_key, comparison comp_upper)
{
	//For point query
	if (comp_lower == FilterIterator::AttributeComparator::EQ)
	{
		if (lower_key != upper_key)
			cout << "[ERROR: CSBPlusTree.cpp->rangeQuery()]: For the equal point query, the lower_key " << lower_key << " != the upper_key " << upper_key << endl;
		return Search(lower_key);
	}
	// Range Query
	else if ((!(comp_lower == FilterIterator::AttributeComparator::G || comp_lower == FilterIterator::AttributeComparator::GEQ)) || (!(comp_upper == FilterIterator::AttributeComparator::L || comp_upper == FilterIterator::AttributeComparator::LEQ)))
	{
		cout << "[ERROR: CSBPlusTree.cpp->rangeQuery()]: For the range query, the given two key isn't a range\n";
		exit(-1);
	}
	map<index_offset, vector<index_offset> > ret;
	ret.clear();
	if (lower_key > upper_key)
		return ret;
	int i = 0;
/*for testing*/	char* sPath = (char*)malloc(1024);
/*for testing*/	memset((void*)sPath, 0, 1024);

//For testing begin
	int offset = 0;
	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "The serach path is:");
		offset+=19;
	}
//For testing end

	CCSBNode<T>* search_node = csb_root;

	//find the leaf node
	for (unsigned depth = 1; depth < this->csb_depth; depth++)
	{
		//find the first search_node.key > lower_key for GEQ and search_node.key >= lower_key for G
		if (comp_lower == FilterIterator::AttributeComparator::GEQ)
			for (i = 0; (lower_key > search_node->getElement(i)._key)&&(i < search_node->used_keys); i++);
		else if (comp_lower == FilterIterator::AttributeComparator::G)
			for (i = 0; (lower_key >= search_node->getElement(i)._key)&&(i < search_node->used_keys); i++);

//For testing begin
		if (NULL != sPath)
		{
			(void)sprintf(sPath+offset, " %3d -->", search_node->getElement(0)._key);
			offset+=8;
		}
//For testing end

		search_node = (search_node->getPointer())->getNode(i);
	}
	//not found
	if (NULL == search_node)
		return ret;

//For testing begin
	if (NULL != sPath)
	{
		(void)sprintf(sPath+offset, "%3d", search_node->getElement(0)._key);
		offset+=3;
	}
//For testing end

	//finding the first data in leaf_node whose key >= lower_key
/*for testing*/	bool found = false;
	for (i = 0; (i < search_node->used_keys); i++)
	{
		if (comp_lower == FilterIterator::AttributeComparator::GEQ)
		{
			if (lower_key <= search_node->getElement(i)._key)
			{
/*for testing*/				found = true;
				break;
			}
		}
		if (comp_lower == FilterIterator::AttributeComparator::G)
		{
			if (lower_key < search_node->getElement(i)._key)
			{
/*for testing*/				found = true;
				break;
			}
		}
	}

	//collect all tuples whose key is between lower_key and upper_key
	if (search_node->getFather() == NULL)
	{
		if (comp_lower == FilterIterator::AttributeComparator::GEQ && comp_upper == FilterIterator::AttributeComparator::LEQ)
		{
			for (; i < search_node->getUsedKeys(); i++)
			{
				if ((lower_key <= search_node->getElement(i)._key) && upper_key >= search_node->getElement(i)._key)
					ret[search_node->getElement(i)._block_off].push_back(search_node->getElement(i)._tuple_off);
				else
					return ret;
			}
		}
		else if (comp_lower == FilterIterator::AttributeComparator::GEQ && comp_upper == FilterIterator::AttributeComparator::L)
		{
			for (; i < search_node->getUsedKeys(); i++)
			{
				if ((lower_key <= search_node->getElement(i)._key) && upper_key > search_node->getElement(i)._key)
					ret[search_node->getElement(i)._block_off].push_back(search_node->getElement(i)._tuple_off);
				else
					return ret;
			}
		}
		else if (comp_lower == FilterIterator::AttributeComparator::G && comp_upper == FilterIterator::AttributeComparator::LEQ)
		{
			for (; i < search_node->getUsedKeys(); i++)
			{
				if ((lower_key < search_node->getElement(i)._key) && upper_key >= search_node->getElement(i)._key)
					ret[search_node->getElement(i)._block_off].push_back(search_node->getElement(i)._tuple_off);
				else
					return ret;
			}
		}
		else if (comp_lower == FilterIterator::AttributeComparator::G && comp_upper == FilterIterator::AttributeComparator::L)
		{
			for (; i < search_node->getUsedKeys(); i++)
			{
				if ((lower_key < search_node->getElement(i)._key) && upper_key > search_node->getElement(i)._key)
					ret[search_node->getElement(i)._block_off].push_back(search_node->getElement(i)._tuple_off);
				else
					return ret;
			}
		}
	}
	CCSBNodeGroup<T>* search_node_group = search_node->getFather()->getPointer();
	unsigned j = 0;
	for (j = 0; j < search_node_group->getUsedNodes(); j++)
	{
		if (search_node == search_node_group->getNode(j))
			break;
	}
	while (search_node_group != NULL)
	{
		if (comp_lower == FilterIterator::AttributeComparator::GEQ && comp_upper == FilterIterator::AttributeComparator::LEQ)
		{
			for (; j < search_node_group->getUsedNodes(); j++)
			{
				for (; i < search_node_group->getNode(j)->getUsedKeys(); i++)
				{
					if ((lower_key <= search_node_group->getNode(j)->getElement(i)._key) && (upper_key >= search_node_group->getNode(j)->getElement(i)._key))
						ret[search_node_group->getNode(j)->getElement(i)._block_off].push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
					else
						return ret;
				}
				i = 0;
			}
			i = 0;
			j = 0;
			search_node_group = search_node_group->getTailerNG();
		}
		else if (comp_lower == FilterIterator::AttributeComparator::GEQ && comp_upper == FilterIterator::AttributeComparator::L)
		{
			for (; j < search_node_group->getUsedNodes(); j++)
			{
				for (; i < search_node_group->getNode(j)->getUsedKeys(); i++)
				{
					if ((lower_key <= search_node_group->getNode(j)->getElement(i)._key) && (upper_key > search_node_group->getNode(j)->getElement(i)._key))
						ret[search_node_group->getNode(j)->getElement(i)._block_off].push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
					else
						return ret;
				}
				i = 0;
			}
			i = 0;
			j = 0;
			search_node_group = search_node_group->getTailerNG();
		}
		else if (comp_lower == FilterIterator::AttributeComparator::G && comp_upper == FilterIterator::AttributeComparator::LEQ)
		{
			for (; j < search_node_group->getUsedNodes(); j++)
			{
				for (; i < search_node_group->getNode(j)->getUsedKeys(); i++)
				{
					if ((lower_key < search_node_group->getNode(j)->getElement(i)._key) && (upper_key >= search_node_group->getNode(j)->getElement(i)._key))
						ret[search_node_group->getNode(j)->getElement(i)._block_off].push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
					else
						return ret;
				}
				i = 0;
			}
			i = 0;
			j = 0;
			search_node_group = search_node_group->getTailerNG();
		}
		else if (comp_lower == FilterIterator::AttributeComparator::G && comp_upper == FilterIterator::AttributeComparator::L)
		{
			for (; j < search_node_group->getUsedNodes(); j++)
			{
				for (; i < search_node_group->getNode(j)->getUsedKeys(); i++)
				{
					if ((lower_key < search_node_group->getNode(j)->getElement(i)._key) && (upper_key > search_node_group->getNode(j)->getElement(i)._key))
						ret[search_node_group->getNode(j)->getElement(i)._block_off].push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
					else
						return ret;
				}
				i = 0;
			}
			i = 0;
			j = 0;
			search_node_group = search_node_group->getTailerNG();
		}
	}

//For testing begin
	if (NULL != sPath)
	{
		if (true == found)

			(void)sprintf(sPath+offset, " ,succeeded.");
		else
			(void)sprintf(sPath+offset, " ,failed.");
	}
//For testing end

	return ret;
}

/* 插入指定的数据，分几种情况讨论
 * 1. 初始索引树为空，直接生成根结点，并将要插入的数据填入
 * 2. 要插入的目标叶子结点未满，直接填入data
 * 3. 要插入的目标叶子结点已满，判断其上层结点情况
   * 3.1 若该叶子结点无父结点，即其本身为根结点，则将原始的node分裂为同在一个leafNG中的两个node，并建立新的根结点索引该leafNG
   * 3.2 若该叶子结点的父结点未满，则说明该叶子结点所在的leafNG未满，申请新的leafNG=leafNG+1，分裂目标插入结点将data插入，并在其父结点中增加一个索引key
   * 3.3 若该叶子结点的父结点已满，则说明该叶子结点所在的leafNG也已满，申请新的leafNG1，大小为CSB_MAXMUM_KEY/2+1，将原始leafNG中的一半node移动到leafNG1中，插入data；
         并向其父结点中增加一个索引key，导致父结点分裂为两个结点，分别索引leafNG和leafNG1，父结点的分裂策略依其父结点的情况而定
*/
template <typename T>
bool CSBPlusTree<T>::Insert(data_offset<T> data)
{
//	//check if the data is already exists
//	bool found = Search(data._key, NULL);
//	if (true == found)
//	{
//		return false;
//	}

	//search the insert_node
	CCSBNode<T> *leaf_node = SearchLeafNode(data._key);

	//1. insert_node = NULL: the indexing tree is empty, create the index uses insert_data
	if (NULL == leaf_node)
	{
		this->csb_depth = 1;
		CCSBLeafNodeGroup<T>* leaf = new CCSBLeafNodeGroup<T>(1);

		//将数据填入leafnode中
		((CCSBNode<T>*)(leaf->getNode(0)))->setElement(0, data);
		((CCSBNode<T>*)(leaf->getNode(0)))->setUsedKeys(1);
		((CCSBNode<T>*)(leaf->getNode(0)))->setFather(NULL);
		((CCSBNode<T>*)(leaf->getNode(0)))->setPointer(NULL);
		leaf->setHeaderNG(NULL);
		leaf->setTailerNG(NULL);
		this->leaf_header = leaf;
		this->leaf_tailer = leaf;
		this->csb_root = leaf->getNode(0);
		return true;
	}

	//2. the insert_node is not full: insert directly
	if (leaf_node->getUsedKeys() < CSB_MAXNUM_DATA)
		return leaf_node->Insert(data);

	//3.1 the insert_node is full and it's the root node
	else if (leaf_node->getFather() == NULL)
	{
		//allocate a new leaf_node_group which has two nodes, split the insert_node into the two new nodes
		CCSBNodeGroup<T>* leaf = (CCSBNodeGroup<T>*) new CCSBLeafNodeGroup<T>(2);
		for (int i = 0; i < CSB_MAXNUM_DATA; i++)
		{
			if (i < CSB_MAXNUM_DATA/2)
				((CCSBNode<T>*)(leaf->getNode(0)))->setElement(i, leaf_node->getElement(i));
			else
				((CCSBNode<T>*)(leaf->getNode(1)))->setElement(i-CSB_MAXNUM_DATA/2, leaf_node->getElement(i));
		}
		((CCSBNode<T>*)(leaf->getNode(0)))->setUsedKeys(CSB_MAXNUM_DATA/2);
		((CCSBNode<T>*)(leaf->getNode(1)))->setUsedKeys(CSB_MAXNUM_DATA-CSB_MAXNUM_DATA/2);

		//set the double linked list
		leaf->setHeaderNG(NULL);
		leaf->setTailerNG(NULL);
		this->leaf_header = leaf;
		this->leaf_tailer = leaf;

		//construct the new root node
		CCSBNodeGroup<T>* root = (CCSBNodeGroup<T>*) new CCSBInternalNodeGroup<T>(1);

		//insert the index_key into the root node
		((CCSBNode<T>*)(root->getNode(0)))->setElement(0, ((CCSBNode<T>*)(leaf->getNode(1)))->getElement(0)._key);
		((CCSBNode<T>*)(root->getNode(0)))->setUsedKeys(1);
		((CCSBNode<T>*)(root->getNode(0)))->setFather(NULL);
		((CCSBNode<T>*)(root->getNode(0)))->setPointer(leaf);
		this->csb_root = root->getNode(0);
		this->csb_depth += 1;

		//set the parents-children relationship
		((CCSBNode<T>*)(leaf->getNode(0)))->setFather(this->csb_root);
		((CCSBNode<T>*)(leaf->getNode(1)))->setFather(this->csb_root);

		//insert the insert_data into one of the new leaf nodes
		if (data._key < this->csb_root->getElement(0)._key)
			return ((CCSBNode<T>*)(this->csb_root->getPointer()->getNode(0)))->Insert(data);
		else
			return ((CCSBNode<T>*)(this->csb_root->getPointer()->getNode(1)))->Insert(data);
	}

	//3.2 the insert_node is full but its parent is not full
	if (((CCSBNode<T>*)(leaf_node->getFather()))->getUsedKeys() < CSB_MAXNUM_KEY)
	{
		//find the leaf_node_group which the insert_node is in
		CCSBNodeGroup<T>* original_leaf = ((CCSBNode<T>*)(leaf_node->getFather()))->getPointer();
		//allocate new leaf_node_group whose used_nodes is one more than the original_leaf
		CCSBNodeGroup<T>* leaf = (CCSBNodeGroup<T>*) new CCSBLeafNodeGroup<T>(original_leaf->getUsedNodes()+1);

		unsigned i = 0;
		for (i = 0; i < leaf->getUsedNodes(); i++)
		{
			//for the node which is before the insert_node, copy into leaf
			if (original_leaf->getNode(i) < leaf_node)
				leaf->setNode(i, original_leaf->getNode(i));
			//split the insert_node
			else if (original_leaf->getNode(i) == leaf_node)
			{
				for (int j = 0; j < CSB_MAXNUM_DATA; j++)
				{
					if (j < CSB_MAXNUM_DATA/2)
						((CCSBNode<T>*)(leaf->getNode(i)))->setElement(j, leaf_node->getElement(j));
					else
						((CCSBNode<T>*)(leaf->getNode(i+1)))->setElement(j-CSB_MAXNUM_DATA/2, leaf_node->getElement(j));
				}
				((CCSBNode<T>*)(leaf->getNode(i)))->setFather(leaf_node->getFather());
				((CCSBNode<T>*)(leaf->getNode(i++)))->setUsedKeys(CSB_MAXNUM_DATA/2);
				((CCSBNode<T>*)(leaf->getNode(i)))->setFather(leaf_node->getFather());
				((CCSBNode<T>*)(leaf->getNode(i)))->setUsedKeys(CSB_MAXNUM_DATA-CSB_MAXNUM_DATA/2);

				//add a index key to the parent to index the new node
				((CCSBNode<T>*)(((CCSBNode<T>*)leaf_node)->getFather()))->Insert(((CCSBNode<T>*)(leaf->getNode(i)))->getElement(0)._key);

				//insert the insert_data into one of the two split leaf nodes
				if (data._key < ((CCSBNode<T>*)(leaf->getNode(i)))->getElement(0)._key)
					((CCSBNode<T>*)(leaf->getNode(i-1)))->Insert(data);
				else
					((CCSBNode<T>*)(leaf->getNode(i)))->Insert(data);
				break;
			}
		}
		for (i = i+1;i < leaf->getUsedNodes(); i++)
		{
			//for the node which is before the insert_node, copy into leaf
			leaf->setNode(i, original_leaf->getNode(i-1));
		}
		//make leaf instead of original_leaf and delete the space of original_leaf
		((CCSBNode<T>*)(((CCSBNode<T>*)(original_leaf->getNode(0)))->getFather()))->setPointer(leaf);
		leaf->setHeaderNG(original_leaf->getHeaderNG());
		if (original_leaf->getHeaderNG() == NULL)
			this->leaf_header = leaf;
		else
			original_leaf->getHeaderNG()->setTailerNG(leaf);
		leaf->setTailerNG(original_leaf->getTailerNG());
		if (original_leaf->getTailerNG() == NULL)
			this->leaf_tailer = leaf;
		else
			original_leaf->getTailerNG()->setHeaderNG(leaf);
		delete original_leaf;
		return true;
	}

	//3.3 both the insert_node and its parent are full
	if (((CCSBNode<T>*)(((CCSBNode<T>*)leaf_node)->getFather()))->getUsedKeys() >= CSB_MAXNUM_KEY)
	{
		//allocate two new leaf_node_group to split the original_leaf_node_group which insert_node is in
		CCSBNodeGroup<T>* original_leaf = ((CCSBNode<T>*)(leaf_node->getFather()))->getPointer();
		unsigned insert_node_offset = 0;
		for (; insert_node_offset < original_leaf->getUsedNodes(); insert_node_offset++)
		{
			if (leaf_node == original_leaf->getNode(insert_node_offset))
				break;
		}

		CCSBNodeGroup<T>* left_leaf = (CCSBNodeGroup<T>*) new CCSBLeafNodeGroup<T>();
		CCSBNodeGroup<T>* right_leaf = (CCSBNodeGroup<T>*) new CCSBLeafNodeGroup<T>();
		if (insert_node_offset < CSB_MAXNUM_DATA/2)
		{
			left_leaf->setUsedNodes(CSB_MAXNUM_DATA/2);
			right_leaf->setUsedNodes(CSB_MAXNUM_DATA-CSB_MAXNUM_DATA/2);
		}
		else
		{
			left_leaf->setUsedNodes(CSB_MAXNUM_DATA-CSB_MAXNUM_DATA/2);
			right_leaf->setUsedNodes(CSB_MAXNUM_DATA/2);
		}
		//set the double linked list for leaf layer
		left_leaf->setTailerNG(right_leaf);
		right_leaf->setHeaderNG(left_leaf);
		left_leaf->setHeaderNG(original_leaf->getHeaderNG());
		if (original_leaf->getHeaderNG() == NULL)
			this->leaf_header = left_leaf;
		else
			original_leaf->getHeaderNG()->setTailerNG(left_leaf);
		right_leaf->setTailerNG(original_leaf->getTailerNG());
		if (original_leaf->getTailerNG() == NULL)
			this->leaf_tailer = right_leaf;
		else
			original_leaf->getTailerNG()->setHeaderNG(right_leaf);

		//split the original_leaf and insert the data into new leaf node group
		//record the upper layer index key = index_key
		T index_key;
		for (unsigned i = 0; i < left_leaf->getUsedNodes(); i++)
		{
			if (i < insert_node_offset)
				left_leaf->setNode(i, original_leaf->getNode(i));
			else if (i == insert_node_offset)
			{
				CCSBNode<T>* new_node = new CCSBLeafNode<T>();
				left_leaf->setNode(i, original_leaf->getNode(i));
				index_key = left_leaf->getNode(i)->SplitInsert(new_node, data);
				left_leaf->setNode(++i, new_node);
			}
			else
				left_leaf->setNode(i, original_leaf->getNode(i-1));
		}
		for (unsigned i = 0; i < right_leaf->getUsedNodes(); i++)
		{
			unsigned original_leaf_node_off = i+left_leaf->getUsedNodes();
			if (original_leaf_node_off < insert_node_offset)
				right_leaf->setNode(i, original_leaf->getNode(original_leaf_node_off));
			else if (original_leaf_node_off == insert_node_offset)
			{
				CCSBNode<T>* new_node = new CCSBLeafNode<T>();
				right_leaf->setNode(i, original_leaf->getNode(original_leaf_node_off));
				index_key = right_leaf->getNode(i)->SplitInsert(new_node, data);
				right_leaf->setNode(++i, new_node);
			}
			else
				right_leaf->setNode(i, original_leaf->getNode(original_leaf_node_off-1));
		}

		//insert the index_key into the original_leaf's parent node
	}
	return false;
}

// 为插入而查找叶子结点
template <typename T>
CCSBNode<T>* CSBPlusTree<T>::SearchLeafNode(T key)
{
	int i = 0;
	CCSBNode<T>* search_node = csb_root;

	//find the leaf node
	for (unsigned depth = 1; depth < this->csb_depth; depth++)
	{
		//find the first search_node.key >= key
		for (i = 0; (key >= search_node->getElement(i)._key)&&(i < search_node->used_keys); i++);
		search_node = (search_node->getPointer())->getNode(i);
	}

	if (search_node == NULL)
		return NULL;

	//transfer the forward linked list to find the last key = search_key
	if (search_node->getFather() == NULL) //root, just one node
		return search_node;

	CCSBNodeGroup<T>* search_node_group = search_node->getFather()->getPointer();
	unsigned j = 0;
	for (j = 0; j < search_node_group->getUsedNodes(); j++)
	{
		if (search_node == search_node_group->getNode(j))
			break;
	}
	while (search_node_group != NULL)
	{
		for (; j < search_node_group->getUsedNodes(); j++)
		{
			if (key < search_node_group->getNode(j)->getElement(0)._key)
			{
				search_node = search_node_group->getNode(j-1);
				break;
			}
		}
		search_node = search_node_group->getNode(j-1);
		j = 0;
		search_node_group = search_node_group->getTailerNG();
	}
	return search_node;
}

template <typename T>
bool CSBPlusTree<T>::Delete(T key)
{

}

template<typename T>
bool CSBPlusTree<T>::serialize(FILE* filename)
{
	if (this->csb_root == NULL)
	{
		cout << "The index tree is empty! Nothing could be serialized!\n";
		return true;
	}

	vector<CCSBNodeGroup<T>* > current_level;
	vector<CCSBNodeGroup<T>* > lower_level;
	current_level.clear();
	lower_level.clear();
	int depth = 1;

	fwrite((void*)(&csb_depth), sizeof(unsigned), 1, filename);
	csb_root->serialize(filename);
	lower_level.push_back(((CCSBNode<T>*)(this->csb_root))->getPointer());
	depth++;

	while (depth <= csb_depth)
	{
		current_level.clear();
		while(lower_level.size() != 0)
		{
			current_level.push_back(lower_level.back());
			lower_level.pop_back();
		}
		lower_level.clear();

		while (current_level.size() != 0)
		{
			if (depth != csb_depth)
			{
				for (unsigned i = 0; i < current_level.back()->getUsedNodes(); i++)
					lower_level.push_back(current_level.back()->getNode(i)->getPointer());
			}
			current_level.back()->serialize(filename);
			current_level.pop_back();
		}
		depth++;
	}

	return true;
}

template <typename T>
bool CSBPlusTree<T>::deserialize(FILE* filename)
{
	vector<CCSBNodeGroup<T>* > current_level;
	vector<CCSBNodeGroup<T>* > upper_level;
	unsigned depth = 1;
	fread((void*)(&csb_depth), sizeof(unsigned), 1, filename);

	if (csb_depth == 1)
	{
		CCSBNodeGroup<T>* leaf = (CCSBNodeGroup<T>*)new CCSBLeafNodeGroup<T>(1);
		leaf->getNode(0)->deserialize(filename);

		csb_root = leaf->getNode(0);
		leaf->setHeaderNG(NULL);
		leaf->setTailerNG(NULL);
		leaf_header = leaf;
		leaf_tailer = leaf;
		return true;
	}

	CCSBNodeGroup<T>* root_group = new CCSBInternalNodeGroup<T> (1);
	root_group->getNode(0)->deserialize(filename);
	csb_root = root_group->getNode(0);
	current_level.push_back(root_group);
	depth++;

	while (depth <= csb_depth)
	{
		upper_level.clear();
		while(current_level.size() != 0)
		{
			upper_level.push_back(current_level.back());
			current_level.pop_back();
		}
		while (upper_level.size() != 0)
		{
			for (unsigned i = 0; i < upper_level.back()->getUsedNodes(); i++)
			{
				CCSBNodeGroup<T>* node_group = NULL;
				if (depth < csb_depth)
					node_group = new CCSBInternalNodeGroup<T> (); //(upper_level[i]->getNode(j)->getUsedKeys()+1);
				else
					node_group = new CCSBLeafNodeGroup<T> ();
				node_group->deserialize(filename);

				current_level.push_back(node_group);

				//set the parent-children relationship
				upper_level.back()->getNode(i)->setPointer(node_group);
				for (unsigned j = 0; j < node_group->getUsedNodes(); j++)
					node_group->getNode(j)->setFather(upper_level.back()->getNode(i));
			}
			upper_level.pop_back();
		}
		depth++;
	}
	// depth == csb_depth, the current_level collects all the leafNodeGroup of the CSBPlusTree
	// set the double linked list in the leaf layer
	leaf_header = current_level[0];
	leaf_tailer = current_level[current_level.size()-1];
	current_level[0]->setHeaderNG(NULL);
	current_level[current_level.size()-1]->setTailerNG(NULL);
	if (current_level.size() > 1)
	{
		current_level[0]->setTailerNG(current_level[1]);
		current_level[current_level.size()-1]->setHeaderNG(current_level[current_level.size()-2]);
		for (unsigned i = 1; i < current_level.size()-1; i++)
		{
			current_level[i]->setHeaderNG(current_level[i-1]);
			current_level[i]->setTailerNG(current_level[i+1]);
		}

	}
	return true;
}

template <typename T>
void CSBPlusTree<T>::printTree()
{
	cout << "\n\n---------------------Print the index tree---------------------\n";
	if (this->csb_root == NULL)
	{
		cout << "The index tree is empty!\n";
		return;
	}

	vector<CCSBNodeGroup<T>* > current_level;
	vector<CCSBNodeGroup<T>* > lower_level;
	current_level.clear();
	lower_level.clear();

	int depth = 1;
	//print the root layer and save the child node group in the lower_level
	cout << "---------------------Root Node (depth: " << 1 << ")---------------------\n";
	cout << "Root: " << "\t";
	cout << "Used keys: " << ((CCSBNode<T>*)(this->csb_root))->getUsedKeys() << endl;
	for (unsigned i = 0; i < this->csb_root->getUsedKeys(); i++)
		cout << ((CCSBNode<T>* )(this->csb_root))->getElement(i)._key << " ";
	cout << endl;
	lower_level.push_back(((CCSBNode<T>* )(this->csb_root))->getPointer());
	depth++;

	//print the rest layers
	CCSBNodeGroup<T>* cur_node_group = new CCSBNodeGroup<T>();
	while (depth <= this->csb_depth)
	{
		current_level.clear();
		while (lower_level.size() != NULL)
		{
			current_level.push_back(lower_level.back());
			lower_level.pop_back();
		}
		lower_level.clear();
		if (depth < this->csb_depth)
			cout << "\n\n---------------------Internal Layer (depth: " << depth++ << ")---------------------\n";
		else
			cout << "\n\n---------------------Leaf Layer (depth: " << depth++ << ")---------------------\n";
		cout << "Node Group Num: " << current_level.size() << endl;
		unsigned i = 0;
		while (current_level.size() != 0)
		{
			cout << "NodeGroup: " << i++ << "\t";
			cout << "Used nodes: " << current_level.back()->getUsedNodes() << endl;
			for (unsigned j = 0; j < current_level.back()->getUsedNodes(); j++)
			{
				lower_level.push_back(current_level.back()->getNode(j)->getPointer());
				cout << "Node: " << j << "\t";
				cout << "Used keys: " << ((CCSBNodeGroup<T>*)(current_level.back()))->getNode(j)->getUsedKeys() << endl;
				for (unsigned k = 0; k < ((CCSBNodeGroup<T>*)(current_level.back()))->getNode(j)->getUsedKeys(); k++)
				{
					if (depth > this->csb_depth)
					{
						cout << current_level.back()->getNode(j)->getElement(k)._key << " ";
						cout << "<" << current_level.back()->getNode(j)->getElement(k)._block_off << ", ";
						cout << current_level.back()->getNode(j)->getElement(k)._tuple_off << ">\t";
					}
					else
						cout << current_level.back()->getNode(j)->getElement(k)._key << "\t";
				}
				cout << endl;
			}
			current_level.pop_back();
		}
	}
	cout << "---------------------Print the index tree finished---------------------\n";
}


template <typename T>
void CSBPlusTree<T>::printDoubleLinkedList()
{
	cout << "\n\n---------------------Print the doubly linked list---------------------\n";
	if (this->csb_root == NULL)
	{
		cout << "The index tree is empty!\n";
		return;
	}

	if (this->leaf_header == NULL || this->leaf_tailer == NULL)
	{
		cout << "The double linked list is error!\n";
		return;
	}

	CCSBNodeGroup<T>* header = this->leaf_header;
	cout << "The forward linked list is: \n";
	while (header != NULL)
	{
		for (unsigned i = 0; i < header->getUsedNodes(); i++)
		{
			for (unsigned j = 0; j < ((CCSBNode<T>*)(header->getNode(i)))->getUsedKeys(); j++)
			{
				cout << "<" << ((CCSBNode<T>*)(header->getNode(i)))->getElement(j)._key << ", ";
				cout << ((CCSBNode<T>*)(header->getNode(i)))->getElement(j)._block_off << ", ";
				cout << ((CCSBNode<T>*)(header->getNode(i)))->getElement(j)._tuple_off << "> ";
			}
			cout << "\t";
		}
		header = header->getTailerNG();
	}
	cout << endl;
	CCSBNodeGroup<T>* tailer = this->leaf_tailer;
	cout << "The reverse linked list is: \n";
	while (tailer != NULL)
	{
		for (int i = tailer->getUsedNodes()-1; i >= 0; i--)
		{
			for (int j = ((CCSBNode<T>*)(tailer->getNode(i)))->getUsedKeys()-1; j >= 0; j--)
			{
				cout << "<" << ((CCSBNode<T>*)(tailer->getNode(i)))->getElement(j)._key << ", ";
				cout << ((CCSBNode<T>*)(tailer->getNode(i)))->getElement(j)._block_off << ", ";
				cout << ((CCSBNode<T>*)(tailer->getNode(i)))->getElement(j)._tuple_off << "> ";
			}
			cout << "\t";
		}
		tailer = tailer->getHeaderNG();
	}
	cout << endl;
}
