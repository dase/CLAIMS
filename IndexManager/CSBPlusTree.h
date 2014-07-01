/*
 * CSBPlusTree.h
 *
 *  Created on: 2013年12月27日
 *      Author: SCDONG
 */

#ifndef CSBPLUSTREE_H_
#define CSBPLUSTREE_H_

#include <iostream>
#include <vector>
#include <assert.h>
#include <map>
#include "../configure.h"
#include <math.h>
#include <malloc.h>
#include <string.h>
using namespace std;


#define NULL 0
#define INVALID -1
#define FLAG_LEFT 1
#define FLAG_RIGHT 2

typedef unsigned short index_offset;

enum comparison {EQ, L, LEQ, G, GEQ};

//struct for the offset of the real data: leaf node's element struct
template <typename T>
struct data_offset
{
	data_offset<T>():_key(INVALID), _block_off(INVALID), _tuple_off(INVALID){};
	T _key;
	index_offset _block_off;
	index_offset _tuple_off;
};

//pre-declare of the node_group class
template <typename T>
class CCSBNodeGroup;

//CSB+ Node
template <typename T>
class CCSBNode
{
public:
	CCSBNode():used_keys(0), p_father(NULL){}

	virtual ~CCSBNode();

	//the get and set function for node_type, node_inner_key_count, node's_father
	int getUsedKeys() {return used_keys;}
	virtual bool setUsedKeys(int count) { assert(false); }
	CCSBNode<T>* getFather() { return p_father;}
	void setFather(CCSBNode<T>* father) { p_father = father; }

	static unsigned getMaxKeys() { return (cacheline_size-8)/sizeof(T); }
	static unsigned getMaxDatas() { return cacheline_size/sizeof(data_offset<T>); }

	//for the leaf nodes: get or set a certain data
	virtual data_offset<T> getElement(unsigned i) { assert(false); }
	virtual bool setElement(unsigned i, data_offset<T> value) { assert(false); }

	//for the internal nodes: get or set a certain key
//	virtual T getElement(unsigned i) {};
	virtual bool setElement(unsigned i, T value) { assert(false); }
	//for the internal nodes: get or set the child's pointer
	virtual CCSBNodeGroup<T>* getPointer() { assert(false); }
	virtual void setPointer(CCSBNodeGroup<T>* pointer) { assert(false); }

	//delete the child nodes
	virtual void DeleteChildren() { assert(false); }

	//operations for a index nodes
	virtual bool Insert(T key) { assert(false); }	//for internal node
	virtual bool Insert(data_offset<T> value) { assert(false); }	//for leaf node
	virtual bool Delete(T value) { assert(false); }
	virtual T SplitInsert(CCSBNode<T>* pNode, T key) { assert(false); }	//internal
	virtual T SplitInsert(CCSBNode<T>* pNode, data_offset<T> data) { assert(false); }	//leaf
	virtual bool Combine(CCSBNode<T>* pNode) { assert(false); }
	virtual bool serialize(FILE* filename) { assert(false); }
	virtual bool deserialize(FILE* filename) { assert(false); }
public:
	int used_keys;  //number of datas/keys in the node
	CCSBNode<T>* p_father;  //father pointer

};

//structure for the internal node
template <typename T>
class CCSBInternalNode : public CCSBNode<T>
{
public:
	CCSBInternalNode(){
		CCSBNode<T>();
		node_keys = new T [this->getMaxKeys()];
		for (unsigned i = 0; i < this->getMaxKeys(); i++)
			node_keys[i] = INVALID;
		p_child_node_group = NULL;
	}
	virtual ~CCSBInternalNode();

	bool setUsedKeys(int count)
	{
		if (count <= this->getMaxKeys())
		{
			this->used_keys = count;
			return true;
		}
		return false;
	}
	//get or set a certain key
	data_offset<T> getElement(unsigned i) {
		data_offset<T> ret;
		ret._block_off = INVALID;
		ret._tuple_off = INVALID;
		if ((i >= 0 ) && (i < this->getMaxKeys()))
			ret._key = node_keys[i];
		else
			ret._key = INVALID;
		return ret;
	}
	bool setElement(unsigned i, T key) {
		if ((i >= 0 ) && (i < this->getMaxKeys()))
		{
			node_keys[i] = key;
			return true;
		}
		else
		{
			cout << "[ERROR: CSBPlusTree.h<CCSBInternalNode>->setElement()]: The offset i: " << i << " is invalid!\n";
			return false;
		}
	}

	//get or set the child's pointer
	CCSBNodeGroup<T>* getPointer()
	{
		return p_child_node_group;
	}
	void setPointer(CCSBNodeGroup<T>* pointer)
	{
		p_child_node_group = pointer;
	}

	bool Insert(T key);
	bool Delete(T value);
	T SplitInsert(CCSBNode<T>* pNode, T key);
	bool Combine(CCSBNode<T>* pNode);
	void DeleteChildren();
//	// 从另一结点移一个元素到本结点
//	bool MoveOneElement(CCSBNode<T>* pNode);
	bool serialize(FILE* filename);
	bool deserialize(FILE* filename);

public:
	T* node_keys;  //array for the keys
	CCSBNodeGroup<T>* p_child_node_group;  //the pointer for the child nodes group

};

//structure for the leaf node
template <typename T>
class CCSBLeafNode : public CCSBNode<T>
{
public:
	CCSBLeafNode();
	virtual ~CCSBLeafNode();

	bool setUsedKeys(int count)
	{
		if (count <= this->getMaxDatas())
		{
			this->used_keys = count;
			return true;
		}
		return false;
	}
	//get or set a certain data
	data_offset<T> getElement(unsigned i)
	{
		if ((i >= 0 ) && (i < this->getMaxDatas()))
			return node_datas[i];
		else
		{
			cout << "[ERROR: CSBPlusTree.h<CCSBLeafNode>->getElement()]: The offset i: " << i << " is invalid!	return NULL\n";
			data_offset<T> * ret = NULL;
			return *ret;
		}
	}

	bool setElement(unsigned i, data_offset<T> data)
	{
		if ((i >= 0 ) && (i < this->getMaxDatas()))
		{
			node_datas[i]._key = data._key;
			node_datas[i]._block_off = data._block_off;
			node_datas[i]._tuple_off = data._tuple_off;
			return true;
		}
		else
		{
			cout << "[ERROR: CSBPlusTree.h<CCSBLeafNode>->setElement()]: The offset i: " << i << " is invalid!\n";
			return false;
		}
	}

	CCSBNodeGroup<T>* getPointer()
	{
		return NULL;
	}

	bool Insert(data_offset<T> value);
	bool Delete(T value);
	T SplitInsert(CCSBNode<T>* pNode, data_offset<T> data);
	bool Combine(CCSBNode<T>* pNode);
	void DeleteChildren();

	bool serialize(FILE* filename);
	bool deserialize(FILE* filename);

public:
	data_offset<T>* node_datas;

};

//base structure of Node Group
template <typename T>
class CCSBNodeGroup
{
public:
	CCSBNodeGroup(): used_nodes(0) {}
	CCSBNodeGroup(unsigned n): used_nodes(n) {}
	~CCSBNodeGroup() {};

	unsigned getUsedNodes() { return used_nodes; }
	void nodeCountIncrement() { used_nodes++; }

	virtual CCSBNodeGroup<T>* getHeaderNG() { assert(false); };
	virtual void setHeaderNG(CCSBNodeGroup<T>* header) { assert(false); };
	virtual CCSBNodeGroup<T>* getTailerNG() { assert(false); };
	virtual void setTailerNG(CCSBNodeGroup<T>* tailer) { assert(false); };

	inline virtual bool setUsedNodes(unsigned n) { assert(false); };
	virtual CCSBNode<T>* getNode(unsigned i) { assert(false); };
	virtual void setNode(unsigned i, CCSBNode<T>* node) { assert(false); };

	virtual void DeleteChildren() { assert(false); }

	virtual bool serialize(FILE* filename) { assert(false); }
	virtual bool deserialize(FILE* filename) { assert(false); }
public:
	unsigned used_nodes;

};

//sturcture of internal node group
template <typename T>
class CCSBInternalNodeGroup :public CCSBNodeGroup<T> {
public:
	CCSBInternalNodeGroup(): internal_nodes(NULL) {}
	CCSBInternalNodeGroup(unsigned n){
		internal_nodes = (CCSBNode<T>**) new CCSBInternalNode<T>* [n];
		for (unsigned i = 0; i < n; i++)
			internal_nodes[i] = new CCSBInternalNode<T> ();
		this->setUsedNodes(n);
	}
	~CCSBInternalNodeGroup();

	inline bool setUsedNodes(unsigned n)
	{
		if (n <= CCSBNode<T>::getMaxKeys()+1)
		{
			this->used_nodes = n;
			return true;
		}
		return false;
	}
	CCSBNode<T>* getNode(unsigned i)
	{
		if (i > this->used_nodes)
		{
			cout << "[ERROR: CSBPlusTree.h<CCSBInternalNodeGroup>->getNode()]: The offset i: " << i << " is invalid!\n";
			return NULL;
		}
		return internal_nodes[i];
	}
	void setNode(unsigned i, CCSBNode<T>* node)
	{
		for (unsigned j = 0; j < node->getUsedKeys(); j++)
			internal_nodes[i]->setElement(j, node->getElement(j)._key);
		internal_nodes[i]->setUsedKeys(node->getUsedKeys());
		internal_nodes[i]->setFather(node->getFather());
		internal_nodes[i]->setPointer(node->getPointer());
	}

	void DeleteChildren();

	bool serialize(FILE* filename);
	bool deserialize(FILE* filename);

public:
	CCSBNode<T>** internal_nodes;

};

//sturcture of leaf node group
template <typename T>
class CCSBLeafNodeGroup :public CCSBNodeGroup<T> {
public:
	CCSBLeafNodeGroup(): leaf_nodes(NULL), p_header(NULL), p_tailer(NULL) { this->setUsedNodes(0); }
	CCSBLeafNodeGroup(unsigned n);
	~CCSBLeafNodeGroup();

	CCSBNodeGroup<T>* getHeaderNG() { return p_header; }
	void setHeaderNG(CCSBNodeGroup<T>* header) { p_header = header; }
	CCSBNodeGroup<T>* getTailerNG() { return p_tailer; }
	void setTailerNG(CCSBNodeGroup<T>* tailer) { p_tailer = tailer; }

	inline bool setUsedNodes(unsigned n)
	{
		if (n <= CCSBNode<T>::getMaxKeys()+1)
		{
			this->used_nodes = n;
			return true;
		}
		return false;
	}
	CCSBNode<T>* getNode(unsigned i)
	{
		if (i > this->used_nodes)
		{
			cout << "[ERROR: CSBPlusTree.h<CCSBLeafNodeGroup>->getNode()]: The offset i: " << i << " is invalid!\n";
			return NULL;
		}
		return leaf_nodes[i];
	}
	void setNode(unsigned i, CCSBNode<T>* node)
	{
		for (unsigned j = 0; j < node->getUsedKeys(); j++)
			leaf_nodes[i]->setElement(j, node->getElement(j));
		leaf_nodes[i]->setUsedKeys(node->getUsedKeys());
		leaf_nodes[i]->setUsedKeys(node->getUsedKeys());
		leaf_nodes[i]->setFather(node->getFather());
	}

	void DeleteChildren();
	bool serialize(FILE* filename);
	bool deserialize(FILE* filename);

public:
	CCSBNode<T>** leaf_nodes;
	CCSBNodeGroup<T>* p_header;
	CCSBNodeGroup<T>* p_tailer;

};

template <typename T>
class CSBPlusTree {
public:
	CSBPlusTree():csb_root(NULL), csb_depth(0), leaf_header(NULL), leaf_tailer(NULL)
	{
		max_keys = CCSBNode<T>::getMaxKeys();
		max_datas = CCSBNode<T>::getMaxDatas();
	}
	virtual ~CSBPlusTree();
	//bulkload indexing
	void BulkLoad(data_offset<T>* aray, unsigned aray_num) {
		if (aray_num == 0)
				return;
			//just one CSBNode
			else if (aray_num <= max_datas)
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
				int leaf_node_group_num = ceil((double)(ceil((double)(aray_num)/(double)(max_datas)))/(double)(max_keys+1));
				CCSBNodeGroup<T>** leaf = (CCSBNodeGroup<T>**) new CCSBLeafNodeGroup<T>* [leaf_node_group_num];

				//allocate the parent_key_array of the leaf_node_group
				int internal_key_array_num = ceil((double)aray_num/(double)max_datas)-1;
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
				if (internal_key_array_num <= max_keys)
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
					int internal_node_group_num = ceil((double)leaf_node_group_num/(double)(max_keys+1));
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
					while (upper_internal_key_array_num > max_keys)
					{
						//释放已经建立索引的internalAray数据，将其上层索引in2Aray赋给该指针准备再次建立索引
						delete[] internal_key_array;
						internal_key_array = upper_internal_key_array;
						internal_key_array_num = upper_internal_key_array_num;
						leaf_node_group_num = internal_node_group_num;
						CCSBNodeGroup<T>** leaf = internal;

						//申请足够的中间结点group空间，注：上层索引node的个数即为下层的nodegroup数
						internal_node_group_num = ceil((double)leaf_node_group_num/(double)(max_keys+1));
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
					if (upper_internal_key_array_num <= max_keys)
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
	};
	//search certain records according to the key
	map<index_offset, vector<index_offset>* >* Search(T key);
	map<index_offset, vector<index_offset>* >* rangeQuery(T lower_key, T upper_key);
	map<index_offset, vector<index_offset>* >* rangeQuery(T lower_key, comparison comp_lower, T upper_key, comparison comp_upper){
		map<index_offset, vector<index_offset>* >* ret = new map<index_offset, vector<index_offset>* >;
		ret->clear();
		//For point query
		if (comp_lower == EQ)
		{
			if (lower_key != upper_key)
				cout << "[ERROR: CSBPlusTree.cpp->rangeQuery()]: For the equal point query, the lower_key " << lower_key << " != the upper_key " << upper_key << endl;
			return Search(lower_key);
		}
		// Range Query
		else if ((!(comp_lower == G || comp_lower == GEQ)) || (!(comp_upper == L || comp_upper == LEQ)))
		{
			cout << "[ERROR: CSBPlusTree.cpp->rangeQuery()]: For the range query, the given two key isn't a range\n";
			return ret;
		}
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
			if (comp_lower == GEQ)
				for (i = 0; (lower_key > search_node->getElement(i)._key)&&(i < search_node->used_keys); i++);
			else if (comp_lower == G)
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
			if (comp_lower == GEQ)
			{
				if (lower_key <= search_node->getElement(i)._key)
				{
	/*for testing*/				found = true;
					break;
				}
			}
			if (comp_lower == G)
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
			if (comp_lower == GEQ && comp_upper == LEQ)
			{
				for (; i < search_node->getUsedKeys(); i++)
				{
					if ((lower_key <= search_node->getElement(i)._key) && upper_key >= search_node->getElement(i)._key)
					{
						index_offset tmp = search_node->getElement(i)._block_off;
						if (ret->find(tmp) == ret->end())
							(*ret)[tmp] = new vector<index_offset>;
						(*ret)[tmp]->push_back(search_node->getElement(i)._tuple_off);
					}
	//					ret[search_node->getElement(i)._block_off].push_back(search_node->getElement(i)._tuple_off);
					else
						return ret;
				}
			}
			else if (comp_lower == GEQ && comp_upper == L)
			{
				for (; i < search_node->getUsedKeys(); i++)
				{
					if ((lower_key <= search_node->getElement(i)._key) && upper_key > search_node->getElement(i)._key)
					{
						index_offset tmp = search_node->getElement(i)._block_off;
						if (ret->find(tmp) == ret->end())
							(*ret)[tmp] = new vector<index_offset>;
						(*ret)[tmp]->push_back(search_node->getElement(i)._tuple_off);
					}
	//					ret[search_node->getElement(i)._block_off].push_back(search_node->getElement(i)._tuple_off);
					else
						return ret;
				}
			}
			else if (comp_lower == G && comp_upper == LEQ)
			{
				for (; i < search_node->getUsedKeys(); i++)
				{
					if ((lower_key < search_node->getElement(i)._key) && upper_key >= search_node->getElement(i)._key)
					{
						index_offset tmp = search_node->getElement(i)._block_off;
						if (ret->find(tmp) == ret->end())
							(*ret)[tmp] = new vector<index_offset>;
						(*ret)[tmp]->push_back(search_node->getElement(i)._tuple_off);
					}
	//					ret[search_node->getElement(i)._block_off].push_back(search_node->getElement(i)._tuple_off);
					else
						return ret;
				}
			}
			else if (comp_lower == G && comp_upper == L)
			{
				for (; i < search_node->getUsedKeys(); i++)
				{
					if ((lower_key < search_node->getElement(i)._key) && upper_key > search_node->getElement(i)._key)
					{
						index_offset tmp = search_node->getElement(i)._block_off;
						if (ret->find(tmp) == ret->end())
							(*ret)[tmp] = new vector<index_offset>;
						(*ret)[tmp]->push_back(search_node->getElement(i)._tuple_off);
					}
	//					ret[search_node->getElement(i)._block_off].push_back(search_node->getElement(i)._tuple_off);
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
			if (comp_lower == GEQ && comp_upper == LEQ)
			{
				for (; j < search_node_group->getUsedNodes(); j++)
				{
					for (; i < search_node_group->getNode(j)->getUsedKeys(); i++)
					{
						if ((lower_key <= search_node_group->getNode(j)->getElement(i)._key) && (upper_key >= search_node_group->getNode(j)->getElement(i)._key))
						{
							index_offset tmp = search_node_group->getNode(j)->getElement(i)._block_off;
							if (ret->find(tmp) == ret->end())
								(*ret)[tmp] = new vector<index_offset>;
							(*ret)[tmp]->push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
						}
	//						ret[search_node_group->getNode(j)->getElement(i)._block_off].push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
						else
							return ret;
					}
					i = 0;
				}
				i = 0;
				j = 0;
				search_node_group = search_node_group->getTailerNG();
			}
			else if (comp_lower == GEQ && comp_upper == L)
			{
				for (; j < search_node_group->getUsedNodes(); j++)
				{
					for (; i < search_node_group->getNode(j)->getUsedKeys(); i++)
					{
						if ((lower_key <= search_node_group->getNode(j)->getElement(i)._key) && (upper_key > search_node_group->getNode(j)->getElement(i)._key))
						{
							index_offset tmp = search_node_group->getNode(j)->getElement(i)._block_off;
							if (ret->find(tmp) == ret->end())
								(*ret)[tmp] = new vector<index_offset>;
							(*ret)[tmp]->push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
						}
	//						ret[search_node_group->getNode(j)->getElement(i)._block_off].push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
						else
							return ret;
					}
					i = 0;
				}
				i = 0;
				j = 0;
				search_node_group = search_node_group->getTailerNG();
			}
			else if (comp_lower == G && comp_upper == LEQ)
			{
				for (; j < search_node_group->getUsedNodes(); j++)
				{
					for (; i < search_node_group->getNode(j)->getUsedKeys(); i++)
					{
						if ((lower_key < search_node_group->getNode(j)->getElement(i)._key) && (upper_key >= search_node_group->getNode(j)->getElement(i)._key))
						{
							index_offset tmp = search_node_group->getNode(j)->getElement(i)._block_off;
							if (ret->find(tmp) == ret->end())
								(*ret)[tmp] = new vector<index_offset>;
							(*ret)[tmp]->push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
						}
	//						ret[search_node_group->getNode(j)->getElement(i)._block_off].push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
						else
							return ret;
					}
					i = 0;
				}
				i = 0;
				j = 0;
				search_node_group = search_node_group->getTailerNG();
			}
			else if (comp_lower == G && comp_upper == L)
			{
				for (; j < search_node_group->getUsedNodes(); j++)
				{
					for (; i < search_node_group->getNode(j)->getUsedKeys(); i++)
					{
						if ((lower_key < search_node_group->getNode(j)->getElement(i)._key) && (upper_key > search_node_group->getNode(j)->getElement(i)._key))
						{
							index_offset tmp = search_node_group->getNode(j)->getElement(i)._block_off;
							if (ret->find(tmp) == ret->end())
								(*ret)[tmp] = new vector<index_offset>;
							(*ret)[tmp]->push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
						}
	//						ret[search_node_group->getNode(j)->getElement(i)._block_off].push_back(search_node_group->getNode(j)->getElement(i)._tuple_off);
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
	//insert a record
	bool Insert(data_offset<T> data);
	//delete the records according to the key
	bool Delete(T key);
	//save the index structure to disk
	bool serialize(FILE* filename);
	bool deserialize(FILE* filename);

	//for testing
	void printTree();
	void printDoubleLinkedList();

public:
	//the leaf nodes list
	CCSBNodeGroup<T>* leaf_header;
	CCSBNodeGroup<T>* leaf_tailer;
	//root of the index CSB+ Tree
	CCSBNode<T>* csb_root;
	//depth of the index CSB+ Tree
	unsigned csb_depth;

private:
	unsigned max_keys;
	unsigned max_datas;

private:

	//将aray填入leafNodeGroup中，并记录上层索引key
	int makeLeafNodeGroup(data_offset<T>* aray, unsigned aray_num, CCSBNodeGroup<T>** leaf, T* internal_key_array);
	//建立最底层索引，直接索引leafNodeGroup层，并记录其上层索引key
	int makeInternalNodeGroup(T* internalAray, int iArayNo, CCSBNodeGroup<T>** internal, int leafNGNo, T* in2Aray, CCSBNodeGroup<T>** leaf);
//	//循环建立中间索引层
//	int makeInternal2NodeGroup(T* internalAray, int iArayNo, CCSBNodeGroup<T>** internal, int leafNGNo, T* in2Aray, CCSBNodeGroup<T>** leaf);
	// 为插入而查找叶子结点
	CCSBNode<T>* SearchLeafNode(T data);
	// 插入键到中间结点
	bool InsertInternalNode(CCSBInternalNode<T>* pNode, T key, CCSBNode<T>* pRightSon);
	// 在中间结点中删除键
	bool DeleteInternalNode(CCSBInternalNode<T>* pNode, T key);
	// 清除树
	void ClearTree();
};

#endif /* CSBPLUSTREE_H_ */
