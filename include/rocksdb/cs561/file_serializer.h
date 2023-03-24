#pragma once

#include "db/version_edit.h"

namespace ROCKSDB_NAMESPACE {

struct FileMetaData;
class InternalKey;

// This struct is used to serialize the file metadata into a string
struct FileTag{
    InternalKey smallest;  // Smallest internal key served by table
    InternalKey largest;   // Largest internal key served by table
    uint64_t num_entries = 0;
};

class FileSerializer{
private:

public:
    FileSerializer();
    ~FileSerializer();

    // Serialize the file metadata into a string by using FileTag
    // @param file: the file metadata that is going to be serialized
    std::string Serialize(const rocksdb::FileMetaData* file);

    // Check whether the two versions are the same
    // @param version1: the first version
    // @param version2: the second version
    bool IsSameVersion(std::vector<std::string>& version1, std::vector<std::string>& version2);

};

}  // namespace ROCKSDB_NAMESPACE