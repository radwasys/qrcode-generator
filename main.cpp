#include <bits/stdc++.h>
using namespace std;

#include <SDL2pp/SDL2pp.hh>
using namespace SDL2pp;


int main(){
	const int MODULE_NUMBER = 25;
	const int MODULE_SIZE = 15;
	int length = MODULE_SIZE * MODULE_NUMBER;

	// Getting QR Code
	vector<vector<int>> grid(MODULE_NUMBER, vector<int>(MODULE_NUMBER, 1));

	// Generating Oreintation Information

	// Vertical Line 1, 2, 3, 4
	for(int i=0; i<=6; i++){
		grid[i][0] = 0;
		grid[i][6] = 0;

		grid[i][18] = 0;
		grid[i][24] = 0;
	}
	
	// Horizontal Line 1, 2, 3, 4
	for(int j=0; j<=6; j++){
		grid[0][j] = 0;
		grid[6][j] = 0;

		grid[18][j] = 0;
		grid[24][j] = 0;
	}
	
	// Horizontal Line 5, 6
	for(int j=18; j<25; j++){
		grid[0][j] = 0;
		grid[6][j] = 0;
	}
	
	// Vertical Line 5, 6
	for(int i=18; i<25; i++){
		grid[i][0] = 0;
		grid[i][6] = 0;
	}
	
	// Right and Left Inner Squares
	for(int i=2; i<=4; i++){
		for(int j=2; j<=4; j++)
			grid[i][j]=0;
		for(int j=20; j<=22; j++)
			grid[i][j]=0;
	}

	// Bottom Inner Square
	for(int i=20; i<=22; i++)
		for(int j=2; j<=4; j++)
			grid[i][j]=0;

	// Timing Pattern
	bool white = true;
	// Horizontal Pattern
	for(int j=7; j<18; j++){
		grid[6][j] = white;
		white = !white;
	}

	// Vertical Pattern
	white = true;
	for(int i=7; i<18; i++){
		grid[i][6] = white;
		white = !white;
	}

	// Rendering QR Code
	SDLTTF sdl_ttf;
	Window w("demo",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, length, length, SDL_WINDOW_RESIZABLE);
	Renderer r(w, -1, SDL_RENDERER_ACCELERATED);
	
	r.Clear();
	
	for(int i=0; i<MODULE_NUMBER; i++){
		for(int j=0; j<MODULE_NUMBER; j++){
			Rect rect(MODULE_SIZE*i, MODULE_SIZE*j, MODULE_SIZE, MODULE_SIZE);
			if(!grid[i][j]) r.SetDrawColor(0, 0, 0);
			else r.SetDrawColor(255, 255, 255);
			r.FillRect(rect);
			r.SetDrawColor(255,0,0);
			r.DrawRect(rect);
		}
	}

	r.Present();
	
	while(1){
		SDL_Delay(1000);
		SDL_Event event;
		while(SDL_PollEvent(&event))
			if(event.type==SDL_QUIT) return 0;
	}
	return 0;
}

