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
        void remove_from_leaf(int id, char key);
        void remove_from_nonleaf(int id, char key);
        void traverse_Bnodes();
        void insert_nonfull(Node&, char);
        void split_child(int x, Bnode *fullnode); 
        void remove(Node* removed_node, char key);
        void merge(int id);
        void fill(int id);

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
        void delete_node(Node* removed_node, char key);

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

void Bnode::remove(Node* removed_node, char key)
{
    
    int id = 0;
    // if KEY == X
    if(key == 'x')
    {
        // finKey function
        while(id < key_number and keys[id].x < removed_node->x) ++id;

        if(id < key_number and keys[id].x == removed_node->x){

            if(leaf) remove_from_leaf(id,key);
            else remove_from_nonleaf(id,key);
        }
        else
        {
            if(leaf){
                cout<< "the key does not exist";
                return;
            }

            bool flag = false;

            if(key_number == id)
            	flag = true;

            if(children[id]->key_number <= t-1) 
                fill(id);
            
            if(flag and id > key_number){

                children[id-1]->remove(removed_node, key);
                return;
            }
            
            children[id]->remove(removed_node, key);
        }
        return;
    }

    // if KEY == Y
    if(key == 'y')
    {
        // finKey function
        while(id < key_number and keys[id].y < removed_node->y) ++id;

        if(id < key_number and keys[id].y == removed_node->y){

            if(leaf) remove_from_leaf(id,key);
            else remove_from_nonleaf(id,key);
        }
        else
        {
            if(leaf){
                cout<< "the key does not exist";
                return;
            }

            bool flag = false;

            if(key_number == id)
            	flag = true;

            if(children[id]->key_number < t) 
                fill(id);
            
            if(flag and id > key_number){

                children[id-1]->remove(removed_node, key);
                return;
            }
            
            children[id]->remove(removed_node, key);
        }
        return;

    }

     // if KEY == Z
    if(key == 'z')
    {
        // finKey function
        while(id < key_number and keys[id].z < removed_node->z) ++id;

        if(id < key_number and keys[id].z == removed_node->z){

            if(leaf) remove_from_leaf(id,key);
            else remove_from_nonleaf(id,key);
        }
        else
        {
            if(leaf){
                cout<< "the key does not exist";
                return;
            }

            bool flag = false;

            if(key_number == id)
            	flag = true;

            if(children[id]->key_number < t) 
                fill(id);
            
            if(flag and id > key_number){

                children[id-1]->remove(removed_node, key);
            	return;
            }
            children[id]->remove(removed_node, key);
        }
        return;
    }
}

void Bnode::remove_from_leaf(int id, char key)
{
    for(int i = id; i <= key_number-2; i++){
            keys[i].x = keys[i+1].x;
            keys[i].y = keys[i+1].y;
            keys[i].z = keys[i+1].z;
    }
    key_number--;
}

void Bnode::remove_from_nonleaf(int id, char key)
{

    Node* x = &keys[id];

    if(children[id]->key_number >= t){
        Bnode * cur = children[id];
        while(cur->leaf == false)
            cur = cur->children[cur->key_number];

        keys[id] = cur->keys[cur->key_number-1];
        children[id]->remove(&keys[id],key);
        return;
    }
    if(children[id+1]->key_number >= t){
        Bnode * cur = children[id+1];
        while(cur->leaf == false)
            cur = cur->children[0];
    
        keys[id] = cur->keys[0];
        children[id+1]->remove(&keys[id],key);
        return;
    }
    merge(id);
    children[id]->remove(x,key);
    
}

void Bnode::merge(int id)
{
    Bnode * child = children[id];
    Bnode * sibling = children[id+1];

    child ->keys[t-1] = keys[id];

    for(int i=t; i < sibling->key_number+t; i++)
        child->keys[i] = sibling->keys[i-t];
    
    if(child->leaf == false){
        for(int i=t; i <= sibling->key_number+t; i++)
            child->children[i] = sibling->children[i-t];
    }

    for(int i=id+1; i < key_number; i++)
        children[i] = children[i+1];

    for(int i=id; i < key_number-1; i++)
        keys[i] = keys[i+1];
    
    child->key_number = child->key_number + sibling->key_number+1;
    key_number--;

    delete sibling;
}

void Bnode::fill(int id)
{
    if(id != 0 and children[id-1]->key_number >= t)
    {
        //borrow from prev function
        Bnode * child = children[id];
        Bnode * sibling = children[id-1];

        for(int i = child->key_number-1; i >= 0; --i)
            child->keys[i+1] = child->keys[i];

        if(child->leaf == false){
            for(int i = child->key_number; i >= 0; --i)
                child->children[i+1] = child->children[i];
        }
        child->keys[0] = keys[id-1];

        if(child->leaf == false)
            child->children[0] = sibling->children[sibling->key_number];

        keys[id-1] = sibling->keys[sibling->key_number-1];
        child->key_number =  child->key_number +1;
        sibling->key_number = sibling->key_number -1;
        return;
    }

    if(id != key_number and children[id+1]->key_number >= t)
    {
        // borrow from next function
        Bnode * child = children[id];
        Bnode * sibling = children[id+1];

        child->keys[(child->key_number)] = keys[id];

        if (!(child->leaf)) 
            child->children[(child->key_number)+1] = sibling->children[0];

        keys[id] = sibling->keys[0];

        for (int i = 1; i < sibling->key_number; ++i) 
            sibling->keys[i-1] = sibling->keys[i]; 
        
        if (sibling->leaf == false) { 
            for(int i = 1; i <= sibling->key_number; ++i) 
                sibling->children[i-1] = sibling->children[i]; 
        }
        child->key_number = child->key_number + 1; 
        sibling->key_number = sibling->key_number - 1; 
        return;
    }

    if(id == key_number)
    	merge(id-1);
   	else
        merge(id);
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

void BTree::delete_node(Node* removed_node, char key)
{
    if(!root)
        return;
    
    root->remove(removed_node,key);

    if(root-> key_number == 0){
        Bnode * temp = root;

        if(root->leaf) root = NULL;
        else root = root->children[0];

        delete temp;
    }
    return;
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
// get inputs and insert tree
    for(int i=0; i < total_node; i++){
        cin >> x;
        cin >> y;
        cin >> z;
        Node * newnode = new Node(x,y,z);
        mytree.insert(*newnode, key);
    }
// delete process
    Node * deleted_node = new Node;
    if(key =='x'){
        cin >> x;
        deleted_node->x = x;
    }
    if(key =='y'){
        cin >> y;
        deleted_node->y = y;
    }

    if(key =='z'){
        cin >> z;
        deleted_node->z = z;
    }
    mytree.delete_node(deleted_node, key);

// print tree
    mytree.traverse();
    

    return 0;
}
