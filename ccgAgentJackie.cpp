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

namespace
{
   // If you need to define any new types or functions, put them here in
   // this unnamed namespace.  But no variables allowed!
}

// Rename and complete this agent function.
int ccgAgentJackie(Hand hand, Card lastBowledCard, bool isBatting, const MatchState &match)
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
	int recordQuality[6] = {0};
	int bestQuality = INT_MIN, bestCardIndex = 0, quality;
	int countSuits[5] = {0};	// create an array of suits
	int noSuit = 0;	// count cards with different suits
	
	// count the suits
	for (int i = 0; i < 6; ++i)
		countSuits[hand.getCard(i).getSuit()] += 1;	// get that card
	
	int frequentSuit = 0, count = 0;
	
	// get the most frequent suit counts
	for (int i = 0; i < 5; ++i)
		if (countSuits[i] > count)
		{
			count = countSuits[i];
			frequentSuit = i;
		}
	
	// input the index of the cards with the most frequent suit
	for (int i = 0; i < 6; ++i)
		if (hand.getCard(i).getSuit() == frequentSuit)
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
			
			// if they ahve the same suit, it is already the best
			if (myCard.getSuit() == lastBowledCard.getSuit())
				quality += 200;
			
			// if they have same suit, higher number is more likely to play	
			if (myCard.getSuit() == lastBowledCard.getSuit() && myCard.getNumber() >= lastBowledCard.getNumber())
				quality += 100;
			
			// the fact that the card has higher number (same suit) earn more points
			if (myCard.getSuit() == lastBowledCard.getSuit() && myCard.getNumber() - lastBowledCard.getNumber() > 3)
				quality += 100;
			
			// if they have different suit, less likely playing it
			if (myCard.getSuit() != lastBowledCard.getSuit())
			{
				quality -= 100;
				++noSuit;
			}
			
			// if they have different suit, higher number are played out to prevent point loss
			if (myCard.getSuit() != lastBowledCard.getSuit() && myCard.getNumber() >= lastBowledCard.getNumber())
				quality += 200;
			
			if (myCard.getSuit() != lastBowledCard.getSuit() && myCard.getNumber() < lastBowledCard.getNumber())
				quality -= 100;
			
			recordQuality[i] = quality;
		}
		
		//---------------------special case----------------------
		// special case (very unlikely): when there is no card with the same suit as the last bowled card
		if (noSuit == 6)	// no card with the same suit at all
		{
			for (int i = 0; i < 6; ++i)
			{
				myCard = hand.getCard(i);
				
				// if the card is larger than it is already really good
				if (myCard.getNumber() > lastBowledCard.getNumber())
					recordQuality[i] += 200;
				
				// if my card is larger by 1 or 2
				if (myCard.getNumber() - lastBowledCard.getNumber() == 1 || myCard.getNumber() - lastBowledCard.getNumber() == 2)
					recordQuality[i] += 200;
				
				// if the my card number is smaller by 1, 2, 3
				if (myCard.getNumber() - lastBowledCard.getNumber() == -1 || myCard.getNumber() - lastBowledCard.getNumber() == -2 || myCard.getNumber() - lastBowledCard.getNumber() == -3)
					recordQuality[i] += 200;
			}
		}
		//---------------------special case----------------------
	}
	else // is bowling
	{
		// When bowling, I will try to avoid the condition of the last bowl card
		// then I will play the one with the highest quality: smaller numbers, not same suit/number
		
		for (int i = 0; i < numCardsPerHand; ++i)
		{
			myCard = hand.getCard(i);
			quality = 0;
			// Favour playing a card with a smaller number than the last bowled card.
			if (myCard.getNumber() < lastBowledCard.getNumber())
				quality += 100;
			
			if (myCard.getNumber() == lastBowledCard.getNumber())
				quality -= 200;
			
			if (myCard.getNumber() - lastBowledCard.getNumber() == 1)
				quality -= 200;
			
			// take advantage of the batter having played the card with the same suit as before
			if (myCard.getSuit() == lastBowledCard.getSuit())
				quality += 100;
			
			// the smaller the number, the more likely to go out
			quality += 5 * (11 - myCard.getNumber());
			
			// if the card on hand is the same as the last bowled card, prioritize smaller number
			if (myCard.getSuit() == lastBowledCard.getSuit() && myCard.getNumber() < lastBowledCard.getNumber())
				quality += 5 * (11 - myCard.getNumber());
			
			recordQuality[i] = quality;
		}
	}
	
	// find the highest quality card to return
	for (int i = 0; i < 6; ++i)
		if (recordQuality[i] > bestQuality)
		{
			bestQuality = recordQuality[i];
			bestCardIndex = i;
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
	I will be making an agressive batsman (prioritize higher number) and a defensive bowler (prioritize lower number)
		- before that, at the start, it will find out which suit appears the most (suit > 3) and increase their chance of returning
		- base on the rules of the bowler, I follow it, give the cards minus points if called "wide" or "bye"
			- prioritize lower number cards
		- for the batter, i prioritize higher number, with the same suit
			- a special case: when none of the cards on hand have the same suit as the last bowled card
				- then: prioritize higher number (especially by 1 or 2) to minimize the loss of a higher number card
				- prioritize lower cards with difference no more than 3 to only lose 1 points
*/
