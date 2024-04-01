wsl2是一个真正的虚拟机，他的dns主机是和windows本机处于同一个局域网的。
因此代理地址需要设置为dns地址，port为windows本机设置的代理的port，这个过程注意打开防火墙！另外如果是CFW则需要打开允许局域网访问的功能。

dns地址
cat /etc/resolv.conf

设置代理
export http_proxy="ip:port"