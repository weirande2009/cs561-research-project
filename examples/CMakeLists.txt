cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

set(ROCKSDB_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../librocksdb.a)
set(OTHERS_LIB pthread rt dl snappy gflags z bz2 lz4 zstd)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(..)
include_directories(../include)

#add_executable(cs561 cs561/FileSerializer.cc cs561/FileSerializer.h cs561/PickingHistoryCollector.cc cs561/PickingHistoryCollector.h)
#target_link_libraries(cs561
#  ${ROCKSDB_LIB} ${OTHERS_LIB})

add_executable(simple_example
  simple_example.cc)
target_link_libraries(simple_example
  ${ROCKSDB_LIB} ${OTHERS_LIB})

add_executable(column_families_example
  column_families_example.cc)
target_link_libraries(column_families_example
  ${ROCKSDB_LIB} ${OTHERS_LIB})

add_executable(compact_files_example
  compact_files_example.cc)
target_link_libraries(compact_files_example
  ${ROCKSDB_LIB} ${OTHERS_LIB})

add_executable(c_simple_example
  c_simple_example.c)
target_link_libraries(c_simple_example

  ${ROCKSDB_LIB} ${OTHERS_LIB})
add_executable(optimistic_transaction_example
  optimistic_transaction_example.cc)
target_link_libraries(optimistic_transaction_example
  ${ROCKSDB_LIB} ${OTHERS_LIB})

add_executable(transaction_example
  transaction_example.cc)
target_link_libraries(transaction_example
  ${ROCKSDB_LIB} ${OTHERS_LIB})

add_executable(compaction_filter_example
  compaction_filter_example.cc)
target_link_libraries(compaction_filter_example
  ${ROCKSDB_LIB} ${OTHERS_LIB})

add_executable(options_file_example
  options_file_example.cc)
target_link_libraries(options_file_example
  ${ROCKSDB_LIB} ${OTHERS_LIB})

add_executable(multi_processes_example
  EXCLUDE_FROM_ALL
  multi_processes_example.cc)
target_link_libraries(multi_processes_example
  ${ROCKSDB_LIB} ${OTHERS_LIB})
