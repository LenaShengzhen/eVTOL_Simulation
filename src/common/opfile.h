#ifndef OPFILE_H
#define OPFILE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

vector<int> readGlobaData(string str);
vector<vector<double>> readCompanyData(string str);
vector<string> read_txtFile(string fileName);

vector<string> read_csvFile(string fileName);

#endif
