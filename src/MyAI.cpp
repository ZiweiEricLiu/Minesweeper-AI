// ======================================================================
// FILE:        MyAI.cpp
//
// AUTHOR:      Jian Li
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#include "MyAI.hpp"

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent()
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
	rowDimension = _rowDimension;
	colDimension = _colDimension;
	totalMines = _totalMines;
	agentX = _agentX;
	agentY = _agentY;

	action.x = agentX;
	action.y = agentY;
	
	board = new int*[rowDimension];
	for(int i=0; i<rowDimension; i++){
		board[i] = new int[colDimension];
	}

	for(int i=0; i<rowDimension; i++){
		for(int j=0; j<colDimension; j++){
			board[i][j] = 0;
		}
	}
	
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

Agent::Action MyAI::getAction( int number )
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    	board[action.x][action.y] = 1;
	if(!danger_tiles.empty()){
		if(findFlags()){
			action = {FLAG, mine.first, mine.second};
			end = true;
			danger_tiles.clear();
			return action;
		}
	}

	if(number == 0){
		addUncoverTiles(action.x, action.y);
	}
	else if(number == -1){

	}
	else{
		if(!end){
			danger_tiles.emplace(action.x, action.y);
		}
	}

	if(!uncover_tiles.empty()){
		auto tile = *uncover_tiles.begin();
		action = {UNCOVER, tile.first, tile.second};
		uncover_tiles.erase(uncover_tiles.begin());
		return action;
	}
	else{
		return guess();		
	}

    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================

}


// ======================================================================
// YOUR CODE BEGINS
// ======================================================================
bool MyAI::isInBounds(int x, int y){
	return (x >= 0 && x < colDimension && y >= 0 && y < rowDimension);
}

set<pair<int, int>> MyAI::getNeighbours(int x, int y){
	set<pair<int, int>> ret;
	set<pair<int, int>> delta = {{-1,1},{0,1},{1,1},{-1,0},{1,0},{-1,-1},{0,-1},{1,-1}};
	for(pair<int,int> d : delta){
		int newX = x + d.first;
		int newY = y + d.second;
		if(isInBounds(newX, newY) && board[newX][newY] != 1){
			ret.emplace(newX, newY);
		}
	}
	return ret;
}

void MyAI::addUncoverTiles(int x, int y){
	for(pair<int, int> n : getNeighbours(x, y)){
		int newX = n.first;
		int newY = n.second;
		
		if(!uncover_tiles.count({newX, newY})){
			uncover_tiles.emplace(newX, newY);
		}
	}
}

bool MyAI::findFlags(){
	for(auto d : danger_tiles){
		set<pair<int, int>> n = getNeighbours(d.first, d.second);
		if(n.size() == 1){
			mine.first = (*n.begin()).first;
			mine.second = (*n.begin()).second;
			return true;
		}
	}	
	return false;
}

Agent::Action MyAI::guess(){
	for(int i=0; i<colDimension; i++){
		for(int j=0; j<rowDimension; j++){
			if(board[i][j] == 0){
				board[i][j] = 1;
				return {UNCOVER, i, j};
			}
		}
	}
	return {LEAVE, -1, -1};
}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================
