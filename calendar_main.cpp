#include <iostream>
#include <ctime>
#include <iomanip>

using namespace std;

int isLeapYear(int year);//判断是否为闰年
int daysOfMonth(int year, int month);//获取某年某月有多少天
struct tm & setTime(time_t second, struct tm & now, int selectYear, int selectMonth);
void drawCalendar(struct tm & t_time, int nowyear, int nowmon, int nowday);

int main() {
    // 获取当前时间,距离1970年1月1日过去了多少秒
    time_t second = time(0);
    //今天时间
    struct tm t_time;
    localtime_s(&t_time, &second);
    int nowday = t_time.tm_mday, nowyear = t_time.tm_year + 1900, nowmon = t_time.tm_mon + 1;

    cout << "今天是" << nowyear << "年" << nowmon << "月" << nowday << "日" << endl;
    cout << "------------------------------------\n选择你要做的：\n（1）打印本月日历\n（2）打印本年日历\n（3）选择你想要跳转的\n------------------------------------\n";
    int choice;
    cin >> choice;
toSwitch:
    switch (choice)
    {
    case 1:
    {
        time_t getSecond = time(0);
        setTime(getSecond, t_time, nowyear, nowmon);
        drawCalendar(t_time, nowyear, nowmon, nowday);
        break;
    }
    case 2:
    {
        int selectYear, selectMonth;
        time_t getSecond = time(0);
        selectYear = nowyear;
        for (selectMonth = 1; selectMonth <= 12; selectMonth++) {
            setTime(getSecond, t_time, selectYear, selectMonth);
            drawCalendar(t_time, nowyear, nowmon, nowday);
        }
        break;
    }
    case 3:
    {
        int selectYear, selectMonth;
        cout << "请输入年份："; cin >> selectYear;

        cout << "------------------------------------\n现在你来到了" << selectYear << "年，请输入你想做的：\n（1）打印" << selectYear << "年的日历\n（2）选择月份日历\n------------------------------------" << endl;
        int choice_deep;
    toSelectSwitch:
        cin >> choice_deep;
        switch (choice_deep) {
        case 1:
        {
            time_t getSecond = time(0);
            for (selectMonth = 1; selectMonth <= 12; selectMonth++) {
                setTime(getSecond, t_time, selectYear, selectMonth);
                drawCalendar(t_time, nowyear, nowmon, nowday);
            }
            goto breakSwitch;
            break;
        }
        case 2:
        {
            cout << "请输入月份："; cin >> selectMonth;
            time_t getSecond = time(0);
            //获取用户选择的年月的第一天
            setTime(getSecond, t_time, selectYear, selectMonth);
            //画出日历
            drawCalendar(t_time, nowyear, nowmon, nowday);
            goto breakSwitch;
            break;
        }
        default:
            cout << "请输入正确的选项数字: 1 / 2 " << endl;
            cin.ignore(std::numeric_limits< streamsize >::max(), '\n');
            cin >> choice_deep;
            goto toSelectSwitch;
            break;
        }
    }
    default:
        cout << "请输入正确的选项数字: 1 / 2 / 3 " << endl;
        cin.ignore(std::numeric_limits< streamsize >::max(), '\n');
        cin >> choice;
        goto toSwitch;
        break;
    }
breakSwitch:
    //main();
    return 0;
}

//判断是否为闰年
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

//获取某年某月有多少天
int daysOfMonth(int year, int month) {
    switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    case 2:
        return isLeapYear(year) ? 29 : 28;
    }
    return 0;
}

//设置用户选择的时间
struct tm & setTime(time_t second, struct tm & inputTime, int selectYear, int selectMonth) {
    localtime_s(&inputTime,&second);
    inputTime.tm_year = selectYear - 1900;
    inputTime.tm_mon = selectMonth - 1;
    inputTime.tm_mday = 1;
    mktime(&inputTime);
    return inputTime;
}

//画出日历
void drawCalendar(struct tm & t_time, int nowyear, int nowmon, int nowday) {
    cout << "下面是" << (t_time.tm_year) + 1900 << "年" << (t_time.tm_mon) + 1 << "月的日历" << endl;
    cout << "=============================" << endl;
    string week[7] = { "日","一","二","三","四","五","六" };
    for (int i = 0; i < 7; i++)
        cout << setw(5) << week[i];
    cout << endl;
    //目前输出的日期所对应的星期
    int weekDay = t_time.tm_wday, day = t_time.tm_mday, month = t_time.tm_mon + 1, year = t_time.tm_year + 1900;
    //输出空格
    for (int i = 0; i < weekDay; i++)
        cout << setw(5) << " ";
    int lastloopday = 7 - weekDay + 1;
    //输出日期
    for (int i = 1; i <= daysOfMonth(year, month); i++) {
        lastloopday--;
        if (lastloopday == 0) {
            cout << endl;
            lastloopday = 7;
        }
        //如果是今天则输出*
        if (i == nowday && year == nowyear && month == nowmon)cout << setw(3) << "*" << i;
        else cout << setw(5) << i;
        weekDay++;
    }
    cout << endl;
    cout << "=============================" << endl;
}

