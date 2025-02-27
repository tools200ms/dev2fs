
PROJ_NAME := DevFS

SRC_DIR := ./src
BUILD_DIR := ./build
EXEC_NAME := dev2fs

PREFIX := /usr/local/bin

ABOUT_H = src/${EXEC_NAME}.h
VERSION := ${shell grep "#define.DEV2FS_VERSION" $(ABOUT_H) | sed -re 's/(.+)\"(.+)\"/\2/' | tr ' ' '_'}
RELEASE_NAME := ${EXEC_NAME}-${VERSION}

.PHONY: compile install clean clean_current_dir release

compile:
	@echo "----- Compailing ---------------------------"

	$(MAKE) -C ${SRC_DIR} ${EXEC_NAME}
	
	@echo "----- Done. --------------------------------"

install:
	@echo "----- Installing ---------------------------"
	
	cp "${BUILD_DIR}/${EXEC_NAME}" "${PREFIX}"

	[ -f "/sbin/mount.${EXEC_NAME}" ] && \
		rm "/sbin/mount.${EXEC_NAME}" || true

	ln -s "${PREFIX}/${EXEC_NAME}" "/sbin/mount.${EXEC_NAME}"
	@echo "----- Done. --------------------------------"

clean_current_dir:
	@echo "----- Cleaning current directory -----------"
	rm -f *~
	@echo "----- Done. --------------------------------"

clean: clean_current_dir
	@echo "----- Cleaning -----------------------------"

	$(MAKE) -C ${SRC_DIR} clean
	@echo "----- Done. --------------------------------"

unittests:
	@echo "----- Running Unit Tests -------------------"
	$(MAKE) -C ${SRC_DIR} tests
	@echo "----- Done. --------------------------------"

funtests: compile
	mkdir -p log
	sudo ./build/dev2fs -d -s /d2str /home/test/mnt/ -o allow_other

	#sleep .2; while $(mount | grep -q /home/test/mnt); do sleep .2; done
	#echo "Starting tests"

release: clean_current_dir
	@echo "----- Releasing ----------------------------"
	$(MAKE) -C ${SRC_DIR} deepclean
	$(shell [ -d ./release ] || mkdir ./release)
ifeq ($(shell [ -f ./release/${RELEASE_NAME}.tar.xz ] && echo 1 || echo 0),1)
	@echo "File './release/${RELEASE_NAME}.tar.xz' already exists, do you want to remove it and re-release (y),"
	@echo "or keep the file unchanged (n)?"
	@rm -i ./release/${RELEASE_NAME}.tar.xz
endif

ifneq ($(shell [ -f ./release/${RELEASE_NAME}.tar.xz ] && echo 1),1)
	tar -cvJf ./release/${RELEASE_NAME}.tar.xz --exclude-vcs-ignores --exclude-vcs . --transform s/^\./${RELEASE_NAME}/
	@echo "New ${PROJ_NAME} release created: ../${RELEASE_NAME}.tar.xz"
endif
	@echo -n "MD5 sum is: "
	@md5sum ./release/${RELEASE_NAME}.tar.xz

	@echo -n "Archive's size is: "
	@ls -lh ./release/${RELEASE_NAME}.tar.xz

	@echo "----- Done. --------------------------------"
