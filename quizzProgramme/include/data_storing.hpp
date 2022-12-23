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
#include <time.h>
#include <chrono>
#include "..\\include\\quizz_interfaces.hpp"

using namespace std;

struct record
{
    string name;
    int result;
    int questionNumber;
    int time;
};

enum class EGameState
{
    invalid = 0,
    not_started,
    started,
    finished

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
    int get_scorePercent();
    void incr_score();
    void resetScore();
    int get_questionNumber();
    void incr_questionNumber();
    void resetQuestionNumber();
    void store();
    void getHistory();
    void setTheme(themes f_theme);
    themes getTheme();
    void setMode(quizz_mode f_mode);
    void setStart();
    std::chrono::system_clock::time_point getStart();
    quizz_mode getMode();
    // Dont want 3 methods to set/reset and read
    EGameState m_gameStarted;
    int m_indexPeople;
private:
    std::chrono::system_clock::time_point m_start;
    quizz_mode m_mode;
    themes m_theme;
    record m_record;
    vector<record> m_records;
};



#endif // DATA_STORING_HEADER_HPP
