#include<simplecpp>
#include<stdlib.h>  //included this library as the rand and srand function might not work in older versions of gcc (from std in iostream)
#include "bubble.h"
#include<sstream>
#include <time.h>
#include<cstdlib>
//structure for the replay
struct replay{
	char entry;
	double t;
};

string tostring(int n){
	stringstream ss;
    ss << n;
    string s;
    ss >> s;
    return s;
}


//function returning a random color
Color randomcolor(){
	std::srand((unsigned int)time(NULL)); //updating the random value
	int r=rand()%255, b=rand()%255 , g=rand()%255;  //mod ensures that the value is always <=255
	return COLOR(r+1,b+1,g+1);  //adding 1 in all three to prevent the rare case of pure white
}

//gives the data for a particular level
void dataforlevel(unsigned int &level,unsigned int &bubbleradius, unsigned int &bubblevx, unsigned int &bubblevy, Color &col, unsigned int &losstime){
	if (level<=4)
		bubbleradius=10+ 15*(level);

	else
		bubbleradius=10*(level);

	bubblevx=80 + 100 *level;
	bubblevy=80 + 100 *level;
	col=randomcolor();
	losstime=55-level*5;
}



//checks if the user completes a level
bool check_win(vector<Bubble> &bubbles)
{
	if (bubbles.size()==0)
		{
			Text win(200,50,"Congratulations!! You have won");
			win.setColor(randomcolor());
			wait(1);
			return 0;
		}
	return 1;
}



