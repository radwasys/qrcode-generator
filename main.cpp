#include <bits/stdc++.h>
#include <string>
using namespace std;

#include <SDL2pp/SDL2pp.hh>
using namespace SDL2pp;

#include "qrcode_gen.h"
#include "encoding.h"

// Constants
const int MAX_CHARACTERS = 32;
const int MODULE_NUMBER = 25;
const int MODULE_SIZE = 20;
int length = MODULE_SIZE * (MODULE_NUMBER+8);
const vector<int> REV_INFO_PATTERN = {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1};

int main(){
	multimap<int, int> generator_polynomial = {
		{10, 0},
		{9, 251},
		{8, 67},
		{7, 46},
		{6, 61},
		{5, 118},
		{4, 70},
		{3, 64},
		{2, 94},
		{1, 32},
		{0, 45}
	};

	// Getting Data
	string data;
	do{
		cout << "Please Enter the Data to be Converted to QR Code (MAX: " << MAX_CHARACTERS << ") ";
		getline(cin, data);
	}while(data.length() > MAX_CHARACTERS);
	
	// Encode Data
	string encoded_data = encode_data(data);
	vector<int> data_cw = get_data_cw(encoded_data);
	vector<int> ec_cw = get_ec_cw(data_cw, generator_polynomial);
	string msg = get_final_msg(ec_cw, data_cw);

	// Represent Data in QR code
	vector<vector<int>> grid(MODULE_NUMBER, vector<int>(MODULE_NUMBER, 2));

	add_finder_patterns(grid, MODULE_NUMBER);
	add_timing_pattern(grid, MODULE_NUMBER);
	add_alignment_pattern(grid);
	add_format_info(grid, REV_INFO_PATTERN, MODULE_NUMBER);
	add_data_pattern(grid, msg, MODULE_NUMBER);
	add_mask(grid, MODULE_NUMBER);


	// Rendering QR Code
	SDLTTF sdl_ttf;
	Window w("QR Code",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, length, length, SDL_WINDOW_RESIZABLE);
	Renderer r(w, -1, SDL_RENDERER_ACCELERATED);
	
	r.Clear();
	
	Rect bg(0, 0, length, length);
	r.SetDrawColor(255, 255, 255);
	r.FillRect(bg);
	for(int i=0; i<MODULE_NUMBER; i++){
		for(int j=0; j<MODULE_NUMBER; j++){
			Rect rect(MODULE_SIZE*(j+4), MODULE_SIZE*(i+4), MODULE_SIZE, MODULE_SIZE);
			if(grid[i][j]==0 || grid[i][j] == 3) r.SetDrawColor(0, 0, 0);
			else r.SetDrawColor(255, 255, 255);
			r.FillRect(rect);
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

