if ! [ $# -eq 4 ]; then
    echo 'in this shell script, there will be four parameters, which are:'
    echo '1. the number of runs'
    echo '2. the number of all inserted bytes'
    echo '3. the path of the rocksdb'
    echo '4. the path of the experiment log'
    exit 1
fi
for i in $(seq 1 $1)
do
    echo 'run' $i
    find $3 -mindepth 1 -delete
    ./simple_example kEnumerateAll $2 $3 $4

    # check whether to stop
    ./check_finish_enumeration

    # check whether over exists
    if [ -e ./over ]; then
        echo 'Stop enumerating'
        rm ./over
        break
    fi
done

echo 'Finish all runs'
