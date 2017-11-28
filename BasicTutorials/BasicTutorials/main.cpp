#include <iostream>

#include <SDL2\SDL.h>

#undef main

int main(){

	SDL_Window *_window = SDL_CreateWindow("Engine v1.0.0", 50, 50, 1280, 720, SDL_WINDOW_OPENGL);

	SDL_GLContext _context = SDL_GL_CreateContext(_window);

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

		if (_break)break;
	}

	SDL_Quit();
	SDL_DestroyWindow(_window);
	SDL_GL_DeleteContext(_context);

	return 0;
}