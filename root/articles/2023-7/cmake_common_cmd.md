# cmake常用命令&&关键字解释

## INTERFACE
表示
> We want to state that anybody linking to MathFunctions needs to include the current source directory, while MathFunctions itself doesn't.
即，链接目标cmakeLists.txt的发起者，将受到interface语句影响，而被链接模块本身，不受影响；常用于简化include操作，cxx版本设置操作；

## cmake --DUSEMYMATH=ON
用于开启option USEMYMATH选项为true

## cmake --install . --config Debug --prefix "/home/myuser/installdir"
将当前构建目录的debug版本安装到--prefix "/home/myuser/installdir"路径；需要cmake构建时写好install相关命令；

## target_compile_definition(SqrtLibrary PRIVATE "HAVE_LOG")
给当前模块添加 HAVE_LOG宏进行编译，PRIVATE表示该属性不可传递；

## ctest -C Debug -VV
执行当前构建目录debug模式单元测试，需要cmake编写unit test用例；

## cpack -G ZIP -C Debug
当前目录项目debug模式依赖库，include，lib打包为tgz
