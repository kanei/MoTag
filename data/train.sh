#!/bin/sh

# run: nohup ./test.sh (computer name) &

id=pcnlp3
ntrain=10000
ntest=5000
iter=50

#remove existing data
rm $id.log $id.result $id.train $id.test $id.model $id.test

ulimit -t unlimited
ulimit -v unlimited
ulimit -s unlimited
date > $id.log
#prepare data
./$id.py -n $ntrain <train-3-ann.cst > $id.train
#run training
/var/mnt/eva/home/users/xv/xvantu00/git/MoTag/test/crfsuite-0.10/bin/crfsuite learn -p regularization=L1 -p lbfgs.max_iterations=$iter -m $id.model $id.train >>$id.log 
date >>$id.log
#prepare data for testing
./$id.py -n $ntest <dtest-3-ann.cst > $id.test
#run testing
/var/mnt/eva/home/users/xv/xvantu00/git/MoTag/test/crfsuite-0.10/bin/crfsuite tag -q -t -m $id.model $id.test >$id.result 
date >> $id.result

