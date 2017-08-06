#ifndef _TWO_THREE_TREE_H_
#define _TWO_THREE_TREE_H_

#include <iostream>
using namespace std;

#define TREE23_DEBUG

class treeNode {
public:
    treeNode(int _key = -1) {
        key[0] = _key;
        key[1] = -1;
        parent = NULL;
        child[0] = child[1] = child[2] = NULL; 
    }

    ~treeNode() {}

    //
    bool isLeaf() {
        return (child[0] == NULL && child[1] == NULL && child[2] == NULL);
    }

    // 
    int getTreeNodeDegree() {
        if(key[0] == -1 && key[1] == -1)
            return 0;
        else if(key[0] != -1 && key[1] != -1)
            return 2;
        return 1;
    }


public:
    int       key[2];
    treeNode* parent;
    treeNode* child[3];
};

class tree23 {
public:
    tree23() {
        tRoot = new treeNode;
    }

    ~tree23() {
        // pass
        // to be continue
    }

    // insert
    bool insert(int key) { 
        // 第一次插入
        if(tRoot->child[0] == NULL) {
            tRoot->child[0] = new treeNode(key); 
            tRoot->child[0]->parent = tRoot;
            return true;
        }

        // 如果不是第一次插入, 则找到合适的插入点, 返回插入点的parent指针
        treeNode* spot = findSpot(tRoot->child[0], key); 

        // 如果插入的值已经存在, 则返回false
        if(spot == NULL)
            return false;

        // 调用内部insert
        _insert(spot, key);

        return true;
    }

#ifdef TREE23_DEBUG
public:
#else
private:
#endif
    void treePrint() {
        if(tRoot->child[0] == NULL) {
            cout << "Nothing in the 23Tree" << endl;
        }    

        if(tRoot->child[1] != NULL) {
            cout << "Error: tRoot->child[1] != NULL" << endl;
        }

        if(tRoot->child[2] != NULL) {
            cout << "Error: tRoot->child[2] != NULL" << endl;
        }

        _treePrint(tRoot->child[0]);
    }

    void _treePrint(treeNode* node) {
        if(node == NULL)
            return ;
        
        if(node->parent == tRoot) {
            cout << "My parent node is tRoot" << endl; 
        }
        else {
            cout << "My parent node is " << node->parent->key[0] << " " << node->parent->key[1] << endl;
        }

        if(node == node->parent->child[0]) {
            cout << "I'm L node : ";
        }
        else if(node == node->parent->child[1]) {
            cout << "I'm M node : ";
        }
        else {
            cout << "I'm R node : ";
        }

        cout << node->key[0] << " " << node->key[1] << endl;

        _treePrint(node->child[0]);
        _treePrint(node->child[1]);
        _treePrint(node->child[2]);
    }

private:
    // 寻找合适的插入点, 返回父指针
    treeNode* findSpot(treeNode* node, int key) {
        while(!node->isLeaf()) {
            if(key == node->key[0] || key == node->key[1])
                return NULL;
            else if(key < node->key[0])
                node = node->child[0];
            else if(node->key[1] != -1 && key > node->key[1])
                node = node->child[2];
            else    
                node = node->child[1];
        } 

        if(node->key[0] == key || node->key[1] == key)
            return NULL;

        return node->parent;
    }

    // 
    void _insert(treeNode* root, int key) {
        treeNode* node;

        // 寻找插入点
        // 如果插入的父节点是根节点, 需要特殊处理
        if(root == tRoot) {
            node = tRoot->child[0]; 
        }
        else {  
            if(key < root->key[0])
                node = root->child[0];
            else if(root->key[1] != -1 && key > root->key[1])
                node = root->child[2];
            else
                node = root->child[1];
        }

        // 插入元素
        treeNode* upNode = _insertNode(node, key);
        
        if(upNode != NULL)
            _combine(root, upNode);
    }

    treeNode* _insertNode(treeNode* node, int key) { 
        // 如果节点中key的数量为1情况下, 字节直接插入
        if(node->getTreeNodeDegree() == 1) {
            if(key < node->key[0]) {
                node->key[1] = node->key[0];
                node->key[0] = key;
            }
            else {
                node->key[1] = key;
            }
        
            return NULL;
        }
        
        // 如果节点中key的数量为2情况下, 则需要向上传递中间节点
        // 同时, 由于节点分裂, 需要创建新的节点
        // 传递的节点为node
        treeNode* siblingL = new treeNode; // 左兄弟
        treeNode* siblingR = new treeNode; // 右兄弟

        // node->key[0]被传递上去
        if(key < node->key[0]) {
            siblingL->key[0] = key;
            siblingL->parent = node;

            siblingR->key[0] = node->key[1];
            siblingR->parent = node;

            node->key[1]   = -1;
            node->child[0] = siblingL;
            node->child[1] = siblingR;
            
        }
        // node->key[1]被传递上去
        else if(key > node->key[1]) {
            siblingL->key[0] = node->key[0];
            siblingL->parent = node;

            siblingR->key[0] = key;
            siblingR->parent = node;

            node->key[0]   = node->key[1];
            node->key[1]   = -1;
            node->child[0] = siblingL;
            node->child[1] = siblingR;
        }
        // key被传递上去
        else {
            siblingL->key[0] = node->key[0];
            siblingL->parent = node;

            siblingR->key[0] = node->key[1];
            siblingR->parent = node;

            node->key[0]   = key;
            node->key[1]   = -1;
            node->child[0] = siblingL;
            node->child[1] = siblingR;
        }

        return node;
    }

    // 合拼两个节点p和q, 其中p节点是q节点的子节点, q节点是即将插入到p节点的子节点
    // 需要注意的是, 由于根节点在代码中保持唯一性, 即tRoot->child[0]永远是指向23树的根节点
    // 所以合并的时候需要注意p节点为tRoot->child[0] & 满足p节点是3节点的情况下需要特殊处理
    // 更新tRoot->child[0]关系
    // 使用递归的方法, 从下向上依次合并 
    treeNode* _combine(treeNode* p, treeNode* q) {
        if(p == tRoot) {
            p->child[0] = q;
            return NULL;
        }
        
        // 此时p节点只有1个key, 合并后无需向上传递
        if(p->getTreeNodeDegree() == 1) {
            // 判断q属于p的左孩子还有右孩子
            if(p->child[0] == q) {
                // 更新key关系
                p->key[1] = p->key[0];
                p->key[0] = q->key[0];

                // 更改节点关系
                p->child[2] = p->child[1];
                p->child[0] = q->child[0];
                p->child[1] = q->child[1];
            }
            else {
                // 更改key关系
                p->key[1] = q->key[0];

                // 更改节点关系
                p->child[1] = q->child[0];
                p->child[2] = q->child[1];
            }

            // 更改父节点关系
            q->child[0]->parent = p;
            q->child[1]->parent = p;

            // q的内容已经存放在p中, 释放q节点
            delete q;

            return NULL;
        }

        // 此时p节点只有2个key, 合并后需向上传递
        // 如果p为2-key情况下, 情况变得复杂
        treeNode* t = new treeNode; 

        /* case 1
                A - - B                 A   -----> p
               /   |   \    ====>      / \  
              q    C    D             q   B -----> t
             / \                     / \ / \
            -  =                    -  = C  D  
        */
        if(p->child[0] == q) {
            // set t
            t->key[0]   = p->key[1];
            t->child[0] = p->child[1];
            t->child[1] = p->child[2];
            t->parent   = p;

            t->child[0]->parent = t;
            t->child[0]->parent = t;

            // set p
            p->key[1]   = -1;
            p->child[1] = t;
            p->child[2] = NULL;

            // set q, nothing to do
        }
        /* case 2
                A - - B                 q      p: <- q
               /   |   \               / \     q: <- A
              C    q   D    ====>     A   B    t: <- B
                  / \                / \ / \
                 *   =              C  * =  D
        */
        else if(p->child[1] == q) {
            // set t
            t->key[0] = p->key[1];
            t->child[0] = q->child[1];
            t->child[1] = p->child[2];
            t->parent = p;

            t->child[0]->parent = t;
            t->child[0]->parent = t;

            // set q
            int tmp     = q->key[0];
            q->key[0]   = p->key[0];
            q->child[1] = q->child[0];
            q->child[0] = p->child[0];

            q->child[0]->parent = q;
            q->child[1]->parent = q;

            // set p
            p->key[0]   = tmp;
            p->key[1]   = -1;
            p->child[0] = q;
            p->child[1] = t;
            p->child[2] = NULL;
        }
        /* case 3
                A - - B                 B
               /   |   \               / \
              C    D    q    ====>    A   q
                       / \           / \ / \
                      *   =         C  D * =
        */
        else if(p->child[2] == q) { 
            // set t
            t->key[0]   = p->key[0];
            t->child[0] = p->child[0];
            t->child[1] = p->child[1];
            t->parent   = p;

            t->child[0]->parent = t;
            t->child[1]->parent = t;

            // set p
            p->key[0]   = p->key[1];
            p->key[1]   = -1;
            p->child[0] = t;
            p->child[1] = q;
            p->child[2] = NULL;
        }
        
        return _combine(p->parent, p);
    }

private:
    treeNode* tRoot;
};


#endif