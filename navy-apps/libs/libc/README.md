# C运行库

从newlib-1.6.1移植。

编译后得到archive file，应用程序与libc和libos静态链接后即可执行。

## 与操作系统的接口

libc会使用系统调用实现自身无法完成的功能。系统调用在libos中定义：

```
_open()
_close()
_read()
_write()
_sbrk()
...
```

