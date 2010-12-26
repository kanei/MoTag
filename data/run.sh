#!/bin/sh

# run: nohup ./run.sh -d [pc_id] -t [train_count] -e [test_count] -i [iterations] -p [tag_parts] -r [reg]&
# default values:        ^pcnlp3    ^10000           ^7000           ^50             ^111110011     ^L1

id=pcnlp3
ntrain=10000
ntest=7000
iter=50
parts=111110011
reg=L1


while getopts "d:t:e:i:p:r:" optname
do
  case "$optname" in 
  "d")
  id=$OPTARG
  ;;
  "t")
  ntrain=$OPTARG
  ;; 
  "e")
  ntest=$OPTARG
  ;; 
  "i")
  iter=$OPTARG
  ;; 
  "p")
  parts=$OPTARG
  ;; 
  "r")
  reg=$OPTARG
  ;;
  esac
done

#remove existing data
rm $id.log $id.result $id.train $id.test $id.model $id.test

date > $id.log
#prepare data
python2.7 $id.py -n $ntrain <train-3-ann.cst > $id.train
#run training
/var/mnt/eva/home/users/xv/xvantu00/git/MoTag/test/crfsuite-0.10/bin/crfsuite learn -p regularization=$reg -p lbfgs.max_iterations=$iter -m $id.model $id.train >>$id.log 
date >>$id.log
#prepare data for testing
python2.7 $id.py -n $ntest <dtest-3-ann.cst > $id.test 
#run testing
/var/mnt/eva/home/users/xv/xvantu00/git/MoTag/test/crfsuite-0.10/bin/crfsuite tag -q -t -m $id.model $id.test >$id.result 
date >> $id.result

