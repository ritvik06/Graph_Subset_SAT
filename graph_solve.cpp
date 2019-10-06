#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include<algorithm>

using namespace std;

class node{
	private:
		int indegree=0;
		int outdegree=0;
		vector<int> neighbours;
		//int count_clauses=0;

	public:
		void set_indegree(){
			indegree++;
		}
		int get_indegree(){
			return indegree;
		}
		void set_outdegree(){
			outdegree++;
		}
		int get_outdegree(){
			return outdegree;
		}
		void set_neg(int val){
			neighbours.push_back(val);
		}
		vector<int> get_neg(){
			return neighbours;
		}
};
int count_clauses=0;
int count_vars=0;
unordered_map<int,node> Graph;
unordered_map<int,node> Graph_Dash;
vector<vector<short int> > var_table; 
string out;

void input(string filename){
	int c1,c2;
	ifstream file;

	file.open(filename);

	if(file.is_open()){
		file >> c1 >> c2;
		do{
			if(c1==0 && c2==0)
			{

				break;
			}

			if(Graph.find(c1)!=Graph.end()){
				Graph[c1].set_outdegree();
				// Graph[c2].indegree++;
				Graph[c1].set_neg(c2);
				if(Graph.find(c2)!=Graph.end()){
					Graph[c2].set_indegree();
				}
				else{
					node new2;
					new2.set_indegree(); 
					Graph[c2] = new2;
				}
			}
			else{
				node new1;
				new1.set_outdegree();
				new1.set_neg(c2);
				if(Graph.find(c2)!=Graph.end()){
					Graph[c2].set_indegree();
				}
				else{
					node new2;
					new2.set_indegree(); 
					Graph[c2] = new2;
				}	
				Graph[c1] = new1;
			}
		}while(c1!=0 && c2!=0);

		while(file.is_open()){
			if(Graph_Dash.find(c1)!=Graph_Dash.end()){
				Graph_Dash[c1].set_outdegree();
				// Graph[c2].indegree++;
				Graph_Dash[c1].set_neg(c2);
				if(Graph_Dash.find(c2)!=Graph_Dash.end()){
					Graph_Dash[c2].set_indegree();
				}
				else{
					node new2;
					new2.set_indegree(); 
					Graph_Dash[c2] = new2;
				}
			}
			else{
				node new1;
				new1.set_outdegree();
				new1.set_neg(c2);
				if(Graph_Dash.find(c2)!=Graph_Dash.end()){
					Graph_Dash[c2].set_indegree();
				}
				else{
					node new2;
					new2.set_indegree(); 
					Graph_Dash[c2] = new2;
				}	
				Graph_Dash[c1] = new1;
			}			
		}

	}
	else{
		cout << " File doesn't exist" << endl;
	}
	

}

void variable_matrix(){
	var_table.clear();
	var_table.resize(Graph_Dash.size(), vector<short int>(Graph.size(),0));

	for(int i=0;i<Graph_Dash.size();i++){
		for(int j=0;j<Graph.size();j++){
			if(Graph[i].get_indegree()<=Graph_Dash[j].get_indegree() || Graph[i].get_outdegree()<=Graph_Dash[j].get_outdegree()){
				var_table[i][j]=1;
				count_vars++;
			}
		}
	}
}

void make_trivial_clauses(){
	//this function makes clauses which force each node in the shorter graph to have exactly one mapping true
	//Assumes that Graph_Dash is the smaller graph and Graph is the bigger graph
	for(int i=0;i<Graph_Dash.size();i++)
	{
		for(int j=0;j<Graph.size();j++)
		{
			out+=(to_string((j+1) + i*Graph.size()) + " ");
		}

		out+="0\n";

		for(int j=0;j<Graph_Dash.size();j++)
		{
			for(int k=0;k<Graph_Dash.size();k++)
			{
				if(j==k)
					out+=(to_string((k+1 + (i*Graph.size()))) + " ");
				else
					out+=(to_string(-1*(k+1 + (i*Graph.size()) )) + " ");
			}
			out+="0\n";
			count_clauses++;
		}
		count_clauses++;		
	}
}

bool finder(vector<int> vec, int val)
{
	for(int i=0;i<vec.size();i++)
	{
		if(vec.at(i)==val)
			return true;
	}
	return false;
}



void make_constrain_clauses()
{
	//this function makes clauses which enforce the constrain that a mapping is valid iff edges are present in both graphs
	for(int i=0;i<Graph_Dash.size();i++)
	{
		for(int j=0;j<Graph.size();j++)
		{
			int v1 = j+1 + (i*Graph.size());
			for(int k=0;k<Graph_Dash.size();k++)
			{
				for(int l=0;l<Graph.size();l++)
				{
					if(i==k || j==l)
						continue;

					int v2 = l+1+(k*Graph.size());

					vector<int> neg_i = (Graph_Dash.at(i).get_neg());
					vector<int> neg_k = (Graph_Dash.at(k).get_neg());
					vector<int> neg_j = (Graph.at(j).get_neg());
					vector<int> neg_l = Graph.at(l).get_neg();

					if( (finder(neg_i, k) && !finder(neg_j,l)) || (!finder(neg_i, k) && finder(neg_j, l)) )
					{
						out += (to_string(-1*v1) + " " + to_string(-1*v2) + "0\n");
					}

				}
			}
		}

	}
}


void make_clause_one(){
	for(int i=0;i<Graph_Dash.size();i++){
		for(int j=0;j<Graph.size();j++){
			if(var_table[i][j]==1){
				out+=(to_string((j+1) + i*Graph.size()) + " "); 	
			}
		}
		out+="0\n";
		count_clauses++;
	}
}

void make_clause_two(){
	for(int i=0;i<Graph_Dash.size();i++){
		for(int j=0;j<Graph.size();j++){
			if(var_table[i][j]==0){
				out+=(to_string(-1*((j+1) + i*Graph.size())) + " "); 	
			}
		}
		out+="0\n";
		count_clauses++;
	}	
}



// void make_clause_clash(){
// 	vector<vector<int> > valid;
// 	vector<int> short1;

// 	for(int i=0;i<Graph_Dash.size();i++){
// 		short1.clear();
// 		for(int j=0;j<Graph.size();j++){
// 			if(var_table[i][j]==1){
// 				short1.push_back((j+1) + i*Graph.size());
// 			}

// 		}
// 		valid.push_back(short1);
// 	}

// 	for(int i=0;i<Graph_Dash.size();i++){
// 		for(int j=0;j<valid[i].size()-1;j++){
// 			for(int k=j+1;k<valid[i].size();k++){
// 				out+= to_string(-1*(valid[i][k])) + " " + to_string(-1*(valid[i][k])) + " 0\n"; 
// 				count_clauses++;
// 			}
// 		}
// 	}

// }

// int count_vars(){

// }

int main(int argc,char *argv[]){

	string file(argv[1]);

	input(file + ".graph");	

	ofstream outfile;
	outfile.open(file + ".sat");

	make_clause_one();
	make_clause_two();
	make_clause_clash();

	out = "p cnf " + to_string(count_vars) + " " + to_string(count_clauses) + "\n" + out;
	
	outfile << out;

	outfile.close();

	return 0;
}



