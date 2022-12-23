#pragma once

#include "..\\include\\Resource.h"

void setQuizzAnswerColor(HDC& f_hdc, int f_scorePercent)
{
    if (f_scorePercent == 100)
    {
        SetTextColor(f_hdc, RGB(0, 0, 255)); //blue
    }
    else if (f_scorePercent > 70)
    {
        SetTextColor(f_hdc, RGB(0, 255, 0)); //green
    }
    else if (f_scorePercent > 50)
    {
        SetTextColor(f_hdc, RGB(169, 197, 58)); // green/yellow
    }
    else if (f_scorePercent > 30)
    {
        SetTextColor(f_hdc, RGB(255, 165, 0)); //orange
    }
    else
    {
        SetTextColor(f_hdc, RGB(255, 99, 71)); // tomato
    }
};