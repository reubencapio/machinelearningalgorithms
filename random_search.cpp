#include <iostream>
#include<math.h>
#include <stdlib.h> 
#include <vector>
#include <map>

//algo gist

//objective function is any random solution function that each set of your data go through 
//and this function will return a certain value result based on said data set, this returned value is the candidate
//if the candidate is lesser than the best cost, then it will become the best cost. the goal is to retain
//the lowest candidate cost


//sum the square of all  vector members
double objective_function(std::vector<double> &mysearch_space)
{
  double result = 0;
  for(auto it : mysearch_space){  
	result = result + pow(it, 2);
  }
  return result;
}


//return a vector(with size same as that of problem_size) with random numbers generated using search_space members as basis
void random_vector(std::vector<std::vector<double>> &mysearch_space, std::vector<double> &mynew_vec, int problem_size)
{
   for(int i = 0; i < problem_size; i++){
	mynew_vec.push_back(mysearch_space[i][0] + ( (mysearch_space[i][1] - mysearch_space[i][0]) * rand() ) );
   }
   return;
}



std::vector<double> search(std::vector<std::vector<double>> &mysearch_space, int max_iter, int problem_size)
{
  std::map<double,std::vector<double>> results;
  double best = 0;
  for(int i = 0; i < max_iter; i++){
    std::vector<double> new_vec;
    //return a vector(with size same as that of problem_size) with random numbers generated using search_space members as basis
    random_vector(mysearch_space, new_vec, problem_size);
    //print out all returned vector values
    for(auto nv: new_vec){
	std::cout << "cost member: " << nv << "\n";
    }
    //sum the square of all  vector members
    double candidate = objective_function(new_vec);
    std::cout << "candidate: " << candidate << "\n\n";
	
    if (0 == best){
	best = candidate;
    }
    //retain the candidate with lowest value from the vectors created from random numbers for max_iter times
    best = candidate < best? candidate : best;
    results[candidate] = new_vec;
  		
  }
  //return vector which resulted with lowest sum of its squared members
  std::cout << "best candidate = " << best << "\n";
  return results[best];
}

int main()
{
  const int problem_size = 2;
  std::vector<std::vector<double>> search_space { { -5, 5 },
 						  { -5, 5 } };
  const int max_iter = 7;
  //get the vector returning the smallest sum of its squares
  std::vector<double> cost = search(search_space, max_iter, problem_size);
  //print out returned vector values
  for(auto m: cost){
      std::cout << "best candidate vector values: " << m << "\n";
  }
}
