
# 🎓 Student Life Simulator v2.0

### *一款基于C++和SFML的校园生活模拟游戏*

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-2.6-green.svg)](https://www.sfml-dev.org/)
[![License](https://img.shields.io/badge/License-Apache%202.0-orange.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()

---

## 📖 项目简介

**Student Life Simulator v2.0** 是一款沉浸式的大学生活模拟游戏，结合了**2D图形界面**与**深度模拟机制**。玩家控制一个角色在可视化校园中自由探索、学习、社交，体验从大一新生到毕业的完整大学生活。

与传统的文字冒险游戏不同，本版本提供了：
- 🗺️ **可视化2D校园地图** - 宿舍、图书馆、教室、食堂、健身房等场景
- 🎮 **实时角色控制** - 使用方向键自由移动
- 💾 **完整的存档系统** - 自动保存和手动读档
- 🎯 **丰富的游戏机制** - 课程、考试、技能、GPA、毕业条件

---

## ✨ 核心功能

### 🎯 学业系统
| 功能 | 描述 |
|------|------|
| 课程选修 | 10+门专业课程（高数、C++、数据结构、AI等） |
| 学习机制 | 投入时间影响成绩，精力值动态调整效率 |
| 考试系统 | 日常考试 + 集中考试周 |
| 成绩评定 | 百分制 → 等级制（A/B/C/D/F），GPA自动计算 |

### 🧠 角色状态
| 属性 | 范围 | 影响 |
|------|------|------|
| 精力值 | 0-100 | 学习效率、行动能力 |
| 快乐值 | 0-100 | 游戏进程、毕业可能性 |
| GPA | 0-4.0 | 毕业必要条件 |

### 🏆 技能系统
- 考试成绩≥85分可获得额外技能
- 技能列表：编程能力、数学思维、英语应用、数据分析、项目管理

### 🎮 交互方式
| 操作 | 功能 |
|------|------|
| ⬆⬇⬅➡ | 角色移动 |
| Enter | 与环境交互（靠近建筑） |
| 1-9 | 执行菜单操作 |
| S | 手动存档 |
| L | 读取存档 |
| R | 修改角色名 |
| ESC | 退出游戏 |

### 🏛️ 校园建筑
| 建筑 | 功能 |
|------|------|
| 🏠 宿舍 | 睡眠恢复精力 |
| 📚 图书馆 | 学习备考 |
| 🏫 教室 | 参加考试周 |
| 🍜 食堂 | 吃饭恢复状态 |
| 💪 健身房 | 运动提升状态 |

### 🎓 毕业条件
- ✅ 完成 **8个学期**
- ✅ GPA ≥ **2.0**
- ✅ 完成 **20门课程**

---

## 🏗️ 系统架构

### 类结构设计

```
┌─────────────────────────────────────────────────────────────────┐
│                    GraphicsStudentSimulator                      │
│  - 游戏主循环 (事件处理/更新/渲染)                               │
│  - 2D场景管理 (校园地图/建筑/角色)                               │
│  - UI系统 (状态面板/菜单/消息日志)                               │
│  - 输入处理 (键盘/文本输入)                                      │
└──────────────────────────┬──────────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│                         Student                                  │
│  - 属性: 姓名/年龄/专业/学期/GPA                                 │
│  - 状态: 精力值/快乐值                                           │
│  - 数据: 课程列表/技能列表                                       │
│  - 位置: 2D坐标 (x, y)                                          │
│  - 方法: 学习/考试/活动/状态管理                                 │
│  - 序列化: 存档/读档                                            │
└──────────────────────────┬──────────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│                         Course                                   │
│  - 属性: 课程名/学分/成绩/等级                                   │
│  - 方法: 成绩设置/序列化                                        │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                       SaveManager                                │
│  - 存档目录管理 (跨平台)                                        │
│  - 保存/读取/列表/删除                                          │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                       FontLoader                                 │
│  - 跨平台字体加载 (Windows/Linux/macOS)                         │
│  - 降级方案 (支持备用字体)                                      │
└─────────────────────────────────────────────────────────────────┘
```

### 数据流

```
用户输入 → 键盘/鼠标事件
    ↓
事件处理 (handleEvents)
    ↓
├─ 移动指令 → 更新角色位置
├─ 交互指令 → 建筑交互逻辑 → 更新学生状态
├─ 菜单指令 → 执行对应功能 → 更新学生状态
├─ 存档指令 → SaveManager → 文件系统
└─ 读档指令 → SaveManager → 恢复学生状态
    ↓
更新 (update)
    ↓
├─ 位置边界检查
├─ 游戏结束条件检查
└─ 自动存档触发
    ↓
渲染 (render)
    ↓
├─ 绘制校园背景/建筑/网格
├─ 绘制角色/状态面板/菜单/消息
└─ 绘制输入提示/快捷键提示
    ↓
窗口显示
```

### 状态机

```
    ┌──────────────────────┐
    │      游戏启动         │
    │  (检测存档/创建角色)  │
    └──────────┬───────────┘
               ▼
    ┌──────────────────────┐
    │     自由探索模式      │◄──────┐
    │  (移动/交互/菜单)     │       │
    └──────────┬───────────┘       │
               ▼                     │
    ┌──────────────────────┐       │
    │    建筑交互/菜单操作   │       │
    │  (学习/考试/社交等)   │       │
    └──────────┬───────────┘       │
               ▼                     │
    ┌──────────────────────┐       │
    │    状态更新/学期推进   │──────┘
    │  (精力/快乐/GPA变化)  │
    └──────────┬───────────┘
               ▼
    ┌──────────────────────┐
    │   毕业条件检查        │
    └──────────┬───────────┘
               ▼
    ┌──────────────────────┐
    │  毕业/退学/游戏结束   │
    └──────────────────────┘
```

---

## 🛠️ 技术实现

### 开发环境
| 技术 | 版本 | 用途 |
|------|------|------|
| C++ | C++17 | 核心语言 |
| SFML | 2.6+ | 图形/窗口/事件处理 |
| STL | C++17 | 容器/算法/字符串 |
| Filesystem | C++17 | 跨平台文件操作 |
| Random | C++11 | 随机数生成 |

### 核心库
```cpp
#include <SFML/Graphics.hpp>    // 2D图形渲染
#include <SFML/Window.hpp>      // 窗口和事件
#include <filesystem>            // 跨平台文件系统
#include <fstream>               // 文件读写
#include <random>                // 随机数
#include <thread>                // 多线程 (备用)
```

### 跨平台支持
| 平台 | 字体路径 | 存档目录 |
|------|----------|----------|
| Windows | `C:\Windows\Fonts\msyh.ttc` | `%APPDATA%\StudentSimulator\saves\` |
| Linux | `/usr/share/fonts/...` | `~/.student_simulator/saves/` |
| macOS | `/System/Library/Fonts/...` | `~/Library/Application Support/...` |

---

## 🚀 快速开始

### 环境准备

#### Windows
```bash
# 1. 安装 MinGW-w64
# 下载: https://www.mingw-w64.org/

# 2. 安装 SFML
# 下载: https://www.sfml-dev.org/download/sfml/2.6.1/
# 解压到 C:\SFML-2.6.1

# 3. 验证安装
g++ --version
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install g++ make libsfml-dev
```

#### macOS
```bash
# 安装Homebrew (如果没有)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 安装SFML
brew install sfml
```

### 编译与运行

#### Windows (MinGW)
```bash
# 编译
g++ -o student_simulator.exe main.cpp -std=c++17 \
    -I C:\SFML-2.6.1\include \
    -L C:\SFML-2.6.1\lib \
    -lsfml-graphics -lsfml-window -lsfml-system \
    -lstdc++fs

# 运行
student_simulator.exe

# 如果需要复制SFML DLL到程序目录
# 将 C:\SFML-2.6.1\bin\*.dll 复制到当前目录
```

#### Linux
```bash
# 编译
g++ -o student_simulator main.cpp -std=c++17 \
    -lsfml-graphics -lsfml-window -lsfml-system \
    -lstdc++fs

# 运行
./student_simulator
```

#### macOS
```bash
# 编译
g++ -o student_simulator main.cpp -std=c++17 \
    -I /usr/local/include \
    -L /usr/local/lib \
    -lsfml-graphics -lsfml-window -lsfml-system \
    -lstdc++fs

# 运行
./student_simulator
```

### 使用CMake (推荐)

创建 `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.15)
project(StudentSimulator VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(student_simulator main.cpp)

target_link_libraries(student_simulator 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Windows平台需要链接stdc++fs
if (WIN32)
    target_link_libraries(student_simulator stdc++fs)
endif()
```

编译:
```bash
mkdir build && cd build
cmake ..
cmake --build .
./student_simulator
```

---

## 🎮 游戏指南

### 启动流程

```
1. 运行程序
   ↓
2. 选择是否加载存档 (y/n)
   ↓
3. 输入角色信息 (姓名/年龄/专业)
   ↓
4. 进入校园主界面
   ↓
5. 开始大学生活！
```

### 游戏策略

#### 🎯 新手建议
1. **第一学期**：选修2-3门基础课程
2. **学习节奏**：每天学习2-3小时，保持精力>60
3. **状态管理**：每学习3天，安排1天社交或休息
4. **建筑利用**：合理利用校园建筑提升效率

#### 💡 进阶技巧
- **高效学习**：精力值>80时学习效果最佳
- **技能搭配**：不同课程可能触发不同技能
- **考试准备**：考前保持精力100%争取高分
- **存档策略**：重要决策前手动存档

#### ⚠️ 注意事项
- 快乐值<30时优先社交活动
- 精力值<30时学习效果减半
- 每个学期至少选修3门课程
- 角色离开边界会被自动限制

### 界面说明

```
┌──────────────────────────────────────────────────────────────┐
│  📊 张三               【状态面板】                         │
│  年龄: 20  学期: 3                                         │
│  专业: 计算机科学                                           │
│  GPA: 3.45                                                 │
│  课程: 5门                                                 │
│  技能: 3个                                                 │
│  精力: ████████░░ 80%                                      │
│  快乐: █████████░ 90%                                      │
├──────────────────────────────────────────────────────────────┤
│  【菜单面板】                                                │
│  1. 📊 查看状态                                             │
│  2. 📖 选修课程                                             │
│  3. 📝 学习备考                                             │
│  4. 🎉 社交活动                                             │
│  5. 📝 考试周                                               │
│  6. 📅 推进学期                                             │
│  7. 😴 休息                                                 │
│  8. 🎮 摸鱼                                                 │
│  9. 🎓 尝试毕业                                             │
│  快捷键: S存档 L读档 R改名                                   │
├──────────────────────────────────────────────────────────────┤
│                🏠宿舍  📚图书馆  🏫教室  🍜食堂  💪健身房   │
│                 ═══════════════════════════════              │
│                                                             │
│                      🎓 张三                                 │
│                      ●  ← 角色                              │
│                                                             │
├──────────────────────────────────────────────────────────────┤
│  消息日志:                                                  │
│  ▶ 欢迎来到学生生活模拟器 v2.0                             │
│  ▶ 使用方向键移动角色                                       │
│  ▶ 靠近建筑按 Enter 交互                                    │
│  ▶ 按 1-9 执行菜单操作                                      │
│  ▶ 按 S 存档 | L 读档                                      │
│                                                             │
│  方向键移动 | Enter交互 | 1-9菜单 | S存档 | L读档 | ESC退出 │
└──────────────────────────────────────────────────────────────┘
```

---

## 📊 数据统计

### 课程列表
| # | 课程名称 | 学分 | 难度 | 推荐学期 |
|---|---------|------|------|----------|
| 1 | 高等数学 | 4 | ⭐⭐⭐ | 1-2 |
| 2 | C++编程 | 3 | ⭐⭐⭐ | 1-2 |
| 3 | 数据结构 | 3 | ⭐⭐⭐⭐ | 2-3 |
| 4 | 操作系统 | 3 | ⭐⭐⭐⭐ | 3-4 |
| 5 | 计算机网络 | 3 | ⭐⭐⭐ | 3-4 |
| 6 | 数据库原理 | 3 | ⭐⭐⭐ | 3-4 |
| 7 | 软件工程 | 2 | ⭐⭐ | 4-5 |
| 8 | 人工智能 | 4 | ⭐⭐⭐⭐⭐ | 5-6 |
| 9 | 机器学习 | 3 | ⭐⭐⭐⭐ | 5-6 |
| 10 | 英语 | 2 | ⭐⭐ | 1-8 |

### 状态影响
| 活动 | 精力变化 | 快乐变化 | 学习影响 |
|------|---------|---------|----------|
| 学习(1h) | -2 | +3 | +3分/小时 |
| 睡眠 | +40 | +5 | - |
| 吃饭 | +20 | +8 | - |
| 社交 | -10 | +20 | - |
| 运动 | +15 | +10 | - |
| 摸鱼 | -5 | +5 | - |

### 存档格式
```
NAME:张三
AGE:20
MAJOR:计算机科学
SEMESTER:3
GPA:3.45
ENERGY:80
HAPPINESS:70
POSX:400
POSY:300
COURSES:5
COURSE:高等数学|4|92|A
COURSE:C++编程|3|85|B
...
SKILLS:3
SKILL:学习基础
SKILL:编程能力
SKILL:数学思维
```

---

## 📝 开发日志

### v2.0.0 (2026-06-26) - 重大更新
- ✅ 完整2D图形界面 (SFML)
- ✅ 可视化校园地图
- ✅ 实时角色移动控制
- ✅ 跨平台字体加载系统
- ✅ 完整的存档/读档系统
- ✅ 角色改名功能
- ✅ 异常处理机制
- ✅ 随机事件系统
- ✅ 建筑交互系统
- ✅ 进度条可视化
- ✅ 消息日志系统

### v1.0.0 (2026-06-26)
- ✅ 初始版本发布
- ✅ 核心游戏循环
- ✅ 课程/考试/技能系统
- ✅ 控制台界面

### 计划功能 (v3.0.0)
- 📌 多人联机模式
- 📌 社团活动系统
- 📌 恋爱系统
- 📌 兼职工作系统
- 📌 成就系统 (Steam成就)
- 📌 数据可视化图表
- 📌 音效和背景音乐
- 📌 动画效果 (行走/学习动画)
- 📌 多语言支持
- 📌 移动端适配

---

## 🤝 贡献指南

欢迎贡献代码！请遵循以下规范：

### 开发流程
1. **Fork** 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开 **Pull Request**

### 代码规范
```cpp
// 1. 类名使用驼峰命名法
class GraphicsStudentSimulator { };

// 2. 函数名使用驼峰命名法
void handleKeyPress() { }

// 3. 变量名使用下划线命名法
int energy_value = 80;

// 4. 常量使用全大写
const int MAX_ENERGY = 100;

// 5. 充分注释
/**
 * 处理键盘输入事件
 * @param key SFML键盘按键枚举
 */
void handleKeyPress(sf::Keyboard::Key key) { }

// 6. 异常处理
try {
    // 可能抛出异常的代码
} catch (const std::exception& e) {
    std::cerr << "错误: " << e.what() << std::endl;
}
```

### 测试用例
```bash
# 功能测试
1. 正常游戏流程: 启动→选课→学习→考试→毕业
2. 存档测试: 存档→退出→读档→状态恢复
3. 边界测试: 角色移动边界限制
4. 状态测试: 精力/快乐值归零游戏结束
5. 输入测试: 特殊字符/负数输入处理
```

---

## 📄 许可证

本项目采用 **Apache License 2.0** 开源协议。

```
Copyright 2026 [Your Name]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

### Apache 2.0 许可证要点

| 特性 | 说明 |
|------|------|
| ✅ 商业使用 | 允许将本软件用于商业目的 |
| ✅ 修改源码 | 允许修改源代码 |
| ✅ 分发 | 允许分发原始或修改后的版本 |
| ✅ 专利授权 | 贡献者授予用户专利使用权 |
| ⚠️ 保留版权声明 | 分发时必须保留原始版权和许可证声明 |
| ⚠️ 声明修改 | 修改过的文件必须注明修改内容 |
| ⚠️ 免责声明 | 软件按"原样"提供，不提供任何担保 |

---

## 👨‍💻 作者

- **开发者**: [你的名字]
- **GitHub**: [你的GitHub链接]
- **Email**: [你的邮箱]
- **项目主页**: [GitHub仓库链接]

---

## 🙏 致谢

- **SFML团队** - 提供优秀的2D图形库
- **MinGW-w64** - Windows编译环境
- **Apache Software Foundation** - 提供Apache 2.0许可证
- **所有贡献者** - 代码审查和建议

---

## 📊 项目统计

[![GitHub stars](https://img.shields.io/github/stars/yourusername/student-simulator?style=social)](https://github.com/yourusername/student-simulator)
[![GitHub forks](https://img.shields.io/github/forks/yourusername/student-simulator?style=social)](https://github.com/yourusername/student-simulator)
[![GitHub issues](https://img.shields.io/github/issues/yourusername/student-simulator)](https://github.com/yourusername/student-simulator/issues)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/yourusername/student-simulator)](https://github.com/yourusername/student-simulator/pulls)
[![License](https://img.shields.io/badge/License-Apache%202.0-orange.svg)](LICENSE)

### 代码统计
- **总代码行数**: ~800行
- **类数量**: 6个
- **函数数量**: ~40个
- **支持平台**: Windows / Linux / macOS

---

## ⭐ Star History

如果这个项目对你有帮助，请点亮右上角的 Star ⭐ ！

[![Star History Chart](https://api.star-history.com/svg?repos=yourusername/student-simulator&type=Date)](https://star-history.com/#yourusername/student-simulator&Date)

---

**祝你大学生活愉快！🎉**
```

---

## 📋 完整的项目文件清单

现在你需要创建以下文件：

### 1. LICENSE (Apache-2.0)
完整的 Apache 2.0 许可证文本（我上面已经提供了完整内容）

### 2. README.md
上面的完整 README 内容

### 3. main.cpp
之前提供的完整游戏源代码

### 4. .gitignore
```
# 编译输出
*.exe
*.o
*.obj
*.pdb
*.ilk
*.out
*.app

# IDE
.vscode/
.idea/
*.swp
*.swo
*~

# 存档文件
*.sav
saves/

# SFML DLL
*.dll

# 构建目录
build/
cmake-build-*/
bin/
lib/

# 日志
*.log
```

### 5. CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.15)
project(StudentSimulator VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 查找SFML
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# 可执行文件
add_executable(student_simulator main.cpp)

# 链接库
target_link_libraries(student_simulator 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Windows平台特殊处理
if (WIN32)
    target_link_libraries(student_simulator stdc++fs)
    # 复制SFML DLL到输出目录
    add_custom_command(TARGET student_simulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_BINARY_DIR}/sfml-graphics-2.dll"
            "${SFML_BINARY_DIR}/sfml-window-2.dll"  
            "${SFML_BINARY_DIR}/sfml-system-2.dll"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/")
endif()

# Linux平台
if (UNIX AND NOT APPLE)
    target_link_libraries(student_simulator stdc++fs)
endif()

# macOS平台
if (APPLE)
    set_target_properties(student_simulator PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
    )
endif()
```
---

