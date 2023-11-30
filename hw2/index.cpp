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
