#include <stdio.h>
#include <stdbool.h>

short CalculateDayOfYear(char month, char day, short year)
{
    char daysInMonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Define number of days for each month
    short dayOfYear = (short)day;                                           // Updated value will be return at the end

    // Check if the year valid
    if (year <= 0)
        return 0;

    // Leap year check and February days update if necessary
    bool isLeap = ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0) ? true : false;
    if (isLeap)
        daysInMonths[1] = 29;

    // Check if the day and month inputs are valid
    if ((month < 1 || month > 12) || (day < 1 || day > daysInMonths[month - 1]))
        return 0;

    // Calculate the day of year according to inputs
    for (char idxMonth = 1; idxMonth < month; idxMonth++)
    {
        dayOfYear += daysInMonths[idxMonth - 1];
    }

    return dayOfYear;
}

int main(int argc, char *argv[])
{
    char month;
    char day;
    short year;
    short dayOfYear;

    printf("Enter the year: ");
    scanf("%hd", &year);

    printf("Enter the month: ");
    scanf("%hhd", &month);

    printf("Enter the day: ");
    scanf("%hhd", &day);

    dayOfYear = CalculateDayOfYear(month, day, year);

    // Check if the input is invalid
    if (!dayOfYear)
    {
        printf("Invalid date input!\n");
    }
    else
    {
        printf("It is %hd. day of %hd.\n", dayOfYear, year);
    }

    return 0;
}
