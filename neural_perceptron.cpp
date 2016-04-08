#include <iostream>
#include <cmath>
#include <stdlib.h> 
#include <vector>
#include <map>
#include <random>
#include <functional>

//return a vector(with size same as that of problem_size) with random numbers generated using minmax members as basis
void random_vector(const std::vector<std::vector<double>> &minmax, std::vector<double> &array_new)
{
	  // Seed with a real random value, if available
        std::random_device rd;
        std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> distribution(0.000000000000001, 0.999999999999999);
	auto rand = std::bind(distribution, generator);
	for (int i = 0; i < (int)minmax.size(); i++){
		array_new.push_back(minmax[i][0] + ((minmax[i][1] - minmax[i][0]) * rand()));
	}
}


//intialize a 2 dimension vector with -1.0 and 1 then feed it to random_vector() function
void initialize_weights(const int problem_size, std::vector<double> &array_new)
{
	std::vector<std::vector<double>> minmax;
	std::vector<double> in_minmax = { -1.0, 1.0 };
	for (int i = 0; i < problem_size + 1; i++){
		minmax.push_back(in_minmax);
	}
	random_vector(minmax, array_new);
}

//update wights based on input vector, expected value, output value and learning rate, called from train_weights
void update_weights(const int num_inputs, std::vector<double> &weights, const std::vector<double> &input, const double out_exp,
	const double out_act, const double l_rate)
{
	for (int i = 0; i < num_inputs; i++){
		double temp = weights[i] + l_rate * (out_exp - out_act) * input[i];
		//bug? wights[0] does not seem to change value
		weights[i] += l_rate * (out_exp - out_act) * input[i];
	}
	weights[num_inputs] += l_rate * (out_exp - out_act) * 1.0;
}

//sum up product of weight and 2 member from domain 2d vector(a_vector)
double activate(const std::vector<double> &weights, const std::vector<double> &a_vector)
{
	double sum = weights[weights.size() - 1] * 1.0;
	for (int i = 0; i < (int)a_vector.size(); i++){
		sum += weights[i] * a_vector[i];
	}
	return sum;
}


//return 1 or 0 based on value of activation
double transfer(const double activation)
{
	//std::cout << "activation: " << activation << "\n";
	return (activation >= 0) ? 1.0 : 0.0;
}

//sum up product of weight and 2 member from domain 2d vector(a_vector)
//then return 1 or 0 based on value of activation
double get_output(const std::vector<double> &weights, const  std::vector<double> &a_vector)
{
	double activation = activate(weights, a_vector);
	return transfer(activation);
}

void train_weights(std::vector<double> &weights, const std::vector<std::vector<double>> &domain, const int num_inputs,
	const int iterations, const double lrate)
{
	//iterations is 20
	for (int i = 0; i < iterations; i++){
		double error = 0.0;
		//domain is a 2 dimensional vector
		//pattern contains 3 members
		for (auto pattern : domain){
			std::vector<double> input;
			//num_inputs is 2
			//get first 2 members of pattern array then push them inside input vector
			for (int j = 0; j < num_inputs; j++){
				input.push_back(pattern[j]);
			}
			//sum up product of weight and 2 member from domain 2d vector(a_vector)
			//then return 1 or 0 based on value of activation
			double output = get_output(weights, input);
			//get last member of pattern array
			double expected = pattern.back();
			error += abs(output - expected);
			//update wights based on input vector, expected value, output value and learning rate
			update_weights(num_inputs, weights, input, expected, output, lrate);
		}
		std::cout << "epoch: " << i << ", error: " << error << "\n";
	}
}

//test weights after training them
int test_weights(const std::vector<double> &weights, const std::vector<std::vector<double>> domain, const int num_inputs)
{
	int correct = 0;
	for (auto pattern : domain){
		std::vector<double> input_vector;
		for (int j = 0; j < num_inputs; j++){
			input_vector.push_back(pattern[j]);
		}
		double output = get_output(weights, input_vector);
		if ((int)output == pattern.back()){
			correct += 1;

		}
	}
	std::cout << "Finished test with a score of: " << correct << "/" << domain.size() << "\n";
	return correct;
}

void execute(const std::vector<std::vector<double>> domain, const int num_inputs, const int iterations, const double learning_rate)
{
	std::vector<double> weights;
	//intialize a 2 dimension vector with -1.0 and 1 then feed it to random_vector() function, return it as weights
	initialize_weights(num_inputs, weights);
	train_weights(weights, domain, num_inputs, iterations, learning_rate);
	test_weights(weights, domain, num_inputs);
}


int main()
{
	//problem configuration
	const std::vector<std::vector<double>> or_problem = { { 0, 0, 0 }, { 0, 1, 1 }, { 1, 0, 1 }, { 1, 1, 1 } };
	const int inputs = 2;
	//algorithm configuration
	const int iterations = 20;
	const double learning_rate = 0.1;
	//execute the algorithm
	execute(or_problem, inputs, iterations, learning_rate);
}
