
### Compile project within continer:


```bash
# Create container in Alpine Linux (Musl)
# (./src and ./Makefile is shared inside)
docker compose up

# Compile Dev2FS inside continer: 
docker exec -it test-dev2fs_with_musl-1 sh -c \
    "make clean && make"
```

### launch containerised Devfs

```bash
# launch DevFS with root privilages, 
# Bind: '/home/test/mnt/' (test user owned) with '/d2str' (root user owned)

docker exec -it -u root test-dev2fs_with_musl-1 bash -c \
    "./build/dev2fs -d /d2str /home/test/mnt/ -o allow_other"
```

### Test as 'test' user:

```bash
docker exec -it -u test test-dev2fs_with_musl-1 bash

cd ~/mnt/
```
