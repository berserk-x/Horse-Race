//compile with g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system -o horserace
// run with ./horserace
#include<SFML/Graphics.hpp>
#include<vector>
#include<tuple>
#include<ctime>
#include "animatedgif.cpp"
using namespace std;

////// horse race skeleton

const int HN = 6;  // number of horses in race, numbered 0-5
const int wwidth = 1024; // window dimensions in pixels
const int wheight = 680;  

int index_of_highest(int A[], int len) {
    if (len<1) return -1;
    int an = 0;
    int mi = 0;
    for(int i=1;i<len;i++) { 
        if (A[i] > A[mi]) { mi = i; an = i;}
        else if (A[i] == A[mi]) { an = -1; }
        }
        return an;
}

int main(int argc, char** argv)
{
    using namespace sf;
    srand((unsigned int)time(0) % 10000); //seed random number generator
    AnimatedGIF gif("racer.gif");
    Vector2i size = gif.getSize();
    RenderWindow window(VideoMode(wwidth, wheight), "Horse Race");
    window.setFramerateLimit(30);
    Sprite sprite;
    int finishline = wwidth - 20; // position of finish line
    Vertex lines[HN*2+2]; // 2 vertices for each line
    int gap = (wheight) / HN; // distance between horizontal lines
    for (int i = 0; i < HN * 2; i += 2) { // draw all lines
        lines[i] = Vertex(Vector2f(0.0f, 2 + (i / 2) * gap), sf::Color::Yellow);
        lines[i+1] = Vertex(Vector2f(wwidth-1, 2 + (i / 2) * gap), sf::Color::Yellow);
    }
    // last two vertices represent finishline
    lines[HN * 2] = Vertex(Vector2f(finishline, 2), Color::Red);
    lines[HN * 2 + 1] = Vertex(Vector2f(finishline, wheight - 2), Color::Red);
    sf::Font font;
    font.loadFromFile("Arial.ttf");
    sf::Text HNames[HN];
    for (int i = 0; i < HN; i++) {
        HNames[i].setFont(font);
        HNames[i].setString(std::to_string(i) + ":");
        HNames[i].setCharacterSize(24);
        HNames[i].setPosition(0, 12 + (i * gap));
        HNames[i].setFillColor(Color::Blue);
    }
    sf::Text bottommsg;
    bottommsg.setFont(font);
    bottommsg.setCharacterSize(24);
    bottommsg.setFillColor(sf::Color::Blue);
    //bottommsg.setStyle(sf::Text::Bold);
    bottommsg.setPosition(300, wheight - 30);


    /////// Code pertaining to your assignment...

    int HX[HN]; // x position of each horse - set initial position of horse
    for (int i = 0; i < HN; i++) HX[i] = 0; // horizontal position of horse i
    bool raceover = false;  // set this to true when race is over.  The
    // race is over if any HX[i] > finishline- size.x + 5.

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }// event detection loop

        if (!raceover) gif.update(sprite); // move the legs
        window.clear(sf::Color::Green);    // erase background and draw lines
        window.draw(lines, 2 * HN+2, sf::Lines); // display names (numbers)
        for (int i = 0; i < HN; i++) {
            window.draw(HNames[i]);
        }
        for (int h = 0; h < HN; h++) { // draw each horse
            sprite.setPosition(HX[h], h * gap +((gap-size.y)/2));
            window.draw(sprite);
        }// draw each horse
        
	// you should first check the raceover variable before proceeding to
	// the next step:
        int wi = -1;
        if (!raceover) {
            
            for (int k=0; k<6; k++) {
                HX[k] = HX[k] + 1+rand()%6;
                if (HX[k] >= finishline - size.x + 5) {
                    raceover = true;
                }
                wi = index_of_highest(HX, 6);
                bottommsg.setString("The horse in the lead is " + std::to_string(wi));
                window.draw(bottommsg);
            }
            
        }
	
	// If the race is not over, move each horse forward by a random amount:
	// rand() generates a random integer.  rand()%N gives a number between
	// 0 and N-1.  Experiment with values to get good results: hint:
	// 1+rand()%N gives a number between 1 and N.
	// You also need to set the raceover to true if the race is over.
	// The race is over if a horse crosses the finish line, something like
	//   if (H[i] >= finishline- size.x + 5) raceover = true;

        // The X,Y coordinates of each horse indicates the upper-left hand
	// corner of the box containing the animated gif.
	   if (raceover == true) {
        wi = index_of_highest(HX, 6);
        bottommsg.setString("The winner is " + std::to_string(wi));
        window.draw(bottommsg);
       }

	// determine winner by finding out which horse is in the lead.  Note
	// that you do NOT want to combine this step with the above loop
	// because you want to give every horse a chance to move forward.

	// print out who's winning..

	// Challenge: determine if there's a tie and which horses are tied
	// for the lead, and display an appropriate message
	
        window.display(); // this line must be at the very end of the loop
    }//while window is open

    return EXIT_SUCCESS;
}//main