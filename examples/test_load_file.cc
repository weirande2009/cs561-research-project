//
// Created by bruce on 4/10/2023.
//

#include <cassert>
#include <fstream>
#include <iostream>

using namespace std;

void LoadFromFile() {
    std::string file_path = "history/picking_history_level0";

    assert(!file_path.empty());

    std::ifstream f(file_path);
    assert(f);
    if (f.eof()) return;

    // version_nodes
    size_t vn_size = -1;
    std::cout << "file_path: " << file_path << std::endl;
    std::cout << "vn_size: " << vn_size << std::endl;
    f >> vn_size;
    std::cout << "vn_size: " << vn_size << std::endl;
//    VersionNode node{};
//    version_nodes.clear();
//    if(vn_size != 0)
//        version_nodes.reserve(vn_size);
//    for (size_t i = 0; i < vn_size; ++i) {
//        f >> node;
//        version_nodes.emplace_back(std::move(node));
//    }
//
//    // hash_to_id
//    if (!version_nodes.empty())
//        hash_to_id.reserve(version_nodes.size());
//    size_t sz = version_nodes.size();
//    for (size_t i = 0; i < sz; ++i) {
//        // hash collision is strictly prohibited
//        assert(hash_to_id.try_emplace(version_nodes[i].hash_value, i).second);
//    }
}

int main() {
    LoadFromFile();

    return 0;
}