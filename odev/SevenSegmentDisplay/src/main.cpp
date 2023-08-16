#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

vector<int> DisplayTime(int hour, int min)
{
    vector<int> hexNumbers{0xEE, 0x24, 0xBA, 0xB6, 0x74, 0xD6, 0xDE, 0xA4, 0xFE, 0xF6};
    vector<int> displayValues(4);

    displayValues[0] = hour < 10 ? (hour == 0 ? hexNumbers[0] : 0x00) : hexNumbers[hour / 10];
    displayValues[1] = hour < 10 ? hexNumbers[hour] : hexNumbers[hour % 10];

    displayValues[2] = min < 10 ? hexNumbers[0] : hexNumbers[min / 10];
    displayValues[3] = min < 10 ? hexNumbers[min] : hexNumbers[min % 10];

    return displayValues;
}

void CalculateTimeValue(int seconds)
{

    vector<int> result;
    if (seconds > 86400)
    {
        seconds = seconds % 86400;
        int minutes = seconds / 60;
        int hours = minutes / 60;
        if (minutes % 60 != 0)
        {
            minutes = minutes % 60;
        }
        cout << "Given seconds = " << hours << ":" << setw(2) << setfill('0') << minutes << endl;
        result = DisplayTime(hours, minutes);
    }
    else
    {
        int minutes = seconds / 60;
        int hours = minutes / 60;
        if (minutes % 60 != 0)
        {
            minutes = minutes % 60;
        }
        cout << "Given seconds = " << hours << ":" << setw(2) << setfill('0') << minutes << endl;
        result = DisplayTime(hours, minutes);
    }

    for (int i = 0; i < result.size(); i++)
    {
        cout << hex << result[i] << endl;
    }
}

int main(int argc, char *argv[])
{
    int seconds;
    cout << "Enter seconds value: " << flush;
    cin >> seconds;

    CalculateTimeValue(seconds);
    return 0;
}
