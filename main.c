#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 100

char studentNames[MAX_STUDENTS][MAX_NAME_LEN];
int studentCount = 0;

void refreshStudentList() {
    char lineBuffer[MAX_NAME_LEN];
    FILE *studentFile = fopen("students.txt", "r");

    if(!studentFile) {
        printf(">> ERROR: Can't find students text file!\n\n");
        return;
    }

    studentCount = 0;
    while(studentCount < MAX_STUDENTS && fgets(lineBuffer, sizeof(lineBuffer), studentFile)) {
        lineBuffer[strcspn(lineBuffer, "\n")] = 0;
        
        if(strlen(lineBuffer) > 0) {
            strcpy(studentNames[studentCount], lineBuffer);
            studentCount++;
        }
    }

    fclose(studentFile);
}

void AttendanceCheck() {
    char studentStatus[MAX_STUDENTS][20];

    if(studentCount == 0) {
        printf(">> NO STUDENTS TO CHECK FOR ATTENDANCE!\n\n");
        return;
    }

    for(int i = 0; i < studentCount; i++) {
        while(true) {
            char status;

            printf("\n%d. %s\n", i+1, studentNames[i]);
            printf("\t+ A: Present\n");
            printf("\t+ B: Late\n");
            printf("\t+ C: Absent\n");
            printf("\t>> ");
            scanf(" %c", &status); 
            status = tolower(status);

            switch(status) {
                case 'a': { strcpy(studentStatus[i], "Present"); break; }
                case 'b': { strcpy(studentStatus[i], "Late"); break; }
                case 'c': { strcpy(studentStatus[i], "Absent"); break; }
                default: { printf(">> INVALID!\n"); continue; }
            }

            printf("%s is %s\n\n", studentNames[i], studentStatus[i]);
            break;
        }
    }

    time_t currentTime = time(0);
    struct tm* localTime = localtime(&currentTime);

    char filename[100];
    sprintf(filename, "AttendanceLogs/attendance_%d-%d-%d.txt", 
            localTime->tm_mon + 1, 
            localTime->tm_mday, 
            localTime->tm_year % 100);
    
    FILE *generatedAttendance = fopen(filename, "w");
    if(!generatedAttendance) {
        printf(">> ERROR: Can't create attendance log!\n\n");
        return;
    }

    for(int i = 0; i < studentCount; i++) {
        fprintf(generatedAttendance, "%s\t\t|\t\t%s\n", studentStatus[i], studentNames[i]);
    }

    fclose(generatedAttendance);
    printf("\n>> ATTENDANCE LOG SUCCESSFULLY GENERATED!\n\n");
}

void AddStudent() {
    if(studentCount >= MAX_STUDENTS) {
        printf(">> MAXIMUM STUDENTS LIMIT REACHED!\n\n");
        return;
    }

    while(true) {
        char studentName[MAX_NAME_LEN];
        char confirm;

        printf("\nEnter a name to add (type 'exit' to stop): ");
        
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 

        fgets(studentName, sizeof(studentName), stdin);
        studentName[strcspn(studentName, "\n")] = 0;

        if(strcmp(studentName, "exit") == 0) break;
        int found = 0;

        for(int i = 0; i < studentCount; i++) {
            if(strcmp(studentName, studentNames[i]) != 0) continue;
            found = 1;
            break;
        }
        
        if(found) {
            printf(">> %s ALREADY EXISTS!\n", studentName);
            continue;
        }

        while(true) {
            printf("Are you sure you want to add\n");
            printf("+ %s? (Y/n)\n", studentName);
            printf(">> ");
            scanf(" %c", &confirm);
            confirm = tolower(confirm);

            if(confirm == 'y' || confirm == 'n') break;
            else printf(">> INVALID!\n");
        }

        if(confirm == 'n') continue;

        FILE *studentFile = fopen("students.txt", "a");
        if(!studentFile) {
            printf(">> ERROR: Can't find students text file!\n\n");
            return;
        }

        fprintf(studentFile, "%s\n", studentName);
        fclose(studentFile);

        printf(">> %s HAS BEEN ADDED!\n", studentName);
    }

    refreshStudentList();
}

void RemoveStudent() {
    if(studentCount <= 0) {
        printf(">> THERE ARE NO STUDENTS!\n\n");
        return;
    }

    while(true) {
        printf("\n\t\tCurrent Students\n");
        if(studentCount == 0) printf("\tNone...\n");
        else {
            for(int i = 0; i < studentCount; i++) {
                printf("\t%d. %s\n", i+1, studentNames[i]);
            }
        }

        char answer[50];
        char confirm;

        printf("\nEnter a student number to remove (type 'exit' to stop): ");
        
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0;

        if(strcmp(answer, "exit") == 0) break;
        bool isNumeric = true;

        for(int i = 0; answer[i] != '\0'; i++) {
            if(isdigit(answer[i])) continue;

            isNumeric = false;
            break;
        }

        if(!isNumeric) {
            printf("INVALID!\n");
            continue;
        }

        int studentNo = atoi(answer) - 1;
        if(studentNo >= studentCount || studentNo < 0) {
            printf(">> STUDENT NO. %d DOESN'T EXIST!", studentNo + 1);
            continue;
        }

        while(true) {
            printf("Are you sure you want to remove\n");
            printf("+ %s? (Y/n)\n", studentNames[studentNo]);
            printf(">> ");
            scanf(" %c", &confirm);
            confirm = tolower(confirm);

            if(confirm == 'y' || confirm == 'n') break;
            else printf(">> INVALID!\n");
        }

        if(confirm == 'n') continue;

        FILE *studentFile_R = fopen("students.txt", "r");
        char tempStudentNames[MAX_STUDENTS][MAX_NAME_LEN];
        char lineBuffer[MAX_NAME_LEN];
        int tempStudentCount = 0;

        if(!studentFile_R) {
            printf(">> ERROR: Can't find students text file!\n\n");
            return;
        }

        while(fgets(lineBuffer, sizeof(lineBuffer), studentFile_R)) {
            lineBuffer[strcspn(lineBuffer, "\n")] = 0;
            if(strcmp(lineBuffer, studentNames[studentNo]) == 0) continue;
            
            strcpy(tempStudentNames[tempStudentCount], lineBuffer);
            tempStudentCount++;
        }
        fclose(studentFile_R);

        
        FILE *studentFile_W = fopen("students.txt", "w");
        for(int i = 0; i < tempStudentCount; i++) {
            fprintf(studentFile_W, "%s\n", tempStudentNames[i]);
        }
        fclose(studentFile_W);

        printf(">> %s HAS BEEN REMOVED!\n", studentNames[studentNo]);
        refreshStudentList();
    }
}

void ModifyStudents() {
    while(true) {
        char choice;

        printf("\n\t\tCurrent Students\n");
        if(studentCount == 0) printf("\tNone...\n");
        else {
            for(int i = 0; i < studentCount; i++) {
                printf("\t%d. %s\n", i+1, studentNames[i]);
            }
        }

        printf("\n+ A: Add Student\n");
        printf("+ B: Remove Student\n");
        printf("+ C: Exit\n");
        printf(">> ");
        scanf(" %c", &choice);
        choice = tolower(choice);

        if(choice == 'a') AddStudent();
        else if(choice == 'b') RemoveStudent();
        else if(choice == 'c') break;
        else printf(">> INVALID!\n");
    }
}

int main() {
    refreshStudentList();

    while(true) {
        char choice;

        printf("------------------------------------------ << \n");
        printf("\n\t\tAttendify\n\n");
        
        printf("+ A: Start Attendance Check\n");
        printf("+ B: Modify Students\n");
        printf("+ C: Exit App\n");
        printf(">> ");
        scanf(" %c", &choice);
        choice = tolower(choice);

        if(choice == 'a') {
            printf(">> STARTING ATTENDANCE CHECK\n\n");
            printf("------------------------------------------ << \n");
            AttendanceCheck();
        } else if(choice == 'b') {
            printf(">> MODIFYING STUDENTS\n\n");
            printf("------------------------------------------ << \n");
            ModifyStudents();
        } else if(choice == 'c') {
            printf(">> EXITING APP\n");
            break;
        } else {
            printf(">> INVALID!\n");
            continue;
        }
    }
    return 0;
}