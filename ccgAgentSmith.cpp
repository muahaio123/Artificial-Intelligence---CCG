// YOUR NAME: Thanh Long Le
//
// CS 4318, spring 2021
// Agent Challenge 4: Cricket card game
//
// Rename this file and the function below.  For example, if your agent name
// is Jones, rename this ccgAgentSmith.cpp file to ccgAgentJones.cpp and the
// ccgAgentSmith function below to ccgAgentJones.  Complete your agent
// function and turn it in on Blackboard.  Random-number generation is not
// allowed; your agent must be entirely deterministic.  Feel free to create
// other agents--each in a separate .cpp file--for yours to play against,
// but turn in only one.  Test your agent(s) with
//
//    nice bash ccgBuild.bash
//
// and then
//
//    nice ./ccgRunSim
//
// Each submitted agent will play each other, batting first and batting
// second an equal number of times, to determine the standings, which will
// be posted soon after the agents are due.

#include "ccg.h"
#include <stack>
#include <vector>

namespace
{
   // If you need to define any new types or functions, put them here in
   // this unnamed namespace.  But no variables allowed!
}

// Rename and complete this agent function.
int ccgAgentSmith(Hand hand, Card lastBowledCard, bool isBatting, const MatchState &match)
{
	// Your function must end up returning a valid int between 0 and numCardsPerHand - 1.
	// No random-number generation allowed!
	// hand.getCard(0) gives the first card in your hand.
	// hand.getCard(0).getNumber() gives the number of that card, 1 to 11.
	// hand.getCard(0).getSuit() gives the suit of that card.
	// match.getRuns(0) gives the number of runs scored by player A so far.
	// numRuns(d) gives the number of runs scored when the card difference is d.
	// See the definitions of the Hand, Card and MatchState classes for more helpful functions.
	
	Card myCard;
	stack<int> matchSuit;
	vector<int> recordQuality(6, 0);
	int bestQuality = INT_MIN, bestCardIndex = 0, quality;
	int countSuits[5] = {0};	// create an array of suits
	
	// count the suits
	for (int i = 0; i < 6; ++i)
		countSuits[hand.getCard(i).getSuit()] += 1;	// get that card
	
	int bestSuit = 0, count = 0;
	
	// get the most frequent suits count
	for (int i = 0; i < 5; ++i)
	{
		if (countSuits[i] > count)
		{
			count = countSuits[i];
			bestSuit = i;
		}
	}
	
	// input the index of the cards with the most frequent suit
	for (int i = 0; i < 6; ++i)
		if (hand.getCard(i).getSuit() == bestSuit)
			matchSuit.push(i);
	
	// increase the quality of cards with the most frequent suits
	// only if there are > 2 cards with the same suit
	if (matchSuit.size() > 2)
		while (!matchSuit.empty())
		{
			recordQuality[matchSuit.top()] += 200;
			matchSuit.pop();
		}
	
	if (isBatting)	// be agressive
	{
		for (int i = 0; i < 6; ++i)
		{
			myCard = hand.getCard(i);
			quality = 0;	// reset the quality count for each card
			
			// if they have same suit, higher number is more likely to play
			if (myCard.getSuit() == lastBowledCard.getSuit())
			{
				if (myCard.getNumber() >= lastBowledCard.getNumber())
					quality += 400;
				else
					quality += 300;
			}
			// if they have different suit, higher number are played out to prevent point loss
			else
			{
				if (myCard.getNumber() >= lastBowledCard.getNumber())
					quality += 400;
				else
					quality -= 300;
			}
			
			// the fact that the card has higher number earn more points
			if (myCard.getNumber() - lastBowledCard.getNumber() > 3)
				quality += 300;
			
			// the higher the number, the more likely to go out
			quality += 79 * myCard.getNumber();
			
			// if no card on hand is the same as the last bowled card, prioritize higher number
			if (myCard.getSuit() != lastBowledCard.getSuit() && myCard.getNumber() > lastBowledCard.getNumber())
				quality += 79 * myCard.getNumber();
			
			recordQuality[i] = quality;
		}
	}
	else // is bowling
	{
		// When bowling, I will try to avoid the condition of the last bowl card
		// then I will play the one with the highest quality: smaller numbers, not same suit/number
		
		for (int whichCard = 0; whichCard < numCardsPerHand; whichCard += 1)
		{
			myCard = hand.getCard(whichCard);
			quality = 0;
			// Favour playing a card with a larger number than the last bowled card.
			if (myCard.getNumber() > lastBowledCard.getNumber())
				quality += 100;
			
			recordQuality[whichCard] = quality;
		}
	}
	
	// find the highest quality card to return
	for (int i = 0; i < 6; ++i)
	{
		if (recordQuality[i] > bestQuality)
		{
			bestQuality = recordQuality[i];
			bestCardIndex = i;
		}
	}
	
	return bestCardIndex;
}

/*

 - First, carefully comment your code above to clarify how it works.
 - Here, describe your approach and analyze it.  How exactly did you develop
   and test it?  What are its strengths and weaknesses?  Why do you expect
   it to do well against the other submitted agents?
 - Also make a note here if you talked about the assignment with anyone or
   gave or received any kind of help.
	I will be making an offensive batsman and a defensive bowler
*/
