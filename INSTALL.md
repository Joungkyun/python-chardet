Mod_chardet Installation
===

## Dependency

In order to build the chardet python modue, is required [libchardet](https://github.com/Joungkyun/libchardet) library.
See also follow url:

    https://github.com/Joungkyun/libchardet/blob/master/INSTALL

When you build a chardet module, you will need chardet-config of libcharset.
If the path of chardet-config does not include the PATH environment, please
add a chardet-config directory in the PATH environment variable.

```bash
[user@host chardet]$ # if chardet-config is located in /opt/libchardet-1.0.5/bin
[user@host chardet]$ PATH="$PATH:/opt/libchardet-1.0.5/bin" make build
[user@host chardet]$ # OR
[user@host chardet]$ PATH="$PATH:/opt/libchardet-1.0.5/bin" python setup.py build
```

## Quick Install

```bash
[user@host ~]$ make install
```

## Build option

* Build only  
```bash
    [user@host ~]$ make build
```
* Build test
```bash
    [user@host ~]$ make test
```
*  Clean build
```bash
    [user@host ~]$ make clean
```
*  If your python path is not /usr/bin/python
```bash
    [user@host ~]$ make PYTHON=/usr/bin/python3 [build|install|test]
```
*  build source package
```bash
    [user@host ~]$ make dist
```
