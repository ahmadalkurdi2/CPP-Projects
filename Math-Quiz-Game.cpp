#include<iostream>

using namespace std;

enum enQuestionLevel
{
	Easy = 1, Medium, Hard, Mix
};

enum enOperationType
{
	Add = 1, Subtract, Multiply, Divide, MixOperation
};

struct stQuestion
{
	int Number1{ 0 }, Number2{ 0 }, CorrectAnswer{ 0 }, PlayerAnswer{ 0 };
	enQuestionLevel QuestionLevel;
	enOperationType OperationType;
	bool AnswerResult{ false };
};

struct stQuiz
{
	stQuestion QuestionList[100];
	short NumberOfQuesions{ 0 }, NumberOfRightAnswers{ 0 }, NumberOfWrongAnswers{ 0 };
	enQuestionLevel QuestionLevel;
	enOperationType OperationType;
	bool IsPassed{ false };
};

void ResetScreen()
{
	system("cls");
	system("color 03");
}

short ReadNumberOfQuestions()
{
	short NumberOfQuestions{ 0 };
	cout << "How many questions do you want to answer? (1-10): ";
	while (!(cin >> NumberOfQuestions) || NumberOfQuestions < 1 || NumberOfQuestions>10)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid number, try again: ";
		cout << "\n\nHow many questions do you want to answer? ";
	}
	return NumberOfQuestions;
}

enQuestionLevel ReadQuestionLevel()
{
	short QuestionLevel{ 0 };
	cout << "Enter the question level (1-Easy, 2-Medium, 3-Hard, 4-Mix): ";
	while (!(cin >> QuestionLevel) || QuestionLevel < 1 || QuestionLevel>4)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid number, try again: ";
		cout << "\n\nEnter the question level (1-Easy, 2-Medium, 3-Hard, 4-Mix): ";
	}
	return static_cast<enQuestionLevel>(QuestionLevel);
}

enOperationType ReadOperationType()
{
	short OperationType{ 0 };
	cout << "Enter the operation type (1-Add, 2-Subtract, 3-Multiply, 4-Divide, 5-Mix): ";
	while (!(cin >> OperationType) || OperationType < 1 || OperationType>5)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid number, try again: ";
		cout << "\n\nEnter the operation type (1-Add, 2-Subtract, 3-Multiply, 4-Divide, 5-Mix): ";
	}
	return static_cast<enOperationType>(OperationType);
}

int RandomNumber(int From, int To)
{
	return rand() % (To - From + 1) + From;
}

int CalculateAnswer(int Number1, int Number2, enOperationType OperationType)
{
	switch (OperationType)
	{
	case enOperationType::Add:
		return Number1 + Number2;
	case enOperationType::Subtract:
		return Number1 - Number2;
	case enOperationType::Multiply:
		return Number1 * Number2;
	case enOperationType::Divide:
		if (Number2 == 0)
			throw runtime_error("Division by zero is not allowed");
		return Number1 / Number2;
	default:
		return Number1 + Number2;
	}
}

stQuestion GenerateQuestion(enQuestionLevel QuestionLevel, enOperationType OperationType)
{
	stQuestion Question;
	if (QuestionLevel == enQuestionLevel::Mix)
		QuestionLevel = enQuestionLevel(RandomNumber(1, 3));
	if (OperationType == enOperationType::MixOperation)
		OperationType = enOperationType(RandomNumber(1, 4));
	Question.OperationType = OperationType;

	switch (QuestionLevel)
	{
	case enQuestionLevel::Easy:
		Question.Number1 = RandomNumber(1, 9);
		Question.Number2 = RandomNumber(1, 9);
		break;
	case enQuestionLevel::Medium:
		Question.Number1 = RandomNumber(10, 99);
		Question.Number2 = RandomNumber(10, 99);
		break;
	case enQuestionLevel::Hard:
		Question.Number1 = RandomNumber(100, 999);
		Question.Number2 = RandomNumber(100, 999);
		break;
	default:
		break;
	}
	if (OperationType == enOperationType::Divide && Question.Number2 == 0)
		Question.Number2 = 1;
	Question.CorrectAnswer = CalculateAnswer(Question.Number1, Question.Number2, Question.OperationType);
	Question.QuestionLevel = QuestionLevel;
	return Question;
}

void GeneratQuizQuestions(stQuiz& Quiz)
{
	for (short Question{ 0 }; Question < Quiz.NumberOfQuesions; Question++)
		Quiz.QuestionList[Question] = GenerateQuestion(Quiz.QuestionLevel, Quiz.OperationType);
}

string GetOperationTypeSymbol(enOperationType OperationType)
{
	string OpType[]{ "+","-","*","/","Mix" };
	return OpType[OperationType - 1];
}

void PrintQuestion(stQuiz& Quiz, short QuestionNumber)
{
	cout << "\nQuestion [" << QuestionNumber + 1 << "/" << Quiz.NumberOfQuesions << "]\n";
	cout << "Question [" << QuestionNumber + 1 << "]: " << Quiz.QuestionList[QuestionNumber].Number1 << " ";
	cout << GetOperationTypeSymbol(Quiz.QuestionList[QuestionNumber].OperationType) << " ";
	cout << Quiz.QuestionList[QuestionNumber].Number2 << "\n_________\n";
}

int ReadPlayerAnswer()
{
	int PlayerAnswer{ 0 };
	cout << "Enter your answer: ";
	while (!(cin >> PlayerAnswer))
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid number, try again: ";
		cout << "\n\nEnter your answer: ";
	}
	return PlayerAnswer;
}

void SetScreenColor(bool AnswerResult)
{
	if (AnswerResult)
		system("color 02");
	else
	{
		system("color 04");
		cout << "\a";
	}
}

void CorrectTheQuestionAnswer(stQuiz& Quiz, short QuestionNumber)
{
	if (Quiz.QuestionList[QuestionNumber].PlayerAnswer != Quiz.QuestionList[QuestionNumber].CorrectAnswer)
	{
		Quiz.QuestionList[QuestionNumber].AnswerResult = false;
		Quiz.NumberOfWrongAnswers++;
		cout << "\nWrong Answer, the correct answer is: " << Quiz.QuestionList[QuestionNumber].CorrectAnswer << "\n";
	}
	else
	{
		Quiz.QuestionList[QuestionNumber].AnswerResult = true;
		Quiz.NumberOfRightAnswers++;
		cout << "\nCorrect Answer, well done!\n";
	}
	SetScreenColor(Quiz.QuestionList[QuestionNumber].AnswerResult);
}

void AskAndCorrectQuestionListAnswers(stQuiz& Quiz)
{
	for (short QuestionNumber{ 0 }; QuestionNumber < Quiz.NumberOfQuesions; QuestionNumber++)
	{
		PrintQuestion(Quiz, QuestionNumber);
		Quiz.QuestionList[QuestionNumber].PlayerAnswer = ReadPlayerAnswer();
		CorrectTheQuestionAnswer(Quiz, QuestionNumber);
	}
}

string GetFinalResultText(bool IsPassed)
{
	return IsPassed ? "Passed" : "Failed";
}

string GetQuestionLevelText(enQuestionLevel QuestionLevel)
{
	string arrQuestionLevel[]{ "Easy","Medium","Hard","Mix" };
	return arrQuestionLevel[QuestionLevel - 1];
}

void PrintQuizResult(stQuiz& Quiz)
{
	Quiz.IsPassed = Quiz.NumberOfRightAnswers > Quiz.NumberOfWrongAnswers;
	cout << "Final Result: " << GetFinalResultText(Quiz.IsPassed) << "\n";
	cout << "______________________________\n";
	cout << "Number of Questions: " << Quiz.NumberOfQuesions << endl;
	cout << "Question Level: " << GetQuestionLevelText(Quiz.QuestionLevel) << endl;
	cout << "Operation Type: " << GetOperationTypeSymbol(Quiz.OperationType) << endl;
	cout << "Number of Right Answers: " << Quiz.NumberOfRightAnswers << endl;
	cout << "Number of Wrong Answers: " << Quiz.NumberOfWrongAnswers << endl;
	cout << "______________________________\n";
	SetScreenColor(Quiz.IsPassed);
}

void PlayMathGame()
{
	stQuiz Quiz;
	Quiz.NumberOfQuesions = ReadNumberOfQuestions();
	Quiz.QuestionLevel = ReadQuestionLevel();
	Quiz.OperationType = ReadOperationType();
	GeneratQuizQuestions(Quiz);
	AskAndCorrectQuestionListAnswers(Quiz);
	PrintQuizResult(Quiz);
}

void startGame()
{
	char PlayAgain{ 'y' };
	do
	{
		ResetScreen();
		PlayMathGame();
		cout << "Do you want to play again? (Y/N): ";
		cin >> PlayAgain;
	} while (toupper(PlayAgain) == 'Y');
}

int main()
{
	srand(unsigned(time(NULL)));
	startGame();
}