#include "opfile.h"

vector<int> readGlobaData(string str) {
    vector<string> lines = read_txtFile(str);
    vector<int> ret;
    for(auto & line : lines) {
        string word;
        std::istringstream iss(line);
        iss >> word;
        ret.push_back(stoi(word));
    }
    return ret;
}

vector<vector<double>> readCompanyData(string str) {
    vector<vector<double>> ret;
    vector<string> lines = read_csvFile(str);
    for(auto & line : lines) {
        vector<double> doubleLine;
        string word;
        std::istringstream iss(line);
        while(iss >> word) {
            double nu = atof(word.c_str());
            doubleLine.push_back(nu);
        }

        ret.push_back(doubleLine); 
    }

    return ret;
}

vector<string> read_txtFile(string fileName) {
    vector<string> ret;
    ifstream read_file;
    fileName = "../data/" + fileName;
    read_file.open(fileName, ios::binary);
    
    string line;
    string comment_str = "#";
    while(getline(read_file, line))
    {
        line.erase(0, line.find_first_not_of(" "));     // Remove the space at the beginning of the line
        line.erase(line.find_last_not_of(" ") + 1);     // Remove the space at the end of the line

        
        int n_comment_start = line.find_first_of(comment_str);  // Find the location of the comment
        if (n_comment_start != std::string::npos)      
            line.erase(n_comment_start);                // Delete comment

        
        if(line.size() > 0) {
            ret.push_back(line); 
        }
    }

    return ret;
}

vector<string> read_csvFile(string fileName) {
    vector<string> ret;
    // read file
    ifstream read_file;
    fileName = "../data/" + fileName + "_csv.csv";
    read_file.open(fileName, ios::in);

    string lineStr;
    getline(read_file, lineStr);    // Skip the first line
    while (getline(read_file, lineStr)) {

        stringstream ss(lineStr);
        string str;
        string lineArray = " ";

        // Separated by comma
        getline(ss, str, ',');      // Skip the first column
        while (getline(ss, str, ','))  lineArray = lineArray + str + " ";
        ret.push_back(lineArray);
    }

    return ret;
}

