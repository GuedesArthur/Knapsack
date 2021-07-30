#include "Knapsack.h"
#include <array>
#include <algorithm>
#include <optional>
#include <string>

#pragma region Item

Item::Item(int w, int v) : weight(w), value(v)
{

}

Item::Item(nlohmann::json json) : Item(json["weight"], json["value"])
{

}

float Item::Ratio()
{
	return (float) value / weight;
}

bool Item::CompareRatio(Item i1, Item i2)
{
	return i1.Ratio() < i2.Ratio();
}

#pragma endregion

#pragma region Knapsack

// Returns the biggest of the two integers
int Knapsack::Max(int i1, int i2)
{
	return i1 > i2
		? i1
		: i2;
}

// Solves Knapsack problem using given algorithm
int Knapsack::Solve(int maxWeight, int n, vector<Item>& itemList, string algorithm, bool measurePerf)
{
	if (!measurePerf)
		return _methodMap.at(algorithm)(maxWeight, n, itemList);
	StopWatch stopwatch(algorithm);

	auto result = _methodMap.at(algorithm)(maxWeight, n, itemList);

	stopwatch.Stop();

	if (algorithm != "All")
		cout << std::endl << "Knapsack result: " << result << std::endl;

	return result;
}

// Algorithm dictionary
const unordered_map<string, Algorithm> Knapsack::_methodMap =
{
	{"BruteForce", Knapsack::BruteForce},
	{"Greedy", Knapsack::Greedy},
	{"Lazy", Knapsack::Lazy},
	{"All", Knapsack::All}
};

// Brute force algorithm
int Knapsack::BruteForce(int maxWeight, int n, vector<Item>& itemList)
{
	return BruteForceR(maxWeight, n, itemList, 0);
}

// Brute force algorithm - Recursive segment
int Knapsack::BruteForceR(int capacity, int n, vector<Item>& itemList, int value)
{
	// Base case
	if (capacity == 0 || n == 0)
		return value;

	auto item = itemList[n - 1]; // Gets current item

	// If item's weight exceeds knapsack's current capacity,
	// skips to next item
	if (item.weight > capacity)
		return BruteForceR(capacity, n - 1, itemList, value);

	// Compares knapsack's value when current item to when it's absent from it
	// then, returns the greater one
	return Max(
		BruteForceR(capacity - item.value, n - 1, itemList, value + item.value),
		BruteForceR(capacity, n - 1, itemList, value)
	);
}

// Greedy approximation algorithm
int Knapsack::Greedy(int maxWeight, int n, vector<Item>& itemList)
{
	// Sorts list
	// - std::sort performance - O(N*logN)
	std::sort(itemList.begin(), itemList.end(), Item::CompareRatio);

	return GreedyR(maxWeight, n, itemList, 0);
}

// Greedy approximation algorithm recursive segment
int Knapsack::GreedyR(int capacity, int n, vector<Item>& sortedList, int value)
{
	// Base case
	if (capacity == 0 || n == 0)
		return value;

	auto item = sortedList[n - 1]; // Gets current item

	// If item's weight exceeds knapsack's current capacity,
	// skips to next item
	if (item.weight > capacity)
		return GreedyR(capacity, n - 1, sortedList, value);

	// Chooses currents item and goes to the next one
	return GreedyR(capacity - item.value, n - 1, sortedList, value + item.value);
}

// Lazy algorithm
int Knapsack::Lazy(int maxWeight, int n, vector<Item>& itemList)
{
	StopWatch sw("\nStoring matrix " + to_string(n) + " X " +to_string(maxWeight + 1));
	// Creates matrix(n, w), for storing item values
	LazyMatrix matrix(n, std::vector<optional<int>>(maxWeight + 1));
	sw.Stop();
	return LazyR(maxWeight, n, itemList, matrix, 0);

}

// Lazy algorithm recursive segment
int Knapsack::LazyR(int capacity, int n, vector<Item>& itemList, LazyMatrix& memory, int value)
{
	// Base case
	if (capacity == 0 || n == 0)
		return value;

	// If current item/capacity's value was previously calculated, reads it
	if (memory[n - 1][capacity].has_value())
		return memory[n - 1][capacity].value();

	auto item = itemList[n - 1]; // Gets current item

	// If item's weight exceeds knapsack's current capacity,
	// skips to next item
	if (item.weight > capacity)
	{
		memory[n - 1][capacity] = LazyR(capacity, n - 1, itemList, memory, value);

		return memory[n - 1][capacity].value();
	}

	// Compares knapsack's value when current item to when it's absent from it
	// then, stores the greater one on memory array
	memory[n - 1][capacity] = Max(
		LazyR(capacity - item.value, n - 1, itemList, memory, value + item.value),
		LazyR(capacity, n - 1, itemList, memory, value)
	);

	return memory[n - 1][capacity].value();
}

// Executes all algorithms for the given problem
int Knapsack::All(int capacity, int n, vector<Item>& itemList)
{
	int result;
	for (auto keyValue : _methodMap)
	{
		if (keyValue.first == "All"s)
			continue;
		cout << "\n\nSolving by " << keyValue.first;
		result = Solve(capacity, n, itemList, keyValue.first, true);
		cout << "Result: " << result << endl;
	}

	return result;
}

#pragma endregion

#pragma region StopWatch

StopWatch::StopWatch(string opName) : start(chrono::high_resolution_clock::now()), operationName(opName)
{

}

chrono::duration<double> StopWatch::Stop()
{
	auto duration = chrono::high_resolution_clock::now() - start;
	cout << "Duration of " << operationName << ": " << duration.count() << " nanoseconds ( " << ((double)duration.count()) / 1000000000 << " seconds )."
		<< std::endl << "-------------------------------------" << std::endl;

	return duration;
}

#pragma endregion