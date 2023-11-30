#ifndef INDEX_H_
#define INDEX_H_

#include <iostream>
#include <fstream>

#include <vector>
#include <utility>
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
        keys.reserve(order);
        values.reserve(order);
        children.reserve(order+1);
    }

};

class Index{
private:
    Node* root;

    Node* find_leaf(int key);
    void insert_key(int key, int value);
    void split(Node* node);
    void delete_key(int key);
    void merge(Node* node);
    void redistribute(Node* node);
public:
    Index(int num_rows, vector<int> key, vector<int> value);
    void key_query(vector<int> query_kes);
    void range_query(vector<pair<int,int>> query_pairs);
    void clear_index();
    void build_b_plus_tree();
};

#endif // INDEX_H_