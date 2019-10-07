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
		// cout << "File opened" << endl;
		while(file >> c1 >> c2){				
			// cout << "Loop1" << endl;
			// cout << c1 << " " << c2;
			if(c1==0 && c2==0)
			{

				break;
			}

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

		while(file >> c1 >> c2){
			// cout << "Loop2 " << endl;
			// cout << c1 << " " << c2 << endl;
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
		}

	}
	else{
		cout << " File doesn't exist" << endl;
	}
	
	// cout << "I/O complete" << endl;
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

void variable_matrix(){

	var_table.clear();
	var_table.resize(Graph_Dash.size(), vector<short int>(Graph.size(),1));		

	for(int i=1;i<Graph_Dash.size()+1;i++){
		for(int j=1;j<Graph.size()+1;j++){
			if(Graph_Dash[i].get_indegree()>Graph[j].get_indegree() || Graph_Dash[i].get_outdegree()>Graph[j].get_outdegree()){
				var_table[i-1][j-1]=0;
			count_vars++;
			}
		}
	}
}

void make_all_clauses()
{
	//this function makes all clauses for the sat solver, assuming that Graph is the bigger graph

	string out_trivial=""; //for all trivial clauses enforcing at least 1 true per node of smaller graph
	string out_exact=""; //for all clauses which enforce exactly 1 true per node of smaller graph
	string out_c2=""; //a negation single literal for all invalid connections based only on indegree and outdegree
	string out_constrain=""; //constraints for if and only if edges exist
	//make the variable table
	// var_table.clear();
	// var_table.resize(Graph_Dash.size(), vector<short int>(Graph.size(),0));
	variable_matrix();

	for(int i=0;i<Graph_Dash.size();i++)
	{
		for(int j=0;j<Graph.size();j++)
		{

			//make out_trivial
			out_trivial += (to_string((j+1) + i*Graph.size()) + " ");

			//make out_exact
			for(int k=0;k<Graph.size();k++)
			{
				int val1 = k+1 + (i*Graph.size());
				if(j==k){
					out_exact+=(to_string(val1) + " ");
				}
				else{
					out_exact+=(to_string(-1*val1) + " ");
				}
			}
			out_exact+="0\n";	
			count_clauses++;

			int v1 = j+1 + (i*Graph.size());

			//make out_c2
			if(var_table[i][j]==0){
				// cout << "Out_C2 " << (-1*v1) << endl;
				out_c2+=(to_string(-1*v1) + " "); 	
			}	

			//make out_constrain
			for(int k=0;k<Graph_Dash.size();k++)
			{
				for(int l=0;l<Graph.size();l++)
				{
					if(i==k || j==l)
						continue;

					int v2 = l+1+(k*Graph.size());

					vector<int> neg_i = (Graph_Dash.at(i+1).get_neg());
					vector<int> neg_k = (Graph_Dash.at(k+1).get_neg());
					vector<int> neg_j = (Graph.at(j+1).get_neg());
					vector<int> neg_l = Graph.at(l+1).get_neg();

					if( (finder(neg_i, k+1) && !finder(neg_j,l+1)) || (!finder(neg_i, k+1) && finder(neg_j, l+1)) )
					{
						out_constrain += (to_string(-1*v1) + " " + to_string(-1*v2) + " 0\n");
						count_clauses++;
					}

				}
			}


		}

		out_trivial+="0\n";
		count_clauses++;
		out_c2+="0\n";
		count_clauses++;
	}

	out = out_trivial + out_exact + out_c2 + out_constrain;
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

	make_all_clauses();

	//Question == why is count_var not simply equal to the product of the sizes of the two graphs?
	out = "p cnf " + to_string(Graph_Dash.size()*Graph.size()) + " " + to_string(count_clauses) + "\n" + out;
	
	outfile << out;

	outfile.close();

	return 0;
}



