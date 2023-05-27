# cpp的内存布局

## 虚函数表的内存结构
虚函数表可以在汇编和编译器层面被感知到，是cpp多态实现的基础，也是除了模板和c语言最大区别的地方；

每个有独特虚函数的类均会增加一个虚表，在类实例化的地址首部存虚函数指针；
对于不同继承情况的类实例化内存布局情况，在不同编译器可能是不同的；这也是cpp不跨编译器的另一重要原因；

可以通过msvc编译器工具可视化类的内存布局

可以参考我很喜欢的一个作者froser的一篇文章[C++虚函数原理](https://zhuanlan.zhihu.com/p/619853377)

## cpp内存分类

1. 代码区/文本区：这个区域用来存放程序的可执行代码，通常是只读的。
2. 全局区/静态数据区：这个区域用来存放全局变量和静态变量，它们在程序运行期间都是存在的，直到程序终止时才被销毁。
> const char* ic = "ab";
3. 栈：每个线程都有一个栈，用来存储局部变量、临时变量和函数返回地址等信息。每次函数调用都会在栈上分配一些内存，函数返回时从栈中释放这些内存。（1m左右）
4. 堆：这个区域用来存放动态分配的内存，由程序员自己控制内存的分配和释放。（32位系统2-3g左右）