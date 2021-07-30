#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <unordered_map>
#include <chrono>
#include <execution>
#include <optional>
#include "json.hpp"

using namespace std;

// Structure for each knapsack ite
struct Item
{
    int weight;
    int value;

    Item(int w, int v);
    Item(nlohmann::json json);

    float Ratio();

    static bool CompareRatio(Item i1, Item i2);
};

using Algorithm = function<int (int, int, vector<Item>&)>;
using LazyMatrix = vector<vector<optional<int>>>;

// Static Knapsack0-1 solving class
class Knapsack
{
    const static unordered_map<string, Algorithm> _methodMap;

    static int Max(int i1, int i2);
    static int BruteForce(int maxWeight, int n, vector<Item>& itemList);
    static int BruteForceR(int capacity, int n, vector<Item>& itemList, int value);

    static int Greedy(int maxWeight, int n, vector<Item>& itemList);
    static int GreedyR(int capacity, int n, vector<Item>& sortedList, int value);

    static int Lazy(int maxWeight, int n, vector<Item>& itemList);
    static int LazyR(int capacity, int n, vector<Item>& itemList, LazyMatrix& memory, int value);

    static int All(int maxWeight, int n, vector<Item>& itemList);

public:
    static int Solve(int maxWeight, int n, vector<Item>& itemList, string algorithm, bool measurePerf);
};

// Measures execution time
class StopWatch
{
    chrono::high_resolution_clock::time_point start;
    string operationName;
public:
    StopWatch(string opName);

    chrono::duration<double> Stop();
};

