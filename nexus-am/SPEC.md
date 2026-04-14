# The Abstract Machine Specification

## 数据结构

* `_Area`代表一段连续的内存，组成`[start, end)`的左闭右开区间。
* `_Screen`描述系统初始化后的屏幕（后续可通过PCI总线设置显示控制器，则此设置不再有效）。
* 按键代码由`_KEY_XXX`指定，其中`_KEY_NONE = 0`。
* `_RegSet`代表体系结构相关的寄存器组。
* `_Event`表示一个异常/中断事件，event域由_EVENT_XXX指定，cause由具体事件指定。
* `_Protect`描述一个被保护的地址空间(`_area`)，以及一个体系结构相关的虚拟地址空间描述符(`ptr`)，如在x86中为页目录基地址。

## 体系结构相关代码规范

每个体系结构在`arch/`中包含一个同名目录，例如`mips32-npc`, `mips32-qemu`, `x86-qemu`，第一个词是ISA，第二个词是运行平台。对于每一个体系结构目录（例如`arch/mips32-npc`），包含：

* `README.md`相关说明。
* `include/`存放相关的头文件。
* `include/arch.h`描述体系结构相关的信息，包括：
  * 整数类型`size_t`, `off_t`的定义。
  * `typedef struct _RegSet {}`代表所有体系结构寄存器。
* `src/`存放相关的源代码文件。
* `img/`存放制作镜像必要的文件。执行`img/burn target files`能将`files`对应的文件列表(.a)链接，并烧录成名为`target`的镜像。

## Turing Machine

* `void _putc(char ch);` 调试输出一个字符，输出到最容易观测的地方。对qemu输出到串口，对Linux native输出到本地控制台。
* `void _halt(int code);` 终止运行并报告返回代码。`code`为0表示正常终止。
* `extern _Area _heap;` 一段可读、可写、可执行的内存，作为可分配的堆区。

## IO Extension

* `void _ioe_init();` 初始化Extension。
* `unsigned long _uptime();` 返回系统启动后的毫秒数。溢出后归零。
* `int _read_key();` 返回按键。如果没有按键返回`_KEY_NONE`。
* `void _draw_rect(const uint32_t *pixels, int x, int y, int w, int h);`绘制`pixels`指定的矩形，其中按行存储了w*h的矩形像素，绘制到(x, y)坐标。像素颜色由32位整数确定，从高位到低位是`00rrggbb`（不论大小端），红绿蓝各8位。
* `void _draw_sync();` 保证之前绘制的内容显示在屏幕上。
* `extern _Screen _screen;` 屏幕的描述信息。在`_ioe_init`后调用后可用。

## Asynchronous Extension

* `void _asye_init(_RegSet* (*l)(Event ev, _RegSet *regs));`初始化Extension。初始化后并不响应异步事件。`l`是监听中断/异常事件的回调函数。在中断/异常事件到来时调用l(ev, regs)。中断结束后将返回到返回值指定的寄存器现场(可以返回传入的参数或NULL)。系统事件：
  * `_EVENT_IRQ_TIME`:时钟中断(无cause)
  * `_EVENT_IRQ_IODEV`:I/O设备中断(无cause)
  * `_EVENT_ERROR`:一般错误(无cause)
  * `_EVENT_PAGE_FAULT`:缺页/页保护错(cause: 产生缺页的地址)
  * `_EVENT_BUS_ERROR`:总线错误(cause: 产生错误的地址)
  * `_EVENT_NUMERIC`:数值错误(无cause)
  * `_EVENT_TRAP`:内核态自陷(无cause)
  * `_EVENT_SYSCALL`: 系统调用(无cause)
* `SYSCALL_ARGx(reg);`从寄存器现场中获取系统调用的参数。其中`x`为`1`~`4`。
* `_RegSet *_make(_Area kstack, void *entry, void *arg);`创建一个内核上下文,参数arg。
* `void _trap();`在内核态自陷。线程需要睡眠/让出CPU时使用。
* `int _istatus(int enable);`设置中断状态(enable非0时打开)。返回设置前的中断状态(0/1)。

## Protection Extension

* `void _pte_init(void*(*palloc)(), void (*pfree)(void*));`初始化Extension。传入两个函数，分别代表分配/释放一个物理页(分配需保证多线程/多处理器安全)。
* `void _protect(_Protect *p);` 创建一个保护的地址空间。
* `void _release(_Protect *p);` 释放一个保护的地址空间。
* `void _map(_Protect *p, void *va, void *pa);`将地址空间的虚拟地址va映射到物理地址pa。单位为一页。
* `void _unmap(_Protect *p, void *va);`释放虚拟地址空间va的一页。
* `void _switch(_Protect *p);`切换到一个保护的地址空间。注意在内核态下，内核代码将始终可用。
* `_RegSet *_umake(_Protect *p, _Area ustack, _Area kstack, void *entry, char *const argv[], char *const envp[]);`创建一个用户进程(地址空间p，用户栈地址ustack，内核栈地址kstack，入口地址entry，参数argv，环境变量envp，argv和envp均以NULL结束).

## Multi-Processor Extension

* `void _mpe_init(void (*entry)());`启动多处理器。
* `int _cpu();`返回当前CPU的编号(从0开始)。
* `intptr_t _atomic_xchg(volatile void *addr, intptr_t newval);`原子交换两数。
* `void _barrier();`保证内存顺序一致性。
* `extern int _NR_CPU;`处理器数量。在`_mpe_init()`调用后有效。
