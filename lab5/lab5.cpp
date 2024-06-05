#include <iostream>
#include <vector>
#include <Windows.h>
#include <map>
#include <string>
#include <fstream>
using namespace std;
void ind_eqv(map <int, char> m, vector <char> v, int* mas, double* ind)
{
	int mas1[256]{};
	for (int i = 0; i < v.size(); i++)
	{
		mas1[int(v[i]) + 128]++;
	}
	for (int i = 0; i < 33; i++)
	{
		mas[i] = mas1[int(m[i]) + 128];
	}
	*ind = 0;
	for (int i = 0; i < 33; i++)
	{
		*ind += (mas[i] * (mas[i] - 1));
	}
	*ind /= (v.size() * (v.size() - 1));
}
void ind_vz(int mas[5][33], int num, double* ind, int size, int size2, int step)
{
	*ind = 0;
	int rotate_mas[33];
	int temp[33];
	int i;
	for (int i = 0; i < 33; i++)
	{
		rotate_mas[i] = mas[num][i];
	}
	for (int i = 0; i < 33; i++)
	{
		temp[(i + step) % 33] = rotate_mas[i];
	}
	for (int i = 0; i < 33; i++)
	{
		rotate_mas[i] = temp[i];
	}
	for (int i = 0; i < 33; i++)
	{
		*ind += mas[0][i] * rotate_mas[i];
	}
	*ind /= (size * size2);
}
int main()
{
	setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	map <char, int> alphabet;
	map <int, char> alphabet1;
	for (int i = 0; i < 6; i++)
	{
		alphabet[char(int('А') + i)] = i;
		alphabet1[i] = char(int('А') + i);
	}
	alphabet['Ё'] = 6;
	alphabet1[6] = 'Ё';
	for (int i = 7; i < 33; i++)
	{
		alphabet[char(int('Ж') + i - 7)] = i;
		alphabet1[i] = char(int('Ж') + i - 7);
	}
	string pathIN = "1v.txt";
	string pathout = "out.txt";
	ifstream fin;
	ofstream fout;
	fin.open(pathIN);
	fout.open(pathout);
	const int lks = 5;
	vector <char> text_in[lks];
	int n = 0;
	char ch;
	vector<char> text;
	while (fin.get(ch))
	{
		text_in[n].push_back(ch);
		text.push_back(ch);
		n++;
		if (n == lks) n = 0;
	}
	int mas[lks][33];
	double ind[lks];
	for (int i = 0; i < lks; i++)
	{
		ind_eqv(alphabet1, text_in[i], mas[i], &ind[i]);
		cout << "индекс совпадения для столбца " << i + 1 << " = " << ind[i] << endl;
	}
	int otn_sdvig[lks - 1]{};
	double vz_ind[lks - 1]{};
	for (int i = 1; i < lks; i++)
	{
		for (int j = 0; j < 33; j++)
		{
			ind_vz(mas, i, &vz_ind[i - 1], text_in[0].size(), text_in[i].size(), j);
			if (vz_ind[i - 1] > 0.05)
			{
				otn_sdvig[i - 1] = j;
				cout << " взаимный индекс совпадения между столбцом 1 и столбцом "
					<< i + 1 << " = " << vz_ind[i - 1] << endl;
				cout << " относительный сдвиг = " << otn_sdvig[i - 1] << endl;
				break;
			}
		}
	}
	string alph = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
	for (int j = 0; j < 33; j++)
	{
		string word = "";
		word += alph[j];
		for (int i = 0; i < 4; ++i) {
			int pos = (otn_sdvig[i] % 33 + 33) % 33;
			int neW = (j - pos + 33) % 33;
			word += alph[neW];
		}
		cout << word << endl;
	}
	string key;
	cout << "Выберите и введите осмысленное слово из найденных для расшифровки: ";
	cin >> key;
	vector <char> text_out;
	int j = 0;
	for (char CH : text)
	{
		bool a = CH >= 'А';
		bool ya = CH <= 'Я';
		bool yo = CH == 'Ё';
		if ((a and ya) or yo)
		{
			char code;
			code = alphabet1[(alphabet[CH] - alphabet[key[j]] + 33) % 33];
			if (j == key.length() - 1)
			{
				j = 0;
			}
			else {
				j++;
			}
			text_out.push_back(code);
		}
		else {
			text_out.push_back(CH);
		}
	}
	for (char ch : text_out)
	{
		cout << ch;
		fout << ch;
	}
}