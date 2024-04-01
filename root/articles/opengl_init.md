# opengl初体验

## 关于入门opengl，一定要知道的事
1. opengl是右手坐标系:

让右手拇指，食指，中指依次垂直， 依次指向x,y,z轴

2. 包含文件时，先包含windows.h， glew需要在gl前包含

3. 对于新项目或者新手，对于opengl api库的选择，建议使用GLAD 而不是glew；

4. opengl有很多跨平台界面库，SFML，SDL，GLUT，GLFW，我建议你使用GLFW，因为可以有很好的学习资料[link](https://learnopengl.com/Getting-started/Hello-Triangle)

## opengl编程包含哪些内容
OpenGL 是一个底层的图形渲染 API，需要通过编程来操作它的渲染管线 (Rendering Pipeline) ，实现图形渲染效果。在 OpenGL编程中，一般可以分为以下几个部分：

1. 渲染管线: OpenGL渲染数据传入GPU后，经过一系列的操作被渲染到屏幕上，此过程称之为渲染管线
2. 顶点和片元着色器: OpenGL进行渲染的最后一步是按照一定的材质和光照条件对顶点和片元进行着色，顶点和片元着色器就是用于完成这个过程的。
3. 纹理贴图: 纹理贴图是用来增强3D模型的真实感的。OpenGL 可以通过多种方式对纹理进行贴图操作。
常用库：OpenGL 除了上述你所说的 glew、glm、glfw 之外，还有其他常用库，如：SOIL, FreeImage, Assimp 等，用于处理纹理、图像加载和π各种格式模型导入等。

## opengl常用搭配的库
GLEW 用于解决 OpenGL 标准库的扩展问题，它提供了一个简单的方法来获取 OpenGL 扩展和核心函数的指针。通过调用 GLEW 接口函数，你可以查询 OpenGL 和 OpenGL 扩展的功能，并提供一个静态库来导出所有函数指针。
有着同样功能的是GLAD，不同的是glad是一个相对较新的库；

GLM 则是一个针对 OpenGL 编程的数学库。它提供了多种数学运算类和函数，用于简化 OpenGL 编程，比如矩阵、向量、四元数和其他数学对象等，可以方便地用于场景变换和坐标变换等操作。

使用 GLEW 和 GLM 可以使得 OpenGL 开发人员更加专注于产品的创新和开发，避免了使用原始的平台特定扩展机制去获得 OpenGL 扩展和一致扩展接口的麻烦。

glfw是opengl的一个窗口界面库，跨平台，可以用来显示图形，作为测试使用

## 使用glfw和glm，glew的一个测试代码
```
#include <windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>

#include <iostream>

int main(void)
{
    GLFWwindow* window = nullptr;

    /* 初始化 glfw */
    if (!glfwInit())
        return -1;

    /* 创建 glfw 窗口 */
    window = glfwCreateWindow(640, 480, "OpenGL example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* 设置当前窗口为当前线程的显卡上下文 */
    glfwMakeContextCurrent(window);

    /* 循环绘制 */
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;

        /* 获取窗口大小 */
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        /* 切换到投影矩阵，并重置矩阵 */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        /* 设置视口 */
        glViewport(0, 0, width, height);

        /* 设置透视投影矩阵 */
        gluPerspective(45.0f, ratio, 1.0f, 100.0f);

        /* 切换回模型视图矩阵，并重置矩阵 */
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /* 设置观察点 */
        gluLookAt(0.0f, 0.0f, 5.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f);

        /* 旋转立方体 */
        //glRotatef((float)glfwGetTime() * 50, 0.0f, 1.0f, 0.0f);

        /* 绘制立方体 */
        glBegin(GL_QUADS);
        /* front face */
        glColor3f(1.0f, 0.0f, 0.0f);     // red
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        /* back face */
        glColor3f(0.0f, 1.0f, 0.0f);     // green
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        /* top face */
        glColor3f(0.0f, 0.0f, 1.0f);     // blue
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        /* bottom face */
        glColor3f(1.0f, 1.0f, 0.0f);     // yellow
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        /* right face */
        glColor3f(1.0f, 0.0f, 1.0f);     // magenta
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        /* left face */
        glColor3f(0.0f, 1.0f, 1.0f);     // cyan
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glEnd();

        /* 交换前后缓冲 */
        glfwSwapBuffers(window);

        /* 检测事件 */
        glfwPollEvents();
    }

    /* 清理 */
    glfwTerminate();
    return 0;
}
```

显示一个立方体的六个面，依次绘制，会有后画覆盖先画的情况
