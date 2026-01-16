#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

const string ClientFileName = "Clientr.txt";


enum EnATMMainMnueOption{EQuickWithDarw=1,ENormalWithdraw=2, EDeposit =3,ECheckBalance =4,ELogout=5};

//decleration
void ShowATMMainMnueScreen();
void ShowQuickWithDrawScreen();

void Login();

struct StClient
{
	string AcountNumber;
	string PINCode;
	string PhoneNumber;
	string Name;
    int Balance;
   bool MarkFoDelete;
};

vector<string>SplitString(string Line, string Delimt = "#//#")
{
	vector<string>VClinetRecord;
	string Word;
	short Pos;

	while ((Pos=Line.find(Delimt))!=std::string::npos)
	{
		Word = Line.substr(0, Pos);

		if (Word != "")
		{
			VClinetRecord.push_back(Word);

			Line.erase(0, Pos + Delimt.length());
		}

	}


	if (Line != "")
	{

		VClinetRecord.push_back(Line);
	}


	return VClinetRecord;

}

StClient CurentClient;

StClient ReverseLineDataToClinetRecord(string SClinet)
{
	vector<string>VClient = SplitString(SClinet);
	StClient Client;

	Client.AcountNumber = VClient[0];
	Client.PINCode = VClient[1];
	Client.Name = VClient[2];
	Client.PhoneNumber = VClient[3];
	Client.Balance = stoi(VClient[4]);

	return Client;
}

string ReverseClientRecordToLine(StClient Client, string Delimt = "#//#")
{
	string Line;


	Line = Client.AcountNumber + Delimt;
	Line += Client.PINCode + Delimt;
	Line += Client.Name + Delimt;
	Line += Client.PhoneNumber + Delimt;
	Line +=to_string(Client.Balance);

	return Line;
}

vector<StClient>LoadClientsDataFromFile(string ClientFileName)
{
	vector<StClient>VClinets;
	fstream MyFile;

	MyFile.open(ClientFileName,ios::in);

	if (MyFile.is_open())
	{
		string Line;

		while (getline(MyFile,Line))
		{
			VClinets.push_back(ReverseLineDataToClinetRecord(Line));
		}
		MyFile.close();
	}

	return VClinets;
}

bool FindClientByAcountNumber(string AcountNumber,string PINCode, StClient &Client)
{
	vector<StClient>VClients=LoadClientsDataFromFile(ClientFileName);

	for (StClient& C : VClients)
	{
		if (C.AcountNumber == AcountNumber&&C.PINCode==PINCode)
		{
			Client = C;
			return true;
		}
	}

	return false;
}

bool LoadClinetInfo(string AcountNumber,string PINCode)
{
	if (FindClientByAcountNumber(AcountNumber,PINCode,CurentClient))
		return true;

	else
	return false;
	
}

void ShowLoginScreen()
{
	cout << "\n---------------------------------------------------\t\t\n";
	cout << "\t\tLogin Screen\t\t\n";
	cout << "---------------------------------------------------\t\t\n";
}

short ReadATMMainMnueOption()
{
	short Option;
	cout << "Choose! What do you want to do[1-5]\n";
	cin >> Option;
	return Option;
}

void GoBackToATMMainMnueScreen()
{
	cout << "Press any Key to go to ATM main mnue screen\n";
 	system("pause>0");

	system("cls");
	ShowATMMainMnueScreen();
}

void SaveClientsToFile(vector<StClient> VClients)
{
	fstream MyFile;

	MyFile.open(ClientFileName, ios::out);

	if (MyFile.is_open())
	{
		for (StClient& C : VClients)
		{
			MyFile<< ReverseClientRecordToLine(C) <<endl;
		}

	}

	MyFile.close();
}

short ReadQuickWithDrawOption()
{
	short Option;

	

	do
	{
		cout << "Choose! What do you want to do [1] to [8]\n";
		cin >> Option;
	} while (Option < 1 || Option>9);


		return Option;

}

short GetQuickWithDrawAmount(short QuickWithOption)
{
	switch (QuickWithOption)
	{
	case 1:
		return 20;

	case 2:
		return 50;

	case 3:
		return 100;

	case 4:
		return 200;

	case 5:
		return 400;
		
	case 6:
		return 600;

	case 7:
		return 800;

	case 8:
		return 1000;

	default:
		return 0;
	}
}

bool DepositBalancToClientByACountNumber(string AcountNumber, int Amount, vector<StClient> &VClients)
{
	char Answer = 'Y';

	cout << "Are you sure to perform this transaction\n";
	cin>>Answer;

	if (toupper(Answer) == 'Y')
	{
	  for (StClient& U : VClients)
	  {
		if (AcountNumber == U.AcountNumber)
		{
			
			U.Balance += Amount;
			SaveClientsToFile(VClients);

			cout << "Done successfuly.New balance is =" << U.Balance << endl;
			return true;
		}
			
	  }
	}
	return false;
}

void PerFormQuiclwithDrawOption(short QuickWithOption)
{
	char Answer = 'Y';

	if (QuickWithOption == 9)
	{
		return;
	}

	short WithDrawAmount = GetQuickWithDrawAmount(QuickWithOption);


	if (WithDrawAmount > CurentClient.Balance)
	{
		cout << "Amount Decceds Your balance,Make another choise\n";
		cout << "Press AnyKey to Continue\n";
		system("pause>0");

		ShowQuickWithDrawScreen();

		return;
	}

	

	vector<StClient> VClients = LoadClientsDataFromFile(ClientFileName);
	DepositBalancToClientByACountNumber(CurentClient.AcountNumber, WithDrawAmount * -1, VClients);
	CurentClient.Balance -= WithDrawAmount;

}

void ShowQuickWithDrawScreen()
{
	
	cout << "\n---------------------------------------------------\t\t\n";
	cout << "\t\tQuick WithDraw Screen\t\t\n";
	cout << "---------------------------------------------------\t\t\n";

	cout << "\t[1] 20\t\t[2] 50\n\n";
	cout << "\t[3] 100\t\t[4] 200\n\n";
	cout << "\t[5] 400\t\t[6] 600\n\n";
	cout << "\t[7] 800\t\t[8] 1000\n\n";
	cout << "\t[9] Exit\n";
	cout << "---------------------------------------------------\t\t\n";

	cout << "Your Balance is:" << CurentClient.Balance << endl;

	PerFormQuiclwithDrawOption(ReadQuickWithDrawOption());
}

int  ReadWithDrawAMount()
{
	int  WithDraw;
	do
	{
		cout << "Please enter Amount Multiple 5 \n";
		cin >> WithDraw;
		
		
	} while(((WithDraw%5)!=0));


	return WithDraw;
}

void PerFormNormalWithDraw()
{
	int WithDrawAmount = ReadWithDrawAMount();
	

	if (WithDrawAmount > CurentClient.Balance)
	{
		cout << "Amount Decceds Your balance,Make another choise\n";
		cout << "Press AnyKey to Continue\n";
		system("pause>0");

		ShowQuickWithDrawScreen();

		return;
	}

	vector<StClient> VClients = LoadClientsDataFromFile(ClientFileName);
	DepositBalancToClientByACountNumber(CurentClient.AcountNumber,WithDrawAmount*-1,VClients);
	CurentClient.Balance-= WithDrawAmount;
}

void ShowNormalWithdrawScreen()
{
	cout << "\n---------------------------------------------------\t\t\n";
	cout << "\t\tNormal WithDraw Screen\t\t\n";
	cout << "---------------------------------------------------\t\t\n";
	PerFormNormalWithDraw();
}

void CheckBalanceScreen()
{
	cout << "\n---------------------------------------------------\t\t\n";
	cout << "\t\tCheck Balance Screen\t\t\n";
	cout << "---------------------------------------------------\t\t\n";

	cout << "Your blance is: " << CurentClient.Balance << endl;
}

int ReadAmountToDeposit()
{
	int Amount = 0;

	cout << "Please enter positive Amount\n";
	cin >> Amount;

	while (Amount < 0)
	{
	cout << "Please enter positive Amount\n";
	cin >> Amount;
	}
	return Amount;
}

void PerFormDepositOption()
{
	int Amount =ReadAmountToDeposit();
	

		vector<StClient> VClients = LoadClientsDataFromFile(ClientFileName);

		DepositBalancToClientByACountNumber(CurentClient.AcountNumber, Amount, VClients);
		CurentClient.Balance += Amount;
	
}

void ShowDepositScreen()
{
	cout << "\n---------------------------------------------------\t\t\n";
	cout << "\t\t Deposit Screen\t\t\n";
	cout << "---------------------------------------------------\t\t\n";

	PerFormDepositOption();
}

void PerFormainATMMainMnueOption(EnATMMainMnueOption Option)
{

	switch (Option)
	{
	
	case EnATMMainMnueOption::EQuickWithDarw:
		system("cls");
		
		ShowQuickWithDrawScreen();
		GoBackToATMMainMnueScreen();

		break;

	case EnATMMainMnueOption::ENormalWithdraw:
		system("cls");

		ShowNormalWithdrawScreen();
		GoBackToATMMainMnueScreen();

		break;
	
	case EnATMMainMnueOption::EDeposit:
		system("cls");

		ShowDepositScreen();
		GoBackToATMMainMnueScreen();

		break;

	case EnATMMainMnueOption::ECheckBalance:
		system("cls");

		CheckBalanceScreen();
		GoBackToATMMainMnueScreen();

		break;

	case EnATMMainMnueOption::ELogout:
		system("cls");

		Login();

		break;

	}

}

void ShowATMMainMnueScreen()
{
	system("cls");
	cout << "\n---------------------------------------------------\t\t\n";
	cout << "\t\tATM Main Mnue Screen\t\t\n";
	cout << "---------------------------------------------------\t\t\n";

	cout << "\t[1] Quick WithDraw\n";
	cout << "\t[2] Normal WithDraw\n";
	cout << "\t[3] Deposit\n";
	cout << "\t[4] Check Balance\n";
	cout << "\t[5] Logout\n";
	cout << "\n---------------------------------------------------\n";
	
	PerFormainATMMainMnueOption(EnATMMainMnueOption(ReadATMMainMnueOption()));
}

void Login()
{
	string AcountNumber;
	string PINCode;
	bool ClientFailed = false;
	
	

	  do
	  {
		system("cls");

		ShowLoginScreen();

		if (ClientFailed)
		{
			cout << "Invaild AcountNumber/PIN\n";
		}

		cout << "Please enter ACountNumber\n";
		cin >> AcountNumber;

		cout << "Please enter Pincode\n";
		cin >> PINCode;


		ClientFailed = !(LoadClinetInfo(AcountNumber,PINCode));

	  } while(ClientFailed);

	ShowATMMainMnueScreen();
}

int main()
{
	Login();


	system("pause>0");
	return 0;
}