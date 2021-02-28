// Beyza Ozan
// 150180056


#include <stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;

typedef struct player{
    string season;
    string name;
    string team;
    int rebound;
    int assist;
    int point;

    bool color; // 0 if it is black, 1 if it is red
    player* parent;
    player* left;
    player* right;
}player;

typedef struct rbtree{
    player* root;
    player* max_rebound;
    player* max_assist;
    player* max_point;

    void init();
    void insert_fixup(player* new_player); // ++
    void insert(player* root, player* new_player); // ++
    void decide_max(player* person); // ++
    void print_tree_preorder(player* the_player,int depth); // ++
    player* rotate_right(player* x); // +!
    player* rotate_left(player* x); // +!
    void read_file_and_print(char* file); // ++

}rbtree;

void rbtree::init(){ // initializing
    player* parent = NULL;
    player* left = NULL;
    player* right = NULL;
    player* root = NULL;

}

void rbtree:: insert_fixup(player* new_player)
{   
     player* uncle = NULL;

    if(new_player == root){
        root->color = 0;
        return;}
    if(new_player->color == 0 or new_player->parent->color == 0 )
        return;

    if(!new_player->parent->parent){
        new_player->parent->color = 0;
        return;
    }
    if(new_player->parent == new_player->parent->parent->right) // uncle is left child
        uncle  = new_player->parent->parent->left;
    else if(new_player->parent == new_player->parent->parent->left) // uncle is right child
        uncle = new_player->parent->parent->right;
    
    //  If new_player’s uncle is exists and RED
    if (uncle and uncle->color == 1){
        uncle->color = 0;   //Change color of uncle as BLACK
        new_player->parent->color = 0;   //Change color of parent as BLACK
        new_player->parent->parent->color = 1;   //Change color of grand parent as RED 

        if(new_player->parent->parent)
            insert_fixup(new_player->parent->parent); //recursive..
        return;
    }
    // If new_player's uncle is now BLACK 
    if(new_player->parent->parent and new_player->parent->parent->left and new_player->parent->parent->left->left == new_player) //  Left Left Case (new_player is left child of parent and parent is also left child)
    {   
        // swap colors of parent and grandparent  
        bool temp = new_player->parent->parent->color;
        new_player->parent->parent->color = new_player->parent->color;
        new_player->parent->color = temp;
        rotate_right(new_player->parent->parent);
        insert_fixup(new_player->parent);
        return;
    }

    if(new_player->parent->parent and new_player->parent->parent->left and new_player->parent->parent->left->right == new_player) // Left Right Case (new_player is right child of parent and parent is left child)
    {

        // swap colors of parent and grandparent
        bool temp = new_player->parent->parent->color;
        new_player->parent->parent->color = new_player->color;
        new_player->color = temp;

        player* Node = rotate_left(new_player->parent);
        rotate_right(Node->parent);        
        insert_fixup(new_player);
        return;
    }

    if(new_player->parent->parent and new_player->parent->parent->right and new_player->parent->parent->right->right == new_player)  // Right Right Case (new_player is right child of parent and parent is also right child)
    {   
        // swap colors of parent and grandparent
        bool temp = new_player->parent->parent->color;
        new_player->parent->parent->color = new_player->parent->color;
        new_player->parent->color = temp;
        rotate_left(new_player->parent->parent);
        insert_fixup(new_player->parent);
        return;
    }

    if(new_player->parent->parent and new_player->parent->parent->right and new_player->parent->parent->right->left == new_player) // Right Left Case (new_player is left child of parent and parent is right child)
    {   

        // swap colors of parent and grandparent
        bool temp = new_player->parent->parent->color;  
        new_player->parent->parent->color = new_player->color;
        new_player->color = temp;
        rotate_left(rotate_right(new_player->parent)->parent);
        insert_fixup(new_player);
        return;
        
    }
}
void rbtree::insert(player* root, player* new_player) // inserting person to the tree
{
    if(new_player->name == root->name){ // it is updating process
        root->rebound = root->rebound + new_player->rebound;
        root->assist = root->assist + new_player->assist;
        root->point = root->point + new_player->point;
        return;
    }
    else if(new_player->name > root->name){ // if added player bigger than root, put it as right child of root
        if(root->right != NULL){
            insert(root->right,new_player);
            return;
        }
         // if there is no right child of root yet.
        else{
            new_player->parent = root;
            root->right = new_player;
            insert_fixup(new_player);
        }

    }
    else if(root->name > new_player->name){  // if added player smaller than root, put it as left child of root
        if(root->left != NULL){
            insert(root->left,new_player);
            return;
        }
        // if there is no left child of root yet.
        else{
            new_player->parent = root;
            root->left = new_player;
            insert_fixup(new_player);
        }
    }
}


void rbtree::decide_max(player* person) // find maximum values of rebounds, assists and points
{
    if(!person) return;

    if(max_rebound &&  person->rebound > max_rebound->rebound){
        max_rebound->name = person->name;
        max_rebound->team = person->team;
        max_rebound->rebound = person->rebound;
    }
    if(max_assist && person->assist > max_assist->assist){
        max_assist->name = person->name;
        max_assist->team = person->team;
        max_assist->assist = person->assist;
    }
        if(max_point && person->point > max_point->point){
        max_point->name = person->name;
        max_point->team = person->team;
        max_point->point = person->point;
    }
    decide_max(person->right);
    decide_max(person->left);
}

void rbtree::print_tree_preorder(player* the_player, int depth) // printing the red-black tree
{  
    if(!the_player)
        return;

    for(int i = 0; i < depth; i++){
        cout<<"-";
    }
    if(the_player->color == 0)
        cout<<"(BLACK) "<< the_player->name<<endl;

    else if(the_player->color == 1)
        cout<< "(RED) "<< the_player->name<<endl;
    if(the_player->left)
        print_tree_preorder(the_player->left,depth + 1);
    if(the_player->right)
        print_tree_preorder(the_player->right,depth + 1);

}

player* rbtree::rotate_left(player* person){
    player* pt_right = person->right;

    if(person->right->left)
    {
        person->right->left->parent = person;
    }  
    if(person->parent == NULL)
    {
        root = person->right;
    } 
    else if(person->parent->left == person)
    {
        person->parent->left = person->right;
    }  
    else
    {
        person->parent->right = person->right;
    } 
    person->right->parent = person->parent;
    person->right = person->right->left;
    pt_right->left = person;
    person->parent = pt_right;
    return person->parent;
}
player* rbtree::rotate_right(player* person){
    player* pt_left = person->left;

    if(person->left->right)
    {
        person->left->right->parent = person;
    }
    if(person->parent == NULL)
    {
        root = person->left;
    }
    else if(person->parent->right == person)
    {
        person->parent->right = person->left;
    }
    else
    {
        person->parent->left = person->left;
    }
    person->left->parent = person->parent;
    person->left = person->left->right;
    pt_left->right = person;
    person->parent = pt_left;
    return person->parent;
}

void rbtree::read_file_and_print(char* file) // reading data from file and printing the results to terminal
{  
    max_assist = new player;
    max_rebound = new player;
    max_point = new player;

    ifstream myfile(file);
    if(myfile.is_open()){   
        string data;
        string previous_season, current_season;
        bool is_it_root = true;
        bool flag = 1;
        getline(myfile,data); // reading headers
        while(!myfile.eof()){
            player* new_player = new player;
            getline(myfile,data,',');
            new_player -> season = data;
            getline(myfile,data,',');
            new_player -> name = data;
            getline(myfile,data,',');
            new_player -> team = data;
            getline(myfile,data,',');
            stringstream s(data);
            int reb;
            s>>reb;
            new_player -> rebound = reb;
            getline(myfile,data,',');
            stringstream s2(data);
            int assist;
            s2>>assist;
            new_player -> assist = assist;
            getline(myfile,data,'\n');
            stringstream s3(data);
            int poi;
            s3>>poi;
            new_player -> point = poi;
            //cout<<"name:"<<new_player -> name <<endl;

            if(is_it_root == false){ // if readed player is NOT first one (root).
                new_player -> color = 1;
                current_season = new_player -> season;
                if(previous_season == current_season){
                    insert(root,new_player);}
            
                else{
                    decide_max(root); // check and fix the maxs if they have changed
                    cout<<"End of the "<< previous_season <<" Season"<<endl;
                    cout<<"Max Points: "<< max_point->point <<" - Player Name: "<< max_point->name<<endl;
                    cout<<"Max Assists: "<< max_assist->assist <<" - Player Name: "<< max_assist->name<<endl;
                    cout<<"Max Rebs: "<< max_rebound->rebound <<" - Player Name: "<< max_rebound->name<<endl;
                    if(flag == 1){
                        print_tree_preorder(root,0);
                        flag = 0;
                    }
                        
                    insert(root,new_player);
                }
                previous_season = current_season;
            
            }
            else{ // if readed player is the first one.
                new_player -> color = 0;
                root = new_player;
                previous_season = new_player->season;
                max_rebound->rebound = new_player->rebound;
                max_rebound->name = new_player->name;
                max_assist->assist = new_player->assist;
                max_assist->name = new_player->name;
                max_point->point = new_player->point;
                max_point->name = new_player->name;
                is_it_root = false;
            } 
        } // end of while

        //printing last season
        decide_max(root); // check and fix the maxs if they have changed
        cout<<"End of the "<< previous_season <<" Season"<<endl;
        cout<<"Max Points: "<< max_point->point <<" - Player Name: "<< max_point->name<<endl;
        cout<<"Max Assists: "<< max_assist->assist <<" - Player Name: "<< max_assist->name<<endl;
        cout<<"Max Rebs: "<< max_rebound->rebound <<" - Player Name: "<< max_rebound->name<<endl;
    }
    else{
        cout<<"File cannot opened"<<endl;
        exit(1);}
}

rbtree mytree;
int main(int argc, char* argv[]){

    char* myfile = argv[1];
    mytree.read_file_and_print(myfile);

    return 0;
}
