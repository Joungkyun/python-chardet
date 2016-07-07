CHARDET extension v2
===

## COPYRIGHT AND LICENCE

Copyright (C) 2016 by JoungKyun.Kim &lt;http://oops.org&gt; And all rights reserved.

Version: ***MPL 1.1***

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"); you may not use this file except in compliance with
the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.


## DESCRIPTION

This is python extension that is [libchardet](https://github.com/Joungkyun/libchardet) python frontend.

[libchardet](https://github.com/Joungkyun/libchardet) is based on Mozilla Universal Charset Detector library and, detects
the character set used to encode data.

From 2.0.0, this module has compatible API with [python-chardet](https://pypi.python.org/pypi/chardet). This means that,
this module can be in place the [python-chardet](https://pypi.python.org/pypi/chardet) in PYPI without code changes.

This module is a c-binding, is much faster than the [python-chardet](https://pypi.python.org/pypi/chardet).


## INSTALLATION

### 1. Requirement

This module requires follow library:

 * libchardet (https://github.com/Joungkyun/libchardet)

### 2. Build

To install this module type the following:

```bash
  [root@host python-chardet]$ make build
  [root@host python-chardet]$ make install
  [root@host python-chardet]$ # or
  [root@host python-chardet]$ python setup.py build or make build
  [root@host python-chardet]$ python setup.py install or make install
```
for details, read [INSTALL.md](/INSTALL.md) document.

## USAGE

See also http://chardet.readthedocs.io/en/latest/usage.html or
test*.py in this source
