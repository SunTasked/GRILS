#include <iostream>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <math.h>

#include "types.h"
#include "Turn.h"
#include "functions.h"
#include "Config.h"

using namespace std;


// --------------- UTILITIES

Vertex * copyVertex(Vertex* p_vtx) {
	Vertex * p_vtx_new = new Vertex;
	*p_vtx_new = *p_vtx;
	p_vtx_new->v_tws = p_vtx->v_tws;
	for (int i = 0; i < 10; i++) {
		p_vtx_new->v_constraint[i] = p_vtx->v_constraint[i];
	}
	p_vtx_new->v_c_ICN = p_vtx->v_c_ICN;
	return p_vtx_new;
}


vector <string> split(string sentence) {
	istringstream iss(sentence);
	vector<string> words (istream_iterator<string>{iss},istream_iterator<string>{} );
	return words;
}

void ReleaseAllocatedVertices (vector<Vertex *> & v_vertices) {
	for (auto it_vtx = v_vertices.begin(); it_vtx != v_vertices.end(); it_vtx++) {
		delete (*it_vtx);
	}
}


void ReleaseAllocatedTurns(vector<Turn *> & v_Turns) {
	for (auto it_turn = v_Turns.begin(); it_turn != v_Turns.end(); it_turn++) {
		delete (*it_turn);
	}
}


void ReleaseAllocatedTWs (vector<Vertex *> & v_vertices) {
	for (auto it_vtx = v_vertices.begin(); it_vtx != v_vertices.end(); it_vtx++) {
		// Release time windows
		for (auto it_tw = (*it_vtx)->v_tws.begin(); it_tw != (*it_vtx)->v_tws.end(); it_tw++) {
			delete (*it_tw);
		}
	}
}


float getDistance(Vertex * p_vtx_1, Vertex * p_vtx_2) {
	// optimisation locale peut-être nécessaire (bit shift ?)
	float result = (p_vtx_2->x - p_vtx_1->x)*(p_vtx_2->x - p_vtx_1->x) + (p_vtx_2->y - p_vtx_1->y)*(p_vtx_2->y - p_vtx_1->y);
	result = sqrt(result);
	return result;
}


bool sortInsertion(Insertion i, Insertion j) {
	return i.heuristicScore > j.heuristicScore;
}


int countInsertible(vector<Vertex *> & v_vertices_STW, int n_turns) {
	int n_vertices_insertable = 0;

	for (auto it_vtx = v_vertices_STW.begin() + 2; it_vtx != v_vertices_STW.end(); it_vtx++) {
		for (int i = 0; i < n_turns; i++) {
			if ((*it_vtx)->v_c_ICN[i] == 'C') {
				n_vertices_insertable++;
				break;
			}
		}
	}

	return n_vertices_insertable;
}


float getSolutionScore(vector<Turn *> v_Turns) {
	float f_score = 0;
	for (int i_turns = 0; i_turns < v_Turns.size(); i_turns++) {
		f_score += v_Turns[i_turns]->GetScore();
	}
	return f_score;
}



// --------------- READABILITY

void parseFile(istream & myfile, int & n_Turns, int & n_vertices, float & n_budgetLim, vector<Vertex *> & v_vertices, vector<Vertex *> & v_vertices_STW, vector<Turn *> & v_Turns, vector<int> & v_n_TypeConstraint, float & T0, float & Tmax) {
	string line;

	// retreiving problem core informations
	// n_Turns n_vertices n_budgetLim (nTypeConstraint)x10
	getline(myfile, line);
	vector<string> v_coreInfo = split(line);
	n_Turns = stoi(v_coreInfo[0]);
	n_vertices = stoi(v_coreInfo[1]);
	n_budgetLim = stoi(v_coreInfo[2]);
	for (int i = 0; i < 10; i++) {
		v_n_TypeConstraint.push_back(stoi(v_coreInfo[3 + i]));
	}

	// retrieving first and last vertex informations
	// index, x, y, serviceTime, score, timeOfOpenning, timeOfClosing
	getline(myfile, line);
	vector<string> v_firstVertex = split(line);
	Vertex * p_vtx_firstVertex = new Vertex;
	Vertex * p_vtx_lastVertex = new Vertex;
	// first goes the core infos
	T0 = stof(v_firstVertex[5]);
	Tmax = stof(v_firstVertex[6]);
	// first vertex
	p_vtx_firstVertex->id = 0;
	p_vtx_firstVertex->x = stof(v_firstVertex[1]);
	p_vtx_firstVertex->y = stof(v_firstVertex[2]);
	p_vtx_firstVertex->serviceTime = stof(v_firstVertex[3]);
	p_vtx_firstVertex->score = stof(v_firstVertex[4]);
	// last vertex (identical to 1st one except for it's id)
	p_vtx_lastVertex->id = 1;
	p_vtx_lastVertex->x = stof(v_firstVertex[1]);
	p_vtx_lastVertex->y = stof(v_firstVertex[2]);
	p_vtx_lastVertex->serviceTime = stof(v_firstVertex[3]);
	p_vtx_lastVertex->score = stof(v_firstVertex[4]);
	// only time window of both vertices.
	TW * p_tw_TurnLimits = new TW;
	TW * p_tw_TurnLimits_last = new TW;
	p_tw_TurnLimits->O = stof(v_firstVertex[5]);
	p_tw_TurnLimits->C = stof(v_firstVertex[6]);
	*p_tw_TurnLimits_last = *p_tw_TurnLimits;
	p_vtx_firstVertex->v_tws.push_back(p_tw_TurnLimits);
	p_vtx_lastVertex->v_tws.push_back(p_tw_TurnLimits_last);
	// fee is 0 for both vertices
	p_vtx_firstVertex->fee = 0; // fee
	p_vtx_lastVertex->fee = 0; // fee
	//Next values are for better init
	p_vtx_firstVertex->MaxShift = 0; // MaxShift
	p_vtx_lastVertex->MaxShift = stof(v_firstVertex[6]); // MaxShift
	p_vtx_firstVertex->Departure = 0; // Departure from First Vertex is at t = 0
	p_vtx_lastVertex->TA = 0; // Arrival time at last Vertex is at t = 0

	// both vertex belong to 0 type of vertices
	for (int i = 0; i < 10; i++) {
		p_vtx_firstVertex->v_constraint[i] = 0;
		p_vtx_lastVertex->v_constraint[i] = 0;
	}
	// adding both vertices to v_vertices
	v_vertices.push_back(p_vtx_firstVertex);
	v_vertices.push_back(p_vtx_lastVertex);


	// retrieving all other vertices informations.
	for (int i = 0; i < n_vertices; i++) {
		getline(myfile, line);
		vector<string> v_vertexInfo = split(line);
		Vertex * p_vtx_aVertex = new Vertex;
		// std infos
		p_vtx_aVertex->id = stoi(v_vertexInfo[0]);
		p_vtx_aVertex->x = stof(v_vertexInfo[1]);
		p_vtx_aVertex->y = stof(v_vertexInfo[2]);
		p_vtx_aVertex->serviceTime = stof(v_vertexInfo[3]);
		p_vtx_aVertex->score = stof(v_vertexInfo[4]);
		// time windows
		for (int j = 0; j < 4; j++) {
			TW * p_tw_atw = new TW;
			p_tw_atw->O = stof(v_vertexInfo[5 + j]);
			p_tw_atw->C = stof(v_vertexInfo[6 + j]);
			p_vtx_aVertex->v_tws.push_back(p_tw_atw);
		}
		// E and fee
		p_vtx_aVertex->e = stoi(v_vertexInfo[10]);
		p_vtx_aVertex->fee = stof(v_vertexInfo[11]); // fee
		// types
		for (int j = 0; j < 10; j++) {
			p_vtx_aVertex->v_constraint[j] = stoi(v_vertexInfo[12 + j]);
		}
		v_vertices.push_back(p_vtx_aVertex);
	}
}


void convertMTWtoSTW(vector<Vertex *> & v_vertices, vector<Vertex *> & v_vertices_STW) {

	for (auto it_p_vtx = v_vertices.begin(); it_p_vtx != v_vertices.end(); it_p_vtx++) {
		vector<Vertex *> v_p_vtx_Family;
		
		// Copying vertices Infos
		for (int i_p_tw = 0; i_p_tw < (*it_p_vtx)->v_tws.size(); i_p_tw++) {
			Vertex * p_vtx_v = new Vertex;
			p_vtx_v->id = (*it_p_vtx)->id;
			p_vtx_v->x = (*it_p_vtx)->x;
			p_vtx_v->y = (*it_p_vtx)->y;
			p_vtx_v->serviceTime = (*it_p_vtx)->serviceTime;
			p_vtx_v->score = (*it_p_vtx)->score;
			p_vtx_v->v_tws.push_back((*it_p_vtx)->v_tws[i_p_tw]);
			for (int i = 0; i < 11; i++) p_vtx_v->v_constraint[i] = (*it_p_vtx)->v_constraint[i];

			// special treatement for e.
			//  
			if ((*it_p_vtx)->e == 0) { // -> turn with even index are associated to TW with odd index and vice versa
				p_vtx_v->e = (i_p_tw + 1) % 2;
			}
			else { // -> turn with even index are associated to TW with even index and vice versa
				p_vtx_v->e = i_p_tw % 2;
			}


			v_p_vtx_Family.push_back(p_vtx_v);
			v_vertices_STW.push_back(p_vtx_v);
		}
		
		// Association of vertices from a same family.
		for (auto it_p_vtx = v_p_vtx_Family.begin(); it_p_vtx != v_p_vtx_Family.end(); it_p_vtx++) {
			(*it_p_vtx)->v_p_vtx_Family = v_p_vtx_Family;
		}

	}
}


void initSTWVertices(vector<Vertex *> & v_vertices_STW, int n_Turns) {


	for (auto it_p_vtx = v_vertices_STW.begin() + 2; it_p_vtx != v_vertices_STW.end(); it_p_vtx++) {
		(*it_p_vtx)->isInserted = false;
		for (int i = 0; i < n_Turns; i++) {
			// From this point, we suppose that ALL Vertices are initially considered for insertion
			// Should it be otherwise, add initialisation here.
			//
			(*it_p_vtx)->v_c_ICN.push_back('C');
		}
	}
}


void initTurns(float n_budgetLim, float  n_Turns , Vertex * start, Vertex * finish , vector<int> & v_n_TypeConstraint, vector<Turn *> & v_Turns) {
	Turn::f_BudgetSlack = n_budgetLim;
	Turn::f_BudgetLimitation = n_budgetLim;
	for (int i = 0; i < n_Turns; i++) {
		Turn * p_Turn = new Turn(i, start, finish); // init with First and last vertex
		p_Turn->SetConstraint(v_n_TypeConstraint);
		p_Turn->Update();
		v_Turns.push_back(p_Turn);
	}
}


void initBestSolution(vector<Turn *> & v_BestSolution, int n_Turns) {
	for (int i = 0; i < n_Turns; i++) {
		v_BestSolution.push_back(new Turn());
	}
}


void insert_vtxIntoTurn(Insertion & ins, vector<Turn *> & v_Turns, vector<Vertex *> & v_vertices_STW) {

	// INSERTION
	v_Turns[ins.n_turn]->InsertVertex(ins.p_vtx);

	// UPDATING KNAPSACK CONSTRAINTS
	updateBudgetSlack(v_Turns);
	v_Turns[ins.n_turn]->UpdateKnapSlack();


	// UPDATING NEIGHBORHOOD CONSISTENCY
	
	// Vertex family.
	for (auto it_vtx = ins.p_vtx->v_p_vtx_Family.begin() ; it_vtx != ins.p_vtx->v_p_vtx_Family.end(); it_vtx++) {
		(*it_vtx)->isInserted = true;
		for (int i = 0; i < v_Turns.size(); i++) {
			(*it_vtx)->v_c_ICN[i] = 'N';
		}
		(*it_vtx)->v_c_ICN[ins.n_turn] = 'I';
	}

	// Remaining vertices
	for (auto it_vtx = v_vertices_STW.begin() + 2; it_vtx != v_vertices_STW.end(); it_vtx++) {
		if ((*it_vtx)->v_c_ICN[ins.n_turn] == 'C') { // vertices considered for insertion before
			if (!v_Turns[ins.n_turn]->IsInsertable(*it_vtx)) {
				(*it_vtx)->v_c_ICN[ins.n_turn] = 'N'; // if not insertable anymore.
			}
		}
	}
}


void initRandSeed() {
	unsigned int seed = time(NULL);
	srand(seed);
}


int getRandomNumber(int lowerBound, int upperBound) {
	return (lowerBound + ( rand() % upperBound));
}


void updateBudgetSlack(vector<Turn *> v_Turns) {
	Turn::f_BudgetSlack = Turn::f_BudgetLimitation;
	for (auto it_turn = v_Turns.begin(); it_turn != v_Turns.end(); it_turn++) {
		Turn::f_BudgetSlack -= (*it_turn)->GetCost();
	}
}


void buildTurns(vector<Vertex *> & v_vertices_STW, vector<Turn *> & v_Turns, float & greed) {

	vector<Insertion> v_insertions; //--- Vector of possible insertions (max size = n_turns*n_intervals*n_verticesremaining)

	while (1) {


		// for each turn
		for (int m_turn = 0; m_turn < v_Turns.size(); m_turn++) {

			// for each interval within the turn.
			for (int j_inter = 0; j_inter < v_Turns[m_turn]->GetIntervals(); j_inter++) {

				//-------- Computing : Time importance in tour of index m_tour. ---------//
				float f_impTime = 0;
				
				vector <Vertex *> v_InsertibleVertices;

				// computation of shift for each vertex (considered for insertion and insertible in the interval)
				for (int i_vtx = 2; i_vtx < v_vertices_STW.size(); i_vtx++) {
					if (v_vertices_STW[i_vtx]->v_c_ICN[m_turn] != 'C') continue;

					float f_shift_j = v_Turns[m_turn]->GetShift(v_vertices_STW[i_vtx], j_inter); // shift generated by insertion of vertex: v_vertices_STW[i_vtx] beetween j_inter and j_inter + 1
					if (f_shift_j == -1) continue;

					// if vertex is insertible we keep it in vector below.
					v_InsertibleVertices.push_back(v_vertices_STW[i_vtx]);

					f_impTime += f_shift_j * v_vertices_STW[i_vtx]->score;
				}
				f_impTime = f_impTime / v_Turns[m_turn]->GetAvailableTime();

				//------ Computing : knapSack-constraints importance in tour of index m_tour. -----//
				float v_f_impknapZ [10] = {0,0,0,0,0,0,0,0,0,0};
				float f_impBudget = 0;
				
				// for each knapsack constraint:
				for (int knap = 0; knap < 10; knap++) {

					if (v_Turns[m_turn]->GetSlack(knap) == 0) {
						v_f_impknapZ[knap] = 0;
						continue;
					}

					// for each vertex considered for insertion
					for (auto it_vtx = v_InsertibleVertices.begin(); it_vtx != v_InsertibleVertices.end(); it_vtx++) {
						v_f_impknapZ[knap] += (*it_vtx)->v_constraint[knap] * (*it_vtx)->score;
					}
					v_f_impknapZ[knap] = v_f_impknapZ[knap] / v_Turns[m_turn]->GetSlack(knap);
				}

				for (auto it_vtx = v_InsertibleVertices.begin(); it_vtx != v_InsertibleVertices.end(); it_vtx++) {
						f_impBudget += (*it_vtx)->fee * (*it_vtx)->score;
					}
				f_impBudget = f_impBudget / Turn::f_BudgetSlack; 
 
				//  NOW COMPUTING HEURISTIC VALUE FOR EACH VERTEX INSERTION.
				for (auto it_vtx = v_InsertibleVertices.begin(); it_vtx != v_InsertibleVertices.end(); it_vtx++) {

					float f_vtx_impTime = f_impTime * v_Turns[m_turn]->GetShift(*it_vtx, j_inter);
					float f_vtx_impKnapSacks = 0;

					for (int knap = 0; knap < 10; knap++) {
						f_vtx_impKnapSacks += v_f_impknapZ[knap] * (*it_vtx)->v_constraint[knap];
					}

					// adding budget constraint.
					f_vtx_impKnapSacks += f_impBudget * (*it_vtx)->fee;

					// heuristic value for vertex it_vtx in turn m_turn at position j_inter
					float f_Hvalue = ((*it_vtx)->score*(*it_vtx)->score) / (f_vtx_impKnapSacks + f_impTime);

					Insertion insert;
					insert.heuristicScore = f_Hvalue;
					insert.index = j_inter;
					insert.n_turn = m_turn;
					insert.p_vtx = (*it_vtx);
					v_insertions.push_back(insert);

				}
			}
		}

		if (v_insertions.size() == 0) break; // means we haven't found any vertex to insert.

		// sorting v_insertions (descending order)
		sort(v_insertions.begin(), v_insertions.end(), sortInsertion);


		// max and min value.
		float f_MaxHValue = v_insertions.front().heuristicScore;
		float f_minHValue = v_insertions.back().heuristicScore;

		float f_Threshold = f_minHValue + (f_MaxHValue - f_minHValue)*greed;
		while (v_insertions.back().heuristicScore < f_Threshold) {
			v_insertions.pop_back();
		}

		// pick a random number beetween 0 and v_insertions.size() - 1 included
		int n_vtx_to_insert = getRandomNumber(0, v_insertions.size());

		insert_vtxIntoTurn(v_insertions[n_vtx_to_insert], v_Turns, v_vertices_STW);

		v_insertions.clear();
	}

}


vector<Vertex *> shakeSolution(vector<Turn *> & v_Turns, int n_Shake_StartIndex, int n_Shake_Range) {
	vector<Vertex *> v_p_removedVertices;
	for (int i_turn = 0; i_turn < v_Turns.size(); i_turn++) {
		vector<Vertex *> v_removedID_PARTIAL = v_Turns[i_turn]->RemoveVertices(n_Shake_StartIndex, n_Shake_Range);
		v_p_removedVertices.insert(v_p_removedVertices.end(), v_removedID_PARTIAL.begin(), v_removedID_PARTIAL.end());
	}
	for (auto it_turn = v_Turns.begin(); it_turn != v_Turns.end(); it_turn++) {
		(*it_turn)->Update();
	}
	updateBudgetSlack(v_Turns);

	return v_p_removedVertices;
}


void updateVerticesCompatibility(vector<Vertex *> v_vertices_STW,vector<Vertex *> v_p_removedVertices, vector<Turn *>v_Turns) {

	// uninserting vertices Family of erased Vertices.
	for (auto it_vtx = v_p_removedVertices.begin(); it_vtx != v_p_removedVertices.end(); it_vtx++) {
		for (auto it_vtx_fam = (*it_vtx)->v_p_vtx_Family.begin(); it_vtx_fam != (*it_vtx)->v_p_vtx_Family.end(); it_vtx_fam++) {
			(*it_vtx_fam)->isInserted = false;
			for (int i_turn = 0; i_turn < v_Turns.size(); i_turn++) {
				(*it_vtx_fam)->v_c_ICN[i_turn] = 'N'; // forces constraint re-check
			}
		}
	}

	// recheck constraint for each non inserted vertex.
	for (auto it_vtx = v_vertices_STW.begin() + 2; it_vtx != v_vertices_STW.end(); it_vtx++) {
		if ((*it_vtx)->isInserted) {
			continue;
		}

		// for each turn
		for (int i_turn = 0; i_turn < v_Turns.size(); i_turn++) {
			if ((*it_vtx)->v_c_ICN[i_turn] == 'C') { // if vertex is already considered, nothing to be done.
				continue;
			}
			else if (v_Turns[i_turn]->IsInsertable((*it_vtx))) {
				(*it_vtx)->v_c_ICN[i_turn] = 'C';
			}
		}
	}
}


void updateShake(int & n_Shake_StartIndex, int & n_Shake_Range, vector<Turn *> & v_Turns, int n_vertices) {

	// getting smallest turn size.
	int n_SmallestTurnSize = v_Turns[0]->GetIntervals() + 1;
	for (int i_turn = 1; i_turn < v_Turns.size(); i_turn++) {
		n_SmallestTurnSize = min(n_SmallestTurnSize, v_Turns[i_turn]->GetIntervals() + 1);
	}
	n_Shake_StartIndex += n_Shake_Range;
	n_Shake_Range++;
	if (n_Shake_StartIndex >= n_SmallestTurnSize) {
		n_Shake_StartIndex -= n_SmallestTurnSize;
		n_SmallestTurnSize = max(n_SmallestTurnSize, 1); // for n_SmallestTurnSize to be at least 1.
	}
	if (n_Shake_Range == int(n_vertices / (3 * v_Turns.size()))) {
		n_Shake_Range = 1;
	}
}