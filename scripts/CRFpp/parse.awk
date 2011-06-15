#script for creating data for crf++
#parameter D - name of file with original data
#parameter F - format of part of the tag
#parameter S - which column of input file to skip
#column of data
{
  getline line < D; 	#loads data from original data file
  split (line, items);
  for(i = 1; i <= NF; i++)	#writes all data except for column with original training data  
  {
    if(i != S)
      printf "%s\t",$i;
  }


  for(j = 1; j <= 15; j++)
  {
    if(substr(F,j,1) == "1")
      printf "%s", substr(items[4],j,1);    
  }
  printf "\n";
}
