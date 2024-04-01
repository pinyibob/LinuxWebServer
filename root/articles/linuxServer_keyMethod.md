# 当我们讨论ET/LT，（非）阻塞， 同步/异步的时候，到底在说什么

## 名词解释

### 非阻塞socket
- 阻塞和非阻塞的概念应用于所有的文件描述符；阻塞io可以由于无法立即完成而被操作系统挂起，socket基础api中可能阻塞的有，accpet, connect, recv, send;
- 针对非阻塞io的系统调用总是会立即返回，对于accept,send,recv， error通常为EAGAIN（再来一次）或者EWOULDBLOCK(期望阻塞)，对于connect则是EINPORGRESS（处理中）,可以根据返回值，使用非阻塞connect的策略；   ---page 126

### io复用
- 用io复用函数，在内核对一些对象注册一组事件，将对象就绪的事件，返回给应用程序；（监听多个io事件）

### 非阻塞connet
- 对非阻塞socket调用connnet时，连接可能未建立；如果此时调用select、poll监听这个socket的可写事件，当函数返回后，再利用getsockopt读取错误码并清除该socket上的错误。错误码是0则连接成功，否则失败； ---page 163

### ET/LT, EPOLLONESHOT
- ET/LT是指epoll工作的两种模式，LT是默认模式，允许用户不处理（完）某次io事件，下次调用epoll时，仍然返回该事件，直到用户处理结束；对于ET模式，用户必须处理完某次io事件，不然epoll不再告知该事件。ET的效率提高，来源于降低同一个epoll事件被触发的次数；（ET模式工作的文件描述符，必须处于非阻塞状态）
- EPOLLONESHOT的使用场景，通常是让某个文件描述符只被一个线程处理，在重置该事件前，这个文件描述符的事件将不再被触发；例如某次处理EPOLLIN读取socket信息的主体是线程1， 当socket再次可读时，可能又调用线程2来处理这个socket，此时就有***两个线程处理一个socket***；

- 重置方法：
``` c++
int reset_event(int epollfd, int sock_fd)
{
    epoll_event ev;
    ev.data.fd = sock_fd;
    ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}
```

### 同步/异步 && 半同步/半异步 && 领导者模式
同步程序完全按照代码的顺序执行，异步指程序执行的需要由系统时间来驱动；
---

半同步半异步中，同步线程用于处理客户逻辑，异步线程处理随机的用户请求，形成的io事件；处理事件后，将逻辑任务插入工作队列，即客户逻辑同步处理；
服务器程序中,进一步引入了半同步、半反应堆的模式；异步线程只有一个负责监听事件，其他的线程睡眠在请求队列上，准备处理rw，信号等任务；
缺点： 主线程和工作线程共享请求队列，竞争锁；如果客户多，工作线程少，可能积攒大量的用户数事件；工作线程太多，则耗费cpu资源；因此引出了，更高效的，半同步，半异步模式； 即每个工作线程处理一个用户socket产生的信息，自己维护一个epoll循环；
