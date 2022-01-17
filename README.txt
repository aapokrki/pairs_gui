Pairs_gui

!!!Remember: At the time of making this, I was a novice in programming. There are a lot of mistakes and bad decisions!!!

Program author:
Name: Aapo Kärki


Pairs_gui is a Pair memory game

GAMEPLAY:
1. 	Write a player's name to the lineEdit slot on the right and press Add. 
	A new player is created. You can add a maximum of 6 players to one game.

2. 	Select the amount of cards on the board with the slider on the top left. 
	You can choose between 2-50 even number of cards.
	(Some numbers like 38 will layout a bit weird 2*19)

3. 	Start the game by pressing Start. 
	A grid of the desired amount of cards will appear. 
	On top of the grid, is an indicator that says who'se turn it is. 
	Click on two different cards to turn them over. 
	If those cards have the same letter in them, a pair gets added to your hand (next to your name). 
	The cards you clicked get removed from the game and you may continue.
	If the cards you picked weren't a pair, they get flipped back and it will be the next player's turn.


4. 	Once the game has been played to its end, a winner with the most amount of pairs is shown on the top left.
	If there happens to be a tie, all the players in the tie are shown.

5. 	You can now press New game, which lets you to start a new game with the same players. 
	You may add players and change the card amount at this moment.

FEATURES:
	RESET:
	At any point, you may restart the gameboard by pressing Reset. 
	This deletes all players, clears the grid and sets everything to their default values.

	RANDOMSEED:
	You may add your own random seed to be used in the card layout process by clicking the checkbox on the top right of the screen and inputing a seed number. 
	To always have random seeds each game, leave the checkbox checked.

	CLOSE:
	Closes the program

	TIMER:
	At the top-middle there is a timer that shows (min : sec) the time the current game has taken. 

----------REQUIREMENTS----------

1. Atleast 2 players
2. You can see who is in turn
3. The amount of cards can be changed
4. The amount of columns and rows are as close as they can
5. You can turn cards
6. Only two cards can be turned at at time
7. Different colours on each side
8. There is a delay on how long the cards are turned
9. The score can be seen at the end and during the game
10. This document


Minimum requirements have been met. (max 50p)


Extra features are as follows:

1. The amount of players can be chosen (10p)
2. The names of the players can be chosen (5p)
3. The game status is shown graphically with cards in a QGraphicsView. (10p)
4. The program tells which cards each player has collected (5p)
5. The time used in the game is counted (time used so far)(10p)
6. A reset button (10p)

Extra: max. 50p


--------Decisions----------

The program consists of a grid area for the cards and a form area for the players. 
These areas are filled in the code. 
This ensures that we can add additional buttons and stuff outside these areas and the cards and such dont come in our way.

A slider for the amount of cards seemed like an easy solution

The size of cards is based on the size of the layout.
 This is so that the grid gets fille no matter what and doesn't feel empty with only just two cards for example.

Cards inherit the QPushbutton to make everything easier. 

Card distribution system is copied from the first project, since i found that to be sufficient. 

The formlayout for the players could've been done better, but it works quite well anyways.
A pair of rectangles is made when a player gets a point, and the pair's letter is added on top.

The random factor is done in a scuffed way, but it lets everyone enjoy the game with totally random cards, but also lets the developer test stuff with set seeds.

A max limit of 50 cards seemed enough, since some numbers would be laid out oddly. 
Also i think 6 players is quite enough for a game like this, but there is room for more.

Max player name is 10 characters, so that the name doesnt push the QGraphicsScene to the right too much

There's not much to add, so thats it.

----NOTES----
There seems to be some bugs, because of lag. 
Sometimes a debug note is printed because you somehow managed to turn a removed card. 
This is purely because of lag and won't be an issue normally. 
Also this doesn't make any problems and the game can be continued without issues

On startup there is an error message
An assistant said that these are fine:

QMetaObject::connectSlotsByName: No matching signal for on_card_clicked()
QMetaObject::connectSlotsByName: No matching signal for on_timer_timeout()














