#include "pch.h"
#include "Planetary_Motion.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <cmath>

std::vector<BODIES> planet_vec; //vector of bodies to store input file information
std::vector<double> pos_vec; //vector to store the x and y positions of each body
std::vector<BODIES> force_vec; //vector of bodies to hold force components and distances between bodies

using namespace std;

//////////////////////////////////////////////////////////////////////////
// This function gets the information of any number of planetary bodies //
// from a file, and stores the information in a vector for further use  //
//////////////////////////////////////////////////////////////////////////
void BODIES::get_planets() {

	string filename; //holds the filename input by the user
	ifstream planets; //input filestream to handle data flow
	string line; //temporary string containing the current line's information
	int found; //temporary integer to determine where in the file data ends and comments begin
	int count = 0; //an integer to allow looping through the different data types contained in the file

	//Loading of file, while returning error if file not found
	while (1)
	{
		cout << "Name of file to load: ";
		cin >> filename; //ask for user input of file name
		planets.open(filename); //open file by name input by user
		
		//return confirmation message if open, error message if not
		if (planets.is_open()) { 
			cout << "File loaded!" << endl;
			break;
		}//end if
		else
		{
			cout << "File not found, try again" << endl;
		}//end else
	}//end while(1) loop

	getline(planets, line); //get the first line of the file

	found = line.find("#"); //find the start of the comment on that line
	line.erase(line.begin() + found, line.end()); //erase the comment

	num_bodies = stoi(line); //store this line as an integer of the number of bodies
	
	while (count < num_bodies) { //loop through each body

		BODIES new_planet; //create new BODIES object to temporarily store each body's info

		//get next line, find and delete comment, store as the planet's mass (type double)
		getline(planets, line);
		found = line.find("#");
		line.erase(line.begin() + found, line.end());
		new_planet.mass = stod(line);

		//get next line, find and delete comment, store as the planet's initial x coordinate (type double)
		getline(planets, line);
		found = line.find("#");
		line.erase(line.begin() + found, line.end());
		new_planet.x = stod(line);

		//get next line, find and delete comment, store as the planet's initial y coordinate (type double)
		getline(planets, line);
		found = line.find("#");
		line.erase(line.begin() + found, line.end());
		new_planet.y = stod(line);

		//get next line, find and delete comment, store as the planet's initial x velocity (type double)
		getline(planets, line);
		found = line.find("#");
		line.erase(line.begin() + found, line.end());
		new_planet.vx = stod(line);

		//get next line, find and delete comment, store as the planet's initial y velocity (type double)
		getline(planets, line);
		found = line.find("#");
		line.erase(line.begin() + found, line.end());
		new_planet.vy = stod(line);

		planet_vec.push_back(new_planet); //store the planet's information in the planet vector
		
		//store the intial x and y position in the position vector
		pos_vec.push_back(new_planet.x); 
		pos_vec.push_back(new_planet.y);
		
		count++; //increase the count, move on to the next body

	} //end while
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function uses the information acquired from the file to update the force, velocity, and position //
// of each body in the system for the time interval defined above. The positions are stored in a vector  //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void BODIES::trajectory() {

	cout << "Enter the desired time step for calculations (in seconds): "; //let the user determine the time step
	cin >> time_step;

	for (unsigned int i = 0; i < num_bodies; i++) { //loop through each body to find forces
		
		BODIES force; //create a new BODIES object to create intial forces

		//set initial x and y forces to 0
		force.fx = 0;
		force.fy = 0;
		
		for (unsigned int j = 0; j < num_bodies; j++) { //loop through each body to get forces from each on the current body

			if (j == i) { //current body cannot have a force on itself, so skip if j == i
				continue;
			}
			else {

				force.dx = (planet_vec[j].x - planet_vec[i].x); //store the object's x distance from another body
				force.dy = (planet_vec[j].y - planet_vec[i].y); //store the object's y distance from another body
				force.f = (G * planet_vec[i].mass * planet_vec[j].mass) / (pow(force.dy, 2) + pow(force.dx, 2)); //calculate the force from the other body
				force.theta = atan2f(force.dy, force.dx); //calculate the angle of that force from the object's distance values
				force.fx += force.f * cos(force.theta); //calculate the x value of the force and add it to the total
				force.fy += force.f * sin(force.theta); //calculate the y value of the force and add it to the total
			
			} //end if else
		} //end for loop

		force_vec.push_back(force); //store the force object in the force vector

	} //end for loop
		
	current_time += time_step; //increase the time step

	while (current_time <= TOTAL_TIME) { //loop while the current time is less than or equal to the total desired interval

		for (unsigned int i = 0; i < num_bodies; i++) { //loop through each body to calculate new velocities and positions

			//calculate the current velocity values using the force from the previous time step and the mass of the body
			planet_vec[i].vx += (force_vec[i].fx / planet_vec[i].mass) * time_step;
			planet_vec[i].vy += (force_vec[i].fy / planet_vec[i].mass) * time_step;

			//calculate the current position values using the current velocity values
			planet_vec[i].x += (planet_vec[i].vx * time_step);
			planet_vec[i].y += (planet_vec[i].vy * time_step);

			//store the current position values in the position vector
			pos_vec.push_back(planet_vec[i].x);
			pos_vec.push_back(planet_vec[i].y);

		} //end for loop

		for (unsigned int i = 0; i < num_bodies; i++) { //loop through each body to redetermine components of force

			//reset the force component values
			force_vec[i].fx = 0;
			force_vec[i].fy = 0;

			for (unsigned int j = 0; j < num_bodies; j++) { //loop through each other body to redetermine the forces on the current body

				if (j == i) { //current body cannot have a force on itself, so skip if j == i
					continue;
				}
				else {

					force_vec[i].dx = (planet_vec[j].x - planet_vec[i].x); //recalculate the object's x distance from another body
					force_vec[i].dy = (planet_vec[j].y - planet_vec[i].y); //recalculate the object's y distance from another body
					force_vec[i].f = (G * planet_vec[i].mass * planet_vec[j].mass) / (pow(force_vec[i].dy, 2) + pow(force_vec[i].dx, 2)); //recalculate the force from the other body
					force_vec[i].theta = atan2f(force_vec[i].dy, force_vec[i].dx); //recalculate the angle of the force
					force_vec[i].fx += force_vec[i].f * cos(force_vec[i].theta); //calculate the x value of the force and add it to the total
					force_vec[i].fy += force_vec[i].f * sin(force_vec[i].theta); //calculate the y value of the force and add it to the total
				
				} //end if else
			} //end for loop
		} //end for loop

		current_time += time_step; //increase the current time by one time step
	
	} //end while
}

////////////////////////////////////////////////////////////////////////////////////////////
// This function outputs the time step used, the number of bodies, and then the x and y   //
// components of each body over the time interval into a file. The file will then be used //
// to plot the bodies' trajectories in a planetary motion viewer                          //
////////////////////////////////////////////////////////////////////////////////////////////
void BODIES::output() {
	
	string output_name; //holds the filename input by the user

	cout << "What would you like to call output text file? (Exclude .txt): ";
	cin >> output_name;
	output_name.append(".txt"); //append ".txt" to the output file to ensure it can be read by the planetary motion viewer

	ofstream outfile(output_name); //open the file

	outfile << time_step << endl; //output the time step to the file
	outfile << num_bodies << endl; //output the number of bodies to the file

	while (pos_vec.size() > 0) { //loop while there is still something in the position vector

		for (unsigned int i = 0; i < (num_bodies * 2); i++) { //loop through the x and y values for each body in the current time step
			outfile.precision(4); //set the output to 4 decimal places
			outfile << scientific << pos_vec[i] << "      "; //output the current value in the position vector in scientific notation
		}

		pos_vec.erase(pos_vec.begin(), pos_vec.begin() + (num_bodies * 2)); //erase the values that have been output to the file from the vector
		outfile << endl; //end the line in the file
	
	}//end while

	outfile.close(); //close the file
}

