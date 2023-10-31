
# variables declarations
OUTPUT := dev2fs
OUTPUT_LIB := libdev2fs.so

CC := gcc
LIBS := -lfuse -L.
CFLAGS := -D_FILE_OFFSET_BITS=64

SOURCES_LIB := ${wildcard lib/*.c}
OBJS_LIB := ${patsubst %.c, %.o, ${SOURCES_S1}}

SOURCES := ${wildcard *.c format/*.c shell/*.c core/*.c lib/*.c}
OBJS := ${patsubst %.c, %.o, ${SOURCES}}

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

${OUTPUT} : ${OBJS}
	${CC} ${OBJS} ${LIBS} -o ${OUTPUT}

compile : ${OUTPUT}

testrun: compile
	cd ../test && ./test.sh prepare
	echo args
	#./${OUTPUT} ../test/str ../test/mnt

clean:
	rm -f ${OBJS}
	rm -f ${OUTPUT}

	rm -f ${OUTPUT_LIB}

deepclean: clean
	find . -type f -name "*~" -exec rm '{}' \;
	find . -type f -name "*.bak" -exec rm '{}' \;

depend:
	makedepend -Y. *.c core/*.c shell/*.c lib/*.c
# DO NOT DELETE

teafs.o: dev2fs.h lib/messages.h lib/messages_en.h shell/config.h
teafs.o: lib/messages.h shell/loader.h shell/mapper.h shell/config.h
teafs.o: shell/loader.h lib/stringBuffer.h
teafs.o: core/map.h lib/list.h core/resources.h core/map.h
teafs.o: shell/system.h
core/engine.o: lib/messages.h core/engine.h
core/engine.o: core/map.h
core/map.o: lib/messages.h core/map.h
core/operations.o: lib/messages.h dev2fs.h shell/mapper.h shell/config.h
core/operations.o: shell/loader.h lib/stringBuffer.h
core/operations.o: core/map.h lib/list.h
core/operations.o: core/resources.h core/map.h core/resources.h core/engine.h
core/operations.o: core/operations.h core/engine.h
core/resources.o: core/resources.h lib/messages.h
shell/config.o: dev2fs.h shell/config.h
shell/loader.o: lib/messages.h shell/config.h shell/loader.h core/engine.h
shell/mapper.o: lib/messages.h shell/mapper.h shell/config.h shell/loader.h
shell/mapper.o: lib/stringBuffer.h
shell/mapper.o: core/map.h lib/list.h core/resources.h core/map.h
shell/system.o: dev2fs.h shell/config.h shell/mapper.h shell/loader.h
shell/system.o: lib/stringBuffer.h
shell/system.o: core/map.h lib/list.h core/resources.h core/map.h
shell/system.o: shell/system.h core/operations.h core/engine.h
lib/list.o: lib/list.h
lib/messages.o: dev2fs.h lib/messages.h lib/messages_en.h
lib/stringBuffer.o: lib/messages.h lib/messages_en.h lib/stringBuffer.h

