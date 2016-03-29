#pragma once

#include "stdafx.h"

#include <vector>

using namespace std;


// CONSTANTS




// STRUCTS

typedef struct { // Time Window (TW)

	float O; // openning hour
	float C; // closing hour

} TW;


struct Vertex { // Vertex

	// Should you change anything here don't forget to implement accordingly copyVertex(p_vtx) described in function.h

	int id; //------------ unique id
	float x; //----------- x coordinate
	float y; //----------- y coordinate
	float score; //------- score associated with the vertex

	// Constraints
	vector<char> v_c_ICN; //--- v_c_ICN[w] = [I if Included / C if Considered / N if Not possible to include] in turn w
	float v_constraint[10]; //- Slack consumption of the vertex
	float fee; //-------------- fee related to the vertex 

	// Time handling
	vector<TW *> v_tws; // Time Windows of the vertex (in chronological order)
	float serviceTime; //- time necessary to complete the visit
	float TA; //---------- time of arrival
	float Departure; //--- time of departure
	float Wait;	//-------- waiting time
	float MaxShift; //---- Maximum shifting time possibility
	int e; //------------- Exchange of Time Windows (cf format Description)

	// Algorithm optimisation
	bool isInserted; //----- Indicates if turn is inseted in a turn or not.
	vector<struct Vertex *> v_p_vtx_Family;

};typedef struct Vertex Vertex;

typedef struct { // Contains the informations related to the insertion of the vertex into the turn.

	int index; //-------------- Index of insertion
	float heuristicScore; //--- EuristicScore of the insertion
	Vertex * p_vtx; //--------- Vertex concerned
	int n_turn; //------------- [Turn] concerned / cast necessary because of compilation

} Insertion;
