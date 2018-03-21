#include "Timer.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>

using namespace std;

// Used for debugging purpose only
void printString(string str){
	cout << str << endl;
}

// Used for debugging purpose only
void printdpVector(vector < vector <int> > dp){
	for(int r = 0; r < dp.size(); r++){
		for(int c = 0; c < dp[0].size(); c++){
			cout << dp[r][c] << " ";
		}
		cout << endl;
	}
}

// LCS_Algorithm discussed in the textbook
int LCS_Algorithm_1(string str1, string str2){
	// String lengths
	int n = str1.size();
	int m = str2.size();

	// 2-D dp subproblem matrix with one extra row and column for base case
	vector < vector<int> > dp (n+1, vector<int> (m+1));

	for(int i = 0; i <= n; i++){
		for(int j = 0; j <= m; j++){
			if(i == 0 || j == 0)
				dp[i][j] = 0;
			else if(str1[i-1] == str2[j-1])
				dp[i][j] = dp[i-1][j-1] + 1;
			else
				dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
		}
	}

	// Desired result stored at the lowest right-corner of the matrix
	return dp[n][m];
}

// Linear Space Modified LCS Algorithm
// LCS_Algorithm_2 utilizing Linear space by keeping only two rows of dp matrix at an instant
int LCS_Algorithm_2(string str1, string str2){
	// String lengths
	int n = str1.size();
	int m = str2.size();

	// 2-D dp subproblem matrix with  only two rows
	vector < vector<int> > dp (2, vector<int> (m+1));

	bool toggle = true;
	for(int i = 0; i <= n; i++){
		// Getting row index by toggling 0 and 1 everytime
		toggle = !toggle;
		for(int j = 0; j <= m; j++){
			if(i == 0 || j == 0)
				dp[toggle][j] = 0;
			else if(str1[i-1] == str2[j-1])
				dp[toggle][j] = dp[!toggle][j-1] + 1;
			else
				dp[toggle][j] = max(dp[!toggle][j], dp[toggle][j-1]);
		}
	}

	// Desired result stored at the lowest right-corner of the matrix
	return dp[toggle][m];
}

// Generate and return Random String of Given Length
string generateRandomString(int length){
	string result;
	for(int i = 0; i < length; i++){
		char ch = 65 + (rand() % 26);
		result.push_back(ch);
	}
	return result;
}

// Calculate and return sampled user_time value for both the algorithms on the basis of the flag
// if boolean flag is true, run normal LCS algorithm, else run Linear Space Modified LCS Algorithm
double getUserTime(string str1, string str2, bool LCS_1){
	double user_time = 0.0;
	// Sampling 30 times for the accurate readings for LCS_Algorithm_1
	for(int x = 0; x < 50; x++){
		Timer t;
		double u_time;

		if(LCS_1){
			// Run Normal LCS algorithm
			t.start();
			LCS_Algorithm_1(str1, str2);
			t.elapsedUserTime(u_time);
		}
		else{
			// Run Linear Space modified LCS algorithm
			t.start();
			LCS_Algorithm_2(str1, str2);
			t.elapsedUserTime(u_time);
		}
		user_time += u_time;
	}
	// Taking average of 30 readings
	user_time /= 50.0;
	// Convert in milliseconds
	user_time *= 1000.0;
	return user_time;
}

// Write size and running times into the file
void writeIntoFile(vector< vector<double> > output){
	ofstream my_file;
	my_file.open ("LCS_Algorithm_Output.txt", ios::out | ios::app);
	my_file << "String Size,LCS_Algorithm,LCS_Linear_Space_Algorithm" << endl;
	// write Into file
	for(int i = 0; i < output.size(); i++){
		for(int j = 0; j < output[0].size(); j++){
			my_file << output[i][j];
			if(j != output[0].size()-1)
				my_file << ",";
		}
		my_file << endl;
	}

	my_file.close();
}

int main() {

	string str1, str2;
	vector< vector<double> > output;

	// On the basis of size i, get desired running times of both the algorithms into the output file
	for(int i = 100; i < 30001; i = i + 100){

		str1 = generateRandomString(i);
		str2 = generateRandomString(i);

		vector <double> user_time_vector;
		// Push size of the vector
		user_time_vector.push_back(i);
		// Getting time of LCS_Algorithm_1
		user_time_vector.push_back(getUserTime(str1, str2, true));
		// Getting time of LCS_Algorithm_2
		user_time_vector.push_back(getUserTime(str1, str2, false));
		output.push_back(user_time_vector);
	}

	writeIntoFile(output);
	return 0;
}
