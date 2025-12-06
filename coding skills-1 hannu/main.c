#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentUser[20];
char currentRole[15];
int loggedStudentRoll = -1;

// Function prototypes
int loginSystem();
void mainMenu();
void adminMenu();
void staffMenu();
void guestMenu();
void studentMenu();

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void displayOwnRecord(int roll);

// ======================= MAIN =======================
int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nLogin failed, exiting...\n");
    }
    return 0;
}

// ======================= LOGIN SYSTEM =======================
int loginSystem() {
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[15];

    printf("======== LOGIN SCREEN ========\n");
    printf("Username: ");
    scanf("%19s", username);

    printf("Password: ");
    scanf("%19s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: credentials.txt not found!\n");
        return 0;
    }

    // Read credentials line by line
    while (fscanf(fp, "%19s %19s %14s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 &&
            strcmp(password, filePass) == 0) {

            // Save login info
            strcpy(currentUser, fileUser);
            strcpy(currentRole, fileRole);

            // If STUDENT → extract roll number (student1 -> 1)
            if (strcmp(fileRole, "STUDENT") == 0) {
                if (sscanf(fileUser, "student%d", &loggedStudentRoll) != 1)
                    loggedStudentRoll = -1;
            }

            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// ======================= MAIN MENU ROUTER =======================
void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else if (strcmp(currentRole, "GUEST") == 0)
        guestMenu();
    else if (strcmp(currentRole, "STUDENT") == 0)
        studentMenu();
}

// ======================= ADMIN MENU (FULL CRUD) =======================
void adminMenu() {
    int choice;
    while (1) {
        printf("\n========= ADMIN MENU =========\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// ======================= STAFF MENU (Display + Search) =======================
void staffMenu() {
    int choice;
    while (1) {
        printf("\n========= STAFF MENU =========\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// ======================= GUEST MENU (Display only) =======================
void guestMenu() {
    int choice;
    while (1) {
        printf("\n========= GUEST MENU =========\n");
        printf("1. Display Students\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// ======================= STUDENT MENU (Own record only) =======================
void studentMenu() {
    int choice;
    while (1) {
        printf("\n========= STUDENT MENU =========\n");
        printf("1. View My Record\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayOwnRecord(loggedStudentRoll); break;
            case 2: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// ======================= ADD STUDENT =======================
void addStudent() {
    FILE *fp = fopen(STUDENT_FILE, "a");
    struct Student st;

    if (!fp) {
        printf("Error opening student file!\n");
        return;
    }

    printf("Enter Roll: ");
    scanf("%d", &st.roll);
    printf("Enter Name: ");
    scanf("%49s", st.name);
    printf("Enter Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);

    printf("Student added successfully!\n");
}

// ======================= DISPLAY STUDENTS =======================
void displayStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;

    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    printf("\nROLL\tNAME\tMARKS\n");
    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        printf("%d\t%s\t%.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
}

// ======================= SEARCH STUDENT =======================
void searchStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;
    int roll, found = 0;

    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    printf("Enter roll to search: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("\nRecord Found:\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", st.roll, st.name, st.marks);
            found = 1;
        }
    }

    if (!found)
        printf("Record not found.\n");

    fclose(fp);
}

// ======================= UPDATE STUDENT =======================
void updateStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student st;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("File error!\n");
        return;
    }

    printf("Enter roll to update: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("Enter New Name: ");
            scanf("%49s", st.name);
            printf("Enter New Marks: ");
            scanf("%f", &st.marks);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    printf(found ? "Record updated!\n" : "Record not found!\n");
}

// ======================= DELETE STUDENT =======================
void deleteStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student st;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("File error!\n");
        return;
    }

    printf("Enter roll to delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    printf(found ? "Record deleted!\n" : "Record not found!\n");
}

// ======================= DISPLAY OWN RECORD =======================
void displayOwnRecord(int roll) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;

    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("\n=== My Record ===\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", st.roll, st.name, st.marks);
            fclose(fp);
            return;
        }
    }

    printf("Your record is not found!\n");
    fclose(fp);
}
