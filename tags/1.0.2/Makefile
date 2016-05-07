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
	mkdir -p $(NAME)-$$version; \
	cp -af $(DIST_FILES) Makefile *.c *.h *.py $(NAME)-$$version/; \
	tar cvfpj $(NAME)-$$version.tar.bz2 $(NAME)-$$version; \
	rm -fr $(NAME)-$$version
