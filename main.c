#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 256

char studentNames[MAX_STUDENTS][MAX_NAME_LENGTH];
int studentCount = 0;

void initStudentList() {
	memset(studentNames, 0, sizeof(studentNames));
	studentCount = 0;

	char line_buffer[MAX_NAME_LENGTH];
	FILE *studentsFile = fopen("students.txt", "r");

	if(studentsFile == NULL) {
		printf("STUDENTS FILE NAMES CAN'T BE FOUND!\n");
		return;
	}

	while(fgets(line_buffer, MAX_NAME_LENGTH, studentsFile)) {
		line_buffer[strcspn(line_buffer, "\n")] = '\0';
		strcpy(studentNames[studentCount], line_buffer);
		studentCount++;
	}
	fclose(studentsFile);
}

void startAttendance() {
	char *studentStatus[MAX_STUDENTS];

	if(studentCount == 0) {
		printf("THERE ARE NO STUDENTS TO CHECK FOR ATTENDANCE!");
		return;
	}

	for(int i = 0; i < studentCount; i++) {
		while(1) {
			int status;
			char *statusStr;

			printf("\n%d. %s\n", i+1, studentNames[i]);
			printf("\t0 - Absent\n");
			printf("\t1 - Present\n");
			printf("\t2 - Late\n\n");
			printf("SET STATUS: ");
			scanf("%d", &status);

			switch(status) {
				case 0: { statusStr = "Absent"; break; }
				case 1: { statusStr = "Present"; break; }
				case 2: { statusStr = "Late"; break; }
				default: { printf("INVALID STATUS!\n"); continue; }
			}

			studentStatus[i] = statusStr;
			printf("%s is %s\n", studentNames[i], statusStr);
			break;
		}
	}

	time_t currentTime = time(NULL);
	struct tm *localTime = localtime(&currentTime);
	char filename[100];

	sprintf(filename, "AttendanceLogs/attendance_%d-%d-%d.txt",
		localTime->tm_mon + 1,
		localTime->tm_mday,
		(localTime->tm_year + 1900) % 100
	);

	FILE *attendanceFile = fopen(filename, "w");
	if(attendanceFile == NULL) {
		printf("ATTENDANCE LOG CAN'T BE SAVE!\n");
		return;
	}

	for(int i = 0; i < studentCount; i++) {
		fprintf(attendanceFile, "%s\t\t|\t\t%s\n", studentStatus[i], studentNames[i]);
	}
	fclose(attendanceFile);

	printf("\n-------------------------------------\n");
	printf("ATTENDANCE FOR TODAY IS NOW RECORDED!\n");
	printf("-------------------------------------");
}

void studentAdd() {
    FILE *studentFile = fopen("students.txt", "a");

    if(studentFile == NULL) {
        printf("STUDENT FILE NAMES CAN'T BE FOUND!\n");
        return;
    }

    char studentName[MAX_NAME_LENGTH];
    int confirm;

    while(getchar() != '\n'); 
    while(1) {
        printf("\nEnter a Name (type 'exit' to stop): ");
        fgets(studentName, MAX_NAME_LENGTH, stdin);
        studentName[strcspn(studentName, "\n")] = '\0';
        if(strcmp(studentName, "exit") == 0) break;

        printf("\nAre you sure you want to add\n");
        printf("       %s?\n", studentName);
        printf("0 - No\n");
        printf("1 - Yes\n");
        printf("CONFIRM: ");

        scanf("%d", &confirm);
        while(getchar() != '\n'); 

        if(confirm == 0) continue;
        else if(confirm == 1) {
            fprintf(studentFile, "%s\n", studentName);
            printf("SUCCESSFULLY ADDED!\n");
        }
        else printf("INVALID OPTION!\n");
    }

    fclose(studentFile);
	initStudentList();
}

void studentRemove() {
    if (studentCount == 0) {
        printf("THERE ARE NO STUDENTS TO REMOVE!\n");
        return;
    }

    char studentName[MAX_NAME_LENGTH];
    int confirm;

    while(getchar() != '\n');
    while(1) {
        printf("\nEnter a Name to remove (type 'exit' to stop): ");
        fgets(studentName, MAX_NAME_LENGTH, stdin);
        studentName[strcspn(studentName, "\n")] = '\0';

        if(strcmp(studentName, "exit") == 0) break;
        int found = -1;

        for(int i = 0; i < studentCount; i++) {
            if(strcmp(studentNames[i], studentName) == 0) {
                found = i;
                break;
            }
        }

        if(found == -1) {
            printf("STUDENT NOT FOUND!\n");
            continue;
        }

        printf("\nAre you sure you want to remove\n");
        printf("       %s?\n", studentName);
        printf("0 - No\n");
        printf("1 - Yes\n");
        printf("CONFIRM: ");
        scanf("%d", &confirm);
        while(getchar() != '\n');

        if(confirm == 1) {
            for(int j = found; j < studentCount - 1; j++) {
                strcpy(studentNames[j], studentNames[j + 1]);
            }
            studentCount--;
            printf("SUCCESSFULLY REMOVED!\n");

            FILE *studentFile = fopen("students.txt", "w");
            if(studentFile == NULL) {
                printf("STUDENT FILE NAMES CAN'T BE FOUND!\n");
                return;
            }

            for(int i = 0; i < studentCount; i++) {
                fprintf(studentFile, "%s\n", studentNames[i]);
            }
			
            fclose(studentFile);
        } else if (confirm == 0) continue;
		else printf("INVALID OPTION!\n");
    }

    initStudentList();
}

void modifyStudents() {
	int choice;

	while(1) {
		printf("\nCURRENT STUDENTS:\n");
		if(studentCount == 0) printf("None\n");
		else {
			for(int i = 0; i < studentCount; i++) {
				printf("%d. %s\n", i+1, studentNames[i]);
			}
		}

		printf("\n0 - Remove Student\n");
		printf("1 - Add Student\n");
		printf("2 - Exit\n");
		printf("SELECT AN ACTION: ");
		scanf("%d", &choice);

		if(choice == 0) studentRemove();
		else if(choice == 1) studentAdd();
		else if(choice == 2) break;
		else printf("INVALID ACTION!\n");
	}
}

int main() {
	initStudentList();

	while(1) {
		int choice;
		printf("\n\n\t\tTrackMate\n");

		printf("\n0 - Start Attendance Checking\n");
		printf("1 - Modify Students\n");
		printf("2 - Exit App\n");
		printf("SELECT AN ACTION: ");
		scanf("%d", &choice);

		if(choice == 0) {
			printf("STARTING ATTENDANCE...\n");
			startAttendance();
		} else if(choice == 1) {
			printf("MODIFYING STUDENTS...\n");
			modifyStudents();
		} else if(choice == 2) {
			printf("EXITING...\n");
			break;
		} else printf("INVALID ACTION!\n");
	}

	return 0;
}