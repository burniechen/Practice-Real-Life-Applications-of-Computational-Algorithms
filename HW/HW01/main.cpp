#include <bits/stdc++.h>

std::vector<int> GetInput(std::string input_file)
{
	std::ifstream Input(input_file);
	if(!Input)
	{
		std::cout << "Can't read file: " << input_file << "\n";
		exit(1);
	}
	
	int tmp;
	std::vector<int> vin;

	while(Input >> tmp)
		vin.push_back(tmp);	

	Input.close();

	return vin;
}

void CellExactlyOneT(int i, int j, std::vector<std::string> &TMP, int n)
{
	int base = i*n*n + j*n;

	// At least one true
	std::string s = "";
	for(int d=1; d<=n; d++)
		s = s + std::to_string(base+d) + ' ';

	s = s + "0" + "\n";
	TMP.push_back(s);

	// At most one true
	s = "";
	for(int x=1; x<=n; x++)
	{
		for(int y=x+1; y<=n; y++)
		{
			s = s + std::to_string(-1*(base+x)) + ' ';
			s = s + std::to_string(-1*(base+y)) + ' ';
			s = s + "0" + "\n";
			TMP.push_back(s);
			s = "";
		}
	}
}

void ExactlyOneTfromPool(std::vector<int> &idx_pool, std::vector<std::string> &TMP, int n)
{
	// At least one true
	std::string s = "";
	for(int d=1; d<=n; d++)
	{
		for(auto idx:idx_pool)
			s = s + std::to_string(idx + d) + ' ';
		
		s = s + "0" + "\n";
		TMP.push_back(s);
		s = "";
	}

	// At most one true
	s = "";
	for(int d=1; d<=n; d++)
	{
		for(int x=0; x<idx_pool.size(); x++)
		{
			for(int y=x+1; y<idx_pool.size(); y++)
			{
				s = s + std::to_string(-1*(idx_pool[x] + d)) + ' ';
				s = s + std::to_string(-1*(idx_pool[y] + d)) + ' ';
				s = s + "0" + "\n";
				TMP.push_back(s);
				s = "";
			}
		}
	}
}

void RowColExactlyOneT(int i, int j, std::vector<std::string> &TMP, int n, bool type)
{
	std::vector<int> idx_pool;
	
	// row
	if(type)
		for(int idx=0; idx<n; idx++)
			idx_pool.push_back(idx*n*n + j*n);

	// col
	else
		for(int idx=0; idx<n; idx++)
			idx_pool.push_back(i*n*n + idx*n);
	
	ExactlyOneTfromPool(idx_pool, TMP, n);
}

void BlockExactlyOneT(int i, int j, std::vector<std::string> &TMP, int n)
{
	int sqrtN = sqrt(n);
	// from upper left corner on each block
	int init_x = i, init_y = j;
	
	std::vector<int> idx_pool;

	for(int x=0; x<sqrtN; x++)
		for(int y=0; y<sqrtN; y++)
			idx_pool.push_back((init_x+x)*n*n + (init_y+y)*n);

	ExactlyOneTfromPool(idx_pool, TMP, n);
}

int main(int argc, char *argv[])
{
	if (argc<4) exit(1);

	std::string input_file = argv[1], output_file = argv[2], MiniSat = argv[3];

	std::vector<int> vin = GetInput(input_file);
	int n = (int) sqrt(vin.size());
	int sqrtN = sqrt(n);

	// 2D array
	std::vector<std::vector<int>> vin2D(n, std::vector<int> (n));
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			vin2D[i][j] = vin[i*n+j];

	// Clauses array
	std::vector<std::string> TMP;
	std::string s;
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			if(vin2D[i][j] != 0)
			{
				s = std::to_string(i*n*n + j*n + vin2D[i][j]) + ' ' + "0" + "\n";
				TMP.push_back(s);
				continue;
			}
			// Cell
			CellExactlyOneT(i, j, TMP, n);
		}
	}

	// Row & Col
	for(int fix=0; fix<n; fix++)
	{
		// row (row: [0~8], col: [fix])
		RowColExactlyOneT(0, fix, TMP, n, true);
		// col (row: [fix], col: [0~8])
		RowColExactlyOneT(fix, 0, TMP, n, false);
	}

	// Block
	for(int i=0; i<sqrtN; i++)
		for(int j=0; j<sqrtN; j++)
			BlockExactlyOneT(i*sqrtN, j*sqrtN, TMP, n);

	// Encode
	std::ofstream Encode("encode");
	Encode << 'p' << ' ' << "cnf" << ' ' << n*n*n << ' ' << TMP.size() << "\n";

	for(int i=0; i<TMP.size(); i++)
		Encode << TMP[i];

	Encode.close();

	// Run MiniSat encode out
	std::string str_run = MiniSat + ' ' + "encode" + ' ' + output_file;
	const char *run = str_run.c_str();
	system(run);

	// Delete encode
	std::string str_del = "rm encode";
	const char *del = str_del.c_str();
	system(del);
	
	// Decode
	std::ifstream Decode(output_file);
	std::string ss; 
	Decode >> ss; // Line 1
	if(ss == "UNSAT") 
	{
		std::cout << "NO" << "\n";
		return 0;
	}

	std::vector<std::vector<int>> re(n, std::vector<int>(n));
	int i=0, j=0, x;
	while(Decode >> x) // Line 2
	{
		if(x == 0) break;

		if(j == n)
		{
			i++;
			j = 0;
		}

		if(x > 0)
		{
			x = (x - 1) % n + 1;
			re[i][j] = x;
			j++;
		}
	}

	Decode.close();

	// Print
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			std::cout << re[i][j] << ' ';
		}
		std::cout << "\n";
	}

	return 0;
}
