#include <bits/stdc++.h>

// ----- Init -----
void GetData(
	std::string cnf_file, 
	int &l_count,
	int &c_count,
	std::vector<std::vector<int>> &box,
	std::vector<std::pair<int, int>> &two )
{
	std::string s;
	std::ifstream cnf(cnf_file);
	if(!cnf) std::cout << "Can't read file: " << cnf_file << "\n", exit(0);

	cnf >> s >> s;
	cnf >> l_count >> c_count;

	int tmp, i=0;
	std::vector<int> c_tmp;

	while(cnf >> tmp)
	{
		if(tmp == 0)
		{
			box.push_back(c_tmp);
			c_tmp.clear();

			i++;
			continue;
		}

		c_tmp.push_back(tmp);
	}

	// init iterator
	for(auto c:box)
	{
		if(c.size() > 1) two.push_back(std::make_pair(0, 1));
		else two.push_back(std::make_pair(0, 0));
	}

	cnf.close();
}

std::vector<std::vector<int>> GetPos(
	int l_count,
	std::vector<std::vector<int>> c_box )
{
	std::vector<std::vector<int>> pos(l_count+1);
	for(int i=0; i<c_box.size(); i++)
		for(auto l:c_box[i])
			pos[abs(l)].push_back(i);

	return pos;
}

//std::priority_queue<std::tuple<int, bool, int>> GetAct(
//	int l_count,
//	std::vector<std::queue<int>> l_pos )
//{
//	std::priority_queue<std::tuple<int, bool, int>> pq;
//
//	for(int i=1; i<l_pos.size(); i++)
//		pq.push(std::tuple<int, bool, int> (l_pos[i].size(), false,  i));
//
//	return pq;
//}

//void PrintPQ(std::priority_queue<std::tuple<int, bool, int>> pq)
//{
//	while(!pq.empty())
//	{
//		std::cout << "freq: " << std::get<0>(pq.top()) << ", "
//				  << "literal: " << std::get<2>(pq.top()) << "\n";
//		pq.pop();
//	}
//}

// ----- Print -----
void Print1D(std::vector<int> v)
{
	for(auto ele:v)
		std::cout << ele << " | ";
	std::cout << "\n"; 
}

void Print2D(std::vector<std::vector<int>> v)
{
	for(auto row:v)
	{
		for(auto ele:row)
			std::cout << ele << " -> ";
		std::cout << "\n";
	}
}

void PrintIt(std::vector<std::pair<int, int>> two)
{
	for(auto it:two)
		std::cout << it.first << ", " << it.second << " | ";
	std::cout << "\n";
}

// ----- Check -----
bool CheckAns(std::vector<int> ans)
{
	for(int i=1; i<ans.size(); i++)
		if(ans[i] == 0) return false;

	return true;
}

//bool CheckUnit(
//		std::vector<int> clause,
//		int x )
//{
//	std::vector<int> c;
//	if(clause.size() == 1 && clause[0] == x) return true;
//	
//	for(auto l:clause)
//	{
//		if(l == x) return false;
//		if(l == -x) continue;
//		c.push_back(l);
//	}
//
//	std::cout << "-----Reduce-----\n";
//	for(auto l:c)
//		std::cout << l << " -> ";
//	std::cout << "\n";
//
//	if(c.size() == 1) return true;
//	
//	return false;
//}
bool ReduceClause(
		std::vector<std::vector<int>> &box,
		std::vector<std::pair<int, int>> &two,
		std::vector<int> &ans,
		std::queue<int> &q,
		int x,
		int decision)
{
	std::vector<std::vector<int>> box_tmp;
	int i = 0;
	for(auto c:box)
	{
		auto it = std::find(c.begin(), c.end(), x);
		if(it == c.end())
		{
			it = std::find(c.begin(), c.end(), -x);
			if(it != c.end())
			{
				c.erase(it);
				if(two[i].first+2 <= box[i].size())
					two[i] = std::make_pair(two[i].second, two[i].first+2);
			}
			box_tmp.push_back(c);
			if(c.size() == 1)
			{
				std::cout << "===> `select: " << c[0] << "\n";
				if(ans[abs(c[0])] == -c[0])
				{
					std::cout << "Conflict on: " << decision << "\n";
					ans[decision] = -decision;
					for(int i=decision+1; i<ans.size(); i++)
						ans[i] = 0;
					return false;
				}
				ans[abs(c[0])] = c[0];
				q.push(-c[0]);
			}
		}
		i++;
	}
	box = box_tmp;
	return true;
}

bool UnitPropagate(
	std::vector<std::vector<int>> box,
	std::vector<std::pair<int, int>> &two,
	std::vector<int> &ans,
	std::vector<int> relate,
	std::queue<int> &q )
{
	std::cout << "prop_queue: " << q.size() << "\n";
	while(!q.empty())
	{
		int x = q.front();
		std::cout << "first literal: " << x << "\n";
		q.pop();
		for(auto idx:relate)
		{
			if(box[idx][two[idx].first] == x and two[idx].first+2 <= box[idx].size())
			{
				std::cout << "first\n";
				two[idx] = std::make_pair(two[idx].second, two[idx].first+2);
			}
			else if(box[idx][two[idx].second] == x and two[idx].second+1 <= box[idx].size())
			{
				std::cout << "second\n";
				two[idx] = std::make_pair(two[idx].first, two[idx].second+1);
			}
			std::vector<int> tmp;
			auto it = std::find(box[idx].begin(), box[idx].end(), x);
			box[idx].erase(it);
			tmp = box[idx];
			Print1D(tmp);
		}
	}

	return true;
}

bool DPLL(
	std::vector<std::vector<int>> box,
	std::vector<std::pair<int, int>> &two,
	std::vector<int> ans,
	std::stack<std::vector<int>> &level,
	std::queue<int> &q )
{
	int x;
	while(!CheckAns(ans))
	{
		for(int i=1; i<ans.size(); i++)
		{
			if(ans[i] == 0)
			{	
				x = i;
				break;
			}
		}
		std::vector<int> relate;
		for(int i=0; i<two.size(); i++)
			if(box[i][two[i].first] == -x or box[i][two[i].second] == -x)
				relate.push_back(i);
		q.push(-x);
 		std::cout << "\n===> Select: " << x << "\n";

		// update answer in ans
		ans[x] = x;

		// add ans to level
		level.push(ans);

		std::vector<std::vector<int>> box_tmp = box;
		for(auto a:ans)
			if(a != 0)
				if(!ReduceClause(box_tmp, two, ans, q, a, x)) break;
		Print2D(box_tmp);

		// Print
		//std::cout << "-----relative clause-----\n";
		//Print1D(relate);
		//std::cout << "-----iterator-----\n";
		//PrintIt(two);
		std::cout << "-----Answer-----\n";
		Print1D(ans);

		//while(!UnitPropagate(box, two, ans, relate, q))
		//{
		//	if(level.size() == 0) return false;
		//	level.pop();

		//	ans[x] = -x;
		//	level.push(ans);
		//}
	}
	
	return true;
}

int main(int argc, char* argv[])
{
	if(argc < 2) return 0;
	std::string cnf_file = argv[1];

	int l_count, c_count;
	std::vector<std::vector<int>> c_box;
	std::vector<std::pair<int, int>> two;
	GetData(cnf_file, l_count, c_count, c_box, two);

	std::vector<int> ans(l_count+1 ,0);
	std::stack<std::vector<int>> level;
	std::queue<int> prop_q;
	level.push(ans);

	// Print
	std::cout << "-----All clause-----\n";
	Print2D(c_box);

	PrintIt(two);

	DPLL(c_box, two, ans, level, prop_q);

	return 0;
}
