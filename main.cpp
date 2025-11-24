#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <limits>
using namespace std;

#define MAX_STUDENTS 100

string studentNames[MAX_STUDENTS];
int studentCount = 0; 

void refreshStudentList() {
	string lineBuffer;
	ifstream studentFile("students.txt");

	if(!studentFile) {
		cout << ">> ERROR: Can't find students text file!" << "\n\n";
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
		while(true) {
			char status;

			cout << endl << i+1 << ". " << studentNames[i] << endl;
			cout << "\t+ A: Present" << endl;
			cout << "\t+ B: Late" << endl;
			cout << "\t+ C: Absent" << endl;
			cout << "\t>> ";
			cin >> status;
			status = tolower(status);

			switch(status) {
				case 'a': { studentStatus[i] = "Present"; break; }
				case 'b': { studentStatus[i] = "Late"; break; }
				case 'c': { studentStatus[i] = "Absent"; break; }
				default: { cout << ">> INVALID!" << endl; continue; }
			}

			cout << studentNames[i] << " is " << studentStatus[i] << "\n\n";
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
		cout << ">> ERROR: Can't create attendance log!" << "\n\n";
		return;
	}

	for(int i = 0; i < studentCount; i++) {
		generatedAttendance << studentStatus[i] << "\t\t|\t\t" << studentNames[i] << endl;
	}

	generatedAttendance.close();
	cout << "\n>> ATTENDANCE LOG SUCCESSFULLY GENERATED!" << "\n\n";
}

void AddStudent() {
	if(studentCount >= MAX_STUDENTS) {
		cout << ">> MAXIMUM STUDENTS LIMIT REACHED!" << "\n\n";
		return;
	}

	while(true) {
		string studentName;
		char confirm;

		cout << endl << "Enter a name to add (type 'exit' to stop): ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, studentName);

		if(studentName == "exit") break;
		int found = 0;

		//handles name duplication
		for(int i = 0; i < studentCount; i++) {
			if(studentName != studentNames[i]) continue;
			found = 1;
			break;
		}
		
		if(found) {
			cout << ">> " << studentName << " ALREADY EXISTS!" << endl;
			continue;
		}

		while(true) {
			cout << "Are you sure you want to add" << endl;
			cout << "+ " << studentName << "? (Y/n)" << endl;
			cout << ">> ";
			cin >> confirm;
			confirm = tolower(confirm);

			if(confirm == 'y' || confirm == 'n') break;
			else cout << ">> INVALID!" << endl;
		}

		if(confirm == 'n') continue;

		ofstream studentFile("students.txt", ios::app);
		if(!studentFile) {
			cout << ">> ERROR: Can't find students text file!" << "\n\n";
			return;
		}

		studentFile << studentName << endl;
		studentFile.close();

		cout << ">> " << studentName << " HAS BEEN ADDED!" << endl;
	}

	refreshStudentList();
}

void RemoveStudent() {
	if(studentCount <= 0) {
		cout << ">> THERE ARE NO STUDENTS!" << "\n\n";
		return;
	}

	while(true) {
		cout << "\n\t\tCurrent Students" << endl;
		if(studentCount == 0) cout << "\tNone..." << endl;
		else {
			for(int i = 0; i < studentCount; i++) {
				cout << "\t" << i+1 << ". " << studentNames[i] << endl;
			}
		}

		string answer;
		char confirm;

		cout << endl << "Enter a student number to remove (type 'exit' to stop): ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, answer);

		if(answer == "exit") break;
		bool isNumeric = true;

		for(char c : answer) {
			if(isdigit(c)) continue;

			isNumeric = false;
			break;
		}

		if(!isNumeric) {
			cout << "INVALID!" << endl;
			continue;
		}

		int studentNo = stoi(answer) - 1;
		if(studentNo > studentCount || studentNo < 0) {
			cout << ">> STUDENT NO. " << studentNo << " DOESN'T EXIST!";
			continue;
		}

		while(true) {
			cout << "Are you sure you want to remove" << endl;
			cout << "+ " << studentNames[studentNo] << "? (Y/n)" << endl;
			cout << ">> ";
			cin >> confirm;
			confirm = tolower(confirm);

			if(confirm == 'y' || confirm == 'n') break;
			else cout << ">> INVALID!" << endl;
		}

		if(confirm == 'n') continue;

		ifstream studentFile_R("students.txt");
		string tempStudentNames[MAX_STUDENTS];
		string lineBuffer;
		int tempStudentCount = 0;

		if(!studentFile_R) {
			cout << ">> ERROR: Can't find students text file!" << "\n\n";
			return;
		}

		while(getline(studentFile_R, lineBuffer)) {
			if(lineBuffer == studentNames[studentNo]) continue;
			tempStudentNames[tempStudentCount] = lineBuffer;
			tempStudentCount++;
		}
		studentFile_R.close();

		//ano meaning ng studentFile_W??
		ofstream studentFile_W("students.txt");
		for(int i = 0; i < tempStudentCount; i++) {
			studentFile_W << tempStudentNames[i] << endl;
		}
		studentFile_W.close();

		cout << ">> " << studentNames[studentNo] << " HAS BEEN REMOVED!" << endl;
		refreshStudentList();
	}
}

void ModifyStudents() {
	while(true) {
		char choice;

		cout << "\n\t\tCurrent Students" << endl;
		if(studentCount == 0) cout << "\tNone..." << endl;
		else {
			for(int i = 0; i < studentCount; i++) {
				cout << "\t" << i+1 << ". " << studentNames[i] << endl;
			}
		}

		cout << endl << "+ A: Add Student" << endl;
		cout << "+ B: Remove Student" << endl;
		cout << "+ C: Exit" << endl;
		cout << ">> ";
		cin >> choice;
		choice = tolower(choice);

		if(choice == 'a') AddStudent();
		else if(choice == 'b') RemoveStudent();
		else if(choice == 'c') break;
		else cout << ">> INVALID!" << endl;
	}
}

int main() {
	refreshStudentList();

	while(true) {
		char choice;

		cout << "------------------------------------------ << " << endl;
		cout << endl << "\t\tAttendify" << "\n\n";
		
		cout << "+ A: Start Attendance Check" << endl;
		cout << "+ B: Modify Students" << endl;
		cout << "+ C: Exit App" << endl;
		cout << ">> ";
		cin >> choice;
		choice = tolower(choice);

		if(choice == 'a') {
			cout << ">> STARTING ATTENDANCE CHECK" << "\n\n";
			cout << "------------------------------------------ << " << endl;
			AttendanceCheck();
		} else if(choice == 'b') {
			cout << ">> MODIFYING STUDENTS" << "\n\n";
			cout << "------------------------------------------ << " << endl;
			ModifyStudents();
		} else if(choice == 'c') {
			cout << ">> EXITING APP" << "\n";
			break;
		} else {
			cout << ">> INVALID!" << endl;
			continue;
		}
	}
	return 0;
}