#include "algorithm.h"
#include<bits/stdc++.h>
using namespace std;

// Nathan Ploos Van Amstel
// Student ID: 20010307
// Submited: 19/09/2022

// I used this enum from the puzzle.h file
// enum heuristicFunction{misplacedTiles, manhattanDistance};


// state struct to store states of board 
struct State{ 
    int cost = 0, hCost = -1, gCost = 0; // total cost F(n) = H(n) + G(n) - A-Star Search
    int state[3][3]; // Board               total cost F(n) = G(n) - Uniform Cost Search
    int x, y; // 0 pos
    string action; // stores the action the state took
    State *parent = NULL;
    
    // defualt state constructor
    State(const string element){ // 
        int n;
        n = 0;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){	
                state[i][j] = element[n] - '0';
                if(state[i][j] == 0){
                    x = j;
                    y = i;
                }
                n++;
            } 
        }
    }
    // copy costructor
    State(State *s){
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                this->state[i][j] = s->state[i][j];
            }
        }
        this->x = s->x;
        this->y = s->y;
        this->cost = s->cost;
        this->hCost = s->hCost;
        this->gCost = s->gCost;
        this->action = s->action;
        this->parent = s->parent;
    }
};

class Search{
    private:
        // struct containing operator to organise the Queue by cost
        struct Cost_Compare{ 
            bool operator()(State* const& n1, State* const& n2) {
                // return true if n1 cost is greater
                return n1->cost > n2->cost; 
            }
        };

        // Containers
        // priority_queue is the same as a heap with extra functionality
        // allows us to organise the queue by the F cost
        priority_queue<State*, vector<State*>, Cost_Compare> Queue; // Q container
        vector<State*> visitedList; // visited list

        // start and end states
        State *initial_state;
        State *final_state;

        // heuristic enum
        heuristicFunction heuristic;
       
        // statistical values
        int stateExpansions = 0, maxQLength = 0, deletions = 0;

    public:

        // A Star Search constructor
        Search(const string init, const string final, heuristicFunction heuristic){
            this->initial_state = new State(init); // set init state
            this->final_state = new State(final);  // set final state
            this->Queue.push(this->initial_state); // push init state to queue
            this->maxQLength++; // add to queue length
            this->heuristic = heuristic; // set heuristic function
        }

        // Uniform Cost Search constructor
        Search(const string init, const string final){
            this->initial_state = new State(init); // set init state
            this->final_state = new State(final);  // set final state
            this->Queue.push(this->initial_state); // push init state to queue
            this->maxQLength++; // add to queue length
        }

        // Cost Functions
        State* Calculate_H(State *n){
            int sum = 0;
            switch(heuristic){

                case misplacedTiles:
                // misplaced tiles calculation
                    for(int i = 0; i < 3; i++){
                        for(int j = 0; j < 3; j++){
                            if(n->state[i][j] != final_state->state[i][j]){
                                sum++;
                            }
                        }
                    }
                    n->hCost = sum; // assign the hCost
                    break;

                case manhattanDistance:
                // manhattan distance calculation
                    for (int i = 0; i < 3; i++) {
				        for (int j = 0; j < 3; j++) {
					        if(n->state[i][j] != 0){
						        for (int k = 0; k < 3; k++) {
							        for (int l = 0; l < 3; l++) {
								        if (n->state[i][j] == final_state->state[k][l]) {
									        sum += abs(i-k) + abs(j-l);
								        }
							        }
						        }
					        }
				        }
			        }
                    n->hCost = sum; // assign the hCost
                    break;
            }
            return n; // return the state
        }

        // basic cost function to calculate total tile cost
        int calculate_cost(State *n){
            int cost;
            cost = n->hCost + n->gCost;
            return cost;
        }   

        // swap helper function to assist in moving blank tile
        void swap(int *a, int *b){ 
            int temp = *a;
            *a = *b;
            *b = temp;
        }

        // State Actions for A-Star search
        State* move_left(State* current){
            State *curr = new State(current);
            // Move Blank tile
            swap(&curr->state[curr->y][curr->x], &curr->state[curr->y][curr->x-1]);
            curr->x = curr->x - 1;

            curr = Calculate_H(curr); // calculate hCost
            curr->gCost = curr->gCost + 1; // add depth
            curr->cost = calculate_cost(curr); // calc total tile cost
            curr->action = "L"; // assign action
            curr->parent = current; // assign parent node

            stateExpansions++;
            return curr;
        }

        State* move_right(State* current){
            State *curr = new State(current);

            // Move Blank tile
            swap(&curr->state[curr->y][curr->x], &curr->state[curr->y][curr->x+1]);
            curr->x = curr->x + 1;

            curr = Calculate_H(curr); // calculate hCost
            curr->gCost = curr->gCost + 1; // add depth
            curr->cost = calculate_cost(curr); // calc total tile cost
            curr->action = "R"; // assign action
            curr->parent = current; // assign parent node

            stateExpansions++;
            return curr;
        }

        State* move_down(State* current){
            State *curr = new State(current);

            // Move Blank tile
            swap(&curr->state[curr->y][curr->x], &curr->state[curr->y+1][curr->x]);
            curr->y = curr->y + 1;

            curr = Calculate_H(curr); // calculate hCost
            curr->gCost = curr->gCost + 1; // add depth
            curr->cost = calculate_cost(curr); // calc total tile cost
            curr->action = "D"; // assign action
            curr->parent = current; // assign parent node

            stateExpansions++;
            return curr;
        }   

        State* move_up(State* current){
            State *curr = new State(current);        

            // Move Blank tile
            swap(&curr->state[curr->y][curr->x], &curr->state[curr->y-1][curr->x]);
            curr->y = curr->y - 1;

            curr = Calculate_H(curr); // calculate hCost
            curr->gCost = curr->gCost + 1; // add depth
            curr->cost = calculate_cost(curr); // calc total tile cost
            curr->action = "U"; // assign action
            curr->parent = current; // assign parent node

            stateExpansions++;
            return curr;
        }

        // uc state actions
        State* uc_move_left(State* current){
            State *curr = new State(current);
            // Move Blank tile
            swap(&curr->state[curr->y][curr->x], &curr->state[curr->y][curr->x-1]);
            curr->x = curr->x - 1;

            curr->gCost = curr->gCost + 1; // add depth
            curr->cost = curr->gCost; // cost = gCost for uc
            curr->action = "L"; // assign action
            curr->parent = current; // assign parent node

            stateExpansions++;
            return curr;
        }

        State* uc_move_right(State* current){
            State *curr = new State(current);

            // Move Blank tile
            swap(&curr->state[curr->y][curr->x], &curr->state[curr->y][curr->x+1]);
            curr->x = curr->x + 1;

            curr->gCost = curr->gCost + 1; // add depth
            curr->cost = curr->gCost; // cost = gCost for uc
            curr->action = "R"; // assign action
            curr->parent = current; // assign parent node

            stateExpansions++;
            return curr;
        }

        State* uc_move_down(State* current){
            State *curr = new State(current);

            // Move Blank tile
            swap(&curr->state[curr->y][curr->x], &curr->state[curr->y+1][curr->x]);
            curr->y = curr->y + 1;

            curr->gCost = curr->gCost + 1; // add depth
            curr->cost = curr->gCost; // cost = gCost for uc
            curr->action = "D"; // assign action
            curr->parent = current; // assign parent node

            stateExpansions++;
            return curr;
        }   

        State* uc_move_up(State* current){
            State *curr = new State(current);        

            // Move Blank tile
            swap(&curr->state[curr->y][curr->x], &curr->state[curr->y-1][curr->x]);
            curr->y = curr->y - 1;

            curr->gCost = curr->gCost + 1; // add depth
            curr->cost = curr->gCost; // cost = gCost for uc
            curr->action = "U"; // assign action
            curr->parent = current; // assign parent node

            stateExpansions++;
            return curr;
        }

        // to check  if two state boards are equal
        bool isEqual(State *s1, State *s2){       
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    if(s1->state[i][j] != s2->state[i][j]){
                        return false;
                    }
                }
            }
            return true;
        }

        // to check if state is present in visited list
        bool isPresentInVisited(State *curr){   
            bool flag1 = false;
            for(int i = 0; i < visitedList.size(); i++){
                if(isEqual(curr, visitedList[i])){
                    return true;
                }
            }
            return false;
        }

        // to check if state is present in queue
        bool isPresentInQueue(State *s1, priority_queue<State*, vector<State*>, Cost_Compare> q){ 
            while(!q.empty()){
                State *curr = q.top();
                q.pop();
                if(isEqual(s1, curr)){
                    return true;
                }
            }
            return false;
        }

        // Uniform Cost Search algorithm
        void uc_search(){
            State *current = Queue.top();

            // check if goal
            if(isEqual(current, final_state)){
                visitedList.push_back(current);
                return;
            }

            while(!Queue.empty()){
                State* next_state;
                
                // assign to top of Queue
                next_state = Queue.top();

                // pop element off Q
                Queue.pop(); 
                maxQLength--;
                deletions++;
                
                // add to visited list
                visitedList.push_back(next_state);

                // keep track of successors states(neighbours)
                vector<State*> successors; 

                // check goal
                if(isEqual(next_state, final_state)){ 
                    return;
                } 
                // else expand state
                // expansions
                else if(next_state->x == 0 && next_state->y == 0){
                    /*
                    0 1 2    0->down
                    3 4 5    0->right
                    6 7 8
                    */
                   
                    State *down = uc_move_down(next_state);
                    State *right = uc_move_right(next_state);

                    successors.push_back(down);
                    successors.push_back(right);
                }

                else if(next_state->x == 1 && next_state->y == 0){
                    /*
                    1 0 2    0->down
                    3 4 5    0->right
                    6 7 8    0->left
                    */
                    State *down = uc_move_down(next_state);
                    State *right = uc_move_right(next_state);
                    State *left = uc_move_left(next_state);

                    successors.push_back(down);
                    successors.push_back(right);
                    successors.push_back(left);
                }

                else if(next_state->x == 2 && next_state->y == 0){
                    /*
                    1 2 0    0->down
                    3 4 5    0->left
                    6 7 8    
                    */
                    State *down = uc_move_down(next_state);
                    State *left = uc_move_left(next_state);

                    successors.push_back(down);
                    successors.push_back(left);
                }

                else if(next_state->x == 0 && next_state->y == 1){
                    /*
                    1 2 3    0->down
                    0 4 5    0->left
                    6 7 8    0->up
                    */
                    State *down = uc_move_down(next_state);
                    State *left = uc_move_left(next_state);
                    State *up = uc_move_up(next_state);

                    successors.push_back(down);
                    successors.push_back(left);
                    successors.push_back(up);
                }

                else if(next_state->x == 1 && next_state->y == 1){
                    /*
                    1 2 3    0->down
                    4 0 5    0->left
                    6 7 8    0->up, 0-right
                    */
                    State *down = uc_move_down(next_state);
                    State *left = uc_move_left(next_state);
                    State *right = uc_move_right(next_state);
                    State *up = uc_move_up(next_state);

                    successors.push_back(down);
                    successors.push_back(left);
                    successors.push_back(right);
                    successors.push_back(up);
                }

                else if(next_state->x == 2 && next_state->y == 1){
                    /*
                    1 2 3    0->down
                    4 5 0    0->left
                    6 7 8    0->up
                    */
                    State *down = uc_move_down(next_state);
                    State *left = uc_move_left(next_state);
                    State *up = uc_move_up(next_state);

                    successors.push_back(down);
                    successors.push_back(left);
                    successors.push_back(up);
                }

                else if(next_state->x == 0 && next_state->y == 2){
                    /*
                    1 2 3    
                    4 5 6    0->right
                    0 7 8    0->up
                    */
                    State *right = uc_move_right(next_state);
                    State *up = uc_move_up(next_state);

                    successors.push_back(right);
                    successors.push_back(up);
                }

                else if(next_state->x == 1 && next_state->y == 2){
                    /*
                    1 2 3    0->right
                    4 5 6    0->left
                    7 0 8    0->up
                    */
                    State *left = uc_move_left(next_state);
                    State *right = uc_move_right(next_state);
                    State *up = uc_move_up(next_state);

                    successors.push_back(left);
                    successors.push_back(right);
                    successors.push_back(up);
                }

                else if(next_state->x == 2 && next_state->y == 2){
                    /*
                    1 2 3    0->up
                    4 5 6    0->left
                    7 8 0    
                    */
                    State *up = uc_move_up(next_state);
                    State *left = uc_move_left(next_state);

                    successors.push_back(up);
                    successors.push_back(left);
                }

                for(int i=0; i<successors.size(); i++){  
                    if(isEqual(successors[i], final_state)){   
                        visitedList.push_back(successors[i]); 
                        return;                      
                    }
                    else if(!isPresentInVisited(successors[i]) && !isPresentInQueue(successors[i], Queue)){
                        Queue.push(successors[i]);
                        maxQLength++;
                    }
                }
            }
            return;
        }

        // A-Star Search algorithm
        void as_search(){
            State *current = Queue.top();

            // check if goal
            if(current->hCost == 0){
                visitedList.push_back(current);
                return;
            }

            while(!Queue.empty()){
                State* next_state;
                
                // assign to top of Queue
                next_state = Queue.top();

                // pop element off Q
                Queue.pop(); 
                maxQLength--;
                deletions++;
                
                // add to visited list
                visitedList.push_back(next_state);

                // keep track of successors states(neighbours)
                vector<State*> successors; 

                // check goal
                if(next_state->hCost == 0){ 
                    return;
                } 
                // else expand state
                // expansions
                else if(next_state->x == 0 && next_state->y == 0){
                    /*
                    0 1 2    0->down
                    3 4 5    0->right
                    6 7 8
                    */
                   
                    State *down = move_down(next_state);
                    State *right = move_right(next_state);

                    successors.push_back(down);
                    successors.push_back(right);
                }

                else if(next_state->x == 1 && next_state->y == 0){
                    /*
                    1 0 2    0->down
                    3 4 5    0->right
                    6 7 8    0->left
                    */
                    State *down = move_down(next_state);
                    State *right = move_right(next_state);
                    State *left = move_left(next_state);

                    successors.push_back(down);
                    successors.push_back(right);
                    successors.push_back(left);
                }

                else if(next_state->x == 2 && next_state->y == 0){
                    /*
                    1 2 0    0->down
                    3 4 5    0->left
                    6 7 8    
                    */
                    State *down = move_down(next_state);
                    State *left = move_left(next_state);

                    successors.push_back(down);
                    successors.push_back(left);
                }

                else if(next_state->x == 0 && next_state->y == 1){
                    /*
                    1 2 3    0->down
                    0 4 5    0->left
                    6 7 8    0->up
                    */
                    State *down = move_down(next_state);
                    State *left = move_left(next_state);
                    State *up = move_up(next_state);

                    successors.push_back(down);
                    successors.push_back(left);
                    successors.push_back(up);
                }

                else if(next_state->x == 1 && next_state->y == 1){
                    /*
                    1 2 3    0->down
                    4 0 5    0->left
                    6 7 8    0->up, 0-right
                    */
                    State *down = move_down(next_state);
                    State *left = move_left(next_state);
                    State *right = move_right(next_state);
                    State *up = move_up(next_state);

                    successors.push_back(down);
                    successors.push_back(left);
                    successors.push_back(right);
                    successors.push_back(up);
                }

                else if(next_state->x == 2 && next_state->y == 1){
                    /*
                    1 2 3    0->down
                    4 5 0    0->left
                    6 7 8    0->up
                    */
                    State *down = move_down(next_state);
                    State *left = move_left(next_state);
                    State *up = move_up(next_state);

                    successors.push_back(down);
                    successors.push_back(left);
                    successors.push_back(up);
                }

                else if(next_state->x == 0 && next_state->y == 2){
                    /*
                    1 2 3    
                    4 5 6    0->right
                    0 7 8    0->up
                    */
                    State *right = move_right(next_state);
                    State *up = move_up(next_state);

                    successors.push_back(right);
                    successors.push_back(up);
                }

                else if(next_state->x == 1 && next_state->y == 2){
                    /*
                    1 2 3    0->right
                    4 5 6    0->left
                    7 0 8    0->up
                    */
                    State *left = move_left(next_state);
                    State *right = move_right(next_state);
                    State *up = move_up(next_state);

                    successors.push_back(left);
                    successors.push_back(right);
                    successors.push_back(up);
                }

                else if(next_state->x == 2 && next_state->y == 2){
                    /*
                    1 2 3    0->up
                    4 5 6    0->left
                    7 8 0    
                    */
                    State *up = move_up(next_state);
                    State *left = move_left(next_state);

                    successors.push_back(up);
                    successors.push_back(left);
                }

                for(int i=0; i<successors.size(); i++){  
                    if(successors[i]->hCost == 0){   
                        visitedList.push_back(successors[i]); 
                        return;                      
                    }
                    else if(!isPresentInVisited(successors[i]) && !isPresentInQueue(successors[i], Queue)){
                        Queue.push(successors[i]);
                        maxQLength++;
                    }
                }
            }
            return;
        }

        // getters
        priority_queue<State*, vector<State*>, Cost_Compare> getQueue(){
            return this->Queue;
        }

        vector<State*> getVisited(){
            return this->visitedList;
        }

        State *getInit(){
            return this->initial_state;
        }

        State *getFinal(){
            return this->final_state;
        }

        int getMaxQLength(){
            return this->maxQLength;
        }

        int getStateExpansions(){
            return this->stateExpansions;
        }

        int getDeletions(){
            return this->deletions;
        }
};

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  UC with Strict Expanded List
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////

string uc_explist(string const initialState, string const goalState, int& pathLength, int &numOfStateExpansions, int& maxQLength,
    float &actualRunningTime, int &numOfDeletionsFromMiddleOfHeap, int &numOfLocalLoopsAvoided, int &numOfAttemptedNodeReExpansions){
											 
											 
    string path; // final path
    clock_t startTime;

    // start clock
	actualRunningTime = 0.0;	
    startTime = clock();

    // UC start

    State *state;
    vector<string> pathVector;

    Search search = Search(initialState, goalState);
    search.uc_search(); // begin search

    // assign last state, the last state in the visited list should be the goal state
    state = search.getVisited()[search.getVisited().size()-1]; 
    
    // iter back through solution
    while(state->parent){ 
        pathVector.push_back(state->action); // push actions
        state = state->parent; // assign to next parent
    }

    // need to iter backwards because we follow the perant states
    for(int i = pathVector.size()-1; i >= 0; i--){ 
        path += pathVector[i]; // add to path
    }

    maxQLength = search.getMaxQLength();
    numOfDeletionsFromMiddleOfHeap = search.getDeletions();
    numOfStateExpansions = search.getStateExpansions();

    // UC end
    pathLength = path.size();
    actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	return path;		
}




///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  A* with the Strict Expanded List
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////

string aStar_ExpandedList(string const initialState, string const goalState, int& pathLength, int &numOfStateExpansions, int& maxQLength,
float &actualRunningTime, int &numOfDeletionsFromMiddleOfHeap, int &numOfLocalLoopsAvoided, int &numOfAttemptedNodeReExpansions, heuristicFunction heuristic){
											 
    string path; // final path
    clock_t startTime;

    // start clock
	actualRunningTime = 0.0;	
    startTime = clock();
    // a star start

    State *state;
    vector<string> pathVector;

    Search search = Search(initialState, goalState, heuristic);
    search.as_search(); // begin search

    // assign last state, the last state in the visited list should be the goal state
    state = search.getVisited()[search.getVisited().size()-1]; 
    
    // iter back through solution
    while(state->parent){ 
        pathVector.push_back(state->action); // push actions
        state = state->parent; // assign to next parent
    }

    // need to iter backwards because we follow the perant states
    for(int i = pathVector.size()-1; i >= 0; i--){ 
        path += pathVector[i]; // add to path
    }

    maxQLength = search.getMaxQLength();
    numOfDeletionsFromMiddleOfHeap = search.getDeletions();
    numOfStateExpansions = search.getStateExpansions();

    pathLength = path.size();
    actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	return path;	
}

