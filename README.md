# confine.c
Program to run an exicutable binary in a restricted enviroment. 
[Assignment Source](https://www.cs.rochester.edu/~sree/courses/csc-252-452/fall-2019/a4.html)


## Usage
`$ ./confine ./test <arg1> <arg2> ...`


## To build
- Run `make`
* Compiles confine.c and test.c
* Creates exicutable `confine` and `test`


#### Custom test program
- Run `make`
- Exicute program with `./your_program` in place of `./test`


## Restricted enviroment parameters:
### The exicuted program may not
- Use more than 64MB of memory in its virtual address space
- Create a file larger than or equal to 4MB
- Run for longer than 1 minute 
