#include "Knapsack.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <string>
constexpr auto configPath = "config.json";

// Loads configuration json
bool LoadJson(nlohmann::json& json, std::string path)
{
	std::ifstream ifs(path, std::ifstream::in);

	if (!ifs.is_open()) return false;
	else ifs >> json;

	ifs.close();
	return true;
}

int main()
{
	// Initial Setup
	nlohmann::json config;
	if (!LoadJson(config, configPath))
	{
		cout << "Error loading json " << configPath;
		return 1;
	}

	vector<Item> itemVector;
	auto jsonItems = config["items"];

	// Stores json's items on a Item(struct) vector
	itemVector.insert(itemVector.begin(), jsonItems.begin(), jsonItems.end());
	
	Knapsack::Solve(config["maxWeight"], config["n"], itemVector, config["algorithm"], config["measurePerf"]);

	// Waits for key press to exit program
	cin.get();
	return 0;
}