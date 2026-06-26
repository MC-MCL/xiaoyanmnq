#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

// ========== 跨平台字体加载工具 ==========
class FontLoader {
public:
    static bool loadFont(sf::Font& font) {
        // 跨平台字体路径列表
        std::vector<std::string> fontPaths = {
            // Windows
            "C:\\Windows\\Fonts\\msyh.ttc",
            "C:\\Windows\\Fonts\\msyhbd.ttc",
            "C:\\Windows\\Fonts\\simhei.ttf",
            "C:\\Windows\\Fonts\\simsun.ttc",
            "C:\\Windows\\Fonts\\arial.ttf",
            // Linux
            "/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc",
            "/usr/share/fonts/truetype/arphic/uming.ttc",
            "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc",
            "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
            // macOS
            "/System/Library/Fonts/PingFang.ttc",
            "/System/Library/Fonts/STHeiti Light.ttc",
            "/Library/Fonts/Arial Unicode.ttf",
            // 内置备用（使用SFML内置字体）
            ""
        };

        for (const auto& path : fontPaths) {
            if (path.empty()) {
                // 如果所有字体都失败，创建默认字体（可能不支持中文）
                sf::Font defaultFont;
                if (defaultFont.loadFromFile("")) {
                    font = defaultFont;
                    return true;
                }
                continue;
            }
            
            if (std::filesystem::exists(path)) {
                if (font.loadFromFile(path)) {
                    std::cout << "✅ 加载字体成功: " << path << std::endl;
                    return true;
                }
            }
        }

        // 最后尝试：从内存创建简单字体
        std::cerr << "⚠️ 警告: 无法加载字体文件，使用默认字体（可能不支持中文）" << std::endl;
        return false;
    }
};

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

    // 序列化
    std::string serialize() const {
        return name + "|" + std::to_string(credit) + "|" + std::to_string(score) + "|" + grade;
    }

    static Course deserialize(const std::string& data) {
        std::vector<std::string> parts;
        std::stringstream ss(data);
        std::string part;
        while (std::getline(ss, part, '|')) {
            parts.push_back(part);
        }
        if (parts.size() >= 4) {
            Course c(parts[0], std::stoi(parts[1]));
            c.setScore(std::stoi(parts[2]));
            return c;
        }
        return Course();
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
    float x, y;

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
        : name(n), age(a), major(m), semester(1), gpa(0.0), energy(80), happiness(70), x(400), y(300) {
        rng.seed(std::time(nullptr) + std::hash<std::string>{}(name));
        skills.push_back("学习基础");
    }

    // 默认构造函数（用于反序列化）
    Student() : name(""), age(18), major(""), semester(1), gpa(0.0), energy(80), happiness(70), x(400), y(300) {
        rng.seed(std::time(nullptr));
    }

    // ========== 位置控制 ==========
    void setPosition(float px, float py) { x = px; y = py; }
    float getX() const { return x; }
    float getY() const { return y; }
    void move(float dx, float dy) { x += dx; y += dy; }

    // ========== 访问方法 ==========
    std::vector<Course>& getCourses() { return courses; }
    const std::vector<Course>& getCourses() const { return courses; }
    std::vector<std::string>& getSkills() { return skills; }
    const std::vector<std::string>& getSkills() const { return skills; }

    // ========== 游戏方法 ==========
    void takeCourse(const Course& course) {
        // 检查是否已选过
        for (const auto& c : courses) {
            if (c.getName() == course.getName()) {
                return; // 已选过
            }
        }
        courses.push_back(course);
        energy -= 5;
        if (energy < 0) energy = 0;
    }

    void studyForExam(const std::string& courseName, int hours) {
        for (auto& course : courses) {
            if (course.getName() == courseName) {
                std::uniform_int_distribution<int> baseDist(50, 70);
                int baseScore = baseDist(rng);
                int bonus = std::min(hours * 3, 30);
                
                if (energy < 30) {
                    bonus = bonus * 0.5;
                }
                
                int finalScore = std::min(baseScore + bonus, 100);
                course.setScore(finalScore);
                
                energy -= hours * 2;
                if (energy < 0) energy = 0;
                
                happiness += 3;
                if (happiness > 100) happiness = 100;
                
                if (finalScore >= 85) {
                    std::vector<std::string> possibleSkills = {
                        "编程能力", "数学思维", "英语应用", "数据分析", "项目管理"
                    };
                    std::uniform_int_distribution<int> skillDist(0, possibleSkills.size() - 1);
                    std::string newSkill = possibleSkills[skillDist(rng)];
                    
                    if (std::find(skills.begin(), skills.end(), newSkill) == skills.end()) {
                        skills.push_back(newSkill);
                    }
                }
                break;
            }
        }
        gpa = calculateGPA();
    }

    void examWeek() {
        for (auto& course : courses) {
            if (course.getScore() == 0) {
                std::uniform_int_distribution<int> studyDist(1, 3);
                int hours = studyDist(rng);
                studyForExam(course.getName(), hours);
            }
        }
    }

    void sleep() {
        energy = std::min(energy + 40, 100);
        happiness += 5;
        if (happiness > 100) happiness = 100;
    }

    void eat() {
        energy = std::min(energy + 20, 100);
        happiness += 8;
        if (happiness > 100) happiness = 100;
    }

    void socialize() {
        happiness = std::min(happiness + 20, 100);
        energy -= 10;
        if (energy < 0) energy = 0;
    }

    void exercise() {
        energy = std::min(energy + 15, 100);
        happiness += 10;
        if (happiness > 100) happiness = 100;
    }

    void slackOff() {
        happiness += 5;
        if (happiness > 100) happiness = 100;
        energy -= 5;
        if (energy < 0) energy = 0;
    }

    void nextSemester() {
        semester++;
        energy = std::min(energy + 20, 100);
        happiness = std::min(happiness + 10, 100);
        gpa = calculateGPA();
    }

    // ========== Getter/Setter ==========
    std::string getName() const { return name; }
    void setName(const std::string& n) { name = n; }
    int getEnergy() const { return energy; }
    int getHappiness() const { return happiness; }
    double getGPA() const { return gpa; }
    int getSemester() const { return semester; }
    int getAge() const { return age; }
    std::string getMajor() const { return major; }

    bool canGraduate() const {
        return semester >= 8 && gpa >= 2.0 && courses.size() >= 20;
    }

    // ========== 存档系统 ==========
    std::string serialize() const {
        std::stringstream ss;
        ss << "NAME:" << name << "\n";
        ss << "AGE:" << age << "\n";
        ss << "MAJOR:" << major << "\n";
        ss << "SEMESTER:" << semester << "\n";
        ss << "GPA:" << gpa << "\n";
        ss << "ENERGY:" << energy << "\n";
        ss << "HAPPINESS:" << happiness << "\n";
        ss << "POSX:" << x << "\n";
        ss << "POSY:" << y << "\n";
        ss << "COURSES:" << courses.size() << "\n";
        for (const auto& course : courses) {
            ss << "COURSE:" << course.serialize() << "\n";
        }
        ss << "SKILLS:" << skills.size() << "\n";
        for (const auto& skill : skills) {
            ss << "SKILL:" << skill << "\n";
        }
        return ss.str();
    }

    bool deserialize(const std::string& data) {
        try {
            std::stringstream ss(data);
            std::string line;
            std::vector<Course> loadedCourses;
            std::vector<std::string> loadedSkills;
            
            while (std::getline(ss, line)) {
                if (line.find("NAME:") == 0) name = line.substr(5);
                else if (line.find("AGE:") == 0) age = std::stoi(line.substr(4));
                else if (line.find("MAJOR:") == 0) major = line.substr(6);
                else if (line.find("SEMESTER:") == 0) semester = std::stoi(line.substr(9));
                else if (line.find("GPA:") == 0) gpa = std::stod(line.substr(4));
                else if (line.find("ENERGY:") == 0) energy = std::stoi(line.substr(7));
                else if (line.find("HAPPINESS:") == 0) happiness = std::stoi(line.substr(10));
                else if (line.find("POSX:") == 0) x = std::stof(line.substr(5));
                else if (line.find("POSY:") == 0) y = std::stof(line.substr(5));
                else if (line.find("COURSE:") == 0) {
                    loadedCourses.push_back(Course::deserialize(line.substr(7)));
                }
                else if (line.find("SKILL:") == 0) {
                    loadedSkills.push_back(line.substr(6));
                }
            }
            
            if (!loadedCourses.empty()) courses = loadedCourses;
            if (!loadedSkills.empty()) skills = loadedSkills;
            
            return true;
        } catch (...) {
            return false;
        }
    }
};

// ========== 存档管理器 ==========
class SaveManager {
private:
    std::string saveDirectory;

public:
    SaveManager() {
        // 跨平台存档目录
        #ifdef _WIN32
            saveDirectory = std::string(getenv("APPDATA")) + "\\StudentSimulator\\saves\\";
        #elif __APPLE__
            saveDirectory = std::string(getenv("HOME")) + "/Library/Application Support/StudentSimulator/saves/";
        #else
            saveDirectory = std::string(getenv("HOME")) + "/.student_simulator/saves/";
        #endif
        
        // 创建目录
        std::filesystem::create_directories(saveDirectory);
    }

    bool save(const Student& student, const std::string& slotName = "autosave") {
        std::string filename = saveDirectory + slotName + ".sav";
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        
        file << student.serialize();
        file.close();
        return true;
    }

    bool load(Student& student, const std::string& slotName = "autosave") {
        std::string filename = saveDirectory + slotName + ".sav";
        if (!std::filesystem::exists(filename)) return false;
        
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        
        return student.deserialize(ss.str());
    }

    std::vector<std::string> listSaves() {
        std::vector<std::string> saves;
        try {
            for (const auto& entry : std::filesystem::directory_iterator(saveDirectory)) {
                if (entry.path().extension() == ".sav") {
                    saves.push_back(entry.path().stem().string());
                }
            }
        } catch (...) {}
        return saves;
    }

    bool deleteSave(const std::string& slotName) {
        std::string filename = saveDirectory + slotName + ".sav";
        if (!std::filesystem::exists(filename)) return false;
        return std::filesystem::remove(filename);
    }
};

// ========== 2D图形游戏类 ==========
class GraphicsStudentSimulator {
private:
    sf::RenderWindow window;
    Student student;
    SaveManager saveManager;
    std::vector<Course> availableCourses;
    std::vector<std::string> messages;
    sf::Font font;
    bool isRunning;
    int selectedOption;
    std::string inputBuffer;
    bool isInputting;
    std::string inputPrompt;
    std::string inputCallback; // 用于区分输入用途

    // UI元素
    sf::RectangleShape statusPanel;
    sf::RectangleShape menuPanel;
    sf::RectangleShape messagePanel;
    sf::RectangleShape characterSprite;

    // 校园建筑
    sf::RectangleShape dormitory;
    sf::RectangleShape library;
    sf::RectangleShape classroom;
    sf::RectangleShape canteen;
    sf::RectangleShape gym;
    sf::CircleShape studentIcon;

    // 建筑标签
    sf::Text dormLabel, libLabel, classLabel, canteenLabel, gymLabel;

    sf::Color bgColor = sf::Color(34, 139, 34);
    sf::Color panelColor = sf::Color(50, 50, 50, 230);
    sf::Color textColor = sf::Color::White;

public:
    GraphicsStudentSimulator(const Student& s) 
        : student(s), isRunning(true), selectedOption(0), isInputting(false) {
        
        window.create(sf::VideoMode(1024, 768), "🎓 学生生活模拟器 v2.0", sf::Style::Close);
        window.setFramerateLimit(60);

        // 加载字体（带异常处理）
        try {
            if (!FontLoader::loadFont(font)) {
                throw std::runtime_error("无法加载字体");
            }
        } catch (const std::exception& e) {
            std::cerr << "字体加载失败: " << e.what() << std::endl;
            // 创建备用字体（不支持中文）
            if (!font.loadFromFile("")) {
                std::cerr << "严重错误：无法创建备用字体" << std::endl;
            }
        }

        // 初始化课程库
        initCourses();

        messages.push_back("🎓 欢迎来到学生生活模拟器 v2.0");
        messages.push_back("🏠 使用方向键移动角色");
        messages.push_back("📋 靠近建筑按 Enter 交互");
        messages.push_back("⌨️ 按 1-9 执行菜单操作");
        messages.push_back("💾 按 S 存档 | L 读档");
        messages.push_back("🔄 按 R 修改角色名");

        setupUI();
        setupBuildingLabels();

        // 自动尝试加载存档
        if (saveManager.load(student)) {
            messages.push_back("💾 自动加载存档成功");
        }
    }

    void initCourses() {
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

    void setupUI() {
        // 状态面板
        statusPanel.setSize(sf::Vector2f(300, 200));
        statusPanel.setPosition(sf::Vector2f(10, 10));
        statusPanel.setFillColor(panelColor);
        statusPanel.setOutlineColor(sf::Color::White);
        statusPanel.setOutlineThickness(1);

        // 菜单面板
        menuPanel.setSize(sf::Vector2f(300, 400));
        menuPanel.setPosition(sf::Vector2f(10, 220));
        menuPanel.setFillColor(panelColor);
        menuPanel.setOutlineColor(sf::Color::White);
        menuPanel.setOutlineThickness(1);

        // 消息面板
        messagePanel.setSize(sf::Vector2f(700, 150));
        messagePanel.setPosition(sf::Vector2f(320, 600));
        messagePanel.setFillColor(panelColor);
        messagePanel.setOutlineColor(sf::Color::White);
        messagePanel.setOutlineThickness(1);

        // 学生图标
        studentIcon.setRadius(15);
        studentIcon.setFillColor(sf::Color::Cyan);
        studentIcon.setPosition(student.getX(), student.getY());
        studentIcon.setOutlineColor(sf::Color::White);
        studentIcon.setOutlineThickness(2);

        // 校园建筑
        dormitory.setSize(sf::Vector2f(80, 60));
        dormitory.setPosition(100, 500);
        dormitory.setFillColor(sf::Color(139, 69, 19));
        dormitory.setOutlineColor(sf::Color::White);
        dormitory.setOutlineThickness(1);

        library.setSize(sf::Vector2f(80, 60));
        library.setPosition(250, 500);
        library.setFillColor(sf::Color(70, 130, 180));
        library.setOutlineColor(sf::Color::White);
        library.setOutlineThickness(1);

        classroom.setSize(sf::Vector2f(80, 60));
        classroom.setPosition(400, 500);
        classroom.setFillColor(sf::Color(255, 140, 0));
        classroom.setOutlineColor(sf::Color::White);
        classroom.setOutlineThickness(1);

        canteen.setSize(sf::Vector2f(80, 60));
        canteen.setPosition(550, 500);
        canteen.setFillColor(sf::Color(255, 69, 0));
        canteen.setOutlineColor(sf::Color::White);
        canteen.setOutlineThickness(1);

        gym.setSize(sf::Vector2f(80, 60));
        gym.setPosition(700, 500);
        gym.setFillColor(sf::Color(50, 205, 50));
        gym.setOutlineColor(sf::Color::White);
        gym.setOutlineThickness(1);
    }

    void setupBuildingLabels() {
        dormLabel.setFont(font);
        dormLabel.setString("🏠 宿舍");
        dormLabel.setCharacterSize(14);
        dormLabel.setFillColor(sf::Color::White);
        dormLabel.setPosition(110, 570);

        libLabel.setFont(font);
        libLabel.setString("📚 图书馆");
        libLabel.setCharacterSize(14);
        libLabel.setFillColor(sf::Color::White);
        libLabel.setPosition(260, 570);

        classLabel.setFont(font);
        classLabel.setString("🏫 教室");
        classLabel.setCharacterSize(14);
        classLabel.setFillColor(sf::Color::White);
        classLabel.setPosition(415, 570);

        canteenLabel.setFont(font);
        canteenLabel.setString("🍜 食堂");
        canteenLabel.setCharacterSize(14);
        canteenLabel.setFillColor(sf::Color::White);
        canteenLabel.setPosition(565, 570);

        gymLabel.setFont(font);
        gymLabel.setString("💪 健身房");
        gymLabel.setCharacterSize(14);
        gymLabel.setFillColor(sf::Color::White);
        gymLabel.setPosition(710, 570);
    }

    void run() {
        while (window.isOpen() && isRunning) {
            handleEvents();
            update();
            render();
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                autoSave();
                window.close();
                isRunning = false;
            }

            if (event.type == sf::Event::KeyPressed) {
                handleKeyPress(event.key.code);
            }

            if (event.type == sf::Event::TextEntered && isInputting) {
                if (event.text.unicode == 13) { // Enter
                    processInput();
                } else if (event.text.unicode == 8) { // Backspace
                    if (!inputBuffer.empty()) inputBuffer.pop_back();
                } else if (event.text.unicode < 128) {
                    inputBuffer += static_cast<char>(event.text.unicode);
                }
            }
        }
    }

    void handleKeyPress(sf::Keyboard::Key key) {
        // 如果正在输入，优先处理输入
        if (isInputting) return;

        float speed = 5.0f;
        if (key == sf::Keyboard::Up) student.move(0, -speed);
        if (key == sf::Keyboard::Down) student.move(0, speed);
        if (key == sf::Keyboard::Left) student.move(-speed, 0);
        if (key == sf::Keyboard::Right) student.move(speed, 0);

        // 菜单快捷键（角色位置不变，但执行功能）
        if (key >= sf::Keyboard::Num1 && key <= sf::Keyboard::Num9) {
            int option = key - sf::Keyboard::Num1 + 1;
            executeMenuOption(option);
        }

        // 交互键 (Enter)
        if (key == sf::Keyboard::Enter) {
            interactWithEnvironment();
        }

        // 存档 (S)
        if (key == sf::Keyboard::S) {
            if (saveManager.save(student)) {
                addMessage("💾 游戏已存档");
            } else {
                addMessage("❌ 存档失败");
            }
        }

        // 读档 (L)
        if (key == sf::Keyboard::L) {
            if (saveManager.load(student)) {
                addMessage("💾 读档成功");
                studentIcon.setPosition(student.getX(), student.getY());
            } else {
                addMessage("❌ 读档失败");
            }
        }

        // 改名 (R)
        if (key == sf::Keyboard::R) {
            startInput("请输入新名字:", "rename");
        }

        // ESC退出
        if (key == sf::Keyboard::Escape) {
            autoSave();
            window.close();
            isRunning = false;
        }
    }

    void startInput(const std::string& prompt, const std::string& callback) {
        isInputting = true;
        inputPrompt = prompt;
        inputCallback = callback;
        inputBuffer.clear();
        addMessage("✏️ " + prompt);
    }

    void processInput() {
        isInputting = false;
        
        if (inputCallback == "rename") {
            if (!inputBuffer.empty()) {
                student.setName(inputBuffer);
                addMessage("✅ 名字已改为: " + inputBuffer);
            } else {
                addMessage("❌ 名字不能为空");
            }
        }
        
        inputBuffer.clear();
        inputPrompt.clear();
        inputCallback.clear();
    }

    void interactWithEnvironment() {
        float px = student.getX();
        float py = student.getY();

        // 检查是否靠近某个建筑（带碰撞检测）
        if (px > 100 && px < 180 && py > 500 && py < 560) {
            addMessage("🏠 在宿舍睡觉... 精力恢复！");
            student.sleep();
        } else if (px > 250 && px < 330 && py > 500 && py < 560) {
            addMessage("📚 在图书馆学习！");
            if (!student.getCourses().empty()) {
                student.studyForExam(student.getCourses()[0].getName(), 2);
                addMessage("✅ 学习完成！");
            } else {
                addMessage("⚠️ 请先选修课程！");
            }
        } else if (px > 400 && px < 480 && py > 500 && py < 560) {
            addMessage("🏫 在教室参加考试周！");
            if (!student.getCourses().empty()) {
                student.examWeek();
                addMessage("✅ 考试周完成！");
            } else {
                addMessage("⚠️ 请先选修课程！");
            }
        } else if (px > 550 && px < 630 && py > 500 && py < 560) {
            addMessage("🍜 在食堂吃饭！");
            student.eat();
        } else if (px > 700 && px < 780 && py > 500 && py < 560) {
            addMessage("💪 在健身房运动！");
            student.exercise();
        } else {
            // 随机事件
            std::uniform_int_distribution<int> eventDist(0, 100);
            int event = eventDist(student.rng);
            if (event < 10) {
                addMessage("📱 捡到了学生卡！快乐+5");
                student.happiness = std::min(student.happiness + 5, 100);
            } else if (event < 15) {
                addMessage("🌧️ 下雨了！赶紧跑回室内");
            } else {
                addMessage("🚶 在校园散步...");
            }
        }

        // 自动存档
        saveManager.save(student);
    }

    void executeMenuOption(int option) {
        std::stringstream ss;
        switch (option) {
            case 1:
                ss << "📊 " << student.getName() << " | GPA: " 
                   << std::fixed << std::setprecision(2) << student.getGPA()
                   << " | 课程: " << student.getCourses().size() << "门";
                addMessage(ss.str());
                break;
            case 2:
                if (!availableCourses.empty()) {
                    // 选课菜单（简化版：选第一门未修的）
                    for (const auto& course : availableCourses) {
                        bool alreadyTaken = false;
                        for (const auto& c : student.getCourses()) {
                            if (c.getName() == course.getName()) {
                                alreadyTaken = true;
                                break;
                            }
                        }
                        if (!alreadyTaken) {
                            student.takeCourse(course);
                            addMessage("📖 选修了: " + course.getName());
                            break;
                        }
                    }
                } else {
                    addMessage("⚠️ 没有更多课程可选");
                }
                break;
            case 3:
                if (!student.getCourses().empty()) {
                    // 学习第一门未考试的课程
                    for (auto& course : student.getCourses()) {
                        if (course.getScore() == 0) {
                            student.studyForExam(course.getName(), 2);
                            addMessage("📝 学习了: " + course.getName());
                            break;
                        }
                    }
                } else {
                    addMessage("⚠️ 请先选修课程！");
                }
                break;
            case 4:
                student.socialize();
                addMessage("🎉 社交活动！快乐值 +20");
                break;
            case 5:
                student.examWeek();
                addMessage("📝 考试周结束！");
                break;
            case 6:
                student.nextSemester();
                addMessage("📅 进入第 " + std::to_string(student.getSemester()) + " 学期！");
                break;
            case 7:
                student.sleep();
                addMessage("😴 睡了一觉，精力恢复！");
                break;
            case 8:
                student.slackOff();
                addMessage("🎮 摸鱼中... 快乐+5 精力-5");
                break;
            case 9:
                if (student.canGraduate()) {
                    addMessage("🎉🎉🎉 恭喜毕业！！！");
                    isRunning = false;
                } else {
                    std::string reason;
                    if (student.getSemester() < 8) 
                        reason += "学期数不足(" + std::to_string(student.getSemester()) + "/8) ";
                    if (student.getGPA() < 2.0) 
                        reason += "GPA不足(" + std::to_string(student.getGPA()) + "/2.0) ";
                    if (student.getCourses().size() < 20) 
                        reason += "课程数不足(" + std::to_string(student.getCourses().size()) + "/20)";
                    addMessage("❌ 不满足毕业条件: " + reason);
                }
                break;
            default:
                addMessage("无效选项");
        }

        // 自动存档
        saveManager.save(student);
    }

    void addMessage(const std::string& msg) {
        messages.push_back(msg);
        if (messages.size() > 20) {
            messages.erase(messages.begin());
        }
    }

    void autoSave() {
        saveManager.save(student);
    }

    void update() {
        // 更新学生图标位置
        studentIcon.setPosition(student.getX(), student.getY());

        // 边界限制
        float margin = 20;
        if (student.getX() < margin) student.setPosition(margin, student.getY());
        if (student.getX() > 1024 - margin) student.setPosition(1024 - margin, student.getY());
        if (student.getY() < margin) student.setPosition(student.getX(), margin);
        if (student.getY() > 750 - margin) student.setPosition(student.getX(), 750 - margin);

        // 检查游戏结束条件
        if (student.getHappiness() <= 0) {
            addMessage("😢 太不开心了，退学了...");
            isRunning = false;
        }
        if (student.getEnergy() <= 0) {
            addMessage("😵 精力耗尽，累倒了...");
            isRunning = false;
        }
    }

    void render() {
        window.clear(bgColor);

        // 绘制地面网格
        for (int i = 0; i < 10; i++) {
            sf::RectangleShape line(sf::Vector2f(1000, 1));
            line.setPosition(10, 100 + i * 60);
            line.setFillColor(sf::Color(255, 255, 255, 30));
            window.draw(line);
        }

        // 绘制建筑
        window.draw(dormitory);
        window.draw(library);
        window.draw(classroom);
        window.draw(canteen);
        window.draw(gym);

        // 绘制建筑标签
        window.draw(dormLabel);
        window.draw(libLabel);
        window.draw(classLabel);
        window.draw(canteenLabel);
        window.draw(gymLabel);

        // 绘制状态面板
        window.draw(statusPanel);
        drawStatusInfo();

        // 绘制菜单面板
        window.draw(menuPanel);
        drawMenu();

        // 绘制消息面板
        window.draw(messagePanel);
        drawMessages();

        // 绘制学生角色
        window.draw(studentIcon);
        drawText("🎓 " + student.getName(), student.getX() - 20, student.getY() - 30, 14);

        // 绘制提示栏
        drawText("方向键移动 | Enter交互 | 1-9菜单 | S存档 | L读档 | R改名 | ESC退出", 320, 750, 12);

        // 如果正在输入，显示输入框
        if (isInputting) {
            sf::RectangleShape inputBox(sf::Vector2f(400, 30));
            inputBox.setPosition(320, 720);
            inputBox.setFillColor(sf::Color(0, 0, 0, 200));
            inputBox.setOutlineColor(sf::Color::White);
            inputBox.setOutlineThickness(1);
            window.draw(inputBox);
            
            drawText("> " + inputBuffer + "_", 325, 722, 16);
        }

        window.display();
    }

    void drawStatusInfo() {
        std::stringstream ss;
        ss << "📊 " << student.getName() << "\n";
        ss << "年龄: " << student.getAge() << "  学期: " << student.getSemester() << "\n";
        ss << "专业: " << student.getMajor() << "\n";
        ss << "GPA: " << std::fixed << std::setprecision(2) << student.getGPA() << "\n";
        ss << "课程: " << student.getCourses().size() << "门\n";
        ss << "技能: " << student.getSkills().size() << "个";

        sf::Text text;
        text.setFont(font);
        text.setString(ss.str());
        text.setCharacterSize(16);
        text.setFillColor(textColor);
        text.setPosition(20, 20);
        window.draw(text);

        // 绘制进度条
        drawText("精力:", 20, 145, 14);
        drawProgressBar(70, 148, 200, 10, student.getEnergy(), sf::Color::Green);
        
        drawText("快乐:", 20, 170, 14);
        drawProgressBar(70, 173, 200, 10, student.getHappiness(), sf::Color::Yellow);
    }

    void drawProgressBar(float x, float y, float width, float height, int value, sf::Color color) {
        sf::RectangleShape bg(sf::Vector2f(width, height));
        bg.setPosition(x, y);
        bg.setFillColor(sf::Color(80, 80, 80));
        window.draw(bg);

        float fill = std::max(0.0f, std::min(1.0f, value / 100.0f));
        sf::RectangleShape bar(sf::Vector2f(width * fill, height));
        bar.setPosition(x, y);
        bar.setFillColor(color);
        window.draw(bar);

        // 显示数值
        std::string val = std::to_string(value) + "%";
        drawText(val, x + width + 5, y - 2, 12);
    }

    void drawMenu() {
        std::vector<std::string> menuItems = {
            "1. 📊 查看状态", "2. 📖 选修课程", "3. 📝 学习备考",
            "4. 🎉 社交活动", "5. 📝 考试周", "6. 📅 推进学期",
            "7. 😴 休息", "8. 🎮 摸鱼", "9. 🎓 尝试毕业"
        };

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(textColor);

        for (size_t i = 0; i < menuItems.size(); i++) {
            text.setString(menuItems[i]);
            text.setPosition(20, 230 + i * 35);
            window.draw(text);
        }

        // 显示快捷键提示
        drawText("快捷键: S存档 L读档 R改名", 20, 555, 14);
    }

    void drawMessages() {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(14);
        text.setFillColor(textColor);

        int y = 610;
        int start = std::max(0, (int)messages.size() - 6);
        for (int i = start; i < (int)messages.size(); i++) {
            text.setString(messages[i]);
            text.setPosition(330, y);
            window.draw(text);
            y += 24;
        }
    }

    void drawText(const std::string& str, float x, float y, int size) {
        sf::Text text;
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(textColor);
        text.setPosition(x, y);
        window.draw(text);
    }
};

// ========== 主函数 ==========
int main() {
    std::cout << "🎓 学生生活模拟器 v2.0 - 2D图形版" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "✨ 新功能:" << std::endl;
    std::cout << "  💾 自动存档/读档 (按S保存, L读取)" << std::endl;
    std::cout << "  🏠 可视化校园地图" << std::endl;
    std::cout << "  🎮 角色移动与建筑交互" << std::endl;
    std::cout << "  📝 改名功能 (按R)" << std::endl;
    std::cout << "  🔄 跨平台字体支持" << std::endl;
    std::cout << "========================================" << std::endl;

    try {
        // 尝试加载存档
        SaveManager saveManager;
        Student student;
        
        std::cout << "是否加载已有存档? (y/n): ";
        char choice;
        std::cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            if (saveManager.load(student)) {
                std::cout << "✅ 加载存档成功！" << std::endl;
                std::cout << "  姓名: " << student.getName() << std::endl;
                std::cout << "  学期: " << student.getSemester() << std::endl;
                std::cout << "  GPA: " << student.getGPA() << std::endl;
            } else {
                std::cout << "❌ 没有找到存档，创建新角色" << std::endl;
                // 创建新角色
                std::string name, major;
                int age;
                std::cout << "请输入学生姓名: ";
                std::cin >> name;
                std::cout << "请输入年龄: ";
                std::cin >> age;
                std::cout << "请输入专业: ";
                std::cin >> major;
                student = Student(name, age, major);
            }
        } else {
            // 创建新角色
            std::string name, major;
            int age;
            std::cout << "请输入学生姓名: ";
            std::cin >> name;
            std::cout << "请输入年龄: ";
            std::cin >> age;
            std::cout << "请输入专业: ";
            std::cin >> major;
            student = Student(name, age, major);
        }

        std::cout << "启动图形界面..." << std::endl;
        std::cout << "如果窗口未出现，请检查SFML是否正确安装" << std::endl;

        GraphicsStudentSimulator game(student);
        game.run();

    } catch (const std::exception& e) {
        std::cerr << "❌ 程序异常: " << e.what() << std::endl;
        std::cout << "按任意键退出..." << std::endl;
        std::cin.get();
        return 1;
    }

    std::cout << "游戏已结束！" << std::endl;
    return 0;
}
