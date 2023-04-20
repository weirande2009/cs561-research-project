for i in {1..60}
do
    echo 'run' $i
    # rm -rf /mnt/ramdisk/cs561_project1/
    rm -rf /tmp/cs561_project1
    ./simple_example
done