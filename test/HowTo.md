### Compile project for usage within continer:
'''
docker exec -it 453b59665976 bash -c \
    "make clean && make"
'''

### launch conteneraised devfs as root attached to mount point (owned by 'test') and storage (owned by 'proj')

```
docker exec -it -u root test-dev2fs_with_musl-1 bash -c \
    "./build/dev2fs -d /d2str /home/test/mnt/ -o allow_other"
```

### Test as 'test' user:
```
docker exec -it -u test test-dev2fs_with_musl-1 bash
cd
```
