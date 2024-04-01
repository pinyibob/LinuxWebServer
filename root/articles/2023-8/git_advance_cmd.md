# git的项目构建应用场景相关命令

设置用户信息
git config --global user.name ""
git config --global user.email ""

git 查看远端
git remote -v

设置origin地址
git remote add origin url

将当前分支推送到远端分支
git push -u origin <branchname>

在远程建立分支并推送本地
git push --set-upstream origin <branchname>

github下载的zip包建立git管理
// zip link remote lib
git init
git remote add origin https:/....
git remote update

添加子仓库
git submodule add <url> <target directory>

初始化子仓库
git submodule init -r

## situation
// git分支游离态解决，创建新分支即可
[link](https://blog.csdn.net/u011240877/article/details/76273335)

// 如何把本地仓库推送到新的远程仓库
[link](https://blog.csdn.net/weixin_44038264/article/details/119671062)