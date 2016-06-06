/* 
Alexander Ho
comScore Coding Challenge
1) Importer and Datastore 

The program allows the user to enter multiple input files, up to a maximum amount of entries MAX_ENTRY_SIZE.
The input file is parsed by the '|' and takes 6 fields at a time, making up one entry.
The 6 fields are then populated into a new entry object and compares with the other entries to make sure it is unique by STB, TITLE and DATE.
It will store the entries in an array of pointers to Entry objects.
When all of the input files have been entered, it will create a datastore to OUTPUT_PATH.
The datastore keeps each field on it's own line, 6 lines in the field are equal to one entry.
*/


#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <math.h>
using namespace std;

#include "importer_classes.h"

//String to set the output path
const char *OUTPUT_PATH = "output.txt";

const int MAX_CHAR_SIZE = 64; // Max size for STB, TITLE, PROVIDER
const int MAX_ENTRY_SIZE = 50;
const int NUM_CENTS_IN_DOLLAR = 100;
const int NUM_MIN_IN_HOUR = 60;
const int NUM_FIELDS_PER_ENTRY = 6;

const int STB_INDEX = 0;
const int TITLE_INDEX = 1;
const int PROVIDER_INDEX = 2;
const int DATE_INDEX = 3;
const int REV_INDEX = 4;
const int VIEW_TIME_INDEX = 5;



int main () 
{
	char * temp = new char[MAX_CHAR_SIZE+1];
	int match = 0;
	int num_entries = 0;
	int progExit = 0;
	int menuExit = 0;
	string input_array[NUM_FIELDS_PER_ENTRY+1];

	//Create and initialize an array of pointers to Entry objects
	Entry ** object_Array = new Entry *[MAX_ENTRY_SIZE+1];
	for (int i = 0; i < MAX_ENTRY_SIZE; i++) 
	{
		object_Array[i] = NULL;
	}

	// Welcome Screen
	while (progExit == 0) 
	{
		// Import the files
		cout << "Please enter the full-path name of the file you wish to input: " <<endl;
		cin >> temp;

		ifstream inputfile (temp);   
		if (inputfile.is_open()) 
		{
			//Parse the entries by '|'
			while ( getline (inputfile, input_array[0], '|') != NULL) 
			{ 
				//Get rid of any '\n' characters in the string
				//input_array[0].erase( remove(input_array[0].begin(), input_array[0].end(), '\n'), input_array[0].end());

				//Loop through and parse for 5 more '|' to get a total of 6 fields, to get 1 full entry
				for (int i = 1; i < NUM_FIELDS_PER_ENTRY-1; i++) 
				{
					getline (inputfile, input_array[i], '|'); 

					// Remove all of the newlines that may be in the middle of the string
					input_array[i].erase( remove(input_array[i].begin(), input_array[i].end(), '\n'), input_array[i].end());
				}	
				getline (inputfile, input_array[NUM_FIELDS_PER_ENTRY-1], '\n');

				// Create a new Entry object
				Entry * new_entry = new Entry();

				// Copy the STB
				strcpy(temp, input_array[STB_INDEX].c_str());
				new_entry->set_STB(temp);

				// Copy The TITLE
				strcpy(temp, input_array[TITLE_INDEX].c_str());
				new_entry->set_TITLE(temp);

				// Copy the PROVIDER
				strcpy(temp, input_array[PROVIDER_INDEX].c_str());
				new_entry->set_PROVIDER(temp);

				// Copy the Date
				strcpy(temp, input_array[DATE_INDEX].c_str());
				new_entry->set_DATE(temp);

				// Copy the REV
				strcpy(temp, input_array[REV_INDEX].c_str());
				int num_cents = atof(temp) * NUM_CENTS_IN_DOLLAR;  // Convert REV to cents
				new_entry->set_REV(num_cents);
         
				// Copy the VIEW_TIME  
				strcpy(temp, input_array[VIEW_TIME_INDEX].c_str());
				int num_minutes, min, sec; 
				sscanf(input_array[VIEW_TIME_INDEX].c_str(), "%d:%d", &min, &sec);
				num_minutes = min * 60 + sec;
				new_entry->set_VIEW_TIME(num_minutes);

				// Compare the entries before adding it to the array
				// Make sure it is unique by STB, TITLE and DATE
				if (num_entries >= 0) 
				{
					for(int i = 0; i < num_entries; i++) 
					{
						if( strcmp(object_Array[i]->get_STB(), new_entry->get_STB()) == 0)
						{
							if ( strcmp(object_Array[i]->get_TITLE(), new_entry->get_TITLE()) == 0)
							{
								if ( strcmp(object_Array[i]->get_DATE(), new_entry->get_DATE()) == 0) 
								{
									// All 3 cases match, the entry is not unique -- overwrite it
									object_Array[i] = new_entry;
									match = 1;
									break;
								}
							}
						}
					}
					// Went through all of the current entries and the three cases are unique -- Add a new entry
					if ((match == 0) && (num_entries < MAX_ENTRY_SIZE)) 
					{
						object_Array[num_entries] = new_entry;
						num_entries++;
					} 
					else if ((num_entries >= MAX_ENTRY_SIZE) && (match != 1)) 
					{
						cout << "You have reached the max amount of entries allowed" <<endl;
					} 
					// clear flag for next finding
					match = 0;
				}
			} 
			inputfile.close();
		}
		else 
		{
			cout << "Unable to open file" <<endl;
		}


		// Print MENU - Get next input file or quit the program
		cout << "\nDataStore updated in path : " << OUTPUT_PATH << endl;
		while(menuExit == 0) 
		{
			cout << "\n\t1) Input another file " << endl << "\t2) Quit" << endl;
			cin >> progExit;
			if (progExit == 1) 
			{
				progExit = 0;
				menuExit = 1;
			}
			else if (progExit == 2) 
			{ 
				menuExit = 1;
			} 
			else 
			{
				// Invalid input -- Anything other than 1 or 2     
				cout << "\nERROR : Invalid input, please pick again" <<endl;
			}
		}
		menuExit = 0;

	} // while(progExit==0)


	// After all of the files are inputted, output it into a file at the given path
	ofstream outputfile (OUTPUT_PATH);
	if (outputfile.is_open()) 
	{
		for (int i = 0; i < num_entries; i++) 
		{
			outputfile << object_Array[i]->get_STB() <<endl;
			outputfile << object_Array[i]->get_TITLE() <<endl;
			outputfile << object_Array[i]->get_PROVIDER() << endl;
			outputfile << object_Array[i]->get_DATE() << endl;
			outputfile << object_Array[i]->get_REV() << endl;
			outputfile << object_Array[i]->get_VIEW_TIME() <<endl;
		}
	}
	outputfile.close();

	// Make sure to go through and delete everything
	for(int i = 0; i < MAX_ENTRY_SIZE; i++) 
	{
		if (object_Array[i] != NULL) 
		{
			delete object_Array[i];
		}
	} 
	delete [] object_Array;
	delete [] temp;

	return 0;
}
