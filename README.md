
# Coding Style in this code
- Indent tab: 2 space
- Naming style
  - PascalCase: class
  - snake_case: method, function, variable, file
  - camelCase: local constant
  - SNAKE_CASE: macro, global constant


# Explanation of this code 
- User can run the evolutionary simulation of metacognition using the compiled code.


# Example of operation
1. Compile and run FrontmatterGenerater.cpp
  ```bash
  g++ -o frontmatter FrontmatterGenerator.cpp   
  ```

2. Generate frontmatter.h and set meta parameter
  ```bash
  ./frontmatter -rsw 1 -isw 0 -msw 0 -ev 2 -ag 200 -gen 5000 -mt 0.001 -ch 2 -st 10 -inf 10 -wt 1 -wtr 0 -nr 0.1 -rw 5 -pn 0 -act 1000 -sm 0 -stmi 10 -cbc 0 -icc 0.05 -mtmc 0.1 -stmc 0.01
  ```

3. Compile and run the evolutionary simulation
  ```bash
  g++ -O3 -o evomtm EVOMTM.cpp
  ./evomtm
  ```

# Composition of main and header file
- EVOMTM.cpp: Main process and file output destination
- GeneticAlgorithm.hpp: Evolutionary modeling class
- Agent.hpp: Parameters and performance of each individual
- QLearning.hpp: Q-learning algorithm class 
- MLP.hpp: Neural network algorithm
- ContextBandit.hpp: Context bandit task class
- SCORE.hpp: for aggregation