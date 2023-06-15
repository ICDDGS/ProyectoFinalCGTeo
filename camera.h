#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  0.5f;
const float SENSITIVITY =  0.7f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

	// variables camara isometrica
	bool isometric = false;
	float iso_right, iso_up;
	glm::vec3 iso_position;
	float iso_zoom = 2.0f;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		if (isometric == false) {
			return glm::lookAt(Position, Position + Front, Up);
		}
		else {
			return glm::lookAt(iso_position, iso_position + glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	void Recorrido(float xoffset) {
		Yaw = xoffset;
		updateCameraVectors();
	}

	void MovimientoAutomatico(float velocidad) {
		Position += Front * velocidad;
	}

	glm::mat4 ConfIsometric(glm::mat4 model) {
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(35.2644f), glm::vec3(0.0f, 0.0f, 1.0f));
		return model;
	}

	void setIsometric(bool iso) {
		isometric = iso;
	}

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		//glm::vec3 absFront = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), Right)); // Para que el frente no tome en cuenta el giro

		float velocity = MovementSpeed * deltaTime;
		if (!isometric) {
			if (direction == FORWARD)
				Position += Front * velocity;
			//Position += newFront * velocity;
			if (direction == BACKWARD)
				Position -= Front * velocity;
			//Position -= newFront * velocity;
			if (direction == LEFT)
				Position -= Right * velocity;
			if (direction == RIGHT)
				Position += Right * velocity;
		}
		else {
			if (direction == FORWARD) {
				iso_up += 0.5f;
				if (iso_up >= 20.0f) iso_up = 20.0f;
			}
			if (direction == BACKWARD) {
				iso_up -= 0.5f;
				if (iso_up <= -25.0f) iso_up = -25.0f;
			}
			if (direction == LEFT) {
				iso_right -= 0.5f;
				if (iso_right <= -20.0f) iso_right = -20.0f;
			}
			if (direction == RIGHT) {
				iso_right += 0.5f;
				if (iso_right >= 20.0f) iso_right = 20.0f;
			}
			iso_position = glm::vec3(iso_right, iso_up, iso_right);
		}
	}

	bool getIsometric() {
		return isometric;
	}

	float getIsoZoom() {
		return iso_zoom;
	}

	void setZoom(float zoom) {
		iso_zoom = zoom;
	}

	float getZoom() {
		return Zoom;
	}

	glm::vec3 getPosition() {
		return Position;
	}

	glm::vec3 getFront() {
		return Front;
	}

	float getYaw() {
		return Yaw;
	}
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

		if (!isometric) {
			Yaw += xoffset;
			Pitch += yoffset;
		}
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:	

    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

#endif