#include "iostream"
#include "iomanip"
#include "fstream"
#include "string"
#include "ctype.h"
#include "conio.h"

using namespace std;

char *FileName = "firm.txt"; // Име на файл

struct Branch
{
	string BranchNo; // Номер на филиал
	string BranchName; // Наименование на филиал
	string BranchAdress; // Адрес на филиал
	int EmployeeCount; // Брой служители
	double AvgSalary; // Обща заплата
};

// Функция за присвояване на данни за нов филиал

Branch setValue(string Branch_No, string Branch_Name, string Branch_Adress, int Employee_Count, double Avg_Salary)
{
	Branch Bran;
	Bran.BranchNo = Branch_No;
	Bran.BranchName = Branch_Name;
	Bran.BranchAdress = Branch_Adress;
	Bran.EmployeeCount = Employee_Count;
	Bran.AvgSalary = Avg_Salary;
	return Bran;
}

// Меню на файловата система

char DisplayMenu()
{
	cout << "\n\t\t~ M E N U ~\n--------------------------------------------\nC - Create empty file\nW - Write to file\nR - Read from file" 
		<< "\nA - Add a new Branch to File"
		<< "\nE - Edit Branch\nD - Delete a Branch\n"<< "O - Output Branch With Largest Avarage Salary\n"
		<< "X - Exit program\n--------------------------------------------\n  Your choice: ";
	char choice;
	choice = _getch();
	choice = toupper(choice);
	return choice;
}

// Функция за добавяне на данни за филиал

void Input(Branch &Bran)
{
	cout << "Enter a Branch Number: ";	getline(cin, Bran.BranchNo);
	cout << "Enter a Branch Name: ";	getline(cin, Bran.BranchName);
	if(Bran.BranchName.length()>10)
	{
		cerr << "Can't eneter more than 10 symbols !";
		return;
	}
	cout << "Enter a Branch Adress: ";	getline(cin, Bran.BranchAdress);
	if(Bran.BranchAdress.length()>40)
	{
		cerr << "Can't eneter more than 40 symbols !";
		return;
	}
	cout << "Enter an Employee Count: ";	cin >> Bran.EmployeeCount;
	cout << "Enter an Avarage Salary: ";	cin >> Bran.AvgSalary;
}

// Финкция за отпечатване на данни за един филиал

void Output(Branch Bran)
{
	cout << setw(10) << setiosflags(ios::left) << Bran.BranchNo
		<< setw(10) << setiosflags(ios::left) << Bran.BranchName
		<< setw(10) << setiosflags(ios::left) << Bran.BranchAdress << "\t"
		<< setw(10) << setiosflags(ios::left) << Bran.EmployeeCount
		<< setw(10) << setiosflags(ios::left) << Bran.AvgSalary << " LV"
		<< endl;
}

// Фукция за четене на дании на филиал

void ReadFile(char *FileName)
{
	ifstream Branches;
	Branch Bran;

	Branches.open(FileName, ios::in);
	Branches.clear();
	Branches.seekg(0);
	
	while (Branches.read(reinterpret_cast<char *>(&Bran), sizeof(Branch)))
	{
		Output(Bran);
		if (Branches.eof())
			break;
	}
	Branches.close();
}

// Функция за създаване на празен файл

void CreateFile(char *FileName)
{
	fstream Branches;
	Branch Bran;
	Branches.open(FileName, ios::in);
	if (!Branches) // Ако файлът не може да бъде отворен
	{
		cerr << "File couldn't be opened\n";
		cout << "Create a new file...\n";
		Branches.open(FileName, ios::out); //отваряне нов файл
		Branches.close();
		return;
	}
	Branches.seekg(0, ios::end); //установява get указателя в края на файла
	streamoff SizeFile;
	SizeFile = Branches.tellg(); //определя размера на файла
	
	cout << "File isn't empty. It has "
		<< SizeFile / sizeof(Branch) << " Branches.\n";
	
	char Answer;
	cout << "Delete file? (Y/N): ";

	Answer = _getch(); Answer = toupper(Answer);
	
	if (Answer != 'Y')
	{
		Branches.close();
		return;
	}
	Branches.clear();
	Branches.seekg(0);
	Branches.close();
	Branches.open(FileName, ios::out | ios::trunc);
	Branches.close();

}

//Функция за създаване на файл, като замества изцяло информацията в него

void WriteToFile(char *FileName)
{
	ofstream Branches;
	Branch Bran;
	Branches.open(FileName, ios::out);
	long i, count;
	cout << "Enter count of Branches to add: ";
	cin >> count;
	for (i = 0; i < count; i++)
	{
		cin.ignore();
		Input(Bran);
		Branches.write(reinterpret_cast<char *> (&Bran), sizeof(Branch));
	}
	Branches.close();
}

// Функция за редактиране на информация на Филиал

void EditBranch(char *FileName)
{
	fstream Branches;
	Branch Bran;

	Branches.open(FileName, ios::in);
	Branches.clear();
	Branches.seekg(0);
	
	streamoff record;
	cout << "Enter Branch: "; cin >> record;
	
	Branches.seekg((record - 1) * sizeof(Branch));
	Branches.read(reinterpret_cast<char*>(&Bran), sizeof(Branch));
	Output(Bran);
	
	char Answer;
	cout << "Update Branch? (Y/N): ";
	Answer = _getch(); Answer = toupper(Answer);
	
	if (Answer != 'Y')
	{
		Branches.close();
		return;
	}
	
	Branches.close();
	cin.ignore();
	Input(Bran);
	
	Branches.open(FileName, ios::ate | ios::out | ios::_Nocreate);
	Branches.clear();
	Branches.seekp((record - 1) * sizeof(Branch));
	Branches.write(reinterpret_cast<char *> (&Bran), sizeof(Branch));
	Branches.seekp(0, ios::end);
	Branches.close();
}

// Функция за изтриване на филиал

void DeleteBranch(char *FileName)
{
	fstream Branches;
	Branch Bran;
	
	Branches.open(FileName, ios::in);
	Branches.clear();
	Branches.seekg(0);
	streamoff record;
	cout << "Enter Branch: "; cin >> record;
	
	Branches.seekg((record - 1) * sizeof(Branch));
	Branches.read(reinterpret_cast<char*>(&Bran), sizeof(Branch));
	Output(Bran);
	
	char Answer;
	cout << "Delete Branch? (Y/N): ";
	Answer = _getch(); Answer = toupper(Answer);
	
	if (Answer != 'Y')
	{
		Branches.close();
		return;
	}
	Branches.close();
	cin.ignore();
	
	Bran = setValue("No Branch Number", "No Branch Name", "No Branch Adress", 0, 0.00);
	Branches.open(FileName, ios::ate | ios::out | ios::_Nocreate);
	Branches.clear();
	Branches.seekp((record - 1) * sizeof(Branch));
	Branches.write(reinterpret_cast<char *> (&Bran), sizeof(Branch));
	Branches.seekp(0, ios::end);
	Branches.close();
}

void OutputLargestSalary(char *FileName) 
{
	ifstream Branches;
	Branch Bran;

	Branches.open(FileName, ios::in);
	Branches.read(reinterpret_cast<char *> (&Bran), sizeof(Branch));

	double MaxSalary = Bran.AvgSalary;
	while (Branches.read(reinterpret_cast<char *> (&Bran), sizeof(Branch)))
	{
		if (MaxSalary < Bran.AvgSalary) MaxSalary = Bran.AvgSalary;
		if (Branches.eof()) break;
	}
	cout << Bran.BranchName << " - ";
	cout << MaxSalary << " LV" << endl;
	Branches.close();
}

// Функция за добавяне на нов филиал

void AddBranch(char *FileName)
{
	ofstream Branches;
	Branch Bran;
	int i, count;
	Branches.open(FileName, ios::app);
	Branches.seekp(0, ios::end);
	cout << "Enter count of Branches: ";
	cin >> count;
	for (i = 0; i < count; i++)
	{
		cin.ignore();
		Input(Bran);
		Branches.write(reinterpret_cast<char *> (&Bran), sizeof(Branch));
	}
	Branches.close();
}


	int main()
{
		char choice;
		do
		{
			choice = DisplayMenu();
			switch (choice)
			{
			case 'C': {
				cout << "\nCreate empty file\n";
				CreateFile(FileName);
				break;
			}
			case 'W': {
				cout << "\nWrite to file\n";
				WriteToFile(FileName);
				break;
			}
			case 'A': {
				cout << "\nAdd a new Branch to FIle\n";
				AddBranch(FileName);
				break;
			}
			case 'R': {
				cout << "\nRead from file\n";
				ReadFile(FileName);
				break;
			}
			case 'E': {
				cout << "\nEdit a Branch\n";
				EditBranch(FileName);
				break;
			}
			case 'D': {
				cout << "\nDelete a Branch\n";
				DeleteBranch(FileName);
				break;
			}
			case 'O': {
				cout << "\nOutput Branch With Largest Avarage Salary\n";
				OutputLargestSalary(FileName);
				break;
			}
			case 'X': {cout << "\nExiting program...\n"; break; }

			default: {cout << "Invalid choice...\n"; break; }
			}

		} while (choice != 'X');
		cout << "\n\t\t Daniel-Database v 1.0\n";
		cout << "\nPress any key...";
		_getch();

	return 0;
}