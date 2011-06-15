#!/bin/sh
DATE=$(date +%y-%m-%d_%H-%M)
SOURCE="test.csts" #input data
CRFPARAM="-f 3 -e 0.05 -p 2 -c 10 -H 5" #parameters to be pased to CRF++
TRAINAMOUNT=5000 #number of sentences used for training
TESTAMOUNT=1500 #number of sentences used for testing

#defines which columns to use in which steps
FIRST="110000000000000"
SECOND="001100000000000"
THIRD="000011100000000"

#define templates - must be placed in template foulder
FTEMPLATE="template3"
STEMPLATE="template4"
TTEMPLATE="template5"

#write all things which might be important for future analysis to log and screen
echo "CRF STEP AFTER STEP RUNNING by xvantu00" | tee log
echo "------------------------------------------------------------" | tee -a log
echo "CRF++ params: $CRFPARAM" | tee -a log
echo "Training sentences: $TRAINAMOUNT" | tee -a log
echo "Testing sentences: $TESTAMOUNT" | tee -a log

#Recreates training and testing data
if [ "$1" = "n" ] 
then 
  echo "Rebuilding input data"
  ./featureGenerator/qtProj/qtProj -n $TRAINAMOUNT -s 0 < $SOURCE > ./cz/train.data
  ./featureGenerator/qtProj/qtProj -n $TESTAMOUNT -s $TRAINAMOUNT < $SOURCE > ./cz/test.data

fi

#create new dir for saving test results
cd cz/
mkdir $DATE
cd $DATE

for I in $(seq 1 1);
do
  #--------------------------------STEP 1--------------------------------------
  echo "time: $(date +%D-%T)" | tee -a log
  echo "STEP 1: Using $FIRST" | tee -a log
  echo "Preparing data" | tee -a log
  
  #copy templates to working directory
  cp ../../templates/$FTEMPLATE step1.template

  #prepare data for first step
  cat ../train.data | awk -f ../../parse.awk D="../train.data" F=$FIRST S=4 > step1.train  
  cat ../test.data | awk -f ../../parse.awk D="../test.data" F=$FIRST S=4 > step1.test

  echo "Running CRF++ on data" | tee -a log
  ../../CRF/crf_learn step1.template step1.train step1.model $CRFPARAM | tee -a log
  echo "Using trained model on data for next step" | tee -a log
  ../../CRF/crf_test -m step1.model step1.train >step1.trainf
  ../../CRF/crf_test -m step1.model step1.test >step1.testf

  #success rate check
  cat step1.testf | awk 'BEGIN{corr=0;lines=0}
  {if(NF>0){lines++; if($(NF-1) == $(NF)) corr = corr+1;}}
  END{print"Words: " lines "\nCorrectly assigned: " corr "\nSuccess rate:" corr/lines*100 "%\n"}'

  #-----------------------------------STEP 2-----------------------------------
  echo "time: $(date +%D-%T)" | tee -a log
  echo "STEP 2: Using $SECOND" | tee -a log
  echo "Preparing data" | tee -a log

  #copy templates to working directory
  cp ../../templates/$STEMPLATE step2.template

 
  cat step1.trainf | awk -f ../../parse.awk D="../train.data" F=$SECOND S=4 > step2.train  
  cat step1.testf | awk -f ../../parse.awk D="../test.data" F=$SECOND S=4 > step2.test

  echo "Running CRF++ on data" | tee -a log
  ../../CRF/crf_learn step2.template step2.train step2.model $CRFPARAM | tee -a log
  echo "Using trained model on data for next step" | tee -a log
  ../../CRF/crf_test -m step2.model step2.train >step2.trainf
  ../../CRF/crf_test -m step2.model step2.test >step2.testf 

  #success rate check
  cat step2.testf | awk 'BEGIN{corr=0;lines=0}
  {if(NF>0){lines++; if($(NF-1) == $(NF)) corr = corr+1;}}
  END{print"Words: " lines "\nCorrectly assigned: " corr "\nSuccess rate:" corr/lines*100 "%\n"}'
  
  #-----------------------------------STEP 3-----------------------------------
  echo "time: $(date +%D-%T)" | tee -a log
  echo "STEP 3: Using $THIRD" | tee -a log
  echo "Preparing data" | tee -a log

  #copy templates to working directory
  cp ../../templates/$TTEMPLATE step3.template

 
  cat step2.trainf | awk -f ../../parse.awk D="../train.data" F=$THIRD S=5 > step3.train  
  cat step2.testf | awk -f ../../parse.awk D="../test.data" F=$THIRD S=5 > step3.test

  echo "Running CRF++ on data" | tee -a log
  ../../CRF/crf_learn step3.template step3.train step3.model $CRFPARAM | tee -a log
  echo "Using trained model on data for next step" | tee -a log
  ../../CRF/crf_test -m step3.model step3.train >step3.trainf
  ../../CRF/crf_test -m step3.model step3.test >step3.testf 

  #success rate check
  cat step3.testf | awk 'BEGIN{corr=0;lines=0}
  {if(NF>0){lines++; if($(NF-1) == $(NF)) corr = corr+1;}}
  END{print"Words: " lines "\nCorrectly assigned: " corr "\nSuccess rate:" corr/lines*100 "%\n"}'


  echo "time: $(date +%D-%T)" | tee -a log
  echo "FINISHED" | tee -a log
  #clear all unnecesary files
  rm *.test *.train *.trainf *.testf
 

done

exit

