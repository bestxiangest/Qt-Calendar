一个使用Qt C++编写的日历程序，基本功能有查看日历、设置、修改日程、天气预报等。
该项目仅为初步学习Qt参考使用。

使用Qt Creator编写。

# 电子日历程序说明文档

## 一、项目概述
该电子日历程序是一个基于Qt开发的多功能应用，旨在为用户提供便捷的日期查看、日程管理以及天气预报查询功能。它不仅具备直观美观的图形用户界面，还通过整合多种技术，实现了实用的日常功能，帮助用户更好地规划时间和安排活动。

## 二、功能特点
### （一）界面展示
1. **主窗口**
    - 清晰呈现当前日期、时间、星期、月份以及天气信息。
    - 日历部分以月历形式展示，用户可通过点击年份切换月份，方便查看不同日期。
    - 点击日期数字，可将其背景变为绿色以突出显示，同时自动更新并显示该日期的日程安排（最多显示四个，点击“查看”可显示全部）。
    - 左侧信息栏包含“今天”“天气”“查看”“日程”“退出”等按钮，操作便捷。
2. **天气窗口**
    - 支持输入城市中文名称、汉语拼音或进行模糊搜索，查询后展示所选城市当天、昨天及未来六天的天气预报信息。
3. **日程创建窗口**
    - 用户可在指定日期添加日程，输入标题和内容后点击“保存”，日程信息将以json文件形式存储在程序所在目录的“scheduleFile”文件夹中。
4. **查看详细日程信息窗口**
    - 在此窗口可查看日程的详细内容，包括标题、内容和时间，并可直接修改或删除日程。

### （二）功能操作
1. **日历查看与导航**
    - 主窗口日历默认显示当前月份，用户可通过点击“今天”按钮快速返回当前日期，或点击年份输入框手动输入年份和月份跳转至指定日期。
    - 点击日历右上角小三角，可查看全年日历，全年日历以月份为单位展示，方便用户快速定位和查看不同月份的日期分布。
2. **天气预报查询**
    - 点击主窗口“天气”按钮，打开天气窗口，在输入框输入城市信息后点击查询，即可获取该城市详细天气预报，天气数据来源于和风天气api。
3. **日程管理**
    - 添加日程：点击主窗口“日程”按钮，弹出日程创建窗口，输入日程标题和内容后保存，日程将与日期关联并存储。
    - 查看日程：在主窗口点击日期，若该日期有日程安排，日程标题将显示在主窗口日程查看区域，点击“查看”按钮可查看全部日程，点击具体日程可在详细日程信息窗口查看详情。
    - 修改与删除日程：在详细日程信息窗口，用户可直接修改日程内容并保存，或点击“删除”按钮删除该日程，修改或删除后日程信息将实时更新保存至json文件。

### （三）数据存储与管理
1. 日程信息以json格式存储在本地文件中，每个日期对应一个json文件，文件名为日期（如“2024-05-19.json”），文件内容包含日程的标题、内容和时间等信息，方便数据的读取、更新和管理。
2. 程序通过合理的文件操作和数据处理逻辑，确保日程信息的准确存储和及时更新，即使程序关闭，日程数据也不会丢失，下次打开程序仍可查看和继续管理日程。

## 三、技术实现
### （一）开发环境与工具
1. **操作系统**：Windows（开发环境为ThinkBook 16 G6 IRL）
2. **开发工具**：Qt Creator 12.0.2 (Community)
3. **编程语言**：C++

### （二）框架与库
1. **Qt**：作为跨平台的C++图形用户界面库，用于构建程序的图形界面，实现界面的美观设计和交互功能。通过Qt的信号与槽机制，实现了对象间的高效通信，如按钮点击信号与相应功能槽函数的连接，确保了程序的响应性和流畅性。
2. **Qt网络模块**：用于与和风天气api进行网络通信，实现天气预报数据的获取。通过QNetworkAccessManager发送网络请求，并在收到响应后利用QJsonDocument等类解析json数据，将天气信息展示在界面上。
3. **JSON处理**：使用Qt内置的JSON处理功能，将日程信息存储为json文件，实现数据的持久化存储。在日程管理过程中，通过创建、读取、更新和写入json对象和数组，实现日程数据的高效管理。

### （三）核心算法与逻辑
1. **日历绘制算法**：通过计算当前日期所在月份的第一天是星期几，结合月份天数，动态绘制日历界面，确保日历的准确性和完整性。
2. **日程管理逻辑**：在添加日程时，根据日期判断对应的json文件是否存在，若不存在则创建新文件并写入日程信息，若存在则读取文件内容，追加新日程后更新文件。在查看、修改和删除日程时，通过读取和更新json文件中的数据，实现日程信息的实时管理。

## 四、使用说明
### （一）安装与启动
##### 源代码运行：
1. 确保系统已安装Qt运行环境。
2. 下载项目源代码，解压至本地目录。
3. 打开Qt Creator，选择“打开项目”，定位到解压后的项目文件夹，选择项目文件（.pro文件）打开项目。
4. 在Qt Creator中点击“运行”按钮，即可启动电子日历程序。
###### 程序运行
1. 在release下载exe文件
2. 双击运行
### （二）操作指南
1. **主窗口操作**
    - “今天”按钮：点击后，日历将立即切换至当前日期所在月份，并突出显示当前日期。
    - “天气”按钮：点击打开天气窗口，在输入框输入城市名称（支持中文、拼音及模糊搜索）后点击查询，可获取该城市天气预报信息。
    - “查看”按钮：点击后，将在主窗口下方显示所选日期的所有日程安排，若日程较多可滚动查看，再次点击可隐藏日程显示。
    - “日程”按钮：点击弹出日程创建窗口，用户可在此输入日程标题、内容和时间（时间默认为当前时间，可手动修改），点击“保存”按钮将日程添加至指定日期。
    - “退出”按钮：点击关闭程序。
    - 日历区域操作：点击年份数字，可手动输入年份和月份跳转至指定日期；点击日历中的日期数字，可查看该日期日程安排（部分显示），并可通过“查看”按钮查看全部日程；点击右上角小三角，可查看全年日历。
2. **天气窗口操作**
    - 在输入框输入城市信息后，点击右侧查询按钮，即可在窗口中展示所选城市的天气预报，包括日期、天气状况、温度范围、风向风力等信息。
3. **日程创建窗口操作**
    - 在“标题”输入框输入日程标题，在“内容”输入框输入详细内容，点击“保存”按钮将日程信息保存为json文件，点击“清除”按钮可清空输入框内容，点击“关闭”按钮关闭日程创建窗口。
4. **查看详细日程信息窗口操作**
    - 在此窗口中，用户可查看日程的详细信息，包括标题、内容和时间。直接修改文本框中的内容后，点击“修改”按钮可保存修改，点击“删除”按钮可删除该日程，修改或删除后窗口将自动关闭，主窗口日程信息将实时更新。

### （三）注意事项
1. 确保计算机已连接互联网，以便获取天气预报数据。
2. 在输入城市名称查询天气时，确保输入准确，否则可能无法获取正确的天气信息。
3. 日程信息存储在本地json文件中，请勿随意删除或修改文件格式，以免导致数据丢失或程序异常。

## 五、项目结构
项目主要文件结构如下：
- **mainwindow.h/.cpp**：主窗口类的定义与实现，包含界面初始化、日历绘制、日程管理、天气信息获取与展示等功能函数，以及与其他窗口类的信号与槽连接。
- **reminder.h/.cpp**：日程窗口类的定义与实现，负责日程的添加、保存和与主窗口的数据交互。
- **weather.h/.cpp**：天气窗口类的定义与实现，实现城市编码获取、天气数据查询与解析、以及天气信息在界面上的展示。
- **weatherdata.h**：天气预报信息类的定义，存储天气相关的数据结构。
- **dialog.h/.cpp**：详细日程信息窗口类的定义与实现，用于展示和编辑日程的详细信息，实现与主窗口的数据传递和日程修改、删除操作。
- **main.cpp**：程序入口点，创建主窗口并启动应用程序。
- **thewholeyear.h/.cpp**：查看全年日历类的定义与实现，提供全年日历的绘制和展示功能。
- **scheduleFile文件夹**：用于存储日程信息的json文件，每个日期对应一个文件，文件名为日期（如“2024-05-19.json”）。

@.name 犀利的毛毛虫
@.email 1816054322@qq.com
