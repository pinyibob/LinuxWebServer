# tcp优雅拆包

tcp是流式传输，意味着消息没有边界；消息没有边界，但是信息是有边界的，例如某个类的序列化；调用recv api可能多读，将下一个包的序列读出了一部分，也可能少读；所以需要使用一些方法防止粘包，这是一个工程概念，实际tcp是只有消息帧的概念的；

提供两种不适用特殊符号间隔，即使异步读取同一个socket也不会粘包的两种方法，尽量做到一字不多读，一字不少读；

## 通过首部逐个读出消息长度而避免沾包
接收方先解析出消息长度，再按需读取剩下长度字符串即可！

演示代码提供有限的鲁棒支持，重点看消息长度编码和解码

```
//    cnt is msg, len is (msg len)
void sdMsg(const char* cnt, size_t len)
{
        //下方函数将“消息长度”进行编码
        auto msg_size = len;
        std::vector<uint8_t> length_bytes;
        do {
            uint8_t byte = msg_size & 0x7F;
            msg_size >>= 7;
            if (msg_size > 0) {
                byte |= 0x80;
            }
            length_bytes.push_back(byte);
        } while (msg_size > 0);

        //将 长度信息 和本体绑定发送
        
}

//   recv
            //accept socket省略

            // “body长度” 字节数
            uint32_t msg_len = 0;
            // body长度
            uint32_t msg_size = 0;
            // 辅助数字
            int shift = 0;
            //逐个读取信息长度
            do
            {
                uint8_t byte;
                int icharlen = recv(sockConn, (char*)&byte, 1, 0);

                if (icharlen < 0)
                    goto outofloop;

                if(icharlen != 1)
                    continue;

                msg_size |= (byte & 0x7F) << shift;
                shift += 7;
                msg_len += 1;
                if ((byte & 0x80) == 0) {
                    break;
                }
            } while (msg_len < MAX_MESSAGE_SIZE);
            //得到消息长度后，继续读取对应大小的buffer
            size_t bodyRead = 0;
            std::unique_ptr<char>bodyBuff(new char[msg_size] {'\0'});
            while (bodyRead != msg_size)
            {
                int irecvL = recv(sockConn, bodyBuff.get(), msg_size - bodyRead, 0);

                if (irecvL < 0)
                    goto outofloop;

                if (irecvL > 0)
                    bodyRead += irecvL;
            }

            // 安全检查
            if (msg_len > MAX_MESSAGE_SIZE) {
                throw std::runtime_error("Message too large");
            }
            //else if (bytes_received - msg_len < msg_size) {
            //    throw std::runtime_error("Incomplete message received");
            //}

            std::string message(bodyBuff.get(), msg_size);
            //balabala  业务continue
```
编码过程是每次取长度数字的低位7位，首位固定为1组成一个byte保存；最后的一个byte开头必然是0；解码过程只要读取到第一个第八位不是1的char结束，将获得的数字拼接即可；这样就将长度字符和msgBody分隔开不会误读；

好处是，几乎没有传输任何不必要的东西；；坏处是增加recv获取编码的复杂度；

## 通过设置pack为固定256char 或512char大小
这样每次从socket中读取的内容也是不会有粘包风险，但是容易空间浪费