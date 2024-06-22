# Student Course Registration System

## Overview

This project is a small-scale, file-based course registration application that simulates a relational database system. It allows students to register for courses, manage instructors, and handle course registrations. The system is designed to meet specific functional requirements and is implemented using C.

## Functional Requirements

1. **Student Management:**
   - Add, delete, update student information.
   - Manage total courses and credits for each student.

2. **Instructor Management:**
   - Add, delete, update instructor information.
   - List courses taught by each instructor.

3. **Course Management:**
   - Add, delete, update course information.
   - Manage course quotas and assigned instructors.
   - List students registered for each course.

4. **Course Registration:**
   - Register students for courses, considering quotas and credit limits.
   - Update and delete course registrations.
   - List all courses taken by a student.
   - Save class lists to files.

## File Structure

- `main.c`: The main program file containing all the functionality.
- `data/Students.txt`: Contains student data.
- `data/Instructors.txt`: Contains instructor data.
- `data/Courses.txt`: Contains course data.
- `data/StudentCourseRegistrations.txt`: Contains course registration data.

## How to Run

1. **Compile the program:**
   ```sh
   gcc -o course_registration main.c
2. **Run the program:**
```sh
    ./course_registration
