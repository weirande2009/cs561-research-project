#pragma once
#include <vector>
#include <unordered_map>

struct VersionNode{
    // id of the version
    size_t id;
    // id of parent version
    size_t parent_id;
    // indicate the version of the files
    size_t hash_value;
    // total WA until this version
    size_t total_WA;
    // indicate the files that have been chosen
    // children[i] is the index of the chosen file in this version and value is a pair of 
    // corresponding WA and the id of the new version after compaction
    // the real node will be stored in an array
    std::vector<std::pair<size_t, size_t>> chosen_children;
    // the number of files in this version
    int file_num;
    // tag for whether this version is a leaf
    bool is_leaf;
};

// version forest of a level
class LevelVersionForest{
private:
    // a list of version nodes
    std::vector<VersionNode> version_nodes;
    // map hash value of a version to the id of the VersionNode in version_nodes
    std::unordered_map<size_t, size_t> hash_to_id;
    // version id of the next compaction
    size_t current_version_id;
    // minimum WA of current combination selections from this node to the leaf
    size_t min_WA;

    // load forest from file
    void LoadFromFile();

    // dump forest to file
    void DumpToFile();

    // add when doesn't exist the version node of the hash value
    void AddNode(size_t hash_value, int file_num);


public:
    LevelVersionForest();
    ~LevelVersionForest();

    // get the index of the file in the current version
    size_t GetCompactionFile(size_t hash_value, int file_num);
};

// Contain forests for each level
class VersionForests{
private:
    // file path of the version node in disk
    std::string file_path;
    // version forests for each level
    std::vector<LevelVersionForest> level_version_forests;

public:
    VersionForests(const std::string& file_path);
    ~VersionForests();

    // get the index of the file in the current version of a certain level
    size_t GetCompactionFile(int level, size_t hash_value, int file_num);
};



