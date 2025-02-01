#ifndef AUXFUN_H
#define AUXFUN_H

#include "Data.h"
#include <vector>

using namespace std;

vector<int> criarSubtour(Data& data);
void printVector(vector<int>& vector);
bool contem(vector<int>& vector, int target);

#endif