#include <iostream>
#include <fstream>

#include <vector>
#include <utility>
#include <algorithm>

#include "index.h"

using namespace std;

void writefile(string filename, vector<int> data){
    ofstream outfile;
    //overwrite the file
    outfile.open(filename+".txt", ios::out | ios::trunc);
    for (long long i = 0; i < data.size(); i++){
        outfile << data[i] << endl;
    }
    outfile.close();
};

//order 20 B+ tree
Index::Index(int num_rows, vector<int> key, vector<int> value){
    this->num_rows = num_rows;

    //sort (key,value) by value ascending
    vector<pair<int,int>> key_value;
    key_value.reserve(num_rows);
    for (int i = 0; i < num_rows; i++){
        key_value.push_back(make_pair(key[i],value[i]));
    }
    sort(key_value.begin(), key_value.end(), [](const pair<int,int> &left, const pair<int,int> &right) {
        return left.second < right.second;
    });
    vector<int> sorted_key;
    sorted_key.reserve(num_rows);
    vector<int> sorted_value;
    sorted_value.reserve(num_rows);
    for (int i = 0; i < num_rows; i++){
        sorted_key.push_back(key_value[i].first);
        sorted_value.push_back(key_value[i].second);
    }
    this->key = sorted_key;
    this->value = sorted_value;
    //free memory
    key_value.clear();
    build_b_plus_tree();
};

void Index::build_b_plus_tree(){
    //build B+ tree
    //initialize root
    root = new Node();
    
    //keep doing
}