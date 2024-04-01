## general usage
find <root path> -name <target name>
> name 支持通配符

find / -amin -10
查找系统最后10分钟访问的文件， atime 表示天数， mmin表示最后修改的文件（分钟），-user表示文件归属， -size表示文件大小筛选（+1000c）（c：字节； k：kb； M:Mb）

| grep 
[reference](https://blog.csdn.net/xxmonstor/article/details/80507769)