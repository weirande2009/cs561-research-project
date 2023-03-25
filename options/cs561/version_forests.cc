//
// Created by bruce on 3/25/2023.
//

#include <cassert>


#include "rocksdb/cs561/version_forests.h"

void LevelVersionForest::AddNode(size_t hash_value, int file_num){
    // the id is the index in version_nodes which is the size of current version_nodes
    version_nodes.emplace_back(VersionNode(version_nodes.size(), last_version_id, hash_value, file_num));
    // after emplace back, the index become the version_nodes.size()-1
    hash_to_id.try_emplace(hash_value, version_nodes.size()-1);
    // update the index of the new node to the children of last node
    version_nodes[last_version_id].chosen_children.back() = version_nodes.size()-1;
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
        // if the number of children of the child is less than the number of files, this is the file we want
        if(version_nodes[child].chosen_children.size() < static_cast<size_t>(version_nodes[child].file_num)){
            // the size will the index of the next 
            compaction_file_index = i;
            break;
        }
    }
    // check whether we have found the file to compact
    if(compaction_file_index == std::numeric_limits<size_t>::max()){ // if we didn't find one, we need to use the next file
        // if we have selected all files, return long max
        if(version_nodes[index].chosen_children.size() == static_cast<size_t>(version_nodes[index].file_num)){
            return std::numeric_limits<size_t>::max();
        }
        // size is the next index
        compaction_file_index = version_nodes[index].chosen_children.size();
    }
    // add child, use long max as temporary id
    version_nodes[index].chosen_children.push_back(std::numeric_limits<size_t>::max());
    
    return compaction_file_index;
}

size_t VersionForests::GetCompactionFile(int level, size_t hash_value, int file_num){
    return level_version_forests[level].GetCompactionFile(hash_value, file_num);
}

#include "../../include/rocksdb/cs561/version_forests.h"
#include "../../include/rocksdb/cs561/file_serializer.h"

