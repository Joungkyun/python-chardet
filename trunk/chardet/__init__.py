#!/usr/bin/python
# -*- coding: utf-8 -*-

from . import _chardet

__version__ = _chardet.__version__

def detect(aBuf) :
	return _chardet.detect (aBuf)

#
# Local variables:
# tab-width: 4
# c-basic-offset: 4
# End:
# vim600: noet sw=4 ts=4 fdm=marker
# vim<600: noet sw=4 ts=4
#
