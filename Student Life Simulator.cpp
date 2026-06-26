#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iomanip>

// ========== 课程类 ==========
class Course {
private:
    std::string name;
    int credit;
    int score;
    std::string grade;

public:
    Course() : name(""), credit(3), score(0), grade("F") {}
    Course(std::string n, int c) : name(n), credit(c), score(0), grade("F") {}

    void setScore(int s) {
        score = s;
        if (score >= 90) grade = "A";
        else if (score >= 80) grade = "B";
        else if (score >= 70) grade = "C";
        else if (score >= 60) grade = "D";
        else grade = "F";
    }

    std::string getName() const { return name; }
    int getCredit() const { return credit; }
    int getScore() const { return score; }
    std::string getGrade() const { return grade; }

    void display() const {
        std::cout << "  " << std::left << std::setw(15) << name
                  << " 学分: " << credit
                  << "  成绩: " << std::setw(3) << score
                  << "  等级: " << grade << std::endl;
    }
};

// ========== 学生类 ==========
class Student {
private:
    std::string name;
    int age;
    std::string major;
    int semester;
    double gpa;
    int energy;
    int happiness;
    std::vector<Course> courses;
    std::vector<std::string> skills;
    std::mt19937 rng;

    double calculateGPA() const {
        if (courses.empty()) return 0.0;
        double totalPoints = 0.0;
        int totalCredits = 0;
        for (const auto& course : courses) {
            std::string grade = course.getGrade();
            double points = 0.0;
            if (grade == "A") points = 4.0;
            else if (grade == "B") points = 3.0;
            else if (grade == "C") points = 2.0;
            else if (grade == "D") points = 1.0;
            else points = 0.0;
            
            totalPoints += points * course.getCredit();
            totalCredits += course.getCredit();
        }
        return (totalCredits > 0) ? totalPoints / totalCredits : 0.0;
    }

public:
    Student(std::string n, int a, std::string m) 
        : name(n), age(a), major(m), semester(1), gpa(0.0), energy(80), happiness(70) {
        rng.seed(std::time(nullptr) + std::hash<std::string>{}(name));
        skills.push_back("学习基础");
    }

    // ========== 新增公共访问方法 ==========
    std::vector<Course>& getCourses() { return courses; }
    const std::vector<Course>& getCourses() const { return courses; }
    std::vector<std::string>& getSkills() { return skills; }
    const std::vector<std::string>& getSkills() const { return skills; }

    void takeCourse(const Course& course) {
        courses.push_back(course);
        std::cout << name << " 选修了课程: " << course.getName() << std::endl;
        energy -= 5;
        if (energy < 0) energy = 0;
    }

    void studyForExam(const std::string& courseName, int hours) {
        std::cout << "\n📚 " << name << " 为 " << courseName << " 学习了 " << hours << " 小时" << std::endl;
        
        for (auto& course : courses) {
            if (course.getName() == courseName) {
                std::uniform_int_distribution<int> baseDist(50, 70);
                int baseScore = baseDist(rng);
                int bonus = std::min(hours * 3, 30);
                
                if (energy < 30) {
                    bonus = bonus * 0.5;
                    std::cout << "⚠️ 精力不足，学习效果减半！" << std::endl;
                }
                
                int finalScore = std::min(baseScore + bonus, 100);
                course.setScore(finalScore);
                
                energy -= hours * 2;
                if (energy < 0) energy = 0;
                
                happiness += 3;
                if (happiness > 100) happiness = 100;
                
                std::cout << "📊 考试成绩: " << finalScore << " (" << course.getGrade() << ")" << std::endl;
                
                if (finalScore >= 85) {
                    std::vector<std::string> possibleSkills = {
                        "编程能力", "数学思维", "英语应用", "数据分析", "项目管理"
                    };
                    std::uniform_int_distribution<int> skillDist(0, possibleSkills.size() - 1);
                    std::string newSkill = possibleSkills[skillDist(rng)];
                    
                    if (std::find(skills.begin(), skills.end(), newSkill) == skills.end()) {
                        skills.push_back(newSkill);
                        std::cout << "🎯 获得了新技能: " << newSkill << "！" << std::endl;
                    }
                }
                break;
            }
        }
        gpa = calculateGPA();
    }

    void examWeek() {
        std::cout << "\n===== 📝 " << name << " 的考试周 =====" << std::endl;
        for (auto& course : courses) {
            if (course.getScore() == 0) {
                std::cout << "📖 正在考 " << course.getName() << " ..." << std::endl;
                std::uniform_int_distribution<int> studyDist(1, 3);
                int hours = studyDist(rng);
                studyForExam(course.getName(), hours);
            }
        }
    }

    void sleep() {
        std::cout << "\n😴 " << name << " 睡了一觉" << std::endl;
        energy = std::min(energy + 40, 100);
        happiness += 5;
        if (happiness > 100) happiness = 100;
        std::cout << "精力恢复到: " << energy << ", 快乐: " << happiness << std::endl;
    }

    void eat() {
        std::cout << "\n🍜 " << name << " 吃了顿饭" << std::endl;
        energy = std::min(energy + 20, 100);
        happiness += 8;
        if (happiness > 100) happiness = 100;
        std::cout << "精力: " << energy << ", 快乐: " << happiness << std::endl;
    }

    void socialize() {
        std::cout << "\n🎉 " << name << " 和朋友聚会" << std::endl;
        happiness = std::min(happiness + 20, 100);
        energy -= 10;
        if (energy < 0) energy = 0;
        std::cout << "快乐: " << happiness << ", 精力: " << energy << std::endl;
    }

    void exercise() {
        std::cout << "\n🏃 " << name << " 去运动了" << std::endl;
        energy = std::min(energy + 15, 100);
        happiness += 10;
        if (happiness > 100) happiness = 100;
        std::cout << "精力: " << energy << ", 快乐: " << happiness << std::endl;
    }

    void slackOff() {
        std::cout << "\n📱 " << name << " 在摸鱼..." << std::endl;
        happiness += 5;
        if (happiness > 100) happiness = 100;
        energy -= 5;
        if (energy < 0) energy = 0;
        std::cout << "快乐: " << happiness << ", 精力: " << energy << std::endl;
    }

    void displayStatus() const {
        std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
        std::cout << "║          📊 " << name << " 的状态          ║" << std::endl;
        std::cout << "╠════════════════════════════════════════╣" << std::endl;
        std::cout << "║  年龄: " << std::left << std::setw(12) << age 
                  << "  学期: " << semester << std::endl;
        std::cout << "║  专业: " << std::left << std::setw(12) << major 
                  << "  GPA: " << std::fixed << std::setprecision(2) << gpa << std::endl;
        std::cout << "║  精力: " << std::setw(3) << energy << "/100" 
                  << "  快乐: " << std::setw(3) << happiness << "/100" << std::endl;
        std::cout << "╠════════════════════════════════════════╣" << std::endl;
        
        std::cout << "║  已修课程:" << std::endl;
        if (courses.empty()) {
            std::cout << "║    (暂无)" << std::endl;
        } else {
            for (const auto& course : courses) {
                std::cout << "║    " << std::left << std::setw(12) << course.getName()
                          << "  " << course.getGrade() << " (" << course.getScore() << "分)" << std::endl;
            }
        }
        
        std::cout << "╠════════════════════════════════════════╣" << std::endl;
        std::cout << "║  技能: ";
        if (skills.empty()) {
            std::cout << "(无)" << std::endl;
        } else {
            std::cout << std::endl;
            for (const auto& skill : skills) {
                std::cout << "║    ⭐ " << skill << std::endl;
            }
        }
        std::cout << "╚════════════════════════════════════════╝" << std::endl;
    }

    void nextSemester() {
        semester++;
        std::cout << "\n📅 进入第 " << semester << " 学期！" << std::endl;
        energy = std::min(energy + 20, 100);
        happiness = std::min(happiness + 10, 100);
    }

    std::string getName() const { return name; }
    int getEnergy() const { return energy; }
    int getHappiness() const { return happiness; }
    double getGPA() const { return gpa; }
    int getSemester() const { return semester; }

    bool canGraduate() const {
        return semester >= 8 && gpa >= 2.0 && courses.size() >= 20;
    }
};

// ========== 模拟器主类 ==========
class StudentSimulator {
private:
    Student student;
    std::vector<Course> availableCourses;
    int day;

public:
    StudentSimulator(const Student& s) : student(s), day(0) {
        availableCourses.push_back(Course("高等数学", 4));
        availableCourses.push_back(Course("C++编程", 3));
        availableCourses.push_back(Course("数据结构", 3));
        availableCourses.push_back(Course("操作系统", 3));
        availableCourses.push_back(Course("计算机网络", 3));
        availableCourses.push_back(Course("数据库原理", 3));
        availableCourses.push_back(Course("软件工程", 2));
        availableCourses.push_back(Course("人工智能", 4));
        availableCourses.push_back(Course("机器学习", 3));
        availableCourses.push_back(Course("英语", 2));
    }

    void showMenu() const {
        std::cout << "\n═══════════════════════════════════════" << std::endl;
        std::cout << "      🎓 学生生活模拟器 v1.0" << std::endl;
        std::cout << "═══════════════════════════════════════" << std::endl;
        std::cout << "  1. 📚 查看状态" << std::endl;
        std::cout << "  2. 📖 选修课程" << std::endl;
        std::cout << "  3. 📝 学习/考试" << std::endl;
        std::cout << "  4. 🏃 日常活动" << std::endl;
        std::cout << "  5. 🎯 进入考试周" << std::endl;
        std::cout << "  6. 📅 推进到下学期" << std::endl;
        std::cout << "  7. 💤 休息一天" << std::endl;
        std::cout << "  8. 🎮 摸鱼" << std::endl;
        std::cout << "  9. 🎓 尝试毕业" << std::endl;
        std::cout << "  0. 🚪 退出" << std::endl;
        std::cout << "═══════════════════════════════════════" << std::endl;
    }

    void run() {
        std::cout << "\n🎓 欢迎来到学生生活模拟器！" << std::endl;
        std::cout << "你将以 " << student.getName() << " 的身份度过大学生活。" << std::endl;
        std::cout << "合理分配精力和时间，努力学习，快乐生活！" << std::endl;

        int choice;
        do {
            student.displayStatus();
            showMenu();
            std::cout << "请选择操作: ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    break;
                case 2:
                    takeCourseMenu();
                    break;
                case 3:
                    studyMenu();
                    break;
                case 4:
                    dailyActivityMenu();
                    break;
                case 5:
                    student.examWeek();
                    break;
                case 6:
                    student.nextSemester();
                    break;
                case 7:
                    student.sleep();
                    day++;
                    break;
                case 8:
                    student.slackOff();
                    day++;
                    break;
                case 9:
                    tryGraduate();
                    break;
                case 0:
                    std::cout << "👋 再见！" << std::endl;
                    break;
                default:
                    std::cout << "❌ 无效选择，请重新输入。" << std::endl;
            }

            if (student.getHappiness() <= 0) {
                std::cout << "\n😢 " << student.getName() << " 太不开心了，退学了..." << std::endl;
                break;
            }
            if (student.getEnergy() <= 0) {
                std::cout << "\n😵 " << student.getName() << " 精力耗尽，累倒了..." << std::endl;
                break;
            }

        } while (choice != 0);
    }

private:
    void takeCourseMenu() {
        std::cout << "\n📚 可选课程:" << std::endl;
        for (size_t i = 0; i < availableCourses.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << availableCourses[i].getName() 
                      << " (学分: " << availableCourses[i].getCredit() << ")" << std::endl;
        }
        std::cout << "请输入课程编号 (0返回): ";
        int choice;
        std::cin >> choice;
        
        if (choice > 0 && choice <= static_cast<int>(availableCourses.size())) {
            student.takeCourse(availableCourses[choice - 1]);
        }
    }

    void studyMenu() {
        std::cout << "\n📖 选择要学习的课程 (输入课程名): " << std::endl;
        // 使用getCourses()访问
        auto& courses = student.getCourses();
        for (auto& course : courses) {
            if (course.getScore() == 0) {
                std::cout << "  正在学习: " << course.getName() << std::endl;
                int hours;
                std::cout << "学习几个小时? (1-5): ";
                std::cin >> hours;
                hours = std::max(1, std::min(hours, 5));
                student.studyForExam(course.getName(), hours);
                return;
            }
        }
        std::cout << "所有课程都已考完！" << std::endl;
    }

    void dailyActivityMenu() {
        std::cout << "\n🏃 日常活动:" << std::endl;
        std::cout << "  1. 吃饭" << std::endl;
        std::cout << "  2. 运动" << std::endl;
        std::cout << "  3. 社交" << std::endl;
        std::cout << "  4. 睡觉" << std::endl;
        std::cout << "选择活动: ";
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: student.eat(); break;
            case 2: student.exercise(); break;
            case 3: student.socialize(); break;
            case 4: student.sleep(); break;
            default: std::cout << "无效选择" << std::endl;
        }
        day++;
    }

    void tryGraduate() {
        if (student.canGraduate()) {
            std::cout << "\n🎉🎉🎉 恭喜 " << student.getName() << " 顺利毕业！！！" << std::endl;
            std::cout << "最终GPA: " << std::fixed << std::setprecision(2) << student.getGPA() << std::endl;
            std::cout << "掌握技能: ";
            // 使用getSkills()访问
            const auto& skills = student.getSkills();
            for (const auto& skill : skills) {
                std::cout << skill << " ";
            }
            std::cout << std::endl;
            std::cout << "🎊 你完成了大学学业，前途一片光明！" << std::endl;
            exit(0);
        } else {
            std::cout << "\n❌ 还不满足毕业条件:" << std::endl;
            if (student.getSemester() < 8) 
                std::cout << "  - 学期数不足 (需要8学期，当前" << student.getSemester() << ")" << std::endl;
            if (student.getGPA() < 2.0) 
                std::cout << "  - GPA不足 (需要2.0，当前" << std::fixed << std::setprecision(2) << student.getGPA() << ")" << std::endl;
            // 使用getCourses()访问
            if (student.getCourses().size() < 20) 
                std::cout << "  - 课程数不足 (需要20门，当前" << student.getCourses().size() << ")" << std::endl;
        }
    }
};

// ========== 主函数 ==========
int main() {
    std::cout << "🎓 学生生活模拟器" << std::endl;
    std::cout << "==================" << std::endl;
    
    std::string name, major;
    int age;
    
    std::cout << "请输入学生姓名: ";
    std::cin >> name;
    std::cout << "请输入年龄: ";
    std::cin >> age;
    std::cout << "请输入专业: ";
    std::cin >> major;
    
    Student student(name, age, major);
    StudentSimulator simulator(student);
    
    simulator.run();
    
    return 0;
}
