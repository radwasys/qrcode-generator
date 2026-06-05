#include <bits/stdc++.h>
using namespace std;

#include <SDL2pp/SDL2pp.hh>
using namespace SDL2pp;

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
		cw[i] = bitset<8>(byte).set_ulong();
	}
	return cw;
}




int main(){
	const int MODULE_NUMBER = 25;
	const int MODULE_SIZE = 15;
	int length = MODULE_SIZE * MODULE_NUMBER;
	const int EC_CW = 10;
	const string INFO_PATTERN = "111011111000100";

	// Getting Data
	string data;
	cout << "Please Enter the Data to be Converted to QR Code: ";
	cin >> data;

	// Data Encoding
	string encoded_data = encode_data(data);
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
	}
	
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

	// Format Information Pattern
	
	// Finder 1 Horizontal
	int k=0;
	for(int j=0; j<6; j++; k++)
		grid[8][j] = INFO_PATTERN[k];

	grid[8][k+2] = INFO_PATTERN[++k];
	grid[8][k+3] = INFO_PATTERN[++k];
	grid[7][k+3] = INFO_PATTERN[++k];

	// Finder 1 Vertical
	for(int i=5; i>=0; i--; k++)
		grid[i][8] = INFO_PATTERN[k];
		

	// Finder 3
	k=0;
	for(int i=24; i>=18; i--; k++)
		grid[i][8] = INFO_PATTERN[k];

	grid[17][8] = 0; // Dark Module

	// Finder 2
	for(int j=17; j<25; j++; k++)
		grid[8][j] = INFO_PATTERN[k];
	
	

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

