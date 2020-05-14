#pragma once
struct Node {
	//wierzcho³ek do którego prowadzi
	size_t nodeTo;
	//waga
	int weight;
};

//Interpretacja krawêdzi
struct Edge {
	size_t nodeFrom;
	size_t nodeTo;
	int weight;
};