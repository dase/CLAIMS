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
using namespace std;

enum NODE_TYPE
{
	NODE_TYPE_ROOT = 1,  //root node
	NODE_TYPE_INTERNAL = 2,  //internal node
	NODE_TYPE_LEAF = 3,  //leaf node
};

#define NULL 0
#define INVALID -1
#define FLAG_LEFT 1
#define FLAG_RIGHT 2

#define CSB_MAXNUM_KEY 4	//12
#define CSB_MAXNUM_DATA 4	//12
#define CSB_ORDER_V 2		//6

typedef unsigned short index_offset;

//struct for the offset of the real data: leaf node's element struct
template <typename T>
struct data_offset
{
	data_offset<T>():_key(INVALID), _block_off(INVALID), _tuple_off(INVALID){};
	T _key;
	index_offset _block_off;
	index_offset _tuple_off;
};

struct search_result
{
	search_result():_block_off(INVALID), _tuple_off(INVALID) {};
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
	CCSBNode();
	CCSBNode(NODE_TYPE type);

	virtual ~CCSBNode();

	//the get and set function for node_type, node_inner_key_count, node's_father
	NODE_TYPE getType() {return node_type;}
	void setType(NODE_TYPE type) {node_type = type;}
	int getUsedKeys() {return used_keys;}
	virtual bool setUsedKeys(int count) {};
	CCSBNode* getFather() { return p_father;}
	void setFather(CCSBNode* father) { p_father = father; }

	//for the leaf nodes: get or set a certain data
	virtual data_offset<T> getElement(unsigned i) {};
	virtual bool setElement(unsigned i, data_offset<T> value) {};

	//for the internal nodes: get or set a certain key
//	virtual T getElement(unsigned i) {};
	virtual bool setElement(unsigned i, T value) {};
	//for the internal nodes: get or set the child's pointer
	virtual CCSBNodeGroup<T>* getPointer() {};
	virtual void setPointer(CCSBNodeGroup<T>* pointer) {};

	// 获取一个最近的兄弟结点
//	virtual CCSBNode* GetBrother(int& flag);

	//delete the child nodes
	virtual void DeleteChildren() {};

	//operations for a index nodes
	virtual bool Insert(T key) {};	//for internal node
	virtual bool Insert(data_offset<T> value) {};	//for leaf node
	virtual bool Delete(T value) {};
	virtual T SplitInsert(CCSBNode<T>* pNode, T key) {};	//internal
	virtual T SplitInsert(CCSBNode<T>* pNode, data_offset<T> data) {};	//leaf
	virtual bool Combine(CCSBNode<T>* pNode) {};
public:
	NODE_TYPE node_type;  //node type
	int used_keys;  //number of datas/keys in the node
	CCSBNode* p_father;  //father pointer
};

//structure for the internal node
template <typename T>
class CCSBInternalNode : public CCSBNode<T>
{
public:
	CCSBInternalNode();
	virtual ~CCSBInternalNode();

	bool setUsedKeys(int count)
	{
		if (count <= CSB_MAXNUM_KEY)
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
		if ((i >= 0 ) && (i < CSB_MAXNUM_KEY))
			ret._key = node_keys[i];
		else
			ret._key = INVALID;
		return ret;
	}
	bool setElement(unsigned i, T key) {
		if ((i >= 0 ) && (i < CSB_MAXNUM_KEY))
		{
			node_keys[i] = key;
			return true;
		}
		else
		{
			cout << "the offset i: " << i << "is invalid!\n";
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
public:
	T node_keys[CSB_MAXNUM_KEY];  //array for the keys
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
		if (count <= CSB_MAXNUM_DATA)
		{
			this->used_keys = count;
			return true;
		}
		return false;
	}
	//get or set a certain data
	data_offset<T> getElement(unsigned i)
	{
		if ((i >= 0 ) && (i < CSB_MAXNUM_DATA))
			return node_datas[i];
		else
		{
			cout << "the offset i: " << i << "is invalid!	return NULL\n";
			data_offset<T> * ret = NULL;
			return *ret;
		}
	}

	bool setElement(unsigned i, data_offset<T> data)
	{
		if ((i >= 0 ) && (i < CSB_MAXNUM_DATA))
		{
			node_datas[i]._key = data._key;
			node_datas[i]._block_off = data._block_off;
			node_datas[i]._tuple_off = data._tuple_off;
			return true;
		}
		else
		{
			cout << "the offset i: " << i << "is invalid!\n";
			return false;
		}
	}

	bool Insert(data_offset<T> value);
	bool Delete(T value);
	T SplitInsert(CCSBNode<T>* pNode, data_offset<T> data);
	bool Combine(CCSBNode<T>* pNode);
	void DeleteChildren();

public:
	data_offset<T> node_datas[CSB_MAXNUM_DATA];
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
	virtual CCSBNodeGroup<T>* getHeaderNG() {};
	virtual void setHeaderNG(CCSBNodeGroup<T>* header) {};
	virtual CCSBNodeGroup<T>* getTailerNG() {};
	virtual void setTailerNG(CCSBNodeGroup<T>* tailer) {};

	inline virtual bool setUsedNodes(unsigned n) {};
	virtual CCSBNode<T>* getNode(unsigned i) {};
	virtual void setNode(unsigned i, CCSBNode<T>* node) {};
public:
	unsigned used_nodes;
};

//sturcture of internal node group
template <typename T>
class CCSBInternalNodeGroup :public CCSBNodeGroup<T> {
public:
	CCSBInternalNodeGroup(): internal_nodes(NULL) {}
	CCSBInternalNodeGroup(unsigned n);
	~CCSBInternalNodeGroup();

	inline bool setUsedNodes(unsigned n)
	{
		if (n <= CSB_MAXNUM_KEY+1)
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
			cout << "the offset i: " << i << "is invalid!\n";
			return NULL;
		}
		return internal_nodes[i];
	}
	void setNode(unsigned i, CCSBNode<T>* node)
	{
		for (unsigned j = 0; j < node->getUsedKeys(); j++)
			internal_nodes[i]->setElement(j, node->getElement(j)._key);
		internal_nodes[i]->setType(node->getType());
		internal_nodes[i]->setUsedKeys(node->getUsedKeys());
		internal_nodes[i]->setFather(node->getFather());
		internal_nodes[i]->setPointer(node->getPointer());
	}
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
		if (n <= CSB_MAXNUM_DATA+1)
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
			cout << "the offset i: " << i << "is invalid!\n";
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
public:
	CCSBNode<T>** leaf_nodes;
	CCSBNodeGroup<T>* p_header;
	CCSBNodeGroup<T>* p_tailer;
};

template <typename T>
class CSBPlusTree {
public:
	CSBPlusTree();
	virtual ~CSBPlusTree();
	//bulkload indexing
	void BulkLoad(data_offset<T>* aray, unsigned arayNo);
	//search certain records according to the key
	vector<search_result*> Search(T key);
	vector<search_result*> rangeQuery(T lower_key, T upper_key);
	//insert a record
	bool Insert(data_offset<T> data);
	//delete the records according to the key
	bool Delete(T key);

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
	void ClearTree() {};
};

#endif /* CSBPLUSTREE_H_ */
