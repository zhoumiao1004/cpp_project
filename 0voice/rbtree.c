


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED				1
#define BLACK 			2

typedef int KEY_TYPE;

typedef struct _rbtree_node {
	unsigned char color;
	struct _rbtree_node *right;
	struct _rbtree_node *left;
	struct _rbtree_node *parent;
	KEY_TYPE key;
	void *value;
} rbtree_node;

// 树结构抽象，保存根节点
typedef struct _rbtree {
	rbtree_node *root;
	rbtree_node *nil;  // 自定义叶子节点，代替NULL
} rbtree;

/* 遍历树获取最小节点 */
rbtree_node *rbtree_mini(rbtree *T, rbtree_node *x) {
	while (x->left != T->nil) {
		x = x->left;
	}
	return x;
}

/* 遍历树获取最大节点 */
rbtree_node *rbtree_maxi(rbtree *T, rbtree_node *x) {
	while (x->right != T->nil) {
		x = x->right;
	}
	return x;
}

/* 找到x的后继，即中序遍历x的下一个结点 */
rbtree_node *rbtree_successor(rbtree *T, rbtree_node *x) {
	rbtree_node *y = x->parent;

	if (x->right != T->nil) {
		// x的右子树不为空，后继结点即为右子树的最左侧最小的结点
		return rbtree_mini(T, x->right);
	}

	while ((y != T->nil) && (x == y->right)) {
		x = y;
		y = y->parent;
	}
	return y;
}

/* 左旋，修改3对指向 */
void rbtree_left_rotate(rbtree *T, rbtree_node *x) {
	/* 右斜红(x)->黑(y)->红 左旋后x成为y的左孩子 */
	rbtree_node *y = x->right;  // x  --> y  ,  y --> x,   right --> left,  left --> right

	/* x要做y的左孩子，第1对：修改x和y左孩子的指向 */
	x->right = y->left; //1 1
	if (y->left != T->nil) { //1 2
		y->left->parent = x;
	}

	/* y要取代x的位置，第2对：修改x的父结点和y的指向 */
	y->parent = x->parent; //1 3
	if (x->parent == T->nil) { //1 4
		T->root = y;
	} else if (x == x->parent->left) {
		x->parent->left = y;
	} else {
		x->parent->right = y;
	}

	/* x要做y的左孩子，第3对：修改x和y的指向 */
	y->left = x; //1 5
	x->parent = y; //1 6
}

/* 右旋 */
void rbtree_right_rotate(rbtree *T, rbtree_node *y) {

	rbtree_node *x = y->left;

	y->left = x->right;
	if (x->right != T->nil) {
		x->right->parent = y;
	}

	x->parent = y->parent;
	if (y->parent == T->nil) {
		T->root = x;
	} else if (y == y->parent->right) {
		y->parent->right = x;
	} else {
		y->parent->left = x;
	}

	x->right = y;
	y->parent = x;
}

/* 新增节点调整 */
void rbtree_insert_fixup(rbtree *T, rbtree_node *z) {
	/* 父节点为红结点才需要调整 */
	while (z->parent->color == RED) { //z ---> RED
		/* 思路都是先转换成一条线：左斜或右斜 红->黑->红，左斜右旋转，右斜左旋转平衡黑高 */
		if (z->parent == z->parent->parent->left) {
			/* 父节点左斜 */
			rbtree_node *y = z->parent->parent->right;  // 找到叔结点
			if (y->color == RED) {
				// 叔结点红色 --> 变色
				z->parent->color = BLACK;  // 父节点 -> 黑
				y->color = BLACK;  // 叔结点 -> 黑
				z->parent->parent->color = RED;  // 祖父结点 --> 红

				z = z->parent->parent; //z --> RED
			} else {
				// 叔结点黑色/nil（父节点红->祖父结点黑） --> 旋转
				if (z == z->parent->right) {
					// 右插 --> 通过左旋转换为左插统一处理
					z = z->parent;
					rbtree_left_rotate(T, z);  //新节点上位，原父节点成左儿子
				}
				// 先变变色（交换父节点和祖父结点颜色）-> 新结点和父节点不连续红结点了
				z->parent->color = BLACK;
				z->parent->parent->color = RED;  // 祖父结点变红了，影响叔结点那边的黑高
				// 以祖父结点为支点右旋（左斜：红->黑->红 左边黑高大了，右旋平衡）
				rbtree_right_rotate(T, z->parent->parent);
			}
		}else {
			/* 父节点右斜 */
			rbtree_node *y = z->parent->parent->left;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;

				z = z->parent->parent; //z --> RED
			} else {
				if (z == z->parent->left) {
					// 左插 --> 通过右旋转换为右插统一处理
					z = z->parent;
					rbtree_right_rotate(T, z);
				}

				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rbtree_left_rotate(T, z->parent->parent);
			}
		}
		
	}

	T->root->color = BLACK;
}

/* 插入新节点z */
void rbtree_insert(rbtree *T, rbtree_node *z) {

	rbtree_node *y = T->nil;
	rbtree_node *x = T->root;

	// 利用rbtree的有序性找到需要插入的位置，退出循环时x走到叶子节点，y就是待插入位置的父节点
	while (x != T->nil) {
		y = x;
		if (z->key < x->key) {
			x = x->left;
		} else if (z->key > x->key) {
			x = x->right;
		} else { //Exist 已存在重复值，不插入
			return ;
		}
	}

	z->parent = y;
	if (y == T->nil) { // 空树，新增的结点就是根节点
		T->root = z;
	} else if (z->key < y->key) {  // 和父节点比较大小，大于父节点插右边，小于父节点插左边
		y->left = z;
	} else {
		y->right = z;
	}

	z->left