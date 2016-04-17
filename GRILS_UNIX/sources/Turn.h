#ifndef TURN_H
#define TURN_H


#include "types.h"


class Turn
{
	//------------- Methods
public:

	// ----- CONSTRUCTOR AND DESTRUCTOR

	Turn(); // default constructor
	// Instructions :
	//		Creates a new tour

	Turn(int n_id, Vertex * vtx_v0, Vertex * vtx_vN); // constructor
	// Instructions :
	//		Creates a new tour
	//		n_id is the tour number (unique)
	// 		vtx_v0 matches the first vertex of the tour
	//		vtx_vN matches the first vertex of the tour

	Turn(bool test);
	// Instructions :
	//		test purpose constructor
	//		test == false displays an error message on std.err.

	virtual ~Turn(); // default destructor
	// Instructions :
	//		Destroys the tour
	//		releases the ressources allocated depending on the context 



	// ----- GETTERS AND SETTERS -----

	float GetAvailableTime();
	// Instructions :
	//		Computes the remaining amount of time available for the tour.
	//
	// Return value :
	//		The remaining amount of time available for the tour.
	//
	// NB :
	// 		The turn need to be "up-to-date" before use of this method.
	// 		See Turn.UpdateAvailableTime() and UpdateVerticesTimeRelatedInfos().

	float GetCost();
	// Instructions :
	//		Computes the price of the Turn (sum of the vertices fees).
	//
	// Return value :
	//		The price of the Turn (sum of the vertices fees).
	//
	// NB :
	// 		The turn need to be "up-to-date" before use of this method.
	// 		See Turn.UpdateCost().

	int GetIntervals();
	// Instructions :
	//		Computes the number of intervals considered for insertion.
	//
	// Return value :
	// 		The number of intervals open for insertion.

	float GetScore();
	// Instructions :
	//		Computes the score of the Turn (sum of the vertices scores).
	//
	// Return value :
	//		The score of the Turn (sum of the vertices scores).

	float GetSlack(int i_knap);
	// Instructions :
	//		Computes the amount of slack associated with the constraint "knap".
	//
	// Return value :
	//		The amount of slack associated with the constraint "knap".
	//
	// NB :
	// 		The turn need to be "up-to-date" before use of this method.
	// 		See Turn.UpdateKnapSlack().

	Vertex * GetVertexAt(int index);
	// Return value :
	//		pointer to vertex at "index" position (if existing)
	//		null otherwise

	void SetConstraint(vector<int> & v_n_TypeConstraint);
	// Instructions :
	//		Sets constraints of the turns.



	// ----- UPDATES -----

	void Update();
	// Instructions :
	// 		Updates turn and every vertex time constraints.
	// 		Updates every knapsack constraints slack.
	//
	// NB :
	//		This methods is costly in terms of computationnal time.
	//		Try to use the sub Update methods as much as possible.
	
	void UpdateAvailableTime();
	// Instructions :
	//		Updates remaining available time for the tour.

	void UpdateKnapSlack();
	// Instructions :
	//		Updates snap-sack constraints related slack.
	//
	// NB :
	//		ONLY updates Z-type constraint. updating budget related update is implemented in functions.cpp (updateBudgetSlack)

	void UpdateCost();
	// Instructions :
	//		Updates the overall cost of the turn.

	void UpdateVerticesTimeRelatedInfos();
	// Instructions :
	//		Updates vertices time related information
	// 		(TA - Wait - Departure - MaxShift)



	//------ TIME AND CONSTRAINTS MANAGEMENT -----

	int GetBestShift(Vertex * p_vtx, float & bestShift);
	// Instructions :
	//		Computes the best TimeShift of insertion (meaning the smallest) for the given vertex while trying to beat the last bestShift found.
	//
	// Return value :
	//		if the vertex is insertable with better shift than bestShift returns the index of insertion AND updates automatically bestShift.
	//		else returns -1
	//
	// DEPRECATED

	float GetShift(Vertex * p_vtx, int index);
	// Instructions :
	//		Computes time-Shift of insertion
	//
	// Return value :
	//		The amount of time shifted by the insertion if insertion is possible
	//		-1 otherwise.

	void InsertVertex(Vertex * p_vtx, int index = -1);
	// Instructions :
	//		Insert Vertex at the position "index"
	//		Computes all the time changes necessary.
	//		if index == -1: insert where it consumes least possible time and respects timewindows.
	//
	// NB :
	// 		Usage of index == -1 is strongly advised
	//		Using this method suggest that you indicate a correct index, a valid pointer and an insertible vertex !!

	bool IsInsertable(Vertex * p_vtx);
	// Instructions :
	//		Computes if insertion of the summit is feasible given the knapsack constraints.
	//
	// Return value :
	//		true if feasible
	//		false otherwise.



	//----- VALIDATION -----

	bool isTurnValid(bool checkTime = true, bool checkConstraints = true);
	// Instructions :
	//		Checks if the turn is respecting the problem constraints.
	//		checkTime = true means that you want to test time constraints
	//		checkConstraints = true means that you want to test n-type constraints
	//
	// Return value :
	//		true if it respects the tested constraints,
	//		false otherwise.


	// TESTING PURPOSES 
	void _testPurpose_InsertVertex(Vertex * vtx, int index = -1);
	// Instructions :
	//		Inserts Vtx at index "index".
	//		if index == -1 : append vtx to the turn.



	//----- UTILITY -----

	void Copy(Turn * p_turn);
	// Instructions :
	//		Copy the vertices from the turn in parameters into this one
	//
	// NB :
	// 		This function is meant to be used by the "BestSolution" turn and should not be used otherwise.

	void Print();
	// Instructions :
	//		Prints the turn informations on std.out.

	vector<Vertex *> RemoveVertices(int n_start, int n_range);
	// Instructions :
	//		Remove vertices from index "n_start" to index "n_start + n_range".
	// 		if "n_start + n_range" exceeds turn.size() -1 then the remaining vertices to remove are extracted from start.
	//
	// Return value :
	//		A vector containing the ids of the removed vertices.



	//----- CLASS ATTRIBUTES -----

	static float f_BudgetSlack; //------ Budget slack remaining for the solution.
	static float f_BudgetLimitation; //- Budget amount limitation
	static int n_instance_count; //----- Number of turn allocated to check correct ressources release



	//------------- ATTRIBUTES
private:
	// Private Attributes 
	int n_id; //------------------------ Id of the tour
	float f_cost; //-------------------- Cost of the tour
	float f_availableTime; //----------- Remaining amount of time available within tour
	vector<Vertex *> v_vertices; //----- vector of vertices included in the tour
	vector<int> v_n_Slack; //----------- vector of the slack amount remaining for each n-type constraint
	vector<int> v_n_TypeConstraint; //-- vector of n-type constraints for each tour

	// Optimisation Attributes
	bool isTest; //--------------------- While testing, abort the ressources release to prevent SegFault
	bool isBestTurn; //----------------- Indicates that the turn belongs to the best solution found so far and must release its ressources accordingly
};

#endif