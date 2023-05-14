# svg图形解析

笔者近期需要读取svg，在本地渲染出与chrome解析一致的图形，故处理了此问题；

本文假设读者熟悉仿射变换知识

若transform后接matrix则做一次矩阵运算即可，但如果是分栏属性

## transform字段对应矩阵变换

则会遇到将命令转换为矩阵，以及各命令行计算顺序的问题，svg文档似乎没有明说，属实麻烦；笔者验证后得知属性计算是从后往前的；以上图为例，先计算scale，再计算skewX...

因此需要注意的点有：

1，上述计算顺序问题；

2，scale 参数为1个时， 意味着y取相同参数；

3，rotate参数为1时，代表绕原点旋转传入角度。参数为3个时，参数1为角度，2, 3为围绕坐标；

值得注意的是skewX(40):的偏转矩阵为

Eigen::Matrix3d imat = Eigen::Matrix3d::Identity();

imat(0,1) = tan(40);

skewY(40)则 imat(1,0) = tan(40);

参考链接：

[How to calculate SVG transform matrix from rotate/translate/scale values?](http://link.zhihu.com/?target=https%3A//stackoverflow.com/questions/15133977/how-to-calculate-svg-transform-matrix-from-rotate-translate-scale-values)

拓展：

[坐标定位 - SVG：可缩放矢量图形 | MDN](http://link.zhihu.com/?target=https%3A//developer.mozilla.org/zh-CN/docs/Web/SVG/Tutorial/Positions)

## ellipse中心点推算

svg 提供了方便于描述绘制的方法，其表达式整体推算文档已给出

[Implementation Notes - SVG 2](http://link.zhihu.com/?target=https%3A//www.w3.org/TR/SVG/implnote.html%23ArcImplementationNotes)

写成js或c并不困难，注意传入角度为弧度！