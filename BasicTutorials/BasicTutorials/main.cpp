#include <iostream>
#include <vector>
#include <string>

#include <SDL2\SDL.h>
#include <GL\glew.h>
#include <GL\GL.H>

#undef main

unsigned int g_programId = 0;
unsigned int g_vertexShaderId = 0, g_fragmentShaderId = 0;

unsigned int g_bufferId = 0;
unsigned int g_indexBufferId = 0;

unsigned int createShader(unsigned int type, const std::string &source){
	unsigned int _id = glCreateShader(type);

	auto _source = source.c_str();
	glShaderSource(_id, 1, &_source, 0);

	glCompileShader(_id);

	int _compile = false;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &_compile);

	if (_compile == GL_FALSE){

		int _length = 0;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &_length);

		std::vector<char> _info(_length);
		glGetShaderInfoLog(_id, _length, &_length, _info.data());


		std::cout << std::string(_info.begin(), _info.end()) << std::endl;

		glDeleteShader(_id);

		return 0;
	}

	return _id;
}

unsigned int createShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource){
	g_vertexShaderId = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	g_fragmentShaderId = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	if (g_vertexShaderId && g_fragmentShaderId){
		unsigned int _programId = glCreateProgram();

		glAttachShader(_programId, g_vertexShaderId);
		glAttachShader(_programId, g_fragmentShaderId);

		glLinkProgram(_programId);

		int _linked = 0;
		glGetProgramiv(_programId, GL_LINK_STATUS, &_linked);

		if (_linked == GL_FALSE){
			int _length = 0;
			glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &_length);

			std::vector<char> _info(_length);
			glGetProgramInfoLog(_programId, _length, &_length, _info.data());

			std::cout << std::string(_info.begin(), _info.end()) << std::endl;

			glDeleteProgram(g_programId);
			glDeleteShader(g_vertexShaderId);
			glDeleteShader(g_fragmentShaderId);

			return 0;
		}


		glDetachShader(_programId, g_vertexShaderId);
		glDetachShader(_programId, g_fragmentShaderId);

		return _programId;
	}

	return 0;
}

unsigned int createIndexBuffer(){
	std::vector<unsigned int > _data = {
		0, 1, 2, 2, 1, 3
	};

	unsigned int _bufferId = 0;
	glGenBuffers(1, &_bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _data.size() * sizeof(_data[0]), &_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return _bufferId;

}

unsigned int createBuffer(){
	/*std::vector<float> _data = {
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, -0.5f,

		0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f
	};*/

	std::vector<float> _data = {
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f
	};

	unsigned int _bufferId = 0;
	glGenBuffers(1, &_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
	glBufferData(GL_ARRAY_BUFFER, _data.size() * sizeof(_data[0]), &_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return _bufferId;
}

void init(){
	std::string _vertexShaderSource = "#version 330 core \n \
									   layout (location = 0) in vec2 vertex; void main() { gl_Position = vec4(vertex, 0.0, 1.0); }";
	std::string _fragmentShaderSource = " void main() { gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); }";

	g_programId = createShaderProgram(_vertexShaderSource, _fragmentShaderSource);
	g_bufferId = createBuffer();
	g_indexBufferId = createIndexBuffer();

	std::cout << "PROGRAM ID: " << g_programId << std::endl;
}

void drawTriangle(){
	glUseProgram(g_programId);

	glBindBuffer(GL_ARRAY_BUFFER, g_bufferId);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexBufferId);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);


	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void display(){
	float _t = SDL_GetTicks() / 1000.0f;
	float _x = abs(sin(_t));

	glClearColor(1.0, _x, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawTriangle();
}



int main(){

	SDL_Window *_window = SDL_CreateWindow("Basic Tutorials", 50, 50, 1280, 720, SDL_WINDOW_OPENGL);

	SDL_GLContext _context = SDL_GL_CreateContext(_window);

	glewInit();
	init();

	SDL_Event _event;
	while (1){
		bool _break = false;

		while (SDL_PollEvent(&_event)){
			if (_event.type == SDL_KEYDOWN){
				if (_event.key.keysym.sym == SDLK_ESCAPE){
					_break = true;
				}
			}

			if (_event.type == SDL_QUIT){
				_break = true;
			}
		}

		display();

		SDL_Delay(1);

		SDL_GL_SwapWindow(_window);

		if (_break)break;
	}

	glDeleteBuffers(1, &g_bufferId);
	glDeleteBuffers(1, &g_indexBufferId);

	glDeleteProgram(g_programId);
	glDeleteShader(g_vertexShaderId);
	glDeleteShader(g_fragmentShaderId);

	SDL_Quit();
	SDL_DestroyWindow(_window);
	SDL_GL_DeleteContext(_context);

	return 0;
}

