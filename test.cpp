#include "polynomials.h"
#include <bits/stdc++.h>
using namespace std;

int main(){
	multimap<int, int> msg = {
		{15, 32}, {14, 91}, {13, 11}, {12, 120}, {11, 209}, {10, 114},
		{9, 220}, {8, 77}, {7, 67}, {6, 64}, {5, 236}, {4, 17},
		{3, 236}, {2, 17}, {1, 236}, {0, 17}
	};

	multimap<int, int> gen = {
		{10, 0}, {9, 251}, {8, 67}, {7, 46}, {6, 61}, {5, 118},
		{4, 70}, {3, 64}, {2, 94}, {1, 32}, {0, 45}
	};

	multimap<int, int> msg_poly;
	for(auto x : msg)
		msg_poly.insert({x.first+10, x.second});

	vector<int> ec_cw = get_ec_cw(msg_poly, gen);
	for(auto x : ec_cw) cout << x << ", ";
	cout << endl;
	return 0;
}

