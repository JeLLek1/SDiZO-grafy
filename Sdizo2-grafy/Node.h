#pragma once
struct Node {
	//wierzcho�ek do kt�rego prowadzi
	size_t nodeTo;
	//waga
	int weight;
};

//Interpretacja kraw�dzi
struct Edge {
	size_t nodeFrom;
	size_t nodeTo;
	int weight;
};