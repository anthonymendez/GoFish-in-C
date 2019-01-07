# GoFish in C
## Fall 2018
## ECE373 Assignment 2

### Developers
* [Anthony Mendez](https://github.com/anthonymendez)
* [Joshua Howell](https://github.com/Parzival6)

## Description
GoFish in C is a Human vs Computer card game of GoFish created in C. You are the Human. The program is the computer. 

This program is the second assignment in ECE 373 Software Intensive Engineering. We were given basic header files with prototype functions and structs. With these and the guidelines provided by the assignment, we designed the game loop, filling in the functions, modified the struct, fixed edge cases. And created this.

## How To Play
1. Run `make`
2. Run `./gofish`
3. Game will begin, the deck is shuffled and you are presented with the following (an example)
```
Player 1's Hand - 5S 3H JC 9D 9H AS QC
Player 1's Book -
Player 2's Book -
Player 1's turn, enter a Rank: 
```
You must enter a rank that is in your hand that might be in the other player's hand. Entering `5` could present the following output
```
Player 1's turn, enter a Rank: 5
    - Player 2 has no 5's    
    - Go Fish, Player 1 draws 7D    
    - Player 2's turn    

Player 1's Hand - 5S 3H JC 9D 9H AS QC 7D
Player 1's Book -
Player 2's Book -
Player 2's turn, enter a Rank: 9
    - Player 1 has 9D, 9H    
    - Player 2 has 9C, 9S    
    - Player 2 books 9    
    - Player 2 gets another turn    

Player 1's Hand - 5S 3H JC AS QC 7D
Player 1's Book -
Player 2's Book - 9
Player 2's turn, enter a Rank: 4
    - Player 1 has no 4's    
    - Go Fish, Player 2 draws a card    
    - Player 1's turn    

Player 1's Hand - 5S 3H JC AS QC 7D
Player 1's Book -
Player 2's Book - 9
Player 1's turn, enter a Rank: 
```
The game takes care of fishing for cards. 

Keep playing until a player wins like so:
```
Player 1's Hand - 8D
Player 1's Book - 3 2 J A 6 Q
Player 2's Book - 9 5 10 4 5 K
Player 1's turn, enter a Rank: 8
    - Player 2 has 8H, 8S, 8C
    - Player 1 has 8D
    - Player 1 books 8
    - Player 1 gets another turn

Player 1's Hand -
Player 1's Book - 3 2 J A 6 Q 8
Player 2's Book - 9 5 10 4 5 K
Player 1 Wins! 7-6

Do you want to play again [Y/N]: 
```
Entering Y will start a new game.

Entering N will exit the program.
