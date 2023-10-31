### Compile project for usage within continer:
'''
docker exec -it 453b59665976 bash -c \
    "make clean && make"
'''

### launch conteneraised devfs as root attached to mount point (owned by 'test') and storage (owned by 'proj')
'''
docker exec -it -u root 453b59665976 bash -c \
    "./src/dev2fs -d /d2str /home/test/mnt/"
'''

### Test as 'test' user:
```
docker exec -it -u test 453b59665976 bash
cd
```
