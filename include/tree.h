#define RED 0
#define BLACK 1

extern struct node{
	int key;
	int color;
	struct node *parent;
	struct node *left;
	struct node *right;
};

extern struct node *ROOT;
extern struct node *EMPTYNODE;

extern void tr_TreeCreate();
extern void callprocess();
extern void tr_delete(struct node *deleteNode);
extern void tr_delete_fix(struct node *fixUpNode);
extern struct node *tr_minimum(struct node *deleteNode);
extern void tr_transplant(struct node *deleteNode, struct node *deleteNodeChild);
extern struct node *tr_search(int key);
extern void tr_print(struct node *x);
extern void tr_insert(int key);
extern void redBlackRightFix(struct node *x);
extern void redBlackLeftFix(struct node *x);
extern void redBlackTreeRotation(struct node *z);