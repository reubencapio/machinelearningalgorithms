#include <iostream>
#include<math.h>
#include <stdlib.h> 
#include <vector>
#include <map>

using namespace std;


//algorithm: 
//   1. create n-dimension vector
//   2. create 1 dimension vector with size n using values from n-dimension vector and a random number
//   3. using the newly created vector in step 2, square each value then sum them up, this will be the candidate cost,
//      lowest candidate cost will be the needed answer
//   4. store candidate cost mapped to its vector values -> TODO
//   5. display result, lowest candidate cost and its corresponding vector values

double objective_function(std::vector<double> &mysearch_space)
{
  double result = 0;
  for(auto it : mysearch_space){  
		result = result + pow(it, 2);
  }
  return result;
}

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
    random_vector(mysearch_space, new_vec, problem_size);
    for(auto nv: new_vec){
		cout << "cost member: " << nv << "\n";
    }
    double candidate = objective_function(new_vec);
    cout << "candidate: " << candidate << "\n\n";
    if (0 == best){
		best = candidate;
	}
    best = candidate < best? candidate : best;
    results[candidate] = new_vec;
	if(i == max_iter-1 ){
  		cout << "best candidate = " << best << "\n";
		return results[best];
	}   
  }
}

int main()
{
  int problem_size = 2;
  std::vector<std::vector<double>> search_space { { -5, 5 },
 											      { -5, 5 } };
  int max_iter = 7;
  std::vector<double> cost = search(search_space, max_iter, problem_size);
  for(auto m: cost){
		cout << "best candidate vector values: " << m << "\n";
  }
}
