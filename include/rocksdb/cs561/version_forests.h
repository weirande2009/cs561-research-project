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
    // size_t total_WA;

    // indicate the files that have been chosen
    // the index of the vector is the index of the chosen file in this version and value is 
    // the id of the new version after compaction. The real node will be stored in an array 
    // in LevelVersionForest
    std::vector<size_t> chosen_children;

    // the number of files in this version
    int file_num;
    
    // tag for whether this version is a leaf, when constructing, it's default to be true
    // in case of there is not compaction on it
    bool is_leaf;

    VersionNode() {}
    VersionNode(size_t id, size_t parent_id, size_t hash_value, int file_num) : id(id), parent_id(parent_id), hash_value(hash_value), file_num(file_num) {}
};

// version forest of a level
class LevelVersionForest{
private:
    // a list of version nodes
    std::vector<VersionNode> version_nodes;
    // map hash value of a version to the id of the VersionNode in version_nodes
    std::unordered_map<size_t, size_t> hash_to_id;
    // version id of the last compaction
    size_t last_version_id;
    // minimum WA of current combination selections from this node to the leaf
    size_t min_WA;

    // TODO: Peixu
    // load forest from file
    void LoadFromFile();

    // TODO: Peixu
    // dump forest to file
    void DumpToFile();

    // TODO: Ran
    // add when doesn't exist the version node of the hash value
    // this will only happen when 
    void AddNode(size_t hash_value, int file_num);


public:
    // TODO: Peixu
    LevelVersionForest(const std::string& file_path);

    // TODO: Peixu
    ~LevelVersionForest();

    // TODO: Ran
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
    // TODO: Peixu
    VersionForests(const std::vector<std::string>& level_file_path);
    // TODO: Peixu
    ~VersionForests();

    // TODO: Ran
    // get the index of the file in the current version of a certain level
    size_t GetCompactionFile(int level, size_t hash_value, int file_num);
};



