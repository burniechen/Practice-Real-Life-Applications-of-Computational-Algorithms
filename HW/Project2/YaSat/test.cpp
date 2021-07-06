#include <bits/stdc++.h>

int main()
{
	float a = 1.0 / pow(2, 2); // 0.25
	std::vector<int> v = {1, 2, 3, 4, 5};
	auto it = std::find(v.begin(), v.end(), 2);
	if(it != v.end())
		v.erase(it);
	for(auto ele:v)
		std::cout << ele << ' ';
	
	return 0;
}
