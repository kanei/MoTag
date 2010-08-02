#!/bin/sh

#usage ./test.sh filename

rm $1.test $1.result
./$1.py <dtest-3-ann.cst >$1.test
#/var/mnt/eva/home/users/xv/xvantu00/git/MoTag/test/crfsuite-0.10/bin/crfsuite tag -q -t -m $1.model $1.test
nohup /var/mnt/eva/home/users/xv/xvantu00/git/MoTag/test/crfsuite-0.10/bin/crfsuite tag -q -t -m $1.model $1.test >$1.result
