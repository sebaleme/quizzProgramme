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

void training_mode(themes f_theme)
{
    int indexPeople{rand() % static_cast<int>(s_people_with_gt.size())};
    bool wannaPlay{true};
    string answer;
    int score{0}, iteration{0};

    while (wannaPlay)
    {
        iteration++;

        // Interaction with user
        cout << "from which " << cout_1[static_cast<int>(f_theme)] << " is " << s_people_with_gt[indexPeople].first << "?" << endl;
        cin >> answer;

        // Process user response
        std::vector<string>::iterator houseIt;
        houseIt = find(s_house.begin(), s_house.end(), answer);
        if(houseIt != s_house.end())
        {
            if(*houseIt == s_house[s_people_with_gt[indexPeople].second])
            {
                cout << "nice one" << endl;
                score++;
            }
            else
            {
                cout << "nope!" << endl;
            }
        }
        else
        {
            cout << "wrong, this is not even a " << cout_1[static_cast<int>(f_theme)] << " in " << cout_2[static_cast<int>(f_theme)] << endl;
        }

        // Prepare next question
        s_people_with_gt.erase(s_people_with_gt.begin() + indexPeople);
        indexPeople = rand() % s_people_with_gt.size();

        cout << endl << "play again ? y/n" << endl;
        cin >> answer;
        wannaPlay = (answer == "y");
    }

    // Give result
    cout << "================================" << endl;
    float scoreFloat = (float)score / (float)iteration;
    cout << "you got " << score << "/" << iteration << comment(scoreFloat) << endl;
}

#endif // TRAINING_MODE_HEADER_HPP
