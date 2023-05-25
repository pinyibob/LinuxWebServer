# svg/canvas填充区域计算

svg的词条可以设置clipPath attribute属性； 应用clipPath通常有两种填充（生效策略）
1. evenodd
2. nonzero

[svg的两种填充 link](https://developer.mozilla.org/zh-CN/docs/Web/SVG/Attribute/fill-rule)

而svg的clipPath之间是求并集的关系，将svg图像转化为clipper适配的数据结构，进行求交运算，对应的bool运算策略应当选取对应的方式；

[clipper2 fillRule](http://www.angusj.com/clipper2/Docs/Units/Clipper/Types/FillRule.htm)