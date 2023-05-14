有关qt多语言工作的介绍，笔者已经写过一篇文章了，里面详细介绍了ts文件的结构；

ts文件的维护对翻译人员往往不是很友好，因此笔者写了一个工具能把ts，xlsx互转；另外提供了取消行号的实现，便于代码管理；

从这个项目中可以得到这些内容：

qt的cmake构建，

一个基于qt的xlsx读写库，由[有梦想的伟仔/QtXlsxWriter](http://link.zhihu.com/?target=https%3A//gitee.com/ayowin/QtXlsxWriter.git)创作，笔者将其用cmake构建了一下；

ts文件（xml）的读写；

excel文件读写范例；



能够写代码解决的，绝不做重复的事情解决；希望对大家有所帮助，能够复用里面的一些代码写出符合生产需要的工具来；



github:

[pinyibob/TsFile_XlsxFile_transformXlsxFile_transform.git](http://link.zhihu.com/?target=https%3A//github.com/pinyibob/TsFile_XlsxFile_transform.git)