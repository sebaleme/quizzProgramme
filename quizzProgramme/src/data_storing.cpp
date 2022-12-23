/* *****************************************************************************
##
# @file        data_storing.cpp
# @brief       class implementation for permanent storing
# @copyright   MIT license
# ****************************************************************************/

#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include "../include/data_storing.hpp"

//string OUTPUT_PATH_GOT_1 = "..\\database\\got_best_scores.csv";
//string OUTPUT_PATH_NARUTO_1 = "..\\database\\naruto_best_scores.csv";
string OUTPUT_PATH_GOT_1 = "..\\database\\got_name_house.csv";
string OUTPUT_PATH_NARUTO_1 = "..\\database\\naruto_name_village.csv";
map<themes,string> data_out_selection{
    {themes::GoT,OUTPUT_PATH_GOT_1},
    {themes::Naruto,OUTPUT_PATH_NARUTO_1},
};

CCurrentSession::CCurrentSession() : m_indexPeople{ 0 }, m_mode{quizz_mode::TRAINING}, m_theme{ themes::Naruto }
{
    while(m_record.name.length() == 0)
    {
 //       cout << "Please provide your name" << endl;
       // cin >> m_record.name;
        m_record.name = "user";
    }
    m_record.result = 0;
    m_gameStarted = EGameState::not_started;
    getHistory();
};

void CCurrentSession::set_duration(int f_duration)
{
    m_record.time = f_duration;
};

int CCurrentSession::get_score()
{
    return  m_record.result;
};

int CCurrentSession::get_scorePercent()
{
    int scorePercent = m_record.result * 100 / get_questionNumber();
    return scorePercent;
};

void CCurrentSession::incr_score()
{
    m_record.result += 1;
};

void CCurrentSession::resetScore()
{
    m_record.result = 0;
};

int CCurrentSession::get_questionNumber()
{
    return m_record.questionNumber;
};

void CCurrentSession::incr_questionNumber()
{
    m_record.questionNumber += 1;
};

void CCurrentSession::resetQuestionNumber()
{
    m_record.questionNumber = 0;
};

void CCurrentSession::getHistory()
{
    ifstream fin(data_out_selection[m_theme].c_str());

    if(!fin.good())
    {
        cout << "No result yet" << endl;
    }
    else
    {
        // Read file
        string line;
        while(getline(fin,line))
        {
            record rec;
            stringstream ss(line);
            int iter{0};
            string word;

            while(getline(ss,word,','))
            {
                if (iter == 0)
                {
                    try {
                        rec.result = stoi(word);
                    }
                    catch (std::exception& e) {
                        std::cout << "error converting string to number. Correct CSV file?" << '\n';
                    }
                }
                else if (iter == 1)
                {
                    try {
                        rec.time = stoi(word);
                    }
                    catch (std::exception& e) {
                        std::cout << "error converting string to number. Correct CSV file?" << '\n';
                    }
                }
                else if (iter == 2) rec.name = word;
                else cout << "Error during reading records" << endl;
                iter++;
            }
            m_records.push_back(rec);
        }
    }
};

themes CCurrentSession::getTheme()
{
    return m_theme;
}

void CCurrentSession::setStart()
{
    m_start = std::chrono::system_clock::now();
}

std::chrono::system_clock::time_point CCurrentSession::getStart()
{
    return m_start;
}

void CCurrentSession::setTheme(themes f_theme)
{
    m_theme = f_theme;
}

quizz_mode CCurrentSession::getMode()
{
    return m_mode;
}

void CCurrentSession::setMode(quizz_mode f_mode)
{
    m_mode = f_mode;
}

void CCurrentSession::updateRecords()
{
    m_records.push_back(m_record);
    sort(m_records.begin(),m_records.end(), [](const record & a, const record & b) -> bool{return a.result > b.result;});
};

void CCurrentSession::store()
{
    ofstream fout;
    fout.open((data_out_selection[m_theme].c_str()));
    for(auto& rec : m_records)
    {
        fout << to_string(rec.result) << "," << to_string(rec.time) << "," << rec.name << endl;
    }
    fout.close();
};

void CCurrentSession::displayScores()
{
    int index{1};
    for(const auto& rec : m_records)
    {
        cout << to_string(index) << " " << rec.name << " " << rec.result << "pts" << endl;
        if(index++ > 10) break;
    }
}
