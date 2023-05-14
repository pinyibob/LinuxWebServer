| 类 | 描述 |
| --- | --- |
|epoll_ctx_s|管理epoll fd和注册的事件，iowatcher是一个void指针，用来多态不同io复用机制|
|||


|文件|描述|
| --- | --- |
|epoll.c|描述了使用epoll方法时，事件的添加，删除，和epoll获取；盲点：hio_t类的功能，EVENT_PENDING宏|
