all: README examples/TextCMD_synopsis/README

clean:
	rm -f README README.bak \
			examples/TextCMD_synopsis/README examples/TextCMD_synopsis/README.bak

README: TextCMD.h
	pod2readme $< $@ && rm -f $@.bak

examples/TextCMD_synopsis/README: examples/TextCMD_synopsis/TextCMD_synopsis.ino
	pod2readme $< $@ && rm -f $@.bak
