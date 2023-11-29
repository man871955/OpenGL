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

	glViewport(0, 0, 800, 600);
	const GLuint SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;
	
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int texture,hdrBox;
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


		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		GLfloat near_plane = 1.0f, far_plane = 15.0f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		//pointLightPositions[0] = glm::vec3(2*sin(glm::radians(5*glfwGetTime())),2*cos(glm::radians(5*glfwGetTime())),0.0f);
		glm::vec3 lightPos = pointLightPositions[0];
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// - now render scene from light's point of view
		Shadow_shader.use();
		Shadow_shader.setMat4("lightSpaceMatrix",lightSpaceMatrix);
		Shadow_shader.setMat4("model",model);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.82f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		Shadow_shader.setMat4("model", model);
		lamp.Draw(Shadow_shader);
		glBindVertexArray(0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.1f, -0.27f, 0.08f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		Shadow_shader.setMat4("model", model);
		chess.Draw(Shadow_shader);
		glBindVertexArray(0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model,glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f));
		model = glm::scale(model, glm::vec3(1.0f));	
		Shadow_shader.setMat4("model", model);
		table.Draw(Shadow_shader);
		glBindVertexArray(0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -0.45f, -1.25f));
		model = glm::scale(model, glm::vec3(3.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		Shadow_shader.setMat4("model", model);
		statue.Draw(Shadow_shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -1.0f, -1.25f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		Shadow_shader.setMat4("model", model);
		table2.Draw(Shadow_shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -0.2f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		Shadow_shader.setMat4("model", model);
		duck.Draw(Shadow_shader);
		glBindVertexArray(0);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		Shadow_shader.setMat4("model", model);
		table1.Draw(Shadow_shader);
		glBindVertexArray(0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, 800, 600);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		item.use();
		for (unsigned int i = 0; i <= 4; i++) {
			glActiveTexture(GL_TEXTURE0+i);
			glBindTexture(GL_TEXTURE_2D, floor[i]);
		}
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		item.setMat4("projection", projection);
		item.setMat4("view", view);
		item.setVec3("camPos", cam.position);
		item.setVec3("lightPositions[0]", pointLightPositions[0]);
		item.setVec3("lightColors[0]", glm::vec3(10.0, 10.0, 10.0));
		item.setMat4("lightSpaceMatrix",lightSpaceMatrix);
		item.setFloat("far_plane", far_plane);
		
		for (float i = -2; i <= 2; i += 1.0f) {
			for (float j = -2; j <= 2; j += 1.0f) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i, 0.0f, j));
				item.setMat4("model", model);
				item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				renderPlane();
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
		item.setVec3("lightColors[0]", glm::vec3(100.0, 100.0, 100.0));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.82f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		item.setMat4("model", model);
		lamp.Draw(item);
		glBindVertexArray(0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.1f, -0.27f, 0.08f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		item.setMat4("model", model);
		chess.Draw(item);
		glBindVertexArray(0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f));	
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		item.setMat4("model", model);
		item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		table.Draw(item);
		glBindVertexArray(0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -0.45f, -1.25f));
		model = glm::scale(model, glm::vec3(3.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		item.setMat4("model", model);
		item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		statue.Draw(item);
		glBindVertexArray(0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -1.0f, -1.25f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		item.setMat4("model", model);
		item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		table2.Draw(item);
		glBindVertexArray(0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -0.2f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		item.setMat4("model", model);
		item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		duck.Draw(item);
		glBindVertexArray(0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		item.setMat4("model", model);
		item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		table1.Draw(item);
		glBindVertexArray(0);

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
