# OpenGL  
本项目使用 OpenGL 渲染一个场景，包含 Bling-Phong 光照，物体阴影和PBR贴图等。  
  
----------文件结构----------  
src 文件夹中：   
        VertexShaders 中为顶点着色器:  
        FragmentShaders 中为片段着色器;  
		Shader 前缀为只渲染贴图的着色器;   
		skyBox 前缀为渲染天空盒用到的着色器；   
		item 前缀为实际渲染物体或平面用到的PBR着色器；  
		Shadow 前缀为渲染直线光阴影用到的深度缓冲着色器;  
		PointShadow 前缀为渲染点阴影用到的深度缓冲着色器;  

  
	shade.cpp 为主程序入口。  
	glad.c 和 stb_image.h 为资源导入所需的库。  
	camera.h 为摄像机类；mesh.h 为网格类；model.h 为模型类。  
	shader.h 为着色器类，source.h 主要包含一些初始信息设置以及相关函数。 
  
release 文件夹中：  
	CG Project.exe 为可执行文件。  
	assimp 前缀为运行过程需要链接的外部库。  
	model 和 pictures 中为使用的模型与材质贴图。  
	VertexShaders 和 FragmentShaders 中为上述src中的着色器。  
  
demo 文件夹中：  
	包含一些实际效果图与演示视频。  
  
----------渲染过程----------  
首先使用类创建上述的若干个着色器，以及需要渲染的模型，并加载用到的天空盒以及材料材质；  
然后创建一个立方体深度缓冲，并进行所有着色器的相关基础参数设置；  
循环渲染过程：  
	(1)首先使用深度缓冲着色器(PointShadow)绑定前面建立的立方体深度缓冲，然后进行  
	    场景的渲染，此时立方体深度缓冲会存储场景的全部深度信息，且该着色器不会在屏  
	    幕上实际渲染场景。  
	(2)然后使用PBR着色器(item)渲染实际的场景。在渲染每个表面或物体时，着色器都要  
	    绑定相应的贴图材质，同时使用(1)中得到的立方体深度缓冲进行阴影计算。  
	(3)最后使用普通着色器(Shader)渲染唯一的球形白光源，使用天空盒着色器(skyBox)绑  
	    定天空盒材质并渲染天空盒。
