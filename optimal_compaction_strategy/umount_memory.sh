if ! [ $# -eq 1 ]; then
    echo 'in this shell script, there will be one parameters, which are:'
    echo '1. the path of to mount'
    exit 1
fi

sudo umount $1
