#!/bin/bash
TEST_STR='str'
TEST_MNT='mnt'

prepare_test() {
    if [ ! -d $TEST_STR ]; then
        mkdir $TEST_STR
    elif [ -d $TEST_STR ]; then
        echo "WARN: Directory already exists, using for testing puroposes";
    else
        echo "ERROR: '$TEST_STR' shall be a directory"
    fi

    if [ ! -d $TEST_MNT ]; then
        mkdir $TEST_MNT
    elif [ -d $TEST_MNT ]; then
        echo "WARN: Directory already exists, using for testing puroposes";
    else
        echo "ERROR: '$TEST_MNT' shall be a directory"
    fi
}

clean_test() {
    rm -rf $TEST_STR $TEST_MNT
}

do_test() {
    cd $TEST_MNT
    mkdir tagA tagB
    mkdir tagA/tagB
    echo "Belongs to A & B." > tagA/tagB/ABfile

    mkdir tagB/tagA

    cmp tagA/tagB/ABfile tagB/tagA/ABfile
    mv tagA/tagB/ABfile tagA/tagB/ABfileMV
    cmp tagA/tagB/ABfileMV tagB/tagA/ABfileMV

    rm tagA/tagB/ABfileMV
    rmdir tagA/tagB
    rmdir tagA tagB

    cd -
}

# script launched from current directory
if [ $(dirname $0) != '.' ] ; then
    echo "Please launch this script directly from the directory where it is located"

    exit 1;
fi

case $1 in
    'prepare')
        prepare_test
    ;;
    'clean')
        clean_test
    ;;
    *)
        echo "Usage: $0 prepare"
    ;;
esac

exit 0
