#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <math.h>

#include "Turn.h"
#include "types.h"
#include "functions.h"
#include "Config.h"


extern bool TRACE_TOUR;
extern bool TRACE_INSERT;


// ----- INIT OF CLASS ATTRIBUTES ----- //

float Turn::f_BudgetSlack;
float Turn::f_BudgetLimitation;
int Turn::n_instance_count = 0;


// ----- CONSTRUCTOR AND DESTRUCTOR ----- //

Turn::Turn() {
	this->n_id = -1;
	this->f_cost = 0;
	this->isBestTurn = true;
	Turn::n_instance_count++;
}


Turn::~Turn(){
	#ifndef TEST
		if (this->isBestTurn) {
			for (auto it_vtx = this->v_vertices.begin(); it_vtx != this->v_vertices.end(); it_vtx++) {
				delete (*it_vtx);
			}
		}
		else {
			delete (this->v_vertices.front());
			delete (this->v_vertices.back());
		}
	#endif
	Turn::n_instance_count--;
}


Turn::Turn(int n_id, Vertex * vtx_v0, Vertex * vtx_vN) {
	this->n_id = n_id;
	this->v_vertices.push_back(copyVertex(vtx_v0)); // adding first vertex
	this->v_vertices.push_back(copyVertex(vtx_vN)); // adding last vertex
	this->isBestTurn = false;
	Turn::n_instance_count++;
}


Turn::Turn(bool test) {
	if (!test) {
		cerr << "ERROR trying to use wrong Turn class constructor" << endl;
	}
	this->n_id = 0;
	this->isBestTurn = false;
	Turn::n_instance_count++;
}



//-------- GETTERS and SETTERS ----- //

float Turn::GetAvailableTime() {
	return f_availableTime;
}


float Turn::GetCost() {
	return this->f_cost;
}


int Turn::GetIntervals() {
	return this->v_vertices.size() - 1;
}


float Turn::GetScore() {
	float f_score = 0;
	for (auto it_vtx = this->v_vertices.begin(); it_vtx != this->v_vertices.end(); it_vtx++) {
		f_score += (*it_vtx)->score;
	}
	return f_score;
}


float Turn::GetSlack(int i_knap) {
	return this->v_n_Slack[i_knap];
}


Vertex * Turn::GetVertexAt(int index) {
	return this->v_vertices.at(index);
}


void Turn::SetConstraint(vector<int> & v_n_TypeConstraint) {
	// HERE, the budget constraint is added to the set of n_type constraint because both constraints types are treated the same way.
	this->v_n_TypeConstraint = v_n_TypeConstraint;
	this->v_n_Slack = v_n_TypeConstraint;
}



// ----- UPDATES ----- //

void Turn::Update() {

	// UPDATING VERTICES TIME INFOS
	this->UpdateVerticesTimeRelatedInfos();

	// UPDATING Available Time remaining
	this->UpdateAvailableTime();

	// UPDATING TURN knapsack Constraints slacks
	this->UpdateKnapSlack();

	// UPDATING overall cost
	this->UpdateCost();
}


void Turn::UpdateAvailableTime() {
	this->f_availableTime = 0;
	for (auto it_vtx = this->v_vertices.begin(); it_vtx != this->v_vertices.end(); it_vtx++) {
		this->f_availableTime += (*it_vtx)->Wait;
	}
	this->f_availableTime += this->v_vertices.back()->MaxShift;
}


void Turn::UpdateCost() {
	this->f_cost = 0;
	for (auto it_vtx = this->v_vertices.begin() + 1; it_vtx != this->v_vertices.end(); it_vtx++) {
		this->f_cost += (*it_vtx)->fee;
	}
}


void Turn::UpdateKnapSlack() {
	//reset n_types slacks.
	for (int i = 0; i < 10; i++) {
		this->v_n_Slack[i] = this->v_n_TypeConstraint[i];
	}

	for (auto it_vtx = this->v_vertices.begin(); it_vtx != this->v_vertices.end(); it_vtx++) {
		for (int i = 0; i < 10; i++) {
			this->v_n_Slack[i] -= (*it_vtx)->v_constraint[i];
		}
	}
}


void Turn::UpdateVerticesTimeRelatedInfos() {

	// TA - Wait - Departure
	Vertex * p_vtx_first = this->v_vertices.front();
	Vertex * p_vtx_last = this->v_vertices.back();
	p_vtx_first->TA = p_vtx_first->v_tws[0]->O;
	p_vtx_first->Departure = p_vtx_first->TA + p_vtx_first->serviceTime;

	// case : only 2 vertices in turn.
	p_vtx_last->TA = p_vtx_first->Departure + getDistance(p_vtx_first, p_vtx_last) / f_VISITOR_SPEED;

	for (int i_vtx = 0; i_vtx < this->v_vertices.size() - 1; i_vtx++) {
		Vertex * p_vtx_current = this->v_vertices[i_vtx];
		Vertex * p_vtx_next = this->v_vertices[i_vtx + 1];

		p_vtx_current->Wait = fmax(0, p_vtx_current->v_tws[0]->O - p_vtx_current->TA);
		p_vtx_current->Departure = p_vtx_current->TA + p_vtx_current->Wait + p_vtx_current->serviceTime;
		p_vtx_next->TA = p_vtx_current->Departure + getDistance(p_vtx_current, p_vtx_next) / f_VISITOR_SPEED;
	}

	// convention: last vertex waiting time = 0 and last vertex departure time = TA
	p_vtx_last->Wait = 0;
	p_vtx_last->Departure = p_vtx_last->TA + p_vtx_last->serviceTime;
	p_vtx_last->MaxShift = p_vtx_last->v_tws[0]->C - p_vtx_last->TA;

	// MaxShift
	for (int i_vtx = this->v_vertices.size() - 2; i_vtx >= 0; i_vtx--) {
		Vertex * p_vtx_current = this->v_vertices[i_vtx];
		Vertex * p_vtx_next = this->v_vertices[i_vtx + 1];
		p_vtx_current->MaxShift = fmin(p_vtx_current->v_tws[0]->C - (p_vtx_current->TA + p_vtx_current->Wait), p_vtx_next->MaxShift + p_vtx_next->Wait);
	}
}



// ----- TIME AND CONSTRAINTS MANAGEMENT ----- //

int Turn::GetBestShift(Vertex * p_vtx, float & bestShift) {
	int index = -1;
	for (int i = 0; i < this->v_vertices.size() - 1; i++) {
		float shift_i = this->GetShift(p_vtx, i);
		if (shift_i > 0 && bestShift > shift_i) {
			index = i;
			bestShift = shift_i;
		}
	}
	return index;
}


float Turn::GetShift(Vertex * p_vtx, int index) {

	float f_shift_j = -1;



	if (index >= 0 && index < this->v_vertices.size() - 1) {

		Vertex * p_vtx_previous = this->v_vertices[index];
		Vertex * p_vtx_next = this->v_vertices[index + 1];

		float f_t_ij = getDistance(p_vtx, p_vtx_previous) / f_VISITOR_SPEED;
		float f_t_jk = getDistance(p_vtx, p_vtx_next) / f_VISITOR_SPEED;
		float f_t_ik = getDistance(p_vtx_previous, p_vtx_next) / f_VISITOR_SPEED;
		//cout << f_t_ij << " " << f_t_jk << " " << f_t_ik << endl;

		float f_TA_j = p_vtx_previous->Departure + f_t_ij / f_VISITOR_SPEED; // FICTIVE arrival time at vertex j
		//cout << f_TA_j << endl;

		if (f_TA_j > p_vtx->v_tws[0]->C) { // Disrespect TW closing time.
			//cout << " fail here 1";
			return -1;
		}

		float f_wait_j = fmax(0, p_vtx->v_tws[0]->O - f_TA_j); // FICTIVE waiting time at vertex j
		//cout << f_wait_j << endl; 

		float f_Departure_j = p_vtx_previous->Departure + f_t_ij + f_wait_j + p_vtx->serviceTime; // FICTIVE departure time from vertex j
		//cout << f_Departure_j << endl;
		
		if (f_Departure_j + getDistance(p_vtx, this->v_vertices.back()) > this->v_vertices.back()->v_tws[0]->C) { // Disrespect maximal tour duration
			return -1;
			//cout << " fail here 2";
		}

		f_shift_j = f_t_ij + f_wait_j + p_vtx->serviceTime + f_t_jk - f_t_ik;
		//cout << f_shift_j << endl;

		float wait_k = p_vtx_next->Wait;
		float maxShift_k = p_vtx_next->MaxShift;
		//cout << wait_k << " " << maxShift_k << endl;
		
		if (f_shift_j > maxShift_k + wait_k) { // Disrespect Time constraints of the following vertices
			return -1;
			//cout << " fail here 3";
		}
	}

	return f_shift_j;
}


void Turn::InsertVertex(Vertex * p_vtx, int index) {

	if (p_vtx->v_c_ICN[this->n_id] == 'N' || p_vtx->v_c_ICN[this->n_id] == 'I') { // cas ou le sommet a été jugé non insérable ou il est déjà inclu dans un tour.
		return;
	}

	float bestShift = numeric_limits<float>::max(); // BIG SHIFT
	if (index == -1) {
		index = GetBestShift(p_vtx, bestShift);
	}
	else {
		bestShift = GetShift(p_vtx,index);
		//cout << "bestshift is " << bestShift << endl;
	}

	if (index == -1) { // cas ou le sommet n'est pas insérable.
		cerr << "ERROR : The vertex considered for insertion is not insertible " << endl;
		return;
	}

	if (TRACE_INSERT) {
		cout << " In turn n" << this-> n_id << " - INSERTION of vertex n" << p_vtx->id << " at index " << index + 1<< endl;
	}


	//cout << "BestShift is " << bestShift << endl;
	// Insertion cf Algo décrit page 6.
	//
	this->v_vertices.insert(this->v_vertices.begin() + index + 1, p_vtx); 
	p_vtx->isInserted = true;

	//this->UpdateVerticesTimeRelatedInfos();
	// the following lines are bugged
	
	// Update inserted vertex
	p_vtx->TA = this->v_vertices[index]->Departure + getDistance(this->v_vertices[index], p_vtx) / f_VISITOR_SPEED;
	p_vtx->Wait = fmax (p_vtx->v_tws[0]->O - p_vtx->TA , 0);
	p_vtx->Departure = p_vtx->TA + p_vtx->Wait + p_vtx->serviceTime;

	// Update FOLLOWING vertices
	for (int k = index + 2; k < this->v_vertices.size() && bestShift > 0; k++) {
		Vertex * p_vtx_k = this->v_vertices[k];
		float old_wait = p_vtx_k->Wait;
		p_vtx_k->Wait = fmax(0, old_wait - bestShift);
		p_vtx_k->TA = p_vtx_k->TA + bestShift;
		bestShift = fmax(0, bestShift - old_wait);
		p_vtx_k->Departure = p_vtx_k->Departure + bestShift;
	}

	// Update PREVIOUS vertices
	this->v_vertices.back()->MaxShift = this->v_vertices.back()->v_tws[0]->C - this->v_vertices.back()->TA;
	for (int i = this->v_vertices.size() - 2 ; i >= 0; i--) {
		Vertex * p_vtx_i = this->v_vertices[i];
		Vertex * p_vtx_next = this->v_vertices[i + 1];
		p_vtx_i->MaxShift = fmin(p_vtx_i->v_tws[0]->C - (p_vtx_i->TA + p_vtx_i->Wait),
			p_vtx_next->MaxShift + p_vtx_next->Wait);

		// sleep for a while
		if (p_vtx_i->MaxShift < 0){
			this->Print();
			cout << "stop 2" << endl;
			cout << p_vtx->id << endl;
			cout << p_vtx_i->MaxShift << " :: "
				 << p_vtx_i->v_tws[0]->C << " :: "
				 << p_vtx_i->TA << " :: "
				 << p_vtx_next->Wait << " :: "
				 << p_vtx_next->MaxShift
				 << endl;
			exit(0);
		}
		
	}
	
}


bool Turn::IsInsertable(Vertex * p_vtx) {
	bool result = true;

	if (p_vtx->e != (this->n_id % 2)) {
		return false;
	}


	for (int i = 0; i < 10; i++) {
		result = result && (int(p_vtx->v_constraint[i]) <= this->v_n_Slack[i]);
	}
	result = result && (Turn::f_BudgetSlack >= p_vtx->fee);

	return result;
}



// ----- VALIDATION ----- //

bool Turn::isTurnValid(bool checkTime , bool checkConstraints) {
	bool b_testResult = true;

	if (checkTime) {
		// Checks that turn is correctly built
		for (int i = 0; i < this->v_vertices.size() - 2; i++) {
			Vertex * p_vtx_current = this->v_vertices[i];
			Vertex * p_vtx_next = this->v_vertices[i + 1];
			// testing TA, Wait and MaxShift from s0 to 1 before last s.
			b_testResult = b_testResult && (int(p_vtx_current->Departure + getDistance(p_vtx_current, p_vtx_next) / f_VISITOR_SPEED) == int(p_vtx_next->TA));
			b_testResult = b_testResult && (int(p_vtx_current->Wait) == int(fmax(0, p_vtx_current->v_tws[0]->O - p_vtx_current->TA)));
			b_testResult = b_testResult && (int(p_vtx_current->MaxShift) == int(fmin(p_vtx_current->v_tws[0]->C - (p_vtx_current->TA + p_vtx_current->Wait), p_vtx_next->MaxShift + p_vtx_next->Wait)));
		}

		// Checks that turn respects time constraints
		for (int i = 0; i < this->v_vertices.size(); i++) {
			Vertex * p_vtx_current = this->v_vertices[i];
			b_testResult = b_testResult && (p_vtx_current->TA <= p_vtx_current->v_tws[0]->C);
		}
	}
	
	if (checkConstraints) {
		// Checks constraint validation
		int v_constraint[10] = { 0,0,0,0,0,0,0,0,0,0 };
		float total_fee = 0;
		for (int i = 0; i < this->v_vertices.size(); i++) {
			Vertex * p_vtx_current = this->v_vertices[i];
			for (int j = 0; j < 10; j++) {
				v_constraint[j] += p_vtx_current->v_constraint[j];
			}
			total_fee += p_vtx_current->fee;
		}
		for (int i = 0; i < 10; i++) {
			//b_testResult = b_testResult && (v_constraint[i] <= this->v_n_TypeConstraint[i]);
		}
		b_testResult = b_testResult && total_fee <= this->f_BudgetLimitation;

	}
	
	return b_testResult;
}


void Turn::_testPurpose_InsertVertex(Vertex * p_vtx, int index) {
	if (index < 0 || index > this->v_vertices.size() - 1) {
		index = this->v_vertices.size();
	}

	this->v_vertices.insert(this->v_vertices.begin() + index , p_vtx);
}


// UTILITIES

void Turn::Copy(Turn * p_turn) {
	
	// copy turn id
	this->n_id = p_turn->n_id;

	// Release of vertices
	for (int i_vtx = 0; i_vtx < this->v_vertices.size(); i_vtx++) {
		delete (this->v_vertices[i_vtx]);
	}
	this->v_vertices.clear();

	// Copy Vertices
	for (int i = 0; i < p_turn->v_vertices.size(); i++) {
		this->v_vertices.push_back(copyVertex(p_turn->v_vertices[i]));
	}

	// Copy constraints
	this->v_n_Slack = p_turn->v_n_TypeConstraint;
	this->v_n_TypeConstraint = p_turn->v_n_TypeConstraint;

	// updates other parameters
	this->Update();
}


void Turn::Print() {
	cout << endl << "Composition of turn " << this->n_id << endl;
	for (auto it = this->v_vertices.begin(); it != v_vertices.end(); it++) {
		cout << (*it)->id << " : id as STW = " << (*it)->id_STW << " : TA  = " << (*it)->TA << "  Wait = " << (*it)->Wait << "  Departure = " << (*it)->Departure << "  MaxShift = " << (*it)->MaxShift << "  O,C = " << (*it)->v_tws[0]->O << " " << (*it)->v_tws[0]->C << endl;
	}
	cout << endl;
}


vector<Vertex *> Turn::RemoveVertices(int n_start, int n_range) {
	vector<int> v_VtxToRemove;
	vector<Vertex *> v_p_RemovedVtx;
	int n_removed = 0;
	n_range = min(n_range, int(this->v_vertices.size() - 2));

	for (int i = 0; i < n_range; i++) {
		v_VtxToRemove.push_back(1 + (n_start - 1 + i) % (this->v_vertices.size() - 2));
	}
	sort(v_VtxToRemove.rbegin(), v_VtxToRemove.rend());

	for (int i = 0; i < v_VtxToRemove.size(); i++) {

		this->v_vertices[v_VtxToRemove[i]]->v_c_ICN[this->n_id] = 'C';
		v_p_RemovedVtx.push_back(v_vertices[v_VtxToRemove[i]]);

		if (TRACE_INSERT) {
			cout << " In turn n" << this-> n_id << " - DELETION of vertex n" << this->v_vertices[v_VtxToRemove[i]]->id<< endl;
		}

		this->v_vertices.erase(this->v_vertices.begin() + v_VtxToRemove[i]);
	}

	return v_p_RemovedVtx;
}

