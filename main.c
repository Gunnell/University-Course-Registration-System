#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define MAXCREDIT 23
#define MAXCOURSE 9
#define WARNING "malloc failed!"

typedef struct Student {
    char firstName[30];
    char lastName[20];
    char studentID[9]; //PK
    int totalCredit;
    int totalCourses;
    struct Student *next;
} STUDENT;

typedef struct Instructor {
    char firstName[30];
    char lastName[20];
    char id[8]; //PK
    char title[7];
    struct Instructor *next;
} INSTRUCTOR;

typedef struct Course {
    char name[30];
    char code[9]; //PK
    char instructorID[8];
    int credit;
    int quota;
    struct Course *next;
} COURSE;

typedef struct CourseRegistration {
    int registrationNo; //PK
    char date[15];
    char studentID[8];
    char courseCode[9];
    int status;
    struct CourseRegistration *next;
} COURSEREG;

// Function Prototypes
STUDENT* create_new_student(void);
void insert_student_to_list(STUDENT *, STUDENT **);
void print_list_to_file_student(STUDENT **studentHead);
int delete_student(char*, STUDENT**);
bool studentIDControl(STUDENT**, char[]);

INSTRUCTOR* create_new_instructor(void);
void insert_instructor_to_list(INSTRUCTOR *, INSTRUCTOR **);
void print_list_to_file_instructor(INSTRUCTOR **);
int delete_instructor(char*, INSTRUCTOR**);
bool instructorIDControl(INSTRUCTOR**, char[]);
bool update_title(char [], INSTRUCTOR **, char []);
void view_courses_by_instructor(COURSE **, char []);

COURSE* create_new_course(void);
void insert_course_to_list(COURSE *, COURSE **);
void print_list_to_file_course(COURSE **);
int delete_course(char*, COURSE**);
bool courseControl(COURSE**, char[]);
bool update_course(char [], COURSE **, char []);

COURSEREG* create_new_registration(int, char [], char []);
void insert_registration_to_list(COURSEREG **, COURSEREG *);
void print_list_to_file_registration(COURSEREG **);
int check_course_quota(COURSE **, char[]);
int check_student_credit(STUDENT **, char[]);
bool has_taken_before(COURSEREG **, char[], char[]);
void update_course_quota(COURSE **, char[]);
void update_student_credit(STUDENT **, char[], int);
int delete_course_registration(int , COURSEREG **);
int update_course_registration(int , COURSEREG **, char [], char []);
void save_class_list_to_file(COURSEREG **, char [], char []);

int main(void) {
    int choice = -10;

    // Students
    STUDENT *studentHead = NULL;
    char studentID[8];
    STUDENT *newStudent;
    
    FILE *reader = fopen("data/Students.txt", "r");
    if (reader == NULL) {
        printf("File could not be opened\n");
    } else {
        while (!feof(reader)) {
            STUDENT *tmp;
            tmp = (STUDENT*)malloc(sizeof(STUDENT));
            if (tmp == NULL) {
                printf(WARNING);
            }
            fscanf(reader, "%s %s %s %d %d\n", tmp->firstName, tmp->lastName, tmp->studentID, &tmp->totalCredit, &tmp->totalCourses);
            tmp->next = NULL;
            insert_student_to_list(tmp, &studentHead);
        }
        fclose(reader);
    }

    // Instructors
    INSTRUCTOR *instructorHead = NULL;
    char instructorID[8];
    char instructorID2[8];
    INSTRUCTOR *newInstructor;
    char updatedID2[8];
    char newTitle[7];
    reader = fopen("data/Instructors.txt", "r");
    if (reader == NULL) {
        printf("File could not be opened\n");
    } else {
        while (!feof(reader)) {
            INSTRUCTOR *tmp1;
            tmp1 = (INSTRUCTOR*)malloc(sizeof(INSTRUCTOR));
            if (tmp1 == NULL) {
                printf(WARNING);
            }
            fscanf(reader, "%s %s %s %s\n", tmp1->firstName, tmp1->lastName, tmp1->id, tmp1->title);
            tmp1->next = NULL;
            insert_instructor_to_list(tmp1, &instructorHead);
        }
        fclose(reader);
    }

    // Courses
    COURSE *courseHead = NULL;
    COURSE *newCourse;
    char updatedInstructorID[8];
    char updatedCourseCode[9];
    char courseCode[9];
    reader = fopen("data/Courses.txt", "r");
    if (reader == NULL) {
        printf("File could not be opened\n");
    } else {
        while (!feof(reader)) {
            COURSE *tmp2;
            tmp2 = (COURSE*)malloc(sizeof(COURSE));
            if (tmp2 == NULL) {
                printf(WARNING);
            }
            fscanf(reader, "%s %s %s %d %d\n", tmp2->name, tmp2->code, tmp2->instructorID, &tmp2->credit, &tmp2->quota);
            tmp2->next = NULL;
            insert_course_to_list(tmp2, &courseHead);
        }
        fclose(reader);
    }

    // Course Registrations
    int registrationNo = 0;
    COURSEREG *courseRegHead = NULL;
    int credit;
    COURSEREG *newRegistration;
    char studentIDReg[10];
    char courseCodeReg[10];
    reader = fopen("data/StudentCourseRegistrations.txt", "r");
    if (reader == NULL) {
        printf("File could not be opened\n");
    } else {
        while (!feof(reader)) {
            COURSEREG *tmp;
            tmp = (COURSEREG*)malloc(sizeof(COURSEREG));
            if (tmp == NULL) {
                printf(WARNING);
            }
            fscanf(reader, "%d  %s  %s  %s  %d\n", &tmp->registrationNo, tmp->date, tmp->studentID, tmp->courseCode, &tmp->status);
            tmp->next = NULL;
            insert_registration_to_list(&courseRegHead, tmp);
        }
        fclose(reader);
    }

    while (choice != 0) {
        printf("For Student operations press 1\n"
               "For Instructor operations press 2\n"
               "For Course operations press 3\n"
               "For Course Registration operations press 4\n"
               "To view students registered to a course press 5\n"
               "To view all courses taken by a student press 6\n"
               "To view the class list of a course press 7\n"
               "To exit all operations press 0\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("---Student Operations---\n");
                while (choice != -1) {
                    printf("1->Add Student\n2->Delete Student\n-1->Return to Main Menu\n");
                    scanf("%d", &choice);
                    switch (choice) {
                        case 1:
                            newStudent = create_new_student();
                            if (!studentIDControl(&studentHead, newStudent->studentID)) {
                                insert_student_to_list(newStudent, &studentHead);
                                print_list_to_file_student(&studentHead);
                            } else {
                                printf("A student with this ID already exists, the operation could not be completed.\n");
                                free(newStudent);
                            }
                            break;
                        case 2:
                            printf("Enter the ID of the student you want to delete:\n");
                            int result = -1;
                            scanf("%s", studentID);
                            while (strlen(studentID) != 8) {
                                printf("Please enter an 8-digit code\n");
                                scanf("%s", studentID);
                            }
                            result = delete_student(studentID, &studentHead);
                            if (result == 1)
                                print_list_to_file_student(&studentHead);
                            break;
                        default:
                            while (choice != -1) {
                                printf("Please press only -1 to exit\n");
                                scanf("%d", &choice);
                            }
                            break;
                    }
                }
                break;
            case 2:
                printf("---Instructor Operations---\n");
                while (choice != -1) {
                    printf("1->Add Instructor\n2->Delete Instructor\n3->Update Instructor Information\n4->View Courses Taught by Instructor\n-1->Return to Main Menu\n");
                    scanf("%d", &choice);
                    switch (choice) {
                        case 1:
                            newInstructor = create_new_instructor();
                            if (!instructorIDControl(&instructorHead, newInstructor->id)) {
                                insert_instructor_to_list(newInstructor, &instructorHead);
                                print_list_to_file_instructor(&instructorHead);
                            } else {
                                printf("An instructor with this ID already exists, the operation could not be completed.\n");
                                free(newInstructor);
                            }
                            break;
                        case 2:
                            printf("Enter the ID of the instructor you want to delete:\n");
                            int result = -1;
                            scanf("%s", instructorID);
                            while (strlen(instructorID) != 7) {
                                printf("Please enter a 7-digit code\n");
                                scanf("%s", instructorID);
                            }
                            result = delete_instructor(instructorID, &instructorHead);
                            if (result == 1)
                                print_list_to_file_instructor(&instructorHead);
                            break;
                        case 3:
                            printf("Enter the ID of the instructor whose title you want to update:\n");
                            scanf("%s", updatedID2);
                            while (strlen(updatedID2) != 7) {
                                printf("Please enter a 7-digit code\n");
                                scanf("%s", updatedID2);
                            }
                            printf("Enter the new title:\n");
                            scanf("%s", newTitle);
                            while (strlen(newTitle) >= 6) {
                                printf("Abbreviations can be a maximum of 5 characters\n");
                                scanf("%s", newTitle);
                            }
                            if (instructorIDControl(&instructorHead, updatedID2)) {
                                if (update_title(updatedID2, &instructorHead, newTitle))
                                    print_list_to_file_instructor(&instructorHead);
                            } else {
                                printf("No instructor found with this ID, the operation could not be completed.\n");
                            }
                            break;
                        case 4:
                            printf("Enter the ID of the instructor to view their courses:\n");
                            scanf("%s", instructorID2);
                            while (strlen(instructorID2) != 7) {
                                printf("Please enter a 7-digit code\n");
                                scanf("%s", instructorID2);
                            }
                            if (instructorIDControl(&instructorHead, instructorID2)) {
                                view_courses_by_instructor(&courseHead, instructorID2);
                            } else {
                                printf("No instructor found with this ID, the operation could not be completed.\n");
                            }
                            break;
                        default:
                            while (choice != -1) {
                                printf("Please press only -1 to exit\n");
                                scanf("%d", &choice);
                            }
                            break;
                    }
                }
                break;
            case 3:
                printf("---Course Operations---\n");
                while (choice != -1) {
                    printf("1->Add Course\n2->Delete Course\n3->Update Course\n-1->Return to Main Menu\n");
                    scanf("%d", &choice);
                    switch (choice) {
                        case 1:
                            newCourse = create_new_course();
                            if (!courseControl(&courseHead, newCourse->code) && instructorIDControl(&instructorHead, newCourse->instructorID)) {
                                insert_course_to_list(newCourse, &courseHead);
                                print_list_to_file_course(&courseHead);
                            } else {
                                printf("The operation could not be completed, either the code has been assigned to another course or no such instructor exists...\n");
                                free(newCourse);
                            }
                            break;
                        case 2:
                            printf("Enter the code of the course you want to delete:\n");
                            int result = -1;
                            scanf("%s", courseCode);
                            while (strlen(courseCode) != 7) {
                                printf("Please enter a 7-digit code\n");
                                scanf("%s", courseCode);
                            }
                            result = delete_course(courseCode, &courseHead);
                            if (result == 1)
                                print_list_to_file_course(&courseHead);
                            break;
                        case 3:
                            printf("Enter the code of the course you want to update:\n");
                            scanf("%s", updatedCourseCode);
                            while (strlen(updatedCourseCode) != 7) {
                                printf("Please enter a 7-digit code\n");
                                scanf("%s", updatedCourseCode);
                            }
                            printf("Enter the ID of the instructor to be assigned to the course:\n");
                            scanf("%s", updatedInstructorID);
                            while (strlen(updatedInstructorID) != 7) {
                                printf("Please enter a 7-digit code\n");
                                scanf("%s", updatedInstructorID);
                            }
                            if (instructorIDControl(&instructorHead, updatedInstructorID)) {
                                if (update_course(updatedCourseCode, &courseHead, updatedInstructorID))
                                    print_list_to_file_course(&courseHead);
                            } else {
                                printf("No instructor found with this ID, the operation could not be completed.\n");
                            }
                            break;
                        default:
                            while (choice != -1) {
                                printf("Please press only -1 to exit\n");
                                scanf("%d", &choice);
                            }
                            break;
                    }
                }
                break;
            case 4:
                printf("---Course Registration Operations---\n");
                while (choice != -1) {
                    printf("1->Add Registration\n2->Delete Registration\n3->Update Registration\n-1->Return to Main Menu\n");
                    scanf("%d", &choice);
                    switch (choice) {
                        case 1:
                            printf("Enter the course code:\n");
                            scanf("%s", courseCodeReg);
                            while (strlen(courseCodeReg) != 7) {
                                printf("Please enter a 7-digit code:\n");
                                scanf("%s", courseCodeReg);
                            }
                            credit = check_course_quota(&courseHead, courseCodeReg);
                            if (credit != 0) {
                                printf("Enter the student's ID to register for the course:\n");
                                scanf("%s", studentIDReg);
                                while (strlen(studentIDReg) != 8) {
                                    printf("Please enter an 8-digit code\n");
                                    scanf("%s", studentIDReg);
                                }
                                if (credit + check_student_credit(&studentHead, studentIDReg) <= MAXCREDIT && !has_taken_before(&courseRegHead, studentIDReg, courseCodeReg)) {
                                    registrationNo++;
                                    newRegistration = create_new_registration(registrationNo, studentIDReg, courseCodeReg);
                                    insert_registration_to_list(&courseRegHead, newRegistration);
                                    update_course_quota(&courseHead, courseCodeReg);
                                    update_student_credit(&studentHead, studentIDReg, credit);
                                    print_list_to_file_registration(&courseRegHead);
                                }
                            } else {
                                printf("The operation could not be completed\n");
                            }
                            break;
                        case 2:
                            printf("Enter the registration number to delete:\n");
                            int regNo;
                            scanf("%d", &regNo);
                            if (delete_course_registration(regNo, &courseRegHead)) {
                                print_list_to_file_registration(&courseRegHead);
                            }
                            break;
                        case 3:
                            printf("Enter the registration number to update:\n");
                            scanf("%d", &regNo);
                            printf("Enter the new course code:\n");
                            char newCourseCode[9];
                            scanf("%s", newCourseCode);
                            printf("Enter the new student ID:\n");
                            char newStudentID[8];
                            scanf("%s", newStudentID);
                            if (update_course_registration(regNo, &courseRegHead, newCourseCode, newStudentID)) {
                                print_list_to_file_registration(&courseRegHead);
                            }
                            break;
                        default:
                            while (choice != -1) {
                                printf("Please press only -1 to exit\n");
                                scanf("%d", &choice);
                            }
                            break;
                    }
                }
                break;
            case 5:
                printf("---Students Registered to a Course---\n");
                while (choice != -1) {
                    printf("Please press only -1 to return to the main menu\n");
                    scanf("%d", &choice);
                }
                break;
            case 6:
                printf("---All Courses Taken by a Student---\n");
                while (choice != -1) {
                    printf("Please press only -1 to return to the main menu\n");
                    scanf("%d", &choice);
                }
                break;
            case 7:
                printf("---Class List of a Course---\n");
                while (choice != -1) {
                    printf("Please press only -1 to return to the main menu\n");
                    scanf("%d", &choice);
                }
                break;
        }
    }

    printf("Program is closing... :)\n");
    return 0;
}

STUDENT* create_new_student(void) {
    STUDENT *new_node;
    new_node = (STUDENT*)malloc(sizeof(STUDENT));
    if (new_node == NULL) {
        printf(WARNING);
        exit(1);
    }

    // Getting inputs from user
    printf("\nEnter student information..\n");
    printf("First Name:");
    scanf("%s", new_node->firstName);
    printf("Last Name:");
    scanf("%s", new_node->lastName);
    printf("Student ID:");
    scanf("%s", new_node->studentID);
    while (strlen(new_node->studentID) != 8) {
        printf("Please enter an 8-digit code\n");
        scanf("%s", new_node->studentID);
    }
    new_node->totalCredit = 0;
    new_node->totalCourses = 0;
    new_node->next = NULL;
    return new_node;
}

INSTRUCTOR* create_new_instructor(void) {
    INSTRUCTOR *new_node;
    new_node = (INSTRUCTOR*)malloc(sizeof(INSTRUCTOR));
    if (new_node == NULL) {
        printf(WARNING);
        exit(1);
    }

    // Getting inputs from user
    printf("\nEnter instructor information..\n");
    printf("First Name:");
    scanf("%s", new_node->firstName);
    printf("Last Name:");
    scanf("%s", new_node->lastName);
    printf("ID:");
    scanf("%s", new_node->id);
    while (strlen(new_node->id) != 7) {
        printf("Please enter a 7-digit code\n");
        scanf("%s", new_node->id);
    }
    printf("Title:");
    scanf("%s", new_node->title);
    new_node->next = NULL;
    return new_node;
}

COURSE* create_new_course(void) {
    COURSE *new_node;
    new_node = (COURSE*)malloc(sizeof(COURSE));
    if (new_node == NULL) {
        printf(WARNING);
        exit(1);
    }

    // Getting inputs from user
    printf("\nEnter course information..\n");
    printf("Name:");
    scanf("%s", new_node->name);
    printf("Course Code:");
    scanf("%s", new_node->code);
    while (strlen(new_node->code) != 7) {
        printf("Please enter a 7-digit code\n");
        scanf("%s", new_node->code);
    }
    printf("Instructor ID:");
    scanf("%s", new_node->instructorID);
    while (strlen(new_node->instructorID) != 7) {
        printf("Please enter a 7-digit ID\n");
        scanf("%s", new_node->instructorID);
    }
    printf("Course Credit:");
    scanf("%d", &new_node->credit);
    printf("Course Quota:");
    scanf("%d", &new_node->quota);
    new_node->next = NULL;
    return new_node;
}

COURSEREG* create_new_registration(int registrationNo, char studentID[], char courseCode[]) {
    COURSEREG *new_node;
    new_node = (COURSEREG*)malloc(sizeof(COURSEREG));
    if (new_node == NULL) {
        printf(WARNING);
        exit(1);
    }
    new_node->registrationNo = registrationNo;
    printf("Enter the date:\n");
    scanf("%s", new_node->date);
    strcpy(new_node->studentID, studentID);
    strcpy(new_node->courseCode, courseCode);
    new_node->status = 1;
    new_node->next = NULL;
    return new_node;
}

void insert_student_to_list(STUDENT *new_node, STUDENT **head) {
    if ((*head) == NULL) {
        (*head) = new_node;
        return;
    }
    STUDENT *var_head = *head;
    while (var_head->next != NULL) {
        var_head = var_head->next;
    }
    var_head->next = new_node;
}

void insert_instructor_to_list(INSTRUCTOR *new_node, INSTRUCTOR **head) {
    if ((*head) == NULL) {
        (*head) = new_node;
        return;
    }
    INSTRUCTOR *var_head = *head;
    while (var_head->next != NULL) {
        var_head = var_head->next;
    }
    var_head->next = new_node;
}

void insert_course_to_list(COURSE *new_node, COURSE **head) {
    if ((*head) == NULL) {
        (*head) = new_node;
        return;
    }
    COURSE *var_head = *head;
    while (var_head->next != NULL) {
        var_head = var_head->next;
    }
    var_head->next = new_node;
}

void insert_registration_to_list(COURSEREG **head, COURSEREG *new_node) {
    if ((*head) == NULL) {
        (*head) = new_node;
        return;
    }
    COURSEREG *var_head = *head;
    while (var_head->next != NULL) {
        var_head = var_head->next;
    }
    var_head->next = new_node;
}

void print_list_to_file_student(STUDENT **head) {
    FILE *writer = fopen("data/Students.txt", "w");
    STUDENT *tmpHead = *head;
    if (writer == NULL) {
        printf("File could not be opened...\n");
        exit(0);
    }
    while (tmpHead != NULL) {
        fprintf(writer, "%s %s %s %d %d\n", tmpHead->firstName, tmpHead->lastName, tmpHead->studentID, tmpHead->totalCredit, tmpHead->totalCourses);
        tmpHead = tmpHead->next;
    }
    fclose(writer);
    printf("---File Updated---\n");
}

void print_list_to_file_instructor(INSTRUCTOR **head) {
    FILE *writer = fopen("data/Instructors.txt", "w");
    INSTRUCTOR *tmpHead = *head;
    if (writer == NULL) {
        printf("File could not be opened...\n");
        exit(0);
    }
    while (tmpHead != NULL) {
        fprintf(writer, "%s %s %s %s\n", tmpHead->firstName, tmpHead->lastName, tmpHead->id, tmpHead->title);
        tmpHead = tmpHead->next;
    }
    fclose(writer);
    printf("---File Updated---\n");
}

void print_list_to_file_course(COURSE **head) {
    FILE *writer = fopen("data/Courses.txt", "w");
    COURSE *tmpHead = *head;
    if (writer == NULL) {
        printf("File could not be opened...\n");
        exit(0);
    }
    while (tmpHead != NULL) {
        fprintf(writer, "%s %s %s %d %d\n", tmpHead->name, tmpHead->code, tmpHead->instructorID, tmpHead->credit, tmpHead->quota);
        tmpHead = tmpHead->next;
    }
    fclose(writer);
    printf("---File Updated---\n");
}

void print_list_to_file_registration(COURSEREG **head) {
    FILE *writer = fopen("data/StudentCourseRegistrations.txt", "w");
    COURSEREG *tmp = *head;
    if (writer == NULL) {
        printf("File could not be opened...\n");
        exit(0);
    }
    while (tmp != NULL) {
        fprintf(writer, "%d  %s  %s  %s  %d\n", tmp->registrationNo, tmp->date, tmp->studentID, tmp->courseCode, tmp->status);
        tmp = tmp->next;
    }
    fclose(writer);
    printf("---File Updated---\n");
}

int delete_student(char *id, STUDENT **head) {
    STUDENT *tmpHead = *head;
    STUDENT *prev = NULL;
    int found = 0;

    if (tmpHead == NULL) {
        printf("No registered students found, deletion cannot be performed\n");
        return 0;
    }
    if (tmpHead != NULL && strcmp(tmpHead->studentID, id) == 0) {
        *head = tmpHead->next;
        free(tmpHead);
        return 1;
    }
    while (tmpHead != NULL && found == 0) {
        if (strcmp(tmpHead->studentID, id) == 0) {
            found = 1;
            prev->next = tmpHead->next;
            free(tmpHead);
        } else {
            prev = tmpHead;
            tmpHead = tmpHead->next;
        }
    }
    if (found == 0) {
        printf("No student registered with this ID\n");
        return 0;
    } else {
        return 1;
    }
}

int delete_instructor(char *id, INSTRUCTOR **head) {
    INSTRUCTOR *tmpHead = *head;
    INSTRUCTOR *prev = NULL;
    int found = 0;

    if (tmpHead == NULL) {
        printf("No registered instructors found, deletion cannot be performed\n");
        return 0;
    }
    if (tmpHead != NULL && strcmp(tmpHead->id, id) == 0) {
        *head = tmpHead->next;
        free(tmpHead);
        return 1;
    }
    while (tmpHead != NULL && found == 0) {
        if (strcmp(tmpHead->id, id) == 0) {
            found = 1;
            prev->next = tmpHead->next;
            free(tmpHead);
        } else {
            prev = tmpHead;
            tmpHead = tmpHead->next;
        }
    }
    if (found == 0) {
        printf("No instructor registered with this ID\n");
        return 0;
    } else {
        return 1;
    }
}

int delete_course(char *code, COURSE **head) {
    COURSE *tmpHead = *head;
    COURSE *prev = NULL;
    int found = 0;

    if (tmpHead == NULL) {
        printf("No registered courses found, deletion cannot be performed\n");
        return 0;
    }
    if (tmpHead != NULL && strcmp(tmpHead->code, code) == 0) {
        *head = tmpHead->next;
        free(tmpHead);
        return 1;
    }
    while (tmpHead != NULL && found == 0) {
        if (strcmp(tmpHead->code, code) == 0) {
            found = 1;
            prev->next = tmpHead->next;
            free(tmpHead);
        } else {
            prev = tmpHead;
            tmpHead = tmpHead->next;
        }
    }
    if (found == 0) {
        printf("No course registered with this code\n");
        return 0;
    } else {
        return 1;
    }
}

bool studentIDControl(STUDENT **head, char id[]) {
    STUDENT *tmp = *head;
    while (tmp != NULL) {
        if (strcmp(tmp->studentID, id) == 0)
            return true;
        tmp = tmp->next;
    }
    return false;
}

bool instructorIDControl(INSTRUCTOR **head, char id[]) {
    INSTRUCTOR *tmp = *head;
    while (tmp != NULL) {
        if (strcmp(tmp->id, id) == 0)
            return true;
        tmp = tmp->next;
    }
    return false;
}

bool courseControl(COURSE **head, char code[]) {
    COURSE *tmp = *head;
    while (tmp != NULL) {
        if (strcmp(tmp->code, code) == 0)
            return true;
        tmp = tmp->next;
    }
    return false;
}

bool update_course(char code[], COURSE **head, char newInstructorID[]) {
    COURSE *tmp = *head;
    while (tmp != NULL) {
        if (strcmp(tmp->code, code) == 0) {
            strcpy(tmp->instructorID, newInstructorID);
            return true;
        }
        tmp = tmp->next;
    }
    printf("No course found with this code, the operation could not be completed\n");
    return false;
}

bool update_title(char id[], INSTRUCTOR **head, char newTitle[]) {
    INSTRUCTOR *tmp = *head;
    while (tmp != NULL) {
        if (strcmp(tmp->id, id) == 0) {
            strcpy(tmp->title, newTitle);
            return true;
        }
        tmp = tmp->next;
    }
    printf("No instructor found with this ID, the operation could not be completed\n");
    return false;
}

void view_courses_by_instructor(COURSE **head, char id[]) {
    COURSE *tmp = *head;
    int count = 0;
    if (tmp == NULL) {
        printf("No courses found in the course list\n");
        return;
    }
    while (tmp != NULL) {
        if (strcmp(tmp->instructorID, id) == 0) {
            count++;
            printf("%s-->%s\n", tmp->code, tmp->name);
        }
        tmp = tmp->next;
    }
    if (count == 0) {
        printf("No courses assigned to the instructor with ID %s\n\n", id);
    }
}

int check_course_quota(COURSE **head, char courseCode[]) {
    COURSE *tmp = *head;
    while (tmp != NULL) {
        if (strcmp(tmp->code, courseCode) == 0) {
            if (tmp->quota > 0) {
                return tmp->credit;
            } else {
                printf("Course quota is full\n");
                return 0;
            }
        }
        tmp = tmp->next;
    }
    printf("No course found with this code\n\n");
    return 0;
}

int check_student_credit(STUDENT **head, char studentID[]) {
    STUDENT *tmp = *head;
    while (tmp != NULL) {
        if (strcmp(tmp->studentID, studentID) == 0) {
            return tmp->totalCredit;
        }
        tmp = tmp->next;
    }
    printf("No student found with this ID\n\n");
    return 0;
}

bool has_taken_before(COURSEREG **head, char studentID[], char courseCode[]) {
    COURSEREG *tmp = *head;
    while (tmp != NULL) {
        if (strcmp(tmp->studentID, studentID) == 0 && strcmp(tmp->courseCode, courseCode) == 0) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

void update_course_quota(COURSE **head, char courseCode[]) {
    COURSE *tmp = *head;
    while (tmp != NULL) {
        if (strcmp(tmp->code, courseCode) == 0) {
            tmp->quota--;
            return;
        }
        tmp = tmp->next;
    }
}

void update_student_credit(STUDENT **head, char studentID[], int credit) {
    STUDENT *tmp = *head;
    while (tmp != NULL) {
        if (strcmp(tmp->studentID, studentID) == 0) {
            tmp->totalCredit += credit;
            return;
        }
        tmp = tmp->next;
    }
}

int delete_course_registration(int registrationNo, COURSEREG **head) {
    COURSEREG *tmp = *head, *prev = NULL;
    while (tmp != NULL && tmp->registrationNo != registrationNo) {
        prev = tmp;
        tmp = tmp->next;
    }
    if (tmp == NULL) {
        printf("No registration found with this number.\n");
        return 0;
    }
    if (prev == NULL) {
        *head = tmp->next;
    } else {
        prev->next = tmp->next;
    }
    free(tmp);
    return 1;
}

int update_course_registration(int registrationNo, COURSEREG **head, char newCourseCode[], char newStudentID[]) {
    COURSEREG *tmp = *head;
    while (tmp != NULL) {
        if (tmp->registrationNo == registrationNo) {
            strcpy(tmp->courseCode, newCourseCode);
            strcpy(tmp->studentID, newStudentID);
            return 1;
        }
        tmp = tmp->next;
    }
    printf("No registration found with this number.\n");
    return 0;
}

void save_class_list_to_file(COURSEREG **regHead, char courseCode[], char fileName[]) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("File could not be opened.\n");
        return;
    }
    COURSEREG *tmp = *regHead;
    while (tmp != NULL) {
        if (strcmp(tmp->courseCode, courseCode) == 0) {
            fprintf(file, "Student ID: %s, Registration No: %d, Date: %s, Status: %d\n", tmp->studentID, tmp->registrationNo, tmp->date, tmp->status);
        }
        tmp = tmp->next;
    }
    fclose(file);
    printf("Class list saved to %s.\n", fileName);
}
