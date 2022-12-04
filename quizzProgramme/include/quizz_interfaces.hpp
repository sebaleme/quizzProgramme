/* *****************************************************************************
##
# @file        quizz_interfaces.hpp
# @brief       types defined for the quizz
# @copyright   MIT license
# ****************************************************************************/

#ifndef QUIZZ_INTERFACES_HEADER_HPP
#define QUIZZ_INTERFACES_HEADER_HPP

#include <iostream>

// list of existing quizz (TEST are even values)
enum class quizz_mode : uint8_t {
    QUIZZ_RULES = 0U,
    BEST_SCORES,
    TRAINING,
    TEST,
    EXIT,
    END,
};

// existing themes
enum class themes : uint8_t
{
    Invalid = 0,
    GoT = 1,
    Naruto = 2,
    END
};

template<typename T>
inline bool operator< (int f_input1, T f_input2)
{
    return f_input1 < static_cast<int>(f_input2);
};

template<typename T>
inline bool operator<= (int f_input1, T f_input2)
{
    return f_input1 <= static_cast<int>(f_input2);
};

#endif // QUIZZ_INTERFACES_HEADER_HPP
