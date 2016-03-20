#include <iostream>
#include<math.h>
#include <vector>
#include <map>
#include <random>

//gist
//same as adaptive search but use either big or small step values to find solution
//so search will be more optimized, as compared to random

//returns a sum of the squared vector contents
double objective_function(const std::vector<double> &a_vector)
{
	double result = 0;
	for (auto it : a_vector){
		result = result + pow(it, 2);
	}
	return result;
}

//returns a random number based on the min and max of the bounds constants
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


//create a vector with random numbers based on the min and max seed in the given constant bounds
void random_vector(const std::vector<std::vector<double>> &minmax, std::vector<double> &new_vec, const int &problem_size)
{
	for (int i = 0; i < problem_size; i++){
		new_vec.push_back(rand_in_bounds(minmax[i][0], minmax[i][1]));
	}
}


//fill position vector with {problem_size=2} random numbers
void take_step(const std::vector<std::vector<double>> &minmax, std::vector<double> &current, const double &step_size,
	std::vector<double> &position, int problem_size)
{
	for (int i = 0; i < problem_size; i++){
		//if minmax current value(value inside bounds) is bigger than current vector value(vector generated with random numbers using bound values as seeds)
		//use minmax first value as min else use current vector value minus the step size(max bound - min bound * init factor)
		double min = minmax[i][0] > current[i] - step_size ? minmax[i][0] : current[i] - step_size;
		//use current value plus stepsize if minmax value is bigger else use minmax
		double max = minmax[i][1] > current[i] + step_size ? current[i] + step_size : minmax[i][1];
		//put the min and max values in a vector but multiply them first with random value
		position.push_back(rand_in_bounds(min, max));
	}
}

//returns a number based on l and s factor
double large_step_size(const int &iter, const double &step_size, const double &s_factor, const double &l_factor, const int& iter_mult)
{
	//if iter is bigger than zero and iter is divisible by 10
	if (iter>0 && (0 == iter % iter_mult)){
		return step_size * l_factor;
	}
	else {
		return step_size * s_factor;
	}
}

//assign values to step_cost and big_step_cost
void take_steps(const std::vector<std::vector<double>> &bounds, std::vector<double> &current_vector, const double &step_size,
	const double &big_stepsize, double &step_cost, double &big_step_cost, std::vector<double> &step_vector,
	std::vector<double> big_step_vector, const int &problem_size)
{
	//fill step_vector with two random numbers
	take_step(bounds, current_vector, step_size, step_vector, problem_size);
	//debug purposes
	std::cout << "members = ";
	for each(double member in step_vector){
		std::cout << member << " , ";
	}
	//square step_vector contents then sum them up
	step_cost = objective_function(step_vector);
	//clear the step_vector so it will only contain two numbers the most
	step_vector.clear();
	std::cout << "\n step_cost = " << step_cost << "\n";
	//fill big_step_vector vector with {problem_size=2} random numbers
	take_step(bounds, current_vector, big_stepsize, big_step_vector, problem_size);
	//square big_step_vector contents then sum them up
	big_step_cost = objective_function(big_step_vector);
}


double search(const double &max_iter, const std::vector<std::vector<double>> &bounds, const double &init_factor, const double &s_factor,
	const double &l_factor, const int &iter_mult, const int &max_no_impr, const int &problem_size)
{
	double dstep_size = (bounds[0][1] - bounds[0][0]) * init_factor;
	std::vector<double> current_vector;
	std::vector<double> step_vector;
	std::vector<double> big_step_vector;
	double count = 0;
	double dstep_cost;
	double dbig_step_cost = 0;
	//fill a vector(current vector) with random numbers based on the min and max seed in the given constant bounds
	random_vector(bounds, current_vector, problem_size);
	//initial solution
	double dcurrent_cost = objective_function(current_vector);
	std::cout << "dcurrent_cost == " << dcurrent_cost << "\n";
	for (int i = 0; i <= max_iter; i++){
		//return step_size multiplied by bigger factor if iteration is bigger than zero and iter_mult is divisible by 10
		double dbig_stepsize = large_step_size(i, dstep_size, s_factor, l_factor, iter_mult);
		std::cout << "dbig_stepsize :" << dbig_stepsize << "\n";
		//fill value for dstep_cost  and dbig_step_cost
		take_steps(bounds, current_vector, dstep_size, dbig_stepsize, dstep_cost, dbig_step_cost, step_vector, big_step_vector, problem_size);
		//return dcurrent_cost value depending on which of these values are bigger - dbig_step_cost or dstep_cost
		if (dstep_cost <= dcurrent_cost || dbig_step_cost <= dcurrent_cost){
			if (dbig_step_cost <= dstep_cost){
				dstep_size = dbig_stepsize;
				dcurrent_cost = dbig_step_cost;
			}
			else{
				dcurrent_cost = dstep_cost;
			}
			count = 0;
		}
		else{
			count += 1;
			if (count >= max_no_impr){
				count = 0;
				dstep_size = dstep_size / s_factor;
			}
		}
		std::cout << " > iteration #" << i << " " << "best = " << dcurrent_cost << "\n";
	}
	return dcurrent_cost;
}


int main()
{
	std::cout << "start \n";
	const int problem_size = 2;
	const std::vector<std::vector<double>> bounds{ { -5, 5 }, { -5, 5 } };
	const double max_iter = 10;
	const double init_factor = 0.05;
	const double s_factor = 1.3;
	const double l_factor = 3.0;
	const int iter_mult = 10;
	const int max_no_impr = 30;
	double best = search(max_iter, bounds, init_factor, s_factor, l_factor, iter_mult, max_no_impr, problem_size);
	std::cout << "Done. Best Solution: c =" << best << "v = " << "vector result TBD" << "\n";
}
