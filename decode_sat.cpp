#include<iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include<algorithm>

using namespace std;

string out="";

void give_mapping(string filename){
	ifstream file;
	string valid;
	int g_node,dash_node;
	int c1;
	int M;

	file.open("temp.txt");
	file >> M;
	file.close();

	file.open(filename);

	file >> valid;


	if(valid=="UNSAT"){
		out+="0";
		return;
	}

	while(file >> c1){
		dash_node = 0;
		g_node = 0;
		if(c1>0){
			int literal = c1;
			dash_node = literal/M;
			g_node = literal - M*(literal/M);
			if(g_node==0){
				g_node = M;
				dash_node = dash_node-1;
			}

			if(dash_node!=-1 && g_node!=0)		
				out+= to_string(dash_node+1) + " " + to_string(g_node) + '\n';

		}
		else
			continue;
	}


}

int main(int argc,char *argv[]){
	ofstream output;
	string file(argv[1]);
	string file_dash(file+".satoutput");
	output.open(file+".mapping");

	give_mapping(file_dash);
	output << out;

	output.close();

	return 0;
}
