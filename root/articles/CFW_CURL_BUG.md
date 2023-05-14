# 开启proxy时python链接失败

python用的越来越多了，方便的包管理器也是依赖网络的，爬墙的时候遇到一些奇怪的问题，就惹人生气了；

本文记录一次开启代理后，python进行网络访问报错的处理经历；

原因：python的urllib在发送请求时，读取windows代理注册表项127.0.0.1:port_num，推测成https://127.0.0.1:port_num，而我们并没有开启https代理故导致错误

解决方法：

1，使用PAC代理

2，在代理注册表前添加http://前缀， 注册表路径为：HKEY_CURRENT_USER->Software->Microsoft->windows->CurrentVersion->Internet Settings : ProxyServer


参考链接：

[clash for windows 系统代理时 pip 出现 ProxyError 的情况分析记录](https://note.bobo.moe/2021/02/clash-for-windows-pip-proxyerror.html)

[bpo-42627: Fix wrong parsing of Windows registry proxy settings by CrazyBoyFeng · Pull Request #26307 · python/cpython](https://github.com/python/cpython/pull/26307)