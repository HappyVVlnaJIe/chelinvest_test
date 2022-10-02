#include "converter.h"
#include <vector>
#include <fstream>
#include <algorithm>

void NumberConverter::LoadUnitsConfigs()
{
	std::ifstream configs(units_configs_path);
	units.resize(10);
	for (int digit = 1; digit < 10; digit++)
	{
		std::string line;
		units[digit].resize(cases_count);
		for (int case_idx = 0; case_idx < cases_count; case_idx++)
		{
			std::getline(configs, line);
			int word_start = 0, word_end = 0;
			for (int gender_idx = 0; gender_idx < 3; gender_idx++)
			{
				while (line[word_end] != separator) { word_end++; }

				units[digit][case_idx].push_back(line.substr(word_start, word_end - word_start));  
				word_end++;
				word_start = word_end;
			}
		}
		std::getline(configs, line); //пустая строка разделяет таблицы цифр
	}

	configs.close();
}

void NumberConverter::LoadDozensAndHundredsConfigs()
{
	for (int part_idx = 1; part_idx < 3; part_idx++)
	{
		PartsOfNumber cur_part = static_cast<PartsOfNumber>(part_idx);
		std::string cur_configs_path;
		switch (cur_part)
		{
		case PartsOfNumber::Dozens:
			cur_configs_path = dozens_configs_path;
			dozens.resize(10);
			break;

		case PartsOfNumber::Hundreds:
			cur_configs_path = hundreds_configs_path;
			hundreds.resize(10);
			break;
		}

		std::ifstream configs(cur_configs_path);
		std::string word;
		for (int digit = 1; digit < 10; digit++)
		{
			if (part_idx == 1 && digit == 1)
			{
				continue; //т.к. 10 - 19 исключения
			}
			for (int case_idx = 0; case_idx < cases_count; case_idx++)
			{
				configs >> word;
				switch (cur_part)
				{
				case PartsOfNumber::Dozens:
					dozens[digit].push_back(word); 
						break;

				case PartsOfNumber::Hundreds:
					hundreds[digit].push_back(word); 
					break;
				}
			}
			std::getline(configs, word); //пустая строка разделяет таблицы цифр
		}
		
		configs.close();
	}
}

void NumberConverter::LoadExceptionNumbersConfigs()
{
	std::ifstream configs(exception_numbers_configs_path);
	int number;
	std::string word;
	while (!configs.eof())
	{
		configs >> number;
		for (int case_idx = 0; case_idx < cases_count; case_idx++)
		{
			configs >> word;
			exception_numbers[number].push_back(word); 
		}
	}

	configs.close();
}

void NumberConverter::LoadKeyWordsConfigs()
{
	std::ifstream configs(key_words_configs_path);

	ThousandsOfNumber cur_state = ThousandsOfNumber::Thousand;
	thousands.resize(key_words_types_count);
	millions.resize(key_words_types_count);
	billions.resize(key_words_types_count);

	while (!configs.eof())
	{
		for (int i = 0; i < key_words_types_count; i++)
		{
			std::string word;
			std::getline(configs, word); // т.к. первым идёт число, для которого прописано склонение
			for (int case_idx = 0; case_idx < cases_count; case_idx++)
			{
				std::getline(configs, word);
				switch (cur_state)
				{
				case ThousandsOfNumber::Thousand:
					thousands[i].push_back(word);
						break;

				case ThousandsOfNumber::Million:
					millions[i].push_back(word);
					break;

				case ThousandsOfNumber::Billion:
					billions[i].push_back(word);
					break;
				}
			}
		}
		int cur_state_idx = static_cast<int>(cur_state);
		cur_state = static_cast<ThousandsOfNumber>(++cur_state_idx);
	}

	configs.close();
}

NumberConverter::NumberConverter()
{
	LoadUnitsConfigs();
	LoadDozensAndHundredsConfigs();
	LoadExceptionNumbersConfigs();
	LoadKeyWordsConfigs();
}

std::string NumberConverter::sumProp(long long nSum, std::string sGender, std::string sCase)
{
	int number = 0, counter = 1;
	std::string result = "";
	ThousandsOfNumber cur_state = ThousandsOfNumber::Thousand;
	bool is_negative = false;
	auto gender_iterator = std::find(genders.begin(), genders.end(), sGender);
	auto case_iterator = std::find(cases.begin(), cases.end(), sCase);
	
	if (gender_iterator == genders.end())
	{
		return "";
	}
	if (case_iterator == cases.end())
	{
		return "";
	}
	if (nSum > MAX_VALUE)
	{
		return "";
	}

	int gender_idx = gender_iterator - genders.begin();
	int case_idx = case_iterator - cases.begin();
	if (nSum == 0)
	{
		return sumThree(nSum, male_idx, case_idx);									
	}
	if (nSum < 0)
	{
		is_negative = true;
		nSum = abs(nSum);
	}

	while (nSum > 0)
	{
		number += nSum % 10 * counter;
		counter *= 10;
		nSum /= 10;
		if (counter == 1000 || nSum == 0)
		{
			std::string thousands = AddThousandsWord(number, case_idx, cur_state);
			switch (cur_state)
			{
			case ThousandsOfNumber::Thousand:
				result = (sumThree(number, gender_idx, case_idx));
				cur_state = ThousandsOfNumber::Million;
				break;

			case ThousandsOfNumber::Million:
				result = (sumThree(number, female_idx, case_idx)) + " " + thousands + " " + result;
				cur_state = ThousandsOfNumber::Billion;
				break;
				
			case ThousandsOfNumber::Billion:
				result = (sumThree(number, male_idx, case_idx)) + " " + thousands + " " + result;
				cur_state = ThousandsOfNumber::Trillion;
				break;

			case ThousandsOfNumber::Trillion:
				result = (sumThree(number, male_idx, case_idx)) + " " + thousands + " " + result;
				break;
			}
			counter = 1;
			number = 0;
		}
	}

	if (is_negative)
	{
		result = "минус " + result;
	}
	return result;
}

std::string NumberConverter::sumThree(int nSum, int gender_idx, int case_idx)
{
	std::string result = "";

	if (nSum == 0)
	{
		return exception_numbers[nSum][case_idx];
	}

	int digit;
	PartsOfNumber cur_part = PartsOfNumber::Units;
	while (nSum > 0)
	{
		int number = nSum % 100;
		if (number!=0 && cur_part == PartsOfNumber::Units && (exception_numbers.find(number) != exception_numbers.end()))
		{
			result = exception_numbers[number][case_idx];
			nSum /= 100;
			cur_part = PartsOfNumber::Hundreds;
		}
		else {
			digit = nSum % 10;
			nSum /= 10;
			if (digit == 0)
			{
				int cur_part_idx = static_cast<int>(cur_part);
				cur_part = static_cast<PartsOfNumber>(++cur_part_idx);
				continue;
			}
			switch (cur_part)
			{
			case PartsOfNumber::Units:
				result = units[digit][case_idx][gender_idx];
				cur_part = PartsOfNumber::Dozens;
				break;

			case PartsOfNumber::Dozens:
				result = dozens[digit][case_idx] + " " + result;
				cur_part = PartsOfNumber::Hundreds;
				break;

			case PartsOfNumber::Hundreds:
				result = hundreds[digit][case_idx] + " " + result;
				break;
			}
		}
	}
	if (result[result.size() - 1] == ' ')
	{
		result.erase(result.size() - 1);
	}
	return result;
}

std::string NumberConverter::AddThousandsWord(int number, int case_idx, ThousandsOfNumber cur_state)
{	
	int group_idx = 0;
	number %= 100;
	if (number > 19)
	{
		number %= 10;
	}

	if (number > 4 || number == 0)
	{
		group_idx = 2;
	}
	else if (number > 1)
	{
		group_idx = 1;
	}

	switch (cur_state)
	{
	case ThousandsOfNumber::Million:
		return thousands[group_idx][case_idx];
		break;

	case ThousandsOfNumber::Billion:
		return millions[group_idx][case_idx];
		break;

	case ThousandsOfNumber::Trillion:
		return billions[group_idx][case_idx];
		break;
	}
	return "";
}

