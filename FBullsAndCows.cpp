#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include "FBullsAndCows.h"

#define TSet std::unordered_set

void FBullsAndCows::GenerateHiddenWord() { HiddenWord = WordList[GenerateRandomNumber(WordList.size())]; }
FString FBullsAndCows::GetHiddenWord() const { return HiddenWord; }
int FBullsAndCows::GetHiddenWordLength() const { return HiddenWord.size(); }
int FBullsAndCows::GetMaxGuesses() const { return MaxGuesses; }
EDifficulty FBullsAndCows::GetCurrentDifficulty() const { return CurrentDifficulty; }

FBullsAndCows::FBullsAndCows()
{
	CurrentDifficulty = EDifficulty::Easy;
	CurrentWordList = EWordList::Medium;
	auto limits{ LengthMap.at(CurrentWordList) };
	//Make sure to populate the wordlist
	WordList = GetWordList(limits);
}

//Generates a random number within the bounds of ListSize
int FBullsAndCows::GenerateRandomNumber(size_t ListSize) 
{
	std::uniform_int_distribution<int> dist(0, ListSize - 1);
	return dist(gen);
}

void FBullsAndCows::SetWordList(EWordList WordListLength)
{
	//dont repopulate the wordlist if the selected wordlist is the same as the current one
	if (WordListLength != CurrentWordList)
	{
		//gets the upper and lower bound for the word length at the requested lengths
		auto limits{ LengthMap.at(WordListLength) };
		CurrentWordList = WordListLength;
	}
}

void FBullsAndCows::SetGameDifficulty(EDifficulty Difficulty)
{
	CurrentDifficulty = Difficulty;
	GenerateHiddenWord();
	MaxGuesses = CalculateMaxGuessess();
}

int FBullsAndCows::CalculateMaxGuessess() const
{
	//looks up the map to get the multiplier at the current difficulty
	double Multiplier{ MultiplierMap.at(GetCurrentDifficulty())};
	//Using Ben's scaling * multiplier; rounded -- Probably needs tweaking
	return static_cast<int>(std::round((4.1*GetHiddenWordLength() - 9.1)*Multiplier));
}

int FBullsAndCows::CalculateScore() const
{
	return CalculateScore(GetMaxGuesses());
}

int FBullsAndCows::CalculateScore(int CurrentTry) const
{
	//looks up the map to get the multiplier at the current difficulty
	double Multiplier{ MultiplierMap.at(GetCurrentDifficulty()) };
	//use the length + remainder of tries as a base
	int Base{ GetHiddenWordLength() + (MaxGuesses - CurrentTry) };
	return static_cast<int>(std::round(Base / Multiplier));
}

bool FBullsAndCows::IsIsogram(const FString& Word) const
{
	//check for duplicate letters or any spaces
	TSet<char> SeenLetters;
	for (auto c : Word)
		if (!SeenLetters.insert(c).second || c == ' ')
			return false;
	return true;
}

EValidity FBullsAndCows::IsValidGuess(const FString& Guess)
{
	if (Guess.size() != GetHiddenWordLength())
		return EValidity::IncorrectSize;
	if (!IsIsogram(Guess))
		return EValidity::NotIsogram;
	return EValidity::OK;
}

std::pair<int, int> FBullsAndCows::GetResults(const FString& Guess) const
{
	int BullCount{ 0 };
	int CowCount{ 0 };
	for (auto i = Guess.begin(); i != Guess.end(); ++i)
	{
		//find the current letter i from Guess in HiddenWord
		auto found = std::find(HiddenWord.begin(), HiddenWord.end(), *i);
		if (found != HiddenWord.end())
		{
			//if the position of the found letter and i are the same
			if (std::distance(Guess.begin(), i) == std::distance(HiddenWord.begin(), found))
				++BullCount;
			else
				++CowCount;
		}
	}
	return std::make_pair(BullCount, CowCount);
}

std::pair<FString, FString> FBullsAndCows::GetEasyResults(const FString & Guess) const
{
	FString Bulls;
	FString Cows;
	for (auto i = Guess.begin(); i != Guess.end(); ++i)
	{
		//find the current letter i from Guess in HiddenWord
		auto found = std::find(HiddenWord.begin(), HiddenWord.end(), *i);
		if (found != HiddenWord.end())
		{
			//if the position of the found letter and i are the same
			//append the letter to the appropriate string
			if (std::distance(Guess.begin(), i) == std::distance(HiddenWord.begin(), found))
				Bulls.append(FString{ *i } + ",");
			else
				Cows.append(FString{ *found } + ",");
		}
	}
	//remove trailing commas
	if(!Bulls.empty())
		Bulls.pop_back();
	if(!Cows.empty())
		Cows.pop_back();
	return std::make_pair(Bulls, Cows);
}

TArray<FString> FBullsAndCows::GetWordList(const std::pair<int,int> limits)
{
	int LowerBound = limits.first;
	int UpperBound = limits.second;
	TArray<FString> List;
	std::ifstream File("isograms.txt");
	if (File.is_open())
	{
		for (FString Line; std::getline(File, Line); )
		{
			//transform to lowercase and make sure it's an isogram
			std::transform(Line.begin(), Line.end(), Line.begin(), tolower);
			if(IsIsogram(Line) && LowerBound <= Line.size() && Line.size() <= UpperBound)
				List.push_back(Line);
		}
	}
	File.close();
	return List;
}
