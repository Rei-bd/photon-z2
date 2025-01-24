#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

using namespace std;

double ro(int i, double p, double l, double h) {
	double answer;
	switch (i) {
	case 1:
		answer = 1;
		break;
	case 2:
		answer = l;
		break;
	case 3:
		answer = p;
		break;
	case 4:
		answer = h;
		break;
	case 5:
		answer = l * p;
		break;
	case 6:
		answer = l * h;
		break;
	case 7:
		answer = p * h;
		break;
	case 8:
		answer = l * l;
		break;
	case 9:
		answer = p * p;
		break;
	case 10:
		answer = h * h;
		break;
	case 11:
		answer = p * l * h;
		break;
	case 12:
		answer = l * l * l;
		break;
	case 13:
		answer = l * p * p;
		break;
	case 14:
		answer = l * h * h;
		break;
	case 15:
		answer = l * l * p;
		break;
	case 16:
		answer = p * p * p;
		break;
	case 17:
		answer = p * h * h;
		break;
	case 18:
		answer = l * l * h;
		break;
	case 19:
		answer = p * p * h;
		break;
	case 20:
		answer = h * h * h;
		break;
	default:
		answer = 0;
		break;
	}
	return answer;
}

double calcRPC(unordered_map<string, double>& coefs, string name, double p, double l, double h) {
	double answer = 0;
	for (int i = 1; i != 21; ++i) {
		answer += coefs[name + '_' + to_string(i)] * ro(i, p, l, h);
	}
	return answer;
}

int main(int argc, char* argv[]) {
	double latitude, longitude, height;
	string rpc;

	if (argc == 5) {
		try {
		latitude = stod(argv[1]);
		longitude = stod(argv[2]);
		height = stod(argv[3]);
		rpc = argv[4];
		}
		catch (invalid_argument) {
			cout << "Invalid args";
			exit(0);
		}
	}
	else {
		cout << "Invalid count of args";
		exit(0);
	}

	unordered_map<string, double> coefs;
	ifstream in(rpc);
	if (in.is_open()) {
		while (!in.eof()) {
			string name, value;
			in >> name;
			if (name.empty())
				continue;
			if (name == "meters" || name == "degrees" || name == "pixels")
				in >> name;
			if (!name.empty())
				name.pop_back();
			in >> value;
			try {
				coefs[name] = stod(value);
			}
			catch (invalid_argument) {
				coefs[name] = 0;
			}
		}
	}

	double p, l, h;
	p = (latitude - coefs["LAT_OFF"]) / coefs["LAT_SCALE"];
	l = (longitude - coefs["LONG_OFF"]) / coefs["LONG_SCALE"];
	h = (height - coefs["HEIGHT_OFF"]) / coefs["HEIGHT_SCALE"];


	double rn_num, rn_denum;
	double cn_num, cn_denum;

	rn_num = calcRPC(coefs, "LINE_NUM_COEFF", p, l, h);
	rn_denum = calcRPC(coefs, "LINE_DEN_COEFF", p, l, h);
	cn_num = calcRPC(coefs, "SAMP_NUM_COEFF", p, l, h);
	cn_denum = calcRPC(coefs, "SAMP_DEN_COEFF", p, l, h);

	double rn = rn_num / rn_denum;
	double cn = cn_num / cn_denum;

	double row = rn * coefs["LINE_SCALE"] + coefs["LINE_OFF"];
	double col = cn * coefs["SAMP_SCALE"] + coefs["SAMP_OFF"];

	cout.precision(4);
	cout << fixed;
	cout << "Column: " << col << endl;
	cout << "Row: " << row << endl;
}