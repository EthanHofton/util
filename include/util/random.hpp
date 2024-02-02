#ifndef UTIL_RAND_HPP
#define UTIL_RAND_HPP

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <random>
#include <unistd.h>


/**
 * @brief random number generator class and some random util functions
 * @file rand.hpp
 */

namespace util
{
    /**
     * @brief psuedo random number generator class
     * 
     */
    class prng
    {
    public:

        /**
         * @brief get a static instance of the class
         * 
         * @return prng& 
         */
        static prng& instance() { if (s_instance == nullptr) s_instance = new prng(); return (*s_instance); }
        /**
         * @brief destroy the static instance of the class
         * 
         */
        static void release() { if (s_instance != nullptr) delete s_instance; }

        /**
         * @brief Construct a new prng object
         * 
         * @param t_seed the seed to construct the random number genrator with. if blank or empty string, use a process_id * time since unix epoch as seed
         */
        prng(const std::string& t_seed = "")
        {
            std::random_device rd;
            m_generator = std::mt19937(rd());

            m_seed = (t_seed == "") ? std::to_string(getpid() * time(NULL)) : t_seed;

            std::seed_seq seed(m_seed.begin(), m_seed.end());

            m_generator.seed(seed);
        }
        /**
         * @brief Construct a new prng object
         * 
         * @param t_prng prng to copy
         */
        prng(const prng& t_prng)
        { m_seed = t_prng.m_seed; m_generator = t_prng.m_generator; }
        /**
         * @brief Construct a new prng object (move)
         * 
         * @param t_prng prng to move
         */
        prng(prng&& t_prng)
        { m_seed = std::move(t_prng.m_seed); m_generator = std::move(t_prng.m_generator); }

        /**
         * @brief genreratre a new random double
         * 
         * @param t_lower lower bound for number gen
         * @param t_upper upper bound for number gen
         * @return double 
         */
        double rand(const double& t_lower = 0.0, const double& t_upper = 1.0)
        { std::uniform_real_distribution<> dist(t_lower, t_upper); return dist(m_generator); }
        /**
         * @brief generate a new random interger
         * 
         * @param t_lower lower bound for number gen
         * @param t_upper upper bound for number gen
         * @return int 
         */
        int randi(const int& t_lower = 0, const int& t_upper = 1)
        { std::uniform_int_distribution<> dist(t_lower, t_upper); return dist(m_generator); }

    private:

        std::string m_seed;
        std::mt19937 m_generator;

        static prng* s_instance;
    };

    inline prng *prng::s_instance = nullptr;

    /**
     * @brief shuffle an array
     * 
     * @tparam T the type of the array
     * @param t_data pointer to start of the array
     * @param t_start lower bound for shuffle
     * @param t_end upper bound for shuffle
     * @param t_shuffleIter number of swaps to proform
     */
    template<typename T>
    void shuffle(T* t_data, size_t t_start, size_t t_end, size_t t_shuffleIter)
    { for (int i = 0; (size_t)i < t_shuffleIter; i++) std::swap<T>(t_data[prng::instance().randi(t_start, t_end)], t_data[prng::instance().randi(t_start, t_end)]); }

    /**
     * @brief shuffle an array (shuffle the array the ammount of times of the size of the range)
     * 
     * @tparam T the type of the array
     * @param t_data pointer to first element in array
     * @param t_start shuffle lower bound
     * @param t_end shuffle upper bound
     */
    template<typename T>
    void shuffle(T* t_data, size_t t_start, size_t t_end)
    { shuffle(t_data, t_start, t_end, (t_end - t_start)); }

    /**
     * @brief shuffle an array
     * 
     * @tparam T the type of the array
     * @param t_data pointer to first element in array
     * @param t_size ammount of elements in the array
     */
    template<typename T>
    void shuffle(T* t_data, size_t t_size)
    { shuffle(t_data, 0, t_size-1); }


}

#endif
