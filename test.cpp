#include <bits/stdc++.h>
using namespace std;

#include "encoding.h"

int main(){
	multimap<int,int> generator_polynomial = {
    {6, 47},   // 87
    {5, 83},   // 229
    {4, 248},  // 146
    {3, 251},  // 149
    {2, 198},  // 238
    {1, 6},    // 102
    {0, 191}   // 21
	};

	vector<int> dc = {32, 91, 11, 120, 209, 114, 220, 77,
 67, 64, 236, 17, 236, 17, 236, 17,
 236, 17, 236, 17};

	vector<int> ec_cw = get_ec_cw(dc, generator_polynomial, 7);
	for(auto x : ec_cw) cout << x << " , ";

}
