#pragma once
#include <vector>
#include <unordered_map>
#include <fstream>

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

    friend std::ostream& operator<< (std::ostream& os, const VersionNode& node) {
        static const std::string DELIM_COMMA = ",";

        os << node.id << DELIM_COMMA <<
              node.parent_id << DELIM_COMMA <<
              node.hash_value << DELIM_COMMA <<
              node.total_WA << DELIM_COMMA;

        os << node.chosen_children.size() << DELIM_COMMA;
        for (const auto& [WA, id]: node.chosen_children) {
            os << WA << DELIM_COMMA << id << DELIM_COMMA;
        }

        os << node.file_num << DELIM_COMMA << node.is_leaf << std::endl;

        return os;
    }

    friend std::istream& operator>> (std::istream& is, VersionNode& node) {
        is >> node.id >> node.parent_id >> node.hash_value >> node.total_WA;
        size_t sz;
        is >> sz;

        node.chosen_children.clear();
        node.chosen_children.reserve(sz);
        std::pair<size_t, size_t> tmp;
        for (size_t i = 0; i < sz; ++ i) {
            is >> tmp.first >> tmp.second;
            node.chosen_children.push_back(tmp);
        }

        is >> node.file_num >> node.is_leaf;

        return is;
    };
};

// version forest of a level
class LevelVersionForest{
private:
    // a list of version nodes
    std::vector<VersionNode> version_nodes;
    // map hash value of a version to the id of the VersionNode in version_nodes
    std::unordered_map<size_t, size_t> hash_to_id;
    // version id of the next compaction
    size_t current_version_id = 0;
    // minimum WA of current combination selections from this node to the leaf
    size_t min_WA = 0;
    // file path of this LevelVersionForest
    // Important: file_path cannot contain ','/'\n'
    std::string file_path;

    // TODO: Peixu
    // load forest from file
    void LoadFromFile() {
        assert(!file_path.empty());

        std::ifstream f(file_path);

        // version_nodes
        size_t vn_size;
        f >> vn_size;
        VersionNode node{};
        version_nodes.clear();
        version_nodes.reserve(vn_size);
        for (size_t i = 0; i < vn_size; ++i) {
            f >> node;
            version_nodes.emplace_back(std::move(node));
        }

        // current_version_id
        f >> current_version_id;

        // min_WA
        f >> min_WA;

        // file_path
        f >> file_path;

        // hash_to_id
        hash_to_id.reserve(version_nodes.size());
        size_t sz = version_nodes.size();
        for (size_t i = 0; i < sz; ++i) {
            auto p = hash_to_id.try_emplace(version_nodes[i].hash_value, i);
            // hash collision is strictly prohibited
            assert(p.second);
        }
    }

    // TODO: Peixu
    // dump forest to file
    void DumpToFile() {

    }

    // TODO: Ran
    // add when doesn't exist the version node of the hash value
    void AddNode(size_t hash_value, int file_num);


public:
    // TODO: Peixu
    explicit LevelVersionForest(const std::string& file_path) {
        this->file_path = file_path;
        LoadFromFile();
    }

    // TODO: Peixu
    ~LevelVersionForest() {
        DumpToFile();
    }

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
    explicit VersionForests(const std::vector<std::string>& level_file_path) {
        for (const auto& path: level_file_path)
            level_version_forests.emplace_back(path);
    }

    // TODO: Peixu
    ~VersionForests() = default;

    // TODO: Ran
    // get the index of the file in the current version of a certain level
    size_t GetCompactionFile(int level, size_t hash_value, int file_num);
};



