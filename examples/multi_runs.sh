for i in $(seq 1 $1)
do
    echo 'run' $i
    rm -rf /mnt/ramdisk/cs561_project1/
    # rm -rf /tmp/cs561_project1
    ./simple_example kEnumerateAll $2
done