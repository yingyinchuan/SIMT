#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

struct Student
{
    std::string name;
    std::vector<int> scores;
    double average;
    bool hasFailed;

    Student(const std::string &n, const std::vector<int> &s) : name(n), scores(s), average(0.0), hasFailed(false) {}
};

// 计算平均分并标记不及格
void calculateAverageAndMarkFailures(Student &student)
{
    int sum = 0;
    int numSubjects = student.scores.size();

    for (int score : student.scores)
    {
        sum += score;
        if (score < 60)
        {
            student.hasFailed = true;
        }
    }

    student.average = static_cast<double>(sum) / numSubjects;
}

// 排序比较函数
bool compareStudents(const Student &s1, const Student &s2)
{
    return s1.average > s2.average;
}

int main()
{
    int numStudents, numSubjects;

    // 输入学生和科目数量
    std::cout << "Enter the number of students: ";
    std::cin >> numStudents;
    std::cout << "Enter the number of subjects: ";
    std::cin >> numSubjects;

    // 输入学生成绩数据
    std::vector<Student> students;
    for (int i = 0; i < numStudents; ++i)
    {
        std::string name;
        std::cout << "Enter name of student " << i + 1 << ": ";
        std::cin >> name;

        std::vector<int> scores;
        std::cout << "Enter scores for " << name << " (separated by space): ";
        for (int j = 0; j < numSubjects; ++j)
        {
            int score;
            std::cin >> score;
            scores.push_back(score);
        }

        students.emplace_back(name, scores);
        calculateAverageAndMarkFailures(students[i]);
    }

    // 排序
    std::cout << "Choose sorting method:\n";
    std::cout << "1. Sort by average descending\n";
    std::cout << "2. Sort by average ascending\n";
    int sortingOption;
    std::cin >> sortingOption;

    if (sortingOption == 1)
    {
        std::sort(students.begin(), students.end(), compareStudents);
    }
    else if (sortingOption == 2)
    {
        std::sort(students.begin(), students.end(), [](const Student &s1, const Student &s2)
                  { return s1.average < s2.average; });
    }

    // 输出平均分前5名的学生信息
    std::cout << "Top 5 students:\n";
    for (int i = 0; i < std::min(5, numStudents); ++i)
    {
        if (!students[i].hasFailed)
        {
            std::cout << std::setw(10) << students[i].name << " : " << std::fixed << std::setprecision(2) << students[i].average << "\n";
        }
    }

    // 输出含有不及格科目的学生信息
    std::cout << "Do you want to display students with failing grades? (1 for yes, 0 for no): ";
    int displayFailures;
    std::cin >> displayFailures;

    if (displayFailures == 1)
    {
        std::cout << "Students with failing grades:\n";
        for (const Student &student : students)
        {
            if (student.hasFailed)
            {
                std::cout << std::setw(10) << student.name << " : " << std::fixed << std::setprecision(2) << student.average << "\n";
            }
        }
    }

    return 0;
}
