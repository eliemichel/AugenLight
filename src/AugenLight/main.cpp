/**
 * This file is part of Augen Light
 *
 * Copyright (c) 2017 - 2018 -- Élie Michel <elie.michel@exppad.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and non-infringement. In no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising
 * from, out of or in connection with the software or the use or other dealings
 * in the Software.
 */

#define _DEBUG

#include <glad/glad.h> // must be the first include
#include "utils/window.h" // for startup() and shutdown()
#include "utils/shader.h" // for checkShader() and checkProgram()
#include "utils/file.h" // for loadTextfile()
#include "utils/debug.h" // for enableGlDebug()
#include <cstdlib> // for EXIT_FAILURE and EXIT_SUCCESS
#include <cmath> // for sin and cos
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "TinyPngOut.hpp"

class Scene {
public:
	Scene(const std::string & vertexShaderFilename, const std::string & fragShaderFilename, int instanceCount, float endTime)
		: m_endTime(endTime)
		, m_instanceCount(static_cast<GLuint>(instanceCount))
	{
		init(vertexShaderFilename, fragShaderFilename);
	}

	~Scene() {
		finish();
	}

	void update(double time) {
		// B. UPDATE UNIFORMS

		GLint loc;

		if (!m_realtime) {
			time = m_frameIndex / m_fps;
		}

		loc = glGetUniformLocation(program, "time");
		if (loc != -1) {
			glProgramUniform1f(program, loc, static_cast<GLfloat>(time));
		}

		loc = glGetUniformLocation(m_blit_program, "prevPass");
		if (loc != -1) {
			glProgramUniform1i(m_blit_program, loc, static_cast<GLint>(0));
		}

		m_hasFinished = time > m_endTime;
	}

	void render() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glViewport(0, 0, m_fbo_width, m_fbo_height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_instanceCount);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1920, 1080);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(m_blit_program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_color_rendertexture);

		glBindVertexArray(vao);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
		
		// Save frame
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glReadPixels(0, 0, m_fbo_width, m_fbo_width, GL_RGB, GL_UNSIGNED_BYTE, static_cast<void*>(m_pixels));

		char filename[16];
		sprintf(filename, "frame%04d.png", m_frameIndex++);
		std::ofstream out(filename, std::ios::binary);
		TinyPngOut pngout(static_cast<uint32_t>(m_fbo_width), static_cast<uint32_t>(m_fbo_height), out);
		pngout.write(m_pixels, m_fbo_width * m_fbo_height);
	}

	bool hasFinished() const {
		return m_hasFinished;
	}

private:
	GLuint vbo;
	GLuint vao;
	GLuint program;
	bool m_hasFinished;
	float m_endTime;
	GLuint m_instanceCount;

	// Framebuffer
	GLuint m_fbo;
	size_t m_fbo_width;
	size_t m_fbo_height;
	GLuint m_color_rendertexture;
	GLuint m_depth_rendertexture;
	GLuint m_blit_program;

	// PNG output
	bool m_realtime;
	int m_frameIndex;
	float m_fps;
	uint8_t *m_pixels;

	void init(const std::string & vertexShaderFilename, const std::string & fragShaderFilename) {

		// 0. FRAMEBUFFER

		m_fbo_width = 2048;
		m_fbo_height = 2048;

		glCreateFramebuffers(1, &m_fbo);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_color_rendertexture);
		glTextureStorage2D(m_color_rendertexture, 1, GL_RGBA8, m_fbo_width, m_fbo_height);
		glTextureParameteri(m_color_rendertexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_color_rendertexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_color_rendertexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_color_rendertexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0, m_color_rendertexture, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_rendertexture);
		glTextureStorage2D(m_depth_rendertexture, 1, GL_DEPTH_COMPONENT32F, m_fbo_width, m_fbo_height);
		glTextureParameteri(m_depth_rendertexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_depth_rendertexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_depth_rendertexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_depth_rendertexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glNamedFramebufferTexture(m_fbo, GL_DEPTH_ATTACHMENT, m_depth_rendertexture, 0);

		m_realtime = false; // for recording set to false, for live play set to true
		m_frameIndex = 0;
		m_fps = 25;
		m_pixels = new uint8_t[3 * m_fbo_width * m_fbo_height];

		// 0.1. BLIT PROGRAM
		// To copy framebuffer to screen

		{
			const std::string vertexShaderSource = loadTextfile("shaders/blit.vert");
			const std::string fragmentShaderSource = loadTextfile("shaders/blit.frag");

			const char *shaderSource;
			m_blit_program = glCreateProgram();
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			shaderSource = vertexShaderSource.data();
			glShaderSource(vertexShader, 1, &shaderSource, NULL);
			glCompileShader(vertexShader);
			checkShader(vertexShader, "vertexShader");
			glAttachShader(m_blit_program, vertexShader);
			glDeleteShader(vertexShader);

			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			shaderSource = fragmentShaderSource.data();
			glShaderSource(fragmentShader, 1, &shaderSource, NULL);
			glCompileShader(fragmentShader);
			checkShader(fragmentShader, "fragmentShader");
			glAttachShader(m_blit_program, fragmentShader);
			glDeleteShader(fragmentShader);

			glLinkProgram(m_blit_program);
			checkProgram(m_blit_program);
		}

		// 1. VERTEX BUFFER

		const GLfloat points[] = {
			/*     a_position             a_normal     a_texcoord   */
			 -1.f,  -1.f,  0.f, 1.f,   0.f, 0.f, 1.f,   0.f, 0.f,
			  1.f,  -1.f,  0.f, 1.f,   0.f, 0.f, 1.f,   1.f, 0.f,
			 -1.f,   1.f,  0.f, 1.f,   0.f, 0.f, 1.f,   0.f, 1.f,
			  1.f,   1.f,  0.f, 1.f,   0.f, 0.f, 1.f,   1.f, 1.f,
		};

		glCreateBuffers(1, &vbo);
		glNamedBufferStorage(vbo, sizeof(points), NULL, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(vbo, 0, sizeof(points), points);

		// 2. VERTEX ARRAY

		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));

		glBindVertexArray(0);

		// 3. SHADERS

		const std::string vertexShaderSource = loadTextfile(vertexShaderFilename);
		const std::string fragmentShaderSource = loadTextfile(fragShaderFilename);

		const char *shaderSource;
		program = glCreateProgram();
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		shaderSource = vertexShaderSource.data();
		glShaderSource(vertexShader, 1, &shaderSource, NULL);
		glCompileShader(vertexShader);
		checkShader(vertexShader, "vertexShader");
		glAttachShader(program, vertexShader);
		glDeleteShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		shaderSource = fragmentShaderSource.data();
		glShaderSource(fragmentShader, 1, &shaderSource, NULL);
		glCompileShader(fragmentShader);
		checkShader(fragmentShader, "fragmentShader");
		glAttachShader(program, fragmentShader);
		glDeleteShader(fragmentShader);

		glLinkProgram(program);
		checkProgram(program);

		// 4. OTHER STUFF

		glEnable(GL_DEPTH_TEST);

		// Camera
		//glm::mat4 p = glm::perspective(60.f, 1.77f, .01f, 100.f);
		glm::mat4 p = glm::perspective(60.f, 1.f, .01f, 100.f);
		glm::mat4 v = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -3.f));
		v = glm::scale(v, glm::vec3(-10.f, -10.f, 10.f));
		glm::mat4 mvp = p * v;
		GLint loc = glGetUniformLocation(program, "mvp");
		if (loc != -1) {
			glUseProgram(program);
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mvp));
		}
	}

	void finish() {
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);

		glDeleteFramebuffers(1, &m_fbo);
		glDeleteTextures(1, &m_color_rendertexture);
		glDeleteTextures(1, &m_depth_rendertexture);
	}
};

/**
 * /!\ key codes assume that you are using a Qwerty keyboard
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Closes the application if the escape key is pressed
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}
}

std::vector<Scene*> loadProg(const std::string & filename) {
	std::vector<Scene*> scenes;
	std::string prog = loadTextfile(filename);
	std::istringstream ss(prog);
	std::string line;
	while (getline(ss, line)) {
		std::vector<std::string> tokens;
		std::string t;
		std::istringstream sss(line);
		while (getline(sss, t, '\t')) {
			tokens.push_back(t);
		}
		int instanceCount = std::atoi(tokens[2].c_str());
		float endTime = std::atof(tokens[3].c_str());
		scenes.push_back(new Scene(tokens[0], tokens[1], instanceCount, endTime));
		std::cout << "Loaded scene (" << tokens[0] << ", " << tokens[1] << ", " << instanceCount << ", " << endTime << ")" << std::endl;
	}
	return scenes;
}

int main(int argc, char *argv[]) {
	GLFWwindow *window = startup();
	if (!window) {
		return EXIT_FAILURE;
	}

	glfwSetKeyCallback(window, key_callback);

	// Load scenes
	std::vector<Scene*> scenes = loadProg("shaders/prog.csv");
	int currentScene = 0;
	double startTime = glfwGetTime();

	while (!glfwWindowShouldClose(window) && currentScene < scenes.size()) {
		scenes[currentScene]->update(glfwGetTime() - startTime);
		scenes[currentScene]->render();
		if (scenes[currentScene]->hasFinished()) {
			currentScene++;
			startTime = glfwGetTime();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Delete scenes
	for (auto it = scenes.begin() ; it != scenes.end() ; ++it) {
		delete *it;
	}

	shutdown(window);
	return EXIT_SUCCESS;
}
