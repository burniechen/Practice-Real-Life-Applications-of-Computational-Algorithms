#include <bits/stdc++.h>
#include "parser.h"

void GetData(
		std::string cnf_file, 
		int &literal, 
		int &clauses, 
		std::vector<std::list<int>> &box,
		std::vector<int> &t )
{
	std::vector<std::vector<int>> vv;

	parse_DIMACS_CNF(vv, literal, cnf_file.data());
	clauses = vv.size();

	// no w0
	box.push_back(std::list<int> ());

	for(int i=1; i<literal+1; i++)
		t.push_back(i), t.push_back(-i);
		
	std::list<int> c_tmp;
	for(auto c:vv)
	{
		for(auto l:c)
			c_tmp.push_back(l);
		box.push_back(c_tmp);
		c_tmp.clear();
	}
}

std::vector<int> UpdateTable(int x, std::vector<int> table)
{
	std::vector<int> t_tmp;
	for(auto ele:table)
		if(ele != x && ele != -x) t_tmp.push_back(ele);

	return t_tmp;
}

std::vector<std::list<int>> ReduceClause(
		std::vector<std::list<int>> box,
		int x )
{
	std::vector<std::list<int>> box_tmp;
	box_tmp.push_back(std::list<int> ());

	for(int i=1; i<box.size(); i++)
	{
		auto it = std::find(box[i].begin(), box[i].end(), x);
		if(it == box[i].end())
			box[i].remove(-x), box_tmp.push_back(box[i]);
	}

	return box_tmp;
}

int FindUnitClause(std::vector<std::list<int>> box)
{
	for(int i=1; i<box.size(); i++)
		if(box[i].size() == 1) return box[i].front();

	return 0;
}

bool DPLL(
		std::vector<std::list<int>> box, 
		int x, 
		std::vector<int> t,
		std::vector<int> &ans )
{
	std::cout << "\nSelect: " << x;
	ans.push_back(x);
	box = ReduceClause(box, x);

	if(box.size() == 1) return true;
	
	for(int i=1; i<box.size(); i++)
		if(box[i].empty()) return false;
	
	if(FindUnitClause(box) != 0)
	{
		x = FindUnitClause(box);
		std::vector<int> t_tmp = UpdateTable(x, t);
		
		return DPLL(box, x, t_tmp, ans);
	}

	x = t[rand() % t.size()];
	std::cout << "\nRandom select: " << x;
	std::vector<int> t_tmp = UpdateTable(x, t);

	if(DPLL(box, x, t_tmp, ans)) return true;
	else 
	{
		std::cout << "\n\n-----[node] Select branch: " << -x << "-----";

		std::vector<int> ans_tmp;
		for(auto ele:ans)
		{
			if(abs(ele) == abs(-x)) break;
			ans_tmp.push_back(ele);
		}
		ans = ans_tmp;

		return DPLL(box, -x, t_tmp, ans);
	}

	return false;
}

void WriteSatis(
		std::ofstream &output, 
		std::vector<int> ans, 
		int literal )
{
	output << "s SATISFIABlE\n";
	std::vector<bool> ans_print(literal+1, true);
	for(auto ele:ans)
		if(ele < 0) ans_print[abs(ele)] = false;

	output << "v ";
	for(int i=1; i<ans_print.size(); i++)
	{
		if(ans_print[i]) output << i << ' ';
		else output << -i << ' ';
	}

	std::cout << "\n\n=> SATISFIABLE\n";
}

int main(int argc, char* argv[])
{
	if(argc < 2) return 0;
	std::string cnf_file = argv[1];

	int literal, clauses;
	std::vector<std::list<int>> clauseBox;
	std::vector<int> table, ans;

	GetData(cnf_file, literal, clauses, clauseBox, table);	

	srand (time(NULL));
	int x = table[rand() % table.size()];
	std::vector<int> t_tmp = UpdateTable(x, table);

	auto sat_file = cnf_file.substr(0, cnf_file.size()-3) + "sat";
	std::ofstream output(sat_file);

	if(DPLL(clauseBox, x, t_tmp, ans)) WriteSatis(output, ans, literal);	
	else 
	{
		std::cout << "\n\n*****[Root] Select branch: " << -x << "*****";

		if(DPLL(clauseBox, -x, t_tmp, ans)) WriteSatis(output, ans, literal);
		else output << "s UNSATISFIABLE\n", std::cout << "\n\n=> UNSTISFIABLE\n";
	}

	output.close();
	
	return 0;
}
