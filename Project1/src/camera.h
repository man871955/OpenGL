#ifndef  CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum Camera_Movement {
	FORWARD,BACKWARD,LEFT,RIGHT, UP,DOWN,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const int limits[6] = { 2.0f,2.0f,2.0f,2.0f,2.0f,2.0f };
class camera {
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldup;
	float yaw;
	float pitch;
	float speed;
	float sensitivity;
	float zoom;
	camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f), 
		float y = YAW, float pit = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), 
		speed(SPEED), sensitivity(SENSITIVITY) {
		position = pos;
		worldup = u;
		yaw = y;
		pitch = pit;
		update();
	}
    camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float y, float pit) : front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM)
    {
        position = glm::vec3(posX, posY, posZ);
        worldup = glm::vec3(upX, upY, upZ);
        yaw = y;
        pitch = pit;
        update();
    }
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = speed * deltaTime;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
        if (direction == UP)
            position += up * velocity;
        if (direction == DOWN)
            position -= up * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;
        if (constrainPitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }
        update();
    }

    void ProcessMouseScroll(float yoffset)
    {
        zoom -= (float)yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }
private:
	void update() {
        glm::vec3 fr;
        fr.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        fr.y = sin(glm::radians(pitch));
        fr.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(fr);
        right = glm::normalize(glm::cross(front, worldup));
        up = glm::normalize(glm::cross(right,front));
	}
};

#endif // ! CAMERA_H
