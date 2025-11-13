# TrackMate

TrackMate is a simple **C++ console application** designed to help teachers or organizers manage student attendance efficiently. It allows adding, removing, and tracking students while generating daily attendance logs automatically.

---

## Features

- **Attendance Check**  
  Record the attendance status of students as **Present, Late, or Absent**.

- **Student Management**  
  Easily **add** or **remove** students from the system with checks to prevent duplicates.

- **Automatic Log Generation**  
  Attendance logs are automatically saved in a folder named `AttendanceLogs` with a **timestamped filename** for easy tracking.

- **User-Friendly Interface**  
  Interactive console menus make navigation simple and intuitive.

---

## File Structure

TrackMate/
├─ AttendanceLogs/ # Folder where attendance logs are saved
├─ students.txt # list of student names
├─ README.md # project documentation
└─ main.cpp # main program file

---

## How It Works

1. On startup, `TrackMate` reads the list of students from `students.txt`.
2. Users can choose to:
   - Start an attendance check
   - Modify the student list
   - Exit the application
3. During an attendance check, each student is prompted for their status:
   - **A** – Present  
   - **B** – Late  
   - **C** – Absent
4. After completing attendance, a log file is generated in the `AttendanceLogs` folder with the format:  
   `attendance_MM-DD-YY.txt`

5. Adding or removing students updates `students.txt` automatically.

---

## Usage

1. Compile the program with a C++ compiler:

```bash
g++ main.cpp -o TrackMate
