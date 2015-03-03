sBWT indexer and aligner
=========
sBWT is a Burrows–Wheeler transform (BWT) based fast indexer/aligner specialized in parallelized indexing and searching for Next Generation Sequencing data. 

sBWT is released under GPLv2 with additional restriction so that is only applicable to individuals and non-profits and that any for-profit company must purchase a different license.    

##INSTALL
*Only 64 bits systems are able to compile and run sBWT aligner.

### Compile from the source code
#### Install the dependencies
- 1.1 Relative recent C++ compiler that support most features of C++11. We recommend [GCC](http://gcc.gnu.org/).
- 1.2 [Boost](http://www.boost.org/users/download/)
- 1.3 [CMake](http://www.cmake.org/)

#### Get the latest version of the software
```bash
git clone git@github.com:avtomaton/sBWT.git
```

#### Create build scripts:
- Create folder when you want to build sBWT and enter to it;
- Set environmental variable `$BOOST_ROOT` to the directory of boost if CMake cannot find boost automatically;
- Set environmental variable `$CC` and `$CXX` to the gcc/g++ compiler you want to use.
```bash
cmake SBWT_SOURCES_FOLDER
```

#### Compile the software by typing:
```bash
make
```

#### troubleshooting
- If you got linker error, it is possible that the default library in the lib/ is not suitable to your platform. 
 There are one librarie for linux available, rename the file that fit the best to "libabwt_table.a",
 and retype 
```bash
make
```
	
##USAGE

#### Build genomic index (similar to bowtie-build)
```bash
sbwt build -i [genome.fa] -p [ouput prefix]
```

#### Mapping 
```bash
sbwt map -i [reads.fq] -p [index prefix] -o [output.sam]
```

GPU version
===========

##Install

### Compile from the source code
#### Install the dependencies
- 1.1 Relative recent C++ compiler that support most features of C++11. We recommend [GCC](http://gcc.gnu.org/).
- 1.2 [Boost](http://www.boost.org/users/download/)
- 1.3 CUDA's Minimal Compute Capability 2.0


#### Enter the folder sbwtCuda and:
- Set environmental variable `$CC` and `$CXX` to the gcc/g++ compiler you want to use.
```bash
make
```

#### Build genomic index
```bash
sbwt-cuda build -i [genome.fa] -p [ouput prefix]
```

#### Mapping
```bash
sbwt map -i [reads.fq] -p [index prefix] -o [output.sam]  # for mapping
```


## Contact
- Viktor Pogrebniak <avtomaton@gmail.com>

## Original Repository
https://github.com/jhhung/sBWT


