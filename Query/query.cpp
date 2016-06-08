/*
Alexander Ho
comScore Coding Challenge
2) Query Tool - 2.1, 2.2 & 2.3

This takes the datastore created from the Importer program and reads in 6 lines at a time to create a new Entry.
It parses the command line arguments and sets flags based on what commands are being called.
Each new entry will first get checked through a filter if the filter flag has been set, if it doesn't match it will not get added to the list.
It will then check for the order flag, and compare it with the current list to determine where it should be placed to keep the linked list in order.
The order comparisons are done with order_string, it is a concatenated string in the order of the fields we need to order
After the entire input file has been added to the list, it will go through and print out all of the selected fields.

It will also handle grouping and aggregate functions. It will go through the command line arguments and set flags accordingly.
The grouping and aggregate functions will create a second list, then compare and link nodes from the original list to it.
It will begin by checking for the field that it is being grouped by, if it matches it will loop through all of the aggregate flags and perform their operations.
If it is not found in the new grouped list, it will be added to the end and be considered a new grouping.
The aggregate functions are performed and overwritten into the fields of the Entry nodes in the grouped list.

When handling the AND/OR statements, the program will take the expression and send it to a perl script that will use the eval() function to determine 
whether the expression is true/false and return it.

*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

#include "query_classes.h"


int main(int argc, char* argv[]) 
{
	const char *DATASTORE_FILE_PATH = "output.txt";
	string inputFields[NUM_FIELDS_PER_ENTRY+1];
	EntryList * EntryHead = new EntryList();

	// Check the command and determine all of the flags
	// Go through marking all of the flags that were found
	EntryHead->parse_filter_flag(argv,argc);
	EntryHead->parse_order_flag(argv, argc);
	EntryHead->parse_select_flag(argv, argc);
	EntryHead->parse_group_flag(argv, argc);


	// Get the data from our datastore
	ifstream inputfile(DATASTORE_FILE_PATH);
	if (inputfile.is_open()) 
	{
		// Grab 6 lines at a time -- This will be one entry
		while ( getline (inputfile, inputFields[0], '\n') != NULL) 
		{
			for (int i = 1; i < NUM_FIELDS_PER_ENTRY; i++) 
			{
				getline (inputfile, inputFields[i], '\n');
			}

			// Check filter -- Possibly ignore entry
			if (EntryHead->get_FILTER_FLAG() == true)
			{
				//Set the FILTER_MATCH flag based on the results
				EntryHead->check_filter_match(inputFields);
			}

			// Create a new Entry if it passes the filter check
			if ((EntryHead->get_FILTER_FLAG() == false) || (EntryHead->get_FILTER_MATCH() == true)) 
			{
				//Create a new Entry
				Entry * new_entry = EntryHead->create_new_entry_from_datastore(inputFields);

				//Check to see if we need to create an ordered_string
				if(EntryHead->get_ORDER_FLAG() == true) 
				{
					//Create an ordered_string to compare the entries with later to make sure they are sorted correctly
					new_entry->create_ordered_string(EntryHead->get_order_array(), EntryHead->get_num_order_fields());
				} 

				//Add the entries to the list
				EntryHead->add_new_entry_to_list(new_entry);
			}
		}
	}

	//Complete list of the whole datastore has been created
	if(EntryHead->get_GROUP_FLAG() == true) 
	{
		// Create a new head for the grouped list and copy the contents of the original head
		EntryList * GroupedHead = new EntryList();
		*GroupedHead = *EntryHead;
		GroupedHead->set_head(NULL);

		// Do group processing on the original list, storing it into the new grouped list
		GroupedHead->do_group_processing(EntryHead);

		// All entries added - Print out the selected fields for the grouped list
		GroupedHead->print_selected_fields();

		//After printing out, delete the list
		GroupedHead->delete_entry_list(GroupedHead);
		delete GroupedHead;
	} 
	else
	{
		// All entries added - Print out the selected fields for the ungrouped list
		EntryHead->print_selected_fields();
	}

	cin.get();
	inputfile.close();
	EntryHead->delete_entry_list(EntryHead);
	delete EntryHead;


	return 0;
}
