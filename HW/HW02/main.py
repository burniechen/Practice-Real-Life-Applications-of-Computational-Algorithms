import math
import argparse
from pyeda.inter import *

def getDigit(n):
	tmp = ''
	for i in range(1, n+1):
		tmp += str(i)
	
	return tmp

def getGrid(data):
	data2D = []
	for ele in data:
		data2D.append(ele.split())	

	grid = ''
	for i in range(len(data2D)):
		for j in range(len(data2D)):
			if data2D[i][j] == '0':
				grid+='.'
				continue
			grid+=data2D[i][j]

	return grid

def get_val(point, r, c):
	for v in range(1, n+1):
		if point[X[r, c, v]]:
			return DIGITS[v-1]
	return "X"

def display(point, f):
	print(point)
	f.write(str(point))

def parse_grid(grid, n):
	chars = [c for c in grid if c in DIGITS or c in "0."]
	assert len(chars) == n**2
	return And(*[ X[i//n+1, i%n+1, int(c)]
			for i, c in enumerate(chars) if c in DIGITS ])

def solve(grid, S, n):
	S = And(S, parse_grid(grid, n))
	return S.satisfy_count()

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument("inputFile", type=str)
	parser.add_argument("outputFile", type=str)
	args = parser.parse_args()
	
	return args.inputFile, args.outputFile

if __name__ == "__main__":

	inputFile, outputFile = main()

	f = open(inputFile)
	data = f.readlines()
	f.close

	n = len(data)
	sq_n = int(math.sqrt(n))
	DIGITS = getDigit(n)

	X = exprvars('x', (1, n+1), (1, n+1), (1, n+1))
	
	# Value Vonstraints
	V = And(*[
			And(*[
				OneHot(*[ X[r, c, v]
					for v in range(1, n+1) ])
				for c in range(1, n+1) ])
			for r in range(1, n+1) ])
	
	# Row and Column Constraints
	R = And(*[
			And(*[
				OneHot(*[ X[r, c, v]
					for c in range(1, n+1) ])
				for v in range(1, n+1) ])
			for r in range(1, n+1) ])
	
	C = And(*[
			And(*[
				OneHot(*[ X[r, c, v]
					for r in range(1, n+1) ])
				for v in range(1, n+1) ])
			for c in range(1, n+1) ])
	
	# Box Constraints
	B = And(*[
			And(*[
				OneHot(*[ X[sq_n*br+r, sq_n*bc+c, v]
					for r in range(1, sq_n+1) for c in range(1, sq_n+1) ])
				for v in range(1, n+1) ])
			for br in range(sq_n) for bc in range(sq_n) ])
	
	S = And(V, R, C, B)
	
#	grid = ( ".73|...|8.."
#			 "..4|13.|.5."
#			 ".85|..6|31."
#			 "---+---+---"
#			 "5..|.9.|.3."
#			 "..8|.1.|5.."
#			 ".1.|.6.|..7"
#			 "---+---+---"
#			 ".51|6..|28."
#			 ".4.|.52|9.."
#			 ".42|...|64." )
#	grid = ( "..|1."
#			 "..|.."
#			 "--+--"
#			 "3.|2."
#			 "..|..")

	grid = getGrid(data)
	f = open(outputFile, 'w')
	display(solve(grid, S, n), f)
	f.close()
