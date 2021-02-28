#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class Node{
    public:
        int x;
        int y;
        char z;
        Node(){};
        Node(int x, int y, char z){this->x = x; this->y = y; this->z = z;}
        void print_node() {cout <<"("<< x <<","<< y <<","<< z<<")";}

    friend class Bnode;
    friend class BTree;
};

class Bnode{
    private:
        Node *keys;
        bool leaf;
        Bnode** children;
        int t;
        int key_number;
    public:
        Bnode(int x, bool leaf);
        void traverse_Bnodes();
        void insert_nonfull(Node&, char);
        void split_child(int x, Bnode *fullnode); 

    friend class Node;
    friend class BTree;
};
class BTree{
    private:
        Bnode *root;
        int t;
    public:
        BTree(int t){ this->t = t; root = NULL; }

        void insert(Node& newnode, char key);
        void traverse(); // print tree

    friend class Node;
    friend class Bnode;

};
Bnode::Bnode(int x, bool leaf) // constructor 
{
    t = x;
    this->leaf = leaf;
    keys = new Node [2*t-1]; //?
    children = new Bnode *[2*t];
    key_number = 0;
}

void Bnode::traverse_Bnodes() // print node B ++
{
    for(int i = 0; i < key_number; i++)
        keys[i].print_node();
    cout <<endl;
    if(!leaf){
        for(int i = 0; i < key_number + 1; i++)
            children[i]->traverse_Bnodes();  // recursive
    }
}

void Bnode::insert_nonfull(Node& newnode, char key) // ++
{
    int k = key_number;

    if(leaf)
    {
        if(key == 'x'){
            while(k > 0 and newnode.x < keys[k-1].x){
                keys[k] = keys[k-1];
                k--;
            }
        }
        if(key == 'y'){
            while(k > 0 and newnode.y < keys[k-1].y){
                keys[k] = keys[k-1];
                k--;
            }
        }
        if(key == 'z'){
            while(k > 0 and newnode.z < keys[k-1].z){
                keys[k] = keys[k-1];
                k--;
            }
        }
        keys[k] = newnode;
        key_number++;
        return;
    }
    if(!leaf)
    {
        if(key == 'x'){
            while(k > 0 and newnode.x < keys[k-1].x){
                k--;
            }
        }
        if(key == 'y'){
            while(k > 0 and newnode.y < keys[k-1].y){
                k--;
            }
        }
        if(key == 'z'){
            while(k > 0 and newnode.z < keys[k-1].z){
                k--;
            }
        }
        if(children[k]->key_number == 2*t-1){
            split_child(k,children[k]);

            if(key == 'x' and newnode.x > keys[k].x) k++;
            if(key == 'y' and newnode.y > keys[k].y) k++;
            if(key == 'z' and newnode.z > keys[k].z) k++;
        }
        children[k]->insert_nonfull(newnode,key);
    }

}
void Bnode::split_child(int x, Bnode *fullnode) // ++
{
    Bnode * newnode = new Bnode(fullnode->t, fullnode->leaf);
    newnode->key_number = t - 1;

    int s = 0;
    while(s <= t-2){
        newnode->keys[s] = fullnode->keys[t+s];
        s++;
    }

    if(!fullnode->leaf){
        int ss = 0;
        while(ss <= t-1){
            newnode->children[ss] = fullnode->children[t+ss];
            ss++;
        }
    }
    fullnode->key_number = t-1;
    
    for(int i = key_number; i > x; i--)
        children[i+1] = children[i];
    
    children[x+1] = newnode;

    for(int i = key_number-1; i >= x; i--)
        keys[i+1] = keys[i];
       
       key_number++;
    keys[x] = fullnode->keys[t-1];
}

void BTree::insert(Node& newnode, char key) // ++
{
    if(!root)
    {
        root = new Bnode(t,true);

        root->key_number = 1;
        root->keys[0] = newnode;
        return;
    }
    if(2*t == root-> key_number +1)
    {
        int i = 0;
        Bnode *newone = new Bnode(t,false);
        newone->children[0] = root;
        newone->split_child(0,root);

        if(key == 'x' and newnode.x > newone->keys[i].x) i++;

        if(key == 'y' and newnode.y > newone->keys[i].y) i++;

        if(key == 'z' and newnode.z > newone->keys[i].z) i++;

        newone -> children[i] -> insert_nonfull(newnode, key);
        root  = newone;
        return;
    }
    
    root->insert_nonfull(newnode, key);
}

void BTree::traverse() // ++
{
    if(root) root->traverse_Bnodes();
}


int main() {
    int x;
    int y;
    char z; 

    int total_node;
    int degree;
    char key;

    cin >> total_node;
    cin >> degree;
    BTree mytree(degree);
    cin >> key;

    for(int i=0; i < total_node; i++){
        cin >> x;
        cin >> y;
        cin >> z;
        Node * newnode = new Node(x,y,z);
        mytree.insert(*newnode, key);
    }

    mytree.traverse();

    return 0;
}
