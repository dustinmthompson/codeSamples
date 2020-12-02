#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <list>
#include <queue>
#include "vehicles.h"


using namespace std;

void menu(vehicle*, vehicle*, ofstream&);

int main(){
	ofstream myFile;
	myFile.open("SimulationLog.txt");

	vehicle* vehicleList = NULL;
	vehicle* curr;

	cout << "AFV Repair Yard" << endl;
	cout << "------------------\n" << endl;
	menu(vehicleList, curr, myFile);
}

void menu(vehicle* vehicleList, vehicle* curr, ofstream& myFile){
	int selection = 0;
	default_random_engine* rand;
	rand = new default_random_engine(time(NULL));

	priority_queue<vehicle*> repairQueue;	// Repair Queue
	list<int> junkYard;					  	// Junk Yard
	vector<int> supplyVector;				// Supply Vector
	vector<vehicle*> repairBay(3);			// 3 Repair Bays
	int possibleParts[45] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45}; // Array of possible parts

	uniform_int_distribution<int> percent(1,100); // 1-100 for random percent chance
	uniform_int_distribution<int> numberParts(2,4); // 2-4 for possible generated parts for supply vector

	normal_distribution<double> randTank(3.0,1.0); // Number of tank parts
	normal_distribution<double> randOther(5.0,2.0); // Number of wheeled vehicle and half-track parts

	uniform_int_distribution<int> tankParts(1,11); // Random gen 1-11 (tankOneParts)
	int tankTwoParts[11] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22}; // tankTwo parts
	int tankThreeParts[11] = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33}; // tankThree parts

	uniform_int_distribution<int> wheelParts(1,10); // Random gen 1-10 (wheeled vehicle parts)
	int wheelOneParts[10] = {31, 32, 33, 34, 35, 36, 37, 38, 39, 40}; // Wheeled Vehicle One Parts
	int wheelTwoParts[10] = {36, 37, 38, 39, 40, 41, 42, 43, 44, 45}; // Wheeled Vehicle Two Parts

	uniform_int_distribution<int> halfTrackGen(1,15); // Random gen 1-15 (half-track parts)
	int halfTrackParts[15] = {1, 2, 3, 4, 5, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};

	cout << "Select an option: " << endl;
	cout << "1. Generate and add a damaged Tank type 1" << endl;
	cout << "2. Generate and add a damaged Tank type 2" << endl;
	cout << "3. Generate and add a damaged Tank type 3" << endl;
	cout << "4. Generate and add a damaged wheeled vehicle type 1" << endl;
	cout << "5. Generate and add a damaged wheeled vehicle type 2" << endl;
	cout << "6. Generate and add a half-track" << endl;
	cout << "7. Display all vehicles" << endl;
	cout << "8. Repair Vehicles" << endl;
	cout << "9. Exit\n" << endl;

	cin >> selection;

//---------------------------------------------------------

	if (selection == 1) // Add damaged tank 1
	{
		if (vehicleList == NULL)
		{
			vehicleList = new tankOne();
			curr = vehicleList;
			myFile << "Generated Damaged Tank One with broken parts ";

			for (int i = 0; i < static_cast<int>(randTank(*rand)); ++i) // Loop for number of parts from generated part distribution
			{
				int part = tankParts(*rand);
				curr -> addPart(part); // Add allowed parts for vehicle based on above loops
				myFile << part << " ";
			}

			myFile << endl;
			cout << "Added Damaged Tank One.\n\n";
			menu(vehicleList, curr, myFile);
		}
		else
        {
            curr = new tankOne();
            myFile << "Generated Damaged Tank One with broken parts ";
			for (int i = 0; i < static_cast<int>(randTank(*rand)); ++i)
			{
				int part = tankParts(*rand);
				curr -> addPart(part); // Add allowed parts for vehicle based on above loops
				myFile << part << " ";
			}

            curr -> setNext(vehicleList);
            vehicleList = curr;

            myFile << endl;
            cout << "Added Damaged Tank One.\n\n";
			menu(vehicleList, curr, myFile);
        }
	}

//---------------------------------------------------------

	else if (selection == 2) // Add damaged tank 2
	{
		if (vehicleList == NULL)
		{
			vehicleList = new tankTwo();
			curr = vehicleList;
			myFile << "Generated Damaged Tank Two with broken parts ";

			for (int i = 0; i < static_cast<int>(randTank(*rand)); ++i)
			{
				int part = tankTwoParts[tankParts(*rand)];
				curr -> addPart(part);
				myFile << part << " ";
			}

			myFile << endl;
			cout << "Added Damaged Tank Two.\n\n";
			menu(vehicleList, curr, myFile);
		}
		else
        {
            curr = new tankTwo();
            myFile << "Generated Damaged Tank Two with broken parts ";

			for (int i = 0; i < static_cast<int>(randTank(*rand)); ++i)
			{
				int part = tankTwoParts[tankParts(*rand)];
				curr -> addPart(part);
				myFile << part << " ";
			}

            curr -> setNext(vehicleList);
            vehicleList = curr;

            myFile << endl;
            cout << "Added Damaged Tank Two.\n\n";
			menu(vehicleList, curr, myFile);
        }
	}

//---------------------------------------------------------

	else if (selection == 3) // Add damaged tank 3
	{
		if (vehicleList == NULL)
		{
			vehicleList = new tankThree();
			curr = vehicleList;
			myFile << "Generated Damaged Tank Three with broken parts ";

			for (int i = 0; i < static_cast<int>(randTank(*rand)); ++i)
			{
				int part = tankThreeParts[tankParts(*rand)];
				curr -> addPart(part);
				myFile << part << " ";
			}

			myFile << endl;
			cout << "Added Damaged Tank Three.\n\n";
			menu(vehicleList, curr, myFile);
		}
		else
        {
            curr = new tankThree();
            myFile << "Generated Damaged Tank Three with broken parts ";

			for (int i = 0; i < static_cast<int>(randTank(*rand)); ++i)
			{
				int part = tankThreeParts[tankParts(*rand)];
				curr -> addPart(part);
				myFile << part << " ";
			}

            curr -> setNext(vehicleList);
            vehicleList = curr;

            myFile << endl;
            cout << "Added Damaged Tank Three.\n\n";
			menu(vehicleList, curr, myFile);
        }
	}

//---------------------------------------------------------

	else if (selection == 4) // Add wheeled vehicle 1
	{
		if (vehicleList == NULL)
		{
			vehicleList = new wheelOne();
			curr = vehicleList;
			myFile << "Generated Wheeled Vehicle One with broken parts ";

			for (int i = 0; i < static_cast<int>(randOther(*rand)); ++i)
			{
				int part = wheelOneParts[wheelParts(*rand)];
				curr -> addPart(part);
				myFile << part << " ";
			}

			myFile << endl;
			cout << "Added Wheeled Vehicle One.\n\n";
			menu(vehicleList, curr, myFile);
		}
		else
        {
            curr = new wheelOne();
            myFile << "Generated Wheeled Vehicle One with broken parts ";

			for (int i = 0; i < static_cast<int>(randOther(*rand)); ++i)
			{
				int part = wheelOneParts[wheelParts(*rand)];
				curr -> addPart(part);
				myFile << part << " ";
			}

            curr -> setNext(vehicleList);
            vehicleList = curr;

            myFile << endl;
            cout << "Added Wheeled Vehicle One.\n\n";
			menu(vehicleList, curr, myFile);
        }
	}

//---------------------------------------------------------

	else if (selection == 5) // Add wheeled vehicle 2
	{
		if (vehicleList == NULL)
		{
			vehicleList = new wheelTwo();
			curr = vehicleList;
			myFile << "Generated Wheeled Vehicle Two with broken parts ";

			for (int i = 0; i < static_cast<int>(randOther(*rand)); ++i)
			{
				int part = wheelTwoParts[wheelParts(*rand)];
				curr -> addPart(part);
				myFile << part << " ";
			}

			myFile << endl;
			cout << "Added Wheeled Vehicle Two.\n\n";
			menu(vehicleList, curr, myFile);
		}
		else
        {
            curr = new wheelTwo();
            myFile << "Generated Wheeled Vehicle Two with broken parts ";

			for (int i = 0; i < static_cast<int>(randOther(*rand)); ++i)
			{
				int part = wheelTwoParts[wheelParts(*rand)];
				curr -> addPart(part);
				myFile << part << " ";
			}

            curr -> setNext(vehicleList);
            vehicleList = curr;

            myFile << endl;
            cout << "Added Wheeled Vehicle Two.\n\n";
			menu(vehicleList, curr, myFile);
        }
	}

//---------------------------------------------------------

	else if (selection == 6) // Add half-track
	{
		if (vehicleList == NULL)
		{
			vehicleList = new halfTrack();
			curr = vehicleList;
			myFile << "Generated Half-Track with broken parts ";

			for (int i = 0; i < static_cast<int>(randOther(*rand)); ++i)
			{
				int part = halfTrackParts[halfTrackGen(*rand)];
				curr -> addPart(part);
				myFile << part << " ";
			}

			myFile << endl;
			cout << "Added Half-Track.\n\n";
			menu(vehicleList, curr, myFile);
		}
		else
        {
            curr = new halfTrack();
            myFile << "Generated Half-Track with broken parts ";

			for (int i = 0; i < static_cast<int>(randOther(*rand)); ++i)
			{
				int part = halfTrackParts[halfTrackGen(*rand)];
				curr -> addPart(part);
				myFile << part << " ";
			}

            curr -> setNext(vehicleList);
            vehicleList = curr;

            myFile << endl;
            cout << "Added Half-Track.\n\n";
			menu(vehicleList, curr, myFile);
        }
	}

//---------------------------------------------------------

	else if (selection == 7) // Display vehicles
	{
		cout << "\n";
		curr = vehicleList;
		while (curr != NULL){
			curr -> display();
			curr = curr -> getNext();
		}
		cout << "\n";
		system("pause");
		menu(vehicleList, curr, myFile);
	}

//---------------------------------------------------------

	else if (selection == 8) // Repair Vehicles
	{
		myFile << "\n" << "Generated Supply Vector with parts ";
		for (int i = 0; i < 45; ++i)						// Generate Supply Vector with 90% of 2-4 parts for each part
		{
			if (percent(*rand) <= 90)
			{
				int parts = numberParts(*rand);
				supplyVector.push_back(parts);
				myFile << possibleParts[i] << "(" << parts <<") ";
			}
			else{
				supplyVector.push_back(0);
				myFile << possibleParts[i] << "(0) ";
			}
		}
		myFile << endl << endl;

		curr = vehicleList;
		while(curr != NULL){							// If vehicle has 5 or more broken parts, send to scrap yard. Otherwise, priority queue
			int numberOfParts = curr -> numberOfParts();
			if (numberOfParts >= 5)
			{
				myFile << "Junk Yard filled with removed parts ";
				for (int i = 0; i < numberOfParts; ++i)
				{
					int part = curr -> getPart(i);
					junkYard.push_back(part);
					myFile << part << " ";
				}
				myFile << endl;
			}
			else{
				repairQueue.push(curr);
				switch (curr->getType()){
					case 1: myFile << "Added Damaged Tank One to Repair Queue"<<endl;
						break;
					case 2: myFile << "Added Damaged Tank Two to Repair Queue"<<endl;
						break;
					case 3: myFile << "Added Damaged Tank Three to Repair Queue"<<endl;
						break;
					case 4: myFile << "Added Damaged Wheeled Vehicle One to Repair Queue"<<endl;
						break;
					case 5: myFile << "Added Damaged Wheeled Vehicle Two to Repair Queue"<<endl;
						break;
					case 6: myFile << "Added Damaged Half-Track to Repair Queue"<<endl;
						break;
				}
			}
			curr = curr -> getNext();
		}

		myFile << endl;
		while(!repairQueue.empty()){				// While vehicles are in the repair queue, move them to an available repair bay and attempt to repair them
			//for (int i = 0; i < 3; ++i)
			//{
				//cout<<"repairbay loop"<<endl;
				if (repairBay.back()==NULL)
				{
					repairBay.push_back(repairQueue.top());
					repairQueue.pop();

					switch (repairBay.back()->getType()){
					case 1: myFile << "Added Damaged Tank One to Repair Bay 1" <<endl;
						break;
					case 2: myFile << "Added Damaged Tank Two to Repair Bay 1" <<endl;
						break;
					case 3: myFile << "Added Damaged Tank Three to Repair Bay 1" <<endl;
						break;
					case 4: myFile << "Added Damaged Wheeled Vehicle One to Repair Bay 1" <<endl;
						break;
					case 5: myFile << "Added Damaged Wheeled Vehicle Two to Repair Bay 1" <<endl;
						break;
					case 6: myFile << "Added Damaged Half-Track to Repair Bay 1" <<endl;
						break;
					}

					int numberOfParts = repairBay.back()->numberOfParts(); // Number of vehicle parts
					for (int x = 0; x < numberOfParts; ++x)				// For loop for each part
					{
						if (!junkYard.empty())
						{
							bool removed = false;
							for (list<int>::iterator it = junkYard.begin(); it != junkYard.end(); it++)		// If junk yard is not empty, search for part in junk yard
							{
								if (*it == repairBay.back()->getPart(0))
								{
									myFile << "Part " << *it << " fixed from junk yard."<<endl;
									junkYard.erase(it);
									myFile<<"erased"<<endl;
									repairBay.back()->removePart(0);
									removed = true;
									myFile<<"true"<<endl;
									break;
								}
							}
							cout<<endl;
							if ((removed == false) && (supplyVector.at((repairBay.back()->getPart(0))-1) >= 1))		// If not in junk yard, search supply vector
							{
								myFile << "Part " << repairBay.back()->getPart(0) << " fixed from supply vector."<<endl;
								supplyVector.at(repairBay.back()->getPart(0)) = supplyVector.at(repairBay.back()->getPart(0)) - 1;
								repairBay.back()->removePart(0);
							}
						}
						else if(supplyVector.at((repairBay.back()->getPart(0))-1) >= 1)							// If junk yard is empty, search supply vector
						{
							myFile << "Part " << repairBay.back()->getPart(0) << " fixed from supply vector."<<endl;
							supplyVector.at(repairBay.back()->getPart(0)) = supplyVector.at(repairBay.back()->getPart(0)) - 1;
							repairBay.back()->removePart(0);
						}

					}

					if (repairBay.back()->numberOfParts() == 0)
					{
						if (percent(*rand) <= 97)
						{
							myFile << "Vehicle was successfully fixed."<<endl;
							repairBay.back()=NULL;
						}
						else{
							myFile << "Vehicle parts were fixed but vehicle failed test. Moved back into repair queue."<<endl;
							repairQueue.push(repairBay.back());
							repairBay.back()=NULL;
						}
					}
					else{
						myFile << "Vehicle could not be fixed due to lack of parts."<<endl;
						repairBay.back()=NULL;
					}
				}
			//}
		myFile<<endl;
		}

		cout<<endl<<endl<<"Simulation complete! Please check the output file \"SimulationLog.txt\" for a full breakdown of the simulation."<<endl<<endl;
	}

//---------------------------------------------------------

//---------------------------------------------------------

	else if (selection == 9) //
	{
		exit(0);
	}

//---------------------------------------------------------

	else // Not 1-8 selection
	{
		cout << "\n\nPlease select a valid option (1-8)\n";
		menu(vehicleList, curr, myFile);
	}
}
