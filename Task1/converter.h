#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

class BaseConverter
{
public:
	virtual std::string sumProp(int nSum, std::string sGender, std::string sCase) = 0;
	// nSum - ����� ����� ����� ��������� (�������� 999 999 999 999)
	// sGender - ���(� - �������, � - �������, � - �������)
	// sCase - �����( � -������������, �, � - ����������)
};

class NumberConverter :public BaseConverter
{
public:
	NumberConverter();
	std::string sumProp(int nSum, std::string sGender, std::string sCase) override;
private:
	std::string sumThree(int nSum, std::string sGender, std::string sCase);
	void LoadUnitsConfigs();
	void LoadDozensAndHundredsConfigs();
	void LoadExceptionNumbersConfigs();

	enum class ThousandsOfNumber // ����� ����������� �� �������: ������ ��� ������� - ������, ��������� - ������� � �.�.
	{
		Thousand,
		Million,
		Billion,
		Trillion,
	};

	enum class PartsOfNumber
	{
		Units,
		Dozens,
		Hundreds,
	};

	const std::vector<const std::string> genders = { "�" , "�", "�"};
	const char separator = '|';
	const std::vector<const std::string> cases = { "�", "�", "�", "�", "�", "�"};

	const std::string units_configs_path = "configs\\units_configs.txt";
	const std::string dozens_configs_path = "configs\\dozens_configs.txt";
	const std::string hundreds_configs_path = "configs\\hundreds_configs.txt";
	const std::string exception_numbers_configs_path = "configs\\exception_numbers_configs.txt";

	using declination_table = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>; 
	// ������� ����: ������ ��� �����, ������� ��� ���, � ������� �����. 
	// ��������� ���� ������ �������, ����� ���� ����� ����������, ����� �������� ������

	using cases_table = std::unordered_map<std::string, std::string>;
	// ������� ����: ������ ��� �����, � ������� �����. 
												
	std::unordered_map<int, declination_table> units;
	std::unordered_map<int, cases_table> dozens;
	std::unordered_map<int, cases_table> hundreds;
	std::unordered_map<int, cases_table> exception_numbers; // 0, 11, 12, 13, 14, 15, 16, 17, 18, 19
};

