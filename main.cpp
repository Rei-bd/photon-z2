#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	unordered_map<string, double> RPC;
	ifstream in("RPC.txt");
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
				RPC[name] = stod(value);
			}
			catch (invalid_argument) {
				RPC[name] = 0;
			}
		}
		for (pair<string, double> p : RPC)
		{
			cout << p.first << " " << p.second << endl;
		}
	}
}