#!/usr/bin/env python
# -*- coding: iso-8859-2 -*-

import getopt
import re
import sys

def usage():
   print "Script for converting data from csts to format suitable \
	for crfsuite program. \
	arguments: \
	-n [sentence count]	defines how many sentences will be processed"

def encode(x):
   x = x.replace('\\', '\\\\')
   x = x.replace(':', '\\:')
   return x

# sets format of tag - only several parts of tag might be used
def formattag(tag):
   tag = tag[:5] + tag[7:9]
   return tag

#main function
def main():

   fi = sys.stdin
   fo = sys.stdout

   #
   # regular expressions for parsing input data
   #
   sr = re.compile('^<s.*?>.*')	# <s .. > ...
   fr = re.compile(r"""
         ^<[fd].*?>	# <f> or <d>
         ([^<]*)		# original word
         <l>([^<]*)	# correct lemma
         <t>([^<]*)	# correct tag
         """, re.VERBOSE);

   tr = re.compile('<t>([^<]*)')	# <t> .. - correct tag
   pltr = re.compile('<MMl.*?>([^<]+).*?((?:<MMt.*?>[^<]+)+)')
      # <MMl> .. <MMt> .. <MMt> .. - possible lemmas and tags
   mmtr = re.compile('<MMt.*?>([^<]+)')
      # <MMt> .. - possible tag

   i = 0
   max = 50000

   associations = {} 



   for line in fi:
      line = line.strip('\n')
      if i > max:
         break
      
      if sr.match(line):
         i = i + 1
      else:
      #
      # match line for correct f object, parse it into lemmas and tags
      #
         f = fr.match(line)
         if f:
            # get correct tag and lemma
            w = f.group(1)
            t = formattag(tr.search(line).group(1))

            if not w in associations:
               associations[w] = {}
            if not t in associations[w]:
               associations[w][t] = 0

            associations[w][t] = associations[w][t] + 1

   fo.write('Sentence count: %d\n' % i)
#EVALUATION OF LOADED DATA

   testFile = open('./dtest-3-ann.cst', 'r')

   total = 0
   correct = 0
   missed = 0

   i = 0
   max = 7000
   seq = []

#LOAD DATA 
   for line in testFile:
      line = line.strip('\n')
      if i > max:
         break; 

      if sr.match(line):
         i = i + 1
      else:
         f = fr.match(line)
         if f:
            alltags = []
            plt = pltr.findall(line)
            lp = ''

            for tags in plt:
               # save possible lemmas into lp
               lp = lp + "/" + tags[0]

               mmt = mmtr.findall(tags[1])
               # append tags from this lemma to common array
               for tag in mmt:
                  alltags.append(formattag(tag))

            # get correct tag and lemma
            w = f.group(1)
            t = formattag(tr.search(line).group(1))

            seq.append((encode(w),encode(t), alltags))

#EVALUATION
   for elem in seq:
      word = elem[0]
      tag = elem[1]
      alltags = elem[2]

      if word == '\:':
         continue

      maxcount = 0
      besttag = ''

      if word in associations:
         for atag in associations[word]:
            if atag in alltags and associations[word][atag] > maxcount:
               maxcount = associations[word][atag]
               besttag = atag     
      else: 
         missed = missed + 1

      if besttag == '':   
         besttag = alltags[0]      

      if besttag == tag:
         correct = correct + 1
      
      total = total + 1

      i = i + 1

   fo.write('Correct: %d\tTotal: %d\tMissed: %d\tRate: %.3f\n' % (correct, total, missed, float(correct)/float(total)*100))
if __name__ == "__main__":
     main()
