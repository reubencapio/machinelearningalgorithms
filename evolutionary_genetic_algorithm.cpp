#include <random>
#include <functional>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <string>

//create and return a random number between min and max inputs
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

//count all "1"s in a string vector
//return sum
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

//create a string vector fille with random "1"s and "0"s
void random_bitstring(int num_bits, std::vector<std::string> &bitstring)
{
	const double min = 0.000000000000000001;
	const double max = 0.999999999999999999;
	for (int i = 0; i < num_bits; i++) {
		createRandom(min, max) < 0.5 ? bitstring.push_back("1") : bitstring.push_back("0");
	}
}

//access 2 random places from a vector(i and j), if "i" is bigger, 
//return bistring vector member "i" else return "j"
std::vector<std::string> binary_tournament(const std::vector<int> &population_fitness, const std::vector<std::vector<std::string>> population_bitstring)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int> distribution(population_fitness.size());
	auto rand = std::bind(distribution, generator);
	int i = (int)createRandom(0, (int)population_fitness.size());
	int j = (int)createRandom(0, (int)population_fitness.size());
	while (j == i) {
		j = rand();
		if (j != i) {
			break;
		}
	}
	return (population_fitness[i] > population_fitness[j]) ? population_bitstring[i] : population_bitstring[j];
}

//create a child string vector based on random value generator and input bitstring
//this serves as mutation function
void point_mutation(const std::vector<std::string> &bitstring, std::vector<std::string> &child, double rate)
{
	//double rate = 1 / bitstring.size();
	const double min = 0.000000000000000001;
	const double max = 0.999999999999999999;
	for (int i = 0; i < (int)bitstring.size(); i++) {
		std::string bit = bitstring[i];
		child.push_back((createRandom(min, max) < rate) ? ((bit == "1") ? "0" : "1") : bit);
	}
}

//return a vector of string created from a mix of both parent string vectors if random is lesser than rate
//else return parent1 input vector
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

		for (int i = point; i < (int)parent1.size(); i++) {
			child_bitstring.push_back(parent1[i]);
		}
	}
}

//create a vector of vector of strings(children) from output of reproduction of both parents and mutations to the children
void reproduce(std::vector<std::vector<std::string>> selected, int pop_size, double p_cross, double p_mutation, std::vector<std::vector<std::string>> &children)
{
	for (int i = 0; i < (int)selected.size(); i++) {
		std::vector<std::string> p1_bitstring = selected[i];
		std::vector<std::string> p2_bitstring;
		if (i == selected.size() - 1) {
			p2_bitstring = selected[0];
		}else{
			p2_bitstring = ((i % 2) == 0) ? selected[i + 1] : selected[i - 1];
		}
		std::vector<std::string> start_child_bitstring;
		std::vector<std::string> end_child_bitstring;
		//return a vector of string(start_child_bitstring) created from a mix of both parent 
		//string vectors if random is lesser than rate(p_cross) else return parent1 input vector
		crossover(p1_bitstring, p2_bitstring, p_cross, start_child_bitstring);
		//create a child string vector(end_child_bitstring) based on random value generator 
		//and input bitstring(start_child_bitstring) this serves as mutation function
		point_mutation(start_child_bitstring, end_child_bitstring, p_mutation);
		children.push_back(end_child_bitstring);
		if ((int)children.size() >= pop_size) {
			break;
		}
	}
}

double search(const int max_gens, const int num_bits, const int pop_size, const double p_crossover, const double p_mutation)
{
	std::vector<std::string> population_part;
	std::vector<std::vector<std::string>> population_bitstring;
	std::vector<int> population_fitness;
	std::map<int, std::vector<std::string>> population_map;
	std::map<int, std::vector<std::string>> best_map;
	std::vector<std::vector<std::string>> sorted_population_bitstring;

	//create random bitstring and push them in population_bitstring vector
	for (int i = 0; i < pop_size; i++) {
		random_bitstring(num_bits, population_part);
		population_bitstring.push_back(population_part);
	}

	int max_sum_of_vec = 0;
	std::vector<std::string> best_string;
	//for each population_bitstring vector member, sum the bitstring members
	//save the member with the highest sum as best(best_string)
	for (auto member : population_bitstring) {
		int sum_of_vec = onemax(member);
		if (sum_of_vec > max_sum_of_vec) {
			max_sum_of_vec = sum_of_vec;
			best_string = member;
		}
		population_map[sum_of_vec] = member;
		population_fitness.push_back(sum_of_vec);
	}
	//save the best string among the population with its sum
	best_map[max_sum_of_vec] = best_string;
	//descending sort the population_fitness
	std::sort(population_fitness.rbegin(), population_fitness.rend());   

	//sort population_bitstring based on fitness 
	//save values in sorted_population_bitstring
	for (auto val : population_fitness) {
		std::vector<std::string> best_string = population_map[val];
		sorted_population_bitstring.push_back(best_string);
	}


	std::vector<std::vector<std::string>> selected;
	for (int i = 0; i < max_gens; i++) {
		for (auto pop_fitness_member : population_fitness) {
			//access 2 random places(i and j) from an int vector(population_fitness), if "i" is bigger, 
			//return population_bitstring vector member "i" else return "j"
			std::vector<std::string> result = binary_tournament(population_fitness, population_bitstring);
			selected.push_back(result);
		}
		std::vector<std::vector<std::string>> children_bitstring;
		std::vector<std::vector<std::string>> sorted_children_bitstring;
		std::vector<int> children_fitness;
		std::map<int, std::vector<std::string>> children_map;

		//create a vector of vector of strings(children_bitstring) from 
		//output of reproduction of both parents(selected[i] and selected[i+/-1) and mutations to the children
		//reproduce function uses point_mutation and crossover function
		reproduce(selected, pop_size, p_crossover, p_mutation, children_bitstring);


		for (auto child : children_bitstring) {
			int child_fitness = onemax(child);
			if (child_fitness > max_sum_of_vec) {
				max_sum_of_vec = child_fitness;
				best_string = child;
			}
			children_fitness.push_back(onemax(child));
			best_map[child_fitness] = child;
		}
		//TODO: sort children to get biggest value then compare to current best

		std::sort(children_fitness.rbegin(), children_fitness.rend());   // descending sort


		for (auto val : children_fitness) {
			std::vector<std::string> child_string = best_map[val];
			sorted_children_bitstring.push_back(child_string);
		}

		if (children_fitness[0] > max_sum_of_vec) {
			best_string = sorted_children_bitstring[0];
		}

		population_bitstring = sorted_children_bitstring;
		std::cout << " > gen # " << i << " best: # " << max_sum_of_vec << " << ";
		for (auto m : best_string){
			std::cout << m;
		}
		std::cout << "\n";

		if (max_sum_of_vec == num_bits){
			break;
		}
		return max_sum_of_vec;
	}
	return max_sum_of_vec; //bug?
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
	std::cout << "done! Solution: f= " << best << " \n";// s=#{best[:bitstring]}"
}
