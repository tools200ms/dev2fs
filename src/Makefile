
# variables declarations
OUTPUT := dev2fs
OUTPUT_LIB := libdev2fs.so

CC := gcc
LIBS := -lfuse -L.
CFLAGS := -D_FILE_OFFSET_BITS=64

DIR_BIN      = ../build
SOURCES_LIB := ${wildcard lib/*.c}
OBJS_LIB := ${patsubst %.c, %.o, ${SOURCES_S1}}

SOURCES := ${wildcard *.c shell/*.c lib/*.c core/*.c}
BIN_OBJS := ${patsubst %.c, %.o, ${SOURCES}}

# setting additional options

CFLAGS += -Wall
#CFLAGS += -Wimplicit-function-declaration

ifeq ("${DEBUG}","y")
	CFLAGS += -g1
else ifeq ("${DEBUG}","Y")
	CFLAGS += -g3
endif

# targets definitions

.PHONY : one-command-compile \
         clean deepclean

#all: ${OUTPUT}

${OUTPUT}: ${BIN_OBJS}
	${CC} ${BIN_OBJS} ${LIBS} -o ${DIR_BIN}/${OUTPUT}

compile: ${OUTPUT}

testrun: compile
	cd ../test && ./test.sh prepare
	echo args
	#./${OUTPUT} ../test/str ../test/mnt

clean:
	rm -f ${BIN_OBJS}
	rm -f ${DIR_BIN}/${OUTPUT}

	rm -f ${OUTPUT_LIB}

deepclean: clean
	find . -type f -name "*~" -exec rm '{}' \;
	find . -type f -name "*.bak" -exec rm '{}' \;

depend:
	makedepend -Y. *.c core/*.c shell/*.c lib/*.c
# DO NOT DELETE

dev2fs.o: dev2fs.h lib/messages.h lib/messages_en.h lib/debug.h
dev2fs.o: shell/config.h lib/messages.h shell/loader.h shell/mapper.h
dev2fs.o: shell/config.h shell/loader.h lib/stringBuffer.h shell/system.h
core/engine.o: lib/messages.h core/engine.h
core/operations.o: lib/messages.h dev2fs.h shell/mapper.h shell/config.h
core/operations.o: shell/loader.h lib/stringBuffer.h lib/op_stats.h
core/operations.o: core/engine.h core/operations.h
shell/config.o: dev2fs.h shell/config.h
shell/loader.o: lib/messages.h shell/config.h shell/loader.h core/engine.h
shell/mapper.o: lib/messages.h shell/mapper.h shell/config.h shell/loader.h
shell/mapper.o: lib/stringBuffer.h lib/list.h
shell/system.o: dev2fs.h shell/mapper.h shell/config.h shell/loader.h
shell/system.o: lib/stringBuffer.h shell/system.h core/operations.h
shell/system.o: core/engine.h
lib/list.o: lib/list.h
lib/messages.o: dev2fs.h lib/messages.h lib/messages_en.h lib/debug.h
lib/op_stats.o: lib/op_stats.h lib/messages.h
lib/stringBuffer.o: lib/messages.h lib/messages_en.h lib/debug.h
lib/stringBuffer.o: lib/stringBuffer.h
