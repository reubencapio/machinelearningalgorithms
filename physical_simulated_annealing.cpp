#include <vector>
#include <iostream>
#include <random>
#include <math.h>
#include <algorithm>
#include <functional>

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
void stochastic_two_opt(std::vector<int> &perm, const std::vector<int> &permutation)
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
	for (auto m : perm) {
		std::cout << m << " ";
	}
	std::reverse(perm.begin() + c1, perm.begin() + c2);
}



def create_neighbor(current, cities)
  candidate = {}
  candidate[:vector] = Array.new(current[:vector])
  stochastic_two_opt!(candidate[:vector])
  candidate[:cost] = cost(candidate[:vector], cities)
  return candidate
end

def should_accept?(candidate, current, temp)
  return true if candidate[:cost] <= current[:cost]
  return Math.exp((current[:cost] - candidate[:cost]) / temp) > rand()
end

def search(cities, max_iter, max_temp, temp_change)
  current = {:vector=>random_permutation(cities)}
  current[:cost] = cost(current[:vector], cities)
  temp, best = max_temp, current
  max_iter.times do |iter|
    candidate = create_neighbor(current, cities)
    temp = temp * temp_change
    current = candidate if should_accept?(candidate, current, temp)
    best = candidate if candidate[:cost] < best[:cost]
    if (iter+1).modulo(10) == 0
      puts " > iteration #{(iter+1)}, temp=#{temp}, best=#{best[:cost]}"
    end
  end
  return best
end

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
	const double temp_change = 0.98
	//execute the algorithm
	std::vector<int> best_vector;
	int best_cost = search(berlin52, max_iterations, max_temp, temp_change, best_vector);
	std::cout << "Done. Best Solution: c= " << best_cost << " best_vector: ";
	for (auto m : best_vector) {
		std::cout << m << " ";
	}
	std::cout << "\n";
}
