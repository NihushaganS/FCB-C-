#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Student {
    std::string stream;
    std::string className;
    std::string name;
    int biology;
    int physics;
    int chemistry;
    int totalMarks;
    int schoolRank;
    int classRank;
};


int calculateTotalMarks(int bio, int phy, int chem) {
    return bio + phy + chem;
}


std::vector<Student> readStudentData(const std::string& fileName) {
    std::ifstream file(fileName);
    std::vector<Student> students;

    if (file.is_open()) {
        Student student;
        while (file >> student.stream >> student.className >> student.name >>
               student.biology >> student.physics >> student.chemistry) {
            student.totalMarks = calculateTotalMarks(student.biology, student.physics, student.chemistry);
            students.push_back(student);
        }
        file.close();
    }

    return students;
}


void calculateRanks(std::vector<Student>& students) {

    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.totalMarks > b.totalMarks;
    });


    for (size_t i = 0; i < students.size(); ++i) {
        students[i].schoolRank = static_cast<int>(i) + 1;
    }

    std::vector<Student>::iterator classStart = students.begin();
    while (classStart != students.end()) {
        std::vector<Student>::iterator classEnd = std::find_if(classStart, students.end(),
                                                               [classStart](const Student& student) {
                                                                   return student.className != classStart->className;
                                                               });


        std::sort(classStart, classEnd, [](const Student& a, const Student& b) {
            return a.totalMarks > b.totalMarks;
        });

        // Assign class ranks
        for (size_t i = 0; i < static_cast<size_t>(classEnd - classStart); ++i) {
            (classStart + i)->classRank = static_cast<int>(i) + 1;
        }

        classStart = classEnd;
    }
}


void displayAndWriteRanks(const std::vector<Student>& students, const std::string& outputFileName) {
    std::ofstream outputFile(outputFileName);

    if (outputFile.is_open()) {
        for (const auto& student : students) {
            std::cout << "Name: " << student.name << "\tStream: " << student.stream
                      << "\tClass: " << student.className << "\tTotal Marks: " << student.totalMarks
                      << "\tSchool Rank: " << student.schoolRank << "\tClass Rank: " << student.classRank << std::endl;


            outputFile << student.name << "\t" << student.stream << "\t" << student.className << "\t"
                        << student.totalMarks << "\t" << student.schoolRank << "\t" << student.classRank << std::endl;
        }
        outputFile.close();
    } else {
        std::cerr << "Unable to open the output file." << std::endl;
    }
}

int main() {

    std::vector<Student> students = readStudentData("student_data.txt");


    calculateRanks(students);


    displayAndWriteRanks(students, "rank_output.txt");

    return 0;
}
