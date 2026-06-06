#include <bits/stdc++.h>
using namespace std;

#include <SDL2pp/SDL2pp.hh>
using namespace SDL2pp;

// Constants
const int MODULE_NUMBER = 25;
const int MODULE_SIZE = 15;
int length = MODULE_SIZE * MODULE_NUMBER;
const int EC_CW = 10;
const vector<int> REV_INFO_PATTERN = {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1};
int side_length = 7;
int side_length2 = 3;

string encode_data(string data){
	const int DATA_BITS = 272;
	string encoded_data = "0100"; //Mode Indicator
	encoded_data += bitset<8>(data.length()).to_string(); // CC Indicator

	// Encoding Characters
	for(int i=0; i<data.length(); i++)
		encoded_data += bitset<8>(int(data[i])).to_string();

	// Terminator
	string terminator(min(DATA_BITS-int(encoded_data.length()), 4), '0');

	// Divisible By 8
	while ((encoded_data.length()+terminator.length())%8!=0){
		cout << encoded_data.length() << endl;
		encoded_data = "0" + encoded_data;
	}

	// Padding Bytes
	string pad_bytes = "1110110000010001";
	int index=0;
	while(encoded_data.length()+terminator.length()<DATA_BITS){
		encoded_data += pad_bytes[index%pad_bytes.length()];
	index++;
	}

	// Final Message
	encoded_data += terminator;
	return encoded_data;
}

vector<int> get_data_cw(string encoded_data){
	vector<int> cw(encoded_data.length()/8);
	for(int i=0; i<encoded_data.length()/8; i++){
		string byte = "";
		for(int j=0; j<8; j++){ 
			byte += encoded_data[i+j];
		}
		cw[i] = bitset<8>(byte).to_ulong();
	}
	return cw;
}

void add_finder_patterns(vector<vector<int>> &grid){
	// Top Left Finder
	for(int i=0; i<side_length; i++){
		grid[i][0] = 0;
		grid[i][side_length-1] = 0;
	}

	for(int j=0; j<side_length; j++){
		grid[0][j] = 0;
		grid[side_length-1][j] = 0;
	}

	// Top Right Finder
	for(int i=0; i<side_length; i++){
		grid[i][MODULE_NUMBER-side_length] = 0;
		grid[i][MODULE_NUMBER-1] = 0;
	}

	for(int j=MODULE_NUMBER-side_length; j<MODULE_NUMBER; j++){
		grid[0][j] = 0;
		grid[side_length-1][j] = 0;
	}

	// Bottom Left Finder
	for(int i=MODULE_NUMBER-side_length; i<MODULE_NUMBER; i++){
		grid[i][0] = 0;
		grid[i][side_length-1] = 0;
	}

	for(int j=0; j<side_length; j++){
		grid[MODULE_NUMBER-side_length][j] = 0;
		grid[MODULE_NUMBER-1][j] = 0;
	}

	// Top Left Inner Square
	for(int i=2; i<side_length2+2; i++)
		for(int j=2; j<side_length2+2; j++)
			grid[i][j] = 0;

	// Top Right Inner Square
	for(int i=2; i<side_length2+2; i++)
		for(int j=MODULE_NUMBER-side_length+2; j<side_length2+MODULE_NUMBER-side_length+2; j++) 
			grid[i][j] = 0;

	// Bottom Left Inner Square
	for(int i=MODULE_NUMBER-side_length+2; i<side_length2+MODULE_NUMBER-side_length+2; i++)
		for(int j=2; j<side_length2+2; j++)
			grid[i][j] = 0;
}

void add_timing_pattern(vector<vector<int>> &grid){
	// Horizontal Timer
	bool white = true;
	for(int j=side_length; j<MODULE_NUMBER-side_length; j++){
		grid[side_length-1][j] = int(white);
		white = !white;
	}

	// Vertical Timer
	white = true;
	for(int i=side_length; i<MODULE_NUMBER-side_length; i++){
		grid[i][side_length-1] = int(white);
		white = !white;
	}
}

void add_format_info(vector<vector<int>> &grid){
	// Finder Pattern 1
	int k=0;
	for(int j=0; j<side_length+2; j++){
		if(grid[side_length+1][j] == 0) continue;
		grid[side_length+1][j] = REV_INFO_PATTERN[k];
		k++;
	}

	for(int i=side_length+1; i>=0; i--){
		if(grid[i][side_length+1] == 0) continue;
		grid[i][side_length+1] = REV_INFO_PATTERN[k];
		k++;
	}

	// Finder Pattern 3
	k=0;
	for(int i=MODULE_NUMBER-1; i>=MODULE_NUMBER-side_length-1; i--){
		if(i==MODULE_NUMBER-side_length-1){
			grid[i][side_length+1] = 0;
			break;
		}
		grid[i][side_length+1] = REV_INFO_PATTERN[k];
		k++;
	}

	// Finder Pattern 2
	for(int j=MODULE_NUMBER-side_length-1; j<MODULE_NUMBER; j++){
		grid[side_length+1][j] = REV_INFO_PATTERN[k];
		k++;
	}

}

int main(){
	multimap<int, int> generator_polynomial = {
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
	cout << "Please Enter the Data to be Converted to QR Code: ";
	cin >> data;

	// Data Encoding
	string encoded_data = encode_data(data);
	
	// Forming QR Code
	vector<vector<int>> grid(MODULE_NUMBER, vector<int>(MODULE_NUMBER, 1));

	// Generating Finder Patterns
	add_finder_patterns(grid);
	add_timing_pattern(grid);
	// Vertical Pattern
	add_format_info(grid);

	// Rendering QR Code
	
	SDLTTF sdl_ttf;
	Window w("QR Code",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, length, length, SDL_WINDOW_RESIZABLE);
	Renderer r(w, -1, SDL_RENDERER_ACCELERATED);
	
	r.Clear();
	
	for(int i=0; i<MODULE_NUMBER; i++){
		for(int j=0; j<MODULE_NUMBER; j++){
			Rect rect(MODULE_SIZE*j, MODULE_SIZE*i, MODULE_SIZE, MODULE_SIZE);
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

