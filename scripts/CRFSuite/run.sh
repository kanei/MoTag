#!/bin/sh

# run: nohup ./[computername].sh &

id=computer 		#computername
mail=mail@mail.com 	#email address to send email after completion
dtrain=train_data.cst	#data used for training
dtest=test_data.cst	#data used for testing

crfsuite="./crfsuite-0.10/bin/crfsuite" #address of the crfsuite program

ntrain=1000 	#training count
ntest=7000 	#tagging count
iter=250 	#maximum number of iterations
delta="0.1" 	#delta treshold
reg="L2" 	#regularization

params="-p lbfgs.max_iterations=$iter -p lbfgs.delta=$delta " #lbfgs
#params="-p algorithm=sgd -p sgd.delta=$delta " #sgd

#remove existing data
rm $id.log $id.result $id.train $id.test $id.model

ulimit -t unlimited
ulimit -v unlimited
ulimit -s unlimited

date > $id.log
#prepare data
python2.7 $id.py -n $ntrain < $dtrain > $id.train
#run training
$crfsuite learn -p regularization=$reg $params -m $id.model $id.train >> $id.log 
date >> $id.log
#prepare data for testing
python2.7 $id.py -n $ntest < $dtest > $id.test 
#run testing
$crfsuite tag -q -t -m $id.model $id.test > $id.result 
date >> $id.result
mail -s \'$id\' $mail < $id.log

