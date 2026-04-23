#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;


//Course Structure
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;

    Course() {
        courseNumber = "";
        title = "";
    }
};

//Function 
void loadCourses(vector<Course>& courses);
void printCourseList(const vector<Course>& courses);
void printCourse(const vector<Course>& courses, const string& courseNumber);
string trim(const string& str);
string toUpperCase(const string& str);
vector<string> parseCSVLine(const string& line);
bool compareByCourseNumber(const Course& a, const Course& b);

//Main Function
int main() {
    vector<Course> courses;
    bool dataLoaded = false;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        //Display menu
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        //Clear input
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            //Load data structure
            loadCourses(courses);
            dataLoaded = true;
            break;
        }

        case 2: {
            //Print course list
            if (!dataLoaded) {
                cout << "Please load data structure first." << endl;
            }
            else {
                printCourseList(courses);
            }
            break;
        }

        case 3: {
            //Print course
            if (!dataLoaded) {
                cout << "Please load data structure first." << endl;
            }
            else {
                string courseNumber;
                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);
                printCourse(courses, courseNumber);
            }
            break;
        }

        case 9: {
            //Exit
            cout << "Thank you for using the course planner!" << endl;
            break;
        }

        default: {
            cout << choice << " is not a valid option." << endl;
            break;
        }
        }
        cout << endl;
    }

    return 0;
}

//Load Courses
void loadCourses(vector<Course>& courses) {
    //Clear existing courses
    courses.clear();

    //Open the file
    ifstream file("CS_300_ABCU_Advising_Program_Input.csv");

    if (!file.is_open()) {
        cout << "Error: Could not open file "  << endl;
        return;
    }

    string line;
    int coursesLoaded = 0;
    int lineNumber = 0;

    // Read file line by line
    while (getline(file, line)) {
        lineNumber++;

        // Skip empty lines
        if (trim(line).empty()) {
            continue;
        }

        // Parse CSV line
        vector<string> fields = parseCSVLine(line);

        // Need at least course number and title
        if (fields.size() < 2) {
            continue;
        }

        // Create course object
        Course newCourse;
        newCourse.courseNumber = trim(fields[0]);
        newCourse.title = trim(fields[1]);

        // Skip if course number is empty
        if (newCourse.courseNumber.empty()) {
            continue;
        }

        // Add prerequisites (fields 2 and beyond)
        for (size_t i = 2; i < fields.size(); i++) {
            string prereq = trim(fields[i]);
            if (!prereq.empty()) {
                newCourse.prerequisites.push_back(prereq);
            }
        }

        // Add to vector
        courses.push_back(newCourse);
        coursesLoaded++;
    }

    file.close();
    cout << "Data loaded successfully. " << coursesLoaded << " courses read." << endl;
}

//Displays all courses sorted alphanumerically by course number
void printCourseList(const vector<Course>& courses) {
    //Create a copy for sorting
    vector<Course> sortedCourses = courses;

    //Sort alphanumerically by course number
    sort(sortedCourses.begin(), sortedCourses.end(), compareByCourseNumber);

    //Print header
    cout << "Here is a sample schedule:" << endl;

    //Print each course
    for (const Course& course : sortedCourses) {
        cout << course.courseNumber << ", " << course.title << endl;
    }
}

//Case insensitive search
void printCourse(const vector<Course>& courses, const string& courseNumber) {
    string searchNumber = toUpperCase(courseNumber);

    //Search for the course
    for (const Course& course : courses) {
        if (toUpperCase(course.courseNumber) == searchNumber) {
            //Print course info
            cout << course.courseNumber << ", " << course.title << endl;

            //Print prerequisites
            if (!course.prerequisites.empty()) {
                cout << "Prerequisites: ";
                for (size_t i = 0; i < course.prerequisites.size(); i++) {
                    cout << course.prerequisites[i];
                    if (i < course.prerequisites.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << endl;
            }
            return;
        }
    }

    //Course not found
    cout << "Course not found." << endl;
}

//Parse CSV Line
vector<string> parseCSVLine(const string& line) {
    vector<string> fields;
    string field;
    istringstream stream(line);

    while (getline(stream, field, ',')) {
        fields.push_back(field);
    }

    return fields;
}

//Removes leading and trailing whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

//Converts string to uppercase for comparison
string toUpperCase(const string& str) {
    string result = str;
    for (char& c : result) {
        c = toupper(c);
    }
    return result;
}

//Comparison function for alphanumeric sorting
bool compareByCourseNumber(const Course& a, const Course& b) {
    return a.courseNumber < b.courseNumber;
}