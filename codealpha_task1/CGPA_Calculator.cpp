#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

struct Course {
    string name;
    double gradePoint;
    double creditHours;
};

int main() {
    int n;

    cout << "===== CGPA CALCULATOR =====\n";
    cout << "Enter number of courses: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of courses.\n";
        return 0;
    }

    vector<Course> courses(n);
    double totalCredits = 0, totalPoints = 0;

    for (int i = 0; i < n; i++) {
        cout << "\nCourse " << i + 1 << " name: ";
        cin >> ws;
        getline(cin, courses[i].name);

        cout << "Grade point (0-4): ";
        cin >> courses[i].gradePoint;

        cout << "Credit hours: ";
        cin >> courses[i].creditHours;

        if (courses[i].gradePoint < 0 ||
            courses[i].gradePoint > 4 ||
            courses[i].creditHours <= 0) {
            cout << "Invalid input.\n";
            return 0;
        }

        totalCredits += courses[i].creditHours;
        totalPoints += courses[i].gradePoint *
                       courses[i].creditHours;
    }

    double previousCredits, previousPoints;

    cout << "\nPrevious total credit hours (0 if none): ";
    cin >> previousCredits;

    cout << "Previous total quality points (0 if none): ";
    cin >> previousPoints;

    double gpa = totalPoints / totalCredits;

    double cgpa = (totalPoints + previousPoints) /
                  (totalCredits + previousCredits);

    cout << "\n===== RESULT =====\n";

    cout << fixed << setprecision(2);

    cout << left << setw(25) << "Course"
         << setw(15) << "Grade Point"
         << "Credit Hours\n";

    for (const Course &c : courses) {
        cout << left << setw(25) << c.name
             << setw(15) << c.gradePoint
             << c.creditHours << '\n';
    }

    cout << "\nSemester GPA: " << gpa;
    cout << "\nOverall CGPA: " << cgpa << endl;

    return 0;
}