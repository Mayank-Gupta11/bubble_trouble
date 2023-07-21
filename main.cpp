#include <simplecpp>
#include "shooter.h"
#include "moderator.h"
//#include <windows.h>

/* Simulation Vars */
double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets, int &score){
    // this function moves all bullets, added the score to reduce the score of the user by 10 each time the bullet doesn't hit any bubble and leaves the canvas

    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME, score)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles, unsigned int level){
    // this function moves all bubbles, added level to modify acceleration of the bubble in y direction based on the level
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME, level);
    }
}

vector<Bubble> create_bubbles(unsigned int BUBBLE_RADIUS,unsigned int BUBBLE_VX, unsigned int BUBBLE_VY, Color col, unsigned int level)
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
	int vx=BUBBLE_VX;   //converted the unsigned int BUBBLE_VX to int to pass negative velocity to the function

	//here color col is also called from main program to change the color in every level and every replay, it is called from the random function in the header file moderator.h

	bubbles.push_back(Bubble(3*WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_RADIUS, -vx, BUBBLE_VY, col));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_RADIUS, vx, BUBBLE_VY, col));
	
	if (level>2){
		bubbles.push_back(Bubble(WINDOW_X/3.0, BUBBLE_START_Y, BUBBLE_RADIUS, vx, BUBBLE_VY, col));
		bubbles.push_back(Bubble(2*WINDOW_X/3.0, BUBBLE_START_Y, BUBBLE_RADIUS, -vx, BUBBLE_VY, col));
	}
    return bubbles;
}


int ind_of_attack=-1;
/*
here a global variable is created to save the index of the bubble which attacked the shooter, using this over multiple iterations the value of the index can be stored and ensured that the health doesn't get 0 in 1 collision, it is assigned the value -1 when the bubble moves away from the shooter, calling a variable by reference is also possible to avoid a global variable, but this is more compact and efficient than that
*/

int clashes(vector<Bubble> &bubbles,Shooter &sh,vector<Bullet> &bullets, int &score, unsigned int &health)
{

	float shooter_x, shooter_y, shooter_radius,	shooter_width, shooter_height, shooter_head_y;

	sh.shooter_information(shooter_x, shooter_y, shooter_radius, shooter_width, shooter_height, shooter_head_y); //this collects all the information pertaining to the shooter, it calls a member function in the class shooter

	for (unsigned int i=0;i<bubbles.size();i++)
	{
		float bubble_x, bubble_y,bubble_radius,bubble_vx,bubble_vy;
		Color c;
		bubbles[i].bubble_information(bubble_x,bubble_y,bubble_radius,bubble_vx,bubble_vy,c); //this collects all the information pertaining to the bubble, it calls a member function in the class bubble

		for (unsigned int j=0; j<bullets.size();j++){


				float bullet_x,	bullet_y, bullet_width,	bullet_height;
				bullets[j].bullet_information( bullet_x, bullet_y, bullet_width, bullet_height);  //this collects all the information pertaining to the bullet, it calls a member function in the class bullet

				bool dist_in_x=(abs(bubble_x-bullet_x)<(bubble_radius+bullet_width)) , dist_in_y=(abs(bubble_y-bullet_y)<(bubble_radius+bullet_height));
				//for checking the relative distance

				if (dist_in_x and dist_in_y)
				{
					/*divides the bubble if its radius is greater than 25, else erases it, here the dividing factor is 0.4*/
					if (bubble_radius>25){
						bubbles.erase(bubbles.begin()+i);
						bubbles.push_back(Bubble(bubble_x,bubble_y, 0.4*bubble_radius, -bubble_vx, bubble_vy, c));
						bubbles.push_back(Bubble(bubble_x,bubble_y, 0.4*bubble_radius, bubble_vx, bubble_vy, c));
					}
					else{
						bubbles.erase(bubbles.begin()+i);
					}
					bullets.erase(bullets.begin()+j);
					score+=50;  //score is increased by 50 beacause of a successful attempt

				}
			}

		bool  checky=(abs(bubble_y-shooter_y+shooter_radius)<(bubble_radius+0.5*shooter_height+shooter_radius)),checkx;
		//for checking the relative distance


		/*checking separately for the circular and rectangular part of the shooter*/
		if ((bubble_y+bubble_radius) > (shooter_head_y+shooter_radius))
			checkx=(abs(bubble_x-shooter_x)<(bubble_radius+0.5*shooter_width));
		else
			checkx=(abs(bubble_x-shooter_x)<(bubble_radius+shooter_radius));


		int ind=i; //converted unsigned int i to int ind, to compare ind_of_attack and the index easily (making the same data type)
		if (checkx and checky){

			/*the following if statement checks the 1st occurrence of a contact between the bubble and the shooter
			  this also updates the health
			  score is reduced by 100
			  ind_of_attack is made i so that in the few immediate next iteration, when the bubble collides with the shooter it won't change the health
			  the shooter color is also changed to cyan in this
			*/
			if (ind_of_attack !=ind){
				ind_of_attack=ind;
				health--;
				score-=100;
				sh.changeshootercol(false);
			}

			//lost statement when health == 0
			if (health <=0){
				Text lose(200,50,"Bad luck. You have lost");
				lose.setColor(COLOR(255,0,0));
				wait(1);
				return 0;
			}

		}
		/*the following else if executes only when the bubble has left the vicinity of the shooter (because the if statement checks for the vicinity)
		it makes the global variable -1 so that no next index matches with it as the collision is complete
		changes the color of the shooter back to green
		*/
		else if (ind_of_attack==ind){
			ind_of_attack=-1;
			sh.changeshootercol(true);
		}
	}
	return 1; //to indicate that it didn't lose (any value other than 0 can do)
}


int main()
{

	unsigned int numberofrep,losstime; //number of replays user wants, number of attempts of the game, the time (in seconds) which if exceeded will result in a loss

	initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y); //initialising the canvas, out of all the loops to ensure that the same canvas is always used
	//getClick();
		//Circle c1(100,100, 50);

	while (true){
		
		cout << "Enter the number of times you want to see the replay of this game: ";
		cin >> numberofrep;

		unsigned int healthmain=3, healthreplay=3, level=1;
		 //2 variables to store the health are used (similarly for the score in the following line), 1 for the main game which the user plays and the 2nd for the replay

		int scoremain=0, scorereplay=0;

		bool loseflag=0;  //variable to indicate if the user has lost the game, easy to exit the loop with this flag

		//an infinite while loop, to make levels as long as the user doesn't lose
		while (true){
			loseflag=0;

			//replay is a structure made in the header moderator.h, which is made to store all the data required for a replay

			vector<replay> repdata; //made a vector of type replay to store all the inputs given by the user on the canvas, and the frame number at which the input was given
			unsigned int health;
			int score;
			for (unsigned int repl=0;repl<=numberofrep;repl++){
				//int fg=0;
				/*all text declarations to display the data on the canvas
				conditional message display is used to display different messages for replays and main games
				repl=0 is the only case which is the main game case (input by user)
				all other values of repl are for replays
				*/
				Text reptext(70,90, "  ");
				Text repnum(140,90, "  ");
				Text lvlrep1(220,90, "  ");
				Text lvlrep2(290,90, "  ");
				if (repl!=0){
					reptext.setMessage("REPLAY");
					repnum.setMessage(tostring(repl));
					lvlrep1.setMessage("OF LEVEL");
					lvlrep2.setMessage(tostring(level));

				}

				else{
					reptext.setMessage("LEVEL");
					repnum.setMessage(tostring(level));
				}

				Color textcol=randomcolor(); //calling a random color for the text (function in moderator.h)
				reptext.setColor(textcol);
				repnum.setColor(textcol);
				lvlrep1.setColor(textcol);
				lvlrep2.setColor(textcol);

				unsigned int bubbleradius, bubblevx, bubblevy;
				Color col;

				dataforlevel(level, bubbleradius, bubblevx, bubblevy, col, losstime); //passing the level and requesting the data of that particular level (function in moderator.h)

				Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);  //making the blue line at the bottom (surface)
				b1.setColor(COLOR(0, 0, 255));

				string msg_cmd("Cmd: _");
				Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);  //displaying the char entered
				Text timtext(50,20,"Time: ");
				Text tim(100,20, 0);  //text declaration to display the time
				Text losstim1(120,20,"/");
				Text losstim2(140,20,tostring(losstime));

				Text healtext(WINDOW_X-270,BOTTOM_MARGIN, "Health: ");
				Text heal(WINDOW_X-220, BOTTOM_MARGIN,healthmain); //text declaration to display the health
				Text maxheal(WINDOW_X-200,BOTTOM_MARGIN,"/3");
				
				Text sctext(WINDOW_X-120, BOTTOM_MARGIN, "Score: ");
				Text sc(WINDOW_X-50,BOTTOM_MARGIN,0); //text declaration to display the score

				Text lev(0.5*WINDOW_X, 0.5* WINDOW_Y, tostring(level));  //text to display the level
				wait(1);
				lev.setMessage("  ");//vanishing the level

				// Intialize the shooter
				Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

				// Initialize the bubbles
				vector<Bubble> bubbles = create_bubbles(bubbleradius, bubblevx, bubblevy, col, level);

				// Initialize the bullets (empty)
				vector<Bullet> bullets;
				//try{
				XEvent event, dumm;

				unsigned int frames=1, time=0, fgg=0;
				int trap=0, counterr=0;				

				/*frames indicate every iteration of a particular replay or a particular attempt of a game
				time is the time in seconds
				health used to indicate the health in the entire code henceforth
				score used to indicate the score in the entire code henceforth

				following this is a conditional based assignment of health and score from scoremain or scorereplay, for that particular case (similar for score)
				*/


				if (repl==0){
					health=healthmain;
					score=scoremain;
				}
				else{
					health=healthreplay;
					score=scorereplay;
				}

				// Main game loop
				while (time!=losstime)
				{
					event=dumm;
					//event=NULL;
					bool pendingEvent = checkEvent(event), checker=true;
					char c;
					// this loop traverses the loop in replays to find the input given at a corresponding frame. Once a replay is found, break has been used to exit the loop (to increase efficiency)
					if (counterr==0){
						checker=mouseButtonPressEvent(event);
					}
					if (!checker or fgg==1){
						STEP_TIME=0.02;
					if (repl!=0){
						for (unsigned int i=0; i<repdata.size();i++){
							if (repdata[i].t==frames){
								c=repdata[i].entry;
								pendingEvent=true; //making pending event true to indicate that a suitable input has been found
								break;
							}
							else
								c=' ';
						}
					}

					if (pendingEvent)
					{
						pendingEvent=false;
						//adding all the keys entered in the vector to store them for the replay

						if (repl==0){
							// Get the key pressed
							c = charFromEvent(event);
							replay forpush;
							forpush.entry=c;
							forpush.t=frames;
							repdata.push_back(forpush);

						}
						msg_cmd[msg_cmd.length() - 1] = c;
						charPressed.setMessage(msg_cmd);

						// Update the shooter
						if(c == 'a')
							shooter.move(STEP_TIME, true);
						else if(c == 'd')
							shooter.move(STEP_TIME, false);
						else if(c == 'w')
							bullets.push_back(shooter.shoot());
						else if (c=='m')
							STEP_TIME=0;
						else if(c == 'q')
							return 0;
					dumm=event;
					}
					}
					/*
					else if (mouseButtonReleaseEvent(event)){
						STEP_TIME=0.02;
						
						//getClick();
						//Circle c1(300, 300, 40);
					}
					*/
				
					else if (trap==0){
					//	STEP_TIME=0;
					trap=1;
						cout << '#'<<endl;
						getClick();
						
						
					}
					//cout << trap << ' ';
					else if (trap==1){
					while (mouseButtonReleaseEvent(event)){
							fgg=1;
							cout << '*';
						}
					}
					//fgg=0;
					// Update the bubbles
					move_bubbles(bubbles, level);

					// Update the bullets
					move_bullets(bullets, score);

					//checking for clashes between all the possible cases, clash between bubble and bullet doesn't return anything, but is checked here because the function is called in the if statement

					if (clashes(bubbles, shooter, bullets, score, health)==0){
						loseflag=1;
						break; //breaking to exit the while loop
					}
					if (check_win(bubbles)==0)
						break;  //breaking to exit the loop for a win, loseflag hasn't been changed which indicates its  a win

					frames++; //adding all the frames to calculate time

					if (frames%38==0){
						/*a second passes after approximately every 38 frames (experimental, ideally should be 50 due to STEP_TIME, but takes a bit more time due to execution of other statements)
						*/
						time +=1;
					}


					tim.setMessage(tostring(time));

					heal.setMessage(tostring(health));

					sc.setMessage(tostring(score));

					wait(STEP_TIME);
				}
				/* while loop can exit due to 2 reasons:
				1) due to time constraints, which is checked by the 1st condition in the if statement
				2) due to the break statements, teh 2nd condition ensures that this isn't the case
				(this is a if statement to declare a loss based on time)
				*/
				if (bubbles.size()!=0 && check_win(bubbles)!=0 && time==losstime){
					Text lose(200,50,"Time Up! Bad luck. You have lost");
					lose.setColor(COLOR(255,0,0));
					loseflag=1;
					wait(1);
				}

				if (repl==0)
					cout <<"Your score in level "<<level<< " is: "<< score<<endl; //returns the score in the main game in the terminal

				/*assigning the healthmain or healthreplay variables the value of health based on their respective cases.*/
				if (repl==0){
					healthmain=health;
					scoremain=score;
				}
				
				
			}
			healthreplay=health;
			scorereplay=score;
			if (loseflag==1){
					break; //for exiting the loop if the user loses (to prevent it from continuing the next level)
			}

			level++;
		}

	}
}
