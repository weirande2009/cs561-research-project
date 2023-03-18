#pragma once

#include "db/version_set.h"

class PickingHistoryCollector{

private:


public:
    PickingHistoryCollector();
    ~PickingHistoryCollector();

    // Add one selection to history (Maybe later we need to also collect the corresponding WA of this selection)
    // @param temp: the current file version
    // @param index: the index of the file that is selected
    void AddPickingFile(std::vector<rocksdb::Fsize>& temp, int index);

    // Check whether the index of the current file version is in the history
    // @param temp: the current file version
    // @param index: the index of the file that is going to be checked
    // @return: true if the index is in the history, false otherwise
    bool IsInHistory(std::vector<rocksdb::Fsize>& temp, int index);

};