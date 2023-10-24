
SRC_DIR := ./src/

EXEC_NAME := dev2fs
PREFIX := /usr/local/bin

RELEASE_NAME := ${EXEC_NAME}-${shell echo test}
#grep "#define[[:space:]][[:space:]]*DEV2FS_VERSION" src/${EXEC_NAME}.h | sed -re 's/(.+)\"(.+)\"/\2/' | tr ' ' '_'}


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

ifeq ($(shell [ -f ./release/${RELEASE_NAME}.tar.xz ] && echo 1 || echo 0),1)
	@echo "File './release/${RELEASE_NAME}.tar.xz' already exists, do you want to remove it and re-release (y),"
	@echo "or keep the file unchanged (n)?"
	@rm -i ./release/${RELEASE_NAME}.tar.xz
endif

ifneq ($(shell [ -f ./release/${RELEASE_NAME}.tar.xz ] && echo 1),1)
	tar -cvJf ./release/${RELEASE_NAME}.tar.xz --exclude-vcs-ignores --exclude-vcs . --transform s/^\./${RELEASE_NAME}/
	@echo "New teaFS's release file created: ../${RELEASE_NAME}.tar.xz"
endif

	@echo -n "MD5 sum is: "
	@md5sum ./release/${RELEASE_NAME}.tar.xz
	#| separate -N 0

	@echo -n "Archive's size is: "
	@ls -lh ./release/${RELEASE_NAME}.tar.xz
	#| separate -N 4

	@echo "----- Done. --------------------------------"
