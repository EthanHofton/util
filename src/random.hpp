#ifndef UTIL_RANDOM_HPP
#define UTIL_RANDOM_HPP

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <random>
#include <unistd.h>

namespace util {

    /**
    * @brief a pseudo random number generator class. Uses a Mersenne Twiste generator to generate the numbers and a uniform distribution to distrubute the random number
    */
    class prng {
    public: 
        
        /**
        * @brief prng constructor. initalizes the mt generator with a random seed
        *
        * @param t_seed seed to be used to initalize the random generator, if left empty, a random seed is used
        */
        prng(std::string t_seed = "") {
            std::random_device rd;
            m_generator = std::mt19937(rd());

            m_seed = (t_seed == "") ? std::to_string(getpid() * time(NULL)) : t_seed;

            std::seed_seq seed(m_seed.begin(), m_seed.end());

            m_generator.seed(seed);
        }

        /**
        * @brief generate a random float between `t_lower` and `t_upper`
        *
        * @param t_lower the lower bound (0 default)
        * @param t_upper the upper bound (1 default)
        *
        * @return a random float between the lower and upper bound
        */
        float randf(float t_lower = 0.0f, float t_upper = 1.0f) {
            std::uniform_real_distribution<> dist(t_lower, t_upper);
            return dist(m_generator);
        }

        /**
        * @brief generate a random interger between `t_lower` and `t_upper`
        *
        * @param t_lower the lower bound for the distrubution (0 default)
        * @param t_upper the upper bound for the distrubution (1 default)
        *
        * @return a random interger between the lower and upper bound
        */
        int randi(int t_lower = 0, int t_upper = 1) {
            std::uniform_int_distribution<> dist(t_lower, t_upper);
            return dist(m_generator);
        }


    private:

        std::string m_seed;
        std::mt19937 m_generator;
    };
}

#endif
