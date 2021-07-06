#include <bits/stdc++.h>
#define all(x) x.begin(), x.end()

struct Assign
{
	std::vector<std::vector<int>> c_box, two;
	std::vector<int> ans;
	int node;

	int RandomPick()
	{
		std::set<int> s;
		for(size_t i=0; i<c_box.size(); i++)
		{
			int x = c_box[i][two[i][0]];
			if(!s.count(x))
				s.insert(x);
			x = c_box[i][two[i][1]];
			if(!s.count(x))
				s.insert(x);
		}
		for(auto ele:s)
			std::cout << ele << " | ";

		srand(time(NULL));
		std::set<int>::iterator it = s.begin();
		advance(it, rand()%s.size());
	
		return *it;
	}

	void Setting(int x)
	{
		node = x;
		ans[abs(node)] = node;
		std::cout << "================> Pick: " << node << "\n";

		std::vector<std::vector<int>> c_box_tmp, two_tmp;
		int idx = 0;
		for(auto c:c_box)
		{
			auto tmp = c;
			auto it = std::find(all(tmp), node);
			if(it == tmp.end())
			{
				it = std::find(all(tmp), -node);
				if(it != tmp.end())
				{	
					tmp.erase(it);
				}
				c_box_tmp.push_back(tmp);
				if(tmp.size() > 1)
					two_tmp.push_back(two[idx]);
				else
					two_tmp.push_back({0, 0});
			}
			idx++;
		}
		c_box = c_box_tmp;
		two = two_tmp;
	}

	Assign(
			std::vector<std::vector<int>> c_box,
			std::vector<std::vector<int>> two,
			std::vector<int> ans)
		: c_box(c_box), two(two), ans(ans) {}

	// Assign(const Assign &A) = delete;
};

void Print1D(std::vector<int> v)
{
	for(auto ele:v)
		std::cout << ele << " | ";
	std::cout << "\n";
}

void Print2D(std::vector<std::vector<int>> v)
{
	for(auto r:v)
	{
		for(auto ele:r)
			std::cout << ele << " -> ";
		std::cout << "\n";
	}
}

void PrintAll(Assign &A)
{
	Print2D(A.c_box);
	Print2D(A.two);
	Print1D(A.ans);
	std::cout << A.node << "\n";
}

void GetData(
		std::string cnf_file,
		int &l_num,
		int &c_num,
		std::vector<std::vector<int>> &box,
		std::vector<std::vector<int>> &two)
{
	std::string s;
	std::ifstream cnf(cnf_file);
	if(!cnf) std::cout << "Can't read file: " << cnf_file << "\n", exit(0);

	cnf >> s >> s;
	cnf >> l_num >> c_num;

	int tmp;
	std::vector<int> c_tmp;
	while(cnf >> tmp)
	{
		if(tmp == 0)
		{
			box.push_back(c_tmp);
			c_tmp.clear();
			continue;
		}
		c_tmp.push_back(tmp);
	}
	for(auto c:box)
	{
		if(c.size() > 1) two.push_back(std::vector<int> {0, 1});
		else two.push_back(std::vector<int> {0, 0});
	}
}

bool UnitPropagate(
		std::queue<int> &q,
		std::stack<Assign> &s)
{
	// TODO
	auto A = s.top();
	std::cout << "UnitProp - node: " << A.node << "\n";
	auto c_box = A.c_box;
	auto two = A.two;
	auto ans = A.ans;
	std::cout << "-----Check-----\n";
	PrintAll(A);
	while(!q.empty())
	{
		int x = q.front();
		q.pop();
		int idx=0;
		for(auto c:c_box)
		{
			if(c.size() == 0) return false;
			int y = c[two[idx][0]];
			std::cout << "--> See y: " << y << "\n";
			idx++;
			if(c.size() == 1)
			{
				if(ans[abs(y)] == 0)
				{
					std::cout << "Unit: " << y << "*******\n";
					auto unit(A);
					unit.Setting(y);
					s.push(unit);
					ans[abs(y)] = y;
					return true;
				}
				else if(ans[abs(y)] != y)
					return false;
			}
			std::cout << idx << " $ !!!!!Ans_tmp!!!!!\n";
			Print1D(ans);
		}
	}
	return true;
}

bool Backtrack()
{
	// TODO
}

std::vector<int> DPLL(std::string cnf)
{
	// init
	int l_num, c_num;
	std::vector<std::vector<int>> c_box;
	std::vector<std::vector<int>> two;
	GetData(cnf, l_num, c_num, c_box, two);
	std::vector<int> ans(l_num+1, 0);
	std::queue<int> prop;
	std::stack<Assign> A_stack;

	std::cout << "-----All clauses-----\n";
	Print2D(c_box);

	// TODO
	Assign init(c_box, two, ans);
	int x = init.RandomPick();
	init.Setting(x);
	A_stack.push(init);
	prop.push(-x);
	std::cout << "-----INIT-----\n";

	while(!A_stack.empty())
	{
		while(!UnitPropagate(prop, A_stack))
		{
			if(A_stack.empty()) return std::vector<int> (l_num+1, 0);
			A_stack.pop();
			auto back(A_stack.top());
			back.Setting(-back.node);
			A_stack.pop();
			A_stack.push(back);
			std::cout << "-----Backtracking-----\n";
		}
		auto tmp(A_stack.top());
		std::cout << "-----TMP-----\n";
		if(tmp.c_box.size() == 0) 
		{
			std::cout << "#####Ans#####\n";
			Print1D(tmp.ans);
			return tmp.ans;
		}
		x = tmp.RandomPick();
		auto next(tmp);
		next.Setting(x);
		A_stack.push(next);
		prop.push(-x);
		std::cout << "-----Next-----\n";
	}
}

int main(int argc, char* argv[])
{
	if(argc < 2) return 0;
	std::string cnf_file = argv[1];
	DPLL(cnf_file);

	return 0;
}
