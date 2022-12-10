#ifndef UTIL_RANDOM_HPP
#define UTIL_RANDOM_HPP

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <random>
#include <unistd.h>

namespace util {

    /**
    * @brief a psuedorandom number generator
    */
    class prng {
    public: 
        
        /**
        * @breif prng constructor
        *
        * @param t_seed the seed to initalize the prng with
        */
        prng(std::string t_seed = "") {
            std::random_device rd;
            m_generator = std::mt19937(rd());

            m_seed = (t_seed == "") ? std::to_string(getpid() * time(NULL)) : t_seed;

            std::seed_seq seed(m_seed.begin(), m_seed.end());

            m_generator.seed(seed);
        }

        /**
        * @breif generate a random float
        *
        * @param t_lower the lower bound to gernerate the random number with
        * @param t_upper the upper bound to gernerate the random number with
        * @return float a random float between t_lower and t_upper
        */
        float randf(float t_lower = 0.0f, float t_upper = 1.0f) {
            std::uniform_real_distribution<> dist(t_lower, t_upper);
            return dist(m_generator);
        }

        /**
        * @breif generate a random int
        *
        * @param t_lower the lower bound to gernerate the random number with
        * @param t_upper the upper bound to gernerate the random number with
        * @return float a random int between t_lower and t_upper
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
