#!/usr/bin/env python
# -*- coding: iso-8859-2 -*-

import getopt
import re
import sys

#usage function
def usage():
   print "Script for converting data from csts to format suitable \
	for crfsuite program. \
	arguments: \
	-n [sentence count]	defines how many sentences will be processed\
	-p 111---		uses parts of tag which are defined as 1"

def output_features(fo, seq):
   for i in range(2, len(seq)-2):
      fs = []
        
      # word
      fs.append('U00=%s' % seq[i-2][0])
      fs.append('U01=%s' % seq[i-1][0])
      fs.append('U02=%s' % seq[i][0])
      fs.append('U03=%s' % seq[i + 1][0])
      fs.append('U04=%s' % seq[i + 2][0])

      # tag
      fs.append('U10=%s' % seq[i-2][1])
      fs.append('U11=%s' % seq[i-1][1])
      fs.append('U12=%s' % seq[i][1])
      fs.append('U13=%s' % seq[i + 1][1])
      fs.append('U14=%s' % seq[i + 2][1])

   # lemma
      fs.append('U20=%s' % seq[i-2][2])
      fs.append('U21=%s' % seq[i-1][2])
      fs.append('U22=%s' % seq[i][2])
      fs.append('U23=%s' % seq[i + 1][2])
      fs.append('U24=%s' % seq[i + 2][2])

   #postfix
      fs.append('U30=%s' % seq[i-2][3])
      fs.append('U31=%s' % seq[i-1][3])
      fs.append('U32=%s' % seq[i][3])
      fs.append('U33=%s' % seq[i + 1][3])
      fs.append('U34=%s' % seq[i + 2][3])

   # features for prefixes of word
      w = seq[i][0]
      fs.append('U40=%s' % w[:1])
      fs.append('U41=%s' % w[:2])
      fs.append('U42=%s' % w[:3])
      fs.append('U43=%s' % w[-1:])

      # capitalized?
      c = 0
      if w.isupper():
         c = 1
      if w.islower():
         c = 2
      if w[0].isupper() & w[1:].islower():
         c = 3
      fs.append('U50=%s' % c)

      # position in sentence
      fs.append('U51=%d' % (i-2))

      # word length
      fs.append('U52=%d' % len(w))

      v = ""
      # closest verbs tag
      for j in range(1, len(seq) - 2):
         #check for verb tag
         #dirrection to beginning of sequence
         if i-j >= 2:
            if seq[i-j][1][0] == "V":
               v = seq[i-j][1]
               break
         #dirrection to end of sentence
         if i + j < len(seq) - 2:
            if seq[i + j][1][0] == "V":
               v = seq[i + j][1]
               break

      fs.append('U53=%s' % v)
      fo.write('%s\t%s\n' % (seq[i][4], '\t'.join(fs)))

   fo.write('\n')

def encode(x):
   x = x.replace('\\', '\\\\')
   x = x.replace(':', '\\:')
   return x

# sets format of tag - only several parts of tag might be used
def formattag(tag, parts):
   res = ''
   for i in range(0, len(parts)):
      if parts[i] == "1":
         res = res + tag[i]
   return res

#main function
def main():
   try:
      opts, args = getopt.getopt(sys.argv[1:], "hn:p:", ["help", "count=","parts="])
   except getopt.GetoptError, err:
      print str(err)
      usage()
      sys.exit(2)

   count = 5000
   tagParts = "1111100110000000"
   for o, a in opts:
      if o in ("-h", "--help"):
         usage()
         sys.exit()
      elif o in ("-n", "--count"):
         count = int(a)
      elif o in ("-p", "--parts"):
         tagParts = a
      else:
         assert False, "unhandled option"

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

   lr = re.compile('<l>([^<]*)')	# <l> .. - correct lemma
   tr = re.compile('<t>([^<]*)')	# <t> .. - correct tag
   pltr = re.compile('<MMl.*?>([^<]+).*?((?:<MMt.*?>[^<]+)+)')
      # <MMl> .. <MMt> .. <MMt> .. - possible lemmas and tags
   mmtr = re.compile('<MMt.*?>([^<]+)')
      # <MMt> .. - possible tag


   d = ('', '', '', '', '')
   seq = [d, d]
   i = 0


   for line in fi:
      line = line.strip('\n')
      if i > count:
         break
      
      if sr.match(line):
         seq.append(d)
         seq.append(d)
         output_features(fo, seq)
         seq = [d, d]
         i = i + 1
         #fo.write('%s\n' % ( line ))
      else:
      #
      # match line for correct f object, parse it into lemmas and tags
      #
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
                  alltags.append(tag)

            # search for common parts of tag and save them to uni
            uni = alltags[0]
            for tag in alltags:
               p = 0
               for l in tag:
                  if l != uni[p]:
                     uni = uni[:p] + "." + uni[p + 1:]
                  p = p + 1

            # get correct tag and lemma
            t = formattag(tr.search(line).group(1), tagParts)
            l = lr.search(line).group(1)
            w = f.group(1)

            #get postfix
            if len(w) > 2:
               pof = w[-2:]
            else:
               pof = ''

            seq.append((
	       encode(w),	# word
	       encode(formattag(uni, tagParts)), # common part of all possible tags
	       encode(lp),	# possible lemmas
	       encode(pof),	# postfix
	       encode(t)))		# correct tag

if __name__ == "__main__":
    main()
