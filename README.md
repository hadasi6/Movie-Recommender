# Movie Recommender (C++)
**A Content-Based & Collaborative Filtering System**

This project is a movie recommendation system that suggests movies based on either the content of the movies or the similarity to other users’ tastes. It processes users’ movie ratings using two approaches:
1. **Content-based Filtering:** Recommends movies similar to those a user already likes by analyzing movie attributes.
2. **Collaborative Filtering:** Recommends movies that users with similar tastes have enjoyed.

Implemented in modern C++17, the project leverages the STL for data structures (such as vectors and maps) and smart pointers (`std::shared_ptr`) for efficient memory management.

## How to Run
1. **Ensure a C++ Compiler is Installed:** (Supporting C++11 or above, e.g., g++ 9+).
2. **Clone the Repository.**
3. **Compile the Code:**  
   For example, using g++:
   ```bash
   g++ -std=c++17 *.cpp -o movie_recommender
