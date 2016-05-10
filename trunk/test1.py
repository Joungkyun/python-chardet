#!/usr/bin/python
# -*- coding: utf-8 -*-

# $Id$

from __future__ import print_function
import sys
import os

try:
    from urllib.request import urlopen
except ImportError:
    from urllib2 import urlopen

osinfo = os.uname ()
testmodpath = os.getcwd () + \
            '/build/lib.' + \
            osinfo[0].lower() + '-' + \
            osinfo[4] + '-' + \
            str (sys.version_info[0]) + '.' + \
            str(sys.version_info[1])

sys.path.insert (0, testmodpath)

from chardet.universaldetector import UniversalDetector

PyVer = int (sys.version[0])

urls = [
	'http://google.cn',
	'http://yahoo.co.jp',
	'http://amazon.co.jp',
	'http://pravda.ru',
	'http://auction.co.kr',
	'http://haaretz.co.il'
]

detector = UniversalDetector()

for url in urls :
	print ("URL %-20s : " % url, end=""),

	usock = urlopen(url)

	detector.reset ();
	for line in usock.readlines():
		detector.feed(line)
		if detector.done: break

	detector.close()
	usock.close()

	# det member => encoding(string), confidence(.2f)
	print ("encoding: %-15s, confidence: %.2f" % (detector.result['encoding'], detector.result['confidence']))

sys.exit (0)
