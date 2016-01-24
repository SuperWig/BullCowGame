#pragma once
#include <map>
#include <random>
#include <string>
#include <vector>

#define TArray std::vector
#define TMap std::map
using FString = std::string;

enum class EDifficulty { Easy, Normal, Hard };
enum class EWordList { Short, Medium, Long };
enum class EValidity { Invalid, NotIsogram, IncorrectSize, OK};


class FBullsAndCows
{
	std::mt19937 gen{ std::random_device{ }() };
public:
	FBullsAndCows();

	void SetWordList(EWordList WordList);
	void SetGameDifficulty(EDifficulty Difficulty);
	void GenerateHiddenWord();

	FString GetHiddenWord() const;
	int GetHiddenWordLength() const;
	int GetMaxGuesses() const;
	int CalculateScore() const; //TODO move all score handling in class
	int CalculateScore(int CurrentTry) const;
	EDifficulty GetCurrentDifficulty() const;
	EValidity IsValidGuess(const FString& Guess);
	std::pair<int, int> GetResults(const FString& Guess) const;
	std::pair<FString, FString> GetEasyResults(const FString& Guess) const;
private:
	TArray<FString> GetWordList(const std::pair<int,int> limits);
	int GenerateRandomNumber(size_t ListSize);
	bool IsIsogram(const FString& Word) const;
	int CalculateMaxGuessess() const;

	const TMap<const EDifficulty,const double> MultiplierMap
	{ {EDifficulty::Easy, 1.3},{EDifficulty::Normal, 1.0},{EDifficulty::Hard, 0.7} };
	const TMap<const EWordList, std::pair<int, int>> LengthMap
	{ {EWordList::Short, {3,4} }, {EWordList::Medium, {4,7}}, {EWordList::Long, {7,26}} };

	EDifficulty CurrentDifficulty;
	EWordList CurrentWordList;
	FString HiddenWord;
	TArray<FString> WordList;
	int MaxGuesses{ 0 };
};
