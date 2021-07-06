#include <bits/stdc++.h>

struct LinkedList
{
	int data;
	LinkedList *next;
	LinkedList(): data(777), next(nullptr) {};
	LinkedList(int x): data(x), next(nullptr) {};
	LinkedList(int x, LinkedList *next): data(x), next(next) {};
};

LinkedList* Traversal(LinkedList* first)
{
	LinkedList* curr = first;
	while(curr->next != nullptr)
		curr = curr->next;

	return curr;
}

void PrintAll(LinkedList* first)
{
	LinkedList* curr = first;
	while(curr->next != nullptr)
		std::cout << curr->data << "->", curr = curr->next;
	std::cout <<  curr->data;
}

void GetData(std::string cnf_file,
		int &literal, 
		int &clauses,
		std::vector<LinkedList*> &box,
		std::vector<LinkedList*> &p)
{
	std::string s;
	std::ifstream cnf(cnf_file);
	if(!cnf)
		std::cout << "Can't read file: " << cnf_file << "\n", exit(0);

	cnf >> s >> s;
	cnf >> literal;
	cnf >> clauses;

	int tmp;
	int i=0;
	bool flag = true;
	
	LinkedList* currV = new LinkedList;
	
	for(int i=0; i<literal; i++)
	{	
		LinkedList* currP = new LinkedList;
		p.push_back(currP);
	}

	while(cnf >> tmp)
	{
		if(tmp == 0) 
		{
			flag = true;
			++i;
			continue;
		}

		if(flag)
		{
			LinkedList* node = new LinkedList(tmp);
			box.push_back(node);

			currV = node;
			flag = false;
		}
		else
		{
			LinkedList* next = new LinkedList(tmp);
			currV->next = next;
			currV = currV->next;
		}

		LinkedList* clauseNum = new LinkedList(i);
		LinkedList* currP = Traversal(p[abs(tmp)-1]);
		currP->next = clauseNum;
	}

	cnf.close();
}

int main(int argc, char *argv[])
{
	if(argc < 2) exit(0);
	std::string cnf_file = argv[1];

	int literal, clauses;
	std::vector<LinkedList*> clauseBox;
	std::vector<LinkedList*> literalPos;

	GetData(cnf_file, literal, clauses, clauseBox, literalPos);

	for(auto ele:clauseBox)
		PrintAll(ele);

	return 0;
}
