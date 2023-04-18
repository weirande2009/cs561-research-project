//
// Created by bruce on 3/25/2023.
//

#include <cassert>

#include "cs561/version_forests.h"
#include "cs561/cs561_log.h"

// #include "../../include/rocksdb/cs561/version_forests.h"
// #include "../../include/rocksdb/cs561/file_serializer.h"

std::ostream& operator<< (std::ostream& os, const VersionNode& node) {
    static const std::string DELIM_COMMA = " ";

    os << node.id << DELIM_COMMA <<
       node.parent_id << DELIM_COMMA <<
       node.hash_value << DELIM_COMMA <<
       node.file_num << DELIM_COMMA <<
       node.is_leaf << DELIM_COMMA <<
       node.fully_enumerated << DELIM_COMMA;

    os << node.chosen_children.size() << DELIM_COMMA;
    for (size_t child_id: node.chosen_children) {
        os << child_id << DELIM_COMMA;
    }

    os << std::endl;

    return os;
}

std::istream& operator>> (std::istream& is, VersionNode& node) {
    is >> node.id >> node.parent_id >> node.hash_value >> node.file_num >> node.is_leaf >> node.fully_enumerated;
    size_t sz;
    is >> sz;

    node.chosen_children.clear();
    node.chosen_children.reserve(sz);
    size_t tmp;
    for (size_t i = 0; i < sz; ++ i) {
        is >> tmp;
        node.chosen_children.push_back(tmp);
    }

    return is;
}

void LevelVersionForest::LoadFromFile() {
    assert(!file_path.empty());

    std::fstream f(file_path);

    // version_nodes
    size_t vn_size;
    f >> vn_size;
    VersionNode node{};
    version_nodes.clear();
    if(vn_size != 0)
        version_nodes.reserve(vn_size);
    for (size_t i = 0; i < vn_size; ++i) {
        f >> node;
        version_nodes.emplace_back(std::move(node));
    }

    // hash_to_id
    if(version_nodes.size() != 0)
        hash_to_id.reserve(version_nodes.size());
    size_t sz = version_nodes.size();
    std::cout << "loaded node number: " << sz << std::endl;
    for (size_t i = 0; i < sz; ++i) {
        hash_to_id[version_nodes[i].hash_value] = i;
        // std::cout << version_nodes[i].hash_value << " " << i << std::endl;
        // hash collision is strictly prohibited
        // assert(hash_to_id.try_emplace(version_nodes[i].hash_value, i).second);
        // std::cout << hash_to_id[version_nodes[i].hash_value] << std::endl;
    }
}

void LevelVersionForest::DumpToFile() {
    static const std::string DELIM_COMMA = " ";

    assert(!file_path.empty());

    std::ofstream f(file_path);

    // version_nodes
    size_t vn_size = version_nodes.size();
    f << vn_size << std::endl;
    for (const auto& vn: version_nodes) {
        f << vn;
    }

    // no need for dumping hash_to_id
}

void LevelVersionForest::AddNode(size_t hash_value, int file_num){
    // check whether hash_value is a new version
    assert(hash_to_id.find(hash_value) == hash_to_id.end());
    // the id is the index in version_nodes which is the size of current version_nodes
    version_nodes.emplace_back(VersionNode(version_nodes.size(), last_version_id, hash_value, file_num));
    // FIXME: it may not emplace to the hashmap
    // after emplace back, the index become the version_nodes.size()-1
    // hash_to_id.insert(hash_value, version_nodes.size()-1);
    hash_to_id[hash_value] = version_nodes.size()-1;
    // we should check whether there is a last version, if this is the first compaction, there will no
    // last version and we should not set the index
    if(last_version_id != std::numeric_limits<size_t>::max()){
        assert(!version_nodes[last_version_id].chosen_children.empty());
        // update the index of the new node to the children of last node
        version_nodes[last_version_id].chosen_children.back() = version_nodes.size()-1;
    }
}

size_t LevelVersionForest::GetCompactionFile(size_t hash_value, int file_num){
    // first check whether the version of hash_value already exists
    if(hash_to_id.find(hash_value) == hash_to_id.end()){
        // create a new version
        AddNode(hash_value, file_num);
    }
    size_t index = hash_to_id[hash_value];
    // iterate the file that has been selected and check whether the children node
    // has already iterate over all its files. If not, find the best as the compaction
    // file
    size_t compaction_file_index = std::numeric_limits<size_t>::max();
    for(size_t i=0; i<version_nodes[index].chosen_children.size(); i++){
        size_t child = version_nodes[index].chosen_children[i];
        // if child is long max, it's a leaf, we can't choose.
        if(child == std::numeric_limits<size_t>::max()){
            continue;
        }
        // if the child is still not fully enumerated, we still want to go to this child
        if(!version_nodes[child].fully_enumerated){
            // the size will the index of the next 
            compaction_file_index = i;
            break;
        }
    }
    // check whether we have found the file to compact
    if(compaction_file_index == std::numeric_limits<size_t>::max()){ // if we didn't find one, we need to use the next file
        // if we have selected all files, return long max
        if(version_nodes[index].chosen_children.size() == static_cast<size_t>(version_nodes[index].file_num)){
            version_nodes[index].fully_enumerated = true;
            CS561Log::Log("Version: " + std::to_string(hash_value) + "All files has been selected");
            return std::numeric_limits<size_t>::max();
        }
        // size is the next index
        compaction_file_index = version_nodes[index].chosen_children.size();
        // add child, use long max as temporary id
        version_nodes[index].chosen_children.push_back(std::numeric_limits<size_t>::max());
    }
    // set the last version
    last_version_id = index;
    
    return compaction_file_index;
}

LevelVersionForest::LevelVersionForest(const std::string& fp): file_path(fp) {
    std::cout << "Initialize LevelVersionForest" << std::endl;
    LoadFromFile();
}

LevelVersionForest::~LevelVersionForest() noexcept {
    DumpToFile();
}


VersionForests::VersionForests(const std::vector<std::string>& level_file_path) {
    std::cout << "Initialize VersionForests" << std::endl;
    for (const auto& path: level_file_path)
        level_version_forests.emplace_back(LevelVersionForest(path));
}

size_t VersionForests::GetCompactionFile(int level, size_t hash_value, int file_num){
    return level_version_forests[level].GetCompactionFile(hash_value, file_num);
}

void VersionForests::DumpToFile(){
    for(auto& lvf: level_version_forests){
        lvf.DumpToFile();
    }
}

