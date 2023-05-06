# in this shell script, there will be four parameters, which are:
# 1. the number of inserted in the workload
# 2. the number of updates in the workload
# 3. the number of deletes in the workload
# 4. the path to save the experiments result
# 5. the number of times to run kEnumerate
if ! [ $# -eq 5 ]; then
    echo 'in this shell script, there will be four parameters, which are:'
    echo '1. the number of inserted in the workload'
    echo '2. the number of updates in the workload'
    echo '3. the number of deletes in the workload'
    echo '4. the path to save the experiments result'
    echo '5. the number of times to run kEnumerate'
    exit 1
fi

# reset history and result
mkdir history
mkdir result
echo '18446744073709551615 0' > result/minimum.txt
touch result/minimum.txt
touch result/result.txt
touch log/log.txt
echo '0' > history/picking_history_level0
echo '0' > history/picking_history_level1

# generate workload
./load_gen -I $1 -U $2 -D $3

# compute the number of bytes will be inserted to database
ENTRY_SIZE=8
total_written_bytes=$((($1 + $2) * $ENTRY_SIZE))
# echo $total_written_bytes

# run baseline
./run_baseline.sh $total_written_bytes

# run enumeration
./multi_runs.sh $5 $total_written_bytes

# copy the experiment result to a given folder $4
new_dir_path=$1I+$2U+$3D
mkdir $4
mkdir $4/$new_dir_path
mv history $4/$new_dir_path
mv result $4/$new_dir_path
mv workload.txt $4/$new_dir_path
mv log/log.txt $4/$new_dir_path
