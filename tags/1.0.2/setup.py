#!/usr/bin/python

# $Id$

from distutils.core import setup, Extension
import os
import sys
import re
import string

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
		version      = '1.0.2',
		description  = 'python binding for lichardet API',
		author       = 'JoungKyun.Kim',
		author_email = 'admin@oops.org',
		url          = 'http://oops.org',
		license      = 'MPL 1.1',
		platforms    = 'x86/x86_64',
		ext_modules  = [
			Extension (
						'chardet',
						['chardet.c' ],
						include_dirs  = incdir,
						library_dirs  = libdir,
						libraries     = libs,
						define_macros = defs
					)
		]
)
