/* *****************************************************************************
##
# @file        quizz_evaluation.hpp
# @brief       evaluation of the session result
# @copyright   MIT license
# ****************************************************************************/

#ifndef QUIZZ_EVALUATION_HEADER_HPP
#define QUIZZ_EVALUATION_HEADER_HPP

using namespace std;

string comment(float f_score)
{
    string result;

    if (f_score == 0.F)
    {
        result = ", that bad?";
    }
    else if ((f_score > 0.F) && (f_score < .5F))
    {
        result = ", you have a long road to climb";
    }
    else if ((f_score >= .5F) && (f_score < 1.F))
    {
        result = ", still need to improve";
    }
    else
    {
        result = ", perfect";
    }
    return result;
}


#endif // QUIZZ_EVALUATION_HEADER_HPP
