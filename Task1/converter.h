#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

class BaseConverter
{
public:
	virtual std::string sumProp(long long nSum, std::string sGender, std::string sCase) = 0;
	// nSum - целое число менее триллиона (максимум 999 999 999 999)
	// sGender - род(М - мужской, Ж - женский, С - средний)
	// sCase - падеж( И -именительный, …, П - предложный)
	// возвращает пустую строку если невозможно преобразовать
};

class NumberConverter :public BaseConverter
{
public:
	NumberConverter();
	std::string sumProp(long long nSum, std::string sGender, std::string sCase) override;
private:
	enum class ThousandsOfNumber // число разбивается по тройкам: первые три разряда - тысяча, следующие - миллион и т.д.
	{
		Thousand,
		Million,
		Billion,
		Trillion
	};

	enum class PartsOfNumber
	{
		Units,
		Dozens,
		Hundreds
	};

	const long long MAX_VALUE = 999999999999;
	const std::vector<std::string> genders = { "М", "Ж", "С"};
	const int male_idx = 0;
	const int female_idx = 1;
	const char separator = '|';
	const std::vector<std::string> cases = { "И", "Р", "Д", "В", "Т", "П"};
	const int cases_count = 6;

	const std::string units_configs_path = "configs\\units_configs.txt";
	const std::string dozens_configs_path = "configs\\dozens_configs.txt";
	const std::string hundreds_configs_path = "configs\\hundreds_configs.txt";
	const std::string exception_numbers_configs_path = "configs\\exception_numbers_configs.txt";
	const std::string key_words_configs_path = "configs\\key_words_configs.txt"; 
	//файлы должны быть в кодировке ANSI

	using declination_table = std::vector<std::vector<std::string>>; 
	// таблица вида: строки это падеж, столбцы это род, в ячейках слово. 
																													
	using cases_table = std::vector<std::string>;
	// таблица вида: строки это падеж, в ячейках слово. 
												
	std::vector<declination_table> units;
	std::vector<cases_table> dozens;
	std::vector<cases_table> hundreds;
	std::unordered_map<int, cases_table> exception_numbers; // 0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
	std::vector<cases_table> thousands; // key words
	std::vector<cases_table> millions;  // key words
	std::vector<cases_table> billions;  // key words
	const int key_words_types_count = 3; // первая группа для 1, вторая для 2-4, третья для 5-19 и 0

	std::string sumThree(int nSum, int gender_idx, int case_idx);
	std::string AddThousandsWord(int number, int case_idx, ThousandsOfNumber cur_state);
	void LoadUnitsConfigs();
	void LoadDozensAndHundredsConfigs();
	void LoadExceptionNumbersConfigs();
	void LoadKeyWordsConfigs();
};

