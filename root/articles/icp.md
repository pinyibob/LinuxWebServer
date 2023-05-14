# point_registration ICP

ICP全写为 （iterated closest point），即迭代最近点，用于将两个点群的配准；

配准：同一区域内以不同成像手段所获得的不同图像图形的地理坐标的匹配。包括几何纠正、投影变换与统一比例尺三方面的处理。

简单来讲两个点阵，其中一个点阵通过矩阵变化和平移能够和另一个点阵坐标”重合“，其所构成的匹配点对的距离总和最小时的状态；

ICP名字其实已经说明了策略，和运算过程了：两个待匹配的点云，根据去中心化后的点的最近欧氏距离点对，作为一组映射点；找到若干组对应点对后，计算两个点阵的偏转矩阵（R）和平移向量(t)，然后计算此时的误差值并将待配准点阵的数据更新(M * R + t)，若此时误差值小于预设值或者运算的次数达到预设次数，则停止循环，否则将更新后的M矩阵 坐标重新代入上述过程中；伪代码：

矩阵 M，S；误差 error_keep；计算次数 looptime；
``` code
while(looptime < set_loopTime && current_error > set_error_tolerance)
{
    looptime++;
    TmpMat = 找到距离最近点阵对（M，S）
    [theta, t] = 计算当前点阵对的偏移矩阵（tmpMat， S）
    M = 更新M的坐标（theta， t， M）
    current_error = 自定义误差计算的方法（M，S）
}
```
笔者以为，用户需要自定义的主要有两个部分：1，误差衡量函数；2，得到对映点对计算过程的优化；

ICP在3d，视焦靠近的情况下，可靠性还是可以的；根据笔者的使用体验，在2d条件下，偏转角度过大时，ICP很难得到正确的结果；另外，在2d简单点阵（数目小）的配准中，鲁棒性较差，但如果使用KC算法则需要求一个函数的极值，可能需要三方数学库；

初次了解ICP，最大的难点可能在于计算偏转矩阵过程的证明，对于2d点，笔者找到了两种计算方法的证明

推导过程

方法1：svd分解：点云配准方法--ICP--原理推导及代码证明 - 古月居

方法2：参数变化解耦：标准ICP算法原理总结及基于二维的ICP算法原理_吴小风风的博客-CSDN博客_二维icp算法


``` matlab
附2份matlab的计算偏转矩阵代码

% x->y
function [theta, T] = ICP2D(x,y)
M = x;
S = y;
ma = mean(M)
sa = mean(S)
Mc = M;
Ms = S;
Mc(:,1) = Mc(:,1) - ma(1)
Mc(:,2) = Mc(:,2) - ma(2)
Ms(:,1) = Ms(:,1) - sa(1)
Ms(:,2) = Ms(:,2) - sa(2)

W = Ms'*Mc;
[u,r,v] = svd(W)
R = v*u'
T = sa - ma*R

%验算
MFi = zeros(size(M))
MFi(:,1) = M * R(:,1) + T(1)
MFi(:,2) = M * R(:,2) + T(2)
all(abs(MFi(:)-S(:))<0.00001)
```
easy type:
这个地方要注意的是计算出来的角度按照逆时针为正的方向
``` matlab
function [theta, dp] = pICP(x,y)
s = y;
m = x;
my = mean(y)
mx = mwan(x)

s(:,1) = y(:,1) - my(1);
s(:,2) = y(:,2) - my(2);
m(:,1) = x(:,1) - mx(1);
m(:,2) = x(:,2) - mx(2);

len = size(x,1)
ups = 0;
downs = 0;
for ii = 1 :len;
   ups = ups + s(ii,1)*m(ii,2) - s(ii,2)*m(ii,1);
   downs = downs + s(ii,1)*m(ii,1) + s(ii,2)*m(ii,2);
end;

theta = -atan(ups/downs);
mat = [cos(theta),sin(theta);-sin(theta),cos(theta)]
dp = my - mx * mat;
```