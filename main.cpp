#include <algorithm>
#include <iostream>
#include <string>
#include "FBullsAndCows.h"


using FText = std::string;

void PlayGame();
EWordList GetWordList();
EDifficulty GetDifficulty();
FText GetInput();
FText GetValidGuess();
int GetChoice();
void PrintResults(const FText& Guess);
bool IsPlayingAgain();

FBullsAndCows BullCowGame;

int main()
{
	std::cout << R"(   /             \            .        .
  ((__-^^-,-^^-__))           \'.____.'/
   `-_---' `---_-'           __'-.  .-'__
    <__|o` 'o|__>   Bulls    '_i:'oo':i_'
       \  `  /        &        /._  _.\
        ): :(       Cows      (  o  o  )
        :o_o:                  '-.__.-'
         "-")";
	std::cout << "\n\n\t   Welcome to Bulls and Cows.\n";
	std::cout << "The aim of the game is to guess the word.\n";
	std::cout << "Bulls mean you have a correct letter in the correct place.\n";
	std::cout << "Cows mean you have a correct letter in the wrong place.\n";
	std::cout << "Got it? Okay, lets play!\n\n";
	do
		PlayGame();
	while (IsPlayingAgain());
	return 0;
}


void PlayGame()
{
	BullCowGame.SetWordList(GetWordList());
	BullCowGame.SetGameDifficulty(GetDifficulty());

	int MaxGuesses{ BullCowGame.GetMaxGuesses() };
	std::cout << "You have to guess a " << BullCowGame.GetHiddenWordLength() << " letter word.\n";
	std::cout << "You have " << MaxGuesses << " guesses to guess the word.\n\n";
	for (int CurrentTry = 0; CurrentTry < MaxGuesses; ++CurrentTry)
	{
		FText Guess{ GetValidGuess() };
		if (Guess == BullCowGame.GetHiddenWord())
		{
			std::cout << "You got it!" << std::endl;
			return;
		}
		else
			PrintResults(Guess);
		std::cout << "Guesses remaining: " << MaxGuesses - CurrentTry - 1 << "\n\n";
	}
	std::cout << "Sadly you failed to guess the word. Better luck next time.\nThe word was ";
	std::cout << BullCowGame.GetHiddenWord() << "\n\n";
}

EWordList GetWordList()
{
	std::cout << "What kind of word lengths would you like to guess?\n";
	std::cout << "1. Short\n2. Medium\n3. Long\n(Pick a number)" << std::endl;
	int input{ GetChoice() };
	switch (input)
	{
	case 1:
		return EWordList::Short;
	case 2:
		return EWordList::Medium;
	case 3:
		return EWordList::Long;
	}
	return EWordList::Medium;
}

EDifficulty GetDifficulty()
{
	std::cout << "What difficulty would you like to play?\n";
	std::cout << "1. Easy\n2. Normal\n3. Hard\n(Pick a number)" << std::endl;
	int input{ GetChoice() };
	switch (input)
	{
	case 1:
		return EDifficulty::Easy;
	case 2:
		return EDifficulty::Normal;
	case 3:
		return EDifficulty::Hard;
	default:
		return EDifficulty::Normal;
	}
}

FText GetInput()
{
	FText Guess;
	std::getline(std::cin, Guess);
	//transform Guess to lowercase
	std::transform(Guess.begin(), Guess.end(), Guess.begin(), tolower);
	return Guess;
}

FText GetValidGuess()
{
	FText Guess;
	EValidity bValidGuess = EValidity::Invalid;
	//hard setting uses up a guess on invalid input
	//might actually make it easier
	do
	{
		std::cout << "Enter your guess: ";
		Guess = GetInput();
		bValidGuess = BullCowGame.IsValidGuess(Guess);
		switch (bValidGuess)
		{
		case EValidity::NotIsogram:
			std::cout << Guess << " is not an isogram. Try again.\n\n";
			break;
		case EValidity::IncorrectSize:
			std::cout << Guess << " is not a " << BullCowGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		default:
			break;
		}
	} while (bValidGuess != EValidity::OK && BullCowGame.GetCurrentDifficulty() != EDifficulty::Hard);
	return Guess;
}
//Get an input from 1 - 3
int GetChoice()
{
	int input{ 0 };
	//Makes sure to get a valid input
	while (!(std::cin >> input) || input < 1 || input > 3)
	{
		std::cout << "Error, incorrect input\nOnly enter 1, 2 or 3." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	std::cout << std::endl;
	//Don't bleed into next inputs
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return input;
}

void PrintResults(const FText& Guess)
{
	if (BullCowGame.GetCurrentDifficulty() == EDifficulty::Easy)
	{
		auto Results{ BullCowGame.GetEasyResults(Guess) };
		std::cout << "Bulls: " << Results.first << "\n";
		std::cout << "Cows: " << Results.second << "\n";
	}
	else
	{
		auto Results{ BullCowGame.GetResults(Guess) };
		std::cout << "Bull count: " << Results.first << '\n';
		std::cout << "Cow count: " << Results.second << '\n';
	}
}

bool IsPlayingAgain()
{
	std::cout << "Do you want to play again(y/n)? ";
	char Response{ 'a' };
	while (!(std::cin >> Response) || tolower(Response) != 'y' && tolower(Response) != 'n')
	{
		std::cout << "\nInvalid input\nDo you want to play again(y/n)? ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << '\n';
	return tolower(Response) == 'y';
}
