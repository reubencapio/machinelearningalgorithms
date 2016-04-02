#include <iostream>
#include<math.h>
#include <stdlib.h> 
#include <vector>
#include <map>
#include <random>
#include <functional>

//return a vector(with size same as that of problem_size) with random numbers generated using search_space members as basis
void random_vector(const std::vector<std::vector<double>> &minmax, std::vector<double> &array_new)
{
	for (int i = 0; i < (int)minmax.size(); i++){
		std::cout << rand() << "\n";
		std::default_random_engine generator;
		std::uniform_real_distribution<double> distribution(0.000000000000001, 0.999999999999999);
		auto rand = std::bind(distribution, generator);
		array_new.push_back(minmax[i][0] + ((minmax[i][1] - minmax[i][0]) * rand()));
	}
}


void initialize_weights(int problem_size, std::vector<std::vector<double>> &minmax, std::vector<double> &array_new)
{
	std::vector<double> in_minmax = { -1.0, 1.0 };
	for (int i = 0; i < problem_size + 1; i++){
		minmax.push_back(in_minmax);
	}
	random_vector(minmax, array_new);
}

void update_weights(int num_inputs, std::vector<double> &weights, std::vector<double> &input, double out_exp,
	double out_act, double l_rate)
{
	for (int i = 0; i < num_inputs; i++){
		weights[i] += l_rate * (out_exp - out_act) * input[i];
	}
	weights[num_inputs] += l_rate * (out_exp - out_act) * 1.0;
}

double activate(std::vector<double> &weights, std::vector<double> &a_vector)
{
	double sum = weights[weights.size() - 1] * 1.0;
	for (int i = 0; i < (int)a_vector.size(); i++){
		sum += weights[i] * a_vector[i];
	}
	return sum;
}



double transfer(double activation)
{
	return (activation >= 0) ? 1.0 : 0.0;
}

double get_output(std::vector<double> &weights, std::vector<double> &a_vector)
{
	double activation = activate(weights, a_vector);
	return transfer(activation);
}

void train_weights(std::vector<double> &weights, std::vector<std::vector<double>> &domain, const int num_inputs, 
	              const int iterations, const double lrate)
{
	for (int i = 0; i < iterations; i++){
		double error = 0.0;
		for (auto pattern : domain){
			std::vector<double> input;
			for (int j = 0; j < num_inputs; j++){
				input.push_back(pattern[j]);
			}
			double output = get_output(weights, input);
			double expected = pattern.back();
			error += output - expected;
			update_weights(num_inputs, weights, input, expected, output, lrate);
		}
	}
}


int main()
{
	const int inputs = 2;
	std::vector<std::vector<double>> minmax;
	std::vector<double> array_new;
	double v2 = ((double)rand()) / ((double)RAND_MAX) * 0.9999999999999999 + 0.000000000000001;
	std::cout << "v2 = " << v2 << "\n";
	initialize_weights(inputs, minmax, array_new);
}
