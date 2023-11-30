#ifndef INDEX_H_
#define INDEX_H_

#include <iostream>
#include <fstream>

#include <vector>
#include <utility>
#include <limits>

using namespace std;

const int order = 1022;
void writefile(string filename, vector< int > data);

//order 1020 B+ tree
struct Node{
    vector< int > keys;
    vector< int > values; //use only by leaf node
    vector<Node*> children;
    Node* parent;
    bool is_leaf;
    Node* next;
    Node* prev;

    Node(){
        parent = nullptr;
        next = nullptr;
        prev = nullptr;
        //+1 for split
        keys.reserve(order+1);
        values.reserve(order+1);
        children.reserve(order+1+1);
    }
};

class Index{
private:
    Node* root;
    vector<Node*> usednodes;
    Node* find_leaf(int key);
    int search_keyidx(Node* leaf,int key);
    int search_key(int key);
    int search_range(int key1, int key2);
    void clearAll(Node* node);
    
    void insert(int key, int value);
    void insert_key(Node* internalnode,int key,Node *new_node);
    void split(Node* node);
public:
    void key_query(vector< int > query_kes);
    void range_query(vector<pair< int , int >> query_pairs);
    void clear_index();
    Index(int num_rows, vector< int > key, vector< int > value);
};

#endif // INDEX_H_