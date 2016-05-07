PYTHON = /usr/bin/python
NAME = mod_chardet
PROG = chardet

DIST_FILES = COPYING INSTALL Changes README


all: install

build:
	$(PYTHON) setup.py build

install:
	$(PYTHON) setup.py install

test: build
	$(PYTHON) test.py
	$(PYTHON) test1.py

clean:
	rm -rf build *.bz2

dist:
	version=`cat version.h | grep MOD_CHARDET_VERSION | awk '{print $$3}' | sed 's/"//g'`; \
	mkdir -p chardet-$$version; \
	cp -af $(DIST_FILES) Makefile *.c *.h *.py chardet-$$version/; \
	tar cvfpj chardet-$$version.tar.bz2 chardet-$$version; \
	rm -fr chardet-$$version
