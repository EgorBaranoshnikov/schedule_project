#include <iostream>
#include <vector>
// #include <set>
#include <ctime>
#include <string>
#include <fstream>
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

int post_all_lessons(vector<vector<pair<string, int>>> &data_v1, vector<vector<string>> &data_v2, int &classs) // разместить все уроки
{
    vector<vector<vector<string>>> schedule(classs);
    for (int i = 0; i < classs; i++)
    {
        schedule[i] = vector<vector<string>>(stoi(data_v2[i][2]));
        for (int j = 0; j < stoi(data_v2[i][2]); j++)
        {
            int cr_kol_yr = data_v1[i].size();
        }
    }
    return 0;
}

int schedule(vector<vector<pair<string, int>>> &data_v1, vector<vector<string>> &data_v2, vector<vector<vector<string>>> &data_v3, vector<pair<pair<string, pair<int, int>>, vector<string>>> &data_v4, int &classs)
{
    int a = post_all_lessons(data_v1, data_v2, classs);
    return 0;
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
    int classs, otv;
    file1 >> classs;

    vector<vector<pair<string, int>>> data_v1;
    data_v1 = data_1v(classs, file1);
    vector<vector<string>> data_v2;
    data_v2 = data_2v(classs);
    vector<vector<vector<string>>> data_v3;
    data_v3 = data_3v(classs);
    vector<pair<pair<string, pair<int, int>>, vector<string>>> data_v4;
    data_v4 = data_4v();
    otv = schedule(data_v1, data_v2, data_v3, data_v4, classs);
}

// srand(time(0));
// int x = rand() % (data_v1[i].size() + 1);
