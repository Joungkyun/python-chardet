#!/usr/bin/python

# $Id$

from distutils.core import setup, Extension
import os
import sys
import re
import string

def readme() :
	with open('README') as f:
		return f.read()

incdir = []
libdir = []
libs   = [ 'chardet', 'stdc++' ]
defs   = []

major_version = sys.version[0]

chardet_env = (os.popen ('chardet-config --libs')).read ()
envlist   = chardet_env.split ()

for arg in envlist :
	if arg[1] == 'L' :
		libdir.append (arg[2:])

del envlist

chardet_defs = (os.popen ('chardet-config --defs')).read ()
envlist    = chardet_defs.split ()

for arg in envlist :
	if arg[1] == 'I' :
		incdir.append (arg[2:])

del envlist


setup (
		name         = 'chardet',
		version      = '2.0.0',
		description  = 'python binding for libchardet API',
		long_description=readme(),
		author       = 'JoungKyun.Kim',
		author_email = 'admin@oops.org',
		url          = 'http://oops.org',
		license      = 'MPL 1.1',
		platforms    = 'x86/x86_64',
		packages     = { 'chardet' },
		ext_modules  = [
			Extension (
						'chardet._chardet',
						[ 'src/chardet.c' ],
						include_dirs  = incdir,
						library_dirs  = libdir,
						libraries     = libs,
						define_macros = defs
					),
			Extension (
						'chardet.universaldetector',
						[ 'src/universaldetector.c' ],
						include_dirs  = incdir,
						library_dirs  = libdir,
						libraries     = libs,
						define_macros = defs
					),
			Extension (
						'chardet_c',
						[ 'src/chardet_c.c' ],
						include_dirs  = incdir,
						library_dirs  = libdir,
						libraries     = libs,
						define_macros = defs
					),
		],
		classifiers  = [
			("License :: OSI Approved :: MIT License" +
			 " Public License (LGPL)"),
			"Programming Language :: C++",
			"Programming Language :: Python",
			'Programming Language :: Python :: 2.6',
			'Programming Language :: Python :: 2.7',
			'Programming Language :: Python :: 3',
			'Topic :: Software Development :: Libraries :: Python Modules'
		],
		keywords     = [ 'encoding', 'charset', 'i18n' ]
)

#
# Local variables:
# tab-width: 4
# c-basic-offset: 4
# End:
# vim600: noet sw=4 ts=4 fdm=marker
# vim<600: noet sw=4 ts=4
#
