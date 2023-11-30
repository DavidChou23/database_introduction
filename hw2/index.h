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
    int values[order-1];
    Node* children[order];
    Node* parent;
    bool is_leaf;
    Node* next;
    Node* prev;

};

class Index{
private:
    vector<int> key;
    vector<int> value;
    int num_rows;
    Node* root;

    Node* find_leaf(int key);
    void insert_key(int key, int value);
    void inserttoleaf(Node* node, int key, int value);
    void split(Node* node);
    void delete_key(int key);
    void deletefromleaf(Node* node, int key);
    void merge(Node* node);
    void redistribute(Node* node);
public:
    Index(int num_rows, vector<int> key, vector<int> value);
    void key_query(vector<int> query_kes);
    void range_query(vector<pair<int,int>> query_pairs);
    void clear_index();
    void build_b_plus_tree();
    ~Index();
};

#endif // INDEX_H_