#include<xinu.h>
#include<tree.h>

struct node *ROOT;
struct node *EMPTYNODE;
int value=0;
int previous_value;




void redBlackRightFix(struct node *rotateRightNode){

    struct node *temp;
    temp = rotateRightNode->left;
    rotateRightNode->left = temp->right;
    if(temp->right->key != -1){
        temp->right->parent = rotateRightNode;
    }
    temp->parent = rotateRightNode->parent;
    if(temp->parent->key == -1){
        ROOT = temp;
    }else if(rotateRightNode == rotateRightNode->parent->left){
        rotateRightNode->parent->left = temp;
    }else{
        rotateRightNode->parent->right = temp;
    }

    temp->right = rotateRightNode;
    rotateRightNode->parent = temp;
}
void redBlackLeftFix(struct node *rotateLeftNode){
    struct node *temp;
    temp = rotateLeftNode->right;
    rotateLeftNode->right = temp->left;
    if(temp->left->key != -1){
        temp->left->parent = rotateLeftNode;
    }
    temp->parent = rotateLeftNode->parent;
    if(temp->parent->key == -1){
        ROOT = temp;
    }else if(rotateLeftNode == rotateLeftNode->parent->left){
        rotateLeftNode->parent->left = temp;
    }else{
        rotateLeftNode->parent->right = temp;
    }
    temp->left = rotateLeftNode;
    rotateLeftNode->parent = temp;

}
void redBlackTreeRotation(struct node *rotateNode){
    while(rotateNode->parent->color == RED){
        if(rotateNode->parent->parent->left == rotateNode->parent){
            if(rotateNode->parent->parent->right->color == RED){
                rotateNode->parent->color = BLACK;
                rotateNode->parent->parent->right->color = BLACK;
                rotateNode->parent->parent->color = RED;
                rotateNode = rotateNode->parent->parent;
            }else{
                if(rotateNode == rotateNode->parent->right){
                    rotateNode = rotateNode->parent;
                    redBlackLeftFix(rotateNode);
                }
                rotateNode->parent->color = BLACK;
                rotateNode->parent->parent->color = RED;
                redBlackRightFix(rotateNode->parent->parent);
            }
        }
    else{
        if(rotateNode->parent->parent->left->color == RED){
            rotateNode->parent->color = BLACK;
            rotateNode->parent->parent->color = RED;
            rotateNode->parent->parent->left->color = BLACK;
            rotateNode = rotateNode->parent->parent;
        }else{
            if(rotateNode == rotateNode->parent->left){
                rotateNode = rotateNode->parent;
                redBlackRightFix(rotateNode);
            }
            rotateNode->parent->color = BLACK;
            rotateNode->parent->parent->color = RED;
            redBlackLeftFix(rotateNode->parent->parent);
        }
    }
    }
    ROOT->color = BLACK;

}

void tr_insert(int key){

    struct node *temp;
    temp = getmem(sizeof(struct node));
    temp->key = key;
    temp->color = RED;
    temp->left = EMPTYNODE;
    temp->right = EMPTYNODE;
    struct node *x;
    struct node *y;
    x = ROOT;
    y = EMPTYNODE;
    while(x->key != -1){
        y = x;
        if(temp->key <= x->key){
            x = x->left;
        }else{
            x = x->right;
        }
    }
    if(y->key == -1){
        ROOT = temp;
    }
    else if(temp->key <= y->key){
        y->left = temp;
    }else {
        y->right  = temp;
    }

    temp->parent = y;
    redBlackTreeRotation(temp);
}

void tr_transplant(struct node *deleteNode, struct node *deleteNodeChild){
    if(deleteNode->parent->key == -1){
        ROOT = deleteNodeChild;
    }else if(deleteNode == deleteNode->parent->left){
        deleteNode->parent->left = deleteNodeChild;
    }else{
        deleteNode->parent->right = deleteNodeChild;
    }

    deleteNodeChild->parent = deleteNode->parent;

}

struct node *tr_minimum(struct node *deleteNode){
    while(deleteNode->left->key != -1){
        deleteNode = deleteNode->left;
    }
    return deleteNode;
}

void tr_delete_fix(struct node *fixUpNode){
    struct node *temp;
    while(fixUpNode!=ROOT && fixUpNode->color==BLACK){
        if(fixUpNode == fixUpNode->parent->left){
            temp = fixUpNode->parent->right;
            if(temp->color == RED){
                temp->color = BLACK;
                fixUpNode->parent->color = RED;
                redBlackLeftFix(fixUpNode->parent);
                temp = fixUpNode->parent->right;
            }

            if(temp->left->color == BLACK && temp->right->color==BLACK){
                temp->color = RED;
                fixUpNode->parent->color = BLACK;
                fixUpNode = fixUpNode->parent;

            }else{
                if(temp->right->color == BLACK){
                    temp->color = RED;
                    temp->left->color = BLACK;
                    redBlackRightFix(temp);
                    temp = fixUpNode->parent->right;
                }
                temp->color = fixUpNode->parent->color;
                fixUpNode->parent->color = BLACK;
                fixUpNode->right->color = BLACK;
                redBlackLeftFix(fixUpNode->parent);
                fixUpNode = ROOT;
            }
        }
        else{
            temp = fixUpNode->parent->left;
            if(temp->color == RED){
                temp->color = BLACK;
                fixUpNode->parent->color = BLACK;
                redBlackRightFix(fixUpNode->parent);
                temp = fixUpNode->parent->left;

            }
            if(temp->left->color == BLACK && temp->right->color == BLACK){
                temp->color = RED;
                fixUpNode->parent->color = BLACK;
                fixUpNode = fixUpNode->parent;
            }else{
                if(temp->left->color == BLACK){
                    temp->color = RED;
                    temp->right->color = BLACK;
                    redBlackLeftFix(temp);
                    temp = fixUpNode->parent->left;

                }
                temp->color = fixUpNode->parent->color;
                fixUpNode->parent->color = BLACK;
                temp->left->color = BLACK;
                redBlackRightFix(fixUpNode->parent);
                fixUpNode = ROOT;
            }
        }
    }

    fixUpNode->color = BLACK;

}

void tr_delete(struct node *deleteNode){
    struct node *temp;
    struct node *originalTemp;
    int colorTemp;

    originalTemp = deleteNode;
    colorTemp = deleteNode->color;
    if(deleteNode->left->key == -1){
        temp = deleteNode->right;
        tr_transplant(deleteNode, deleteNode->right);
    }else if(deleteNode->right->key == -1){
        temp = deleteNode->left;
        tr_transplant(deleteNode, deleteNode->left);
    }else{
        originalTemp = tr_minimum(deleteNode->right);
        colorTemp = originalTemp->color;
        temp = originalTemp->right;
        if(originalTemp->parent == deleteNode){
            temp->parent = originalTemp;
        }else{
            tr_transplant(originalTemp, originalTemp->right);
            originalTemp->right = deleteNode->right;
            originalTemp->right->parent = originalTemp;
        }
        tr_transplant(deleteNode, originalTemp);
        originalTemp->left = deleteNode->left;
        originalTemp->left->parent = originalTemp;
        originalTemp->color = deleteNode->color;

    }

    if(colorTemp == BLACK){
        tr_delete_fix(temp);
    }

}

void callprocess(){
    kprintf("\n Process ID for inserting is %d\n", getpid());

    value++;
    previous_value = getpid();
    tr_insert(getpid());

    if(value == 2){
            tr_search(previous_value);
        }
    if(value >= 4){
        tr_print(ROOT);
    }
    suspend(getpid());
   
}