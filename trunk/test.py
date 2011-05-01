#!/usr/bin/python
# -*- coding: utf-8 -*-

# $Id$

import sys
import os
import urllib

try:
	import chardet
except ImportError:
	osinfo = os.uname ()
	sys.path.append (
		os.getcwd () +
		'/build/lib.' +
		osinfo[0].lower() + '-'
		+ osinfo[4] + '-' +
		str (sys.version_info[0]) + '.'
		+ str(sys.version_info[1])
	)
	import chardet

urlread = lambda url: urllib.urlopen (url).read ()

urls = [
	'http://google.cn',
	'http://yahoo.co.jp',
	'http://amazon.co.jp',
	'http://pravda.ru',
	'http://auction.co.kr',
	'http://haaretz.co.il'
]

ch = chardet.init ();

for url in urls :
	err = []
	print "URL %-20s : " % url,
	# det member => encoding(string), confidence(.2f)
	det = chardet.detect (ch, urlread (url), err)
	if ( det == None ) :
		print "Error: %s" % err
	print "encoding: %-15s, confidence: %.2f" % (det.encoding, det.confidence)

chardet.destroy (ch);

sys.exit (0)
