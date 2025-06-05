#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <iomanip>

using namespace std;

void ShowMainMenu();
void ShowTransactionsMenu();

const string ClientsFileName = "Clients.txt";

struct stClient
{
	string AccountNumber, PinCode, Name, Phone;
	double AccountBalance{0.0};
	bool MarkForDelete{false};
};

enum enMainMenuOptions
{
	eListClients = 1,
	eAddNewClient,
	eDeleteClient,
	eUpdateClient,
	eFindClient,
	eShowTransactionsMenu,
	eExit
};

enum enTransactionsMenuOptions
{
	eDeposit = 1,
	eWithdraw,
	eShowTotalBalance,
	eShowMainMenu
};

enMainMenuOptions ReadMainMenuOption()
{
	short Choice{0};
	cout << "Choose what do you want to do? [1 to 7]? ";
	while (!(cin >> Choice) || Choice < 1 || Choice > 7)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input. Please enter a number between 1 and 7: ";
	}
	return static_cast<enMainMenuOptions>(Choice);
}

void GoBackToMainMenu()
{
	cout << "\nPress any key to go back to Main Menu...";
	system("Pause>0");
	ShowMainMenu();
}

vector<string> Split(string Line, string Delim)
{
	vector<string> vString;
	short Pos{0};
	string sWord;
	while ((Pos = Line.find(Delim)) != string::npos)
	{
		sWord = Line.substr(0, Pos);
		if (sWord != "")
			vString.push_back(sWord);
		Line.erase(0, Pos + Delim.length());
	}
	if (Line != "")
		vString.push_back(Line);
	return vString;
}

stClient ConvertLineToRecord(string Line, string Delim = "#//#")
{
	vector<string> vString = Split(Line, Delim);
	stClient Client;
	Client.AccountNumber = vString.at(0);
	Client.PinCode = vString.at(1);
	Client.Name = vString.at(2);
	Client.Phone = vString.at(3);
	Client.AccountBalance = stod(vString.at(4));
	return Client;
}

vector<stClient> LoadClientsDataFromFile(string FileName)
{
	vector<stClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

void PrintHorizontalLine()
{
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n"
		 << endl;
}

void PrintClientsHeader()
{
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
}

void PrintClientRecord(stClient &Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAllClientsList()
{
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	PrintHorizontalLine();
	PrintClientsHeader();
	PrintHorizontalLine();

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
		for (stClient &C : vClients)
		{
			PrintClientRecord(C);
			cout << endl;
		}
	PrintHorizontalLine();
}

stClient ReadNewClient()
{
	stClient Client;

	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);

	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance? ";
	while (!(cin >> Client.AccountBalance))
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input. Please enter a valid Account Balance: ";
	}
	return Client;
}

string ConvertRecordToLine(stClient &Client, string Delim = "#//#")
{
	return Client.AccountNumber + Delim + Client.PinCode + Delim +
		   Client.Name + Delim + Client.Phone + Delim + to_string(Client.AccountBalance);
}

void AddDataLineToFile(string FileName, string DataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::app);
	if (MyFile.is_open())
	{
		MyFile << DataLine << endl;
		MyFile.close();
	}
}

void AddNewClient()
{
	stClient Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
	char AddMore{'y'};
	do
	{
		cout << "\nAdding a new client record...\n";
		AddNewClient();
		cout << "\nDo you want to add another client? (Y/N): ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

void ShowAddNewClientScreen()
{
	cout << "\n===========================================\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n===========================================\n";
	AddNewClients();
}

string ReadClientAccountNumber()
{
	string AccountNumber;
	cout << "Enter Client Account Number: ";
	getline(cin >> ws, AccountNumber);
	return AccountNumber;
}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient> &vClients, stClient &Client)
{
	for (stClient &C : vClients)
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	return false;
}

void PrintClientCard(stClient &Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "\nAccount Number: " << Client.AccountNumber;
	cout << "\nPin Code : " << Client.PinCode;
	cout << "\nName : " << Client.Name;
	cout << "\nPhone : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient> &vClients)
{
	for (stClient &C : vClients)
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	return false;
}

vector<stClient> SaveClientsDataToFile(string FileName, vector<stClient> &vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open())
		for (stClient &C : vClients)
			if (C.MarkForDelete == false)
				MyFile << ConvertRecordToLine(C) << endl;
	MyFile.close();
	return vClients;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient> &vClients)
{
	stClient Client;
	char Answer{'n'};
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		cout << "\nAre you sure you want to delete this client? (Y/N): ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);
			vClients = LoadClientsDataFromFile(ClientsFileName);
			cout << "\nClient with Account Number: " << AccountNumber << " has been deleted successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number: " << AccountNumber << " not found.\n";
		return false;
	}
}

void ShowDeleteClientsScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n-----------------------------------\n";
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;
	Client.AccountNumber = AccountNumber;

	cout << "Enter New PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter New Name? ";
	getline(cin, Client.Name);

	cout << "Enter New Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter New Account Balance? ";
	while (!(cin >> Client.AccountBalance))
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input. Please enter a valid Account Balance: ";
	}
	return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient> &vClients)
{
	stClient Client;
	char Answer{'n'};
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		cout << "\nAre you sure you want to update this client? (Y/N): ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			for (stClient &C : vClients)
			{
				C = ChangeClientRecord(AccountNumber);
				break;
			}
			SaveClientsDataToFile(ClientsFileName, vClients);
			cout << "\nClient with Account Number: " << AccountNumber << " has been updated successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number: " << AccountNumber << " not found.\n";
		return false;
	}
}

void ShowUpdateClientsScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate Client Screen";
	cout << "\n-----------------------------------\n";
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientsScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";
	stClient Client;
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientCard(Client);
	else
		cout << "\nClient with Account Number: " << AccountNumber << " not found.\n";
}

void ShowEndScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tProgram Ends :-) \n";
	cout << "-----------------------------------\n";
	cout << "Thank you for using our client management system!\n";
	cout << "Have a great day!\n";
	cout << "-----------------------------------\n";
}

enTransactionsMenuOptions ReadTransactionsMenuOption()
{
	short Choice{0};
	cout << "Choose what do you want to do? [1 to 4]? ";
	while (!(cin >> Choice) || Choice < 1 || Choice > 4)
	{
		cin.clear();										 // clear the error flag
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
		cout << "Invalid input. Please enter a number between 1 and 4: ";
	}
	return static_cast<enTransactionsMenuOptions>(Choice);
}

void GoBackToTransactionsMenu()
{
	cout << "\nPress any key to go back to Transactions Menu...";
	system("pause>0");
	ShowTransactionsMenu();
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<stClient> &vClients)
{
	char Answer{'n'};
	cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
		for (stClient &C : vClients)
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
				return true;
			}
	return false;
}

void ShowDepositScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n-----------------------------------\n";
	stClient Client;
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	while (!(FindClientByAccountNumber(AccountNumber, vClients, Client)))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);

	double Amount{0.0};
	cout << "\nPlease enter deposit amount? ";
	while (!(cin >> Amount) || Amount <= 0)
	{
		cin.clear();										 // clear the error flag
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
		cout << "Invalid input. Please enter a valid deposit amount: ";
	}
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithdrawScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tWithdraw Screen";
	cout << "\n-----------------------------------\n";

	stClient Client;
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	while (!(FindClientByAccountNumber(AccountNumber, vClients, Client)))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);
	double Amount{0.0};
	cout << "\nPlease enter withdraw amount? ";
	while (!(cin >> Amount) || Amount <= 0 || Amount > Client.AccountBalance)
	{
		cin.clear();										 // clear the error flag
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
		cout << "Invalid input. Please enter a valid withdraw amount (up to " << Client.AccountBalance << "): ";
	}
	DepositBalanceToClientByAccountNumber(AccountNumber, -Amount, vClients);
}

void PrintClientRecordBalanceLine(stClient &Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowTotalBalancesScreen()
{
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	PrintHorizontalLine();
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	PrintHorizontalLine();
	double TotalBalances{0.0};
	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
		for (stClient &C : vClients)
		{
			PrintClientRecordBalanceLine(C);
			TotalBalances += C.AccountBalance;
			cout << endl;
		}
	PrintHorizontalLine();
	cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;
}

void PerformTransactionsMenuOption(enTransactionsMenuOptions TransactionsMenuOption)
{
	switch (TransactionsMenuOption)
	{
	case enTransactionsMenuOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransactionsMenuOptions::eWithdraw:
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransactionsMenuOptions::eShowTotalBalance:
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransactionsMenuOptions::eShowMainMenu:
		ShowMainMenu();
	}
}

void ShowTransactionsMenu()
{
	system("cls");
	cout << "\n===========================================\n";
	cout << "\t\tTransactions Menu Screen\n";
	cout << "===========================================\n";
	cout << "\t\t[1] Deposit\n";
	cout << "\t\t[2] Withdraw\n";
	cout << "\t\t[3] Total Balances\n";
	cout << "\t\t[4] Main Menu\n";
	cout << "===========================================\n";
	PerformTransactionsMenuOption(ReadTransactionsMenuOption());
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOption)
{
	switch (MainMenuOption)
	{
	case enMainMenuOptions::eListClients:
		system("cls");
		ShowAllClientsList();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eAddNewClient:
		system("cls");
		ShowAddNewClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eFindClient:
		system("cls");
		ShowFindClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eShowTransactionsMenu:
		system("cls");
		ShowTransactionsMenu();
		break;
	case enMainMenuOptions::eExit:
		system("cls");
		ShowEndScreen();
		break;
	}
}

void ShowMainMenu()
{
	system("cls");
	cout << "================================================\n";
	cout << "                 Main Menu Screen                 \n";
	cout << "================================================\n";
	cout << "           [1] Show Client List                  \n";
	cout << "           [2] Add New Client                    \n";
	cout << "           [3] Delete Client                     \n";
	cout << "           [4] Update Client Info                \n";
	cout << "           [5] Find Client                       \n";
	cout << "           [6] Transactions                       \n";
	cout << "           [7] Exit                              \n";
	cout << "================================================\n";
	PerformMainMenuOption(ReadMainMenuOption());
}

int main()
{
	ShowMainMenu();
	return 0;
}