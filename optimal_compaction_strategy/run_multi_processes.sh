if ! [ $# -eq 7 ]; then
    echo 'in this shell script, there will be seven parameters, which are:'
    echo '1. the number of inserts in the workload'
    echo '2. the number of updates in the workload'
    echo '3. the number of deletes in the workload'
    echo '4. the path to save the experiments result'
    echo '5. the number of times to run kEnumerate for each workload'
    echo '6. the number of different workloads'
    echo '7. the number of processes'
    exit 1
fi

default_rocksdb_path=/mnt/rocksdb
default_experiment_log_path=experiment

for i in $(seq 1 $7)
do
    ./run_for_a_type.sh $1 $2 $3 $4 $5 $6 $default_rocksdb_path$i $default_experiment_log_path$i $i &
done

