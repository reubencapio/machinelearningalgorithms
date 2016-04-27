#include <random>
#include <functional>
#include <vector>
#include <iostream>
#include <map>

double createRandom(const double min, const double max)
{
	double result = 0;
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> distribution(min, max);
	auto rand = std::bind(distribution, generator);
	result = rand();
	return result;
}

int onemax(std::vector<std::string> &bitstring)
{
	int sum = 0;
	for (auto bit : bitstring) {
		if (bit == "1") {
			sum += 1;
		}
	}
	return sum;
}

void random_bitstring(int num_bits, std::vector<std::string> &bitstring)
{
	const double min = 0.000000000000000001;
	const double max = 0.999999999999999999;
	for (int i = 0; i < num_bits; i++) {
		createRandom(min, max) < 0.5 ? bitstring.push_back("1") : bitstring.push_back("0");
	}
}


std::vector<std::string> binary_tournament(const std::vector<int> &population_fitness, const std::vector<std::vector<std::string>> population_bitstring)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int> distribution(population_fitness.size());
	auto rand = std::bind(distribution, generator);
	int i = rand();
	int j = rand();
	while (j == i) {
		j = rand();
		if (j != i) {
			break;
		}
	}
	return (population_fitness[i] > population_fitness[j]) ? population_bitstring[i] : population_bitstring[j];
}

void point_mutation(const std::vector<std::string> &bitstring, std::vector<std::string> &child, double rate)
{
	//double rate = 1 / bitstring.size();
	const double min = 0.000000000000000001;
	const double max = 0.999999999999999999;
	for (int i = 0; i < bitstring.size(); i++) {
		std::string bit = bitstring[i];
		child.push_back((createRandom(min, max) < rate) ? ((bit == "1") ? "0" : "1") : bit);
	}
}

void crossover(const std::vector<std::string> &parent1, const std::vector<std::string> &parent2, const double rate, std::vector<std::string> &child_bitstring)
{
	const double min = 0.000000000000000001;
	const double max = 0.999999999999999999;
	if (createRandom(min, max) >= rate) {
		child_bitstring = parent1;
	}
	else {
		int point = 1 + (int)createRandom(0, parent1.size() - 2);
		for (int i = 0; i < point; i++) {
			child_bitstring.push_back(parent1[i]);
		}

		for (int i = point; i < parent1.size(); i++) {
			child_bitstring.push_back(parent1[i]);
		}
	}
}


void reproduce(std::vector<std::vector<std::string>> selected, int pop_size, double p_cross, double p_mutation, std::vector<std::vector<std::string>> &children)
{
	for (int i = 0; i < selected.size(); i++) {
		std::vector<std::string> p1_bitstring = selected[i];
		std::vector<std::string> p2_bitstring = ((i % 2) == 0) ? selected[i + 1] : selected[i - 1];
		if (i == selected.size() - 1) {
			p2_bitstring = selected[0];
		}
		std::vector<std::string> start_child_bitstring;
		std::vector<std::string> end_child_bitstring;
		crossover(p1_bitstring, p2_bitstring, p_cross, start_child_bitstring);
		point_mutation(start_child_bitstring, end_child_bitstring, p_mutation);
		children.push_back(end_child_bitstring);
		if (children.size() >= pop_size) {
			break;
		}
	}
}

double search(const int max_gens, const int num_bits, const int pop_size, const double p_crossover, const double p_mutation)
{
	std::vector<std::string> population_part;
	std::vector<std::vector<std::string>> population_bitstring;
	std::vector<int> population_fitness;
	std::map<std::vector<std::string>, int> population_map;
	std::map<int, std::vector<std::string>> best;


	for (int i = 0; i < pop_size; i++) {
		random_bitstring(num_bits, population_part);
		population_bitstring.push_back(population_part);
	}

	int max_sum_of_vec = 0;
	std::vector<std::string> best_string;
	for (auto member : population_bitstring) {
		int sum_of_vec = onemax(member);
		if (sum_of_vec > max_sum_of_vec) {
			max_sum_of_vec = sum_of_vec;
			best_string = member;
		}
		population_map[member] = sum_of_vec;
		population_fitness.push_back(sum_of_vec);
	}
	best[max_sum_of_vec] = best_string;

	std::vector<std::vector<std::string>> selected;
	for (int i = 0; i < max_gens; i++) {
		for (auto pop_fitness_member : population_fitness) {
			std::vector<std::string> result = binary_tournament(population_fitness, population_bitstring);
			selected.push_back(result);
		}
		std::vector<std::vector<std::string>> children_bitstring;
		std::vector<int> children_fitness;
		std::map<std::vector<std::string>, int> children_map;
		reproduce(selected, pop_size, p_crossover, p_mutation, children_bitstring);

		for (auto child : children_bitstring) {
			int child_fitness = onemax(child);
			if (child_fitness > max_sum_of_vec) {
				max_sum_of_vec = sum_of_vec;
				best_string = member;
			}
			children_fitness.push_back(onemax(child));
			children_map[child] = child_fitness;
		}
		//TODO: sort children to get biggest value then compare to current best
		children.sort!{ | x, y | y[:fitness] <= > x[:fitness]}


		best = children.first if children.first[:fitness] >= best[:fitness]
		population = children
		puts " > gen #{gen}, best: #{best[:fitness]}, #{best[:bitstring]}"
		break if best[:fitness] == num_bits
		end
		return best
	}
}

int main()
{
	//problem configuration
	const int num_bits = 64;
	//algorithm configuration
	const int max_gens = 100;
	const int pop_size = 100;
	const double p_crossover = 0.98;
	const double p_mutation = 1.0 / num_bits;
	//execute the algorithm
	double best = search(max_gens, num_bits, pop_size, p_crossover, p_mutation);
	std::cout << "done! Solution: f= " << ; //#{best[:fitness]}, s=#{best[:bitstring]}"
}
