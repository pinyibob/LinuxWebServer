奇异值分解是矩阵运算中的一种降维技巧，可以对矩阵进行特征提取，在图形去噪，推荐系统，主成分分析（PCA），数据压缩，数学求解（如IPC）等方面均有应用；

1. 首先要知道以下几个概念：

- 特征值；
- 特征向量；
- n阶满秩矩阵的特征分解条件及步骤；
- 正交矩阵；

2. 特征分解和奇异值分解

对于n阶满秩对称矩阵A，总是能对其进行特征值、特征向量的分解；（A矩阵可对角化的充要条件是A有n组线性无关的特征向量）

普遍的，对于mxn的普通矩阵B，总是能对其进行奇异值分解；通过BBT的计算，将B转化为mxm的矩阵，再进行特征值分解，通过数学证明可将B分解为UΣVT，其中Σ为奇异值的对角矩阵；

3. 奇异值分解的数学证明：

将矩阵 A*At和At*A分别进行特征值分解，发现其特征向量矩阵分别对应矩阵svd分解的U，V矩阵，且Σ（奇异值矩阵）为其特征值矩阵的平方根；

第1，2个链接均有证明；

4. 特征值分解的过程，实质求的是矩阵在特征向量方向上的不同比例，特征值越大，则表明其受到对应特征向量的影响越大；本质是特征提取的过程；对于特征值分解在不同场景下的实质意义，可以参考链接3提供的多个例子；

参考链接：

[奇异值分解(SVD)原理及实例分析_Freemanzxp-CSDN博客_svd分解例子](http://link.zhihu.com/?target=https%3A//blog.csdn.net/zpalyq110/article/details/86751064)

[机器学习中SVD总结](http://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/Dv51K8JETakIKe5dPBAPVg)

[Feature Column from the AMS](http://link.zhihu.com/?target=http%3A//www.ams.org/publicoutreach/feature-column/fcarc-svd)

编辑于 2022-03-08 16:00