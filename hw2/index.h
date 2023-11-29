#ifndef INDEX_H_
#define INDEX_H_

#include <iostream>
#include <fstream>

#include <vector>
#include <utility>
#include <algorithm>

#include "index.h"

using namespace std;

void writefile(string filename, vector<int> data);

//order 20 B+ tree
const int order = 20;
struct Node{
    int num_keys;
    int keys[order-1];
    Node* children[order];
    Node* parent;
    bool is_leaf;
    Node* next;
    Node* prev;

    Node(){
        num_keys = 0;
        parent = NULL;
        is_leaf = false;
        next = NULL;
        prev = NULL;
    };
};

class Index{
private:
    vector<int> key;
    vector<int> value;
    int num_rows;
    Node* root;
public:
    Index(int num_rows, vector<int> key, vector<int> value);
    void key_query(vector<int> query_kes);
    void range_query(vector<pair<int,int>> query_pairs);
    void clear_index();
    void build_b_plus_tree();
    ~Index();
};

#endif // INDEX_H_