#include<xinu.h>
#include<tree.h>


void tr_print(struct node *x){
	if(x->key > 4){
	    kprintf("\n\n Tree Nodes present are  %d\t", x->key);
		tr_print(x->left);
		tr_print(x->right);
	}
}