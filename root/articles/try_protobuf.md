# protobuf库使用体验

protobuf 可以用来完成很多任务，例如：

1. 数据序列化，特别是网络传输、IO读写、分布式计算等场景下进行数据的序列化和反序列化。
2. 它也可以用作类似 XML 的配置文件，但是 protobuf 的二进制格式更加紧凑，而且更容易进行升级和解析。
3. protobuf 还可以作为编解码器，用于编解码消息以及各种协议。

总之，protobuf 是一种对数据进行序列化和反序列化的工具，常用于数据传输和存储，方便各种不同平台和语言之间的数据交换和传递。protobuf

下载编译：
和ffmpeg一样，建议直接使用vcpkg，一个库的编译而已，win又麻烦，不要走弯路
```
vcpkg install protobuf:x64-windows

//如果需要在 Visual Studio 中调用此库，可以使用以下命令进行集成：
vcpkg integrate install
```

1，编辑需要序列化的类，例如创建person.proto文件
```
syntax = "proto3";
   package tutorial;
   message Person {
     string name = 1;
     int32 id = 2;
     repeated string email = 3;
   }

其中syntax的意思是使用proto3版本，package表示命名空间，message则是对应的类

```
2，使用编译生成的protoc.exe将编辑的person.proto文档输出.h, .cpp文件；

```
 E:\vcpkg\packages\protobuf_x64-windows\tools\protobuf\protoc.exe --proto_path=./ ./person.proto --cpp_out=./
```

3，运行测试代码
```
#include <iostream>
#include <fstream>

#include <google/protobuf/message.h>

#include "person.pb.h"

int main()
{
    using namespace std;

    tutorial::Person person;
    person.set_name("Alice");
    person.set_id(123);
    person.add_email("alice@example.com");
    person.add_email("alice@gmail.com");

    fstream output("person.pb", ios::out | ios::binary);
    if (!person.SerializeToOstream(&output)) {
        cerr << "Failed to write person." << endl;
    }
    output.close();

    // ...

    tutorial::Person person2;
    fstream input("person.pb", ios::in | ios::binary);
    if (!person2.ParseFromIstream(&input)) {
        cerr << "Failed to parse person." << endl;
    }
    input.close();

    cout << person2.name() << ": " << person2.id() << endl;
    for (int i = 0; i < person2.email_size(); i++) {
        cout << person2.email(i) << endl;
    }

    return 0;
}
```

可见对于person类，已经成功序列化
