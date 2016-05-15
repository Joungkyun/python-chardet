#!/usr/bin/python
# -*- coding: utf-8 -*-

# $Id$

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
import chardet

#
# Test code
#

print ()
print ('==================================================')
print ('** python-chardet comaptible basic test         **')
print ('==================================================')
print ()

print ("Python chardet c binding module version: %s" % (chardet.__version__))
print ()
url = r'https://raw.githubusercontent.com/BYVoid/uchardet/master/test/ar/windows-1256.txt'
print ('** %s => ' % os.path.basename (url), end='')

try :
	rawdata = urlopen (url).read ()
	r = chardet.detect (rawdata)
	print (r)
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
