#include <bits/stdc++.h>
using namespace std;

double f(string &s, int L, int R, double p)
{
	if(R-L == 1)
		return s[L]-'0';
	
	int sum=0;
	int tmp=0;
	for(int i=L+3; i<R; i++)
	{
		if(s[i] == '(')
			sum++;
		else if(s[i] == ')')
			sum--;
		if(sum == 0)
		{
			tmp = i+1;
			break;
		}
	}

	return (1-p)*f(s, L+3, tmp, p) + p*f(s, tmp+1, R-1, p);
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
		cout << fixed << setprecision(6) << f(s, 0, s.size(), p) << endl;
	}
}
