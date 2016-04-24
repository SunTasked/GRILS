#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Turn.h"
#include "Config.h"


// FUNCTIONS
//
// Declaration of global usage functions.
//


// --------------- UTILITY ---------------
//
// functions created as programming tools.
//

Vertex * copyVertex(Vertex* p_vtx);
// Instructions :
//		Does a deep copy of vertex object (i.e. a new Vertex object will be instanciated)
//
// Return Value :
//		The new Vertex instance.

int countInsertible(vector<Vertex *> & v_vertices_STW, int n_turns);
// Instructions :
// 		Counts how many remaining vertices are still insertible
//
// DEPRECATED

float getDistance(Vertex * p_vtx_1, Vertex * p_vtx_2);
// Instructions :
//		Computes the Euclidian distance beetween 2 vertices
//
// Return value :
// 		The value of the distance.

int getRandomNumber(int lowerBound, int upperBound);
// Instructions :
// 		Picks a random number between lowerBound (included) and upperBound(excluded)
//
// Return value :
// 		integer respecting the constraints

float getSolutionScore(vector<Turn *> v_Turns);
// Instructions :
//		Computes score of a solution.
//
// Return value :
// 		The score of the given solution.

void insert_vtxIntoTurn(Insertion & ins, vector<Turn *> & v_Turns, vector<Vertex *> & v_vertices_STW);
// Instructions :
// 		inserts a vertex in a turn given the informations provided in Insertion

void PrintHowTo();
// Instructions :
//		Prints the user's manual of the application.
//
// NB :
//		Should you add or modify app functionnalities, please update this method

bool sortInsertion(Insertion i, Insertion j);
// Instructions :
// 		Compares two Insertion objects
//
// Return value :
// 		true if i.heuristicScore > j.heuristicScore
//		false otherwise

vector <string> split(string);
// Instructions :
//		allows user to split sentence on spaces. (cf python.string.split())
//
// Return value :
//		Vector of strings. each string contains a word.


// --------------- READABILITY ---------------
//
// functions created in order to improve code lisibility.
// Most of them are just representing a step of the GRILS algorithm.
//

void buildTurns(vector<Vertex *> & v_vertices_STW, vector<Turn *> & v_Turns, float & greed);
// Instructions :
// 		Build Turns according to the ILS algorithm modified by the random approach.

void convertMTWtoSTW(vector<Vertex *> & v_vertices, vector<Vertex *> & v_vertices_STW);
// Instructions :
// 		Converts MTW vertices into STW vertices

void initSTWVertices(vector<Vertex *> & v_vertices_STW, int n_Turns);
// Instructions :
// 		Inits the STW vertices vector.

void initTurns(float n_budgetLim, float  n_Turns, Vertex * start, Vertex * finish, vector<int> & v_n_TypeConstraint, vector<Turn *> & v_Turns);
// Instructions :
// 		Inits the turns vector.

void initBestSolution(vector<Turn *> & v_BestSolution, int n_Turns);
// Instructions :
// 		Inits the BestSolution vector.

void initRandSeed();
// Instructions :
// 		inits rand() seed.

void parseFile(istream & myfile, int & n_Turns, int & n_vertices, float & n_budgetLim, vector<Vertex *> & v_vertices, vector<Vertex *> & v_vertices_STW, vector<Turn *> & v_Turns, vector<int> & v_n_TypeConstraint, float & T0, float & Tmax);
// Instructions :
// 		Parses input file "myfile"
// 		Values affected are : 
// 			- n_turns (number of turns)
//			- n_vertices (number of vertices)
//			- n_budgetLim (maximal amount of money allocated to the turns)
//			- v_vertices (vector of vertices with multiple time windows)
//			- v_vertices_STW (vector of vertices with single time windows)
//			- v_Turns (vector of turns initialised)
//			- v_n_TypeConstraint (vector of n-type-constraint)
//			- T0 (time at which TOP starts)
//			- Tmax (time at wich TOP ends)
//
// NB: Might throw exception if file is not encoded correctly

void updateBudgetSlack(vector<Turn *> v_Turns);
// Instructions :
// 		updates slack related to budget among the turns.

void ReleaseAllocatedTurns (vector<Turn *> & v_Turns);
// Instructions :
// 		Releases the ressources allocated by the program to the Turns in the given vector.
//
// NB :
// 		Should the allocation of ressources change, this function must be updated.

void ReleaseAllocatedTWs (vector<Vertex *> & v_vertices);
// Instructions :
// 		Releases the ressources allocated by the program to the TW (time windows) of the Vertices in the given vector.
//
// NB :
// 		Should the allocation of ressources change, this function must be updated.

void ReleaseAllocatedVertices (vector<Vertex *> & v_vertices);
// Instructions :
// 		Releases the ressources allocated by the program to the Vertices in the given vector.
//
// NB :
// 		Should the allocation of ressources change, this function must be updated.

vector<Vertex *> shakeSolution(vector<Turn *> & v_Turns, int n_Shake_StartIndex, int n_Shake_Range);
// Instructions :
// 		Shakes solution (removes some vertices from turns)
//
// Return value :
// 		vector of vertices removed from turns.

void updateShake(int & n_Shake_StartIndex, int & n_Shake_Range, vector<Turn *> & v_Turns, int n_vertices);
// Instructions :
// 		Updates Shake parameters: StartIndex and Range.

void updateVerticesCompatibility(vector<Vertex *> v_vertices_STW, vector<Vertex *> v_p_removedVertices, vector<Turn *>v_Turns);
// Instructions :
// 		Updates Vectors neighborhood consistency.



#endif // FUNCTIONS_H

