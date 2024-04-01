//查看当前git设置代理
git config --global --get http.proxy

//设置http，https代理
git config --global http.proxy '127.0.0.1:7890'
git config --global https.proxy '127.0.0.1:7890'

//禁用proxy
git config --global --unset http.proxy

//powershell环境设置代理
$env:HTTP_PROXY="http://127.0.0.1:7890"
$env:HTTPS_PROXY="https://127.0.0.1:7890"

//linux设置代理
export http（https,ftp）_proxy=http://ip:port
或者vim /etc/profile写入