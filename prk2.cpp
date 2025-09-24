#include <iostream>
#include <vector>
// #include <set>
#include <ctime>
#include <string>
#include <fstream>
#include <cmath>
using namespace std;

int can_post_a_lesson() // можно ли разместить урок
{
    return 0;
}

int improve_schedule() // улучшить расписание
{
    return 0;
}

int evaluate_schedule() // оценить_расписание
{
    return 0;
}

vector<vector<int>> schedule_data_1v(int kol_day, vector<pair<string, int>> &data_v1) // количество уроков в день за каждый предмет
{
    vector<vector<int>> schedule_data_1v(data_v1.size());
    for (int i = 0; i < data_v1.size(); i++)
    {
        schedule_data_1v[i] = vector<int>(data_v1[i].second);
        for (int j = 0; j < data_v1[i].second; j++)
        {
            float x = float(kol_day) / data_v1[i].second;
            schedule_data_1v[i][j] = round((j + 1) * (float(kol_day) / data_v1[i].second));
        }
    }
    return schedule_data_1v;
}

pair<int, int> schedule_data_2v(vector<pair<string, int>> &data_v1_kol_yr, int kol_day) // среднее количество уроков
{
    pair<int, int> a;
    int kol_yr = 0;
    for (int i = 0; i < data_v1_kol_yr.size(); i++)
    {
        kol_yr += data_v1_kol_yr[i].second;
    }
    a.first = kol_yr / kol_day;
    a.second = kol_yr % kol_day;
    return a;
}

pair<int, int> schedule_data_3v(vector<vector<pair<string, int>>> &data_v1, vector<pair<pair<string, pair<int, int>>, vector<string>>> &data_v4, int j, int dop) // средний бал сложности дня
{
    pair<int, int> a;
    int kol_slosh = 0;
    for (int i = 0; i < data_v1[j].size(); i++)
    {
        for (int q = 0; q < data_v4.size(); q++)
        {
            if (data_v1[j][i].first == data_v4[q].first.first)
            {
                kol_slosh += data_v4[q].first.second.second * data_v1[j][i].second;
                break;
            }
        }
    }
    a.first = kol_slosh / dop;
    a.second = kol_slosh & dop;
    return a;
}

int fine11(vector<int> &a, int b, vector<vector<string>> schedule_1v, int i) // 1. Штраф за превышение количества уроков
{
    int fine1 = 0;
    for (int q = 0; q < a.size(); q++)
    {
        if (schedule_1v[a[q] - 1].size() + 1 > b)
        {
            fine1 += 3 * schedule_1v[a[q] - 1].size();
        }
    }
    return fine1;
}

int fine22(vector<int> &a, int b, vector<int> f) // 2. Штраф за превышение балла сложности
{
    int fine2 = 0;
    for (int q = 0; q < a.size(); q++)
    {
        if (f[a[q]] > b)
        {
            fine2 += 3;
        }
    }
    return fine2;
}

vector<vector<string>> schedule_v1(vector<vector<int>> &schedule_data_v1, pair<int, int> &schedule_data_v2, pair<int, int> &schedule_data_v3, int kol_day, vector<pair<string, int>> data_v1)
{
    vector<vector<string>> schedule_1v(kol_day);
    vector<int> f(kol_day, 0);
    // 1) находим самое оптимальное расстановку уроков в неделе
    for (int i = 0; i < schedule_data_v1.size(); i++)
    {
        // 2) перебераем каждый день что бы найти лучший
        int good_day;
        int day_fine = INT_MAX;
        for (int j = 0; j < kol_day; j++)
        {
            int fine1 = 0, fine2 = 0;
            fine1 = fine11(schedule_data_v1[i], schedule_data_v2.first, schedule_1v, i);
            fine2 = fine22(schedule_data_v1[i], schedule_data_v3.first, f);
            // 3. Штраф за много уроков одного типа
            // 4. лучший день + следующий день
            if (fine1 + fine2 < day_fine)
            {
                good_day = j;
                day_fine = fine1 + fine2;
            }
            for (int q = 0; q < schedule_data_v1[i].size(); q++)
            {
                schedule_data_v1[i][q]++;
                if (schedule_data_v1[i][q] == kol_day + 1)
                    schedule_data_v1[i][q] = 1;
            }
        }
        // проверяем + ставим
        for (int j = 0; j < schedule_data_v1[i].size(); j++)
        {
            if (((schedule_data_v1[i][j] + good_day) % kol_day) - 1 == -1)
            {
                schedule_1v[kol_day - 1].push_back(data_v1[i].first);
            }
            else
                schedule_1v[((schedule_data_v1[i][j] + good_day) % kol_day) - 1].push_back(data_v1[i].first);
            f[((schedule_data_v1[i][j] + good_day) % kol_day)] += data_v1[i].second;
        }
    }
}

vector<vector<vector<string>>> post_all_lessons(vector<vector<pair<string, int>>> &data_v1, vector<vector<string>> &data_v2, vector<vector<vector<string>>> &data_v3, vector<pair<pair<string, pair<int, int>>, vector<string>>> &data_v4, int &classs) // разместить все уроки 1 версия
{
    vector<vector<vector<string>>> schedule(classs);
    vector<vector<int>> schedule_data_v1;
    pair<int, int> schedule_data_v2, schedule_data_v3;
    vector<vector<string>> schedule_1v;
    for (int i = 0; i < classs; i++)
    {
        schedule_data_v1 = schedule_data_1v(stoi(data_v2[i][2]), data_v1[i]);
        schedule_data_v2 = schedule_data_2v(data_v1[i], stoi(data_v2[i][2]));
        schedule_data_v3 = schedule_data_3v(data_v1, data_v4, i, stoi(data_v2[i][2]));
        schedule[i] = schedule_v1(schedule_data_v1, schedule_data_v2, schedule_data_v3, stoi(data_v2[i][2]), data_v1[i]);
    }
}

vector<vector<vector<string>>> schedule(vector<vector<pair<string, int>>> &data_v1, vector<vector<string>> &data_v2, vector<vector<vector<string>>> &data_v3, vector<pair<pair<string, pair<int, int>>, vector<string>>> &data_v4, int &classs)
{
    vector<vector<vector<string>>> schedule_v1 = post_all_lessons(data_v1, data_v2, data_v3, data_v4, classs);
}

vector<vector<pair<string, int>>> data_1v(int &classs, ifstream &file1)
{
    int count, kol_lesson;
    string lesson;
    vector<vector<pair<string, int>>> data_v1(classs);
    for (int i = 0; i < classs; i++)
    {
        file1 >> kol_lesson;
        data_v1[i] = vector<pair<string, int>>(kol_lesson);

        for (int j = 0; j < kol_lesson; j++)
        {
            file1 >> lesson >> count;
            data_v1[i][j] = make_pair(lesson, count);
        }
    }
    return data_v1;
}

vector<vector<string>> data_2v(int &classs)
{
    ifstream file2("2file_project.txt");
    vector<vector<string>> data_v2(classs);

    for (int i = 0; i < classs; i++)
    {
        data_v2[i] = vector<string>(3);
        file2 >> data_v2[i][0] >> data_v2[i][1] >> data_v2[i][2];
    }
    return data_v2;
}

vector<vector<vector<string>>> data_3v(int &classs)
{
    ifstream file3("3file_project.txt");
    vector<vector<vector<string>>> data_v3(classs);
    int dop;

    for (int i = 0; i < classs; i++)
    {
        file3 >> dop;
        data_v3[i] = vector<vector<string>>(dop);
        for (int j = 0; j < dop; j++)
        {
            data_v3[i][j] = vector<string>(3);
            file3 >> data_v3[i][j][0] >> data_v3[i][j][1] >> data_v3[i][j][2];
        }
    }
    return data_v3;
}

vector<pair<pair<string, pair<int, int>>, vector<string>>> data_4v()
{
    int kol;
    ifstream file4("4file_project.txt");
    file4 >> kol;
    vector<pair<pair<string, pair<int, int>>, vector<string>>> data_v4(kol);
    for (int i = 0; i < kol; i++)
    {
        file4 >> data_v4[i].first.first >> data_v4[i].first.second.first >> data_v4[i].first.second.second;
        data_v4[i].second = vector<string>(data_v4[i].first.second.first);
        for (int j = 0; j < data_v4[i].first.second.first; j++)
        {
            file4 >> data_v4[i].second[j];
        }
    }
    return data_v4;
}

int main()
{
    ifstream file1("1file_project.txt");
    int classs;
    file1 >> classs;

    vector<vector<pair<string, int>>> data_v1;
    data_v1 = data_1v(classs, file1);
    vector<vector<string>> data_v2;
    data_v2 = data_2v(classs);
    vector<vector<vector<string>>> data_v3;
    data_v3 = data_3v(classs);
    vector<pair<pair<string, pair<int, int>>, vector<string>>> data_v4;
    data_v4 = data_4v();
    vector<vector<vector<string>>> otv;
    otv = schedule(data_v1, data_v2, data_v3, data_v4, classs);
}

// srand(time(0));
// int x = rand() % (data_v1[i].size() + 1);