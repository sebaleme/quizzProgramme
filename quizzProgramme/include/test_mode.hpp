/* *****************************************************************************
##
# @file        test_mode.hpp
# @brief       challenging quizz mode with permanent score
# @copyright   MIT license
# ****************************************************************************/

#ifndef TEST_MODE_HEADER_HPP
#define TEST_MODE_HEADER_HPP

#include <time.h>
#include <chrono>
#include <iomanip>
#include "..\\include\\data_acquisition.hpp"
#include "..\\include\\quizz_evaluation.hpp"
#include "..\\include\\data_storing.hpp"

using namespace std;

string test_mode_question(CCurrentSession* f_currentSession, HWND hWnd)
{
    f_currentSession->m_indexPeople = rand() % static_cast<int>(s_people_with_gt.size());
    f_currentSession->incr_questionNumber();

    // Interaction with user
    string question{ "from which village is " };
    question.append(s_people_with_gt[f_currentSession->m_indexPeople].first);
    question.append(" ?");
    return question;
}

string test_mode_evaluate(CCurrentSession* f_currentSession, TCHAR* f_answer, bool& f_correct)
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
        if (*houseIt == s_house[s_people_with_gt[f_currentSession->m_indexPeople].second])
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
        evaluation.append("wrong, this is not even a village in ");
        //evaluation.append(cout_2[static_cast<int>(f_theme)]);
    }

    // Prepare next question
    s_people_with_gt.erase(s_people_with_gt.begin() + f_currentSession->m_indexPeople);

    return evaluation;
}


void test_mode(CCurrentSession f_currentSession)
{
    int indexPeople{ rand() % static_cast<int>(s_people_with_gt.size()) };
    string answer;
    int iteration{ 10 };
    auto start = std::chrono::system_clock::now();

    while (iteration)
    {
        iteration--;

        // Interaction with user
        cout << "from which " << cout_1[static_cast<int>(f_currentSession.getTheme())] << " is "
            << s_people_with_gt[indexPeople].first << "?" << endl;
        cin >> answer;

        // Process user response
        std::vector<string>::iterator houseIt;
        houseIt = find(s_house.begin(), s_house.end(), answer);
        if (houseIt != s_house.end())
        {
            if (*houseIt == s_house[s_people_with_gt[indexPeople].second])
            {
                cout << "nice one" << endl;
                f_currentSession.incr_score();
            }
            else
            {
                cout << "nope!" << endl;
            }
        }
        else
        {
            cout << "wrong, this is not even a " << cout_1[static_cast<int>(f_currentSession.getTheme())] << " in "
                << cout_2[static_cast<int>(f_currentSession.getTheme())] << endl;
        }

        // Prepare next question
        s_people_with_gt.erase(s_people_with_gt.begin() + indexPeople);
        indexPeople = rand() % s_people_with_gt.size();
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    f_currentSession.set_duration(static_cast<int>(elapsed_seconds.count()));

    // Give result
    cout << "================================" << endl;
    float scoreFloat = (float)f_currentSession.get_score() / 10.F;
    cout << "you got " << f_currentSession.get_score() << "/10 in " << std::fixed << std::setprecision(0)
        << elapsed_seconds.count() << "s" << comment(scoreFloat) << endl;
    // Append new session result in database
    f_currentSession.updateRecords();
    f_currentSession.store();
}

#endif // TEST_MODE_HEADER_HPP
