#pragma once

#include "types.h"


class Turn
{
	//------------- Methods
public:

	// default constructors
	Turn();
	Turn(int n_id, Vertex * vtx_v0, Vertex * vtx_vN);

	// default destructor
	virtual ~Turn();

	//----- GETTERS and SETTERS


	// return the number of intervals considered for insertion.
	int GetIntervals();

	// return remaining available time for the tour
	// The turn need to be "up-to-date" before use of this method.
	// see Turn.UpdateAvailableTime() and UpdateVerticesTimeRelatedInfos();
	float GetAvailableTime();

	// returns the slack associated with the constraint "knap"
	// The turn need to be "up-to-date" before use of this method.
	// see Turn.UpdateKnapSlack()
	float GetSlack(int i_knap);

	// returns the price of the Turn (sum of the vertices fees)
	// The turn need to be "up-to-date" before use of this method.
	// see Turn.UpdateCost()
	float GetCost();

	// returns the score of the Turn (sum of the vertices scores).
	float GetScore();

	// returns vertex at "index" position
	Vertex * GetVertexAt(int index);

	// this fonction allows you to set a set of constraints on the turns.
	// Should the problem change, modify this fonction accordingly.
	//
	void SetConstraint(vector<int> & v_n_TypeConstraint);


	//----- UPDATES

	// Updates Tour.
	// Updates turn and every vertex time constraints.
	// Updates every knapsack constraints slack.
	void Update();
	
	// Updates vertices time related information
	// (TA - Wait - Departure - MaxShift)
	void UpdateVerticesTimeRelatedInfos();

	// Updates remaining available time for the tour
	void UpdateAvailableTime();

	// Updates snap-sack constraints related slack.
	// !!! ONLY updates Z-type constraint. updating budget related update is implemented in function.cpp)
	void UpdateKnapSlack();

	// Updates the overall cost of the turn.
	void UpdateCost();


	//------ TIME AND CONSTRAINTS

	// Computes time-Shift of insertion
	// returns -1 if it's not possible to insert vertex while respecting TW.
	float GetShift(Vertex * p_vtx, int index);

	// Computes if insertion of the summit is feasible given the knapsack constraints.
	// returns True if feasible, false otherwise.
	bool IsInsertable(Vertex * p_vtx);

	// Computes the best TimeShift of insertion (meaning the smallest) for the given vertex while trying to beat the last bestShift found.
	// if the vertex is insertable with better shift than bestShift returns the index of insertion AND updates automatically bestShift.
	// else returns -1
	int GetBestShift(Vertex * p_vtx, float & bestShift);


	// Insert Vertex at the position "index"
	// if index == -1: insert where it consumes least possible time and respects timewindows.
	void InsertVertex(Vertex * p_vtx, int index = -1);



	//----- VALIDATION

	// Checks if turns is respecting the problem constraints 
	bool isTurnValid(bool checkTime = true, bool checkConstraints = true);

	// Inserts Vtx at index "index".
	// if index == 0 : append vtx to the turn.
	void _testPurpose_InsertVertex(Vertex * vtx, int index = -1);

	// test purpose constructor
	Turn(bool test);



	//----- UTILITIES

	// Remove vertices from "n_start" to "n_start + n_range".
	// if "n_start + n_range" exceeds turn.size() -1 then the remaining vertices
	// to remove are extracted from start.
	//returns vector containing the ids of the removed vertices
	vector<Vertex *> RemoveVertices(int n_start, int n_range);

	// Prints the turn into the file given as parameter.
	void PrintToFile(ifstream * file);

	// Copy the vertices from the turn in parameters into this one
	// This function is meant to be used by the "BestSolution" turn and should not be used otherwise.
	void Copy(Turn * p_turn);

	//----- Class attributes

	// slack of budget.
	static float f_BudgetSlack;
	static float f_BudgetLimitation;

	// Instance Count
	static int n_instance_count;

	//------------- Attributes
private:
	// Private Attributes 
	int n_id;
	float f_cost;
	float f_availableTime; // Remaining Available time within tour
	vector<Vertex *> v_vertices;
	vector<int> v_n_Slack;

	// Shared Attributes
	vector<int> v_n_TypeConstraint; // Z-type constraints for each tour

	// Optimisation Attributes
	bool isTest;
	bool isBestTurn;
};

