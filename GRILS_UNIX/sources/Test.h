#ifndef TEST_H
#define TEST_H

#include "types.h"
#include "functions.h"
#include "Turn.h"




class Test
{
public:

	// Testing - Loading Core Components
	void static TestLoadingCore(int n_Turns, int n_vertices, float n_budgetLim, vector<int>* v_n_TypeConstraint);

	// Testing - Loading Vertices
	void static TestLoadingVertices(vector<Vertex *> v_vertices);

	// Testing - Converting Vertices
	void static TestConvertingMTWtoSTW(vector<Vertex *> v_vertices_STW);

	// Testing - Creating Tours
	void static TestCreatingTours(vector<Turn *> v_Turns);

	

	// Unit tests

	// global - getDistance()
	void static FuncTest_global_GetDistance();

	// global - copyVertex()
	void static FuncTest_global_copyVertex();

	// Turn - GetShift()
	void static FuncTest_Turn_GetShift();

	// Turn - InsertVertex()
	void static FuncTest_Turn_InsertVertex();

	// Turn - UpdateVerticesTimeRelatedInfos()
	void static FuncTest_Turn_UpdateVerticesTimeRelatedInfos();

	// Turn - UpdateAvailableTime()
	void static FuncTest_Turn_UpdateAvailableTime();

	// Turn - UpdateCost()
	void static FuncTest_Turn_UpdateCost();

	// Turn - UpdateKnapSlack()
	void static FuncTest_Turn_UpdateKnapSlack();

	// Turn - FuncTest_Turn_RemoveVertices()
	void static FuncTest_Turn_RemoveVertices();



	// Asserting if result value is True. Does not block execution
	void static assertValid(string s_expected, bool b_testResult);


	// Building a tour from built vertices
	void static BuildTurn(vector <Vertex *> v_vertices,vector <Turn *> v_Turns, vector<int> v_InsertionSequence);
};

#endif // TEST_H