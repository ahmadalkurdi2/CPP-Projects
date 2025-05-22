#include<iostream>

using namespace std;

enum enWinner { Player1 = 1, Computer, Draw };
enum enGameChoice { Stone = 1, Paper, Scissors };

struct stGameResults
{
	short GameRounds{ 0 }, Player1WinTimes{ 0 }, ComputerWinTimes{ 0 }, DrawTimes{ 0 };
	enWinner GameWinner;
	string GameWinnerName;
};

struct stRoundInfo
{
	short RoundNumber{ 0 };
	enGameChoice Player1Choice, ComputerChoice;
	enWinner RoundWinner;
	string RoundWinnerName;
};

int RandomNumber(int From, int To)
{
	return rand() % (To - From + 1) + From;
}

void ResetScreen()
{
	system("cls");
	system("color 09");
}

string Tabs(short NumberOfTabs)
{
	string Tab;
	for (short i{ 0 }; i < NumberOfTabs; i++)
		Tab += "\t";
	return Tab;
}

short ReadHowManyRounds()
{
	short GameRounds{ 0 };
	cout << "How many rounds do you want to play? ";
	while (!(cin >> GameRounds) || GameRounds < 1 || GameRounds>10)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input. Please enter a number between 1 and 10: ";
	}
	return GameRounds;
}

enGameChoice ReadPlayer1Choice()
{
	short Choice{ 0 };
	cout << "\nYour Choice: \[1]\:Stone, \[2]\:Paper, \[3]\:Scissors? ";
	while (!(cin >> Choice) || Choice < 1 || Choice>3)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input. Please enter a number between 1 and 3: ";
	}
	return static_cast<enGameChoice>(Choice);
}

enGameChoice GetComputerChoice()
{
	return static_cast<enGameChoice>(RandomNumber(1, 3));
}

enWinner WhoWonTheRound(stRoundInfo& RoundInfo)
{
	if (RoundInfo.Player1Choice == RoundInfo.ComputerChoice)
		return enWinner::Draw;
	switch (RoundInfo.Player1Choice)
	{
	case enGameChoice::Stone:
		if (RoundInfo.ComputerChoice == enGameChoice::Paper)
			return enWinner::Computer;
		break;
	case enGameChoice::Paper:
		if (RoundInfo.ComputerChoice == enGameChoice::Scissors)
			return enWinner::Computer;
		break;
	case enGameChoice::Scissors:
		if (RoundInfo.ComputerChoice == enGameChoice::Stone)
			return enWinner::Computer;
		break;
	}
	return enWinner::Player1;
}

string WinnerName(enWinner Winner)
{
	string WinnerName[] { "Player1", "Computer", "No Winner" };
	return WinnerName[Winner - 1];
}

string ChoiceName(enGameChoice Choice)
{
	string RoundChoice[] { "Stone", "Paper", "Scissors" };
	return RoundChoice[Choice - 1];
}

void SetWinnerScreenColor(enWinner Winner)
{
	switch (Winner)
	{
	case enWinner::Player1:
		system("color 0A");
		break;
	case enWinner::Computer:
		system("color 0C");
		cout << "\a";
		break;
	default:
		system("color 06");
		break;
	}
}

void PrintRoundResults(stRoundInfo& RoundInfo)
{
	cout << "\n\_\_\_\_\_\_\_\_\_\_\_\_Round \[" << RoundInfo.RoundNumber << "]\_\_\_\_\_\_\_\_\_\_**\n\n";
	cout << "Player1 Choice: " << ChoiceName(RoundInfo.Player1Choice) << "\n";
	cout << "Computer Choice: " << ChoiceName(RoundInfo.ComputerChoice) << "\n";
	cout << "Round Winner: \[" << RoundInfo.RoundWinnerName << "]\n";
	cout << "**\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\n" << endl;
	SetWinnerScreenColor(RoundInfo.RoundWinner);
}

enWinner WhoWinTheGame(short Player1WinTimes, short ComputerWinTimes)
{
	if (Player1WinTimes > ComputerWinTimes)
		return enWinner::Player1;
	else if (Player1WinTimes < ComputerWinTimes)
		return enWinner::Computer;
	else
		return enWinner::Draw;
}

stGameResults FillGameResults(short HowManyRounds, short Player1WinTimes, short ComputerWinTimes, short DrawTimes)
{
	stGameResults GameResults;
	GameResults.GameRounds = HowManyRounds;
	GameResults.Player1WinTimes = Player1WinTimes;
	GameResults.ComputerWinTimes = ComputerWinTimes;
	GameResults.DrawTimes = DrawTimes;
	GameResults.GameWinner = WhoWinTheGame(Player1WinTimes, ComputerWinTimes);
	GameResults.GameWinnerName = WinnerName(GameResults.GameWinner);
	return GameResults;
}

stGameResults PlayGame(short HowManyRounds)
{
	stRoundInfo RoundInfo;
	short Player1WinTimes{ 0 }, ComputerWinTimes{ 0 }, DrawTimes{ 0 };
	for (short GameRound{ 1 }; GameRound <= HowManyRounds; GameRound++)
	{
		cout << "\nRound \[" << GameRound << "] begins: \n";
		RoundInfo.RoundNumber = GameRound;
		RoundInfo.Player1Choice = ReadPlayer1Choice();
		RoundInfo.ComputerChoice = GetComputerChoice();
		RoundInfo.RoundWinner = WhoWonTheRound(RoundInfo);
		RoundInfo.RoundWinnerName = WinnerName(RoundInfo.RoundWinner);

			if (RoundInfo.RoundWinner == enWinner::Player1)
				Player1WinTimes++;
			else if (RoundInfo.RoundWinner == enWinner::Computer)
				ComputerWinTimes++;
			else
				DrawTimes++;
		PrintRoundResults(RoundInfo);
	}
	return FillGameResults(HowManyRounds, Player1WinTimes, ComputerWinTimes, DrawTimes);

}

void ShowGameOverScreen()
{
	cout << Tabs(5) << "=====================================\n";
	cout << Tabs(5) << "            GAME OVER                \n";
	cout << Tabs(5) << "=============================================\n";
	cout << Tabs(5) << "Thank you for playing! See you next time!\n";
	cout << Tabs(5) << "=============================================\n\n";
}

void ShowFinalGameResults(stGameResults GameResults)
{
	cout << Tabs(5) << "\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ \[Game Results]\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_****\n\n";
	cout << Tabs(5) << "Game Rounds : " <<
		GameResults.GameRounds << endl;
	cout << Tabs(5) << "Player1 won times : " <<
		GameResults.Player1WinTimes << endl;
	cout << Tabs(5) << "Computer won times : " <<
		GameResults.ComputerWinTimes << endl;
	cout << Tabs(5) << "Draw times : " <<
		GameResults.DrawTimes << endl;
	cout << Tabs(5) << "Final Winner : " <<
		GameResults.GameWinnerName << endl;
	cout << Tabs(5) << "****\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\n";
	SetWinnerScreenColor(GameResults.GameWinner);
}

void StartGame()
{
	char PlayAgain{ 'y' };
	do
	{
		ResetScreen();
		stGameResults GameResults = PlayGame(ReadHowManyRounds());
		ShowGameOverScreen();
		ShowFinalGameResults(GameResults);
		cout << endl << Tabs(5) << "Do you want to play again? Y/N? ";
		cin >> PlayAgain;
	} while (toupper(PlayAgain) == 'Y');
}

int main()
{
	srand(unsigned(time(NULL)));
	StartGame();
}
