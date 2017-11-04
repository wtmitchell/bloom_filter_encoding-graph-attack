# A Graph Traversal Attack on Bloom filter-based medical data aggregation

This code implements the attack in the paper:

```
@article{mitchell2017graph,
  title={A graph traversal attack on Bloom filter-based medical data aggregation},
  author={Mitchell, William and Dewri, Rinku and Thurimella, Ramakrishna and Roschke, Max},
  journal={International Journal of Big Data Intelligence},
  volume={4},
  number={4},
  pages={217--226},
  year={2017},
  publisher={Inderscience Publishers (IEL)}
}
```

# Dependencies

The code is primarily C++11 with a little C++14, so a recent compiler is expected. The code has been tested with
both gcc and clang on Linux and clang on MacOS.

Several of the dependencies ([Google Test](https://github.com/google/googletest), [Crypto++](https://cryptopp.com/), [Botan](https://botan.randombit.net/)) are pulled in by the build system.

Dependencies that are not automatically installed are:
* [cmake](https://cmake.org/)
* [libgmp](https://gmplib.org/)


Recommended tools:
* [ninja](https://ninja-build.org/)
* [valgrind](http://valgrind.org/) (on Linux)

On Linux your package manager should provide this. On MacOS, installation via [homebrew](http://brew.sh) is recommended.

# Running the code
If you are on Linux or MacOS, running the included shell script `setup_linux_builds` or `setup_macos_builds` will setup multiple builds of the code using various debug and release mode configurations. A sample to get the code executing:

```
$ ./setup_linux_builds
$ cd build/clang-release
$ ninja && ninja test
```

This will setup multiple builds, and using clang with optimizations, compile everything and run the unit tests to make sure everything works.

# Code structure

The code is organized into several self-contained libraries. These are organized in the `lib` and `include` directories. The `apps` directories contain the programs that use the libraries for various tasks. The libraries are tested via the unit tests in the `test` directory.

Note: The code make heavy use of templates and template meta-programming. The easiest way to understand how things work is to look through the unit tests.
