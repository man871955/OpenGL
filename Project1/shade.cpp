#include "sources.h"

using namespace std;

int  main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif	
	GLFWwindow* window = glfwCreateWindow(800, 600, "21307103", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	Shader myShader = Shader("VertexShaders/Shader.vs", "FragmentShaders/Shader.fs");
	Shader skyShader = Shader("VertexShaders/skyBox.vs", "FragmentShaders/skyBox.fs");
	Shader Shadow_shader("VertexShaders/Shadow.vs", "FragmentShaders/Shadow.fs");
	Shader Point_Shadow("VertexShaders/PointShadow.vs", "FragmentShaders/PointShadow.fs", "VertexShaders/PointShadow.gs");
	Shader item("VertexShaders/item.vs", "FragmentShaders/item.fs");
	Model table("model/table/outdoor_table_chair_set_01_1k.fbx");
	Model chess("model/chess/chess_set_1k.fbx");
	Model table1("model/table1/wooden_table_02_1k.fbx");
	Model duck("model/duck/rubber_duck_toy_1k.fbx");
	Model table2("model/table2/WoodenTable_01_1k.fbx");
	Model statue("model/statue/horse_statue_01_1k.fbx");
	Model lamp("model/lamp/modern_ceiling_lamp_01_1k.fbx");
	Model cat("model/cat/concrete_cat_statue_1k.fbx");
	Model shelves("model/shelves/wooden_display_shelves_01_1k.fbx");
	Model elephant("model/elephant/carved_wooden_elephant_1k.fbx");
	Model book("model/book/book_encyclopedia_set_01_1k.fbx");
	Model plant("model/plant/potted_plant_04_1k.fbx");
	Model camera("model/camera/Camera_01_1k.fbx");
	Model TV("model/TV/Television_01_1k.fbx");
	Model side_table("model/side_table/side_table_01_1k.fbx");

	glViewport(0, 0, 800, 600);
	const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int texture;
	string path;
	path = "pictures/1.png";
	texture = loadTexture(path.c_str());

	vector<std::string> faces
	{
		"pictures/skybox/right.jpg",
		"pictures/skybox/left.jpg",
		"pictures/skybox/top.jpg",
		"pictures/skybox/bottom.jpg",
		"pictures/skybox/front.jpg",
		"pictures/skybox/back.jpg",
	};
	unsigned int skyBox = loadCubemap(faces);

	vector<unsigned int> floor;
	BindPBRTextures(floor, "floor");
	vector<unsigned int> walls;
	BindPBRTextures(walls, "wood");
	vector<unsigned int> steel;
	BindPBRTextures(steel, "steel");
	
	skyShader.use();
	skyShader.setInt("skybox",0);
	myShader.use();
	myShader.setInt("texture_diffuse",0);
	item.use();
	item.setInt("albedoMap", 0);
	item.setInt("normalMap", 1);
	item.setInt("metallicMap", 2);
	item.setInt("roughnessMap", 3);
	item.setInt("aoMap", 4);
	item.setInt("ShadowMap", 5);
	item.setInt("PointShadowMap", 6);

	while (!glfwWindowShouldClose(window)) {

		float current = glfwGetTime();
		deltaTime = current - lastFrame;
		lastFrame = current;
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(cam.zoom), (float)4 / 3, 0.1f, 100.0f);
		glm::mat4 view = cam.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		//pointLightPositions[0].y = 1.0f - abs(sin(50*glm::radians(glfwGetTime())));
		glm::vec3 lightPos = pointLightPositions[0];
		float near_plane = 1.0f;
		float far_plane = 15.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj* glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj* glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj* glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj* glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj* glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj* glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		Point_Shadow.use();
		for (unsigned int i = 0; i < 6; ++i)
			Point_Shadow.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		Point_Shadow.setFloat("far_plane", far_plane);
		Point_Shadow.setVec3("lightPos", lightPos);

		renderModel(Point_Shadow,lamp, glm::vec3(0.0f, 0.82f, 0.0f),1.0f,1);
		renderModel(Point_Shadow, chess, glm::vec3(0.9f, -0.27f, 0.08f), 1.0f,1);
		renderModel(Point_Shadow, table, glm::vec3(1.0f, -1.0f, 0.0f), 1.0f,1);
		renderModel(Point_Shadow, statue, glm::vec3(-0.5f, -0.45f, -2.0f), 3.0f, 1,0,-1);
		renderModel(Point_Shadow, table2, glm::vec3(-0.5f, -1.0f, -2.0f), 1.0f, 1);
		renderModel(Point_Shadow, duck, glm::vec3(1.5f, -0.2f, -2.0f), 1.0f, 1);
		renderModel(Point_Shadow, table1, glm::vec3(1.5f, -1.0f, -2.0f), 1.0f, 1);
		renderModel(Point_Shadow, cat, glm::vec3(1.2f, -0.2f, -2.0f), 1.0f, 1);
		renderModel(Point_Shadow, shelves, glm::vec3(2.2f, -1.0f, 1.6f), 1.5f, 1, 0, 1);
		renderModel(Point_Shadow, elephant, glm::vec3(2.2f, 0.185f, 1.6f), 3.0f, 1, 0, 1);
		renderModel(Point_Shadow, book, glm::vec3(2.18f, 0.757f, 1.01f), 2.0f, 1.2f, 0, 1);
		renderModel(Point_Shadow, plant, glm::vec3(2.2f, 0.185f, 2.12f), 1.8f, 1, 0, 1);
		renderModel(Point_Shadow, camera, glm::vec3(2.3f, 0.2f, 1.1f), 1.5f, 1, 0, 1);
		renderModel(Point_Shadow, side_table, glm::vec3(-2.0f, -1.0f, 0.0f), 1.7f, 1, 0, -1);
		renderModel(Point_Shadow, TV, glm::vec3(-1.95f, -0.08f, 0.0f), 2.0f, 1, 0, -1);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, 800, 600);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		item.use();
		for (unsigned int i = 0; i <= 4; i++) {
			glActiveTexture(GL_TEXTURE0+i);
			glBindTexture(GL_TEXTURE_2D, floor[i]);
		}
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		item.setMat4("projection", projection);
		item.setMat4("view", view);
		item.setVec3("camPos", cam.position);
		item.setVec3("lightPositions[0]", pointLightPositions[0]);
		item.setVec3("lightColors[0]", glm::vec3(10.0, 10.0, 10.0));
		//item.setMat4("lightSpaceMatrix",lightSpaceMatrix);
		item.setFloat("far_plane", far_plane);
		
		glDisable(GL_CULL_FACE); 
		for (float i = -2; i <= 2; i += 1.0f) {
			for (float j = -2; j <= 2; j += 1.0f) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i, -0.5f, j));
				item.setMat4("model", model);
				item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				RenderPlane(roomPlanes, 1, glm::vec3(0.0f, 1.0f, 0.0f));
			}
		}
		for (unsigned int i = 0; i <= 4; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, walls[i]);
		}
		for (float i = -2; i <= 2; i += 1.0f) {
			for (float j = -1; j <= 1; j += 1.0f) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i, j + 0.5f, -2.0f));
				item.setMat4("model", model);
				item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				RenderPlane(roomPlanes, 0, glm::vec3(0.0f, 0.0f, 1.0f));
			}
		}
		for (float i = -2; i <= 2; i += 1.0f) {
			for (float j = -1; j <= 1; j += 1.0f) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i, j + 0.5f, 2.0f));
				item.setMat4("model", model);
				item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				RenderPlane(roomPlanes, 5, glm::vec3(0.0f, 0.0f, -1.0f));
			}
		}
		for (float i = -2; i <= 2; i += 1.0f) {
			for (float j = -1; j <= 1; j += 1.0f) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(2.0f, j + 0.5f, i));
				item.setMat4("model", model);
				item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				RenderPlane(roomPlanes, 2, glm::vec3(-1.0f, 0.0f, 0.0f));
			}
		}

		for (float i = -2; i <= 2; i += 1.0f) {
			for (float j = -1; j <= 1; j += 1.0f) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-2.0f, j + 0.5f, i));
				item.setMat4("model", model);
				item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				RenderPlane(roomPlanes, 3, glm::vec3(1.0f, 0.0f, 0.0f));
			}
		}

		for (float i = -2; i <= 2; i += 1.0f) {
			for (float j = -2; j <= 2; j += 1.0f) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i, 1.5f, j));
				item.setMat4("model", model);
				item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				RenderPlane(roomPlanes, 4, glm::vec3(0.0f, -1.0f, 0.0f));
			}
		}
 
		glEnable(GL_CULL_FACE);
		item.setVec3("lightColors[0]", glm::vec3(50.0, 50.0, 50.0));

		renderModel(item, lamp, glm::vec3(0.0f, 0.82f, 0.0f), 1.0f, 1);
		renderModel(item, chess, glm::vec3(0.9f, -0.27f, 0.08f), 1.0f, 1);
		renderModel(item, table, glm::vec3(1.0f, -1.0f, 0.0f), 1.0f, 1);
		renderModel(item, statue, glm::vec3(-0.5f, -0.45f, -2.0f), 3.0f, 1, 0, -1);
		renderModel(item, table2, glm::vec3(-0.5f, -1.0f, -2.0f), 1.0f, 1);
		renderModel(item, duck, glm::vec3(1.5f, -0.2f, -2.0f), 1.0f, 1);
		renderModel(item, table1, glm::vec3(1.5f, -1.0f, -2.0f), 1.0f, 1);
		renderModel(item, cat, glm::vec3(1.2f, -0.2f, -2.0f), 1.0f, 1);
		renderModel(item, shelves, glm::vec3(2.2f, -1.0f, 1.6f), 1.5f, 1,0,1);
		renderModel(item, elephant, glm::vec3(2.2f, 0.185f, 1.6f), 3.0f,1,0,1);
		renderModel(item, book, glm::vec3(2.18f, 0.757f, 1.01f), 2.0f, 1.2f, 0, 1);
		renderModel(item, plant, glm::vec3(2.2f, 0.185f, 2.12f), 1.8f, 1, 0, 1);
		renderModel(item, side_table, glm::vec3(-2.0f, -1.0f, 0.0f), 1.7f, 1, 0, -1);
		for (unsigned int i = 0; i <= 4; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, steel[i]);
		}
		renderModel(item, camera, glm::vec3(2.3f, 0.2f, 1.1f), 1.5f, 1, 0, 1);
		
		renderModel(item, TV, glm::vec3(-1.95f, -0.08f, 0.0f), 2.0f, 1, 0, -1);

		myShader.use();
		myShader.setMat4("projection", projection);
		myShader.setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		for (unsigned int i = 0; i < NR_POINT_LIGHTS; i++)
		{
			myShader.setVec4("color", glm::vec4(pointLightColors[i], 1));
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.23f));
			myShader.setMat4("model", model);
			renderSphere();
		}
		glBindVertexArray(0);

		glDepthFunc(GL_LEQUAL);
		skyShader.use();
		skyShader.setMat4("view", glm::mat4(glm::mat3(cam.GetViewMatrix())));
		skyShader.setMat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox);
		renderCube();
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
