// ============================================================================
//  Bank Client Management System (console, C++)
//
//  How it works (big picture):
//      Client.txt  --Load-->  vector<strc_Client_Data>  --edit-->  Save-->  Client.txt
//      (the "notebook")        (the "scratch paper")
//
//  Every record is one line in the file, fields separated by "#//#":
//      A101#//#1234#//#Ali Ahmad#//#0790000000#//#500.000000
// ============================================================================
#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>


using namespace std;
const string ClientsFileName = "Client.txt";

// ============================================================================
//  1. Data structures & enums
// ============================================================================

// One client = one record. The two Mark* flags are used by the
// "mark first, then save" approach for delete/update.
struct strc_Client_Data {
	string Account_Number = "";
	string Pin_code = "";
	string Client_Name = "";
	string Phone = "";
	double Account_Balance = 0;
	bool MarkForDelete = false;
	bool MarkForUpdate = false;
};

// Main menu options (enum gives readable names to the numbers 1..7)
enum ChoiceList {
	ClientsList = 1,
	NewClient = 2,
	Delete = 3,
	Update = 4,
	FindClient = 5,
	Transaction = 6,
	Exit = 7
};

// Transactions menu options
enum ChoiceTransaction {
	Deposit = 1,
	Withdraw = 2,
	Total_Balances = 3,
	Main_Menu = 4
};

// ============================================================================
//  2. Forward declarations
//  (needed because some functions are used before they are defined)
// ============================================================================
vector <strc_Client_Data> SaveClientsDataToFile(string FileName, vector<strc_Client_Data>& vClients);
 
void showHeaderClientList(short ClientsNumber);

vector <strc_Client_Data>LoadClientsDataFromFile(string fileName);

void showClient(vector <strc_Client_Data> &Clients);

void MainMenuScreen();

void ResetScreen(string Menu);

void PrintClientsDetails(strc_Client_Data& Struct);

bool FindClientByAccountNumber(string AccountNumber, vector<strc_Client_Data>& vClients, strc_Client_Data& Client);

void showClientsList(vector<strc_Client_Data>& Clients);

// ============================================================================
//  3. Input helpers & string utilities
// ============================================================================

// Reads the transactions menu choice.
// NOTE: if the user types a non-number, cin is cleared and Choice becomes 0,
//       which falls into the "default" case of the switch (Invalid Choice).
ChoiceTransaction ReadTransChoice() {
	short Choice = 0;
	cout << "Choose what do you want to do ? [1 - 4]? ";
	if (!(cin >> Choice))
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		Choice = 0;
	}
	return ChoiceTransaction(Choice);
}

// Splits a string by a delimiter: "a#//#b#//#c" -> {"a","b","c"}
vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;
	short pos = 0;
	string sWord; // define a string variable
	// use find() function to get the position of the delimiters
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos); // store the word
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length());
	}
	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.
	}
	return vString;
}

// The opposite of SplitString: {"a","b","c"} -> "a#//#b#//#c"
// (the last delimiter is cut off at the end)
string JoinString(vector<string>& vString, string Delim)
{
	string S1;
	for (string& s : vString)
	{
		S1 = S1 + s + Delim;
	}
	return S1.substr(0, S1.length() - Delim.length());
}

string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}

// ============================================================================
//  4. File layer: line <-> record, load, add
// ============================================================================

// Line -> record. If the line has fewer than 5 fields, an empty record is returned.
strc_Client_Data ConvertLineToRecord(string line_data, string Separator = "#//#")
{
	vector<string> Records = SplitString(line_data, Separator);
	strc_Client_Data Struct;
	if (Records.size() < 5) return Struct;
	Struct.Account_Number = Records[0];
	Struct.Pin_code = Records[1];
	Struct.Client_Name = Records[2];
	Struct.Phone = Records[3];
	Struct.Account_Balance = stod(Records[4]);

	return Struct;
}

// Reads the whole file into a vector (one record per line).
vector <strc_Client_Data> LoadClientsDataFromFile(string FileName)
{
	vector <strc_Client_Data> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		strc_Client_Data Client;
		while (getline(MyFile, Line))
		{
			if (Line == "") continue;   // تجاهل السطر الفاضي
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

// Asks the user for a new client's data.
strc_Client_Data Read_Client_Data(strc_Client_Data& Struct)
{
	cout << "\n Please, Enter Client Data :" << endl;
	cout << "\n Enter Account Number : ";
	cin >> Struct.Account_Number;

	cout << "\n Enter Pin-code : ";
	cin >> Struct.Pin_code;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');   // ← قبل getline

	cout << "\n Enter Client Name : ";
	getline(cin, Struct.Client_Name);

	cout << "\n Enter Client Phone : ";
	cin >> Struct.Phone;


	cout << "\n Enter Account Balance : ";
	cin >> Struct.Account_Balance;


	return Struct;
}

// Appends one line to the end of the file (ios::app = append mode).
void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

// Record -> line (uses JoinString).
string ConvertRecordToLine(strc_Client_Data &Struct, string Separator = "#//#")
{
	vector <string> Records;
	Records.push_back(Struct.Account_Number);
	Records.push_back(Struct.Pin_code);
	Records.push_back(Struct.Client_Name);
	Records.push_back(Struct.Phone);
	Records.push_back(to_string(Struct.Account_Balance));

	string Str = JoinString(Records, Separator);

	return Str;
}

// Option [2]: read a client, convert to a line, append to the file.
void AddNewClient()
{
	system("cls");
	strc_Client_Data Client;
	Client = Read_Client_Data(Client);
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
	cout << "\nClient is Added Successfully\n";
}

// ============================================================================
//  5. Display screens: client list
// ============================================================================

void showHeaderClientList(short ClientsNumber)
{
	cout << "\t\t\t\tClient List (" << ClientsNumber << ") Client(s)." << endl;
	cout << string(87, '_') << endl;
	cout << left
		<< "| " << setw(15) << "Account Number"
		<< "| " << setw(10) << "Pin Code"
		<< "| " << setw(25) << "Client Name"
		<< "| " << setw(15) << "Phone"
		<< "| " << setw(12) << "Balance" << endl;
	cout << string(87, '_') << endl;
}

// Prints one table row per client.
void showClient(vector<strc_Client_Data>& Clients)
{
	for (short i = 0; i < Clients.size(); i++)
		cout << left
		<< "| " << setw(15) << Clients[i].Account_Number
		<< "| " << setw(10) << Clients[i].Pin_code
		<< "| " << setw(25) << Clients[i].Client_Name
		<< "| " << setw(15) << Clients[i].Phone
		<< "| " << setw(12) << Clients[i].Account_Balance << endl;
}

// Option [1]: reload from file, then print header + rows.
void showClientsList(vector<strc_Client_Data>& Clients) {
	system("cls");
	Clients = LoadClientsDataFromFile("Client.txt");
	showHeaderClientList(Clients.size());
	showClient(Clients);
}

// Reads the main menu choice (same validation idea as ReadTransChoice).
ChoiceList ReadNumberChoice() {
	short Choice = 0;

	if (!(cin >> Choice))          
	{
		cin.clear();                                        
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		Choice = 0;                                 
	}
	return ChoiceList(Choice);
}

// ============================================================================
//  6. Transactions: deposit, withdraw, total balances
//  Flow of Deposit/Withdraw:
//      ask account -> find it -> show card -> ask amount -> confirm -> update -> save
// ============================================================================

void DepositScreen(vector <strc_Client_Data> &vClients) {
	
	strc_Client_Data Client;
	system("cls");
	cout << endl << string(52, '=') << endl;
	cout << left << setw(16) << " " << "Deposit   Screen\n";
	cout << string(52, '=') << endl;
	string AccountNumber;

	// Keep asking until a valid account number is entered.
	while (true)
	{
		AccountNumber = ReadClientAccountNumber();
		if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		{
			PrintClientsDetails(Client);
			cout << " Please enter deposit amount : ";
			double DepositAmount;
			char Sure = 'n';
			cin >> DepositAmount;
			cout << "\n Are you sure you want to perform this transaction? [y/n]?";
			cin >> Sure;
			if (tolower(Sure) == 'y')
			{
				for (strc_Client_Data& C : vClients)
				{
					if (C.Account_Number == AccountNumber)
					{
						C.Account_Balance += DepositAmount;
						SaveClientsDataToFile(ClientsFileName, vClients);
						cout << "\n Done Successfully. New Balance = " << C.Account_Balance << endl<<endl;
						break;
					}
				}
				
			}
			else
				ResetScreen("Transactions");
			break;
		}
		else {
			cout << "Client With [" << AccountNumber << "] does not exist!\n";
		}
	}
	
}

void WithdrawScreen(vector <strc_Client_Data>& vClients)
{
	strc_Client_Data Client;
	system("cls");
	cout << endl << string(52, '=') << endl;
	cout << left << setw(16) << " " << "WithDraw   Screen\n";
	cout << string(52, '=') << endl;
	string AccountNumber;

	// Outer loop: keep asking until a valid account number is entered.
	while (true)
	{
		AccountNumber = ReadClientAccountNumber();
		if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		{
			PrintClientsDetails(Client);
			double WithdrawAmount;
			char Sure = 'n';
			// Inner loop: keep asking until the amount is valid.
			while(true){
				cout << "\n Please enter withdraw amount : ";
				cin >> WithdrawAmount;
				if (WithdrawAmount < Client.Account_Balance)
				{
					cout << "\n Are you sure you want to perform this transaction? [y/n]?";
					cin >> Sure;
					if (tolower(Sure) == 'y')
					{
						for (strc_Client_Data& C : vClients)
						{
							if (C.Account_Number == AccountNumber)
							{
								C.Account_Balance -= WithdrawAmount;
								SaveClientsDataToFile(ClientsFileName, vClients);
								cout << "\n Done Successfully. New Balance = " << C.Account_Balance << endl<<endl;
								break;
							}
						}
						break;
					}
					else
						ResetScreen("Transactions");
					break;
				}
				else
				{
					cout << "\n Amount Exceeds the balance, you can withdraw up to : " << Client.Account_Balance << endl;
				}
			}
			break;
		}
		else {
			cout << "Client With [" << AccountNumber << "] does not exist!\n";
		}
	}

}

// Sum of all balances.
double TotalBalance(vector <strc_Client_Data>& vClients) {
	
	double Total = 0;
	for (short i = 0; i < vClients.size(); i++)
		Total += vClients[i].Account_Balance;
	return Total;
}

void Total_BalanceScreen(vector <strc_Client_Data>& vClients) {
	system("cls");
	short ClientsNumber = vClients.size();
	vClients = LoadClientsDataFromFile("Client.txt");
	{

		cout << "\t\t\tBalances List (" << ClientsNumber << ") Client(s)." << endl;
		cout << string(87, '_') << endl;
		cout << left
			<< "| " << setw(15) << "Account Number"
			<< "| " << setw(25) << "Client Name"
			<< "| " << setw(12) << "Balance" << endl;
		cout << string(87, '_') << endl<<endl;
	}

	for (short i = 0; i < vClients.size(); i++)
		cout << left
		<< "| " << setw(15) << vClients[i].Account_Number
		<< "| " << setw(25) << vClients[i].Client_Name
		<< "| " << setw(12) << vClients[i].Account_Balance << endl;
	cout << string(87, '_') << endl << endl;
	cout << "\t\t\tTotal  Balances = " << TotalBalance(vClients) << endl << endl;

	ResetScreen("Transactions");
}

// ============================================================================
//  7. Transactions menu
//  A while loop keeps the menu alive until the user chooses [4] Main Menu
//  (no function calls itself, so the call stack does not grow).
// ============================================================================

void ShowTransactionsMenuScreen(vector <strc_Client_Data> &Clients) 
{	
	system("cls");
	strc_Client_Data Client;
	ChoiceTransaction Choice = ChoiceTransaction::Total_Balances;
	while (Choice != ChoiceTransaction::Main_Menu)
	{ 
	system("cls");
	cout << endl << string(52, '=') << endl;
	cout << left << setw(16) << " " << "Transaction  Menu  Screen\n";
	cout << string(52, '=') << endl;
	cout << left;
	cout << setw(8) << " " << "[1] Deposit.\n";
	cout << setw(8) << " " << "[2] Withdraw.\n";
	cout << setw(8) << " " << "[3] Total Balance.\n";
	cout << setw(8) << " " << "[4] Main Menu.\n";
	cout << string(52, '=') << endl;
	Choice = ReadTransChoice();
	
		switch (Choice)
		{
		case ChoiceTransaction::Deposit:  // Add Manny
		{
			DepositScreen(Clients);
			ResetScreen("Transactions");
			break;
		}
		case ChoiceTransaction::Withdraw:
		{
			WithdrawScreen(Clients);
			ResetScreen("Transactions");
			break;
		}
		case ChoiceTransaction::Total_Balances:
		{
			Total_BalanceScreen(Clients);
			break;
		}
		case ChoiceTransaction::Main_Menu:
		{
			break;
		}

		default:
			cout << "Invalid Choice ):\n";
		}
	}
}

// ============================================================================
//  8. Main menu printing & client card
// ============================================================================

void PrintMainMenuScreen() {
	
	cout << endl << string(52, '=') << endl;
	cout << left << setw(16) << " " << "Main  Menu  Screen\n";
	cout << string(52, '=') << endl;
	cout << left;
	cout << setw(8) << " " << "[1] Show Client List.\n";
	cout << setw(8) << " " << "[2] Add New Client.\n";
	cout << setw(8) << " " << "[3] Delete Client.\n";
	cout << setw(8) << " " << "[4] Update Client Info.\n";
	cout << setw(8) << " " << "[5] Find Client.\n";
	cout << setw(8) << " " << "[6] Transactions.\n";
	cout << setw(8) << " " << "[7] Exit.\n";
	cout << string(52, '=') << endl;
}

// Prints the "client card" (all fields of one client).
void PrintClientsDetails(strc_Client_Data& Struct)
{
	cout << "\n The following are the client details : \n\n";
	cout << string(38, '=') << endl;
	cout << left;
	cout << setw(17) << " Account Number: " << Struct.Account_Number << endl;
	cout << setw(17) << " Pin Code: " << Struct.Pin_code << endl;
	cout << setw(17) << " Name: " << Struct.Client_Name << endl;
	cout << setw(17) << " Phone: " << Struct.Phone << endl;
	cout << setw(17) << " Account Balance: " << Struct.Account_Balance << endl;
	cout << string(38, '=') << endl<<endl;
}

// ============================================================================
//  9. Find / Delete / Update
//  Delete and Update use the "mark, then save" idea:
//      1) find the client   2) set a flag on it   3) Save writes accordingly
// ============================================================================

// NOTE: this function reloads vClients from the file before searching,
//       so the vector always matches what is stored in Client.txt.
bool FindClientByAccountNumber(string AccountNumber, vector <strc_Client_Data> &vClients, strc_Client_Data& Client)
{
	vClients = LoadClientsDataFromFile(ClientsFileName);
	for (strc_Client_Data& C : vClients)
	{
		if (C.Account_Number == AccountNumber)
		{
			Client = C;
			return true;
		}
	}

	return false;
}

// Sets MarkForDelete = true on the matching client (nothing is deleted yet).
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <strc_Client_Data>& vClients)
{
	for (strc_Client_Data& C : vClients)
	{
		if (C.Account_Number == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

// Asks the user for the new values of an existing client.
strc_Client_Data Update_Client_Data(strc_Client_Data& Struct)
{
	cout << "Enter Pin-code : ";
	cin >> Struct.Pin_code;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');   // ← قبل getline

	cout << "Enter Client Name : ";
	getline(cin, Struct.Client_Name);

	cout << "Enter Client Phone : ";
	cin >> Struct.Phone;


	cout << "Enter Account Balance : ";
	cin >> Struct.Account_Balance;


	return Struct;
}

// Rewrites the whole file from the vector (ios::out = overwrite).
//   - MarkForDelete == true  -> the record is skipped (so it disappears)
//   - MarkForUpdate == true  -> new data is requested, then the record is written
//   - otherwise              -> the record is written as it is
vector <strc_Client_Data> SaveClientsDataToFile(string FileName, vector<strc_Client_Data>& vClients) {

	fstream MyFile;
	MyFile.open(FileName, ios::out);

	string DataLine;
	if (MyFile.is_open())
	{
		for (strc_Client_Data C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				if (C.MarkForUpdate == false)
				{
					DataLine = ConvertRecordToLine(C);
					MyFile << DataLine << endl;
				}
				else
				{
					Update_Client_Data(C);
					DataLine = ConvertRecordToLine(C);
					MyFile << DataLine << endl;
				}
			}
		}
		MyFile.close();
	}
	return vClients;
}

// Flow: reload -> find -> show card -> confirm -> mark -> save -> reload.
bool DeleteClientByAccountNumber(string AccountNumber, vector <strc_Client_Data>& vClients)
{
	vClients = LoadClientsDataFromFile(ClientsFileName);
	strc_Client_Data Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientsDetails(Client);
		cout << "\n Are you sure you want delete this client? y/n ? ";
		cin >> Answer;
		if (tolower(Answer) == 'y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);
			vClients = LoadClientsDataFromFile(ClientsFileName);
			cout << "\n Client Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber
			<< ") is Not Found!";
	}
	return false;   // user said no, or client not found
}

// Sets MarkForUpdate = true on the matching client.
bool MarkClientForUpdateByAccountNumber(string AccountNumber, vector <strc_Client_Data>& vClients)
{
	strc_Client_Data Client;
	for (strc_Client_Data& C : vClients)
	{
		if (C.Account_Number == AccountNumber)
		{
			C.MarkForUpdate = true;
			return true;
		}
	}
	return  false;
}

// Flow: reload -> find -> show card -> confirm -> mark -> save (asks new data) -> reload.
bool UpdateClientDataByAccountNumber(string AccountNumber, vector <strc_Client_Data>& vClients)
{
	vClients = LoadClientsDataFromFile(ClientsFileName);
	strc_Client_Data Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientsDetails(Client);
		cout << "\n Are you sure you want to update this client? y/n ? ";
		cin >> Answer;
		if (tolower(Answer) == 'y')
		{
			MarkClientForUpdateByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);
			vClients = LoadClientsDataFromFile(ClientsFileName);
			cout << "\n Client Updated Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\n Client with Account Number (" << AccountNumber
			<< ") is Not Found!";
	}
	return false;
}

// ============================================================================
//  10. Exit, screen reset & main menu loop
// ============================================================================

void ExitFunction()
{
	cout << "\n" << string(52, '=') << endl;
	cout << setw(14) << " " << "PROGRAM END (:\n";
	cout << string(52, '=') << endl;
}

// "Press any key" pause, then clear the screen.
void ResetScreen(string Menu) {
	cout << " Press any key to go back to "<<Menu<< " Menu...";
	system("pause > 0");
	system("cls");
}

// The program's main loop: show menu -> read choice -> run the matching screen
// -> repeat until the user chooses [7] Exit.
void MainMenuScreen()
{
	vector <strc_Client_Data> Clients;
	string AccountNumber;
	ChoiceList Choice = ChoiceList::ClientsList;
	while (Choice != ChoiceList::Exit)
	{
		PrintMainMenuScreen();
		cout << "Choose what do you want to do ? [1 - 7]? ";
		Choice = ReadNumberChoice();

		switch (Choice)
		{
		case ChoiceList::ClientsList:
		{
			showClientsList(Clients);
			cout << endl;
			ResetScreen("Main");
			break;
		}
		case ChoiceList::NewClient:
		{
			AddNewClient();
			cout << endl;
			ResetScreen("Main");

			break;
		}
		case ChoiceList::Delete:
		{
			system("cls");
			AccountNumber = ReadClientAccountNumber();
			DeleteClientByAccountNumber(AccountNumber,Clients);
			cout << endl;
			ResetScreen("Main");
			break;
		}
		case ChoiceList::Update:
		{
			system("cls");
			AccountNumber = ReadClientAccountNumber();
			UpdateClientDataByAccountNumber(AccountNumber,Clients);
			cout << endl;
			ResetScreen("Main");
			break;
		}
		case ChoiceList::FindClient:
		{
			system("cls");
			strc_Client_Data Client;
			AccountNumber = ReadClientAccountNumber();
			if(FindClientByAccountNumber(AccountNumber,Clients, Client))
			PrintClientsDetails(Client);
			else
			{
				cout << "\n Client with Account Number (" << AccountNumber << ") is Not Found!";
			}
			cout << endl;
			ResetScreen("Main");
			break;
		}
		case ChoiceList::Transaction:
		{
			ShowTransactionsMenuScreen(Clients);
			cout << endl;
			system("cls");
			break;
		}
		case ChoiceList::Exit:
		{
			ExitFunction();
			break;
		}
		default:
		{
			cout << " Invalid Input";
			ResetScreen("Main");
		}
		}
	}
}
