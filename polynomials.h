#include <bits/stdc++.h>
#include <cmath>
using namespace std;

// Convert Decimal to Power of 2
int get_power_two(int num){
	float power=log2(num);
	int twos_divided = 0;
	while(log2(num)-int(log2(num)) > 0){
		if(num%2 != 0) num=num^285;
		num/=2;
		twos_divided++;
	}
	return log2(num) + twos_divided;
}

int get_num_from_power(int power){
	if(power < 8) return pow(2, power);
	else if(power == 8) return 29;
	int num = get_num_from_power(power-1)*2;
	if(num > 255) return (num^285);
	return num;
}

multimap<int, int> get_msg_polynomial(vector<int> data_cw, int ec_cw){
	multimap<int, int> msg_polynomial;
	for(int i=0; i<data_cw.size(); i++)
		msg_polynomial.insert({i+ec_cw, data_cw[i]});
	return msg_polynomial;
}

int get_degree(multimap<int, int> polynomial){
	return (--polynomial.end())->first;
}

int get_coeffecient(multimap<int, int> polynomial){
	return (--polynomial.end())->second;
}

vector<int> get_ec_cw(multimap<int, int> msg_polynomial, multimap<int, int> generator_polynomial){
	
	// Convert message polynomial to alpha exponenets
	for(auto it=msg_polynomial.begin(); it!=msg_polynomial.end(); it++)
			it->second = get_power_two(it->second);

	// Long Division
	multimap<int, int> remainder = msg_polynomial;
	multimap<int, int> old_remainder;

	for(int i=0; i<msg_polynomial.size(); i++){

		// Make them same lead
		int msg_deg = (--remainder.end())->first;
		int gen_deg = (--generator_polynomial.end())->first;

		int diff = msg_deg-gen_deg;
		multimap<int, int> gen_polynomial;
		for(auto x : generator_polynomial){
			gen_polynomial.insert({x.first+diff, x.second});
		}

		// Multiply Last element of Result By Generator
		int last_coeff = get_coeffecient(remainder);
		old_remainder = remainder;
		remainder.clear();
		for(auto x : gen_polynomial){
			remainder.insert({x.first, (last_coeff+x.second)%255});
		}

		// Subtract new remainder from old remainder
		auto new_it = remainder.rbegin();

		for(auto old_it = old_remainder.rbegin(); old_it != old_remainder.rend(); old_it++){
			int prerem_coeff = old_it->second;
			int rem_coeff = new_it->second;
			int prerem_deg = old_it->first;
			int rem_deg = new_it->first;

			// Same Degree
			if(prerem_deg == rem_deg && new_it != remainder.rend()){
				prerem_coeff = get_num_from_power(prerem_coeff);
				rem_coeff = get_num_from_power(rem_coeff);

				new_it->second = get_power_two(int(prerem_coeff^rem_coeff));
				if(new_it->second < 0) remainder.erase(new_it->first);
				else new_it++;
			}else{
				remainder.insert({prerem_deg, prerem_coeff});
			}
		}

	}

	// Getting Coeffecients of Final Remainder
	vector<int> ec_cw;
	for(auto x : remainder)
		ec_cw.push_back(get_num_from_power(x.second));

	return ec_cw;
}
