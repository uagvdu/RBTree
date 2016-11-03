#include<iostream>
#include<assert.h>
using namespace std;
enum Color
{
	RED,
	BLACK
};

template <class K,class V>
struct RBTNode
{
	K _key;
	V _value;
	RBTNode<K,V>* _left;
	RBTNode<K,V>* _right;
	RBTNode<K, V>* _parent;
	Color _col;
	RBTNode(K key,V value)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _col(RED)
	{}
};

template<class K,class V>
class RBT
{
	typedef RBTNode<K, V> Node;
public:
	RBT()
		:_root(NULL)
	{}

	void Inorder()
	{
		_Inorder(_root);
		cout << endl;
	}

	bool IsBlance()
	{
		if (_root == NULL)
			return true;
		if (_root->_col != BLACK)
			return false;
		int count = 0;
		Node* cur = _root;
		while (cur)//统计树的最左路黑节点的个数，作为其余路的基准
		{
			if (cur->_col == BLACK)
			{
				count++;
				cur = cur->_left;
			}
		}
		int num = 0;
		return _IsBlance(_root, count, num);
		
	}

	bool Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (cur->_key > key)
				cur = cur->_left;
			else if (cur->_key < key)
				cur = cur->_right;
			else
				return true;
		}
		return false;
	}


	bool Insert(const K& key, V value)
	{
		if (_root == NULL)
		{
			_root = new Node(key, value);
			_root->_col = BLACK;

			return true;
		}
		else
		{
			Node* cur = _root;
			Node* parent = cur->_parent;
			while (cur)
			{
				if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else if (cur->_key>key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else
					return false;
			}

			cur = new Node(key, value);
			cur->_parent = parent;
			Node* grandparent = NULL;
			if (parent->_key > cur->_key)
				parent->_left = cur;
			else
				parent->_right = cur;          //插入一个红节点


		

			while (cur != _root&&parent->_col == RED)            //cur!=_root的好处是parent一定存在
			{
				Node* uncle = NULL;                      //uncle 有两种可能为NULL，一种是grandparent为NULL，一种是grandparent的子节点为NULL
				Node* grandparent = parent->_parent;
				if (grandparent)
				{
					if (parent == grandparent->_left)
						uncle = grandparent->_right;
					else
						uncle = grandparent->_left;
				}

				if (uncle&&uncle->_col == RED)
				{
					grandparent->_col = RED;
					parent->_col = uncle->_col = BLACK;
					cur = grandparent;
					parent = cur->_parent;
				}
				else//uncle == NULL || uncle ->_col == black  此处的uncle = NULL，代表grandp不等于null
				{
					if (grandparent->_right==NULL||grandparent->_right==uncle)
					{
						if (cur == parent->_left)
						{
							_RotateR(grandparent);
							parent->_col = BLACK;
							grandparent->_col = RED;
						}
						else
						{
							_RotateLR(grandparent);
							cur->_col = BLACK;
							grandparent->_col = RED;
						}

					}
					else if (grandparent->_left==NULL||grandparent->_left==uncle)
					{
						if (cur == parent->_left)
						{
							_RotateRL(grandparent);
							cur->_col = BLACK;
							grandparent->_col = RED;
						}
						else
						{
							_RotateL(grandparent);
							parent->_col = BLACK;
							grandparent->_col = RED;
						}
					}
				}
			}
			if (_root->_col == RED)
				_root->_col = BLACK;
			return true;
		}
	}


protected:
	


	void _Getdel1()  //uncle ==parent->_right
	{
		if (cur == parent->_left) //parent->_col颜色未知
		{
			del = cur;
			Node *uncle = parent->_right;
			if (cur->_col == BLACK&&cur->_right != NULL)          //
			{

				parent->_left = del->_right;
				if (del->_right->_col == RED)  //del右孩子为红-》黑
				{
					del->right->_col = BLACK;
				}
				else                          //del右孩子为黑，看兄
				{
					if (uncle->_col == RED)
					{
						swap(uncle->_col, parent->_col);
						_RotateL(parent);//旋转之后的uncle与parent->_right不是同一个值
						swap(parent->_right->_col, parent->_col);

					}
					else                //黑兄       未知侄，未知父
					{
						if (uncle->_left->_col == BLACK&&uncle->_right->_col == BLACK) //情况二：左右侄子都为黑
						{
							if (parent->_col == BLACK)
							{
								uncle->_col = RED;
							}
							else
							{
								swap(parent->_col, parent->_right->_col);
							}
						}
						else if (uncle->_left->_col == RED&&uncle->_right->_col == BLACK)//情况三：左侄红右侄为黑
						{
							swap(uncle->_col, uncle->_left->_col);
							_RotateR(uncle);
							swap(uncle->_col, uncle->_parent->_col);
						}
						else if (uncle->_right->_col == RED)  //情况四：包含了右为红，或者为空
						{
							uncle = parent->_right;
							swap(uncle->_col, parent->_col);
							_RotateL(parent);
						}
					}

				}
			}
			else  //cur->_color = red || cur->right ==NULL
				parent->_left = del->_right;

		}
		else                         //cur  == parent->_right
		{
			del = cur;
			Node *uncle = parent->_left;
			if (cur->_col == BLACK&&cur->_right != NULL)          //
			{
				del = cur;
				parent->_right = del->_right;
				if (del->_right->_col == RED)  //del右孩子为红-》黑
				{
					del->_right->_col = BLACK;
					parent->_right = del->_right;
				}
				else                          //del右孩子为黑，看兄
				{
					if (uncle->_col == RED)
					{
						swap(uncle->_col, parent->_col);
						_RotateR(parent);//旋转之后的uncle与parent->_right不是同一个值
						swap(parent->_left->_col, parent->_col);

					}
					else                //黑兄       未知侄，未知父
					{
						if (uncle->_left->_col == BLACK&&uncle->_right->_col == BLACK) //情况二：左右侄子都为黑
						{
							if (parent->_col == BLACK)
							{
								uncle->_col = RED;
							}
							else
							{
								swap(parent->_col, parent->_left->_col);
							}
						}
						else if (uncle->_left->_col == RED&&uncle->_right->_col == BLACK)//情况三：左侄红右侄为黑
						{
							swap(uncle->_col, uncle->_left->_col);
							_RotateL(uncle);
							swap(uncle->_col, uncle->_parent->_col);
						}
						else if (uncle->_right->_col == RED)  //情况四：包含了右为红，或者为空
						{
							uncle = parent->_left;
							swap(uncle->_col, parent->_col);
							_RotateR(parent);
						}
					}

				}
			}
			else  //cur->_color = red
				parent->_right = del->_right;

		}
	}

	bool _IsBlance(Node* root,const int& count,int num)
	{
		if (root == NULL)
			return num == count;
		else
		{
			Node* cur = root;

			if ((cur->_col == RED)&&(cur->_parent->_col==RED))
			{
				return false;
			}
			if (cur->_col == BLACK)
			{
				num++;
			}

	    	return 	_IsBlance(cur->_left, count,num)&&_IsBlance(cur->_right,count,num);

		}
	}
	void _Inorder(Node* root)
	{
		if (!root)
		{
			return;
		}
		else
		{
			Node *cur = root;
			_Inorder(cur->_left);
			cout << cur->_key << " ";
			_Inorder(cur->_right);
		}
	}
	void _RotateR(Node* grandparent)
	{
		Node *ppNode = grandparent->_parent;
		Node *Lsub = grandparent->_left;
		Node *LRsub = Lsub->_right;
		Lsub->_right = grandparent;
		grandparent->_parent = Lsub;
		grandparent->_left = LRsub;

		if (LRsub)
		{
			LRsub->_parent = grandparent;
		}
		if (ppNode == NULL)
		{
			_root = Lsub;
			Lsub->_parent = NULL;

		}
		else
		{
			if (grandparent == ppNode->_left)
			{
				ppNode->_left = Lsub;
				Lsub->_parent = ppNode;
			}
			else
			{
				ppNode->_right = Lsub;
				Lsub->_parent = ppNode;
			}
		}

	}

	void _RotateL(Node* grandparent)
	{
		Node *Rsub = grandparent->_right;
		Node *RLsub = Rsub->_left;
		Node *ppNode = grandparent->_parent;
		Rsub->_left = grandparent;
		grandparent->_parent = Rsub;
		grandparent->_right = RLsub;
		if (RLsub)
		{
			RLsub->_parent = grandparent;
		}
		if (ppNode == NULL)
		{
			_root = Rsub;
			Rsub->_parent = NULL;
		}
		else
		{
			if (grandparent == ppNode->_left)
			{
				ppNode->_left = Rsub;
				Rsub->_parent = ppNode;
			}
				
			else
			{
				ppNode->_right = Rsub;
				Rsub->_parent = ppNode;

			}
		}
			
	}

	void _RotateRL(Node* grandparent)
	{
		Node *Rsub = grandparent->_right;
		_RotateR(Rsub);
		_RotateL(grandparent);

	}

	void _RotateLR(Node* grandparent)
	{
		Node *Lsub = grandparent->_left;
		_RotateL(Lsub);
		_RotateR(grandparent);
	}


protected:
	Node* _root;
};

void RBTtest()
{
	RBT<int, int> RB;
	int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	for (int i = 0; i < sizeof(a) / sizeof(a[0]);i++)
	{
		RB.Insert(a[i],0);
	}
	RB.Inorder();
	cout<<"Find: "<<RB.Find(7)<<endl;
	cout << "IsBlance: "<<RB.IsBlance() << endl;
	



}