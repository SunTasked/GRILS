// ------------------------------------------------------------------------
// ----------------------------- GRILS ------------------------------------
// ------------------------------------------------------------------------

// TODO : Optimize !!!
// - decrease "greedDecrease" value. --> 0.01

// INCLUDES

#include "stdafx.h"

#include <ctime>   // for time
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "types.h"
#include "functions.h"
#include "Turn.h"
#include "Test.h"
#include "Config.h"

using namespace std;




int main() { //------------------------------- START OF MAIN --------------------------------------

	// ------------------------------------------------------------------------
	// --------------------------- VARIABLES ----------------------------------
	// ------------------------------------------------------------------------

	// Input data for the problem
	float T0 = 0; //--------------------- Start of Turn
	float Tmax = 0; //------------------- Amount of time available for the Turns
	int n_Turns = 0; //------------------ Number of Turns
	int n_vertices = 0; //--------------- Number of vertices (others than 1st and last vertices)
	float n_budgetLim = 0; //-------------- Maximal amount of ressource spendable
	vector<Vertex *> v_vertices; //------ Vector of vertices (size == n_vertices + 2)
	vector<Turn *> v_Turns; //----------- Vector of Turns (list of vertices)
	vector<int> v_n_TypeConstraint; //--- Vector of the z-type Constaints , i.e., v_n_TypeConstraint[i] contains
									//--- the maximal amount of z-type vertices within the same turn.

	// Data for the treatement of the problem
	vector<Vertex *> v_vertices_STW; //-- Vector of vertices with Single Time Windows (size == n_vertices * N_TW + 2)
	vector<Turn *> v_BestSolution; //---- Vector of the turncomposition representing the best solution found so far
	float f_score_BestSolution = 0; //--- Score associated with the best solution computed so far

	// greed loop parameters
	float startgreed = 0.89; //---------- Initial Greed
	float greedRange = 0.30; //------------- Greed Range
	float greedDecrease = 0.01; //------- Greed Decrease for each iter

	// inner loop parameters
	int n_maxIterNoImprovement = 100; //- Maximal number of iteration with no improvement
	int n_Shake_StartIndex = 1; //------- Index from which the shake procedure should start
	int n_Shake_Range = 1; //------------ Number of vertices to remove from each turn.



	// ------------------------------------------------------------------------
	// ------------------------------ START -----------------------------------
	// ------------------------------------------------------------------------

	cout << "---------- GRILS v1.0 ----------" << endl << endl;

	string s_filePath;

	if (TEST) {
		s_filePath = "../testFiles/MCTOPMTW-1-pr01.txt";
		cout << "----------- TESTING PROCEDURE ---------" << endl << endl;
	}
	else {
		cout << "enter file path: ";
		s_filePath = "../testFiles/MCTOPMTW-1-pr02.txt";
		//cin >> s_filePath;
	}

	if (TEST) {
		cout << "--- UNIT TESTS" << endl;
		Test::FuncTest_global_GetDistance();
		Test::FuncTest_global_copyVertex();
		Test::FuncTest_Turn_GetShift();
		Test::FuncTest_Turn_InsertVertex();
		Test::FuncTest_Turn_UpdateVerticesTimeRelatedInfos();
		Test::FuncTest_Turn_UpdateAvailableTime();
		Test::FuncTest_Turn_UpdateCost();
		Test::FuncTest_Turn_UpdateKnapSlack();
		Test::FuncTest_Turn_RemoveVertices();


		cout << endl << "--- FONC TESTS" << endl;
	}


	// ------------------------------------------------------------------------
	// ------------------------- PARSING OF INPUT FILE ------------------------
	// ------------------------------------------------------------------------

	ifstream myfile(s_filePath);
	if (myfile.is_open())
	{
		parseFile(myfile, n_Turns, n_vertices, n_budgetLim, v_vertices, v_vertices_STW, v_Turns, v_n_TypeConstraint, T0, Tmax);
		myfile.close();
	}
	else {
		cout << "Unable to open file" << endl;
		system("pause");
		return 0;
	}

	// Test validity of file parsing
	if (TEST) {
		Test::TestLoadingCore(n_Turns, n_vertices, n_budgetLim, &v_n_TypeConstraint);
		Test::TestLoadingVertices(v_vertices);
	}



	// ------------------------------------------------------------------------
	// ------------------------------ INIT ------------------------------------
	// ------------------------------------------------------------------------


	// Initialising RandomSeed.
	initRandomness();

	// Converting Multi-Time-Windows vertices to Single-Time-Windows Vertices.
	// Initialising STW Vertices.
	//
	convertMTWtoSTW(v_vertices, v_vertices_STW);
	initSTWVertices(v_vertices_STW, n_Turns);
	if (TEST) { Test::TestConvertingMTWtoSTW(v_vertices_STW); }

	// Initialising Turns.
	//
	initTurns(n_budgetLim, n_Turns, v_vertices_STW[0], v_vertices_STW[1], v_n_TypeConstraint, v_Turns);
	initBestSolution(v_BestSolution, n_Turns);
	if (TEST) { Test::TestCreatingTours(v_Turns); }


	// ------------------------------------------------------------------------
	// ------------- GREEDY ALGORITHM with ITERATIVE LOCAL SEARCH -------------
	// ------------------------------------------------------------------------

	cout << n_Turns << endl;
	
	for (float greed = startgreed; greed > startgreed - greedRange; greed -= greedDecrease) { //---- START OF GRILS
		int n_iterationNoImprovement = 0;

		while (n_iterationNoImprovement < n_maxIterNoImprovement) { //------------------------------ START OF ILS

			// BUILDING SOLUTION
			buildTurns(v_vertices_STW, v_Turns, greed);

			// COMPARING LAST SOLUTION TO BEST SOLUTION
			float f_score_CurrentSolution = getSolutionScore(v_Turns);
			if (f_score_CurrentSolution > f_score_BestSolution) {
				if (TRACE) {
					cout << endl << "Better Solution Found" << endl;
				}
				n_iterationNoImprovement = 0;
				f_score_BestSolution = f_score_CurrentSolution;
				for (int i = 0; i < v_Turns.size(); i++) {
					v_BestSolution[i]->Copy(v_Turns[i]);
				}
			}
			else {
				n_iterationNoImprovement++;
			}

			// SHAKING SOLUTION
			vector<Vertex *> v_p_removedVertices = shakeSolution(v_Turns, n_Shake_StartIndex, n_Shake_Range);

			// UPDATING NEIGHBORHOOD CONSISTENCY
			updateVerticesCompatibility(v_vertices_STW, v_p_removedVertices, v_Turns);

			// UPDATING SHAKE START AND RANGE
			updateShake(n_Shake_StartIndex, n_Shake_Range, v_Turns, n_vertices);

		} // --------------------------------------------------------------------------------------- END OF ILS

	} // ------------------------------------------------------------------------------------------- END OF GRILS

	for (int i = 0; i < v_BestSolution.size(); i++) {
		v_BestSolution[i]->PrintToFile(NULL);
	}
	Test::assertValid("Turn is Valid according to given constraints", v_BestSolution[0]->isTurnValid());
	cout << "Score is " << v_BestSolution[0]->GetScore() << endl;



	// ------------------------------------------------------------------------
	// ------------------------- RELEASE OF RESSOURCES ------------------------
	// ------------------------------------------------------------------------


	cout << endl << "End of the Computation" << endl;
	// provoque un point d'arret ... je verrai plus tard.


	// Releasing MTW vertices
	for (auto it_vtx = v_vertices.begin(); it_vtx != v_vertices.end(); it_vtx++) {
		// Release time windows
		for (auto it_tw = (*it_vtx)->v_tws.begin(); it_tw != (*it_vtx)->v_tws.end(); it_tw++) {
			delete (*it_tw);
		}
		delete (*it_vtx);
	}

	Test::assertValid("Releasing : Vertex and TW objects", true);

	// Releasing STW vertices:
	for (auto it_vtx = v_vertices_STW.begin(); it_vtx != v_vertices_STW.end(); it_vtx++) {
		delete (*it_vtx);
	}

	// Releasing turns:
	for (auto it_turn = v_Turns.begin(); it_turn != v_Turns.end(); it_turn++) {
		delete (*it_turn);
	}
	for (auto it_turn = v_BestSolution.begin(); it_turn != v_BestSolution.end(); it_turn++) {
		delete (*it_turn);
	}

	Test::assertValid("Releasing : Turn objects", Turn::n_instance_count == 0);


	// ------------------------------------------------------------------------
	// ------------------------------- END ------------------------------------
	// ------------------------------------------------------------------------

	cout << endl << endl << "program successfully ended" << endl;
	system("pause");

	return 0;
} //--------------------------- END OF MAIN --------------------------------

