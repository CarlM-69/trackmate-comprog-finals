#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

/*
	MAX_STUDENTS		maximum limit of the students that can be added
	MAX_NAME_LEN		maximum student name length
*/
#define MAX_STUDENTS 100
#define MAX_NAME_LEN 256

/*
	studentNames		this is where we will store the names of every students who are inside students.txt
	studentCount		total number of students stored in studentNames
*/
char studentNames[MAX_STUDENTS][MAX_NAME_LEN];
int studentCount = 0;

/*
	populates the global variable
	studentNames from students.txt file
*/
void refreshStudentList() {
    char lineBuffer[MAX_NAME_LEN];
    FILE *studentFile = fopen("students.txt", "r");

	// checks if students.txt file can't be found
    if(!studentFile) {
        printf(">> ERROR: Can't find students text file!\n\n");
        return;
    }

    studentCount = 0;

	// iterates students.txt to check for non-empty lines
    while(studentCount < MAX_STUDENTS && fgets(lineBuffer, sizeof(lineBuffer), studentFile)) {
        lineBuffer[strcspn(lineBuffer, "\n")] = 0;
        
		/*
			if the line is not empty then we
			will store it in global studentNames
			assigned as studentCount
		*/
        if(strlen(lineBuffer) > 0) {
            strcpy(studentNames[studentCount], lineBuffer);
            studentCount++;
        }
    }

    fclose(studentFile);
}


/*
	iterates through the student list to record status (Present/Late/Absent).
	Automatically generates a dated text file (e.g., 'attendance_11-24-25.txt')
	inside the 'AttendanceLogs' folder to save the results.
*/
void AttendanceCheck() {
    char studentStatus[MAX_STUDENTS][12];

	// early exit if there are no students
    if(studentCount == 0) {
        printf(">> NO STUDENTS TO CHECK FOR ATTENDANCE!\n\n");
        return;
    }

	// loop through each student to check their status
    for(int i = 0; i < studentCount; i++) {

		// force valid input until A, B, or C is entered 
        while(true) {
            char status;

            printf("\n%d. %s\n", i+1, studentNames[i]);
            printf("\t+ A: Present\n");
            printf("\t+ B: Late\n");
            printf("\t+ C: Absent\n");
            printf("\t>> ");
            scanf(" %c", &status); 
            status = tolower(status);

			// maps the input to status string
            switch(status) {
                case 'a': { strcpy(studentStatus[i], "Present"); break; }
                case 'b': { strcpy(studentStatus[i], "Late"); break; }
                case 'c': { strcpy(studentStatus[i], "Absent"); break; }
                default: { printf(">> INVALID!\n"); continue; }
            }

			// confirmation message
            printf("%s is %s\n\n", studentNames[i], studentStatus[i]);
            break;
        }
    }

	// generates a filename based on the current date
    time_t currentTime = time(0);
    struct tm* localTime = localtime(&currentTime);

    char filename[100];
    sprintf(filename, "AttendanceLogs/attendance_%d-%d-%d.txt", 
            localTime->tm_mon + 1, 
            localTime->tm_mday, 
            localTime->tm_year % 100);
    
	// will finally create the log text file
    FILE *generatedAttendance = fopen(filename, "w");

	// will exit if AttendanceLogs folder can't be found
    if(!generatedAttendance) {
        printf(">> ERROR: Can't create attendance log!\n\n");
        return;
    }

	// write the data to the log text file
    for(int i = 0; i < studentCount; i++) {
        fprintf(generatedAttendance, "%s\t\t|\t\t%s\n", studentStatus[i], studentNames[i]);
    }

	// clean up and success message
    fclose(generatedAttendance);
    printf("\n>> ATTENDANCE LOG SUCCESSFULLY GENERATED!\n\n");
}

/*
	Interactively adds new, unique names
	to "students.txt" and updates the list
*/
void AddStudent() {
	// early exit if it already reached the max students
    if(studentCount >= MAX_STUDENTS) {
        printf(">> MAXIMUM STUDENTS LIMIT REACHED!\n\n");
        return;
    }

	// loop continuously to allow adding multiple students
    while(true) {
        char studentName[MAX_NAME_LEN];
        char confirm;

        printf("\nEnter a name to add (type 'exit' to stop): ");
        
		// clear buffer and read input safely
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 

        fgets(studentName, sizeof(studentName), stdin);
        studentName[strcspn(studentName, "\n")] = 0;

		// check exit input
        if(strcmp(studentName, "exit") == 0) break;
        int found = 0;

		// check for duplicate names
        for(int i = 0; i < studentCount; i++) {
            if(strcmp(studentName, studentNames[i]) != 0) continue;
            found = 1;
            break;
        }
        
		// restart if input student name already exists
        if(found) {
            printf(">> %s ALREADY EXISTS!\n", studentName);
            continue;
        }

		// force valid input until Y or N is entered
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

		// initialize stundents.txt
        FILE *studentFile = fopen("students.txt", "a");

		// check if students.txt is missing
        if(!studentFile) {
            printf(">> ERROR: Can't find students text file!\n\n");
            return;
        }

		// add the student to students.txt and success message
        fprintf(studentFile, "%s\n", studentName);
        fclose(studentFile);

        printf(">> %s HAS BEEN ADDED!\n", studentName);
    }

	// refresh the list so the modify student menu student list sync in
    refreshStudentList();
}

/*
	Interactively removes a student by their list number.
	Validates input, confirms deletion, and rewrites
	"students.txt" to exclude the entry.
*/
void RemoveStudent() {
	// early exit if theere's no students to remove
    if(studentCount <= 0) {
        printf(">> THERE ARE NO STUDENTS!\n\n");
        return;
    }

	// loop continuously to allow removing multiple students
    while(true) {
        printf("\n\t\tCurrent Students\n");

		// prints all the students stored in global studentNames
		for(int i = 0; i < studentCount; i++) {
			printf("\t%d. %s\n", i+1, studentNames[i]);
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

		// if not numeric then we'll have to restart
        if(!isNumeric) {
            printf("INVALID!\n");
            continue;
        }


		// checks if input is more than the total student count
        int studentNo = atoi(answer) - 1;
        if(studentNo >= studentCount || studentNo < 0) {
            printf(">> STUDENT NO. %d DOESN'T EXIST!", studentNo + 1);
            continue;
        }

		// force input until Y or N is entered
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

		// initialize students.txt
        FILE *studentFile_R = fopen("students.txt", "r");
        char tempStudentNames[MAX_STUDENTS][MAX_NAME_LEN];
        char lineBuffer[MAX_NAME_LEN];
        int tempStudentCount = 0;

		// check if students.exe is missing
        if(!studentFile_R) {
            printf(">> ERROR: Can't find students text file!\n\n");
            return;
        }

		// copy file to memory, skipping the deleted student
        while(fgets(lineBuffer, sizeof(lineBuffer), studentFile_R)) {
            lineBuffer[strcspn(lineBuffer, "\n")] = 0;

			// if true, this is the student we will delete, so skip iteration
            if(strcmp(lineBuffer, studentNames[studentNo]) == 0) continue;
            
            strcpy(tempStudentNames[tempStudentCount], lineBuffer);
            tempStudentCount++;
        }
        fclose(studentFile_R);

        // overwrite file with the updated list
        FILE *studentFile_W = fopen("students.txt", "w");
        for(int i = 0; i < tempStudentCount; i++) {
            fprintf(studentFile_W, "%s\n", tempStudentNames[i]);
        }
        fclose(studentFile_W);

		// success message and refresh list
        printf(">> %s HAS BEEN REMOVED!\n", studentNames[studentNo]);
        refreshStudentList();
    }
}

/*
	sub-menu to view the current students
	and trigger Add/Remove operations.
*/
void ModifyStudents() {
	// keep showing the menu until 'Exit' is chosen
    while(true) {
        char choice;

		// display the current list of students
        printf("\n\t\tCurrent Students\n");

		// if nothing is in the students.txt
        if(studentCount == 0) printf("\tNone...\n");
        else {
			// print every students
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

		// call the appropriate function or break loop
        if(choice == 'a') AddStudent();
        else if(choice == 'b') RemoveStudent();
        else if(choice == 'c') break;
        else printf(">> INVALID!\n");
    }
}

/*
	Initializes the student list and runs the main 
	menu loop to route user commands (Attendance, Modify, or Exit).
*/
int main() {
	// initialize students.txt
    refreshStudentList();

	// keep the program running until explicit exit
    while(true) {
        char choice;

		// display the program title and main menu options
        printf("------------------------------------------ << \n");
        printf("\n\t\tAttendify\n\n");
        
        printf("+ A: Start Attendance Check\n");
        printf("+ B: Modify Students\n");
        printf("+ C: Exit App\n");
        printf(">> ");
        scanf(" %c", &choice);
        choice = tolower(choice);

		// execute the selected feature or handle errors
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