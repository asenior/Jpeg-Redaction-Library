# makefile for building JPEG/EXIF tests under cygwin
DEPENDFILE=depends.mak
MAKEDEPEND = makedepend -o.o -f$(DEPENDFILE)
MKDEP_CMD = $(MAKEDEPEND) $(CPP_DEFINE_FLAGS) $(CC_INCLUDEDIR_FLAGS) -a

CXXOFLAGS =  $(INCLUDES) -O3
CXXDFLAGS =  $(INCLUDES) -g -DDEBUG
CXXFLAGS = $(CXXDFLAGS)
CFLAGS=$(CXXFLAGS)
LIBS = -lm 
MKLIB = $(AR) $(ARFLAGS) $@ $?; ranlib $@
# cr2.cpp exif_data.cpp  iptc.cpp iptc_tag.cpp 

SRCS  =  iptc.cpp jpeg.cpp  tiff_ifd.cpp tiff_tag.cpp jpeg_decoder.cpp \
	byte_swapping.cpp
# Photoshop3Block.cpp PhotoshopBIM.cpp

OBJS    = $(SRCS:.cpp=.o)

CC = g++ 

LOCALLIB=mylibjpeg.a

BINARY = jpegtest

test: $(BINARY)

$(LOCALLIB): $(OBJS)
	$(MKLIB)

$(BINARY): mylibjpeg.a jpegtest.cpp
	$(CC) $(CXXFLAGS) jpegtest.cpp $(LIBPATH) mylibjpeg.a  -o $@


# 	$(CC) $(OBJS) $(LIBPATH) --enable-stdcall-fixup  ..\\orcha\\utils\\libutils.a $(CAIRO)\\lib\\libcairo.dll.a  $(SDLLIB)\\libSDL.dll.a -lgdi32 -lopengl32 -lglu32 -lsdl -o $@

$(OBJS): $(SRCS:.cpp=.h)

.PHONY: clean test


clean:
	rm -f $(OBJS) $(BINARY) $(LOCALLIB)

depend: 
	$(MAKE) dependlocal


zipfile: gauss-mingw.zip-dummyext

gauss-mingw.zip-dummyext: gauss.exe
	cygcheck gauss.exe | grep -v Found| sed -e 's/^ *//g'|  sort | uniq | zip -@ $@
