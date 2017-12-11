#include <iostream>
#include <vector>
#include <string>

#include <SDL2\SDL.h>
#include <GL\glew.h>
#include <GL\GL.H>

#undef main

unsigned int g_programId = 0;
unsigned int g_vertexShaderId = 0, g_fragmentShaderId = 0;


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

void init(){
	std::string _vertexShaderSource = " void main() { gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";
	std::string _fragmentShaderSource = " void main() { gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); }";

	g_programId = createShaderProgram(_vertexShaderSource, _fragmentShaderSource);

	std::cout << "PROGRAM ID: " << g_programId << std::endl;
}

void display(){
	float _t = SDL_GetTicks() / 1000.0f;
	float _x = abs(sin(_t));

	glClearColor(1.0, _x, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

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

	glDeleteProgram(g_programId);
	glDeleteShader(g_vertexShaderId);
	glDeleteShader(g_fragmentShaderId);

	SDL_Quit();
	SDL_DestroyWindow(_window);
	SDL_GL_DeleteContext(_context);

	return 0;
}

