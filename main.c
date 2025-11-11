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

	for(int i = 0; i < studentCount; i++) {
		fprintf(attendanceFile, "%s\t\t|\t\t%s\n", studentStatus[i], studentNames[i]);
	}
	fclose(attendanceFile);

	printf("\n-------------------------------------\n");
	printf("ATTENDANCE FOR TODAY IS NOW RECORDED!\n");
	printf("-------------------------------------\n\n");
}

void studentAdd() {
	FILE *studentFile = fopen("students.txt", "a");
	char studentName;
	int confirm;

	while(1) {
		printf("\n Enter a Name: ");
		scanf("%c ", &studentName);

		printf("\nAre you sure you want to add\n");
		printf("\t%s?\n", studentName);
		printf("0 - No\n");
		printf("1 - Yes\n");
		printf("CONFIRM: ");
		scanf("%d", &confirm);

		if(confirm == 0) continue;
		else if(confirm == 1) {
			sprintf(studentName, "%s\n", studentName);
			fputs(studentName, studentFile);

			printf("SUCCESSFULLY ADDED!\n");
			initStudentList();
		} else printf("INVALID OPTION!\n");
	}

	fclose(studentFile);
}

void modifyStudents() {
	int choice;

	printf("\nCURRENT STUDENTS:\n");
	for(int i = 0; i < studentCount; i++) {
		printf("%d. %s\n", i+1, studentNames[i]);
	}

	while(1) {
		printf("\n0 - Remove Student\n");
		printf("1 - Add Student\n");
		printf("SELECT AN ACTION: ");
		scanf("%d", &choice);

		if(choice == 0) {

		} else if(choice == 1) {
			studentAdd();
		} else printf("INVALID ACTION!\n");
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