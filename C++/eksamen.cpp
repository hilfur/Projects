#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
/*
* Eksemplet simple_texturing ble brukt som utgangspunkt for denne oppgaven. Dette kan bli funnet på: https://github.com/lavima/itf21215_examples/tree/master/glfw/simple_texturing
* Fps_test.cpp har også blitt brukt som inspirasjon/guide/hjelp for å denne oppgaven til. Hvilke deler som har blitt brukt står forklart i rapporten. 
*
*
*/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

// Vertex Buffer Identifiers
#define VERTICES 0
#define INDICES 1
#define GLOBAL_MATRICES 2
#define MODEL_MATRIX1 3
#define MODEL_MATRIX2 4
#define MODEL_MATRIX3 5
#define MODEL_MATRIX4 6
#define MODEL_MATRIX5 7
#define MODEL_MATRIX6 8

// Vertex Array attributes
#define POSITION 0
#define UV 1

// Vertex Array binding points
#define STREAM0 0

// GLSL Uniform indices
#define TRANSFORM0 0
#define TRANSFORM1 1

#define CAMERA_SPEED 5.0f
#define JUMPING_SPEED 10.0f 
#define CAMERA_SENSITIVITY 0.005


// Keyboard state
bool move[6];
int injump = 0; 
int jumpactive = 1;
int godown = 0; 

// Vertices
GLfloat vertices[] = {
	// Front
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	// Back
	1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
	// Left
	-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	// Right
	1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
	// Top
	-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
	// Bottom
	-1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 1.0f,  1.0f, 1.0f
};


// Indices
GLshort indices[]{
	// Front
	0, 1, 2, 2, 3, 0,
	// Back
	4, 5, 6, 6, 7, 4,
	// Left
	8, 9, 10, 10, 11, 8,
	// Right
	12, 13, 14, 14, 15, 12,
	// Top
	16, 17, 18, 18, 19, 16,
	// Bottom
	20, 21, 22, 22, 23, 20
};


GLfloat pitch, roll;
GLfloat yaw = 0.0f;

// Pointers for updating GPU data
GLfloat *projectionMatrixPtr;
GLfloat *viewMatrixPtr;
GLfloat *modelMatrix1Ptr;
GLfloat *modelMatrix2Ptr;
GLfloat *modelMatrix3Ptr;
GLfloat *modelMatrix4Ptr;
GLfloat *modelMatrix5Ptr;
GLfloat *modelMatrix6Ptr;
// Names
GLuint programName;
GLuint vertexArrayName;
GLuint vertexBufferNames[10];
GLuint textureName1;
GLuint textureName2; 
GLuint textureName3;
double timeDelta;
double previousTime;

glm::vec3 cameraPos;
glm::vec3 cameraForward;
glm::vec3 cameraRight;
glm::vec3 cameraup; 

int centerX, centerY;

/*
* Read shader source file from disk
*/
char *readSourceFile(const char *filename, int *size) {

	// Open the file as read only
	FILE *file = fopen(filename, "r");

	// Find the end of the file to determine the file size
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);

	// Rewind
	fseek(file, 0, SEEK_SET);

	// Allocate memory for the source and initialize it to 0
	char *source = (char *)malloc(fileSize + 1);
	for (int i = 0; i <= fileSize; i++) source[i] = 0;

	// Read the source
	fread(source, fileSize, 1, file);

	// Close the file
	fclose(file);

	// Store the size of the file in the output variable
	*size = fileSize - 1;

	// Return the shader source
	return source;

}

/*
* Callback function for OpenGL debug messages
*/
void glDebugCallback(GLenum sources, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *userParam) {
	printf("DEBUG: %s\n", msg);
}

/*
* Initialize OpenGL
*/
int initGL() {

	// Register the debug callback function
	glDebugMessageCallback(glDebugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	// Create and initialize 4 buffer names
	glCreateBuffers(10, vertexBufferNames);
	// Allocate storage for the vertex array buffers
	glNamedBufferStorage(vertexBufferNames[VERTICES], 6 * 4 * 5 * sizeof(GLfloat), vertices, 0);

	// Allocate storage for the triangle indices
	glNamedBufferStorage(vertexBufferNames[INDICES], 6 * 2 * 3 * sizeof(GLshort), indices, 0);

	// Allocate storage for the transformation matrices and retrieve their addresses
	glNamedBufferStorage(vertexBufferNames[GLOBAL_MATRICES], 16 * sizeof(GLfloat) * 2, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glNamedBufferStorage(vertexBufferNames[MODEL_MATRIX1], 16 * sizeof(GLfloat), NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glNamedBufferStorage(vertexBufferNames[MODEL_MATRIX2], 16 * sizeof(GLfloat), NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glNamedBufferStorage(vertexBufferNames[MODEL_MATRIX3], 16 * sizeof(GLfloat), NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glNamedBufferStorage(vertexBufferNames[MODEL_MATRIX4], 16 * sizeof(GLfloat), NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glNamedBufferStorage(vertexBufferNames[MODEL_MATRIX5], 16 * sizeof(GLfloat), NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	glNamedBufferStorage(vertexBufferNames[MODEL_MATRIX6], 16 * sizeof(GLfloat), NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

	// Get a pointer to the global matrices data
	GLfloat *globalMatricesPtr = (GLfloat *)glMapNamedBufferRange(vertexBufferNames[GLOBAL_MATRICES], 0, 16 * sizeof(GLfloat) * 2,
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	projectionMatrixPtr = globalMatricesPtr;
	viewMatrixPtr = globalMatricesPtr + 16;

	// Get a pointer to the model matrix data
	modelMatrix1Ptr = (GLfloat *)glMapNamedBufferRange(vertexBufferNames[MODEL_MATRIX1], 0, 16 * sizeof(GLfloat),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	modelMatrix2Ptr = (GLfloat *)glMapNamedBufferRange(vertexBufferNames[MODEL_MATRIX2], 0, 16 * sizeof(GLfloat),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	modelMatrix3Ptr = (GLfloat *)glMapNamedBufferRange(vertexBufferNames[MODEL_MATRIX3], 0, 16 * sizeof(GLfloat),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	modelMatrix4Ptr = (GLfloat *)glMapNamedBufferRange(vertexBufferNames[MODEL_MATRIX4], 0, 16 * sizeof(GLfloat),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	modelMatrix5Ptr = (GLfloat *)glMapNamedBufferRange(vertexBufferNames[MODEL_MATRIX5], 0, 16 * sizeof(GLfloat),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	modelMatrix6Ptr = (GLfloat *)glMapNamedBufferRange(vertexBufferNames[MODEL_MATRIX6], 0, 16 * sizeof(GLfloat),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);


	// Create and initialize a vertex array object
	glCreateVertexArrays(1, &vertexArrayName);

	// Associate attributes with binding points
	glVertexArrayAttribBinding(vertexArrayName, VERTICES, STREAM0);
	glVertexArrayAttribBinding(vertexArrayName, UV, STREAM0);

	// Specify attribute format
	glVertexArrayAttribFormat(vertexArrayName, POSITION, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vertexArrayName, UV, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT));

	// Enable the attributes
	glEnableVertexArrayAttrib(vertexArrayName, POSITION);
	glEnableVertexArrayAttrib(vertexArrayName, UV);

	// Bind the indices to the vertex array
	glVertexArrayElementBuffer(vertexArrayName, vertexBufferNames[INDICES]);

	// Bind the buffers to the vertex array
	glVertexArrayVertexBuffer(vertexArrayName, STREAM0, vertexBufferNames[POSITION], 0, 5 * sizeof(GLfloat));

	// Load image from file


	// Generate texture name
	glGenTextures(2, &textureName1);
	glGenTextures(2, &textureName2);
	glGenTextures(2, &textureName3);

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureName1);
	GLint width, height, numChannels;
	GLubyte *imageData1 = stbi_load("woodbox.png", &width, &height, &numChannels, 3);

	// Specify the format of the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData1);

	// Set the sampler parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate mip maps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Deactivate texture
	glBindTexture(GL_TEXTURE_2D, 0);
	/*
	*
	*
	* TEST
	*
	*
	*/
	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureName2);
	GLubyte *imageData2 = stbi_load("grass.png", &width, &height, &numChannels, 3);

	// Specify the format of the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData2);

	// Set the sampler parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate mip maps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Deactivate texture
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, textureName3);
	GLubyte *imageData3 = stbi_load("material.png", &width, &height, &numChannels, 3);

	// Specify the format of the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData3);

	// Set the sampler parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate mip maps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Deactivate texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Load and compile vertex shader
	GLuint vertexName = glCreateShader(GL_VERTEX_SHADER);
	int vertexLength = 0;
	char *vertexSource = readSourceFile("eksamen.vert", &vertexLength);
	glShaderSource(vertexName, 1, (const char * const *)&vertexSource, &vertexLength);
	GLint compileStatus;
	glCompileShader(vertexName);
	glGetShaderiv(vertexName, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		GLint logSize = 0;
		glGetShaderiv(vertexName, GL_INFO_LOG_LENGTH, &logSize);
		char *errorLog = (char *)malloc(sizeof(char) * logSize);
		glGetShaderInfoLog(vertexName, logSize, &logSize, errorLog);
		glDeleteShader(vertexName);
		printf("VERTEX ERROR %s\n", errorLog);
		return 0;
	}
	free(vertexSource);

	// Load and compile fragment shader
	GLuint fragmentName = glCreateShader(GL_FRAGMENT_SHADER);
	int fragmentLength = 0;
	char *fragmentSource = readSourceFile("eksamen.frag", &fragmentLength);
	glShaderSource(fragmentName, 1, (const char * const *)&fragmentSource, &fragmentLength);
	glCompileShader(fragmentName);
	glGetShaderiv(fragmentName, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		GLint logSize = 0;
		glGetShaderiv(fragmentName, GL_INFO_LOG_LENGTH, &logSize);
		char *errorLog = (char *)malloc(sizeof(char) * logSize);
		glGetShaderInfoLog(fragmentName, logSize, &logSize, errorLog);
		glDeleteShader(fragmentName);

		printf("FRAGMENT ERROR %s\n", errorLog);
		return 0;
	}
	free(fragmentSource);

	// Create and link vertex program
	programName = glCreateProgram();
	glAttachShader(programName, vertexName);
	glAttachShader(programName, fragmentName);
	glLinkProgram(programName);
	GLint linkStatus;
	glGetProgramiv(programName, GL_LINK_STATUS, &linkStatus);
if (!linkStatus) {
	GLint logSize = 0;
	glGetProgramiv(programName, GL_INFO_LOG_LENGTH, &logSize);
	char *errorLog = (char *)malloc(sizeof(char) * logSize);
	glGetProgramInfoLog(programName, logSize, &logSize, errorLog);

	printf("LINK ERROR %s\n", errorLog);
	return 0;
}

// Enable Opengl depth buffer testing
glEnable(GL_DEPTH_TEST);
glEnable(GL_TEXTURE_CUBE_MAP); 
glDepthFunc(GL_LESS);
return 1;

}

/*
* Draw OpenGL screne
*/
void drawGLScene() {

	// Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	if (move [0] == 1) {
		if (cameraPos[0] > 18) {
			cameraPos[0] = 17.9;
		}
		else if (cameraPos[0] < -18) {
			cameraPos[0] = -17.9;
		}
		else if (cameraPos[2] > 18) {
			cameraPos[2] = 17.9;
		}
		else if (cameraPos[2] < -18) {
			cameraPos[2] = -17.9;
		}
		cameraPos[0] -= cameraForward[0] * (float)(timeDelta * CAMERA_SPEED);
		cameraPos[2] -= cameraForward[2] * (float)(timeDelta * CAMERA_SPEED);
	}
	else if (move[1] == 1) {
		if (cameraPos[0] > 18) {
			cameraPos[0] = 17.9;
		}
		else if (cameraPos[0] < -18) {
			cameraPos[0] = -17.9;
		}
		else if (cameraPos[2] > 18) {
			cameraPos[2] = 17.9;
		}
		else if (cameraPos[2] < -18) {
			cameraPos[2] = -17.9;
		}
		cameraPos[0] -= cameraRight[0] * (float)(timeDelta * CAMERA_SPEED);
		cameraPos[2] -= cameraRight[2] * (float)(timeDelta * CAMERA_SPEED);
	}
	else if (move[2] == 1) {
		if (cameraPos[0] > 18) {
			cameraPos[0] = 17.9;
		}
		else if (cameraPos[0] < -18) {
			cameraPos[0] = -17.9;
		}
		else if (cameraPos[2] > 18) {
			cameraPos[2] = 17.9;
		}
		else if (cameraPos[2] < -18) {
			cameraPos[2] = -17.9;
		}
		cameraPos[0] += cameraForward[0] * (float)(timeDelta * CAMERA_SPEED);
		cameraPos[2] += cameraForward[2] * (float)(timeDelta * CAMERA_SPEED);
	}
	else if (move[3] == 1) {
		if (cameraPos[0] > 18) {
			cameraPos[0] = 17.9;
		}
		else if (cameraPos[0] < -18) {
			cameraPos[0] = -17.9;
		}
		else if (cameraPos[2] > 18) {
			cameraPos[2] = 17.9;
		}
		else if (cameraPos[2] < -18) {
			cameraPos[2] = -17.9;
		}
		cameraPos[0] += cameraRight[0] * (float)(timeDelta * CAMERA_SPEED);
		cameraPos[2] += cameraRight[2] * (float)(timeDelta * CAMERA_SPEED);
	}
	if (move[4] == 1) {
		roll = -0.4;
	}
	else if (move[5] == 1) {
		roll = 0.4;
	}
	else
		roll = 0;
	
	if (injump == 1) {

		if (cameraPos[1] <= 2.0f && jumpactive == 1) {
			cameraPos[1] += cameraup[1] * (float)(timeDelta * JUMPING_SPEED);
		}
		else if (cameraPos[1] >= 2.0f) {
			jumpactive = 0; 
			godown = 1; 
		}
		if (godown == 1) {
			cameraPos[1] -= cameraup[1] * (float)(timeDelta * JUMPING_SPEED);
		}
		if (cameraPos[1] <= 0 && godown == 1) {
			godown = 0; 
			injump = 0; 
			jumpactive = 1; 
			cameraPos[1] = 0; 
		}
		
	}
	else if (injump == 0) {
	}
	

	// Change the view matrix
	glm::mat4 viewRot = glm::mat4(1.0f);
	viewRot = glm::rotate(viewRot, yaw * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	viewRot = glm::rotate(viewRot, pitch * 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	viewRot = glm::rotate(viewRot, roll * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	cameraForward = viewRot * glm::vec4(0.0f, 0.0f, 1.0f, 1);
	cameraRight = viewRot * glm::vec4(1.0f, 0.0f, 0.0f, 1);
	glm::mat4 viewTrans = glm::mat4(1.0f);
	viewTrans = glm::translate(viewTrans, cameraPos);
	glm::mat4 view = viewTrans * viewRot;
	glm::mat4 viewInverse = glm::inverse(view);
	memcpy(viewMatrixPtr, &viewInverse[0][0], 16 * sizeof(GLfloat));

	// Change the model matrix
	glm::mat4 model1 = glm::mat4(1.0);
	model1 = glm::translate(model1, glm::vec3(1.5f, -1.0f, 0.0f));
	memcpy(modelMatrix1Ptr, &model1[0][0], 16 * sizeof(GLfloat)); 

	glm::mat4 model2 = glm::mat4(1.0);
	model2 = glm::translate(model2, glm::vec3(-1.5f, -1.0f, 0.0f));
	memcpy(modelMatrix2Ptr, &model2[0][0], 16 * sizeof(GLfloat));

	glm::mat4 model3 = glm::mat4(1.0);
	model3 = glm::translate(model3, glm::vec3(0.0f, 1.0f, 0.0f));
	memcpy(modelMatrix3Ptr, &model3[0][0], 16 * sizeof(GLfloat));

	 glm::mat4 model4 = glm::mat4(1.0);
	model4 = glm::translate(model4, glm::vec3(0.0f, -2.0f, 0.0f));
	model4 = glm::scale(model4, glm::vec3(20.0f, 0.1f, 20.0f));
	memcpy(modelMatrix4Ptr, &model4[0][0], 16 * sizeof(GLfloat)); 

	glm::mat4 model5 = glm::mat4(1.0);
	model5 = glm::rotate(model5, (float)glfwGetTime() * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model5 = glm::translate(model5, glm::vec3(-2.0f, 1.5f, 0.0f));
	model5 = glm::scale(model5, glm::vec3(0.5f, 0.5f, 0.5f));
	memcpy(modelMatrix5Ptr, &model5[0][0], 16 * sizeof(GLfloat));

	glm::mat4 model6 = glm::mat4(1.0);
	model6 = glm::rotate(model6, (float)glfwGetTime() * -1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model6 = glm::translate(model6, glm::vec3(-2.0f, 1.5f, 0.0f));
	model6 = glm::scale(model6, glm::vec3(0.5f, 0.5f, 0.5f));
	memcpy(modelMatrix6Ptr, &model6[0][0], 16 * sizeof(GLfloat));



	// Activate the program, vertex array and texture
	glUseProgram(programName);
	glBindVertexArray(vertexArrayName);
	glBindTexture(GL_TEXTURE_2D, textureName1);
	// Bind tranformation buffers to uniform indices
	glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM0, vertexBufferNames[GLOBAL_MATRICES]);
	glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM1, vertexBufferNames[MODEL_MATRIX1]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM1, vertexBufferNames[MODEL_MATRIX2]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM1, vertexBufferNames[MODEL_MATRIX3]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, textureName2);
	// Draw the vertex array

	glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM1, vertexBufferNames[MODEL_MATRIX4]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, textureName3);
	glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM1, vertexBufferNames[MODEL_MATRIX5]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM1, vertexBufferNames[MODEL_MATRIX6]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);



	// Disable
	glUseProgram(0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void resizeGL(GLFWwindow *window, int width, int height) {
	pitch = 0.0f;
	// Prevent division by zero
	if (height == 0)
		height = 1;

	// Change the projection matrix
	glm::mat4 proj = glm::perspective(3.14f / 2.0f, (float)width / height, 0.1f, 100.0f);
	memcpy(projectionMatrixPtr, &proj[0][0], 16 * sizeof(GLfloat));

	// Set the OpenGL viewport
	glViewport(0, 0, width, height);


}

/*
* Error callback function for GLFW
*/
static void glfwErrorCallback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}


void mouse_callback(GLFWwindow *window, double x, double y) {
	centerX = 1024 / 2;
	centerY = 768 / 2;
	yaw = -(x - centerX) * CAMERA_SENSITIVITY;
	pitch = -(y - centerY) * CAMERA_SENSITIVITY;


}
/*
* Input event callback function for GLFW
*/
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << cameraPos[0] << " x \n";
	std::cout << cameraPos[2] << "y \n "; 
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	injump = 1;

	bool active = false;
	if (action == GLFW_PRESS)
		active = true;
	else if (action == GLFW_RELEASE)
		active = false;
	else
		return;


	if (key == GLFW_KEY_W) {
		move[0] = active;
	}
	else if (key == GLFW_KEY_A)
		move[1] = active;

	else if (key == GLFW_KEY_S)
		move[2] = active;
	else if (key == GLFW_KEY_D)
		move[3] = active;
	else if (key == GLFW_KEY_E) {
		move[4] = active;
	}
	else if (key == GLFW_KEY_Q)
		move[5] = active;

		
}



/*
* Window size changed callback function for GLFW
*/
void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {

	resizeGL(window, width, height);

	centerX = width / 2;
	centerY = height / 2;

}

/*
* Program entry function
*/
int main(void) {
	
	// Set error callback
	glfwSetErrorCallback(glfwErrorCallback);

	// Initialize GLFW
	if (!glfwInit()) {
		printf("Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Specify minimum OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);


	// Create window
	GLFWwindow* window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Minimal", NULL, NULL);
	if (!window) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Set input callback functions
	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, glfwMouseCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set window resize callback
	glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);

	// Make the context current
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make GLFW swap buffers directly 
	glfwSwapInterval(0);


	// Initialize OpenGL
	if (!initGL()) {
		printf("Failed to initialize OpenGL\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Initialize OpenGL view
	resizeGL(window, DEFAULT_WIDTH, DEFAULT_HEIGHT);

	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
	cameraup = glm::vec3(0.0f, 1.0f, 0.0f); 

	previousTime = glfwGetTime();



	// Run a loop until the window is closed
	while (!glfwWindowShouldClose(window)) {

		double timeTotal = glfwGetTime();
		timeDelta = timeTotal - previousTime;
		previousTime = timeTotal;

		// Draw OpenGL screne
		drawGLScene();

		// Swap buffers
		glfwSwapBuffers(window);

		// Poll fow input events
		glfwPollEvents();

	}

	// Shutdown GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	// Exit
	exit(EXIT_SUCCESS);

}
