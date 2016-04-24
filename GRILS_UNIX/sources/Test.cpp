#include <iostream>
#include <string>
#include <math.h>


#include "Test.h"

using namespace std;

// STRUCTS

// Fonction Tests

void Test::TestLoadingCore(int n_Turns, int n_vertices, float n_budgetLim, vector<int>* v_n_TypeConstraint) {

	bool b_testResult = true;
	b_testResult = (n_Turns == 1 && n_vertices == 48 && n_budgetLim == 994);
	int tab[] = { 4, 10, 3, 6, 4, 2, 4, 6, 4, 6 };
	for (int i = 0; i < 10; i++) {
		b_testResult = b_testResult && (*v_n_TypeConstraint)[i] == tab[i];
	}
	Test::assertValid("Loading : core Informations", b_testResult);
}

void Test::TestLoadingVertices(vector<Vertex *> v_vertices) {
	bool b_testResult = true;

	// test loading first and last vertices
	b_testResult = b_testResult && (v_vertices[0]->id == 0 && int(v_vertices[0]->x * 1000) == -10442 && int(v_vertices[0]->y * 1000) == 19999 && int(v_vertices[0]->serviceTime) == 0 && int(v_vertices[0]->score) == 0);
	b_testResult = b_testResult && (v_vertices[0]->v_tws[0]->O == 0 && v_vertices[0]->v_tws[0]->C == 1000);
	b_testResult = b_testResult && (v_vertices[1]->id == 1 && int(v_vertices[0]->x * 1000) == -10442 && int(v_vertices[0]->y * 1000) == 19999 && int(v_vertices[0]->serviceTime) == 0 && int(v_vertices[0]->score) == 0);
	b_testResult = b_testResult && (v_vertices[1]->v_tws[0]->O == 0 && v_vertices[1]->v_tws[0]->C == 1000);

	Test::assertValid("Loading : first and last vertices", b_testResult);

	// test loading all others vertices
	// in fact we are just gonna test last vertex and check it's infos / and suppose everything went right for the other ones.
	b_testResult = true;

	int n_lastVertex = v_vertices.size() - 1;
	b_testResult = b_testResult && (n_lastVertex == 49);
	b_testResult = b_testResult && (v_vertices[n_lastVertex]->id == 49 && int(v_vertices[n_lastVertex]->x * 1000) == 42883 && int(v_vertices[n_lastVertex]->y * 1000) == -2966);
	b_testResult = b_testResult && (int(v_vertices[n_lastVertex]->serviceTime * 10) == 170 && int(v_vertices[n_lastVertex]->score * 10) == 100);
	b_testResult = b_testResult && (int(v_vertices[n_lastVertex]->v_tws[0]->O) == 98 && int(v_vertices[n_lastVertex]->v_tws[0]->C) == 131); // static cast to int because == comparison in float type is buggy
	b_testResult = b_testResult && (int(v_vertices[n_lastVertex]->v_tws[1]->O) == 131 && int(v_vertices[n_lastVertex]->v_tws[1]->C) == 165);
	b_testResult = b_testResult && (int(v_vertices[n_lastVertex]->v_tws[2]->O) == 165 && int(v_vertices[n_lastVertex]->v_tws[2]->C) == 199);
	b_testResult = b_testResult && (int(v_vertices[n_lastVertex]->v_tws[3]->O) == 199 && v_vertices[n_lastVertex]->v_tws[3]->C == 233);
	b_testResult = b_testResult && (v_vertices[n_lastVertex]->e == 1 && v_vertices[n_lastVertex]->fee == 76);
	for (int i = 0; i < 10; i++) {
		if (i == 1 || i == 5 || i == 6) {
			b_testResult = b_testResult && (v_vertices[n_lastVertex]->v_constraint[i] == 1);
		}
		else {
			b_testResult = b_testResult && (v_vertices[n_lastVertex]->v_constraint[i] == 0);
		}
	}

	Test::assertValid("Loading : every other vertex", b_testResult);
}

void Test::TestConvertingMTWtoSTW(vector<Vertex *> v_vertices_STW) {

	int n_lastVertex = v_vertices_STW.size() - 1;
	bool b_testResult = n_lastVertex + 1 == 194;

	// de même, on teste les 2 premiers sommets
	b_testResult = b_testResult && (v_vertices_STW[0]->id == 0 && int(v_vertices_STW[0]->x * 1000) == -10442 && int(v_vertices_STW[0]->y * 1000) == 19999 && int(v_vertices_STW[0]->serviceTime) == 0 && int(v_vertices_STW[0]->score) == 0);
	b_testResult = b_testResult && (v_vertices_STW[0]->v_tws[0]->O == 0 && v_vertices_STW[0]->v_tws[0]->C == 1000);
	b_testResult = b_testResult && (v_vertices_STW[1]->id == 1 && int(v_vertices_STW[0]->x * 1000) == -10442 && int(v_vertices_STW[0]->y * 1000) == 19999 && int(v_vertices_STW[0]->serviceTime) == 0 && int(v_vertices_STW[0]->score) == 0);
	b_testResult = b_testResult && (v_vertices_STW[1]->v_tws[0]->O == 0 && v_vertices_STW[1]->v_tws[0]->C == 1000);


	// puis on teste le dernier sommet (qui correspond à la fenetre temporelle du sommet 49.
	b_testResult = b_testResult && (v_vertices_STW[n_lastVertex]->id == 49 && int(v_vertices_STW[n_lastVertex]->x * 1000) == 42883 && int(v_vertices_STW[n_lastVertex]->y * 1000) == -2966);
	b_testResult = b_testResult && (int(v_vertices_STW[n_lastVertex]->serviceTime * 10) == 170 && int(v_vertices_STW[n_lastVertex]->score * 10) == 100);
	b_testResult = b_testResult && (int(v_vertices_STW[n_lastVertex]->v_tws[0]->O) == 199 && int(v_vertices_STW[n_lastVertex]->v_tws[0]->C) == 233);
	b_testResult = b_testResult && (v_vertices_STW[n_lastVertex]->e == 1 && int(v_vertices_STW[n_lastVertex]->fee) == 76);
	for (int i = 0; i < 10; i++) {
		if (i == 1 || i == 5 || i == 6) {
			b_testResult = b_testResult && (int(v_vertices_STW[n_lastVertex]->v_constraint[i]) == 1);
		}
		else {
			b_testResult = b_testResult && (int(v_vertices_STW[n_lastVertex]->v_constraint[i]) == 0);
		}
	}
	Test::assertValid("Converting : MTW vertices to STW Vertices", b_testResult);
}

void Test::TestCreatingTours(vector<Turn *> v_Turns) {

	bool b_testResults;
	b_testResults = v_Turns.size() == 1;
	v_Turns[0]->UpdateAvailableTime();
	b_testResults = b_testResults && (int(v_Turns[0]->GetAvailableTime()) == 1000);
	Test::assertValid("Creating : Tours", b_testResults);
}


// Unit Test

void Test::FuncTest_global_GetDistance() {

	bool b_testResults = true;
	Vertex vtx_1, vtx_2, vtx_3;
	vtx_1.x = 0;
	vtx_1.y = 0;
	vtx_2.x = 5;
	vtx_2.y = 5;
	vtx_3.x = 3;
	vtx_3.y = -6;

	b_testResults = b_testResults && (int(getDistance(&vtx_1, &vtx_2)*10000) == int(sqrt(50)*10000));
	b_testResults = b_testResults && (int(getDistance(&vtx_1, &vtx_3)*10000) == int(sqrt(45)*10000));
	b_testResults = b_testResults && (int(getDistance(&vtx_2, &vtx_3)*10000) == int(getDistance(&vtx_3, &vtx_2)*10000));

	Test::assertValid("global - getDistance(&vtx_1, &vtx_2)", b_testResults);
}

void Test::FuncTest_global_copyVertex() {

	bool b_testResults = true;

	Vertex vtx_1;
	TW tw1;
	tw1.O = 4;
	tw1.C = 8;
	vtx_1.id = 1;
	vtx_1.e = 1;
	vtx_1.fee = 1;
	vtx_1.MaxShift = 500;
	vtx_1.score = 300;
	vtx_1.serviceTime = 700;
	vtx_1.TA = 42;
	vtx_1.x = 13;
	vtx_1.y = 45;
	vtx_1.v_tws.push_back(&tw1);

	Vertex * p_vtx_2 = copyVertex(&vtx_1);


	b_testResults = b_testResults && vtx_1.id == p_vtx_2->id;
	b_testResults = b_testResults && vtx_1.e == p_vtx_2->e;
	b_testResults = b_testResults && vtx_1.MaxShift == p_vtx_2->MaxShift;
	b_testResults = b_testResults && vtx_1.score == p_vtx_2->score;
	b_testResults = b_testResults && vtx_1.serviceTime == p_vtx_2->serviceTime;
	b_testResults = b_testResults && vtx_1.TA == p_vtx_2->TA;
	b_testResults = b_testResults && vtx_1.x == p_vtx_2->x;
	b_testResults = b_testResults && vtx_1.y == p_vtx_2->y;
	b_testResults = b_testResults && vtx_1.v_tws[0]->C == p_vtx_2->v_tws[0]->C;
	b_testResults = b_testResults && vtx_1.v_tws[0]->O == p_vtx_2->v_tws[0]->O;

	for (int i = 0; i < 10; i++) {
		b_testResults = b_testResults && vtx_1.v_constraint[i] == p_vtx_2->v_constraint[i];
	}


	Test::assertValid("global - copyVertex(p_vtx)", b_testResults);
}

void Test::FuncTest_Turn_GetShift() {
	bool b_testResults = true;
	Vertex vtx_1, vtx_2, vtx_3;

	// time windows init
	TW tw_1, tw_2, tw_3;
	tw_1.O = 0;
	tw_1.C = 100;
	tw_2.O = 0;
	tw_2.C = 100;
	tw_3.O = 7;
	tw_3.C = 18;

	// vertices init
	vtx_1.x = 0;
	vtx_1.y = 0;
	vtx_2.x = 0;
	vtx_2.y = 5;
	vtx_3.x = 0;
	vtx_3.y = 5;

	vtx_1.v_tws.push_back(&tw_1);
	vtx_2.v_tws.push_back(&tw_2);
	vtx_3.v_tws.push_back(&tw_3);

	vtx_1.serviceTime = 0;
	vtx_2.serviceTime = 5;
	vtx_3.serviceTime = 5;

	vtx_1.Wait = 0;
	vtx_1.TA = 0;
	vtx_1.Departure = 0;
	vtx_1.MaxShift = vtx_1.v_tws[0]->C;


	Turn turn(0, &vtx_1, &vtx_1);

	// add other tests here
	b_testResults = b_testResults && int(turn.GetShift(&vtx_2, 0)) == 15;
	b_testResults = b_testResults && int(turn.GetShift(&vtx_3, 0)) == 17;

	Test::assertValid("Turn - GetShift(Vertex p_vtx, int index)", b_testResults);
}

void Test::FuncTest_Turn_InsertVertex() {
	bool b_testResults = true;

	Vertex vtx_1, vtx_2, vtx_3, vtx_4;
	TW tw_1, tw_2, tw_3, tw_4;

	// TW Configuration
	tw_1.O = 0;
	tw_1.C = 45;
	tw_2.O = 2;
	tw_2.C = 10;
	tw_3.O = 14;
	tw_3.C = 22;
	tw_4.O = 31;
	tw_4.C = 41;

	// Vertices Configuration
	vtx_1.id = 1;
	vtx_2.id = 2;
	vtx_3.id = 3;
	vtx_4.id = 4;

	vtx_1.x = 0;
	vtx_1.y = 0;
	vtx_2.x = 6;
	vtx_2.y = 0;
	vtx_3.x = 3;
	vtx_3.y = 4;
	vtx_4.x = 3;
	vtx_4.y = -4;

	vtx_1.v_tws.push_back(&tw_1);
	vtx_2.v_tws.push_back(&tw_2);
	vtx_3.v_tws.push_back(&tw_3);
	vtx_4.v_tws.push_back(&tw_4);


	vtx_1.serviceTime = 0;
	vtx_2.serviceTime = 2;
	vtx_3.serviceTime = 7;
	vtx_4.serviceTime = 3;

	vtx_2.v_c_ICN.push_back('C');
	vtx_3.v_c_ICN.push_back('C');
	vtx_4.v_c_ICN.push_back('C');

	Turn turn(0, &vtx_1, &vtx_1);
	turn.UpdateVerticesTimeRelatedInfos();

	turn.InsertVertex(&vtx_2);
	turn.InsertVertex(&vtx_4);
	turn.InsertVertex(&vtx_3);

	Test::assertValid("Turn - InsertVertex(&vtx)", turn.isTurnValid(true, false));
}

void Test::FuncTest_Turn_UpdateAvailableTime() {

	bool b_testResults = true;

	Vertex vtx_1, vtx_2, vtx_3, vtx_4;
	TW tw_1, tw_2, tw_3, tw_4;

	// TW Configuration
	tw_1.O = 0;
	tw_1.C = 45;
	tw_2.O = 2;
	tw_2.C = 10;
	tw_3.O = 14;
	tw_3.C = 22;
	tw_4.O = 31;
	tw_4.C = 41;

	// Vertices Configuration
	vtx_1.id = 1;
	vtx_2.id = 2;
	vtx_3.id = 3;
	vtx_4.id = 4;

	vtx_1.x = 0;
	vtx_1.y = 0;
	vtx_2.x = 6;
	vtx_2.y = 0;
	vtx_3.x = 3;
	vtx_3.y = 4;
	vtx_4.x = 3;
	vtx_4.y = -4;

	vtx_1.v_tws.push_back(&tw_1);
	vtx_2.v_tws.push_back(&tw_2);
	vtx_3.v_tws.push_back(&tw_3);
	vtx_4.v_tws.push_back(&tw_4);


	vtx_1.serviceTime = 0;
	vtx_2.serviceTime = 2;
	vtx_3.serviceTime = 7;
	vtx_4.serviceTime = 3;

	vtx_2.v_c_ICN.push_back('C');
	vtx_3.v_c_ICN.push_back('C');
	vtx_4.v_c_ICN.push_back('C');

	Turn turn(0, &vtx_1, &vtx_1);

	turn.UpdateVerticesTimeRelatedInfos();


	turn.InsertVertex(&vtx_2);
	turn.InsertVertex(&vtx_4);
	turn.InsertVertex(&vtx_3);

	// displays vertices included in file
	//turn.printToFile(NULL);

	turn.UpdateAvailableTime();
	b_testResults = b_testResults && int(turn.GetAvailableTime()) == 9;

	Test::assertValid("Turn - UpdateAvailableTime()", b_testResults);


}

void Test::FuncTest_Turn_UpdateVerticesTimeRelatedInfos() {
	bool b_testResults = true;

	Vertex vtx_1, vtx_2, vtx_3, vtx_4;
	TW tw_1, tw_2, tw_3, tw_4;

	// TW Configuration
	tw_1.O = 0;
	tw_1.C = 45;
	tw_2.O = 2;
	tw_2.C = 10;
	tw_3.O = 14;
	tw_3.C = 22;
	tw_4.O = 31;
	tw_4.C = 41;

	// Vertices Configuration
	vtx_1.id = 1;
	vtx_2.id = 2;
	vtx_3.id = 3;
	vtx_4.id = 4;

	vtx_1.x = 0;
	vtx_1.y = 0;
	vtx_2.x = 6;
	vtx_2.y = 0;
	vtx_3.x = 3;
	vtx_3.y = 4;
	vtx_4.x = 3;
	vtx_4.y = -4;

	vtx_1.v_tws.push_back(&tw_1);
	vtx_2.v_tws.push_back(&tw_2);
	vtx_3.v_tws.push_back(&tw_3);
	vtx_4.v_tws.push_back(&tw_4);


	vtx_1.serviceTime = 0;
	vtx_2.serviceTime = 2;
	vtx_3.serviceTime = 7;
	vtx_4.serviceTime = 3;

	vtx_2.v_c_ICN.push_back('C');
	vtx_3.v_c_ICN.push_back('C');
	vtx_4.v_c_ICN.push_back('C');

	Vertex * p_vtx_0 = copyVertex(&vtx_1);

	Turn turn(true);

	// test pour 2 vertex uniquement
	turn._testPurpose_InsertVertex(&vtx_1);
	turn._testPurpose_InsertVertex(p_vtx_0);

	turn.UpdateVerticesTimeRelatedInfos();

	b_testResults = b_testResults && int(vtx_1.TA) == 0;
	b_testResults = b_testResults && int(p_vtx_0->TA) == 0;
	b_testResults = b_testResults && int(vtx_1.Wait) == 0;
	b_testResults = b_testResults && int(p_vtx_0->Wait) == 0;
	b_testResults = b_testResults && int(vtx_1.Departure) == 0;
	b_testResults = b_testResults && int(p_vtx_0->Departure) == 0;
	b_testResults = b_testResults && int(vtx_1.MaxShift) == 45;
	b_testResults = b_testResults && int(p_vtx_0->MaxShift) == 45;

	turn._testPurpose_InsertVertex(&vtx_2,1);
	turn._testPurpose_InsertVertex(&vtx_3,2);
	turn._testPurpose_InsertVertex(&vtx_4,3);

	turn.UpdateVerticesTimeRelatedInfos();
	b_testResults = b_testResults && int(vtx_1.TA) == 0;
	b_testResults = b_testResults && int(vtx_2.TA) == 6;
	b_testResults = b_testResults && int(vtx_3.TA) == 13;
	b_testResults = b_testResults && int(vtx_4.TA) == 29;
	b_testResults = b_testResults && int(p_vtx_0->TA) == 39;

	b_testResults = b_testResults && int(vtx_1.Wait) == 0;
	b_testResults = b_testResults && int(vtx_2.Wait) == 0;
	b_testResults = b_testResults && int(vtx_3.Wait) == 1;
	b_testResults = b_testResults && int(vtx_4.Wait) == 2;
	b_testResults = b_testResults && int(p_vtx_0->Wait) == 0;

	b_testResults = b_testResults && int(vtx_1.Departure) == 0;
	b_testResults = b_testResults && int(vtx_2.Departure) == 8;
	b_testResults = b_testResults && int(vtx_3.Departure) == 21;
	b_testResults = b_testResults && int(vtx_4.Departure) == 34;
	b_testResults = b_testResults && int(p_vtx_0->Departure) == 39;

	b_testResults = b_testResults && int(vtx_1.MaxShift) == 4;
	b_testResults = b_testResults && int(vtx_2.MaxShift) == 4;
	b_testResults = b_testResults && int(vtx_3.MaxShift) == 8;
	b_testResults = b_testResults && int(vtx_4.MaxShift) == 6;
	b_testResults = b_testResults && int(p_vtx_0->MaxShift) == 6;

	delete p_vtx_0;

	Test::assertValid("Turn - UpdateVerticesTimeRelatedInfos()", b_testResults);

}

void Test::FuncTest_Turn_UpdateCost() {
	bool b_testResults = true;

	Vertex vtx_1, vtx_2, vtx_3, vtx_4;

	vtx_1.fee = 0;
	vtx_2.fee = 4;
	vtx_3.fee = 3;
	vtx_4.fee = 7;

	Turn turn(true);

	turn._testPurpose_InsertVertex(&vtx_1);
	turn._testPurpose_InsertVertex(&vtx_2);
	turn._testPurpose_InsertVertex(&vtx_3);
	turn._testPurpose_InsertVertex(&vtx_4);
	turn._testPurpose_InsertVertex(&vtx_1);

	turn.UpdateCost();


	b_testResults = b_testResults && int(turn.GetCost()) == 14;


	Test::assertValid("Turn - UpdateCost()", b_testResults);

}

void Test::FuncTest_Turn_UpdateKnapSlack() {
	bool b_testResults = true;

	Vertex vtx_1, vtx_2, vtx_3, vtx_4;


	for (int i = 0; i < 10; i++) {
		vtx_1.v_constraint[i] = 0;
		vtx_2.v_constraint[i] = 0;
		vtx_3.v_constraint[i] = 0;
		vtx_4.v_constraint[i] = 0;
	}
	vtx_1.v_constraint[0] = 1;
	vtx_1.v_constraint[1] = 1;
	vtx_1.v_constraint[2] = 1;

	vtx_2.v_constraint[1] = 2;
	vtx_2.v_constraint[2] = 2;
	vtx_2.v_constraint[3] = 2;

	vtx_3.v_constraint[2] = 3;
	vtx_3.v_constraint[3] = 3;
	vtx_3.v_constraint[4] = 3;

	vtx_4.v_constraint[3] = 4;
	vtx_4.v_constraint[4] = 4;
	vtx_4.v_constraint[5] = 4;

	Turn turn(true);

	turn._testPurpose_InsertVertex(&vtx_1);
	turn._testPurpose_InsertVertex(&vtx_2);
	turn._testPurpose_InsertVertex(&vtx_3);
	turn._testPurpose_InsertVertex(&vtx_4);

	vector<int> v_constraint;

	for (int i = 0; i < 10; i++) {
		v_constraint.push_back(10);
	}

	turn.SetConstraint(v_constraint);

	turn.UpdateKnapSlack();

	b_testResults = b_testResults && turn.GetSlack(0) == 9;
	b_testResults = b_testResults && turn.GetSlack(1) == 7;
	b_testResults = b_testResults && turn.GetSlack(2) == 4;
	b_testResults = b_testResults && turn.GetSlack(3) == 1;
	b_testResults = b_testResults && turn.GetSlack(4) == 3;
	b_testResults = b_testResults && turn.GetSlack(5) == 6;


	Test::assertValid("Turn - UpdateSnapSlack()", b_testResults);

}

void Test::FuncTest_Turn_RemoveVertices() {
	bool b_testResults = true;

	Vertex  vtx_0, vtx_1, vtx_2, vtx_3, vtx_4, vtx_5, vtx_6, vtx_7, vtx_8, vtx_9;
	vtx_0.id = 0;
	vtx_1.id = 1;
	vtx_2.id = 2;
	vtx_3.id = 3;
	vtx_4.id = 4;
	vtx_5.id = 5;
	vtx_6.id = 6;
	vtx_7.id = 7;
	vtx_8.id = 8;
	vtx_9.id = 9;
	vtx_0.v_c_ICN.push_back('C');
	vtx_1.v_c_ICN.push_back('C');
	vtx_2.v_c_ICN.push_back('C');
	vtx_3.v_c_ICN.push_back('C');
	vtx_4.v_c_ICN.push_back('C');
	vtx_5.v_c_ICN.push_back('C');
	vtx_6.v_c_ICN.push_back('C');
	vtx_7.v_c_ICN.push_back('C');
	vtx_8.v_c_ICN.push_back('C');
	vtx_9.v_c_ICN.push_back('C');

	Turn turn(true);

	turn._testPurpose_InsertVertex(&vtx_0);
	turn._testPurpose_InsertVertex(&vtx_1);
	turn._testPurpose_InsertVertex(&vtx_2);
	turn._testPurpose_InsertVertex(&vtx_3);
	turn._testPurpose_InsertVertex(&vtx_4);
	turn._testPurpose_InsertVertex(&vtx_5);
	turn._testPurpose_InsertVertex(&vtx_6);
	turn._testPurpose_InsertVertex(&vtx_7);
	turn._testPurpose_InsertVertex(&vtx_8);
	turn._testPurpose_InsertVertex(&vtx_9);
	// 10 vertices inserted

	turn.RemoveVertices(7, 5);

	int test_table[] = { 0, 4, 5, 6, 9 };
	for (int i = 0; i < 5; i++) {
		b_testResults = b_testResults && (test_table[i] == turn.GetVertexAt(i)->id);
	}
	Test::assertValid("Turn - RemoveVertices(int,int)", b_testResults);

}


// Asserts

void Test::assertValid(string s_expected, bool b_testResult) {
	string s_ok = " - OK   : ", s_fail = " - FAIL : ";
	string s_result = (b_testResult) ? s_ok + s_expected : s_fail + s_expected;
	cout << s_result << endl;
}



// Building a tour from built vertices

void Test::BuildTurn(vector <Vertex *> v_vertices_STW, vector <Turn *> v_turns, vector<int> v_InsertionSequence){
	Turn turn (0, v_vertices_STW[0], v_vertices_STW[1]);

	for (int i = 0; i < v_InsertionSequence.size() ; i++){

		cout << endl << "it " << i << endl;
		cout << turn.GetShift(v_vertices_STW[v_InsertionSequence[i]], i)<< endl;
		turn.InsertVertex(v_vertices_STW[v_InsertionSequence[i]],i);
		turn.Print();
		turn.UpdateVerticesTimeRelatedInfos();
		turn.Print();
		// UPDATING KNAPSACK CONSTRAINTS
		//updateBudgetSlack(v_turns);
		//turn.UpdateKnapSlack();
	}


	turn.UpdateVerticesTimeRelatedInfos();



	Test::assertValid("Solution is Valid according to given constraints", turn.isTurnValid());



	cout <<endl << "shift of insertion of vertex 82 into turn : " << turn.GetShift(v_vertices_STW[82],6) << endl;
}





