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
    int questionNumber;
    int time;
};

class CCurrentSession
{
public:
    CCurrentSession();
    void get_credencials();
    void set_duration(int f_duration);
    void updateRecords();
    void displayScores();
    int get_score();
    void incr_score();
    int get_questionNumber();
    void incr_questionNumber();
    void store();
    void getHistory();
    void setTheme(themes f_theme);
    themes getTheme();
    void setMode(quizz_mode f_mode);
    quizz_mode getMode();
    // Dont want 3 methods to set/reset and read
    bool m_gameStarted;

private:
    quizz_mode m_mode;
    themes m_theme;
    record m_record;
    vector<record> m_records;
};



#endif // DATA_STORING_HEADER_HPP
