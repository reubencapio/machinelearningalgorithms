#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <random>
//srand, rand 
#include <stdlib.h>     
#include <time.h> 
#include <string>


//count all the string "1" inside the given vector
int onemax(const std::vector<std::string> &candidate_vector)
{
	int sum = 0;
	for (auto m : candidate_vector) {
		if (0 == m.compare("1")) {
			sum += 1;
		}
	}
	return sum;
}

//change random values in the vector then sum them up to see if it would result to a more optimized value set
void random_neighbor(const std::vector<std::string> &candidate_vector, std::vector<std::string> &mutant)
{
	mutant = candidate_vector;
	std::random_device rd;
	std::default_random_engine generator(rd());
	//0.1 and 0.9 used to generate random numbers just between those 2 decimals
	//based on c++11 style
	std::uniform_int_distribution<int> distribution(0, candidate_vector.size() - 1);
	int pos = distribution(generator);
	std::cout << "pos: " << pos << "\n";
	mutant[pos] = (mutant[pos] == "1") ? "0" : "1";
}


//create a vector with random "1" and "0" strings
void random_bitstring(const int num_bits, std::vector<std::string> &candidate_vector)
{
	for (int i = 0; i < num_bits; i++) {
		std::random_device rd;
		std::default_random_engine generator(rd());
		//0.0 and 1.0 used to generate random numbers just between those 2 numbers
		//based on c++11 style
		std::uniform_real_distribution<double> distribution(0.0, 1.0);
		double random_num = distribution(generator);
		candidate_vector.push_back((random_num < 0.5 ? "1" : "0"));
	}
}



double search(const int max_iterations, const int num_bits)
{
	std::vector<std::string> candidate_vector;
	random_bitstring(num_bits, candidate_vector);
	std::cout << "candidate_vector start: ";
	for (auto mv : candidate_vector) {
		std::cout << mv << " ";
	}
	std::cout << "\n";
	int candidate_cost = onemax(candidate_vector);
	std::cout << "candidate_cost " << candidate_cost << "\n";
	for (int i = 0; i < max_iterations; i++) {
		const std::vector<std::vector<int>> neighbor;
		std::vector<std::string> neighbor_vector;
		random_neighbor(candidate_vector, neighbor_vector);
		int neighbor_cost = onemax(neighbor_vector);
		std::cout << "neighbor_vector: ";
		for (auto mv : neighbor_vector) {
			std::cout << mv << " ";
		}
		std::cout << "\n";
		if (neighbor_cost >= candidate_cost) {
			candidate_cost = neighbor_cost;
			candidate_vector.assign(neighbor_vector.begin(), neighbor_vector.end());
		}
		std::cout << "neighbor_cost: " << neighbor_cost << "\n";
		for (auto mv : neighbor_vector) {
			std::cout << mv << " ";
		}
		std::cout << "iteration : " << i << "  best= " << candidate_cost << "\n";
		if (candidate_cost == num_bits) {
			break;
		}
	}
	std::cout << "candidate_vector end: ";
	for (auto mv : candidate_vector) {
		std::cout << mv << " ";
	}
	std::cout << "\n";
	return candidate_cost;
}


int main()
{
	//problem configuration
	const int num_bits = 64;
	//algorithm configuration
	const int max_iterations = 1000;
	//execute the algorithm
	double best = search(max_iterations, num_bits);
}
