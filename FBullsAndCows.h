#pragma once
#include <string>
#include <vector>

#define TArray std::vector
using FString = std::string;

enum class EDifficulty { Easy, Normal, Hard };
enum class EWordList { Short, Medium, Long };
enum class EValidity { Invalid, NotIsogram, IncorrectSize, OK};

class FBullsAndCows
{
public:
	FBullsAndCows();

	void SetWordList(EWordList WordList);
	void SetGameDifficulty(EDifficulty Difficulty);
	void GenerateHiddenWord();

	FString GetHiddenWord() const;
	int GetHiddenWordLength() const;
	int GetMaxGuesses() const;
	int CalculateMaxGuessess() const;
	EDifficulty GetCurrentDifficulty() const;
	EValidity IsValidGuess(const FString& Guess);
	std::pair<int, int> GetResults(const FString& Guess) const;
	std::pair<FString, FString> GetEasyResults(const FString& Guess) const;

private:
	TArray<FString> GetWordList(const FString& FileName);
	int GenerateRandomNumber(size_t ListSize) const;
	bool IsIsogram(const FString& Word) const;

	//most likely not balanced in the slightest
	static constexpr double M_EASY{ 1.3 };
	static constexpr double M_NORMAL{ 1 };
	static constexpr double M_HARD{ 0.7 };

	EDifficulty CurrentDifficulty;
	EWordList CurrentWordList;
	FString HiddenWord;
	TArray<FString> WordList;
	int MaxGuesses;
};
