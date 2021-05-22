#!/usr/bin/python
# -*- coding: utf-8 -*-

# Copyfight 2021. JoungKyun.Kim all rights reserevd.
#
# The contents of this file are subject to the Mozilla Public License Version
# 1.1 (the "License"); you may not use this file except in compliance with
# the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
# for the specific language governing rights and limitations under the
# License.

from __future__ import print_function
import sys
import os

#
# for compatible python 3
#
try:
	import urllib
	from urllib.request import urlopen
	from urllib.error import HTTPError
except ImportError:
	import urllib2
	from urllib2 import urlopen, HTTPError

#
# include modue on build directory
#
osinfo = os.uname ()
curpath = os.getcwd ();
if ( os.path.basename (curpath) == 'tests' ) :
	curpath = os.path.dirname (os.getcwd ())

testmodpath = curpath + \
            '/build/lib.' + \
            osinfo[0].lower() + '-' + \
            osinfo[4] + '-' + \
            str (sys.version_info[0]) + '.' + \
            str(sys.version_info[1])
sys.path.insert (0, testmodpath)
import chardet.universaldetector
from chardet.universaldetector import UniversalDetector

#
# Test code
#

print ()
print ('===================================================')
print ('** python-chardet comaptible multiple files test **')
print ('===================================================')
print ()

urls = [
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/ar/windows-1256.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/bg/windows-1251.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/da/windows-1252.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/el/windows-1253.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/en/ascii.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/fr/iso-8859-15.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/he/iso-8859-8.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/ja/euc-jp.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/ja/iso-2022-jp.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/ja/shift_jis.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/ko/euc-kr.smi',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/ko/iso-2022-kr.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/ru/ibm866.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/th/tis-620.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/tr/iso-8859-9.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/vi/viscii.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/zh/big5.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/zh/euc-tw.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/zh/gb18030.txt',
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/zh/utf-8.txt',
	'./utf-8-bom.txt'
]

detector = UniversalDetector()

for url in urls :
	purl = os.path.basename (url)
	print ("URL %-20s : " % purl, end=""),

	try :
		if url.startswith ('https://') :
			usock = urlopen(url)

			detector.reset ();
			for line in usock.readlines():
				detector.feed(line)
				if detector.done: break

			usock.close()
		else :
			f = open (url, 'r')

			detector.reset();
			while True:
				line = f.readline()
				if not line : break
				detector.feed(line)
				if detector.done: break

			f.close()

		detector.close()

		# det member => encoding(string), confidence(.2f)
		print ("encoding: %-15s, confidence: %.2f" % (detector.result['encoding'], detector.result['confidence']))
	except HTTPError as e :
		print (e)

sys.exit (0)

#
# Local variables:
# tab-width: 4
# c-basic-offset: 4
# End:
# vim600: noet sw=4 ts=4 fdm=marker
# vim<600: noet sw=4 ts=4
#
