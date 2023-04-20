for i in {1..640}
do
    echo 'run' $i
    # rm -rf /mnt/ramdisk/cs561_project1/
    rm -rf /tmp/cs561_project1
    ./simple_example
done