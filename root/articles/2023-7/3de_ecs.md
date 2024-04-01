# ecs架构在游戏引擎场景的优势

## 什么是ecs
|---|---|
|特点| 通过组合替代继承，解决继承灵活性较差的问题； 解决修改一个复杂对象，内存命中不友好的问题 |
|包含概念|1.Entity；2.Component；3.System；|
|构成组件|0.签名; 1.实体管理器；2.组件管理器；3.系统管理器；4.协调器；|

## 各组件功能
1. 签名
需要有system到所关心的compnent的映射，entity具有哪些compnent的映射，
使用signature来完成，可以使用bitset实现；

2. 实体管理器
entity manager负责分配entity id；
维护id创建、注销，以及其对应签名映射；

3. 组件数组
我们需要创建一个数据结构来存放所有同类型的 Component，它本质上是一个简单的紧凑的数组。如果将 Entity 作为 Component 数组的索引，那么获取 Entity 相关 Component 的过程将会变得非常简单。

4. 组件管理器
该管理器负责在 Component 需要添加或删除时与所有不同的 ComponentArray 进行通信。

5. 系统管理器
System 负责对一组具有特定 Component 签名的 Entity 列表进行处理。这里的“特定”是指 Entity 应该包含所有该 System 需要的 Components。

6. 协调器
我们现在已经实现了大部分基础模块，我们有一个由 EntityManager 管理的 Entity，一个由 ComponentManager 管理的 Component，还有一个由 SystemManager 管理的 System。现在我们需要一个模块来建立这三个 Manager 之间的交流。

## reference
[一文看懂ECS架构](https://zhuanlan.zhihu.com/p/618971664)