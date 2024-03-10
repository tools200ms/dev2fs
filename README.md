# dev2fs (Work in progress)

This is a [FUSE](http://fuse.sourceforge.net) based filesystem, it has been developed to smooth development process of LAMP applications with Docker. [Docker](https://en.wikipedia.org/wiki/Docker_(software)) is a tool that greatly supports development process. Instead of manually setting up a various configurations on development box, I can just launch desired per-configured server with one command. When working on various projects it's very valuable automation tool.
However, I found a few small issues that I want to address with Dev2FS:
- Uid/gid remapping for shared volumes. In order to do smooth development code shall be shared between containerized application and IDE. But, application is expecting uid/gid 33/33, while IDE is launched under 1000/1000 permissions. It means that either applications inside container is capable for writing data (need for e.g. plugin setup, caching), or IDE. It can be solved by playing with chmod/chown, but while doing development I want a smooth setup out of the box.
- Internal directory remapping. While working on plugin or theme development it's code must be located somewhere within CMS/application directory tree. There is a several practical reasons to use an 'internal mount' mechanism as implemented in Dev2FS. First, by using `docker -v $HOST_PATH:$CONT_PATH` you can only mount plugin/theme source 'a priori'. Meaning, before application deployment. For example '/var/www/html/wp-content/plugins/myplugin' would be created and mount by Docker. However, application install script is expecting an empty directory. On the top of that it is expecting appropriate privileges. To address this issue I need to have 'myplugin' directory to 'auto' appear once when '/var/www/html/wp-content/plugins/' is created by installer. 'myplugin' and it's content must also preserve correct (expected by application) uid/gid.

Hence, neat solution is to have to share following two directories with host:
- '<project dir>/app' - application, not to be committed to a code repository (as it's not a code you work on)
- '<project dir>/src' - sources of your plugin/theme/module, to be committed and traced in the repository
Docker would be deployed with a following parameters:
```bash
docker ... -v $(pwd)/app:/code/app -v $(pwd)/src:/code/my -e DEV2MAP='/code/app->/var/www/html, /code/my->wp-content/plugins' ...
```

First code is shared in a fixed manner using Docker, second Def2FS is instructed to dynamically handle directory re-mappings and uid/gid re-mappings.
It is an intermediary FS layer between container mounts and application.

Without Dev2FS there is the need to play with 'docker create/stop/start/save', 'chmod', 'chown' commands. It does not sound 'scary', but after all the aim of programming is to get rid of boring 'repeating' tasks.

At this point it's a work in progress, the code is based on [TeaFS filesystem](https://sourceforge.net/projects/teafs-legacy/) that I developed a long years ago. Purpose was to experiment with treating directory names as 'tags'. The assumption was that it would help in data organization.


# Requirements, compilation and installation

Dev2FS is a C program that requires Libc and Fuse libraries, ensure you have following tools:

> make gcc libc-dev fuse-dev

When requirements are meet go to Dev2FS root directory (directory where this README.md is) and issue:

```bash
make
```

This will compile file ./src/dev2fs

To install issue as root:

```
make install
```

# Launch

The points bellow explain step-by-step how to launch the Dev2FS.


