.DEFAULT_GOAL := help
.PHONY: test help

all: README examples/TextCMD_synopsis/README	## refresh all READMEs

clean:	## clean-up
	rm -f README README.bak \
			examples/TextCMD_synopsis/README examples/TextCMD_synopsis/README.bak
	cd t && make clean

README: TextCMD.h	## refresh
	pod2readme $< $@ && rm -f $@.bak

examples/TextCMD_synopsis/README: examples/TextCMD_synopsis/TextCMD_synopsis.ino	## refresh
	pod2readme $< $@ && rm -f $@.bak

test:	## run tests
	cd t && make

help:	## show help
	@perl -lnE 'if (m{^([a-zA-Z0-9_/\-]+):\s.*?##\s(.*)$$}) { say "$$1\t$$2" }' $(MAKEFILE_LIST)

