
SRC_DIR := ./src/

EXEC_NAME := dev2fs
PREFIX := /usr/local/bin

RELEASE_NAME := ${EXEC_NAME}-${shell cat src/${EXEC_NAME}.h | grep "\#define[[:space:]][[:space:]]*DEV2FS_VERSION" | sed -re 's/(.+)\"(.+)\"/\2/' | tr ' ' '_'}


.PHONY: compile install clean clean_current_dir release

compile:
	@echo "----- Compailing ---------------------------"

	$(MAKE) -C ${SRC_DIR} ${EXEC_NAME}
	
	@echo "----- Done. --------------------------------"

install:
	@echo "----- Installing ---------------------------"
	
	cp "${SRC_DIR}${EXEC_NAME}" "${PREFIX}"
	@echo "----- Done. --------------------------------"

clean_current_dir:
	@echo "----- Cleaning current directory -----------"
	rm -f *~
	@echo "----- Done. --------------------------------"

clean: clean_current_dir
	@echo "----- Cleaning -----------------------------"
	
	$(MAKE) -C ${SRC_DIR} clean
	@echo "----- Done. --------------------------------"

release: clean_current_dir
	@echo "----- Releasing ----------------------------"
	$(MAKE) -C ${SRC_DIR} deepclean

ifeq ($(shell [ -f ../${RELEASE_NAME}.tar.gz ] && echo 1),1)
	@echo "File '../${RELEASE_NAME}.tar.gz' already exists, do you want to remove it and re-release (y),"
	@echo "or keep the file unchanged (n)?"
	@rm -i ../${RELEASE_NAME}.tar.gz
endif

ifneq ($(shell [ -f ../${RELEASE_NAME}.tar.gz ] && echo 1),1)
	tar -cvzf ../${RELEASE_NAME}.tar.gz . --exclude='.svn' --transform s/^\./${RELEASE_NAME}/
	@echo "New teaFS's release file created: ../${RELEASE_NAME}.tar.gz"
endif

	@echo -n "MD5 sum is: "
	@md5sum ../${RELEASE_NAME}.tar.gz | separate -N 0

	@echo -n "Archive's size is: "
	@ls -lh ../${RELEASE_NAME}.tar.gz | separate -N 4

	@echo "----- Done. --------------------------------"
