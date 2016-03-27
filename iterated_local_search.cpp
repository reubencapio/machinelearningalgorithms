#include <vector>
#include <iostream>
#include <random>
#include <math.h>
#include <algorithm>
#include <functional>


int euc_2d(const std::vector<int> &c1, const std::vector<int> &c2)
{
	int result = round(sqrt(pow((c1[0] - c2[0]), 2) + pow((c1[1] - c2[1]), 2)));
	return result;
}

int cost(std::vector<int> &permutation, const std::vector<std::vector<int>> &cities)
{
	int distance = 0;
	for (int i = 0; i < permutation.size(); i++){
		int c1 = permutation[i];
		int c2 = (permutation.size() - 1 == i) ? permutation[0] : permutation[i + 1];
		distance += euc_2d(cities[c1], cities[c2]);
	}
	return distance;
}

void random_permutation(const std::vector<std::vector<int>> &cities, std::vector<int> &perm)
{
	for (int i = 0; i < cities.size(); i++){
		perm.push_back(i);
	}
	for (int i = 0; i < perm.size(); i++){

		std::random_device rd;
		std::default_random_engine generator(rd());
		std::uniform_int_distribution<int> distribution(0, perm.size() - i);
		int r = (distribution(generator) + i);
		perm[r] = perm[i];
		perm[i] = perm[r];
	}
}

void stochastic_two_opt(std::vector<int> &perm, const std::vector<int> &permutation)
{
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
	if (c2 < c1){
		std::swap(c1, c2);
	}
	for (int i = c1; i < c2; i++){
		perm[i] = perm[c2 - i];
	}
}

int local_search(const std::vector<int> &best_vector,  int best_cost, const std::vector<std::vector<int>> &cities, const int max_no_improv)
{
	int count = 0;
	while (count >= max_no_improv){
		std::vector<int> candidate_vector;
		stochastic_two_opt(candidate_vector, best_vector);
		int candidate_cost = cost(candidate_vector, cities);
		count = (candidate_cost < best_cost ? 0 : count + 1);
		if (candidate_cost < best_cost){
			best_cost = candidate_cost;
		}
	 }
	return best_cost;
}

int double_bridge_move(std::vector<int> &perm, std::vector<int> &candidate_vector)
{
	int p1 = 0;
	int p2 = 0;
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int> distribution(0, perm.size() / 4);
	//rand_num is a function to generate
	auto rand_num = std::bind(distribution, generator);
	int pos1 = (rand_num()) + 1;
	int pos2 = pos1 + 1 + rand_num();
	int pos3 = pos2 + 1 + rand_num();
	for (int i = 0; i < pos1; i++){
		p1 += perm[i];
	}
	for (int i = pos3; i < perm.size(); i++){
		p1 += perm[i];
	}
	for (int i = pos2; i < pos3; i++){
		p2 += perm[i];
	}
	for (int i = pos1; i < pos2; i++){
		p2 += perm[i];
	}
	return p1 + p2;
}

int perturbation(const std::vector<std::vector<int>> &cities, int best_cost, std::vector<int> &best_vector, std::vector<int> &candidate_vector)
{
	double_bridge_move(best_vector, candidate_vector);
	int candidate_cost = cost(candidate_vector, cities);
	return candidate_cost;
}

int search(const std::vector<std::vector<int>> &cities, const int max_iterations, const int max_no_improv, std::vector<int> &best_vector)
{
	random_permutation(cities, best_vector);
	int best_cost = cost(best_vector, cities);
	int best = local_search(best_vector, best_cost, cities, max_no_improv);
	for (int i = 0; i < max_iterations; i++){
		std::vector<int> candidate_vector;
		int candidate_cost = perturbation(cities, best_cost, best_vector, candidate_vector);
		candidate_cost = local_search(candidate_vector, candidate_cost, cities, max_no_improv);
		if (candidate_cost < best_cost){
			best_cost = candidate_cost;
			best_vector.assign(candidate_vector.begin(), candidate_vector.end());
		}
		std::cout << " > iteration #" << i + 1 << "best=#" << best_cost << "\n";
	}
	return best;
}


int main()
{
	//problem configuration
	const std::vector<std::vector<int>> berlin52 = { { 565, 575 }, { 25, 185 }, { 345, 750 }, { 945, 685 }, { 845, 655 },
	{ 880, 660 }, { 25, 230 }, { 525, 1000 }, { 580, 1175 }, { 650, 1130 }, { 1605, 620 },
	{ 1220, 580 }, { 1465, 200 }, { 1530, 5 }, { 845, 680 }, { 725, 370 }, { 145, 665 },
	{ 415, 635 }, { 510, 875 }, { 560, 365 }, { 300, 465 }, { 520, 585 }, { 480, 415 },
	{ 835, 625 }, { 975, 580 }, { 1215, 245 }, { 1320, 315 }, { 1250, 400 }, { 660, 180 },
	{ 410, 250 }, { 420, 555 }, { 575, 665 }, { 1150, 1160 }, { 700, 580 }, { 685, 595 },
	{ 685, 610 }, { 770, 610 }, { 795, 645 }, { 720, 635 }, { 760, 650 }, { 475, 960 },
	{ 95, 260 }, { 875, 920 }, { 700, 500 }, { 555, 815 }, { 830, 485 }, { 1170, 65 },
	{ 830, 610 }, { 605, 625 }, { 595, 360 }, { 1340, 725 }, { 1740, 245 } };
	//algorithm configuration
	const int max_iterations = 100;
	const int max_no_improv = 50;
	//execute the algorithm
	std::vector<int> best_vector;
	int best = search(berlin52, max_iterations, max_no_improv, best_vector);
	std::cout << "Done. Best Solution: c= " << best << " best_vector: ";
	for (auto m : best_vector){ 
		std::cout << m << " ";
	}
	std::cout << "\n";
}
