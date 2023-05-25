# 智能指针的几个进阶问题

## 循环引用导致对象无法析构
``` c++
class A;
class B {
public:
    B() {}
    ~B() { cout << "B destructor" << endl; }
    void setSharedPtr(const std::shared_ptr<A> sharedPtr) {
        spA = sharedPtr;
    }
private:
    std::shared_ptr<A> spA;
};

class A {
public:
    A() {}
    ~A() { cout << "A destructor" << endl; }
    void setSharedPtr(const std::shared_ptr<B> sharedPtr) {
        spB = sharedPtr;
    }
private:
    std::shared_ptr<B> spB;
};

void test() {
    std::shared_ptr<A> pA = std::make_shared<A>();
    std::shared_ptr<B> pB = std::make_shared<B>();
    pA->setSharedPtr(pB);
    pB->setSharedPtr(pA);
}

int main() {
    test();
    return 0;
}
```
这是因为当`pB->setSharedPtr(pA)`执行完之后，两个指针的引用计数都变为2，而各自的析构只会让计数减少为1，test推出后不会正常析构；解放方法为，循环引用的一方使用weak_ptr保存另一方的指针；

weak_ptr的使用需要注意
1. weak_ptr必须要从shared_ptr初始化
2. weak_ptr由于不管理对象生命周期，访问对象前应该调用lock方法，判断对象是否存在；

## 不要用shared_ptr保存动态数组
与unique_ptr不同，shared_ptr不支持管理动态数组，如果一定要使用，需要手动输入删除器，不然会出现直接delete指针而缺失[]的情况；
```
shared_ptr<int>sp(new int[10], [](int* p){ delete[]p;});

//析构时自动调用 delete[];
std::unique_ptr<int[]>sp(new int[10]);
```

## 当我们讨论smart ptr的线程安全问题的时候，到底在说什么
1. 智能指针的引用计数变化不会有线程安全问题，因为是原子操作
2. 如果多线程操作同一个smartptr对象，在改变对象时，可能会导致对象的错误释放
3. 智能指针容器的线程安全，和其保存的内容的线程安全，是两码事；

此处引用一个我觉得回答的很好的答案[当我们谈论shared_ptr的线程安全性时，我们在谈论什么 - 果冻虾仁的文章 - 知乎](https://zhuanlan.zhihu.com/p/416289479)