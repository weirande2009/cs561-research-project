if ! [ $# -eq 7 ]; then
    echo 'in this shell script, there will be seven parameters, which are:'
    echo '1. the number of inserted in the workload'
    echo '2. the number of updates in the workload'
    echo '3. the number of deletes in the workload'
    echo 'notes: if all three numbers above are zero, we are going use the existed workload.txt rather than generate a new one.'
    echo '4. the path to save the experiments result'
    echo '5. the number of times to run kEnumerate'
    echo '6. the path of rocksdb'
    echo '7. the path of the experiment log, also can be seen as experiment path'
    exit 1
fi

# reset history and result
# if [ -d $7 ]; then
#     rm -rf $7
# fi
# mkdir $7
if [ ! -d $7 ]; then
    mkdir $7
fi
if [ ! -d $7/history ]; then
    mkdir $7/history
fi
echo '18446744073709551615 0' > $7/minimum.txt
touch $7/log.txt
touch $7/version_info.txt
touch $7/out.txt
echo -e 'Number of nodes\n0' > $7/history/picking_history_level0
echo -e 'Number of nodes\n0' > $7/history/picking_history_level1

# generate workload, if there is already a workload, don't generate a new one
if [ ! -e $7/workload.txt ]; then
    ./load_gen -I $1 -U $2 -D $3 --DIR $7 > $7/out.txt
fi

# check whether workload.txt exists
if [ ! -e $7/workload.txt ]; then
    echo "workload.txt does not exist"
    exit -1
fi

# Run count_workload to compute the number of bytes that will be inserted to database
# and write the result into file "workload_count.txt"
./count_workload $7 > $7/out.txt

# The file contains one lines: "total_written_bytes=xxx". After reading, there will be a 
# variable $total_written_bytes
# Read the file line by line
while IFS='=' read -r key value; do
  # Set the variable based on the key-value pair
  eval "$key=$value"
done < $7/"workload_count.txt" 

# Now you can use the variables in your script
# echo "$total_written_bytes"

# rm $7/workload_count.txt

# run baseline
./run_baseline.sh $total_written_bytes $6 $7 > $7/out.txt

# run enumeration
./multi_runs.sh $5 $total_written_bytes $6 $7 > $7/out.txt

# copy the experiment result to a given folder $4
# new_dir_path=$1I+$2U+$3D
new_dir_path=`date +"%Y-%m-%d-%H:%M:%S"`
if [ ! -d $4 ]; then
    mkdir $4
fi
mkdir $4/$new_dir_path
mv $7 $4/$new_dir_path
