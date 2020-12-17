


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

/* 左旋，修改3对指向
 * x变成右孩子的左孩子
 * 目的：左子树黑高小于右子树黑高时左旋，右子树的左子树高度平衡到左子树了
 */
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

	z->left = T->nil;
	z->right = T->nil;
	z->color = RED;

	rbtree_insert_fixup(T, z);
}

/* 删除x的黑父结点后重平衡黑高
 * 1.x红->直接变黑就平衡了删黑父的损失
 * 2.x黑-> 2.1.x是继父的左孩子
 *         2.2.x是继父的右孩子
 */
void rbtree_delete_fixup(rbtree *T, rbtree_node *x) {

	while ((x != T->root) && (x->color == BLACK)) {
		// x是黑结点
		if (x == x->parent->left) {
			// x是继父左孩子
			rbtree_node *w= x->parent->right;  // x右兄弟结点（原叔结点）
			
			if (w->color == RED) {
				/* 若右兄弟w是红色 -> x继父必是黑色 => 将右兄弟变为黑
				 * 删除节点情况1:被删节点是“黑+黑”节点，被删除的节点是左节点，被删节点的兄弟节点是红色。(此时被删节点的父节点和x的兄弟节点的子节点都是黑节点)。
				 * 	(01) 将x的兄弟节点设为“黑色”。
					(02) 将x的父节点设为“红色”。
					(03) 对x的父节点进行左旋。
					(04) 左旋后，重新设置x的兄弟节点。
				 */
				w->color = BLACK;  // 右兄弟变黑
				x->parent->color = RED;  // 继父变红

				rbtree_left_rotate(T, x->parent);  // x继父(红)变成x的右兄结点(黑)的左孩子
				w = x->parent->right;  // 左旋后，x兄弟结点（必是黑色）变了，w重新指向x的兄弟节点
			}

			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				/* 右兄弟w左右孩子都是黑色 -> w子树已经平衡了，比x子树黑高多1 -> w变红就可解决
					删除节点情况3:被删节点是“黑+黑”节点，被删节点的兄弟节点是黑色，被删节点的兄弟节点的两个孩子都是黑色。
					(01) 将被删节点的兄弟节点设为“红色”。
					(02) 设置“被删节点的父节点”为“新的被删节点节点”。
				 */
				w->color = RED;  // w变红色使和x分支黑高平衡
				x = x->parent;  // x指向继父
			} else {
				/* 删除节点情况4:将被删节点是“黑+黑”节点，被删节点的兄弟节点是黑色；将被删节点的兄弟节点的左孩子是红色，右孩子是黑色的。
				(01) 将被删节点兄弟节点的左孩子设为“黑色”。
				(02) 将被删节点兄弟节点设为“红色”。
				(03) 对被删节点的兄弟节点进行右旋。
				(04) 右旋后，重新设置被删节点的兄弟节点。
				 */
				// 为左旋做准备：需要左黑高>=右黑高
				// 3种情况：1.全红； 2.左红右黑 -> 通过右旋将左黑高做成>=右黑高；3.左黑右红
				if (w->right->color == BLACK) {
					w->left->color = BLACK;  // w左孩子从黑->红，虽然w变红左侧补偿了，但是w右侧黑高减了1 -> 右旋
					w->color = RED;
					rbtree_right_rotate(T, w);
					w = x->parent->right;  // 右旋后，x右兄弟变了
				}
				/* 删除节点情况6:被删节点是“黑+黑”节点，被删节点的兄弟节点是黑色；被删节点的兄弟节点的右孩子是红色的，被删节点的兄弟节点的左孩子任意颜色。
				(01) 将被删节点父节点颜色 赋值给 被删节点的兄弟节点。
				(02) 将被删节点父节点设为“黑色”。
				(03) 将被删节点兄弟节点的右子节点设为“黑色”。
				(04) 对被删节点的父节点进行左旋。
				(05) 设置“被删节点”为“根节点”。
				 */
				w->color = x->parent->color;  // 
				x->parent->color = BLACK;
				w->right->color = BLACK;
				rbtree_left_rotate(T, x->parent);

				x = T->root; // 防止因为旋转导致根结点变红了
			}

		} else {

			rbtree_node *w = x->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				rbtree_right_rotate(T, x->parent);
				w = x->parent->left;
			}

			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			} else {

				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					rbtree_left_rotate(T, w);
					w = x->parent->left;
				}

				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rbtree_right_rotate(T, x->parent);

				x = T->root;
			}

		}
	}

	x->color = BLACK;
}

/* 删除结点
 * 思路：根据z的孩子情况采取不同措施
 * 
 */
rbtree_node *rbtree_delete(rbtree *T, rbtree_node *z) {

	rbtree_node *y = T->nil;  // 待删除的结点
	rbtree_node *x = T->nil;  // 继承人

	if (z->left == T->nil || z->right == T->nil) {
		// 1.z为叶子结点或有独子，好删，直接删z
		y = z;
	} else {
		// 2.z有俩孩子，不好删，转换为删后继结点（右子树最左叶子结点）-> 使用后继节点替换z,删除后继结点
		y = rbtree_successor(T, z);
	}

	/* x取代y的位置 */
	if (y->left != T->nil) {
		x = y->left;
	} else if (y->right != T->nil) {
		x = y->right;
	} else {
		/* 删除节点情况9:被删节点只有一个子节点或无子节点
			(01) 将唯一的子节点替换被删节点。
		 */
		x = T->nil;  // 将唯一的子节点替换被删节点。
	}

	x->parent = y->parent;  // 为删除y做准备：处理y孩子x的问题，过继给y的父结点
	if (y->parent == T->nil) {
		T->root = x;
	} else if (y == y->parent->left) {
		y->parent->left = x;
	} else {
		y->parent->right = x;
	}

	if (y != z) {
		z->key = y->key;
		z->value = y->value;
	}
	/* 若删除的黑结点，需要重新平衡黑高 */
	if (y->color == BLACK) {
		rbtree_delete_fixup(T, x);
	}

	return y;
}

/* 根据key找到所在结点 */
rbtree_node *rbtree_search(rbtree *T, KEY_TYPE key) {

	rbtree_node *node = T->root;
	while (node != T->nil) {
		if (key < node->key) {
			node = node->left;
		} else if (key > node->key) {
			node = node->right;
		} else {
			return node;
		}
	}
	return T->nil;
}

const char *get_color(int color) {
	if (color == RED) {
		return "red";
	} else if (color == BLACK) {
		return "black";
	} else {
		return "unknown";
	}
}

/* 中序遍历 */
void rbtree_traversal(rbtree *T, rbtree_node *node) {
	if (node != T->nil) {
		rbtree_traversal(T, node->left);
		printf("key:%d, color:%d(%s)\n", node->key, node->color, get_color(node->color));
		rbtree_traversal(T, node->right);
	}
}

int main() {

	int keyArray[20] = {24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15};

	/* 树实例初始化 */
	rbtree *T = (rbtree *)malloc(sizeof(rbtree));
	if (T == NULL) {
		printf("malloc failed\n");
		return -1;
	}
	
	T->nil = (rbtree_node*)malloc(sizeof(rbtree_node));
	T->nil->color = BLACK;
	T->root = T->nil;

	/* 依次插入新节点 */
	rbtree_node *node = T->nil;
	int i = 0;
	for (i = 0;i < 20;i ++) {
		node = (rbtree_node*)malloc(sizeof(rbtree_node));
		node->key = keyArray[i];
		node->value = NULL;

		rbtree_insert(T, node);
		
	}

	rbtree_traversal(T, T->root);
	printf("----------------------------------------\n");

	/* 依次删除结点 */
	for (i = 0;i < 20;i ++) {
		printf("i=%d,delete node %d\n", i, keyArray[i]);
		rbtree_node *node = rbtree_search(T, keyArray[i]);
		rbtree_node *cur = rbtree_delete(T, node);
		free(cur);

		rbtree_traversal(T, T->root);
		printf("----------------------------------------\n");
	}
	

	
}
