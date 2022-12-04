/* *****************************************************************************
##
# @file        data_storing.hpp
# @brief       Storing the best results
# @copyright   MIT license
# ****************************************************************************/

#ifndef DATA_STORING_HEADER_HPP
#define DATA_STORING_HEADER_HPP

#include <array>
#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include "..\\include\\quizz_interfaces.hpp"

using namespace std;

struct record
{
    string name;
    int result;
    int time;
};

class CCurrentSession
{
public:
    CCurrentSession(themes f_theme);
    void get_credencials();
    void set_duration(int f_duration);
    void updateRecords();
    void displayScores();
    int get_score();
    void incr_score();
    void store();
    void getHistory();
    themes getTheme();

private:
    themes m_theme;
    record m_record;
    vector<record> m_records;
};



#endif // DATA_STORING_HEADER_HPP
