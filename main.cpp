#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

#define MAX_STUDENTS 100

string studentNames[MAX_STUDENTS];
int studentCount = 0;

void refreshStudentList() {
	string lineBuffer;
	ifstream studentFile("students.txt");

	if(!studentFile) {
		cout << "ERROR: Can't find students text file!" << endl;
		return;
	}

	for(studentCount = 0; studentCount < MAX_STUDENTS && getline(studentFile, lineBuffer); studentCount++) {
		studentNames[studentCount] = lineBuffer;
	}

	studentFile.close();
}

void AttendanceCheck() {
	string studentStatus[MAX_STUDENTS];

	if(studentCount == 0) {
		cout << ">> NO STUDENTS TO CHECK FOR ATTENDANCE!" << "\n\n";
		return;
	}

	for(int i = 0; i < studentCount; i++) {
		while(1) {
			char status;

			cout << endl << i+1 << ". " << studentNames[i] << endl;
			cout << "\t+ A: Absent" << endl;
			cout << "\t+ B: Present" << endl;
			cout << "\t+ C: Late" << endl;
			cout << "\t>> ";
			cin >> status;
			status = tolower(status);

			switch(status) {
				case 'a': { studentStatus[i] = "Absent"; break; }
				case 'b': { studentStatus[i] = "Present"; break; }
				case 'c': { studentStatus[i] = "Late"; break; }
				default: { cout << ">> INVALID!" << endl; continue; }
			}

			cout << studentNames[i] << " is " << studentStatus[i] << endl;
			break;
		}
	}

	time_t currentTime = time(0);
	tm* localTime = localtime(&currentTime);

	string filename = "AttendanceLogs/attendance_"
		+ to_string(localTime->tm_mon + 1) + "-"
		+ to_string(localTime->tm_mday) + "-"
		+ to_string(localTime->tm_year % 100)
		+ ".txt";
	
	ofstream generatedAttendance(filename);
	if(!generatedAttendance) {
		cout << "ERROR: Can't find students text file!" << endl;
		return;
	}

	for(int i = 0; i < studentCount; i++) {
		generatedAttendance << studentStatus[i] << "\t\t|\t\t" << studentNames[i] << endl;
	}

	generatedAttendance.close();
	cout << "\n>> ATTENDANCE LOG SUCCESSFULLY GENERATED!" << "\n\n";
}

void ModifyStudents() {
	while(1) {
		char choice;

		cout << "\t\tCurrent Students" << endl;
		if(studentCount == 0) cout << "None..." << endl;
		else {
			for(int i = 0; i < studentCount; i++) {
				cout << i+1 << ". " << studentNames[i] << endl;
			}
		}

		cout << endl << "+ A: "
	}
}

int main() {
	refreshStudentList();

	while(1) {
		char choice;

		cout << endl << "\t\tTrackMate" << "\n\n";
		
		cout << "+ A: Start Attendance Check" << endl;
		cout << "+ B: Modify Students" << endl;
		cout << "+ C: Exit App" << endl;
		cout << ">> ";
		cin >> choice;
		choice = tolower(choice);

		if(choice == 'a') {
			cout << ">> STARTING ATTENDANCE CHECK" << endl;
			AttendanceCheck();
		} else if(choice == 'b') {
			cout << ">> MODIFYING STUDENTS" << endl;
			ModifyStudents();
		} else if(choice == 'c') {
			cout << ">> EXITING APP" << endl;
			break;
		} else {
			cout << ">> INVALID!" << endl;
			continue;
		}
	}
	return 0;
}