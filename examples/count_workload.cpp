#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

int main(){
    // opening workload file for the first time
    std::ifstream workload_file;
    workload_file.open("workload.txt");

    unordered_map<char, int> counters;
    int counter = 0;

    while (!workload_file.eof()) {
        char instruction;
        std::string value, key, start_key, end_key;
        workload_file >> instruction;
        counters[instruction]++;
        switch (instruction)
        {
        case 'I': // insert
        case 'U': // update
            workload_file >> key >> value;
            break;

        case 'Q': // probe: point query
            workload_file >> key;
            break;

        case 'S': // scan: range query
            workload_file >> start_key >> end_key;
            break;
        case 'D':
            workload_file >> key;
            break;
        default:
            std::cerr << "ERROR: Case match NOT found !!" << std::endl;
            break;
        }
        counter++;
    }

    cout << "Total operations: " << counter << endl;
    for(auto& p: counters){
        cout << "Operation " << p.first << ": " << p.second << endl;
    }

    return 0;
}