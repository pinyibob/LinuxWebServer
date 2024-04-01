# shader

shader是运行在gpu上的小程序，每个核心都顺序执行一个渲染管线的shader，shader函数只考虑输入和输出，是很典型的函数式编程；
opengl中，通过glsl语言写shader；

一个shader总是包括
1. 版本
2. 输入输出结构
3. 函数体
4. uniform : 是cpu传输数据到gpu着色器的一种方式，是一种全局数据；它提供了一种很好的在应用和着色器交流的方式；

uniform 定义交互全局变量方法示例
```
//fragment shader
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";


// code:  set value to fs
int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
```

shader常用关键字有：
1. 数据类型关键字：float，int，bool等用于定义变量类型的关键字。
2. 向量关键字：vec2，vec3，vec4等表示包含2，3或4个浮点数的向量类型。
3. 矩阵关键字：mat2，mat3，mat4等表示2x2，3x3或4x4矩阵类型。
4. 纹理采样器关键字：sampler2D，samplerCube等用于定义纹理采样器类型的关键字。
5. 控制流关键字：if，else，while等用于控制代码流程的关键字。
6. 函数关键字：void，return等用于定义函数类型的关键字。
这些关键字可以通过组合使用来编写各种shader程序，从而实现各种不同的渲染效果。