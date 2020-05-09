#include<xinu.h>
#include<tree.h>

struct node *tr_search(int key){
    struct node *temp;
    temp = ROOT;
    while(temp->key != -1 && temp->key!=key){
        if(key < temp->key){
            temp = temp->left;
        }else{
            temp = temp->right;
        }
    }
    kprintf("\n Searched value is %d\n\n", temp->key);
    tr_delete(temp);
    kprintf("\n Deleting this searched value\n");
    return temp;
}