#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <climits>
#include <numeric>
#include <unordered_set>
#include <functional> // Добавляем этот заголовок

using namespace std;

struct DaySchedule {
    vector<int> lessons;
    int total_lessons = 0;
    int total_difficulty = 0;
    map<string, int> subject_count;
};

int calculate_penalty(const vector<DaySchedule>& schedule, 
                     const pair<int, int>& schedule_data_v2,
                     const pair<int, int>& schedule_data_v3,
                     int kol_day,
                     const vector<pair<string, int>>& data_v1) {
    
    int penalty = 0;
    
    // 1. Штраф за превышение количества уроков
    vector<int> lesson_counts;
    for (const auto& day : schedule) {
        lesson_counts.push_back(day.total_lessons);
    }
    
    sort(lesson_counts.begin(), lesson_counts.end());
    
    int target_lessons = schedule_data_v2.first;
    int days_with_extra = schedule_data_v2.second;
    
    for (int i = 0; i < kol_day; i++) {
        int expected = (i < kol_day - days_with_extra) ? target_lessons : target_lessons + 1;
        if (lesson_counts[i] > expected) {
            penalty += 3 * (lesson_counts[i] - expected);
        }
    }
    
    // 2. Штраф за превышение балла сложности
    vector<int> difficulty_scores;
    for (const auto& day : schedule) {
        difficulty_scores.push_back(day.total_difficulty);
    }
    
    sort(difficulty_scores.begin(), difficulty_scores.end());
    
    int target_difficulty = schedule_data_v3.first;
    int days_with_extra_diff = schedule_data_v3.second;
    
    for (int i = 0; i < kol_day; i++) {
        int expected = (i < kol_day - days_with_extra_diff) ? target_difficulty : target_difficulty + 1;
        if (difficulty_scores[i] > expected) {
            penalty += (difficulty_scores[i] - expected);
        }
    }
    
    // 3. Штраф за много уроков одного типа
    for (const auto& day : schedule) {
        for (const auto& subject : day.subject_count) {
            if (subject.second > 1) { // Если больше одного урока одного типа
                penalty += 5 * (subject.second - 1);
            }
        }
    }
    
    return penalty;
}

vector<vector<int>> find_optimal_schedule(
    const vector<vector<int>>& schedule_data_v1,
    const pair<int, int>& schedule_data_v2,
    const pair<int, int>& schedule_data_v3,
    int kol_day,
    const vector<pair<string, int>>& data_v1) {
    
    int total_lessons = schedule_data_v1.size();
    vector<vector<int>> best_schedule(kol_day);
    int min_penalty = INT_MAX;
    
    // Используем function с правильным синтаксисом
    function<void(int, vector<DaySchedule>&)> backtrack;
    
    backtrack = [&](int lesson_idx, vector<DaySchedule>& current_schedule) {
        if (lesson_idx == total_lessons) {
            int penalty = calculate_penalty(current_schedule, schedule_data_v2, schedule_data_v3, kol_day, data_v1);
            if (penalty < min_penalty) {
                min_penalty = penalty;
                for (int i = 0; i < kol_day; i++) {
                    best_schedule[i] = current_schedule[i].lessons;
                }
            }
            return;
        }
        
        // Пробуем поставить урок в каждый из допустимых дней
        for (int day : schedule_data_v1[lesson_idx]) {
            if (day >= 0 && day < kol_day) {
                // Обновляем информацию о дне
                string subject_name = data_v1[lesson_idx].first;
                int difficulty = data_v1[lesson_idx].second;
                
                current_schedule[day].lessons.push_back(lesson_idx);
                current_schedule[day].total_lessons++;
                current_schedule[day].total_difficulty += difficulty;
                current_schedule[day].subject_count[subject_name]++;
                
                backtrack(lesson_idx + 1, current_schedule);
                
                // Откатываем изменения
                current_schedule[day].lessons.pop_back();
                current_schedule[day].total_lessons--;
                current_schedule[day].total_difficulty -= difficulty;
                current_schedule[day].subject_count[subject_name]--;
                if (current_schedule[day].subject_count[subject_name] == 0) {
                    current_schedule[day].subject_count.erase(subject_name);
                }
            }
        }
    };
    
    // Инициализируем расписание
    vector<DaySchedule> initial_schedule(kol_day);
    backtrack(0, initial_schedule);
    
    return best_schedule;
}

// Вспомогательная функция для вывода результата
void print_schedule(const vector<vector<int>>& schedule, 
                   const vector<pair<string, int>>& data_v1) {
    cout << "Оптимальное расписание по дням:" << endl;
    for (int i = 0; i < schedule.size(); i++) {
        cout << "День " << i + 1 << ": ";
        for (int lesson_id : schedule[i]) {
            cout << data_v1[lesson_id].first << "(" << data_v1[lesson_id].second << ") ";
        }
        cout << endl;
    }
}

int main() {
    // Пример использования
    vector<vector<int>> schedule_data_v1 = {
        {1, 2, 3},    // Урок 0 можно в дни 1, 2, 3
        {0, 2, 4},    // Урок 1 можно в дни 0, 2, 4
        {1, 3},       // Урок 2 можно в дни 1, 3
        {0, 2, 4},    // Урок 3 можно в дни 0, 2, 4
        {1, 3}        // Урок 4 можно в дни 1, 3
    };
    
    pair<int, int> schedule_data_v2 = {2, 1}; // Среднее 2 урока, 1 день с +1
    pair<int, int> schedule_data_v3 = {8, 2}; // Средняя сложность 8, 2 дня с +1
    int kol_day = 5;
    
    vector<pair<string, int>> data_v1 = {
        {"Математика", 5},
        {"Физика", 4},
        {"Химия", 3},
        {"История", 2},
        {"Литература", 2}
    };
    
    vector<vector<int>> optimal_schedule = find_optimal_schedule(
        schedule_data_v1, schedule_data_v2, schedule_data_v3, kol_day, data_v1);
    
    print_schedule(optimal_schedule, data_v1);
    
    return 0;
}