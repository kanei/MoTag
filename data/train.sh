#!/bin/sh

# run: nohup ./test.sh (computer name) &

#remove existing data
rm $1.log $1.result $1.train $1.test $1.model $1.test

ulimit -t unlimited
ulimit -v unlimited
ulimit -s unlimited
date > $1.log
#prepare data
./$1.py <train-3-ann.cst > $1.train
#run training
/var/mnt/eva/home/users/xv/xvantu00/git/MoTag/test/crfsuite-0.10/bin/crfsuite learn -p regularization=L1 -p lbfgs.max_iterations=50 -m $1.model $1.train >>$1.log 
date >>$1.log
#prepare data for testing
./$1.py <dtest-3-ann.cst > $1.test
#run testing
/var/mnt/eva/home/users/xv/xvantu00/git/MoTag/test/crfsuite-0.10/bin/crfsuite tag -q -t -m $1.model $1.test >$1.result 
date >> $1.result

