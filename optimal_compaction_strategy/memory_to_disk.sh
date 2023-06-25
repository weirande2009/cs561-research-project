if ! [ $# -eq 2 ]; then
    echo 'in this shell script, there will be two parameters, which are:'
    echo '1. the path of to mount'
    echo '2. the amount of memory'
    exit 1
fi

sudo mkdir $1
sudo mount -t tmpfs -o size=$2 tmpfs $1