#!/usr/bin/env python

from features import mfcc
from features import logfbank
import scipy.io.wavfile as wav

def dump(name):

	itr = 1

	print str(name) + " ",

	for i in xrange(0, len(mfcc_feat)):

		sub = mfcc_feat[i]

		for j in xrange(0, len(sub)):
			print str(itr) + ":" + str(sub[j]),
			itr += 1

		if(itr > 1000):
			break

	print 


(rate,sig) = wav.read("input.wav")
mfcc_feat = mfcc(sig,rate)
dump(1)

(rate,sig) = wav.read("input3.wav")
mfcc_feat = mfcc(sig,rate)
dump(2)

(rate,sig) = wav.read("input5.wav")
mfcc_feat = mfcc(sig,rate)
dump(3)

(rate,sig) = wav.read("input10.wav")
mfcc_feat = mfcc(sig,rate)
dump(4)
