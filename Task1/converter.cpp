#include "converter.h"
#include <vector>
#include <fstream>

void NumberConverter::LoadUnitsConfigs()
{
	std::ifstream configs(units_configs_path);
	for (int digit = 1; digit < 10; digit++)
	{
		for (int case_idx = 0; case_idx < 6; case_idx++)
		{
			std::string line;
			getline(configs, line);
			int word_start = 0, word_end = 0;
			for (int gender_idx = 0; gender_idx < 3; gender_idx++)
			{
				while (line[word_end] != separator) { word_end++; }
				units[digit][cases[case_idx]][genders[gender_idx]] = line.substr(word_start, word_end - 1 - word_start);
			}
		}
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
			break;

		case PartsOfNumber::Hundreds:
			cur_configs_path = hundreds_configs_path;
			break;
		}

		for (int digit = 1; digit < 10; digit++)
		{
			for (int case_idx = 0; case_idx < 6; case_idx++)
			{
				std::ifstream configs(cur_configs_path);
				std::string line;
				std::getline(configs, line);
				switch (cur_part)
				{
				case PartsOfNumber::Dozens:
					dozens[digit][cases[case_idx]] = line;
						break;

				case PartsOfNumber::Hundreds:
					hundreds[digit][cases[case_idx]] = line;
					break;
				}
			}
		}
	}
}

void NumberConverter::LoadExceptionNumbersConfigs()
{

}

NumberConverter::NumberConverter()
{
	LoadUnitsConfigs();
	LoadDozensAndHundredsConfigs();
	LoadExceptionNumbersConfigs();
}

std::string NumberConverter::sumProp(int nSum, std::string sGender, std::string sCase)
{
	int number = 0, counter = 1;
	std::string result = "";
	ThousandsOfNumber cur_state = ThousandsOfNumber::Thousand;
	bool is_negative = false;
	
	if (nSum == 0)
	{
										//TODO: запустить для 3-х 
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
			switch (cur_state)
			{
			case ThousandsOfNumber::Thousand:
				result = (sumThree(number, sGender, sCase));
				cur_state = ThousandsOfNumber::Million;
				break;

			case ThousandsOfNumber::Million:
				result = (sumThree(number, "Ж", sCase)) + " " + result;
				cur_state = ThousandsOfNumber::Billion;
				break;
				
			case ThousandsOfNumber::Billion:
				result = (sumThree(number, "М", sCase)) + " " + result;
				cur_state = ThousandsOfNumber::Trillion;
				break;

			case ThousandsOfNumber::Trillion:
				result = (sumThree(number, "М", sCase)) + " " + result;
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

std::string NumberConverter::sumThree(int nSum, std::string sGender, std::string sCase)
{
	std::string result = "";
	
	if (nSum == 0)
	{
		
	}

	return result;
}