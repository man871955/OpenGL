# OpenGL
使用OpenGL渲染一个场景
OpenGL文件夹中：  
Assimp 以及 glfw 中为所需的库;   
include为需要用到的头文件等。   
                 
Project1文件夹中：   
FragmentShaders 中为片段着色器;  
VertexShaders 中为顶点着色器:  
item前缀为实际渲染物体或平面用到的PBR着色器；  
PointShadow前缀为渲染点阴影用到的深度缓冲着色器;  
Shader前缀为只渲染贴图的着色器;  
Shadow前缀为渲染直线光阴影用到的深度缓冲着色器;  
skyBox前缀为渲染天空盒用到的着色器。  

model和pictures为用到的模型与材质贴图。  

.cpp和.h为源代码文件。  
其余文件无需关心。  
