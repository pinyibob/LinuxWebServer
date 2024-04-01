# world/fpc camera in opengl

opengl提供了方便的mvp矩阵设置方法，利用这些接口，是很好实现一个camera的；常用的camera包括世界自由相机和FPS平面移动相机，他们的区别只是在于是否考虑y轴方向的移动，对于fps相机，能够对y轴造成影响的只有重力系统和碰撞系统，总的来讲，就是物理系统的设定；

下面这份代码是笔者使用glew的opengl api管理库写的，而不是glad；
对于界面库，也是使用的较旧的freeglut；

## world camera
```
void FPSCamera::updateCameraHoriMovement() {
	float dx = 0;
	float dz = 0;
    float dy = 0;

    // 按键情况
	if (isWPressing)
		dz += 2;
	if (isSPressing)
		dz -= 2;
	if (isAPressing)
		dx -= 2;
	if (isDPressing)
		dx += 2;

    // viewMatrix为成员变量
	if (dz != 0 || dx != 0) 
    {
        // 此处的计算，使用齐次坐标表示法， 这也是笔者认为最简洁明了，最优雅的做法；

        // 构造相机世界的移动向量
        glm::vec4 dpos = glm::vec4(dx, dy, -dz, 0);
        // 将偏移情况，计算到世界坐标系的情况, 此处注意计算顺序！
        auto vworld = dpos * viewMatrix;

        // 转化为普通向量表示, 应用到ca，ta中
        glm::vec3 iv2(vworld[0], vworld[1], vworld[2]);
        cameraPos += iv2 * MoveSpeed;
        targetPos = cameraPos + iv2 * 1.5f;

		//每次做完坐标变换后，先进行碰撞检测来调整坐标
		physicsEngine->outCollisionTest(cameraPos, targetPos);
		physicsEngine->inCollisionTest(cameraPos, targetPos);
	}

}
```

## fps camera
```
void FPSCamera::updateCameraHoriMovement() {
	float dx = 0;
	float dz = 0;
    float dy = 0;

    // 按键情况
	if (isWPressing)
		dz += 2;
	if (isSPressing)
		dz -= 2;
	if (isAPressing)
		dx -= 2;
	if (isDPressing)
		dx += 2;

    // viewMatrix为成员变量
	if (dz != 0 || dx != 0) 
    {
        // 此处的计算，使用齐次坐标表示法， 这也是笔者认为最简洁明了，最优雅的做法；

        // 构造相机世界的移动向量
        glm::vec4 dpos = glm::vec4(dx, dy, -dz, 0);
        // 将偏移情况，计算到世界坐标系的情况, 此处注意计算顺序！
        auto vworld = dpos * viewMatrix;

        // 对于fps相机，将计算结果的y分量永远设置为0即可
        glm::vec3 iv2(vworld[0], 0.0f, vworld[2]);
        cameraPos += iv2 * MoveSpeed;
        targetPos = cameraPos + iv2 * 1.5f;

		//每次做完坐标变换后，先进行碰撞检测来调整坐标
		physicsEngine->outCollisionTest(cameraPos, targetPos);
		physicsEngine->inCollisionTest(cameraPos, targetPos);
	}

}
```

在设置完lookAt函数的参数后，刷新mv矩阵
```
void FPSCamera::updateView() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glm::mat4 matroll = glm::rotate(glm::mat4(1.0), -roll, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 matpitch = glm::rotate(glm::mat4(1.0), -pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 matyaw = glm::rotate(glm::mat4(1.0), -yaw, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mattranslate = glm::translate(glm::mat4(1.0f), -cameraPos);

	viewMatrix = matroll * matpitch * matyaw * mattranslate;

	glMultMatrixf((float*)glm::value_ptr(viewMatrix));
}
```