/**
 * This file is part of Augen Light
 *
 * Copyright (c) 2017 - 2019 -- Élie Michel <elie.michel@exppad.com>
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

#include <glad/glad.h> // must be the first include
#include <cstdlib> // for EXIT_FAILURE and EXIT_SUCCESS
#include <cmath> // for sin and cos

#include "Viewer.h"
#include "utils/shader.h" // for checkShader() and checkProgram()
#include "utils/debug.h" // for enableGlDebug()

/**
 * This file is basically the main.cpp of the level1 branch, so it is largely
 * left uncommented because the reader is invitet to consult the level0 and 
 * level1 branches for details.
 */

Viewer::Viewer() {
#ifndef NDEBUG
	enableGlDebug();
#endif // !NDEBUG

	const GLfloat points[] = {
		 0.f,  .5f,  0.f,   1.f, 0.f, 0.f,
		 .5f, -.5f,  0.f,   0.f, 1.f, 0.f,
		-.5f, -.5f,  0.f,   0.f, 0.f, 1.f,
	};

	glCreateBuffers(1, &m_vbo);
	glNamedBufferStorage(m_vbo, sizeof(points), NULL, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(m_vbo, 0, sizeof(points), points);

	glCreateVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);

	const char *vertexShaderSource =
		"#version 450\n"
		"layout(location=0) in vec3 vPosition;\n"
		"layout(location=1) in vec3 vColor;\n"
		"out vec3 fColor;\n"
		"void main() {\n"
		"    gl_Position = vec4(vPosition, 1.0);\n"
		"    fColor = vColor;\n"
		"}\n";

	const char *fragmentShaderSource =
		"#version 450\n"
		"in vec3 fColor;\n"
		"out vec4 color;\n"
		"uniform float uTime;"
		"void main() {\n"
		"    float s = sin(uTime);\n"
		"    color = vec4(fColor * s * s, 1.0);"
		"}\n";

	m_program = glCreateProgram();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	checkShader(vertexShader, "vertexShader");
	glAttachShader(m_program, vertexShader);
	glDeleteShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	checkShader(fragmentShader, "fragmentShader");
	glAttachShader(m_program, fragmentShader);
	glDeleteShader(fragmentShader);

	glLinkProgram(m_program);
	checkProgram(m_program);

	glEnable(GL_DEPTH_TEST);
}

Viewer::~Viewer() {
	glDeleteProgram(m_program);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void Viewer::update(double time) {
	GLfloat new_vertex[] = {
		-.5f, -.5f,  0.f,   0.f, 0.f, 1.f,
	};
	new_vertex[0] += static_cast<GLfloat>(.1 * cos(time));
	new_vertex[1] += static_cast<GLfloat>(.1 * sin(time));

	GLintptr offset = 2 * sizeof(new_vertex);
	glNamedBufferSubData(m_vbo, offset, sizeof(new_vertex), new_vertex);

	GLint loc = glGetUniformLocation(m_program, "uTime");
	if (loc != -1) {
		glUseProgram(m_program);
		glUniform1f(loc, static_cast<GLfloat>(time));
	}
}

void Viewer::render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_program);

	glBindVertexArray(m_vao);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Viewer::resize(int width, int height) {
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}
