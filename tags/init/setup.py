#!/usr/bin/python

# $Id: setup.py,v 1.1.1.1 2010-07-05 19:03:19 oops Exp $

from distutils.core import setup, Extension
import os
import re
import string

incdir = []
libdir = []
libs   = [ 'chardet', 'stdc++' ]
defs   = []

krisp_env = (os.popen ('chardet-config --libs')).read ()
envlist   = string.split (krisp_env)

for arg in envlist :
	if arg[1] == 'L' :
		libdir.append (arg[2:])

del envlist

krisp_defs = (os.popen ('chardet-config --defs')).read ()
envlist    = string.split (krisp_defs)

for arg in envlist :
	if arg[1] == 'I' :
		incdir.append (arg[2:])

del envlist


setup (
		name         = 'chardet',
		version      = '1.0.0',
		description  = 'python binding for lichardet API',
		author       = 'JoungKyun.Kim',
		author_email = 'admin@oops.org',
		url          = 'http://oops.org',
		license      = 'MPL 1.1',
		platforms    = 'x86/x86_64',
		#py_modules   = [ 'krisp' ],
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