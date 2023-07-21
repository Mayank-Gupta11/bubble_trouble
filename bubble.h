#include <simplecpp>

/* Bubble Vars */
const int BUBBLE_START_X = 250;
const int BUBBLE_START_Y = 50;
const int BUBBLE_DEFAULT_RADIUS = 60;
const int BUBBLE_RADIUS_THRESHOLD = 10;
const int BUBBLE_DEFAULT_VX = 100;
const int BUBBLE_DEFAULT_VY = 100;

class Bubble
{
private:
    Circle circle;  // the circle representing the bubble
    double vx, vy;  // velocity in x and y direction
    Color color;    // color of the bubble

public:
    Bubble(double cx, double cy, double r, double vx_=BUBBLE_DEFAULT_VX, double vy_=BUBBLE_DEFAULT_VY, Color color_=COLOR(0, 0, 255))
    {
        // Bubble constructor
        color = color_;
        circle = Circle(cx, cy, r);
        circle.setColor(color);
        circle.setFill(true);
        vx = vx_;
        vy = vy_;
    }

    void nextStep(double t, unsigned int level)
    {
        float acceleration;
        // move the bubble
        //acceleration in y direction to make it parabolic, increasing the acceleration a bit in vertical direction when moving vertically up to ensure the bubble reaches the top
        if (vy>0){
            acceleration=200+ 50*level;
        }
        else{
            acceleration=180+ 50*level;
        }
        double new_x = circle.getX() + vx*t;
        double new_y = circle.getY() + vy*t + 0.5*acceleration*t*t;
		
        //updating the new velocity (based on acceleration)
        vy=vy+acceleration*t;

        //reflecting in both directions
        if ((vx < 0 && new_x <= (0 + circle.getRadius())) // bounce along X direction at left border
            ||
            (vx > 0 && new_x >= (WINDOW_X - circle.getRadius()))) // bounce along X direction at right border
        {
            
            vx = -vx;
            new_x = circle.getX() + vx*t;
            
        }
		if ((vy < 0 && new_y <= (0 + circle.getRadius())) // bounce along Y direction at top border
            ||
            (vy > 0 && new_y >= (WINDOW_Y-50- circle.getRadius()))) // bounce along Y direction at bottom border
        {
            
            vy = -vy;
            new_y = circle.getY() + vy*t;
            
        }
		

        circle.moveTo(new_x, new_y);
    }

    double get_radius()
    {
        // return the radius of the bubble
        return circle.getRadius();
    }

    double get_center_x()
    {
        // return the x coordinate of the center of the bubble
        return circle.getX();
    }

    double get_center_y()
    {
        // return the y coordinate of the center of the bubble
        return circle.getY();
    }

    double get_vx()
    {
        // return the x velocity of the bubble
        return vx;
    }
    double get_vy()
    {
        // return the y velocity of the bubble
        return vy;
    }

    Color get_color()
    {
        // return the color of the bubble
        return color;
    }
	//gives all the values of the bubble
	void bubble_information(float &bubble_x, float &bubble_y, float &bubble_radius, float &bubble_vx, float &bubble_vy, Color &col)
	{
		bubble_x=get_center_x(),
		bubble_y=get_center_y(),
		bubble_radius=get_radius();
        bubble_vx=get_vx();
        bubble_vy=get_vy();
        col=get_color();
	}
};