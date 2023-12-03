#include "src/sources.h"

using namespace std;

int  main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif	
	GLFWwindow* window = glfwCreateWindow(800, 600, "CG Project", NULL, NULL);
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
	Shader myShader = Shader("VertexShaders/Shader.vs", "FragmentShaders/Shader.fs");
	Shader skyShader = Shader("VertexShaders/skyBox.vs", "FragmentShaders/skyBox.fs");
	Shader Point_Shadow("VertexShaders/PointShadow.vs", "FragmentShaders/PointShadow.fs", "VertexShaders/PointShadow.gs");
	Shader item("VertexShaders/item.vs", "FragmentShaders/item.fs");
	Shader animator_shader("VertexShaders/animator.vs", "FragmentShaders/animator.fs");
	Model table("model/table/outdoor_table_chair_set_01_1k.fbx");
	Model chess("model/chess/chess_set_1k.fbx");
	Model table1("model/table1/wooden_table_02_1k.fbx");
	Model duck("model/duck/rubber_duck_toy_1k.fbx");
	Model table2("model/table2/modern_coffee_table_01_1k.fbx");
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
	Model picture("model/picture/fancy_picture_frame_01_1k.fbx");
	Model picture2("model/picture2/fancy_picture_frame_02_1k.fbx");
	Model cart("model/cart/CoffeeCart_01_1k.fbx");
	Model boombox("model/boombox/boombox_1k.fbx");
	Model win("model/window/030.3dcool.net.obj");
	Model door("model/door/Door.obj");
	Model_animation wolf("model/animator/wolf/Wolf.dae");
	Animation danceAnimation("model/animator/wolf/Wolf.dae", &wolf);
	Animator animator(&danceAnimation);
	Model_animation gun("model/animator/gun/Gun.dae");
	Animation gunAnimation("model/animator/gun/Gun.dae", &gun);
	Animator animator1(&gunAnimation);

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
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int texture,wolf_skin;
	string path;
	path = "pictures/1.png";
	texture = loadTexture(path.c_str());
	path = "pictures/Wolf_Body.jpg";
	wolf_skin = loadTexture(path.c_str());

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
	vector<unsigned int> wood;
	BindPBRTextures(wood, "wood");
	vector<unsigned int> metal;
	BindPBRTextures(metal, "metal");
	vector<unsigned int> plastic;
	BindPBRTextures(plastic, "plastic");
	vector<unsigned int> wood_table;
	BindPBRTextures(wood_table, "table");
	vector<unsigned int> rock;
	BindPBRTextures(rock, "rock");
	vector<unsigned int> wood_door;
	BindPBRTextures(wood_door, "door");
	
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
		animator.UpdateAnimation(deltaTime);
		animator1.UpdateAnimation(deltaTime);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(cam.zoom), (float)4 / 3, 0.1f, 100.0f);
		glm::mat4 view = cam.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 lightPos = pointLightPositions[L];
		float near_plane = 1.0f -0.95f * L;
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
		Point_Shadow.setBool("animate",false);

		renderModel(Point_Shadow,lamp, glm::vec3(0.0f, 0.82f, 0.0f),1.0f,1);
		renderModel(Point_Shadow, chess, glm::vec3(-0.1f, -0.27f, 0.08f), 1.0f,1);
		renderModel(Point_Shadow, table, glm::vec3(0.0f, -1.0f, 0.0f), 1.0f,1);
		renderModel(Point_Shadow, side_table, glm::vec3(-2.0f, -0.98f, 1.9f), 2.0f, 1, 0, 1);

		renderModel(Point_Shadow, table1, glm::vec3(2.1f, -1.0f, -1.8f), 1.0f, 1,0,1);
		renderModel(Point_Shadow, plant, glm::vec3(2.1f, -0.2f, -1.8f), 1.8f, 1, 0, 1);

		renderModel(Point_Shadow, shelves, glm::vec3(2.2f, -1.0f, 1.6f), 1.5f, 1, 0, 1);
		renderModel(Point_Shadow, statue, glm::vec3(2.2f, 0.185f, 2.12f), 2.0f, 1, 0, 0.5);
		renderModel(Point_Shadow, elephant, glm::vec3(2.2f, 0.185f, 1.6f), 3.0f, 1, 0, 1);
		renderModel(Point_Shadow, book, glm::vec3(2.17f, 0.757f, 1.01f), 2.0f, 1.2f, 0, 1);
		renderModel(Point_Shadow, cat, glm::vec3(2.2f, -0.38f, 1.6f), 1.5f, 1, 0, 1);
		renderModel(Point_Shadow, duck, glm::vec3(2.2f, 0.75f, 1.6f), 1.0f, 1, 0, 1);

		renderModel(Point_Shadow, table2, glm::vec3(-2.0f, -1.0f, 0.0f), 1.7f, 1, 0, -1);
		renderModel(Point_Shadow, picture, glm::vec3(0.75f, 1.0f, 2.5f), 1.0f, 1, 0, -2);
		renderModel(Point_Shadow, picture2, glm::vec3(-2.5f, 1.25f, 1.0f), 1.0f, 1, 0, -1);
		renderModel(Point_Shadow, camera, glm::vec3(2.3f, 0.2f, 1.1f), 1.5f, 1, 0, 1);
		renderModel(Point_Shadow, TV, glm::vec3(-1.95f, -0.35f, 0.0f), 2.0f, 1, 0, -1);
		renderModel(Point_Shadow, cart, glm::vec3(-1.1f, -1.0f, -1.945f), 1.0f, 1);
		renderModel(Point_Shadow, boombox, glm::vec3(-2.05f, 0.12f, 1.9f), 1.0f, 1, 0, -1);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.92f, 0.035f, -0.46f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02737f, 0.022f, 0.02737f));
		Point_Shadow.setMat4("model", model);
		win.Draw(Point_Shadow);
		glBindVertexArray(0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.6f, -0.43f, -2.48f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.06f, 0.08f));
		Point_Shadow.setMat4("model", model);
		door.Draw(Point_Shadow);
		glBindVertexArray(0);

		Point_Shadow.setBool("animate",true);
		auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			Point_Shadow.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
		renderModel(Point_Shadow, wolf, glm::vec3(-1.0f,-1.0f,2.0f), 1.5f, 0, 2.5, 0);

		auto transforms1 = animator1.GetFinalBoneMatrices();
		for (int i = 0; i < transforms1.size(); ++i)
			Point_Shadow.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms1[i]);
		renderModel(Point_Shadow, gun, glm::vec3(-0.5f, 1.0f, 2.46f), 1.0f, 0, 0, 0.25);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, 800, 600);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		item.use();
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		item.setMat4("projection", projection);
		item.setMat4("view", view);
		item.setVec3("camPos", cam.position);
		item.setVec3("lightPositions[0]", pointLightPositions[L]);
		item.setVec3("lightColors[0]", glm::vec3(10.0, 10.0, 10.0));
		item.setFloat("far_plane", far_plane);
		item.setBool("animate", false);

		glDisable(GL_CULL_FACE); 
		for (unsigned int i = 0; i <= 4; i++) {
			glActiveTexture(GL_TEXTURE0+i);
			glBindTexture(GL_TEXTURE_2D, floor[i]);
		}
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
			glBindTexture(GL_TEXTURE_2D, wood[i]);
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
		for (float j = -1; j <= 1; j += 1.0f) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(2.0f, j + 0.5f, -2.0f));
			item.setMat4("model", model);
			item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
			RenderPlane(roomPlanes, 0, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		for (float i = -2.0f; i <= -1.0f; i += 1.0f) {
			for (float j = -1; j <= 1; j += 1.0f) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i, j + 0.5f, -2.0f));
				item.setMat4("model", model);
				item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				RenderPlane(roomPlanes, 0, glm::vec3(0.0f, 0.0f, 1.0f));
			}
		}
		for (float i = -1; i <= 1; i += 1.0f) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(i, 1.5f, -2.0f));
			item.setMat4("model", model);
			item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
			RenderPlane(roomPlanes, 0, glm::vec3(0.0f, 0.0f, 1.0f));
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
		for (float i = 1.0; i <= 2; i += 1.0f) {
			for (float j = -1; j <= 1; j += 1.0f) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(2.0f, j + 0.5f, i));
				item.setMat4("model", model);
				item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				RenderPlane(roomPlanes, 2, glm::vec3(-1.0f, 0.0f, 0.0f));
			}
		}
		for (float i = -1.0f; i <= 0.0f; i += 1.0f) {
			for (float j = -0.5f; j <= 2.0f; j += 2.0f) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(2.0f, j, i));
				item.setMat4("model", model);
				item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				RenderPlane(roomPlanes, 2, glm::vec3(-1.0f, 0.0f, 0.0f));
			}
		}
		for (float j = -1; j <= 1; j += 1.0f) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(2.0f, j + 0.5f, -2.0f));
			item.setMat4("model", model);
			item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
			RenderPlane(roomPlanes, 2, glm::vec3(-1.0f, 0.0f, 0.0f));
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

		glEnable(GL_CULL_FACE);
		item.setVec3("lightColors[0]", glm::vec3(50.0, 50.0, 50.0));
		renderModel(item, chess, glm::vec3(-0.1f, -0.27f, 0.08f), 1.0f, 1);
		renderModel(item, elephant, glm::vec3(2.2f, 0.185f, 1.6f), 3.0f,1,0,1);
		renderModel(item, shelves, glm::vec3(2.2f, -1.0f, 1.6f), 1.5f, 1,0,1);
		renderModel(item, picture, glm::vec3(0.75f, 1.0f, 2.5f), 1.0f, 1, 0, -2);
		renderModel(item, picture2, glm::vec3(-2.5f,1.25f,1.0f), 1.0f, 1, 0, -1);
		renderModel(item, book, glm::vec3(2.17f, 0.757f, 1.01f), 2.0f, 1.2f, 0, 1);
		for (unsigned int i = 0; i <= 4; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, wood_table[i]);
		}
		renderModel(item, table, glm::vec3(0.0f, -1.0f, 0.0f), 1.0f, 1);
		renderModel(item, side_table, glm::vec3(-2.0f, -0.98f, 1.9f), 2.0f, 1,0,1);
		renderModel(item, table1, glm::vec3(2.1f, -1.0f, -1.8f), 1.0f, 1,0,1);
		renderModel(item, table2, glm::vec3(-2.0f, -1.0f, 0.0f), 1.7f, 1, 0, -1);
		renderModel(item, cart, glm::vec3(-1.1f, -1.0f, -1.945f), 1.0f, 1);

		for (unsigned int i = 0; i <= 4; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, rock[i]);
		}
		renderModel(item, plant, glm::vec3(2.1f, -0.2f, -1.8f), 1.8f, 1, 0, 1);
		renderModel(item, statue, glm::vec3(2.2f, 0.185f, 2.12f), 2.0f, 1, 0, 0.5);
		renderModel(item, cat, glm::vec3(2.2f, -0.38f, 1.6f), 1.5f, 1,0,1);

		for (unsigned int i = 0; i <= 4; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, plastic[i]);
		}
		renderModel(item, duck, glm::vec3(2.2f, 0.75f, 1.6f), 1.0f, 1,0,1);
		
		for (unsigned int i = 0; i <= 4; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, metal[i]);
		}
		renderModel(item, camera, glm::vec3(2.3f, 0.2f, 1.1f), 1.5f, 1, 0, 1);
		renderModel(item, lamp, glm::vec3(0.0f, 0.82f, 0.0f), 1.0f, 1);
		renderModel(item, boombox, glm::vec3(-2.05f, 0.12f, 1.9f), 1.0f, 1, 0, -1);
		renderModel(item, TV, glm::vec3(-1.95f, -0.35f, 0.0f), 2.0f, 1, 0, -1);

		for (unsigned int i = 0; i <= 4; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, metal[i]);
		}
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.92f, 0.035f, -0.46f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02737f, 0.022f, 0.02737f));
		item.setMat4("model", model);
		item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		win.Draw(item);
		glBindVertexArray(0); 

		glDisable(GL_CULL_FACE);
		for (unsigned int i = 0; i <= 4; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, wood_door[i]);
		}
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.6f, -0.43f, -2.48f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.06f, 0.08f));
		item.setMat4("model", model);
		item.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		door.Draw(item);
		glBindVertexArray(0);
	
		item.setBool("animate", true);
		for (int i = 0; i < transforms1.size(); ++i)
			item.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms1[i]);
		renderModel(item, gun, glm::vec3(-0.5f, 1.0f, 2.46f), 1.0f, 0, 0, 0.25);
		glBindVertexArray(0);
		
		animator_shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,wolf_skin);
		animator_shader.setMat4("projection", projection);
		animator_shader.setMat4("view", view);
		animator_shader.setVec4("lightColor",glm::vec4(glm::vec3(1.0f - 0.5*L), 1.0f));
		for (int i = 0; i < transforms.size(); ++i)
			animator_shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
		renderModel(animator_shader, wolf, glm::vec3(-1.0f, -1.0f, 2.0f), 1.5f, 0, 2.5);
		glBindVertexArray(0);
		
		myShader.use();
		myShader.setMat4("projection", projection);
		myShader.setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		myShader.setVec4("color", glm::vec4(1.0,1.0,1.0, 1));
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[L]);
		model = glm::scale(model, glm::vec3(lightScale[L]));
		myShader.setMat4("model", model);
		renderSphere();
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
