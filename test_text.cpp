#pragma once
#include "Header.h"
#include <cstdlib>
#include <fstream>
#include <memory>
#include "Shape.h"
#include "Window.h"
#include "Matrix.h"


// Display compile result of shader obj
//   shader: シェーダオブジェクト名
//   str: コンパイルエラーが発生した場所を示す文字列
GLboolean printShaderInfoLog(GLuint shader, const char *str) {
	// Get compile status
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) printf("Compile Error in %s\n", str); 

	// Get length of compile log
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize); 
	if (bufSize > 1) {
		// get content of compile log
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		printf("%s\n", &infoLog[0]);
	}

	return static_cast<GLboolean>(status);
}

// Display link result of program obj
//   program: プログラムオブジェクト名
GLboolean printProgramInfoLog(GLuint program) {
	// Get link status
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) printf("Link Error.\n");

	// Get length of link log
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1) {
		// get content of link log
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		printf("%s\n", &infoLog[0]);
	}

	return static_cast<GLboolean>(status);
}

// Create program object
//   vsrc: バーテックスシェーダのソースプログラムの文字列
//   fsrc: フラグメントシェーダのソースプログラムの文字列
GLuint createProgram(const char *vsrc, const char *fsrc)
{
	// Create empty program obj
	const GLuint program(glCreateProgram());

	if (vsrc != NULL) {
		// Create shader obj of vertex shader
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);

		// Attach to program obj
		if (printShaderInfoLog(vobj, "vertex shader"))
			glAttachShader(program, vobj);
		glDeleteShader(vobj);
	}

	if (fsrc != NULL) {
		// Create shader obj of flagment shader
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);

		// Attach to program obj
		if (printShaderInfoLog(fobj, "fragment shader"))
			glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}

	// Link program obj
	glBindAttribLocation(program, 0, "position");
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	if (printProgramInfoLog(program))
		return program;
	glDeleteProgram(program);
	return 0;
}

// シェーダのソースファイルを読み込んだメモリを返す
//   name: シェーダのソースファイル名
//   buffer: 読み込んだソースファイルのテキスト
bool readShaderSource(const char *name, std::vector<GLchar> &buffer) {
	if (name == NULL) return false; 

    // ソースファイルを開く
	std::ifstream file(name, std::ios::binary);
	if (file.fail())   {
		printf("Error: Can't open source file: %s\n", name);
		return false;
	} 

    // ファイルの末尾に移動し現在位置（＝ファイルサイズ）を得る
	file.seekg(0L, std::ios::end); 
	GLsizei length = static_cast<GLsizei>(file.tellg());

	// ファイルサイズのメモリを確保
	buffer.resize(length + 1); 

	// ファイルを先頭から読み込む
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0'; 

	if (file.fail()) {
		printf("Error: Could not read source file: %s\n", name);
		file.close();
		return false;
	} 

	// 読み込み成功
	file.close();
	return true;
} 

// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
//   vert: バーテックスシェーダのソースファイル名
//   frag: フラグメントシェーダのソースファイル名
GLuint loadProgram(const char *vert, const char *frag) {
	// シェーダのソースファイルを読み込む
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	std::vector<GLchar> fsrc;
	const bool fstat(readShaderSource(frag, fsrc)); 

    // プログラムオブジェクトを作成する
	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}
/*
static void OnKeyEvent(Window *window, int key,
	int scancode, int action, int mods) {
	printf("Key: %c\n", key);
}

static void OnMouseEvent(Window *window, int button,
	int action, int mods) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		printf("Mouse: %f %f\n", x, y);
	}
}
*/

// 矩形の頂点の位置
constexpr Object::Vertex rectangleVertex[] =
{
  { -0.5f, -0.5f },
  {  1.5f, -0.5f },
  {  1.5f,  1.5f },
  { -0.5f,  1.5f }
};


int test_text(void)
{
	// Initialize GLFW
	if (!glfwInit()) return -1;

	// Register function at exit
	atexit(glfwTerminate);

	// Select OpenGL Version 3.2 Core Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Make window
	Window window;

	// Set events
	//glfwSetKeyCallback(window, OnKeyEvent);
	//glfwSetMouseButtonCallback(window, OnMouseEvent);

	// Set background color
	glClearColor(0, 1, 1, 0.1);

	// Create program object
	const GLuint program(loadProgram("point.vert", "point.frag"));

	// uniform 変数の場所を取得する
	const GLint modelviewLoc(glGetUniformLocation(program, "modelview"));

	// Create shape data
	std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex)); 
	
	// Loop until the user closes the window
	while (!window.shouldClose()) {
		// Clear window
		glClear(GL_COLOR_BUFFER_BIT);

		// Use shader program
		glUseProgram(program); 

		// 拡大縮小の変換行列を求める
		const GLfloat *const size(window.getSize());
		const GLfloat scale(window.getScale() * 2.0f);
		const Matrix scaling(Matrix::scale(scale / size[0], scale / size[1], 1.0f)); 

		// 平行移動の変換行列を求める
		const GLfloat *const position(window.getLocation());
		const Matrix translation(Matrix::translate(position[0], position[1], 0.0f)); 

		// モデル変換行列を求める
		const Matrix model(translation * scaling);

		// ビュー変換行列を求める
		const Matrix view(Matrix::lookat(0.0f, 0.0f, 0.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f)); 

		// モデルビュー変換行列を求める
		const Matrix modelview(view * model); 

		// uniform 変数に値を設定する
		glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data()); 
		
		// Draw here
		shape->draw();

		// Swap front and back buffers & wait for events
		window.swapBuffers();
	}
	glfwTerminate();
	return 0;
}
