# Nanos-lite

Nanos-lite is the simplified version of Nanos (http://cslab.nju.edu.cn/opsystem).
It is ported to the [AM project](https://github.com/NJU-ProjectN/nexus-am.git).
It is a two-tasking operating system with the following features
* ramdisk device drivers
* raw program loader
* memory management with paging
* a simple file system
  * with fix number and size of files
  * without directory
  * some device files
* 6 system calls
  * open, read, write, lseek, close, brk
* scheduler with two tasks
