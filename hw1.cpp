#include <iostream>
#include <vector>
#include <string>
#include <cmath>


using namespace std;


int Variable_number, Minterm_number, DontCare_number;
vector<int> DontCares, Minterms, OneCounter, PIChosen;
vector<string> Allterms;
vector<vector<int> > PIChart;


string dec_to_bin(int input);
int difference(int a, int b);
bool operator==(const string &a, const string &b);
bool contain(int allterms_index, int minterm_index);
void pickup1(void);
void pickup2(void);

int main(void)
{
	ios::sync_with_stdio(0);
	cin.tie(0);

	int tmp_input;


	/*****************************************
	 ***   Collect necessary information   ***
	 *****************************************/

	cout << "\nPlease enter Variable_number :\n";
	cin >> Variable_number;

	cout << "\nPlease enter the number of Minterms :\n";
	cin >> Minterm_number;

	cout << "\nPlease enter Minterms :\n";
	for (int i = 0; i < Minterm_number; i++) {
		cin >> tmp_input;
		if (tmp_input >= pow(2, Variable_number)) {
			fprintf(stderr, "\nevery term should not be greater than %d\n", (int)pow(2, Variable_number) - 1);
			return 0;
		}
		Minterms.push_back(tmp_input);
	}
	for (int i = 0; i < Minterm_number; i++) {
		Allterms.push_back(dec_to_bin(Minterms[i]));
	}

	cout << "\nPlease enter the number of Don't-Care terms :\n";
	cin >> DontCare_number;
	if (DontCare_number > 0) {
		cout << "\nPlease enter the Don't Care terms :\n";
		for (int i = 0; i < DontCare_number; i++) {
			cin >> tmp_input;
			if (tmp_input >= pow(2, Variable_number)) {
				fprintf(stderr, "\nevery term should not be greater than %d\n", (int)pow(2, Variable_number) - 1);
				return 0;
			}
			DontCares.push_back(tmp_input);
		}
		for (int i = 0; i < DontCare_number; i++) {
			Allterms.push_back(dec_to_bin(DontCares[i]));
		}
	}


	/********************
	 ***   grouping   ***
	 ********************/


	int index = 0;

	for (int i = 0; i < Variable_number; i++) {
		int allterms_end = Allterms.size();
		for (int j = index; j < allterms_end; j++) {
			int onecounter_end = OneCounter.size();
			for (int k = index; k < onecounter_end; k++) {
				if (j != k && OneCounter[j] + 1 == OneCounter[k]) {
					int pos = difference(j, k);
					if (pos < Variable_number) {
						Allterms[j][Variable_number] = '0';
						Allterms[k][Variable_number] = '0';

						string tmp;
						for (int l = 0; l < Variable_number; l++) {
							if (l == pos) {
								tmp += '-';
							}
							else {
								if (Allterms[j][l] == '0') {
									tmp += '0';
								}
								else if (Allterms[j][l] == '1') {
									tmp += '1';
								}
								else {
									tmp += '-';
								}
							}
						}
						tmp += '1';

						bool dup = false;
						for (int l = 0; l < Allterms.size(); l++) {
							if (Allterms[l] == tmp) {
								dup = true;
								break;
							}
						}

						if (!dup) {
							Allterms.push_back(tmp);
							OneCounter.push_back(OneCounter[j]);
						}
					}
				}
			}
		}
		index = allterms_end;
	}


	/********************
	 ***   PI chart   ***
	 ********************/

	for (int i = 0; i < Allterms.size(); i++) {
		if (Allterms[i][Variable_number] == '1') {
			vector<int> arr;
			for (int j = 0; j < Minterm_number; j++) {
				if (contain(i, j)) {
					arr.push_back(1);
				}
				else {
					arr.push_back(0);
				}
			}
			arr.push_back(i);
			arr.push_back(0);
			PIChart.push_back(arr);
		}
	}

	vector<int> col(Minterm_number, 0);
	for (int i = 0; i < Minterm_number; i++) {
		for (int j = 0; j < PIChart.size(); j++) {
			if (PIChart[j][i] == 1) {
				col[i]++;
			}
		}
	}
	PIChart.push_back(col);

	while (!Minterms.empty()) {
		pickup1();
		pickup2();
	}


	/*************************
	 ***   Output result   ***
	 *************************/

	cout << "\n\nF(a";
	for (int i = 1; i < Variable_number; i++) {
		char out = i + 'a';
		cout << "," << out;
	}
	cout << ") = ";

	for (int i = 0; i < PIChosen.size(); i++) {
		index = PIChosen[i];
		for (int j = 0; j < Variable_number; j++) {
			if (Allterms[index][j] == '1') {
				char out = 'a' + j;
				cout << out;
			}
			else if (Allterms[index][j] == '0') {
				char out = 'a' + j;
				cout << out;
				cout << "'";
			}
		}
		if (i < PIChosen.size() - 1) {
			cout << '+';
		}
	}
	cout << "\n\n";

	return 0;
}

/***********************
 ****   END MAIN()   ***
 ***********************/

string dec_to_bin(int input)
{
	string result;
	int tmp[10];
	int one_number = 0;

	for (int i = 0; i < Variable_number; i++) {
		tmp[i] = input % 2;
		input /= 2;
	}
	for (int i = Variable_number - 1; i >= 0; i--) {
		if (tmp[i] == 0) {
			result += '0';
		}
		else {
			result += '1';
			one_number++;
		}
	}
	result += '1';
	OneCounter.push_back(one_number);

	return result;
}

int difference(int a, int b)
{
	int count = 0, pos;

	for (int i = 0; i < Variable_number; i++) {
		if (Allterms[a][i] != Allterms[b][i]) {
			count++;
			pos = i;
		}
	}

	if (count == 1) {
		return pos;
	}
	else {
		return Variable_number;
	}
}

bool operator==(const string &a, const string &b)
{
	for (int i = 0; i < Variable_number; i++) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}

bool contain(int allterms_index, int minterm_index)
{
	for (int i = 0; i < Variable_number; i++) {
		if ((Allterms[allterms_index][i] == '1' && Allterms[minterm_index][i] == '0') || (Allterms[allterms_index][i] == '0' && Allterms[minterm_index][i] == '1')) {
			return false;
		}
	}
	return true;
}

void pickup1(void)
{
	for (int i = 0; i < Minterms.size(); i++) {
		if (PIChart[PIChart.size() - 1][i] == 1) {
			for (int j = 0; j <= PIChart.size() - 2; j++) {
				if (PIChart[j][i] == 1) {
					PIChosen.push_back(PIChart[j][Minterms.size()]);
					for (int k = Minterms.size() - 1; k >= 0; k--) {
						if (PIChart[j][k] == 1) {
							Minterms.erase(Minterms.begin() + k);
							for (int l = 0; l < PIChart.size(); l++) {
								PIChart[l].erase(PIChart[l].begin() + k);
							}
						}
					}
					PIChart.erase(PIChart.begin() + j);
					return;
				}
			}
		}
	}
	return;
}

void pickup2(void)
{
	if (Minterms.empty()) {
		return;
	}
	for (int i = 0; i < Minterms.size(); i++) {
		if (PIChart[PIChart.size() - 1][i] == 1) {
			return;
		}
	}

	for (int i = 0; i <= PIChart.size() - 2; i++) {
		int row_sum = 0;
		for (int j = 0; j < Minterms.size(); j++) {
			if (PIChart[i][j] == 1) {
				row_sum++;
			}
		}
		PIChart[i][PIChart[i].size() - 1] = row_sum;
	}

	int max = 0, index = 0;
	for (int i = 0; i <= PIChart.size() - 2; i++) {
		if (PIChart[i][PIChart[i].size() - 1] > max) {
			max = PIChart[i][PIChart[i].size() - 1];
			index = i;
		}
	}

	PIChosen.push_back(PIChart[index][Minterms.size()]);
	for (int i = Minterms.size() - 1; i >= 0; i--) {
		if (PIChart[index][i] == 1) {
			Minterms.erase(Minterms.begin() + i);
			for (int j = 0; j < PIChart.size(); j++) {
				PIChart[j].erase(PIChart[j].begin() + i);
			}
		}
	}
	PIChart.erase(PIChart.begin() + index);

	return;
}







