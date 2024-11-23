# Downloading, Building and running the projects

This repo contains a simple Hello World, sel4test and sel4bench.

git submodules are used to get all the required repos, but are not checked out
by default, except if you used git clone --recurse-submodules.

```bash
git submodule init
git submodule update
```

Submodules are at a specific commit. To get the latest of everything do:
```bash
git submodule update --remote
```
The specific submodule checkouts can be commited by doing a commit in the
main directory.

To build a specific project 'hello', 'test' or 'bench':

```bash
proj=<hello|test|bench>
mkdir build-$proj
cd build-$proj
../$proj/init-build.sh ...
ninja
```

E.g. for Hello World:
```bash
mkdir build-hello
cd build-hello
../hello/init-build.sh -DPLATFORM=x86_64 -DSIMULATION=TRUE
ninja
./simulate
```

You should see a printf warning followed by the "Hello World!" message.
You can exit QEMU with `Ctrl-a + x`.

If you don't want the screen to be cleared after exiting QEMU, remove
the last line of the `./simulate` script.
