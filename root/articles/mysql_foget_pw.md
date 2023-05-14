# mysql密码修改

## 如果忘记密码

```
// stop service
systemctl stop mysql

// skip authorizen
mysqld --shared-memory --skip-grant-tables

// enter without password
mysql -u root -p

// change pw
use mysql;
update user set authentication_string='' where user='root';

// frash 权限
flush privileges;
ALTER USER 'root'@'localhost' IDENTIFIED BY '123456' PASSWORD EXPIRE NEVER; 
alter user 'root'@'localhost' identified by '123456';
grant all privileges  on *.*  to "root"@'localhost';
flush privileges;
exit;

systemctl start mysql
```

---

[转自](https://blog.csdn.net/qq_40757240/article/details/118068317)