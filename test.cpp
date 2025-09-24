    vector<vector<string>> schedule_1v(kol_day);
    vector<vector<int>> fine(kol_day);
    for (int i = 0; i < schedule_data_v1.size(); i++)
    {
        int fine1 = 0, fine2 = 0;
        int d1 = kol_day - schedule_data_v2.second, d2 = schedule_data_v2.second, f1 = kol_day - schedule_data_v3.second, f2 = schedule_data_v3.second;
        int qday;
        for (int j = 0; j < kol_day; j++)
        {
            fine[j] = vector<int>(3, 1000);
            for (int q = 0; q < schedule_data_v1[i].size(); q++)
            {
                if (schedule_data_v2.first < schedule_1v[schedule_data_v1[i][q] - 1].size() && d1 >= 0)
                {
                    fine1 += 4 * (schedule_1v[schedule_data_v1[i][q] - 1].size() - schedule_data_v2.first);
                    d1--;
                }
                else if (schedule_data_v2.first + 1 < schedule_1v[schedule_data_v1[i][q] - 1].size() && d2 >= 0)
                {
                    fine1 += 4 * (schedule_1v[schedule_data_v1[i][q] - 1].size() - schedule_data_v2.first + 1);
                    d2--;
                }
                else if (schedule_data_v2.first + 1 == schedule_1v[schedule_data_v1[i][q] - 1].size())
                    d2--;
                else if (schedule_data_v2.first == schedule_1v[schedule_data_v1[i][q] - 1].size())
                    d1--;
            }
            if (fine1 + fine2 < fine[j][0] + fine[j][1])
            {
                fine[j][0] += fine1;
                fine[j][1] += fine2;
                qday = j;
            }
            for (int q = 0; q < schedule_data_v1[i].size(); q++)
            {
                if (schedule_data_v1[i][q] == kol_day)
                    schedule_data_v1[i][q] = 1;
                else
                    schedule_data_v1[i][q]++;
            }
        }
        for (int q = 0; q < schedule_data_v1[i].size(); q++)
        {
            schedule_1v[((schedule_data_v1[i][q] + qday) % kol_day) - 1].push_back(data_v1[i].first);
        }
    }
    return schedule_1v;









for (int j = 0; j < kol_day; j++)
        {
            int d1 = kol_day - schedule_data_v2.second, d2 = schedule_data_v2.second;
            int f1 = kol_day - schedule_data_v3.second, f2 = schedule_data_v3.second;
            int fine1 = 0, fine2 = 0;
            // 1. Штраф за превышение количества уроков
            for (int q = 0; q < schedule_data_v1[i].size(); q++)
            {
                if (schedule_1v[schedule_data_v1[i][q] - 1].size() + 1 > schedule_data_v2.first)
                {
                    // if (d1 > 0)
                    //     d1--;
                    // else if (d2 > 0)
                    //     d2--;
                    // if (d1 < 0 || d2 < 0)
                    fine1 += 3;
                }
            }
            // 2. Штраф за превышение балла сложности
            for (int q = 0; q < schedule_data_v1[i].size(); q++)
            {
                if (f[schedule_data_v1[i][q]] > schedule_data_v3.first)
                {
                    // if (f1 > 0)
                    //     f1--;
                    // else if (f2 > 0)
                    //     f2--;
                    // if (f1 < 0 || f2 < 0)
                    fine2 += 3;
                }
            }
            // 3. Штраф за много уроков одного типа
            // 4. лучший день + следующий день
            if (fine1 + fine2 < good_day)
                good_day = j;
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
            schedule_1v[((schedule_data_v1[i][j] + good_day) % kol_day)].push_back(data_v1[i].first);
            f[((schedule_data_v1[i][j] + good_day) % kol_day)] += data_v1[i].second;
        }