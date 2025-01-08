#!/bin/bash
#TEST_STR='str'
TEST_MNT='/home/test/mnt'

prepare_test() {
#    if [ ! -d $TEST_STR ]; then
#        mkdir $TEST_STR
#    elif [ -d $TEST_STR ]; then
#        echo "WARN: Directory already exists, using for testing puroposes";
#    else
#        echo "ERROR: '$TEST_STR' shall be a directory"
#    fi

    if [ ! -d $TEST_MNT ]; then
        mkdir $TEST_MNT
    elif [ -d $TEST_MNT ]; then
        echo "WARN: Directory already exists, using for testing puroposes";
    else
        echo "ERROR: '$TEST_MNT' shall be a directory"
    fi
}

clean_test() {
    rm -rf $TEST_MNT/*
}

# test basic operations (sync way)
do_test() {
    sudo -u test mkdir ${TEST_MNT}/dirA ${TEST_MNT}/dirB
    sudo -u test mkdir ${TEST_MNT}/dirA/subdir
    sudo -u test echo "File A" > ${TEST_MNT}/fileA

    sudo -u test mkfifo ${TEST_MNT}/pipeA
    #sudo -u test rmdir ${TEST_MNT}/tagA/subdir
    #sudo -u test rmdir ${TEST_MNT}/tagA ${TEST_MNT}/tagB
}

do_test_find() {
  	LATES_DUMP=$(find "${APPDUMP_DIR}" -name *.xml -type f | sort | tail -n 1)

}

case $1 in
    'do')
        do_test
    ;;
    'clean')
        clean_test
    ;;
    *)
        echo "Usage: $0 prepare"
    ;;
esac

exit 0
