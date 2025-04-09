# Evolutionary Simulation of Metacognition

This repository provides a simulation framework for exploring the evolution of metacognition using evolutionary algorithms and reinforcement learning techniques.

---

## Coding Style

- **Indentation**: 2 spaces
- **Naming Conventions**:
  - `PascalCase`: Classes
  - `snake_case`: Methods, functions, variables, filenames
  - `camelCase`: Local constants
  - `SNAKE_CASE`: Macros, global constants

---

## Overview

This project simulates the evolutionary development of metacognitive behavior in agents interacting with their environment. The core of the simulation involves agents adapting through genetic algorithms and reinforcement learning strategies in a context bandit task.

---

## How to Run

### Step 1: Compile the frontmatter generator

```bash
g++ -o frontmatter FrontmatterGenerator.cpp
```

### Step 2: Generate `frontmatter.h` and configure simulation parameters

```bash
./frontmatter -rsw 0 -isw 0 -msw 0 -ev 2 -ag 200 -gen 5000 -mt 0.001 -ch 2 -st 10 -inf 10 -wt 1 -wtr 0 -nr 0.1 -rw 5 -pn 0 -act 1000 -sm 0 -stmi 10 -cbc 0 -icc 0.05 -mtmc 0.1 -stmc 0.01
```

> 💡 The above command sets various evolutionary and environmental parameters. See comments in `FrontmatterGenerator.cpp` for parameter descriptions.

### Step 3: Compile and run the simulation

```bash
g++ -O3 -o evomtm EVOMTM.cpp
./evomtm
```

---

## Code Structure

| File                  | Description                                                   |
|-----------------------|---------------------------------------------------------------|
| `EVOMTM.cpp`          | Main execution file; handles process flow and output          |
| `FrontmatterGenerator.cpp` | Parameter generator for `frontmatter.h` (used as configuration) |
| `GeneticAlgorithm.hpp` | Core class for evolutionary model                            |
| `Agent.hpp`           | Defines individual agent traits and performance metrics       |
| `QLearning.hpp`       | Implements Q-learning algorithm                               |
| `MLP.hpp`             | Multilayer perceptron (neural network) implementation         |
| `ContextBandit.hpp`   | Defines the contextual bandit task environment                |
| `SCORE.hpp`           | Aggregates performance metrics                                |

---

## Notes

- The simulation is designed for reproducibility. For long-term experiments, seed parameters can be specified using the `-rsw` option.
- Compilation requires a standard C++11 or later environment.

---

## License

[MIT License](./LICENSE) (or specify another license as applicable)

---

## Contact

For questions or contributions, please feel free to open an issue or contact the repository owner.
