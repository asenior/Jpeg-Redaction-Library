# Makefile for building JPEG/EXIF reading, writing, redaction
# binaries.

include ../Makefile.common

BINARY = redact

LIB = ../lib/libredact.a

default: $(BINARY)

test:
	cd ../test; $(MAKE) test

$(LIB): 
	cd ../lib; $(MAKE) lib

$(BINARY): $(LIB) redaction_main.cpp
	$(CC) $(CXXFLAGS) -I../lib redaction_main.cpp $(LIBPATH) $(LIB)  -o $@

.PHONY: clean cleanall clean_rawgrey clean_test \
	$(LIB)

clean:
	rm -f $(BINARY)

veryclean: cleanall

cleanall: clean
	cd ../lib; $(MAKE) clean
	cd ../test; $(MAKE) clean

depend: 
	$(MAKE) dependlocal

# DO NOT DELETE
