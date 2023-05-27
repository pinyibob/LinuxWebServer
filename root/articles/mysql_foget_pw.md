# 作为站长关于mysql需要知道的事

## 几个重要的mysql变量
max_connections：当前MySQL服务器允许的最大连接数。
datadir：MySQL服务器数据存储目录路径。
innodb_buffer_pool_size： InnoDB引擎占用的内存缓存池的大小。
key_buffer_size： MyISAM引擎占用的内存缓存池的大小。
wait_timeout：当客户端没有向MySQL服务器发送请求时，MySQL服务器将等待的时间量。
query_cache_size：MySQL查询缓存的大小。
log_bin：二进制日志记录文件的名称。该文件用于存储MySQL主从复制或者数据库恢复操作。
log_error：MySQL错误日志文件的路径和名称。该文件存储MySQL服务器运行期间的错误信息。
slow_query_log：日志记录MySQL查询语句执行超时的情况，时间超过了long_query_time值的时间。
max_allowed_packet：一次数据传输时允许的最大尺寸，默认为16MB。

## 不太常用的命令

//显示当前存在连接
show processlist;

//显示两个时间参数
show global variables where Variable_name in ('interactive_timeout', 'wait_timeout');

//设置本次会话的参数
set interactive_timeout = 60;

```
// stop service
systemctl stop mysql

// skip authorizen
mysqld --shared-memory --skip-grant-tables

// enter without password
mysql -u root -p

// abolish pw
use mysql;
update user set authentication_string='' where user='root';

// frash 权限
flush privileges;
// change pw
ALTER USER 'root'@'localhost' IDENTIFIED BY '123456' PASSWORD EXPIRE NEVER; 
alter user 'root'@'localhost' identified by '123456';
grant all privileges  on *.*  to "root"@'localhost';
flush privileges;
exit;

systemctl start mysql
```

---

[参考链接](https://blog.csdn.net/qq_40757240/article/details/118068317)