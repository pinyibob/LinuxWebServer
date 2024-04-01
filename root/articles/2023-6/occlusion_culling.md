# 遮挡剔除

在现代gpu中，遮挡剔除是一个影响效率的最关键部分之一；直接影响最后渲染三角形的数目，对于CIM，BIM等静态多物体重叠的业务更是如此；

以下记录笔者搜罗到的遮挡剔除的主流方案

## 预计算遮挡剔除（Potential Visible Set）
是一种静态方法： 划分场景成网格，再进行光线投射去计算网格可见性，得到潜在可见集Potential Visible Set, 运行时只需要花费查找表的时间即可得到可见信息。该方案运行时极低的消耗。在UE4中对应Precomputed Visibility System；可能存在动态划分网格大小的优化；

## 硬件遮挡查询 (Online occlusion culling) 
(UE4 default)
硬件遮挡查询是3D API告诉应用渲染某个物体时是否会绘制像素的特性。当包围盒被遮挡时，则跳过绘制包围盒内的物体。尽管该特性已经存在，但仍然存在两个问题影响它的广泛使用：
1. 解决遮挡查询本身的开销
2. 等待查询结果的延迟。

图形硬件测试遮挡的步骤如下：

1. 初始化一个遮挡查询
2. 关闭帧缓冲和深度缓冲写入，并禁用多余状态
3. 渲染复杂物体的简单保守的近似（一般为包围盒），GPU统计通过深度测试的片元数目
4. 结束遮挡查询
5. 请求查询的结果也就是近似几何体的可见像素数目
6. 如果像素数超过阈值（一般为0）则渲染该复杂物体
1-4步统称为遮挡查询

## 层级深度缓冲(Software Rasterization)
*遮挡查询*可以优化成层级遮挡查询，将相邻物体打包放入kd，bsp中，进行快速判断；
层级的遮挡剔除算法如下，从根节点开始：

1. 为节点执行遮挡查询
2. 暂停并等待查询结果
3. 如果节点可见：
    1. 如果是内部节点，则将子节点从前到后排序并为它们递归执行此算法
    2. 如果是叶子节点，则渲染该节点包含的物体
该方法可进一步优化为连贯性层级剔除(coherent hierarchical culling)；

[details_link](https://zhuanlan.zhihu.com/p/586628315)

## 软遮挡剔除
这个方案最早是Frostbite提出来，首先利用CPU构造一个低分辨率的Z-Buffer，在Z-Buffer上绘制一些场景中较大的遮挡体（美术设定的一些大物体+地形），在构造好的Z-Buffer上，绘制小物体的包围盒，然后执行类似于occlusion query的操作，查询当前物体是否被遮挡。所有操作都在 CPU 上完成

此算法优点：
1. 遮挡测试结果立即可用，帧延迟为零
2. 它以 CPU 时间为代价节省了 GPU 时间（看你的性能瓶颈在哪，有些情况算是优点吧）
3. 可多线程扩展加速


## Hiz遮挡剔除示例
本算法主要分为四步：

1. 遮挡缓冲区生成
2. Hierarchical-Z mipmap生成
3. 计算着色器判断可见性
4. 绘制可见实体
[计算着色器代码](https://blog.selfshadow.com/publications/practical-visibility/)
> Hierarchical-Z mipmap是一种在Hierarchical-Z图层结构中使用Mipmapping进行优化的技术。Mipmapping是一种用于处理纹理缩小时减少moiré纹理的技术。这种技术维护了一系列被缩小后的mipmap级别，每个级别都是原始纹理的一半。在渲染中，根据距离和屏幕上的尺寸，使用恰当的mipmap层级进行纹理采样，从而避免了浪费许多像素计算的问题。
> 在使用Hierarchical-Z mipmap进行深度缓冲遮挡剔除时，每个层级的深度缓冲区都会放大到和屏幕大小相同。这个放大过程中，深度信息会被合并到较低的层级中，也就是创建了Mipmapping图层。因此，有了Mipmapping支持的深度缓存哈希图层和最小重叠尺寸之后，就可以利用Mipmapping特性提高搜索效率了。
在渲染管道中，物体的深度信息先被写入深度缓冲区，然后在下一帧中，从最低的Mipmapping图层一直到最顶层，检索每个层级下属的子区域是否有可见的物体，一旦找到可见的物体，就可以停止继续向下层级搜索，最终获得物体的可见性。
> 这种方法在处理大型场景和复杂的几何对象时，速度非常快，因为可以避免对不可见物体的重复计算和深度测试。但需要注意的是，Mipmapped缓存消耗的显存会多于没有Mipmapped的缓存，因为多了很多颜色和深度缓存图层，因此需要根据具体情况来选择是否使用这种技术。

参考链接：

[Hiz](https://blog.csdn.net/qq_35312463/article/details/124072312)

[Tolo_GPU_Gems](https://zhuanlan.zhihu.com/p/271830806)

[Nv](https://developer.nvidia.com/gpugems/gpugems2/part-i-geometric-complexity/chapter-6-hardware-occlusion-queries-made-useful)

[PVS && Hierarchical Z Buffer details](https://zhuanlan.zhihu.com/p/441754705)
