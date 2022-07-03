//Code for CS293 Project
//Implemented mandelbrot zoom
//Coded by Jash Kabra(200050054)

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>

//defining new namespace for to_string function as std::to_string deprecated
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

class mandelbrotzoom
{
private:
    unsigned int width;
    unsigned int height;

    //original values of x and y
    double ogxmin;
    double ogxmax;
    double ogymin;
    double ogymax;

	//stores color shade
	int color_shade;

    //private functions
    sf::Color convertHSV_RGB(float H, float S, float V);
	sf::Color convertHSV_RGB_2(float H, float S, float V);
    double mandel_iter(double cx, double cy, int max_iter);
    sf::Texture mandelbrot(double xmin, double xmax, double ymin, double ymax, int iterations);


public:
    mandelbrotzoom(int width, int height, double xmin, double xmax);
    void startzoom();
};

//constructor to set initial values
mandelbrotzoom::mandelbrotzoom(int width, int height, double xmin, double xmax)
{
    this->width=width;
    this->height=height;
    ogxmin=xmin;
    ogxmax=xmax;

	//symmetric about y axis
    double y=(ogxmax - ogxmin) * height / width;
    ogymin=-y/2.0;
    ogymax=y/2.0;

	//initial value by default is 0
	color_shade=0;
}


//f(z)=z^2+c
//z=x+iy
//c is part of mandelbrot set if f(z) does not diverge when iterated from z=0

//function to find number of iterations taken for f(z) to diverge defined by real(z)+imag(z)>4
double mandelbrotzoom::mandel_iter(double cx, double cy, int max_iter)
{
    //x and y are real and imaginary values of a complex number
    double x = 0.0;
	double y = 0.0;
	double xx = 0.0;
	double yy = 0.0;
	double xy = 0.0;

    double iter = max_iter;
	while (iter--) {

        //break on diverging
        if(xx+yy>4)
            break;
        
		xy = x * y;
		xx = x * x;
		yy = y * y;

		//complex number multiplication
		x = xx - yy + cx;
		y = xy*2.0 + cy;
	}

    iter=max_iter-iter;
    //no of iterations taken to diverge
	return iter;
}


//function to create array of RGBA pixel values
sf::Texture mandelbrotzoom::mandelbrot(double xmin, double xmax, double ymin, double ymax, int iterations)
{
    sf::Texture texture;
	texture.create(width, height);

    //pixels array of size *4 to store RGBA value
	sf::Uint8* pixels = new sf::Uint8[width * height * 4];

	for (int ix = 0; ix < width; ix++)
	{
		for (int iy = 0; iy < height; iy++)
		{
			double x = xmin + (xmax - xmin) * ix / (width-1.0);
			double y = ymin + (ymax - ymin) * iy / (height-1.0);

			double iter = mandel_iter(x, y, iterations);

			int current_pos = 4 * (width * iy + ix);

			int hue = (int)(360 * iter / iterations);
			int sat = 100;
			int val; 
            if(iter > iterations)
                val=0;
            else val=100;

			sf::Color hsvtorgb;
			if(color_shade==0){
				hsvtorgb=convertHSV_RGB(hue, sat, val);
			}
			else{
				hsvtorgb=convertHSV_RGB_2(hue, sat, val);
			}

			pixels[current_pos] = (int)hsvtorgb.r;
			pixels[current_pos + 1] = (int)hsvtorgb.g;
			pixels[current_pos + 2] = (int)hsvtorgb.b;

            //alpha(A) set to max value
			pixels[current_pos + 3] = 255;
		}
	}

	texture.update(pixels, width, height, 0, 0);

	//free up memory by deleting array
	delete[] pixels;

	return texture;
}


//algorithm 1 to convert hue,sat and value to rgb pixels
sf::Color mandelbrotzoom::convertHSV_RGB(float H, float S, float V)
{
	//if out of range colour is black
    if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
		return sf::Color::Black;
	}

	H+=120;
	if(H>360)H-=360;
	float s = S / 100;
	float v = V / 100;
	float C = s * v;
	float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;

	if (H >= 0 && H < 60) {
		r = C, g = X, b = 0;
	}
	else if (H >= 60 && H < 120) {
		r = X, g = C, b = 0;
	}
	else if (H >= 120 && H < 180) {
		r = 0, g = C, b = X;
	}
	else if (H >= 180 && H < 240) {
		r = 0, g = X, b = C;
	}
	else if (H >= 240 && H < 300) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}

	int R = (r + m) * 255;
	int G = (g + m) * 255;
	int B = (b + m) * 255;

	return sf::Color(R, G, B);
}

//algorithm 2 to convert hue,sat and value to rgb pixels
sf::Color mandelbrotzoom::convertHSV_RGB_2(float H, float S, float V)
{
	//if out of range colour is black
    if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
		return sf::Color::Black;
	}

	H+=120;
	if(H>360)H-=360;
	float s = S / 100;
	float v = V / 100;
	float C = s * v;
	float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;

	if (H >= 180 && H < 240) {
		r = C, g = X, b = 0;
	}
	else if (H >= 240 && H < 300) {
		r = X, g = C, b = 0;
	}
	else if (H >= 300 && H < 360) {
		r = 0, g = C, b = X;
	}
	else if (H > 0 && H < 60) {
		r = 0, g = X, b = C;
	}
	else if (H >= 60 && H < 120) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}

	int R = (r + m) * 255;
	int G = (g + m) * 255;
	int B = (b + m) * 255;

	return sf::Color(R, G, B);
}


//function which displays mandelbrot set 
void mandelbrotzoom::startzoom()
{

	std::cout<<"Welcome to Mandelbrot Zoom\n";
	std::cout<<"\n";

	std::cout<<"Here are the basic functionalities:\n";
	std::cout<<"Left click: Zoom in to within bounding box\n";
	std::cout<<"Right click: Zoom out\n";
	std::cout<<"Press +/= : Zoom in at centre of screen\n";
	std::cout<<"Left arrow: Move left\n";
	std::cout<<"Right arrow: Move right\n";
	std::cout<<"Up arrow: Move up\n";
	std::cout<<"Down arrow: Move down\n";
	std::cout<<"Press 1: Reduce precision\n";
	std::cout<<"Press 2: Increase precision\n";
	std::cout<<"Press 0: Change colour\n";
	std::cout<<"Press S: Save screenshot\n";
	std::cout<<"Press O: Return to original state\n";

	std::cout<<"\n";
	std::cout<<"Enjoy the game!!\n";
	std::cout<<"\n";

	sf::RenderWindow window(sf::VideoMode(width, height), "mandelbrot");

	window.setFramerateLimit(60);

	sf::Texture mandelTexture;
	sf::Sprite mandelSprite;

	//create zoom border of size window/8
	//will zoom to within this rectangle on left click
	sf::RectangleShape zoomBorder(sf::Vector2f(width / 8, height / 8));
	zoomBorder.setFillColor(sf::Color(0, 0, 0, 0));
	zoomBorder.setOutlineColor(sf::Color::Blue);
	zoomBorder.setOutlineThickness(1.0f);
	zoomBorder.setOrigin(sf::Vector2f(zoomBorder.getSize().x / 2, zoomBorder.getSize().y / 2));

	//initially use original values
	double xmin = ogxmin;
	double xmax = ogxmax;
	double ymin = ogymin;
	double ymax = ogymax;

	//initial recLevel 1
	int recLevel = 1;
	//precision denotes no of iterations
	int precision = 64;

	mandelTexture = mandelbrot(xmin, xmax, ymin, ymax, precision);

	//font not preloaded and have to load from file
    sf::Font arial;
    arial.loadFromFile("arial.ttf");

	//text to display current zoom and precision
	sf::Text zoomText, precText;
    zoomText.setFont(arial);
	precText.setFont(arial);

	zoomText.setFillColor(sf::Color::Blue);
	precText.setFillColor(sf::Color::Blue);

	zoomText.setCharacterSize(24);
	precText.setCharacterSize(24);

	//while loop to handle different events
	while (window.isOpen())
	{
		sf::Event evnt;
		//traverse through events in a queue
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyReleased:

				//sf::Keyboard::Key::O
				//return to original set
				if (evnt.key.code == 14) 
				{
					recLevel = 1;
					precision = 64;

					xmin = ogxmin;
					xmax = ogxmax;
					ymin = ogymin;
					ymax = ogymax;

					color_shade=0;

					std::cout<<"Reverting to original state\n";
				}

				// sf::Keyboard::Key::Num1
				//reduce precision by 1 level
				if( evnt.key.code== 27) 
				{
					precision-=64;
					//min value of precision is 64
					if(precision<64)
						precision=64;

					std::cout<<"Decrease precision\n";
				}

				// sf::Keyboard::Key::Num2
				//Increase precision by 1 level
				if( evnt.key.code== 28)
				{
					precision+=64;
					//max value of precision is 320
					//if value of precision is too high, rendering is slow
					if(precision>320)
						precision=320;
					
					std::cout<<"Increase precision\n";
				}

				//sf::Keyboard::Key::Up
				//Shift plot upwards
				if( evnt.key.code== 73) 
				{
					xmin=xmin;
					xmax=xmax;

					double yrange=ymax-ymin;
					yrange/=4.0;
					ymax-=yrange;
					ymin-=yrange;

					std::cout<<"Shifting plot upwards\n";
				}

				//sf::Keyboard::Key::Down
				//shift plot down
				if( evnt.key.code== 74)
				{
					xmin=xmin;
					xmax=xmax;

					double yrange=ymax-ymin;
					yrange/=4.0;
					ymax+=yrange;
					ymin+=yrange;

					std::cout<<"Shifting plot downwards\n";
				}

				//sf::Keyboard::Key::Right
				//shift plot to right
				if( evnt.key.code== 72)
				{
					ymin=ymin;
					ymax=ymax;

					double xrange=xmax-xmin;
					xrange/=4.0;
					xmax+=xrange;
					xmin+=xrange;

					std::cout<<"Shifting plot rightwards\n";
				}

				//sf::Keyboard::Key::Left
				//shift plot to left
				if( evnt.key.code== 71)
				{
					ymin=ymin;
					ymax=ymax;

					double xrange=xmax-xmin;
					xrange/=4.0;
					xmax-=xrange;
					xmin-=xrange;

					std::cout<<"Shifting plot leftwards\n";
				}

				//sf::Keyboard::Key::Equal (+ sign)
				//zoom in at the centre of the screen by 8 times
				if( evnt.key.code==55) 
                {
					//reclevel increases on zooming
					recLevel++;
					precision+=64;
					//max value of precision is 320
					if(precision>320)
						precision=320;
					
					double txmin = xmin;
					double txmax = xmax;
					double tymin = ymin;
					double tymax = ymax;

					//avg=xmin+xmax/2
					//range=current_range/8=(xmin-xmax)/16;
					//min=avg-range,max=avg+range
					xmin=0.5625*txmin+0.4375*txmax;
					xmax=0.5625*txmax+0.4375*txmin;
					ymin=0.5625*tymin+0.4375*tymax;
					ymax=0.5625*tymax+0.4375*tymin;

					std::cout<<"Zooming into centre of screen\n";	
				}

				//sf::Keyboard::Key::Num0
				//change color shade
				if( evnt.key.code== 26)
				{
					//0 to 1 or 1 to 0
					color_shade=color_shade+1;
					color_shade%=2;

					std::cout<<"Changing colour\n";
				}

				//sf::Keyboard::Key::S
				//Save screenshot of current screen
				if( evnt.key.code==18) 
				{
					sf::Image screenshot=mandelTexture.copyToImage();

					//remove file if already exists
					remove("image.jpg");

					screenshot.saveToFile("image.jpg");
					std::cout<<"Saved screenshot\n";
				}

				//plot mandelbrot after changing values of x and y
				mandelTexture = mandelbrot(xmin, xmax, ymin, ymax, precision);
				break;
			}
		}

		//on right click, zoom out
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			//cant zoom out if reclevel is 1
			if(recLevel<=1)
				continue;
			
			//reclevel decrease on zooming out
			recLevel--;
			precision-=64;
			//min precision is 64
			if(precision<64)
				precision=64;

			double txmin = xmin;
			double txmax = xmax;
			double tymin = ymin;
			double tymax = ymax;

			//avg=xmin+xmax/2
			//range=current_range*8=(xmin-xmax)*4;
			//min=avg-range,max=avg+range
			xmin=4.5*txmin-3.5*txmax;
			xmax=4.5*txmax-3.5*txmin;
			ymin=4.5*tymin-3.5*tymax;
			ymax=4.5*tymax-3.5*tymin;

			mandelTexture = mandelbrot(xmin, xmax, ymin, ymax, precision);

			std::cout<<"Zoom out of centre of screen\n";
		}

		//on left click, zoom to within the bounding box
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			//reclevel increase on zooming in
			recLevel++;
			precision+=64;
			//max value of precision is 320
			if(precision>320)
				precision=320;
			
			double x = zoomBorder.getPosition().x - zoomBorder.getSize().x / 2;
			double y = zoomBorder.getPosition().y - zoomBorder.getSize().y / 2;

			double x2 = x + zoomBorder.getSize().x;
			double y2 = y + zoomBorder.getSize().y;

			//x and x2 are integers value from 0 to width and have to change them to fractal x
			double xmin2=x/width;
			xmin2*=(xmax-xmin);
			xmin2+=xmin;

			double xmax2=x2/width;
			xmax2*=(xmax-xmin);
			xmax2+=xmin;			

			double ymin2=y/height;
			ymin2*=(ymax-ymin);
			ymin2+=ymin;

			double ymax2=y2/height;
			ymax2*=(ymax-ymin);
			ymax2+=ymin;			

			xmin = xmin2;
			xmax = xmax2;
			ymin = ymin2;
			ymax = ymax2;

			mandelTexture = mandelbrot(xmin, xmax, ymin, ymax, precision);

			std::cout<<"Zoom into bounding box\n";
		}

		//zoom=8^reclevel
		zoomText.setString("Zoom: " + patch::to_string(pow(8, recLevel-1)));
		//max iterations=precision
		precText.setString("Max. Iterations: " + patch::to_string(precision));
		precText.setPosition(sf::Vector2f(0, 32));

		if(color_shade==0)
		{
			zoomText.setFillColor(sf::Color::Blue);
			precText.setFillColor(sf::Color::Blue);
			zoomBorder.setOutlineColor(sf::Color::Blue);
		}
		else
		{
			zoomText.setFillColor(sf::Color::White);
			precText.setFillColor(sf::Color::White);
			zoomBorder.setOutlineColor(sf::Color::White);
		}

		zoomBorder.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

		mandelSprite.setTexture(mandelTexture);

		window.clear(sf::Color::White);

		window.draw(mandelSprite);
		window.draw(zoomText);
		window.draw(precText);
		window.draw(zoomBorder);

		window.display();
	}
}


int main(){

	int width=1280;
	int height=720;
	double xmin=-2.5;
	double xmax=1.0;

    mandelbrotzoom * mandel=new mandelbrotzoom(width,height,xmin,xmax);
    mandel->startzoom();
}