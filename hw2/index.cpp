#include <iostream>
#include <fstream>

#include <vector>
#include <utility>
#include <limits>
#include <algorithm>

#include "index.h"

using namespace std;

const int order = 1020;
void writefile(string filename, vector<int> data);

//order 20 B+ tree
struct Node{
    vector<int> keys;
    vector<int> values; //use only by leaf node
    vector<Node*> children;
    Node* parent;
    bool is_leaf;
    Node* next;
    Node* prev;

    Node(){
        //+1 for split
        keys.reserve(order+1);
        values.reserve(order+1);
        children.reserve(order+1+1);
    }
};

class Index{
private:
    Node* root;

    Node* find_leaf(int key);
    int search_keyidx(Node* leaf,int key);
    int search_key(int key);
    int search_range(int key1, int key2);
    void clearAll(Node* node);
    
    void insert(int key, int value);
    void insert_key(Node* internalnode,int key,Node *new_node);
    void split(Node* node);
    void build_b_plus_tree();
public:
    void key_query(vector<int> query_kes);
    void range_query(vector<pair<int,int>> query_pairs);
    void clear_index();
    Index(int num_rows, vector<int> key, vector<int> value);
};


/*********************************************************************** *
 * ********************************************************************* *
 * ********************************************************************* */

void writefile(string filename, vector<int> data){
    ofstream outfile;
    //overwrite the file
    outfile.open(filename+".txt", ios::out | ios::trunc);
    for (long long i = 0; i < data.size(); i++){
        outfile << data[i] << "\n";
    }
    outfile.close();
};

Node* Index::find_leaf(int key){
    Node* node = this->root;
    while(!node->is_leaf){
        //binary search for the idx which key < node->keys[idx]
        int l=0;
        int r=node->children.size()-1;
        int mid;
        while(l<r){
            mid = (l+r)/2;
            if(key < node->keys[mid]){
                r = mid;
            }else{
                l = mid+1;
            }
        }
        node = node->children[l];
    }
    return node;
}

//return the index of the key in the leaf which is the smallest one that is larger than or equal to key
int Index::search_keyidx(Node* leaf,int key){
    int l=0;
    int r=leaf->keys.size()-1;
    int mid;
    while(l<r){
        mid = (l+r)/2;
        if(key <= leaf->keys[mid]){
            r = mid;
        }else{
            l = mid+1;
        }
    }
    return r;
}

int Index::search_key(int key){
    Node* leaf = this->find_leaf(key);
    int idx = this->search_keyidx(leaf,key);
    if(leaf->keys[idx] != key){
        return -1;
    }else{
        return leaf->values[idx];
    }
}

int Index::search_range(int key1, int key2){
    Node* leaf = this->find_leaf(key1);
    int idx = this->search_keyidx(leaf,key1);
    int min=numeric_limits<int>::max();
    if(leaf->keys[idx]<key1){  //not in this leaf
        if(leaf->next == NULL){
            return -1;
        }else{
            leaf = leaf->next;
            idx = 0;
        }
    }
    while(leaf->keys[idx]<=key2){
        if(leaf->values[idx]<min){
            min = leaf->values[idx];
        }
        idx++;
        if(idx>=leaf->keys.size()){
            if(leaf->next == NULL){
                break;
            }else{
                leaf = leaf->next;
                idx = 0;
            }
        }
    }
    return min;
}

void Index::key_query(vector<int> query_keys){
    vector<int> result;
    result.reserve(query_keys.size());
    for(int i=0;i<query_keys.size();i++){
        int value = this->search_key(query_keys[i]);
        result.push_back(value);
    }
    writefile("key_query_out",result);
    return;
}

void Index::range_query(vector<pair<int,int>> query_pairs){
    vector<int> result;
    result.reserve(query_pairs.size());
    for(int i=0;i<query_pairs.size();i++){
        int value = this->search_range(query_pairs[i].first,query_pairs[i].second);
        result.push_back(value);
    }
    writefile("range_query_out",result);
    return;
}

void Index::clearAll(Node* node){
    if(node->is_leaf){
        delete node;
    }else{
        for(int i=0;i<node->children.size();i++){
            clearAll(node->children[i]);
        }
        delete node;
    }
    return;
}

void Index::clear_index(){
    clearAll(this->root);
    return;
}

void Index::insert(int key, int value){
    Node* leaf = find_leaf(key);
    int idx=this->search_keyidx(leaf,key);  //the index of the key in the leaf >= key
    if(leaf->keys[idx] == key){ //duplicate key--> update value
        leaf->values[idx] = value;
        return;
    }else{
        if(leaf->keys[idx]<key){
            idx++;
        }
        leaf->keys.insert(leaf->keys.begin()+idx, key);
        leaf->values.insert(leaf->values.begin()+idx, value);
        if(leaf->keys.size() == order){
            split(leaf);
        }
    }
}

void Index::insert_key(Node* internalnode,int key,Node *new_node){
    int idx=this->search_keyidx(internalnode,key);  //the index of the key in the leaf >= key
    if(internalnode->keys[idx] == key){ 
        return;
    }else{
        if(internalnode->keys[idx]<key){
            idx++;
        }
        internalnode->keys.insert(internalnode->keys.begin()+idx, key);
        if(internalnode->keys.size() == order){
            split(internalnode);
        }
        return;
    }
}

void Index::split(Node* node){
    Node* new_node = new Node();
    int mid = node->keys.size()/2;
    int mid_key = node->keys[mid];
    new_node->is_leaf = node->is_leaf;
    new_node->next = node->next;
    new_node->prev = node;
    node->next = new_node;
    if(new_node->next != NULL){
        new_node->next->prev = new_node;
    }
    /*********************** *
     * parents are unhandled *
     * ***********************/

    if(node->is_leaf){ //only have to handle self and parent
        if(node==this->root){ //also handle new root
            Node *new_root = new Node();
            new_root->is_leaf = false;
            node->parent = new_root;
            new_node->parent = new_root;
            //key,values
            new_node->keys.assign(node->keys.begin()+mid,node->keys.end());
            node->keys.erase(node->keys.begin()+mid,node->keys.end());
            new_node->values.assign(node->values.begin()+mid,node->values.end());
            node->values.erase(node->values.begin()+mid,node->values.end());
            new_root->keys.push_back(mid_key);
            new_root->children.push_back(node);
            new_root->children.push_back(new_node);
            this->root = new_root;
            return;
        }else{
            //key,values
            new_node->keys.assign(node->keys.begin()+mid,node->keys.end());
            node->keys.erase(node->keys.begin()+mid,node->keys.end());
            new_node->values.assign(node->values.begin()+mid,node->values.end());
            node->values.erase(node->values.begin()+mid,node->values.end());
            new_node->parent = node->parent;
            node->parent->children.push_back(new_node);
            insert_key(node->parent,mid_key,new_node);
            return;
        }
    }else{ //not leaf, have to handle self,parent,children
        if(node->is_leaf==false){
             //keep doing from here   
        }else{
            
        }
    }
}