/* *****************************************************************************
##
# @file        training_mode.hpp
# @brief       quizz mode where user can interrupt the test whenever he wants,
#              unlimited time.
# @copyright   MIT license
# ****************************************************************************/

#ifndef TRAINING_MODE_HEADER_HPP
#define TRAINING_MODE_HEADER_HPP

#include <time.h>
#include "..\\include\\data_acquisition.hpp"
#include "..\\include\\quizz_evaluation.hpp"

using namespace std;

string training_mode_question(CCurrentSession* f_currentSession, HWND hWnd)
{
    string question{ "from which " };

    if (s_people_with_gt.size() > 0)
    {
        f_currentSession->m_indexPeople = rand() % static_cast<int>(s_people_with_gt.size());
            f_currentSession->incr_questionNumber();

            // Interaction with user
            question.append(cout_1[static_cast<int>(f_currentSession->getTheme())]);
            question.append(" is ");
            question.append(std::get<0>(s_people_with_gt[f_currentSession->m_indexPeople]));
            question.append(" ?");
            f_currentSession->m_isValidQuestion = true;
    }
    else
    {
        f_currentSession->m_isValidQuestion = false;
        question = "Input data was not read properly";
    }
    return question;
}

string training_mode_evaluate(CCurrentSession* f_currentSession, TCHAR* f_answer, bool& f_correct)
{
    // Interaction with user
    wstring test(&f_answer[0]); //convert to wstring
    string answerGT(test.begin(), test.end()); //and convert to string.

    // Process user response
    string evaluation{ "" };
    std::vector<string>::iterator houseIt;
    houseIt = find(s_house.begin(), s_house.end(), answerGT);
    if (houseIt != s_house.end())
    {
        if (*houseIt == s_house[std::get<1>(s_people_with_gt[f_currentSession->m_indexPeople])])
        {
            evaluation.append("nice one");
            f_currentSession->incr_score();
            f_correct = true;
        }
        else
        {
            evaluation.append("nope!");
        }
    }
    else
    {
        evaluation.append("wrong, this is not even a ");
        evaluation.append(cout_1[static_cast<int>(f_currentSession->getTheme())]);
        evaluation.append(" in ");
        evaluation.append(cout_2[static_cast<int>(f_currentSession->getTheme())]);
    }

    // Prepare next question
    s_people_with_gt.erase(s_people_with_gt.begin() + f_currentSession->m_indexPeople);

    return evaluation;
}

#endif // TRAINING_MODE_HEADER_HPP
