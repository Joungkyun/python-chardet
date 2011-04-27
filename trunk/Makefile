PYTHON = /usr/bin/python

all: install

build:
	$(PYTHON) setup.py build

install:
	$(PYTHON) setup.py install

test: build
	$(PYTHON) test.py
	$(PYTHON) test1.py

clean:
	rm -rf build
