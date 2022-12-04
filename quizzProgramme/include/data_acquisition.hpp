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
#include <vector>

using namespace std;
string INPUT_FILE_PATH = "database\\input.csv";

std::vector<pair<string, int>> s_people_with_gt;
std::vector<string>  s_house;

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
void init_input_data()
{
    ifstream fin(INPUT_FILE_PATH.c_str());
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
