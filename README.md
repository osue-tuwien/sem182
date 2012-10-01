sem182
======

Semaphore libraries used for OSUE@TU-Vienna

Build
-----
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Install
-------
```
$ sudo make install
```

Debug/Test Builds
-----------------
```
$ cd build
$ cmake -D CMAKE_BUILD_TYPE=debug ..
$ make
$ make test
```

Packaging
---------
```
$ cpack -G DEB CPackConfig.make
$ cpack -G RPM CPackConfig.make
```

Man Pages
---------
```
$ pandoc -s -w man sem182.md -o sem182.3
$ pandoc -s -w man msem182.md -o msem182.3
```
