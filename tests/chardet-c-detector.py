#!/usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import print_function
import sys
import os

PyVer = int (sys.version[0])

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

import chardet_c

#
# Test code
#

urlread = lambda url: urlopen (url).read ()

print ()
print ('=================================================')
print ('** Python chardet_c detector test **')
print ('=================================================')
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
	'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/zh/utf-8.txt'
]

print ("Python chardet c binding module version: %s" % (chardet_c.__version__))

for url in urls :
	err = []
	purl = os.path.basename (url)
	print ("URL %-20s : " % purl, end=""),

	try :
		# det member => encoding(string), confidence(.2f)
		det = chardet_c.detector (urlread (url), err)
		if ( det == None ) :
			print ("Error: %s" % err)
		print ("encoding: %-15s, confidence: %.2f" % (det.encoding, det.confidence))
	except HTTPError as e :
		print (e)

print ("\n")

sys.exit (0)

#
# Local variables:
# tab-width: 4
# c-basic-offset: 4
# End:
# vim600: noet sw=4 ts=4 fdm=marker
# vim<600: noet sw=4 ts=4
#
