#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import re

def output_features(fo, seq):
    for i in range(2, len(seq)-2):
        fs = []
        
        fs.append('U00=%s' % seq[i-2][0])
        fs.append('U01=%s' % seq[i-1][0])
        fs.append('U02=%s' % seq[i][0])
        fs.append('U03=%s' % seq[i+1][0])
        #fs.append('U04=%s' % seq[i+2][0])
        #fs.append('U05=%s/%s' % (seq[i-1][0], seq[i][0]))
        #fs.append('U06=%s/%s' % (seq[i][0], seq[i+1][0]))

        fs.append('U10=%s' % seq[i-2][1])
        fs.append('U11=%s' % seq[i-1][1])
        fs.append('U12=%s' % seq[i][1])
        fs.append('U13=%s' % seq[i+1][1])
        fs.append('U14=%s' % seq[i+2][1])
        #fs.append('U15=%s/%s' % (seq[i-2][1], seq[i-1][1]))
        #fs.append('U16=%s/%s' % (seq[i-1][1], seq[i][1]))
        #fs.append('U17=%s/%s' % (seq[i][1], seq[i+1][1]))
        #fs.append('U18=%s/%s' % (seq[i+1][1], seq[i+2][1]))
        
        #fs.append('U20=%s/%s/%s' % (seq[i-2][1], seq[i-1][1], seq[i][1]))
        #fs.append('U21=%s/%s/%s' % (seq[i-1][1], seq[i][1], seq[i+1][1]))
        #fs.append('U22=%s/%s/%s' % (seq[i][1], seq[i+1][1], seq[i+2][1]))

        fo.write('%s\t%s\n' % (seq[i][2], '\t'.join(fs)))
    fo.write('\n')

def encode(x):
    x = x.replace('\\', '\\\\')
    x = x.replace(':', '\\:')
    return x
    
fi = sys.stdin
fo = sys.stdout

s = re.compile('^<s.*?>.*')	# <s .. > ...
f = re.compile(r"""
		^<[fd].*?>	# <f> or <d> 
		([^<]*)		# original word
		<l>([^<]*)	# correct lemma
		<t>([^<]*)	# correct tag
		""", re.VERBOSE);	


d = ('', '', '')

seq = [d, d]

n = 2000
i = 0

for line in fi:
    line = line.strip('\n')
    if i > n*3:
	break
    if i < n:
        fo = sys.stdout
    else:
        fo = sys.stderr
    if s.match(line):
         seq.append(d)
         seq.append(d)
	 output_features(fo, seq)
         seq = [d, d]
         i = i + 1
         #fo.write('%s\n' % ( line ))
    else:
         res = f.match(line)
         if res:
              tag = res.group(3)[:2] + res.group(3)[3:5] + res.group(3)[7:9]
              seq.append((encode(res.group(1)), encode(res.group(2)), encode(tag)))
	      #fo.write('%s\n' % ( res.group(1)))
	     

#    if not line:
#        seq.append(d)
#        seq.append(d)
#        output_features(fo, seq)
#        seq = [d, d]
#    else:
	
        #fields = line.strip('\n').split(' ')
        #seq.append((encode(fields[0]), encode(fields[1]), encode(fields[2])))
