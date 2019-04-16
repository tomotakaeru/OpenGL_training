#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vector>
#include <libpng/png.h>
//#include <Eigen/Core> //not in use, path check


static void OnKeyEvent(GLFWwindow *window, int key,
	int scancode, int action, int mods) {
	printf("Key: %c\n", key);
}

static void OnMouseEvent(GLFWwindow *window, int button,
	int action, int mods) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		printf("Mouse: %f %f\n", x, y);
	}
}
	
GLuint createTextureFromPNGFile(const char *filename) {

	FILE *fp;
	png_structp pPng = NULL;
	png_infop pInfo = NULL;
	int depth, colorType, interlaceType;
	unsigned int width, height;
	int rowSize, imgSize;
	unsigned int i;
	unsigned char *data;
	GLuint texture;

	//PNGファイルを開く
	fopen_s(&fp, filename, "rb");
	if (!fp) {
		fprintf(stderr, "createTextureFromPNGFile: Failed to fopen.");
		return 0;
	}

	//PNGファイルを読み込むための構造体を作成
	pPng = png_create_read_struct(
		PNG_LIBPNG_VER_STRING,
		NULL, NULL, NULL
	);
	pInfo = png_create_info_struct(pPng);

	//初期化
	png_init_io(pPng, fp);

	//画像情報を読み込み
	//画像の幅、高さ、ビット深度、色の表現方法、インターレースの情報を取得する
	png_read_info(pPng, pInfo);
	png_get_IHDR(pPng, pInfo,
		&width, &height,
		&depth, &colorType,
		&interlaceType, NULL, NULL
	);

	//RGBとRGBAのみに対応
	if (colorType != PNG_COLOR_TYPE_RGB && colorType != PNG_COLOR_TYPE_RGBA) {
		fprintf(stderr, "createTextureFromPNGFile: Supprted color type are RGB and RGBA.");
		return 0;
	}

	//インターレースは非対応
	if (interlaceType != PNG_INTERLACE_NONE) {
		fprintf(stderr, "createTextureFromPNGFile: Interlace image is not supprted.");
		return 0;
	}

	//1行のデータサイズと画像の高さから必要なメモリ量を計算して、メモリ確保
	rowSize = png_get_rowbytes(pPng, pInfo);
	imgSize = rowSize * height;
	data = (unsigned char*)malloc(imgSize); //void*の逆参照はキャストする必要

	//ピクセルの読み込み
	for (i = 0; i < height; i++) {
		png_read_row(pPng, &data[i * rowSize], NULL);
	}

	png_read_end(pPng, pInfo);

	//OpenGLテクスチャの作成
	glGenTextures(1, &texture);

	//テクスチャを選択
	glBindTexture(GL_TEXTURE_2D, texture);

	//テクスチャにPNGファイルから読み込んだピクセルを書き込む
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		width, height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, data
	);

	//後片付け
	free(data);
	png_destroy_info_struct(pPng, &pInfo);
	png_destroy_read_struct(&pPng, NULL, NULL);
	fclose(fp);

	return texture;

}


int main(void)
{
	const int windowWidth = 640;
	const int windowHeight = 480;


	// Initialize GLFW
	if (!glfwInit()) {
		return -1;
	}

	// Select OpenGL Version 3.2 Core Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Make window
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -2;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);
	// Set interval for vertical syncronization
	glfwSwapInterval(1);

	// Initialize GLEW
	if ((glewInit() != GLEW_OK)) {
		return -3;
	}

	// Set events
	glfwSetKeyCallback(window, OnKeyEvent);
	glfwSetMouseButtonCallback(window, OnMouseEvent);

	// Set background color
	glClearColor(0, 1, 1, 0.1); //G+B

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		// Render
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw here

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
