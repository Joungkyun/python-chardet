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

usock = urlopen('http://yahoo.co.jp/')
detector = UniversalDetector()

detector.reset ();
for line in usock.readlines():
    detector.feed(line)
    if detector.done: break

detector.close()
usock.close()
print (detector.result)

sys.exit (0)

#
# Local variables:
# tab-width: 4
# indent-tabs-mode
# nil
# c-basic-offset: 4
# End:
# vim600: et sw=4 ts=4 fdm=marker
# vim<600: et sw=4 ts=4
#
