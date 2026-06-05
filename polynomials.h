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
	// Make them same lead
	int msg_deg = (--msg_polynomial.end())->first;
	int gen_deg = (--generator_polynomial.end())->first;
	int diff = msg_deg-gen_deg;
	multimap<int, int> gen_polynomial;
	for(auto x : generator_polynomial){
		gen_polynomial.insert({x.first+diff, x.second});
	}

	// Long Division
	multimap<int, int> result;
	multimap<int, int> remainder = msg_polynomial;

	while(get_degree(remainder) >= get_degree(gen_polynomial)){
		// Divide Leads of Polynomials
		float new_coeff = get_coeffecient(remainder) / float(get_coeffecient(gen_polynomial));
		int new_deg = get_degree(remainder)-get_degree(gen_polynomial);

		result.insert({new_deg, new_coeff});

		// Multiply Last element of Result By Generator
		float last_coeff = get_coeffecient(result);
		int last_deg = get_degree(result);
		
		multimap<int, int> pre_remainder = remainder;
		remainder.clear();
		for(auto x : gen_polynomial){
			remainder.insert({last_deg+x.first, last_coeff*x.second});
		}

		// Subtract new remainder from old remainder
		auto old_it = pre_remainder.end();
		auto new_it = remainder.end();
		old_it--;
		new_it--;

		while(old_it != (--pre_remainder.begin())){
			int prerem_coeff = old_it->second;
			int rem_coeff = new_it->second;
			int prerem_deg = old_it->first;
			int rem_deg = new_it->first;

			// Same Degree
			if(prerem_deg == rem_deg){
				new_it->second = prerem_coeff - rem_coeff;
				//if(new_it->second == 0) new_it.erase();
			} else if(prerem_deg < rem_deg)
				remainder.insert({prerem_deg, prerem_coeff});
				
				new_it--;
				old_it--;
		}
	}
	vector<int> ec_cw;
	for(auto x : remainder)
		ec_cw.push_back(x.second);

	return ec_cw;
}
