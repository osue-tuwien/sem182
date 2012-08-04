sem182
======

Semaphore libraries used for OSUE@TU-Vienna

Build
-----
```
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
```

Packaging
---------
```
$ cpack -G DEB CPackConfig.make
$ cpack -G RPM CPackConfig.make
```
