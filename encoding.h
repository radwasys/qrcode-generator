#include <bits/stdc++.h>
#include <cmath>
using namespace std;

string encode_data(string data, int data_bits=272, string mode_indicator="0100",
		string pad_bytes="1110110000010001"){
	string encoded_data = ""; 
	encoded_data += mode_indicator; // Add Mode Indicator
	
	// Add Character Count Indicator
	encoded_data += bitset<8>(data.length()).to_string();

	// Encoding Characters
	for(int i=0; i<data.length(); i++)
		encoded_data += bitset<8>(int(data[i])).to_string();

	// Terminator
	string terminator(min(data_bits-int(encoded_data.length()), 4), '0');

	// Make Data bits divisible By 8
	while ((encoded_data.length()+terminator.length())%8!=0){
		cout << encoded_data.length() << endl;
		encoded_data = "0" + encoded_data;
	}

	// Add Padding Bytes
	int index=0;
	while(encoded_data.length()+terminator.length()<data_bits){
		encoded_data += pad_bytes[index%pad_bytes.length()];
	index++;
	}
	
	// Add Terminator
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

multimap<int, int> get_msg_polynomial(vector<int> data_cw, int ec_cw=10){
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

string get_final_msg(vector<int> ec_cw, vector<int> data_cw){
	string msg = "";
	for(int i=0; i<data_cw.size(); i++)
		msg += bitset<8>(data_cw[i]).to_string();

	for(int i=0; i<ec_cw.size(); i++)
		msg += bitset<8>(ec_cw[i]).to_string();

	msg += "0000000";
}
