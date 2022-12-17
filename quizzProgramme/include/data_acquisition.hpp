/* *****************************************************************************
##
# @file        data_acquisition.hpp
# @brief       acquisition of the data
# @copyright   MIT license
# ****************************************************************************/

#ifndef DATA_ACQUISITION_HEADER_HPP
#define DATA_ACQUISITION_HEADER_HPP

#include <array>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include "..\\include\\quizz_interfaces.hpp"

// Deactivate warning for mbstowcs being unsafe
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

using namespace std;

std::vector<pair<string, int>> s_people_with_gt;
std::vector<string> s_house;

array<string, 3> cout_1{ "invalid","house","village" };
array<string, 3> cout_2{ "invalid","GoT","Naruto" };

// Helper function for converting types
static wchar_t* charToWChar(const char* text)
{
    const size_t size = strlen(text) + 1;
    wchar_t* wText = new wchar_t[size];
    mbstowcs(wText, text, size);
    return wText;
}

// Helper function, overloading operator
ostream& operator<<(ostream& f_out, const quizz_mode f_input)
{
    const char* s = 0;

    switch (f_input)
    {
    case quizz_mode::QUIZZ_RULES:
    {
        s = "Quizz Rules";
        break;
    }
    case quizz_mode::BEST_SCORES:
    {
        s = "Best Scores";
        break;
    }
    case quizz_mode::TRAINING:
    {
        s = "Training";
        break;
    }
    case quizz_mode::TEST:
    {
        s = "Test";
        break;
    }
    case quizz_mode::EXIT:
    {
        s = "Exit";
        break;
    }
    default:
    {
        s = "INVALID";
    }
    }
    return f_out << s;
};

ostream& operator<<(ostream& f_out, const themes f_input)
{
    const char* s = 0;

    switch (f_input)
    {
    case themes::GoT:
    {
        s = "Game Of Thrones";
        break;
    }
    case themes::Naruto:
    {
        s = "Naruto";
        break;
    }
    default:
    {
        s = "INVALID";
    }
    }
    return f_out << s;
};

// interface to access data
string INPUT_PATH_GOT_1 = "..\\database\\got_name_house.csv";
//string INPUT_PATH_NARUTO_1 = "..\\database\\naruto_name_village.csv";
string INPUT_PATH_NARUTO_1 = "C:\\Users\\lsm1so\\source\\repos\\quizzProgramme\\quizzProgramme\\database\\naruto_name_village.csv";
map<themes, string> data_in_selection{
    {themes::GoT,INPUT_PATH_GOT_1},
    {themes::Naruto,INPUT_PATH_NARUTO_1},
};

/// @brief print GT information (for debugging purpose)
void print_GT()
{
    cout << "===================================================" << endl;
    for (int i{ 0 }; i < s_people_with_gt.size(); i++)
    {
        if (s_people_with_gt[i].second < s_house.size())
        {
            cout << s_people_with_gt[i].first << " is in the house " << s_house[s_people_with_gt[i].second] << endl;
        }
        else
        {
            cout << "house vector was not filled properly" << endl;
        }
    }

    cout << "===================================================" << endl;
    cout << endl;
}

/// @brief read CSV and store data into vectors
///        2 interfaces are provided, a vector of houses and a vector of pair containing the names and responses 
void init_input_data(themes f_selection)
{
    ifstream fin(data_in_selection[f_selection].c_str());

    if (!fin.good())
    {
        cout << "Could not open the input file" << endl;
    }

    // get first line
    string line, word, name, house;
    while (getline(fin, line))
    {
        stringstream s(line);
        int index{ 0 };

        //CSV has only two words by line
        while (getline(s, word, ','))
        {
            if (index == 0) name = word;
            if (index == 1) house = word;
            index++;
        }

        // Update the houses list
        auto houseIt = find(s_house.begin(), s_house.end(), house);
        int houseIndex{ 0 };
        if (houseIt == s_house.end())
        {
            s_house.push_back(house);
            houseIndex = static_cast<int>(s_house.size()) - 1;
        }
        else
        {
            houseIndex = static_cast<int>(houseIt - s_house.begin());
        }

        // Update the people list
        s_people_with_gt.push_back(make_pair(name, houseIndex));
    }
}

#endif // DATA_ACQUISITION_HEADER_HPP
