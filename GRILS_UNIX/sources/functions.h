#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Turn.h"
#include "Config.h"


// FUNCTIONS


// --------------- UTILITIES
// functions created as programming tools.

// allows user to split sentence on spaces. (cf python.string.split())
vector <string> split(string);

// Deep Copy of vertex object (i.e. allocated ressources within the vertex will be duplicated)
Vertex * copyVertex(Vertex* p_vtx);

// Releases the ressources allocated by the program to the Vertices in the given vector.
// Should the allocation of ressources change, this function must be updated.
void ReleaseAllocatedVertices (vector<Vertex *> & v_vertices);

// Releases the ressources allocated by the program to the Turns in the given vector.
// Should the allocation of ressources change, this function must be updated.
void ReleaseAllocatedTurns (vector<Turn *> & v_Turns);

// Releases the ressources allocated by the program to the TW (time windows) of the Vertices in the given vector.
// Should the allocation of ressources change, this function must be updated.
void ReleaseAllocatedTWs (vector<Vertex *> & v_vertices);

// Computes the distance beetween 2 vertices
float getDistance(Vertex * p_vtx_1, Vertex * p_vtx_2);

// Counts how many remaining vertices are still insertible 
int countInsertible(vector<Vertex *> & v_vertices_STW, int n_turns);

// Computes score of a solution.
float getSolutionScore(vector<Turn *> v_Turns);

// Comparison function for Insertion type
bool sortInsertion(Insertion i, Insertion j);



// --------------- READABILITY
// functions created in order to improve lisibility over the program.

// Parses Files
void parseFile(istream & myfile, int & n_Turns, int & n_vertices, float & n_budgetLim, vector<Vertex *> & v_vertices, vector<Vertex *> & v_vertices_STW, vector<Turn *> & v_Turns, vector<int> & v_n_TypeConstraint, float & T0, float & Tmax);

// Converts MTW vertices to STW vertices
void convertMTWtoSTW(vector<Vertex *> & v_vertices, vector<Vertex *> & v_vertices_STW);

// Init vertices
void initSTWVertices(vector<Vertex *> & v_vertices_STW, int n_Turns);

// Init Turns
void initTurns(float n_budgetLim, float  n_Turns, Vertex * start, Vertex * finish, vector<int> & v_n_TypeConstraint, vector<Turn *> & v_Turns);

// Init BestSolution
void initBestSolution(vector<Turn *> & v_BestSolution, int n_Turns);

// inserts a vertex in a turn given the informations provided in Insertion
void insert_vtxIntoTurn(Insertion & ins, vector<Turn *> & v_Turns, vector<Vertex *> & v_vertices_STW);

// updates slack related to budget among the turns.
void updateBudgetSlack(vector<Turn *> v_Turns);

// inits rand() seed.
void initRandSeed();

// Returns a random number between lowerBound (included) and upperBound(excluded)
int getRandomNumber(int lowerBound, int upperBound);

// Build Turns.
void buildTurns(vector<Vertex *> & v_vertices_STW, vector<Turn *> & v_Turns, float & greed);

// Shakes solution (removes some vertices from turns)
// returns vector of vertices removed from turns.
vector<Vertex *> shakeSolution(vector<Turn *> & v_Turns, int n_Shake_StartIndex, int n_Shake_Range);

// Updates Vectors neighborhood consistency.
void updateVerticesCompatibility(vector<Vertex *> v_vertices_STW, vector<Vertex *> v_p_removedVertices, vector<Turn *>v_Turns);

// Updates Shake parameters: StartIndex and Range.
void updateShake(int & n_Shake_StartIndex, int & n_Shake_Range, vector<Turn *> & v_Turns, int n_vertices);

#endif // FUNCTIONS_H