#include <bits/stdc++.h>

using namespace std;

void in(double p, string tree)
{
	double sum=0;
	int L=0, R=0;
	bool state=true;
	for(int i=0; i<tree.size(); i++)
	{
		if(tree[i] == '(')
		{
			L += 1;
			if(tree[i+3]-'0' == 0 or tree[i+3]-'0' == 1)
				state = true;
		}
		else if(tree[i] == ')')
			L -= 1;
		else if(tree[i]-'0' == 1)
		{
			sum += pow(1-p, L)*pow(p, R);
		//	if(state) cout << "curr state: left\n";
		//	else cout << "curr state: right\n";
		//	cout << L << " , " << R << endl;
		//	cout << pow(1-p, L)*pow(p, R) << " <-1: value\n";
			if(state)
				L -= 1, R += 1;
			state = !state;
		}
		else if(tree[i]-'0' == 0)
		{
		//	if(state) cout << "curr state: left\n";
		//	else cout << "curr state: right\n";
		//	cout << L << " , " << R << endl;
		//	cout << pow(1-p, L)*pow(p, R) << " <-0: value\n";
			if(state)
				L -= 1, R += 1;
			state = !state;
		}
		else continue;
	}
	cout << fixed << setprecision(6) <<  sum << endl;
}

int main()
{
	string s;
	while(cin >> s)
	{
		double p = atof(s.c_str());
		cin >> s;
		if(p == 0 and s == "0")
			return 0;
		in(p, s);
	}
	return 0;
}
