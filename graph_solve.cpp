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
		void get_neg(){
			for(int i=0;i<neighbours.size();i++){
				cout << neighbours[i] << " ";
			}
			cout << endl;
		}
};

unordered_map<int,node> Graph;
unordered_map<int,node> Graph_Dash;
vector<vector<short int> > var_table; 

void input(string filename){
	int c1,c2;
	ifstream file;

	file.open(filename);

	if(file.is_open()){
		file >> c1 >> c2;
		do{
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
		}while(c1!=0 && c2!=0)

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
				var_table[i][j]==1;
			}
		}
	}
}

void make_clause(){
	
}