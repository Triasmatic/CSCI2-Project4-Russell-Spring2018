#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
using namespace std;

//MAJOR NOTE: Please do not replace my main as my program can only work if you use the main as it is. Instead i have
//			  added user input so that it works simmilarly to the one given in the project spec!


void readFile(vector<string> &, string);
void checkSyntax(vector<string> &);
void errorFunc(char, int);
void errorFuncAux(string, int);
void exitCmd() {
	system("pause");
	exit(1);
}

int main() {

	vector<string> linesOfCode;
	
	string fileName;

	cout << "Please enter the name of the file (only the file name. Do not include .txt at the end): ";
	cin >> fileName;

	cout << endl;


	//string linesOfCode[MAX_FILESIZE];
	//int sizeOfVector = linesOfCode.size();

	readFile(linesOfCode, fileName);

	for (size_t i = 0; i < linesOfCode.size(); i++) {
		cout << i+1 << ". " << linesOfCode[i] << endl;
	}

	checkSyntax(linesOfCode);

	system("pause");
}

void readFile(vector<string> &strArray, string fn) {
	ifstream infile;
	infile.open(fn + ".txt");
	try {
		if (!infile)
			throw string("404 FILE_NOT_FOUND");
	}
	catch (string x) {
		cout << "File cannot be read because no file found! ERROR EXCEPTION: " << x << endl;
		exitCmd();
	}

	string str;
	while (getline(infile, str)) {
		if (str.size() > 0)//removes blank spaces cause who needs more edge cases amirite???
			strArray.push_back(str);
	}
}

void checkSyntax(vector<string> &strVector) {
	stack<char> st;
	stack<char> sqt;
	stack<int> numSt;
	int errorNum;
	size_t counter = 0;
	char c;
	while (counter < strVector.size()) {
		for (size_t i = 0; i < strVector[counter].size(); ++i) {
			
			c = (strVector[counter])[i]; // gets the char at the current location.

			if (!sqt.empty()) {
				if (sqt.top() == '\'' && c != '\'') {
					continue;
				}

				if (sqt.top() == '\"' && c != '\"') {
					continue;
				}

				if (sqt.top() == '*' && c != '*') {
					continue;
				}
				else
					if (sqt.top() == '*' && c == '*' && (strVector[counter])[i + 1] == '/') {
						cout << "found matching pair: /* and */" << endl;
						sqt.pop();
						numSt.pop();
						continue;
					}
			}

			if (c == '*' && strVector[counter][i + 1] != '/' || c == '*' && strVector[counter][i - 1] != '/')
				continue;

			if ((c == '*' && sqt.empty()) || (c == '*' && sqt.top() != '*' && (strVector[counter])[i+1] == '/')) {
				errorFuncAux("*/", counter+1);
			}

			if (c == '/') { //check for /
				if ((strVector[counter])[i + 1] == '/')
					break;
				else
					if ((strVector[counter])[i + 1] == '*') {
						sqt.push('*');
						numSt.push(counter);
						continue;
					}
			}

			switch (c) { //switch statment that checks all the characters
			case '[':
				st.push(c);
				numSt.push(counter);
				break;
			case '{':
				st.push(c);
				numSt.push(counter);
				break;
			case '(':
				st.push(c);
				numSt.push(counter);
				break;
			case '\'': // checks for '
				
				if (sqt.empty()) {
					sqt.push(c);
					numSt.push(counter);
					break;
				}
				if (sqt.top() == '\'') {
					cout << "Matching pair found: " << sqt.top() << " and " << c << endl;
					sqt.pop();
					numSt.pop();
					break;
				}else
				sqt.push(c);
				numSt.push(counter);
				break;
			case '\"': // checks for "
				if (sqt.empty()) {
					sqt.push(c);
					numSt.push(counter);
					break;
				}
				if (sqt.top() == '\"') {
					cout << "Matching pair found: " << sqt.top() << " and " << c << endl;
					sqt.pop();
					numSt.pop();
					break;
				}
				else
				sqt.push(c);
				numSt.push(counter);
				break;
			case ']'://working
				if (st.empty()) {
					errorFunc(c, counter + 1);
					break;
				}
				if (st.top() == '[') {
					cout << "Matching pair found: " << st.top() << " and " << c << endl;
					st.pop();
					numSt.pop();
					break;
				}
				else {
					//get the top num for counter here.
					errorNum = numSt.top() + 1;
					errorFunc(c, errorNum);
					break;
				}
				break;
			case '}'://working
				if (st.empty()) {
					errorFunc(c, counter + 1);
					break;
				}
				if (st.top() == '{') {
					cout << "Matching pair found: " << st.top() << " and " << c << endl;
					st.pop();
					numSt.pop();
					break;
				}
				else {
					errorNum = numSt.top() + 1;
					errorFunc(c, errorNum);
					break;
				}
				break;
			case ')'://working
				if (st.empty()) {
					errorFunc(c, counter + 1);
					break;
				}
				if (st.top() == '(') {
					cout << "Matching pair found: " << st.top() << " and " << c << endl;
					st.pop();
					numSt.pop();
					break;
				}
				else {
					errorNum = numSt.top() + 1;
					errorFunc(c, errorNum);
					break;
				}
				break;
			default:
				break;
			}
		}
		counter++;
	}
	if (!st.empty()) { //checks if there is anything in the brace stack. 
		errorNum = numSt.top() + 1;
		errorFunc(st.top(), errorNum);
	}

	if (!sqt.empty()) {
		if (sqt.top() == '*') {
			errorFuncAux("/*", numSt.top());
		}
	}

	if (st.empty() && sqt.empty() && numSt.empty()) {
		cout << "OK balance!" << endl;
	}
}

void errorFunc(char chr, int loc) {
	cout << "Unmatched symbol: " << chr << " on line: " << loc << endl;
	exitCmd();

}

void errorFuncAux(string strChr, int loc) {
	cout << "Unmatched symbol: " << strChr << " on line: " << loc << endl;
	exitCmd();
}