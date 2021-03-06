#include <vector>
#include <iostream>
#include <random>
#include <math.h>
#include <algorithm>
#include <functional>

double rand_in_bounds(const double &min, const double &max)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	//0.1 and 0.9 used to generate random numbers just between those 2 decimals
	//based on c++11 style
	std::uniform_real_distribution<double> distribution(0.1, 0.9);
	double random_num = distribution(generator);
	return min + ((max - min) * random_num);
}

//get pythagorean distance between 2 cities
int euc_2d(const std::vector<int> &c1, const std::vector<int> &c2)
{
	//std::cout << "euc_2d() " << "\n";
	int result = (int)round(sqrt(pow((c1[0] - c2[0]), 2) + pow((c1[1] - c2[1]), 2)));
	return result;
}


//add up all the distance between random cities(permutation is used as index)
int cost(const std::vector<int> &permutation, const std::vector<std::vector<int>> &cities)
{
	//std::cout << "cost() " << "\n";
	int distance = 0;
	for (int i = 0; i < (int)permutation.size(); i++) {
		int c1 = permutation[i];
		int c2 = (permutation.size() - 1 == i) ? permutation[0] : permutation[i + 1];
		distance += euc_2d(cities[c1], cities[c2]);
	}
	return distance;
}


//create a vector of random numbers from 0 to size of cities
void random_permutation(const std::vector<std::vector<int>> &cities, std::vector<int> &perm)
{
	//std::cout << "random_permutation() " << "\n";
	for (int i = 0; i < (int)cities.size(); i++) {
		perm.push_back(i);
	}
	for (int i = 0; i < (int)perm.size(); i++) {
		std::random_device rd;
		std::default_random_engine generator(rd());
		std::uniform_int_distribution<int> distribution(0, (perm.size() - 1) - i);
		int r = (distribution(generator) + i);
		perm[r] = perm[i];
		perm[i] = perm[r];
	}
}


//reverse certain parts of best vector(permutation) from a certain random point to another random point
void stochastic_two_opt(const std::vector<int> &permutation, std::vector<int> &perm)
{
	//std::cout << "stochastic_two_opt " << "\n";
	perm = permutation;
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int> distribution(0, perm.size());
	//rand_num is a function to generate
	auto rand_num = std::bind(distribution, generator);
	int c1 = rand_num();
	int c2 = rand_num();
	std::vector<int> exclude;
	exclude.push_back(c1);
	exclude.push_back((c1 == 0) ? perm.size() - 1 : c1 - 1);
	exclude.push_back((c1 == perm.size() - 1) ? 0 : c1 + 1);
	if (std::find(exclude.begin(), exclude.end(), c2) != exclude.end()) {
		c2 = rand_num();
	}
	if (c2 < c1) {
		std::swap(c1, c2);
	}
	//reverse vector values in a certain range
	std::reverse(perm.begin() + c1, perm.begin() + c2);
}



int create_neighbor(const std::vector<int> &current_vector, const std::vector<std::vector<int>> &cities, std::vector<int> &candidate_vector_modified)
{
	std::vector<int> candidate_vector;
	int candidate_cost;
	candidate_vector = current_vector;
	stochastic_two_opt(candidate_vector, candidate_vector_modified);
	candidate_cost = cost(candidate_vector_modified, cities);
	return candidate_cost;
}

bool should_accept(const int candidate_cost, const int current_cost, const int &temp)
{
	if (candidate_cost <= current_cost) {
		return true;
	}
	return (exp(current_cost - candidate_cost) / temp) > rand_in_bounds(0.1, 0.9);
}


int search(const std::vector<std::vector<int>> cities, const int max_iter, const double max_temp, const double temp_change, std::vector<int> &best_vector)
{

	std::vector<int> current_vector;
	int current_cost = 0;
	int best_cost = 0;
	random_permutation(cities, current_vector);
	current_cost = cost(current_vector, cities);
	double temp = max_temp;
	best_cost = current_cost;
	std::vector<int> candidate_vector;

	for (int i = 0; i < max_iter; i++) {
		int candidate_cost = create_neighbor(current_vector, cities, candidate_vector);
		temp = temp * temp_change;
		bool isAccepted = should_accept(candidate_cost, current_cost, temp);
		if (isAccepted) {
			current_cost = candidate_cost;
			current_vector = candidate_vector;
		}
		if (candidate_cost < best_cost) {
			best_cost = candidate_cost;
			best_vector = candidate_vector;
		}
		if ( (i + 1) % (10) == 0 ) {
			std::cout << " > iteration # " << i + 1 << " temp=# " << temp  <<  " , best=# " << best_cost << " \n";
		}
	}
	return best_cost;
}

int main()
{
	//problem configuration
	const std::vector<std::vector<int>> berlin52 = { { 565, 575 },{ 25, 185 },{ 345, 750 },{ 945, 685 },{ 845, 655 },
	{ 880, 660 },{ 25, 230 },{ 525, 1000 },{ 580, 1175 },{ 650, 1130 },{ 1605, 620 },
	{ 1220, 580 },{ 1465, 200 },{ 1530, 5 },{ 845, 680 },{ 725, 370 },{ 145, 665 },
	{ 415, 635 },{ 510, 875 },{ 560, 365 },{ 300, 465 },{ 520, 585 },{ 480, 415 },
	{ 835, 625 },{ 975, 580 },{ 1215, 245 },{ 1320, 315 },{ 1250, 400 },{ 660, 180 },
	{ 410, 250 },{ 420, 555 },{ 575, 665 },{ 1150, 1160 },{ 700, 580 },{ 685, 595 },
	{ 685, 610 },{ 770, 610 },{ 795, 645 },{ 720, 635 },{ 760, 650 },{ 475, 960 },
	{ 95, 260 },{ 875, 920 },{ 700, 500 },{ 555, 815 },{ 830, 485 },{ 1170, 65 },
	{ 830, 610 },{ 605, 625 },{ 595, 360 },{ 1340, 725 },{ 1740, 245 } };
	//algorithm configuration
	const int max_iterations = 2000;
	const double max_temp = 100000.0;
	const double temp_change = 0.98;
	//execute the algorithm
	std::vector<int> best_vector;
	int best_cost = search(berlin52, max_iterations, max_temp, temp_change, best_vector);
	std::cout << "Done. Best Solution: c= " << best_cost << " best_vector: ";
	for (auto m : best_vector) {
		std::cout << m << " ";
	}
	std::cout << "\n";
}
