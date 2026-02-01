#include <iostream>
#include <vector>
// #include <set>
#include <ctime>
#include <string>
#include <fstream>
#include <cmath>
#include <map>
using namespace std;

vector<string> get_teachers_for_subject(const string &subject, vector<pair<pair<string, pair<int, int>>, vector<string>>> &data_v4)
{
    for (auto &x : data_v4)
        if (x.first.first == subject)
            return x.second;

    return {};
}

bool try_swap_in_day(vector<string> &day, int pos, const string &teacher, int day_id, map<string, vector<vector<bool>>> &busy, vector<bool> &processed)
{
    for (int i = 0; i < (int)day.size(); i++)
    {
        if (i == pos)
            continue;
        if (processed[i])
            continue;
        if (!busy[teacher][day_id][i])
        {
            swap(day[i], day[pos]);
            return true;
        }
    }
    return false;
}

vector<vector<vector<string>>> schedule_v2(vector<vector<vector<string>>> schedule, vector<vector<pair<string, int>>> &data_v1, vector<vector<string>> data_v2, int teacher_count, int &classs, vector<pair<pair<string, pair<int, int>>, vector<string>>> &data_v4)
{
    int days = stoi(data_v2[0][2]);
    int max_lessons = 15;
    map<string, vector<vector<bool>>> teacher_busy;
    for (int c = 0; c < classs; c++)
        for (auto &sub : data_v1[c])
        {
            auto teachers = get_teachers_for_subject(sub.first, data_v4);
            for (auto &t : teachers)
                teacher_busy[t] = vector<vector<bool>>(days, vector<bool>(max_lessons, false));
        }

    vector<vector<vector<string>>> result = schedule;
    for (int c = 0; c < classs; c++)
    {
        map<string, string> main_teacher;
        for (int d = 0; d < days; d++)
        {
            vector<bool> processed(result[c][d].size(), false);
            for (int l = 0; l < (int)result[c][d].size(); l++)
            {
                string subject = result[c][d][l];
                if (!main_teacher.count(subject))
                {
                    auto teachers = get_teachers_for_subject(subject, data_v4);
                    if (!teachers.empty())
                        main_teacher[subject] = teachers[0];
                }

                string t;
                if (main_teacher.count(subject))
                    t = main_teacher[subject];

                bool teacher_found = false;

                if (!t.empty())
                {
                    // пробуем переставить уроки
                    if (!teacher_busy[t][d][l])
                    {
                        teacher_found = true;
                        teacher_busy[t][d][l] = true;
                    }
                    else
                    {
                        bool ok = try_swap_in_day(result[c][d], l, t, d, teacher_busy, processed);
                        if (ok)
                        {
                            teacher_found = true;
                            teacher_busy[t][d][l] = true;
                            subject = result[c][d][l];
                            if (!main_teacher.count(subject))
                            {
                                auto teachers = get_teachers_for_subject(subject, data_v4);
                                if (!teachers.empty())
                                    main_teacher[subject] = teachers[0];
                            }
                            t = main_teacher.count(subject) ? main_teacher[subject] : string();
                        }
                        else
                        {
                            // пробуем другого учителя
                            auto teachers = get_teachers_for_subject(subject, data_v4);
                            for (auto &alt : teachers)
                            {
                                if (!teacher_busy[alt][d][l])
                                {
                                    main_teacher[subject] = alt;
                                    t = alt;
                                    teacher_found = true;
                                    teacher_busy[t][d][l] = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (teacher_found && !t.empty())
                {
                    result[c][d][l] = subject + " (" + t + ")";
                }
                else
                {
                    result[c][d][l] = subject + " ()";
                }
                processed[l] = true;
            }
        }
    }

    return result;
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
    a.second = kol_slosh % dop;
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
        int good_day = 0;
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
    return schedule_1v;
}

void save_schedule_to_file(vector<vector<vector<string>>> pr_schedule, vector<vector<string>> &data_v2)
{
    ofstream file("final_schedule.txt");
    for (int i = 0; i < pr_schedule.size(); i++)
    {
        file << "Класс: " << data_v2[i][0] << endl;
        file << "Классный руководитель: " << data_v2[i][1] << endl;
        file << "Дней в неделе: " << data_v2[i][2] << endl
             << endl;

        int days = stoi(data_v2[i][2]);
        for (int j = 0; j < days; j++)
        {
            file << "День " << (j + 1) << ":" << endl;
            for (int q = 0; q < pr_schedule[i][j].size(); q++)
            {
                file << "  Урок " << (q + 1) << ": " << (pr_schedule[i][j][q].empty() ? "---" : pr_schedule[i][j][q]) << endl;
            }
            file << endl;
        }
        file << "---------------------------------" << endl;
    }
}

vector<vector<vector<string>>> post_all_lessons(vector<vector<pair<string, int>>> &data_v1, vector<vector<string>> &data_v2, vector<vector<vector<string>>> &data_v3, vector<pair<pair<string, pair<int, int>>, vector<string>>> &data_v4, int &classs, int teacher) // разместить все уроки 1 версия
{
    vector<vector<vector<string>>> schedule(classs);
    vector<vector<vector<string>>> pr_schedule(classs);
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
    pr_schedule = schedule_v2(schedule, data_v1, data_v2, teacher, classs, data_v4);
    save_schedule_to_file(pr_schedule, data_v2);
    return pr_schedule;
}

vector<vector<vector<string>>> schedule(vector<vector<pair<string, int>>> &data_v1, vector<vector<string>> &data_v2, vector<vector<vector<string>>> &data_v3, vector<pair<pair<string, pair<int, int>>, vector<string>>> &data_v4, int &classs, int teacher)
{
    vector<vector<vector<string>>> schedule_v1 = post_all_lessons(data_v1, data_v2, data_v3, data_v4, classs, teacher);
    return schedule_v1;
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

vector<pair<pair<string, pair<int, int>>, vector<string>>> data_4v(int teacher)
{
    ifstream file4("4file_project.txt");
    int kol;
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
    int classs, teacher;

    ifstream file1("1file_project.txt");
    file1 >> classs;

    vector<vector<pair<string, int>>> data_v1;
    data_v1 = data_1v(classs, file1);
    vector<vector<string>> data_v2;
    data_v2 = data_2v(classs);
    vector<vector<vector<string>>> data_v3;
    data_v3 = data_3v(classs);

    ifstream file4("4file_project.txt");
    file4 >> teacher;

    vector<pair<pair<string, pair<int, int>>, vector<string>>> data_v4;
    data_v4 = data_4v(teacher);
    vector<vector<vector<string>>> otv;
    otv = schedule(data_v1, data_v2, data_v3, data_v4, classs, teacher);
}
