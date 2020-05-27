#### project for compiler

```shell
$ cd /path/to/this/repo
$ mkdir build && cd build
$ cmake .. & make -j
$ ./compiler -s ../test/test.c -d ../mid.txt 
```

mips vm: 

http://courses.missouristate.edu/kenvollmar/mars/

python file "genTarCode.py" is used to generate mips code to run on mips vm.

```shell
python genTarCode.py -s mid.txt -d test.asm
```

