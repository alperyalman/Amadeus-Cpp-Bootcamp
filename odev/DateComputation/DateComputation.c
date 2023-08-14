/*
DateComputation
Created by Alper Yalman
*/

#include <stdio.h>
#include <stdbool.h>

struct CDate {
    int mYear;
    int mMonth;
    int mDay;
};

int minYear = 100;
int daysInMonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Max number of days for each month, February might be uptaded according to leap year check

bool IsLeapYear (int year) {
    //Check if the year is leap or not
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0) ? true : false;
}

bool IsValidDate(struct CDate givenDate) {
    //Check if the given date is valid or not
    if(givenDate.mYear < minYear) // Check the year first
        return false;
    if(IsLeapYear(givenDate.mYear))
        daysInMonths[1] = 29; //Update days in February if it is leap year
    
    if(givenDate.mMonth < 1 || givenDate.mMonth > 12) // Check the month
        return false;
    
    if(givenDate.mDay < 1 || givenDate.mDay > daysInMonths[givenDate.mMonth - 1]) // Check the day
        return false;
    
    return true;
}



struct CDate ComputeNextDate(struct CDate givenDate) {
    //Compute next date
    struct CDate nextDate = givenDate;
    
    if(givenDate.mDay == daysInMonths[givenDate.mMonth - 1])
    {
        if(givenDate.mMonth == 12)
            nextDate = (struct CDate){givenDate.mYear + 1, 1, 1};
        else
            nextDate = (struct CDate){givenDate.mYear, givenDate.mMonth + 1, 1};
    }
    else
        nextDate = (struct CDate) {givenDate.mYear, givenDate.mMonth, givenDate.mDay + 1};
    
    return nextDate;
}


struct CDate ComputePreviousDate(struct CDate givenDate) {
    //Compute previous date
    struct CDate previousDate;
    
    if(givenDate.mDay == 1)
    {
        if(givenDate.mMonth == 1)
            previousDate = (struct CDate){givenDate.mYear - 1, 12, 31}; // Last day of the previous year
        else
            previousDate = (struct CDate){givenDate.mYear, givenDate.mMonth - 1, daysInMonths[givenDate.mMonth - 2]};
    }
    else
        previousDate = (struct CDate) {givenDate.mYear, givenDate.mMonth, givenDate.mDay - 1};
    
    return previousDate;
}




int main(int argc, const char * argv[]) {
    struct CDate givenDate;
    printf("******************************************************************************\n");
    printf("Year input conditions: year >= %d (Can be updated in the program)\n", minYear);
    printf("Month input conditions: month >= 1 and month <= 12.\n");
    printf("Day input conditions: day >= 1 and day <= max days in corresponding month. \n");
    printf("******************************************************************************\n\n\n");
    
    printf("Enter the year: ");
    scanf("%d", &givenDate.mYear);
    
    printf("Enter the month: ");
    scanf("%d", &givenDate.mMonth);
    
    printf("Enter the day: ");
    scanf("%d", &givenDate.mDay);
    
    if(IsValidDate(givenDate))
    {
        struct CDate nextDate = ComputeNextDate(givenDate);
        struct CDate previousDate = ComputePreviousDate(givenDate);
        printf("\nDate format is DD.MM.YYYY\n");
        printf("Given date:\t\t %d.%d.%d \n", givenDate.mDay, givenDate.mMonth, givenDate.mYear);
        printf("Previous date:\t %d.%d.%d \n", previousDate.mDay, previousDate.mMonth, previousDate.mYear);
        printf("Next date:\t\t %d.%d.%d \n", nextDate.mDay, nextDate.mMonth, nextDate.mYear);
    }
    else
    {
        printf("Given date is invalid!\n ");
    }
        
        
    return 0;
}
