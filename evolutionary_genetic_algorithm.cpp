#include <random>
#include <functional>
#include <vector>
#include <iostream>
#include <map>

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
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> distribution(0.000000000000000001, 0.99999999999999999);
	auto rand = std::bind(distribution, generator);
	for (int i = 0; i < num_bits; i++) {
		//std::cout << "rand(): " << rand() << "\n";
		rand() < 0.5 ? bitstring.push_back("1") : bitstring.push_back("0");
	}
}


int binary_tournament(const std::vector<int> &population_fitness)
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
	return (population_fitness[i] > population_fitness[j]) ? population_fitness[i] : population_fitness[j];
}

void point_mutation(const std::vector<std::string> &bitstring, std::vector<std::string> &child)
{
	double rate = 1.0 / bitstring.size();
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> distribution(0.000000000000000001, 0.99999999999999999);
	auto rand = std::bind(distribution, generator);
	for (int i = 0; i < bitstring.size(); i++) {
		std::string bit = bitstring[i];
		child.push_back((rand()<rate) ? ((bit == "1") ? "0" : "1") : bit)
	}
}

void crossover(const std::vector<std::string> &parent1, const std::vector<std::string> &parent2, const double rate, std::vector<std::string> &mixed_parent)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> distribution(0.000000000000000001, 0.99999999999999999);
	auto rand = std::bind(distribution, generator);
	if(rand() >= rate){
		return parent1;
	} else {
		std::random_device rd2;
		std::default_random_engine generator(rd2());
		std::uniform_real_distribution<double> distribution2(parent1.size() - 2);
		auto rand = std::bind(distribution, generator);
		
		int point = 1 + rand();
		for(int i = 0; i < point; i++){
			mixed_parent.push_back(parent1[i]);
		}
		
		for(int i = point; i < parent1.size(); i++){
			mixed_parent.push_back(parent1[i]);
		}
	}
}

def reproduce(selected, pop_size, p_cross, p_mutation)
children = []
selected.each_with_index do | p1, i |
p2 = (i.modulo(2) == 0) ? selected[i + 1] : selected[i - 1]
p2 = selected[0] if i == selected.size - 1
child = {}
child[:bitstring] = crossover(p1[:bitstring], p2[:bitstring], p_cross)
child[:bitstring] = point_mutation(child[:bitstring], p_mutation)
children << child
break if children.size >= pop_size
end
return children
end

double search(const int max_gens, const int num_bits, const int pop_size, const double p_crossover, const double p_mutation)
{
	std::vector<std::string> population_part;
	std::vector<std::vector<std::string>> population_bitstring;
	std::vector<int> population_fitness;
	std::map<std::string, int> population_map;


	for (i = 0; i < pop_size; i++) {
		random_bitstring(num_bits, population_part);
		population_bitstring.push_back(population_part);
	}


	for (auto member : population_bitstring) {
		int sum_of_vec = onemax(member);
		population_map[member] = sum_of_vec;
		population_fitness.push_back(sum_of_vec);
	}

	best = population.sort{ | x,y | y[:fitness] <= > x[:fitness] }.first
		for (int i = 0; i<) {
			selected = Array.new(pop_size){ | i | binary_tournament(population_fitness) }
				children = reproduce(selected, pop_size, p_crossover, p_mutation)
				children.each{ | c | c[:fitness] = onemax(c[:bitstring]) }
				children.sort!{ | x, y | y[:fitness] <= > x[:fitness]}
			best = children.first if children.first[:fitness] >= best[:fitness]
				population = children
				puts " > gen #{gen}, best: #{best[:fitness]}, #{best[:bitstring]}"
				break if best[:fitness] == num_bits
				end
				return best
		}

	int main
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
