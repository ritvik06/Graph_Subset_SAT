#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include<algorithm>

using namespace std;
int count_clauses=0;  //number of clauses
int count_vars=0;  //number of variables
vector<int> smaller_graph_nodes; //nodes in the smaller graph
vector<int> bigger_graph_nodes;  //nodes in the bigger graph
vector<vector<int>> edges_big; //edges in bigger graph
vector<vector<int>> edges_small; //edges in smaller graph
int max_small=0; //biggest node in the smaller graph
int max_big=0; //biggest node in the bigger graph
string out = "";

bool finder(vector<int> vec, int val)
{
	for(int i=0;i<vec.size();i++)
	{
		if(vec.at(i)==val)
			return true;
	}
	return false;
}

void input(string filename)
{
	int c1,c2;
	ifstream file;
	int i;

	file.open(filename);

	if(file.is_open())
	{
		while(file>>c1>>c2)
		{
			if(c1==0 && c2==0)
			{
				break;
			}
			if(c1>max_big)
				max_big = c1;
			if(c2>max_big)
				max_big = c2;

			//Input the bigger graph first
			if(!finder(bigger_graph_nodes,c1))
			{
				bigger_graph_nodes.push_back(c1);
			}
			if(!finder(bigger_graph_nodes,c2))
			{
				bigger_graph_nodes.push_back(c2);
			}
		}

		//Input the smaller graph
		while(file>>c1>>c2)
		{
			if(c1>max_small)
				max_small = c1;
			if(c2>max_small)
				max_small = c2;

			if(!finder(smaller_graph_nodes,c1))
				smaller_graph_nodes.push_back(c1);
			if(!finder(smaller_graph_nodes,c2))
				smaller_graph_nodes.push_back(c2);
		}
	}
	else
		cout<<"FILE DOES NOT EXIST"<<endl;
	file.close();

	//Reading file again to fill the  mapping vector
	edges_big.assign(max_big+1, vector<int>(max_big+1,0));
	edges_small.assign(max_small+1, vector<int>(max_small+1,0));

	file.open(filename);

	if(file.is_open())
	{
		//fill the bigger graph edges
		while(file>>c1>>c2)
		{
			if(c1==0 && c2==0)
				break;
			edges_big.at(c1).at(c2) = 1;
		}

		//fill the smaller graph edges
		while(file>>c1>>c2)
		{
			edges_small.at(c1).at(c2) = 1;
		}
	}
	else
		cout<<"File does not exist"<<endl;
	file.close();

	count_vars = (edges_big.size()-1)*(edges_small.size()-1);
	cout<<"IO DONE "<<(edges_big.size()-1)<<" "<<(edges_small.size()-1)<<endl;

}


void make_all_clauses()
{
	//this function makes all clauses for the sat solver, assuming that Graph is the bigger graph
	string out_trivial="";
	string out_exact="";
	string out_constrain="";
	string out_extra = "";
	int bigger_size = edges_big.size();
	int bigger_size_present = bigger_graph_nodes.size();
	int smaller_size = edges_small.size();
	int smaller_size_present = smaller_graph_nodes.size();
	int big_node,small_node;

	
	for(int j=0;j<bigger_size-1;j++)
	{
		for(int i=1;i<smaller_size; i++)
		{
			for(int k=i+1;k<smaller_size;k++)
			{
				out_extra += (to_string(-1*(j+1 + (bigger_size-1)*(k-1))) + " " + to_string(-1*(j+1+(bigger_size-1)*(i-1))));				
				out_extra += " 0\n";
				count_clauses++;
			}
		}
	}


	for(int i=1;i<smaller_size; i++)
	{
		cout<<i<<endl;
		for(int j=0;j<bigger_size-1;j++)
		{
			
			//make_out_trivial
			out_trivial += (to_string(j+1 + (bigger_size-1)*(i-1)) + " ");
			//cout<<"Made trivial clauses"<<endl;

			//make out_exact
			for(int k=j+1;k<bigger_size-1;k++)
			{

				out_exact += (to_string(-1*(k+1+(bigger_size-1)*(i-1))) + " " + to_string(-1*(j+1+(bigger_size-1)*(i-1))));
				out_exact+=" 0\n"; count_clauses++;
				
			}
			//cout<<"Made exact nodes"<<endl;

			//make out_constrain
			for(int idash=1;idash<edges_small.at(i).size();idash++)
			{
				if(idash==i)
					continue;

				if(edges_small.at(i).at(idash)==0)
				{
					for(int jdash=0;jdash<edges_big.at(j).size()-1;jdash++)
					{
						if(jdash==j)
							continue;

						if(edges_big.at(j).at(jdash)==1)
						{
							out_constrain += (to_string(-1*(j+1+(bigger_size-1)*(i-1))) + " " + to_string(-1*(jdash+1+(bigger_size-1)*(idash-1))) + " 0\n");
							count_clauses++;
						}
					}
				}
				else
				{
					for(int jdash=0;jdash<edges_big.at(j).size()-1;jdash++)
					{
						if(jdash==j)
							continue;
						if(edges_big.at(j).at(jdash)==0)
						{
							out_constrain += (to_string(-1*(j+1+(bigger_size-1)*(i-1))) + " " + to_string(-1*(jdash+1+(bigger_size-1)*(idash-1))) + " 0\n");
							count_clauses++;
						}
					}
				}
			}
		}
		out_trivial+="0\n"; count_clauses++;
	}
	out = out_trivial + out_exact + out_extra + out_constrain;
}

int main(int argc,char *argv[]){

	string file(argv[1]);

	input(file + ".graph");	

	ofstream outfile;
	outfile.open(file + ".sat");

	make_all_clauses();

	//Question == why is count_var not simply equal to the product of the sizes of the two graphs?
	out = "p cnf " + to_string(count_vars) + " " + to_string(count_clauses) + "\n" + out;
	
	outfile << out;

	outfile.close();

	return 0;
}
