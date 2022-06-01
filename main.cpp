#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

double calcAverage(int examNumber, int num_students, int** array);
char calcExamGrade(int testScore, int i, double* array);
char calcFinalGrade(int finalGrade, double classAverage);

int main(int argc, char* argv[]) {
	VS_MEM_CHECK              

	if (argc < 3) {
		cerr << "Please provide name of input and output files";
		return 1;
	}
	cout << "Input file: " << argv[1] << endl;
	ifstream in(argv[1]);
	if (!in) {
		cerr << "Unable to open " << argv[1] << " for input";
		return 2;
	}
	cout << "Output file: " << argv[2] << endl;
	ofstream out(argv[2]);
	if (!out) {
		in.close();
		cerr << "Unable to open " << argv[2] << " for output";
		return 3;
	}

	int num_students;
	int num_exams;
	in >> num_students >> num_exams;
	in.ignore(numeric_limits<int>::max(), '\n');

	string* studentNames = new string[num_students];
	string* studentAnswersLine = new string[num_students];

	for (int i = 0; i < num_students; ++i) {
		string line;
		getline(in, line);
		size_t p = 0;
		while (!isdigit(line[p])) ++p;
		studentNames[i] = line.substr(0, p);
		studentAnswersLine[i] = line.substr(p, line.length() - p);
	}

	int** studentAnswers = new int* [num_students];
	for (int i = 0; i < num_students; ++i) {
		studentAnswers[i] = new int[num_exams];
	}
	
	for (int i = 0; i < num_students; ++i) {
		istringstream iss(studentAnswersLine[i]);
		for (int j = 0; j < num_exams; ++j) {
			iss >> studentAnswers[i][j];
		}
	}
	
	out << "Student Scores:" << endl;
	for (int i = 0; i < num_students; ++i) {
		out << fixed << setw(20) << studentNames[i] << " ";
		for (int j = 0; j < num_exams; ++j) {
			out << fixed << setprecision(0) << setw(6) << studentAnswers[i][j];
		}
		out << endl;
	}

	double* testAverages = new double[num_exams];
	out << "Exam Averages:" << endl;
	for (int i = 0; i < num_exams; ++i) {
		out << fixed << setw(8) << "Exam " << i + 1 << " Average = ";
		out << fixed << setprecision(1) << setw(7) << calcAverage(i, num_students, studentAnswers);
		testAverages[i] = calcAverage(i, num_students, studentAnswers);
		out << endl;
	}

	char** gradeCounter = new char* [num_students];
	for (int i = 0; i < num_students; ++i) {
		gradeCounter[i] = new char[num_exams];
	}
	out << "Student Exam Grades:" << endl;
	for (int i = 0; i < num_students; ++i) {
		out << fixed << setw(20) << studentNames[i] << " ";
		for (int j = 0; j < num_exams; ++j) {
			out << fixed << setprecision(0) << setw(5) << studentAnswers[i][j] << "(" << calcExamGrade(studentAnswers[i][j], j, testAverages) << ")";
			gradeCounter[i][j] = calcExamGrade(studentAnswers[i][j], j, testAverages);
		}
		out << endl;
	}

	out << "Exam Grades:" << endl;
	int counterA = 0;
	int counterB = 0;
	int counterC = 0;
	int counterD = 0;
	int counterE = 0;
	char studentGrade;
	for (int i = 0; i < num_exams; ++i) {
		for (int j = 0; j < num_students; ++j) {
			studentGrade = gradeCounter[j][i];
			if (studentGrade == 'A') {
				++counterA;
			}
			else if (studentGrade == 'B') {
				++counterB;
			}
			else if (studentGrade == 'C') {
				++counterC;
			}
			else if (studentGrade == 'D') {
				++counterD;
			}
			else if (studentGrade == 'E') {
				++counterE;
			}
		}
		out << fixed << setw(8) << "Exam " << i + 1;
		out << fixed << setprecision(0) << setw(6) << counterA << "(A)";
		out << fixed << setprecision(0) << setw(6) << counterB << "(B)";
		out << fixed << setprecision(0) << setw(6) << counterC << "(C)";
		out << fixed << setprecision(0) << setw(6) << counterD << "(D)";
		out << fixed << setprecision(0) << setw(6) << counterE << "(E)";
		out << endl;
		counterA = 0;
		counterB = 0;
		counterC = 0;
		counterD = 0;
		counterE = 0;
	}

	double classAverage = 0.0;
	int sum = 0;
	for (int i = 0; i < num_students; ++i) {
		for (int j = 0; j < num_exams; ++j) {
			sum += studentAnswers[i][j];
		}
	}
	classAverage = sum / static_cast<double>(num_students * num_exams);

	out << "Student Final Grades:" << endl;
	double studentAverage = 0.0;
	double* finalAverages = new double[num_students];
	int sumAnswer = 0;
	for (int i = 0; i < num_students; ++i) {
		for (int j = 0; j < num_exams; ++j) {
			sumAnswer += studentAnswers[i][j]; 
		}
		studentAverage = static_cast<double>(sumAnswer) / num_exams;
		finalAverages[i] = studentAverage;
		sumAnswer = 0;
	}
	for (int i = 0; i < num_students; ++i) {
		out << fixed << setw(20) << studentNames[i];
		out << fixed << setprecision(1) << setw(6) << finalAverages[i] << "(" << calcFinalGrade(finalAverages[i], classAverage) << ")";
		out << endl;
	}

	out << fixed << setprecision(1) << "Class Average Score = " << classAverage << endl;

	delete[] studentNames;
	delete[] studentAnswersLine;
	delete[] testAverages;
	delete[] finalAverages;
	for (int i = 0; i < num_students; ++i)
	{
		delete[] studentAnswers[i];
	}
	delete[] studentAnswers;
	for (int i = 0; i < num_students; ++i) {
		delete[] gradeCounter[i];
	}
	delete[] gradeCounter;

	return 0;
}

double calcAverage(int examNumber, int num_students, int **array) {
	double examAverage = 0.0;
	int sum = 0;
	for (int i = 0; i < num_students; ++i) {
		sum += array[i][examNumber];
	}
	examAverage = sum / static_cast<double>(num_students);
	return examAverage;
}

char calcExamGrade(int testScore, int i, double* array) {
	char gradeA = 'A';
	char gradeB = 'B';
	char gradeC = 'C';
	char gradeD = 'D';
	char gradeE = 'E';
	if (static_cast<double>(testScore) - array[i] <= 5 && static_cast<double>(testScore) - array[i] >= -5) {
		return gradeC;
	}
	else if (static_cast<double>(testScore) - array[i] > 5 && static_cast<double>(testScore) - array[i] <= 15) {
		return gradeB;
	}
	else if (static_cast<double>(testScore) - array[i] < -5 && static_cast<double>(testScore) - array[i] >= -15) {
		return gradeD;
	}
	else if (static_cast<double>(testScore) - array[i] > 15) {
		return gradeA;
	}
	else if (static_cast<double>(testScore) - array[i] < -15) {
		return gradeE;
	}
}

char calcFinalGrade(int finalGrade, double classAverage) {
	char gradeA = 'A';
	char gradeB = 'B';
	char gradeC = 'C';
	char gradeD = 'D';
	char gradeE = 'E';
	if (finalGrade - classAverage <= 5 && finalGrade - classAverage >= -5) {
		return gradeC;
	}
	else if (finalGrade - classAverage > 5 && finalGrade - classAverage <= 15) {
		return gradeB;
	}
	else if (finalGrade - classAverage < -5 && finalGrade - classAverage >= -15) {
		return gradeD;
	}
	else if (finalGrade - classAverage > 15) {
		return gradeA;
	}
	else if (finalGrade - classAverage < -15) {
		return gradeE;
	}
}
