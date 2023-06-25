#include <fstream>
#include <iostream>

inline static const std::string DUMP_FILEPATH_LEVEL1 = "experiments/history/picking_history_level1";

bool CheckFinishEnumeration(){
    std::ifstream f(DUMP_FILEPATH_LEVEL1);
    // the fully enumerating sign is on the fourth line, fifth value
    size_t line = 4;
    size_t column = 5;
    // skip line
    std::string skip_line;
    // skip the first line
    std::getline(f, skip_line);
    // version_nodes
    size_t vn_size;
    f >> vn_size;
    if(vn_size == 0){
        return false;
    }
    std::getline(f, skip_line);
    std::getline(f, skip_line);
    for(size_t i=0; i<column-1; i++){
        f >> skip_line;
    }
    std::string fully_enumerated;
    f >> fully_enumerated;
    if(fully_enumerated == "yes"){
        return true;
    }
    return false;
}

int main(int argc, char* argv[]){
    if(CheckFinishEnumeration()){
        std::cout << "Finish enumerating" << std::endl;
        // create a file to tell run_all script to stop enumerating
        std::ofstream f("over");
        f << "over" << std::endl;
        f.close();
    }
    return 0;
}
