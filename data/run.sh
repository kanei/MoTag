#!/bin/sh

# run: nohup ./[computername].sh &

id=pcnlp5
ntrain=30000
ntest=7000
iter=250
eps="0.1"
reg="L2"
params="-p lbfgs.max_iterations=$iter -p lbfgs.delta=$eps "
#params="-p algorithm=sgd -p sgd.delta=$eps"
#WITHOUT u13 U04 U03 U23 U52 U33 U42 U02 U14

#remove existing data
rm $id.log $id.result $id.train $id.test $id.model

ulimit -t unlimited
ulimit -v unlimited
ulimit -s unlimited

date > $id.log
#prepare data
python2.7 $id.py -n $ntrain <train-3-ann.cst > $id.train
#run training
~/git/MoTag/test/crfsuite-0.10/bin/crfsuite learn -p regularization=$reg $params -m $id.model $id.train >>$id.log 
date >>$id.log
#prepare data for testing
python2.7 $id.py -n $ntest <dtest-3-ann.cst > $id.test 
#run testing
~/git/MoTag/test/crfsuite-0.10/bin/crfsuite tag -q -t -m $id.model $id.test >$id.result 
date >> $id.result
mail -s \'$id\' marek.vantuch@gmail.com < $id.log

