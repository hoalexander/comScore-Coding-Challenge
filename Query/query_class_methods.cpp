#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>

#include "query_classes.h"
using namespace std;

const int MAX_CHAR_SIZE = 64;
const int MAX_DATE_SIZE = 10;
const int MAX_MINUTE_PAD_SIZE = 10;
const string FIELDS[NUM_FIELDS_PER_ENTRY+1] = {"STB", "TITLE", "PROVIDER", "DATE", "REV", "VIEW_TIME"};

bool MIN_FLAG[NUM_FIELDS_PER_ENTRY+1];
bool MAX_FLAG[NUM_FIELDS_PER_ENTRY+1];
bool SUM_FLAG[NUM_FIELDS_PER_ENTRY+1];
bool COUNT_FLAG[NUM_FIELDS_PER_ENTRY+1];
bool COLLECT_FLAG[NUM_FIELDS_PER_ENTRY+1];


const int STB_INDEX = 0;
const int TITLE_INDEX = 1;
const int PROVIDER_INDEX = 2;
const int DATE_INDEX = 3;
const int REV_INDEX = 4;
const int VIEW_TIME_INDEX = 5;


////////////////////////////////// 
// Entry Methods
////////////////////////////////// 
Entry::Entry() 
{
	STB = new char[MAX_CHAR_SIZE+1];
	TITLE = new char[MAX_CHAR_SIZE+1];
	PROVIDER = new char[MAX_CHAR_SIZE+1];
	DATE = new char[MAX_DATE_SIZE+1];
	REV = 0;
	VIEW_TIME = 0;
	next_entry = NULL;
	order_string = new char[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
	aggregate_collect_string = new char[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
	aggregate_count_string = new char[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
	order_string[0] = aggregate_collect_string[0] = aggregate_count_string[0] = '\0';
	aggregate_collect_counter = aggregate_count_counter = 0;
}

Entry::~Entry() 
{
	delete [] STB;
	delete [] TITLE;
	delete [] PROVIDER;
	delete [] DATE;
	REV = 0;
	VIEW_TIME = 0;
	next_entry = NULL;
	delete [] order_string;
	delete [] aggregate_collect_string;
	delete [] aggregate_count_string;
	aggregate_collect_counter = aggregate_count_counter = 0;
}

char * Entry::get_STB() 
{
	return STB;
}

void Entry::set_STB(char* new_STB) 
{
	strcpy(STB, new_STB);
}

char * Entry::get_TITLE() 
{
	return TITLE;
}

void Entry::set_TITLE(char* new_TITLE) 
{
	strcpy(TITLE, new_TITLE);
}

char * Entry::get_PROVIDER() 
{
   return PROVIDER;
}

void Entry::set_PROVIDER(char * new_PROVIDER) 
{
   strcpy(PROVIDER, new_PROVIDER);
}

char * Entry::get_DATE() 
{
   return DATE;
}

void Entry::set_DATE(char* new_DATE) 
{
   strcpy(DATE, new_DATE);
}

int Entry::get_REV() 
{
   return REV;
}

void Entry::set_REV(int new_REV) 
{
   REV = new_REV;
}

int Entry::get_VIEW_TIME() 
{
   return VIEW_TIME;
}

void Entry::set_VIEW_TIME(int new_VIEW_TIME) 
{
   VIEW_TIME = new_VIEW_TIME;
}

Entry * Entry::get_next_entry() 
{
   return next_entry;
}

void Entry::set_next_entry(Entry* new_entry) 
{
   next_entry = new_entry;
}

char * Entry::get_order_string() 
{
   return order_string;
}

void Entry::set_order_string(char * new_order_string) 
{
   strcpy(order_string, new_order_string);
}

char * Entry::get_aggregate_count_string()
{
	return aggregate_count_string;
}

void Entry::set_aggregate_count_string(char * new_string)
{
	strcpy(aggregate_count_string, new_string);
}

char * Entry::get_aggregate_collect_string()
{
	return aggregate_collect_string;
}

void Entry::set_aggregate_collect_string(char * new_string)
{
	strcpy(aggregate_collect_string, new_string);
}

int Entry::get_aggregate_collect_counter()
{
	return aggregate_collect_counter;
}

int Entry::get_aggregate_count_counter()
{
	return aggregate_count_counter;
}

void Entry::set_aggregate_collect_counter(int new_count)
{
	aggregate_collect_counter = new_count;
}

void Entry::set_aggregate_count_counter(int new_count)
{
	aggregate_count_counter = new_count;
}


void Entry::create_ordered_string (string order_array[], int num_order_fields)
{
	char order_entries_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];

	// Create the order string
	order_entries_array[0] = '\0';
	for(int i = 0; i <num_order_fields; i++) 
	{
		for(int k = 0; k < NUM_FIELDS_PER_ENTRY; k++) 
		{
			if(strcmp(order_array[i].c_str(), FIELDS[k].c_str()) == 0) 
			{
				if(k == STB_INDEX) 
				{
					// Concatenate to the order string, add "   " to seperate fields
					strcat(order_entries_array,STB);
					strcat(order_entries_array, "   ");
				}
				if(k == TITLE_INDEX) 
				{
					// Concatenate to the order string, add "   " to seperate fields
					strcat(order_entries_array,TITLE);
					strcat(order_entries_array, "   ");
				}
				if(k == PROVIDER_INDEX) 
				{
					// Concatenate to the order string, add "   " to seperate fields
					strcat(order_entries_array,PROVIDER);
					strcat(order_entries_array, "   ");
				}
				if(k == DATE_INDEX) 
				{
					// Concatenate to the order string, add "   " to seperate fields
					strcat(order_entries_array,DATE);
					strcat(order_entries_array, "   ");
				}
				if(k == REV_INDEX) 
				{
					// Convert REV from int to char
					char buffer[MAX_CHAR_SIZE+1];
					sprintf(buffer, "%d", REV);

					// Concatenate to the order string, add "   " to seperate fields
					strcat(order_entries_array, buffer);
					strcat(order_entries_array, "   ");
				}
				if(k == VIEW_TIME_INDEX) 
				{
					char buffer[MAX_MINUTE_PAD_SIZE+1];
					sprintf(buffer, "%d",VIEW_TIME);
					char padded_view_time[MAX_MINUTE_PAD_SIZE+1];

					// Determine the strlen of VIEW_TIME
					int num_view_time = VIEW_TIME;
					int length_view_time = 0;
					while (num_view_time != 0) 
					{
						length_view_time++;
						num_view_time /= 10;
					}
					// Pad the beginning of VIEW_TIME with 0's for future comparisons
					strncpy(padded_view_time, "0000000000", MAX_MINUTE_PAD_SIZE);
					padded_view_time[MAX_MINUTE_PAD_SIZE] = '\0';
					strncpy(&padded_view_time[(MAX_MINUTE_PAD_SIZE - length_view_time)], buffer, length_view_time);
                             
					// Concatenate the padded string to the order string and add "   " to seperate order fields
					strcat(order_entries_array,padded_view_time);
					strcat(order_entries_array, "   ");
				}
			}
		}
	}

	// Store the string in the new_entry
	strcpy(order_string, order_entries_array);
                 
	// Clear order_entries_array
	strcpy(order_entries_array,"");
}






















/////////////////////////////////////
// EntryList Methods
/////////////////////////////////////

EntryList::EntryList() 
{
	head = NULL;
	string select_array[NUM_FIELDS_PER_ENTRY+1];
	string order_array[NUM_FIELDS_PER_ENTRY+1];
	string filter_array[NUM_FIELDS_PER_ENTRY+1];
	string group_array[NUM_FIELDS_PER_ENTRY+1];
	SELECT_FLAG = false;
	ORDER_FLAG = false;
	FILTER_FLAG = false;
	GROUP_FLAG = false;
	FILTER_MATCH = false;
	AND_OR_FLAG = false;
	num_order_fields = 0;
	num_select_fields = 0;
	num_group_fields = 0;
}

EntryList::~EntryList() 
{
	head = NULL;
	SELECT_FLAG = false;
	ORDER_FLAG = false;
	FILTER_FLAG = false;
	GROUP_FLAG = false;
	AND_OR_FLAG = false;
	FILTER_MATCH = false;
	num_order_fields = 0;
	num_select_fields = 0;
	num_group_fields = 0;
}

Entry * EntryList::get_head() 
{
   return head;
}

void EntryList::set_head(Entry* new_entry) 
{
   head = new_entry;
}

void EntryList::add_entry_in_sorted_order(Entry * new_entry) 
{
	Entry * this_node = head;
	Entry * prev_node = NULL;


	while(this_node != NULL) 
	{
		int strcmp_results = strcmp(this_node->get_order_string(), new_entry->get_order_string());

		// New entry is larger -- Continue to compare next node
		if( strcmp_results < 0) 
		{ 
			// Reached the end of the list -- Add it to the end
			if( this_node->get_next_entry() == NULL) 
			{
				this_node->set_next_entry(new_entry);
				break;                          
			}
			// Traverse to next node
			prev_node = this_node;
			this_node = this_node->get_next_entry();
		}
		// New entry is smaller
		else 
		{
			// Add to head of list
			if(this_node == head) 
			{
				new_entry->set_next_entry(this_node);
				head = new_entry;
			}
			// Add before in the middle of the list
			else 
			{ 
				new_entry->set_next_entry(this_node);
				prev_node->set_next_entry(new_entry);
			}
			break;

		}
	}
}


 void EntryList::add_entry_in_received_order(Entry* new_entry) 
 {
	// Not ordered -- Traverse to end of the LLL to add new entry
	Entry * temp_ptr = head;
	while(temp_ptr->get_next_entry() != NULL) 
	{
		temp_ptr = temp_ptr->get_next_entry();
	}
	temp_ptr->set_next_entry(new_entry);
 }

void EntryList::print_selected_fields() 
{
	Entry * temp_ptr = head;
	while(temp_ptr != NULL) 
	{
		cout << endl;
		for(int i = 0; i < num_select_fields; i++) 
		{
			for(int k = 0; k < NUM_FIELDS_PER_ENTRY; k++) 
			{
				if(strcmp(select_array[i].c_str(), FIELDS[k].c_str()) == 0) 
				{
					// Print STB
					if (k == STB_INDEX) 
					{
						if(COUNT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_count_counter() <= 1)
							{
								cout << "1,";
							}
							else
							{
								cout << temp_ptr->get_aggregate_count_counter() << ",";
							}
						}
						else if(COLLECT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_collect_counter() > 0)
							{
								cout << "[" << temp_ptr->get_aggregate_collect_string() << "],";
							}
							else
							{
								cout << "[" << temp_ptr->get_STB() << "],";
							}
						}
						else
						{
							cout << temp_ptr->get_STB() << ",";
						}
					}
					// Print TITLE
					if (k == TITLE_INDEX) 
					{
						if(COUNT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_count_counter() <= 1)
							{
								cout << "1,";
							}
							else
							{
								cout << temp_ptr->get_aggregate_count_counter() << ",";
							}
						}
						else if(COLLECT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_collect_counter() > 0)
							{
								cout << "[" << temp_ptr->get_aggregate_collect_string() << "],";
							}
							else
							{
								cout << "[" << temp_ptr->get_TITLE() << "],";
							}
						}
						else
						{
							cout << temp_ptr->get_TITLE() << ","; 
						}
					}
					// Print PROVIDER
					if (k == PROVIDER_INDEX) 
					{
						if(COUNT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_count_counter() <= 1)
							{
								cout << "1,";
							}
							else
							{
								cout << temp_ptr->get_aggregate_count_counter() << ",";
							}
						}
						else if(COLLECT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_collect_counter() > 0)
							{
								cout << "[" << temp_ptr->get_aggregate_collect_string() << "],";
							}
							else
							{
								cout << "[" << temp_ptr->get_PROVIDER() << "],";
							}
						}
						else
						{
							cout<< temp_ptr->get_PROVIDER() << ",";
						}
					}
					// Print DATE
					if (k == DATE_INDEX) 
					{
						if(COUNT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_count_counter() <= 1)
							{
								cout << "1,";
							}
							else
							{
								cout << temp_ptr->get_aggregate_count_counter() << ",";
							}
						}
						else if(COLLECT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_collect_counter() > 0)
							{
								cout << "[" << temp_ptr->get_aggregate_collect_string() << "],";
							}
							else
							{
								cout << "[" << temp_ptr->get_DATE() << "],";
							}
						}
						else
						{
							cout<< temp_ptr->get_DATE() << ",";
						}
					}
					// Print REV
					if (k == REV_INDEX) 
					{	
						char temp_array[100];
						char *pBuf = temp_array;

						if(COUNT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_count_counter() <= 1)
							{
								cout << "1,";
							}
							else
							{
								cout << temp_ptr->get_aggregate_count_counter() << ",";
							}
						}
						else if(COLLECT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_collect_counter() > 0)
							{
								char *pStr = temp_ptr->get_aggregate_collect_string();
								for (int j = 0; j < temp_ptr->get_aggregate_collect_counter(); j++)
								{
									int rev;
									sscanf(pStr, "%d", &rev);
									while(*pStr != ',')
                                                                        {
										++pStr;
									}
									++pStr; // Move past the comma
									pBuf += sprintf(pBuf,"%d.%02d,",rev/100, rev%100);
								}
								cout << "[" << temp_array << "],";
							}
							else
							{
								// Convert back to US dollar and cents format
								sprintf(temp_array, "%d.%02d", temp_ptr->get_REV()/100, temp_ptr->get_REV()%100);
								cout << "[" << temp_array << "],";
							}
						}
						else
						{
							// Convert back to US dollar and cents format
							sprintf(temp_array, "%d.%02d", temp_ptr->get_REV()/100, temp_ptr->get_REV()%100);
							cout << temp_array << ",";

						}
					}
					// Print VIEW_TIME
					if (k == VIEW_TIME_INDEX) 
					{
						char temp_array[100];
						char *pBuf = temp_array;

						if(COUNT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_count_counter() <= 1)
							{
								cout << "1,";
							}
							else
							{
								cout << temp_ptr->get_aggregate_count_counter() << ",";
							}
						}
						else if(COLLECT_FLAG[k] == true)
						{
							if(temp_ptr->get_aggregate_collect_counter() > 0)
							{
								char *pStr = temp_ptr->get_aggregate_collect_string();
								for (int j = 0; j < temp_ptr->get_aggregate_collect_counter(); j++)
								{
									int view_time;
									sscanf(pStr, "%d", &view_time);
									while(*pStr != ',')
									{
 										++pStr;
									}
									++pStr; // Move past the comma
									pBuf += sprintf(pBuf,"%d:%02d,",view_time/60,view_time%60);
								}
								cout << "[" << temp_array << "]" <<  ",";
							}
							else
							{
								// Convert back to hours:minutes format
								int total_minutes = temp_ptr->get_VIEW_TIME();
								sprintf(temp_array,"%d:%02d",total_minutes/60,total_minutes%60);
								cout << "[" << temp_array << "],";
							}
						}
						else
						{
							// Convert back to hours:minutes format
							int total_minutes = temp_ptr->get_VIEW_TIME();
							sprintf(temp_array,"%d:%02d",total_minutes/60,total_minutes%60);
							cout << temp_array << ",";
						}
					}
				}
			}
		}
		temp_ptr = temp_ptr->get_next_entry();
	}
}

void EntryList::parse_filter_flag(char * argv[], int argc) {
	for (int i = 0; i < argc; i++) 
	{
		// Check for the FILTER '-f' flag
		if( strcmp(argv[i], "-f") == 0) 
		{
			FILTER_FLAG = true;
         
			// Check for AND/OR
			if ((strstr(argv[i+1], " AND ") != NULL) || (strstr(argv[i+1], " OR ") != NULL))
			{
				// Set the AND_OR_FLAG to true
				AND_OR_FLAG = true;

				// Output the command to an output file to be used by our perl script later
				ofstream outputfile ("command.txt");
				if (outputfile.is_open()) 
				{
					outputfile << argv[i+1] <<endl;
				}
				outputfile.close();

			}

			// Parse the filtered field
			string filter = string(argv[i+1]);
			vector <string> vect;
			stringstream ss(filter);
     
			while ( ss.good()) 
			{
				getline(ss, filter, '=');
				vect.push_back(filter);
			}

			// Save the filtered fields for later
			for (int i = 0; i < vect.size(); i++) 
			{
				filter_array[i] = vect.at(i);
			}

		}
	}
}

void EntryList::parse_order_flag(char * argv[], int argc) 
{
	for (int i = 0; i < argc; i++) 
	{
		// Check for the ORDER '-o' flag
		if ( strcmp(argv[i], "-o") == 0) 
		{
			ORDER_FLAG = true; 
         
			// Parse the ordered fields
			string order = (argv[i+1]);
			vector <string> vect;
			stringstream ss(order);
 
			while (ss.good()) 
			{
				getline(ss, order, ',');
				vect.push_back(order);
			}

			// Save the ordered fields to be used later
			for (int i = 0; i < vect.size(); i++) 
			{
				order_array[i] = vect.at(i);
			}
			num_order_fields = vect.size();
		}
	}
}
void EntryList::parse_select_flag(char * argv[], int argc) 
{
	for (int i = 0; i < argc; i++) 
	{
		// Check for the SELECT '-s' flag
		if( strcmp(argv[i], "-s") == 0) 
		{
			SELECT_FLAG = true;
  
			// Parse the selected fields      
			string select = string(argv[i+1]);
			vector <string> vect;
			stringstream ss(select);

			while (ss.good()) 
			{
				getline(ss, select, ',');
				vect.push_back(select);
			}

			// Save the selected fields to be used later
			for (int i = 0; i < vect.size(); i++) 
			{
				select_array[i] = vect.at(i);
			}
			num_select_fields = vect.size();
		}
	}
}

bool EntryList::get_SELECT_FLAG() 
{
	return SELECT_FLAG;
}

bool EntryList::get_ORDER_FLAG()
{
	return ORDER_FLAG;
}

bool EntryList::get_FILTER_FLAG()
{
	return FILTER_FLAG;
}

bool EntryList::get_FILTER_MATCH()
{
	return FILTER_MATCH;
}

int EntryList::get_num_order_fields()
{
	return num_order_fields;
}

int EntryList::get_num_select_fields()
{
	return num_select_fields;
}

bool EntryList::get_AND_OR_FLAG()
{
	return AND_OR_FLAG;
}

void EntryList::check_filter_match(string inputFields[])
{
	// Check filter -- set flag upon match found
	for(int i = 0; i < NUM_FIELDS_PER_ENTRY; i++) 
	{
		if (  (strcmp(filter_array[0].c_str(), FIELDS[i].c_str()) == 0) && (strcmp(filter_array[1].c_str(), inputFields[i].c_str()) == 0) )
		{
			FILTER_MATCH = true;
		}
	}

	// Check for AND/OR
	if( AND_OR_FLAG == true)
	{
		// Print the fields for the new possible entry into a new input file
		ofstream outputfile ("perl_input.txt");
		if (outputfile.is_open()) 
		{
			for (int i = 0; i < NUM_FIELDS_PER_ENTRY; i++) 
			{
				outputfile << inputFields[i] <<endl;
			}
		}
		outputfile.close();

		// Call the perl script to evaluate the AND/OR expression
		system("perl filter_eval.pl");


		string result;

		// Read the result file created by the perl script
		ifstream inputfile("result.txt");
		if (inputfile.is_open()) 
		{
			getline (inputfile, result, '\n');
		}

		// If true -- set the FILTER_MATCH to true
		if (strcmp(result.c_str(),"1") == 0)
		{
			FILTER_MATCH = true;
		}
		inputfile.close();

	}
}


Entry * EntryList::create_new_entry_from_datastore(string inputFields[])
{
	// Create Entry
	Entry * new_entry = new Entry();

	// Copy the STB
	new_entry->set_STB((char *)inputFields[STB_INDEX].c_str());

	// Copy The TITLE
	new_entry->set_TITLE((char *)inputFields[TITLE_INDEX].c_str());

	// Copy the PROVIDER
	new_entry->set_PROVIDER((char *)inputFields[PROVIDER_INDEX].c_str());

	// Copy the Date
	new_entry->set_DATE((char *)inputFields[DATE_INDEX].c_str());

	// Copy the REV
	int num_cents = atoi((char *)inputFields[REV_INDEX].c_str());
	new_entry->set_REV(num_cents);        

	// Copy the VIEW_TIME  
	int num_minutes = atoi((char *)inputFields[VIEW_TIME_INDEX].c_str());
	new_entry->set_VIEW_TIME(num_minutes);  
 
	// Set it back to false for the next entry
	FILTER_MATCH = false;

	return new_entry;
}

string* EntryList::get_select_array() 
{
	return select_array;
}

string* EntryList::get_order_array()
{
	return order_array;
}

string* EntryList::get_filter_array()
{
	return filter_array;
}


void EntryList::parse_group_flag(char * argv[], int argc)
{
	for (int i = 0; i < argc; i++) 
	{
		if ( strcmp(argv[i], "-g") == 0) 
		{
			GROUP_FLAG = true;
        
			// Save the grouped field for later
			group_array[0] = argv[i+1];
			num_group_fields = 1;

			// Check the SELECT block for aggregate functions
			char * select = argv[2];
         
			// Check for all of the flags
			// MIN flags
			if (strstr(select, "STB:min") != NULL) 
			{
				MIN_FLAG[STB_INDEX] = true;
			}
			if (strstr(select, "TITLE:min") != NULL) 
			{
				MIN_FLAG[TITLE_INDEX] = true;
			}
			if (strstr(select, "PROVIDER:min") != NULL) 
			{
				MIN_FLAG[PROVIDER_INDEX] = true;
			}
			if (strstr(select, "DATE:min") != NULL) 
			{
				MIN_FLAG[DATE_INDEX] = true;
			}
			if (strstr(select, "REV:min") != NULL) 
			{
				MIN_FLAG[REV_INDEX] = true;
			}
			if (strstr(select, "VIEW_TIME:min") != NULL) 
			{
				MIN_FLAG[VIEW_TIME_INDEX] = true;
			}
			// MAX flags
			if (strstr(select, "STB:max") != NULL) 
			{
				MAX_FLAG[STB_INDEX] = true;
			}
			if (strstr(select, "TITLE:max") != NULL) 
			{
				MAX_FLAG[TITLE_INDEX] = true;
			}
			if (strstr(select, "PROVIDER:max") != NULL) 
			{
				MAX_FLAG[PROVIDER_INDEX] = true;
			}
			if (strstr(select, "DATE:max") != NULL) 
			{
				MAX_FLAG[DATE_INDEX] = true;
			}
			if (strstr(select, "REV:max") != NULL) 
			{
				MAX_FLAG[REV_INDEX] = true;
			}
			if (strstr(select, "VIEW_TIME:max") != NULL) 
			{
				MAX_FLAG[VIEW_TIME_INDEX] = true;
			}

			// SUM flags
			if (strstr(select, "REV:sum") != NULL)
			{
				SUM_FLAG[REV_INDEX] = true;
			}
			if (strstr(select, "VIEW_TIME:sum") != NULL) 
			{
				SUM_FLAG[VIEW_TIME_INDEX] = true;
			}

			// COUNT flags
			if (strstr(select, "STB:count") != NULL) 
			{
				COUNT_FLAG[STB_INDEX] = true;
			}
			if (strstr(select, "TITLE:count") != NULL) 
			{
				COUNT_FLAG[TITLE_INDEX] = true;
			}
			if (strstr(select, "PROVIDER:count") != NULL) 
			{
				COUNT_FLAG[PROVIDER_INDEX] = true;
			}
			if (strstr(select, "DATE:count") != NULL) 
			{
				COUNT_FLAG[DATE_INDEX] = true;
			}
			if (strstr(select, "REV:count") != NULL) 
			{
				COUNT_FLAG[REV_INDEX] = true;
			}
			if (strstr(select, "VIEW_TIME:count") != NULL) 
			{
				COUNT_FLAG[VIEW_TIME_INDEX] = true;
			}

			// COLLECT flags
			if (strstr(select, "STB:collect") != NULL) 
			{
				COLLECT_FLAG[STB_INDEX] = true;
			}
			if (strstr(select, "TITLE:collect") != NULL) 
			{
				COLLECT_FLAG[TITLE_INDEX] = true;
			}
			if (strstr(select, "PROVIDER:collect") != NULL) 
			{
				COLLECT_FLAG[PROVIDER_INDEX] = true;
			}
			if (strstr(select, "DATE:collect") != NULL) 
			{
				COLLECT_FLAG[DATE_INDEX] = true;
			}
			if (strstr(select, "REV:collect") != NULL) 
			{
				COLLECT_FLAG[REV_INDEX] = true;
			}
			if (strstr(select, "VIEW_TIME:collect") != NULL) 
			{
				COLLECT_FLAG[VIEW_TIME_INDEX] = true;
			}

			// Reparse the select block to get rid of the aggregation functions
			// Reinitialize the counter
			num_select_fields = 0;

			// Get the select arguments and parse it by the commas
			string select_block = string(argv[2]); 
			string temp_string_array[NUM_FIELDS_PER_ENTRY+1];
			temp_string_array[0] = '\0';
			vector <string> vect;
			stringstream ss(select_block);
 
			while (ss.good()) 
			{
				getline(ss, select_block, ',');
				vect.push_back(select_block);
			}
			for (int k = 0; k < vect.size(); k++) 
			{
				temp_string_array[k] = vect.at(k);
			}

			int vect_size = vect.size();

			// Further parse each of the previous parsed arguments for colons
			for (int k = 0; k < vect_size; k++) 
			{
				// Clear out the stringstream and vector to reuse it
				ss.clear();
				vect.clear();
            
				// Parse out the colons from the aggregate functions
				ss.str(temp_string_array[k]);
				while (ss.good()) 
				{
					getline(ss, temp_string_array[k], ':');
					vect.push_back(temp_string_array[k]);
				}
            
				// Store the select field into the select_array -- overwrite the old one unparsed one
				select_array[num_select_fields] = vect.at(0);

				// Increment the counter
				num_select_fields++;
			}
		}
	}
}

void EntryList::add_new_entry_to_list(Entry * new_entry)
{
	// Add the entries into the list
	if(head == NULL) 
	{
		// Add the first entry
		head = new_entry;
	}
	else 
	{
		// Add entries in order
		if(ORDER_FLAG == true) 
		{
			add_entry_in_sorted_order(new_entry);
		}

		// Add entries in the received order -- add to the end of the list
		else 
		{
			add_entry_in_received_order(new_entry);
		}
	}
}

bool EntryList::get_GROUP_FLAG()
{
	return GROUP_FLAG;
}

void EntryList::do_group_processing(EntryList * list_head)
{
	bool match_found = false;

	// Traverse the complete list of all entries from datastore
	Entry * list_node = list_head->get_head();
	Entry * group_node = NULL;
	Entry * group_end = NULL;

	// Group list is empty, add the first entry
	if(head == NULL) 
	{
		list_head->set_head(list_node->get_next_entry());
		list_node->set_next_entry(NULL);
		head = list_node;
		group_node = group_end = head;
		list_node = list_head->get_head();
	}

	while(list_node != NULL) 
	{
		// Detach a node from head of list_head
		list_head->set_head(list_node->get_next_entry());
		list_node->set_next_entry(NULL);

		match_found = false;
		group_node = head;

		while(group_node != NULL)
		{
			// Compare current node with detached node
			for(int i = 0; i < num_group_fields; i++) 
			{
				for(int k = 0; k < NUM_FIELDS_PER_ENTRY; k++) 
				{
					if(strcmp(group_array[i].c_str(), FIELDS[k].c_str()) == 0) 
					{
						// Check for a match between the current grouped node and the detatched node for the grouped field
						if (  ((k == STB_INDEX) && (strcmp(list_node->get_STB(), group_node->get_STB()) == 0))
						   || ((k == TITLE_INDEX) && (strcmp(list_node->get_TITLE(), group_node->get_TITLE()) == 0))
						   || ((k == PROVIDER_INDEX) && (strcmp(list_node->get_PROVIDER(), group_node->get_PROVIDER()) == 0))
						   || ((k == DATE_INDEX) && (strcmp(list_node->get_DATE(), group_node->get_DATE()) == 0))
						   || ((k == REV_INDEX) && ( list_node->get_REV() == group_node->get_REV()))
						   || ((k == VIEW_TIME_INDEX) && ( list_node->get_VIEW_TIME() == group_node->get_VIEW_TIME()))
						   )
						{
							// We found a match for the grouped field -- perform aggregate functions on two matched Entry nodes
							match_found = true;
							perform_all_aggregate_functions(group_node, list_node);
						}
					}
				}
			}

			// Traverse to the next node in the group list
			group_node = group_node->get_next_entry();
		}


		// No match, add the detatched node to the end of grouped list
		if (match_found == false)
		{
			group_end->set_next_entry(list_node);
			group_end = group_end->get_next_entry();
		}

		// Traverse to the next node in the completed list
		list_node = list_head->get_head();

	} 
}


void EntryList::do_min_aggregate(Entry * group_node, Entry * list_node)
{
	// Check min
	for(int j= 0; j < NUM_FIELDS_PER_ENTRY; j++)
	{
		if (MIN_FLAG[j] == true) 
		{
			switch(j)
			{
				// Find and replace with the lowest value into the grouped node
				// If the grouped node is smaller than the detatched node, do nothing
				case STB_INDEX:
					if (strcmp(list_node->get_STB(), group_node->get_STB()) < 0)
					{
						group_node->set_STB(list_node->get_STB());
					}
					break;
				case TITLE_INDEX:
					if (strcmp(list_node->get_TITLE(), group_node->get_TITLE()) < 0)
					{
						group_node->set_TITLE(list_node->get_TITLE());
					}
					break;
				case PROVIDER_INDEX:
					if (strcmp(list_node->get_PROVIDER(), group_node->get_PROVIDER()) < 0)
					{
						group_node->set_PROVIDER(list_node->get_PROVIDER());
					}
					break;
				case DATE_INDEX:
					if (strcmp(list_node->get_DATE(), group_node->get_DATE()) < 0)
					{
						group_node->set_DATE(list_node->get_DATE());
					}
					break;
				case REV_INDEX:
					if (list_node->get_REV() < group_node->get_REV())
					{
						group_node->set_REV(list_node->get_REV());
					}
					break;
				case VIEW_TIME_INDEX:
					if (list_node->get_VIEW_TIME() < group_node->get_VIEW_TIME())
					{
						group_node->set_VIEW_TIME(list_node->get_VIEW_TIME());
					}
					break;
			}
		}
	}  
}


void EntryList::do_max_aggregate(Entry * group_node, Entry * list_node)
{
	// Check max
	for(int j= 0; j < NUM_FIELDS_PER_ENTRY; j++)
	{
		if (MAX_FLAG[j] == true) 
		{
			switch(j)
			{
				// Find and replace with the highest value into the grouped node
				// If the grouped node is bigger than the detatched node, do nothing
				case STB_INDEX:
					if (strcmp(list_node->get_STB(), group_node->get_STB()) > 0)
					{
						group_node->set_STB(list_node->get_STB());
					}
					break;
				case TITLE_INDEX:
					if (strcmp(list_node->get_TITLE(), group_node->get_TITLE()) > 0)
					{
						group_node->set_TITLE(list_node->get_TITLE());
					}
					break;
				case PROVIDER_INDEX:
					if (strcmp(list_node->get_PROVIDER(), group_node->get_PROVIDER()) > 0)
					{
						group_node->set_PROVIDER(list_node->get_PROVIDER());
					}
					break;
				case DATE_INDEX:
					if (strcmp(list_node->get_DATE(), group_node->get_DATE()) > 0)
					{
						group_node->set_DATE(list_node->get_DATE());
					}
					break;
				case REV_INDEX:
					if (list_node->get_REV() > group_node->get_REV())
					{
						group_node->set_REV(list_node->get_REV());
					}
					break;
				case VIEW_TIME_INDEX:
					if (list_node->get_VIEW_TIME() > group_node->get_VIEW_TIME())
					{
						group_node->set_VIEW_TIME(list_node->get_VIEW_TIME());
					}
					break;
			}
		}
	}  
}

void EntryList::do_sum_aggregate(Entry * group_node, Entry * list_node)
{
	// Check sum
	for(int j= 0; j < NUM_FIELDS_PER_ENTRY; j++)
	{
		if (SUM_FLAG[j] == true) 
		{
			// Only perform sum on fields with numbers -- Only REV and VIEW_TIME
			// Sum the values of both the grouped node and detatched node and store it back into the grouped node
			switch(j)
			{
				case STB_INDEX:
				case TITLE_INDEX:
				case PROVIDER_INDEX:
				case DATE_INDEX:
					break;
				case REV_INDEX:
				{
					int sum = list_node->get_REV() + group_node->get_REV();
					group_node->set_REV(sum);
					break;
				}
				case VIEW_TIME_INDEX:
				{
					int sum = list_node->get_VIEW_TIME() + group_node->get_VIEW_TIME();
					group_node->set_VIEW_TIME(sum);
					break;
				}
			}
		}
	}  
}



void EntryList::do_count_aggregate(Entry * group_node, Entry * list_node)
{
	// Check count
	for(int j= 0; j < NUM_FIELDS_PER_ENTRY; j++)
	{
		if (COUNT_FLAG[j] == true) 
		{
			switch(j)
			{
				case STB_INDEX:
				{
					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_count_counter() == 0)
					{
						group_node->set_aggregate_count_string((strcat(group_node->get_aggregate_count_string(), group_node->get_STB())));
						group_node->set_aggregate_count_string((strcat(group_node->get_aggregate_count_string(), ",")));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					strcat(comparison_array, list_node->get_STB());
					strcat(comparison_array, ",");

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_count_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_count_string(strcat(group_node->get_aggregate_count_string(),comparison_array));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}
					break;
				}
				case TITLE_INDEX:
				{
					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_count_counter() == 0)
					{
						group_node->set_aggregate_count_string((strcat(group_node->get_aggregate_count_string(), group_node->get_TITLE())));
						group_node->set_aggregate_count_string((strcat(group_node->get_aggregate_count_string(), ",")));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					strcat(comparison_array, list_node->get_TITLE());
					strcat(comparison_array, ",");

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_count_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_count_string(strcat(group_node->get_aggregate_count_string(),comparison_array));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}
					break;
				}
				case PROVIDER_INDEX:
				{
					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_count_counter() == 0)
					{
						group_node->set_aggregate_count_string((strcat(group_node->get_aggregate_count_string(), group_node->get_PROVIDER())));
						group_node->set_aggregate_count_string((strcat(group_node->get_aggregate_count_string(), ",")));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					strcat(comparison_array, list_node->get_PROVIDER());
					strcat(comparison_array, ",");

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_count_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_count_string(strcat(group_node->get_aggregate_count_string(),comparison_array));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}
					break;
				}
				case DATE_INDEX:
				{
					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_count_counter() == 0)
					{
						group_node->set_aggregate_count_string((strcat(group_node->get_aggregate_count_string(), group_node->get_DATE())));
						group_node->set_aggregate_count_string((strcat(group_node->get_aggregate_count_string(), ",")));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					strcat(comparison_array, list_node->get_DATE());
					strcat(comparison_array, ",");

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_count_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_count_string(strcat(group_node->get_aggregate_count_string(),comparison_array));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}
					break;
				}
				case REV_INDEX:
				{
					// Convert REV from int to string
					char temp_str[MAX_CHAR_SIZE + 1];
					sprintf(temp_str, "%d,%c", group_node->get_REV(), '\0');

					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_count_counter() == 0)
					{
						group_node->set_aggregate_count_string((strcat(group_node->get_aggregate_count_string(),temp_str)));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					sprintf(temp_str, "%d,%c", list_node->get_REV(), '\0');
					strcat(comparison_array, temp_str);

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_count_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_count_string(strcat(group_node->get_aggregate_count_string(),comparison_array));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}
					break;
				}
				case VIEW_TIME_INDEX:
				{
					// Convert VIEW_TIME from int to string
					char temp_str[MAX_CHAR_SIZE + 1];
					sprintf(temp_str, "%d,%c", group_node->get_VIEW_TIME(), '\0');

					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_count_counter() == 0)
					{
						group_node->set_aggregate_count_string((strcat(group_node->get_aggregate_count_string(),temp_str)));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					sprintf(temp_str, "%d,%c", list_node->get_VIEW_TIME(), '\0');
					strcat(comparison_array, temp_str );

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_count_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_count_string(strcat(group_node->get_aggregate_count_string(),comparison_array));
						group_node->set_aggregate_count_counter(group_node->get_aggregate_count_counter() + 1);
					}
					break;
				}
			}
		}
	}  
}

void EntryList::do_collect_aggregate(Entry * group_node, Entry * list_node)
{
	// Check collect
	for(int j= 0; j < NUM_FIELDS_PER_ENTRY; j++)
	{
		if (COLLECT_FLAG[j] == true) 
		{
			switch(j)
			{
				case STB_INDEX:
				{
					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_collect_counter() == 0)
					{
						group_node->set_aggregate_collect_string((strcat(group_node->get_aggregate_collect_string(), group_node->get_STB())));
						group_node->set_aggregate_collect_string((strcat(group_node->get_aggregate_collect_string(), ",")));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					strcat(comparison_array, list_node->get_STB());
					strcat(comparison_array, ",");

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_collect_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_collect_string(strcat(group_node->get_aggregate_collect_string(),comparison_array));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}
					break;
				}
				case TITLE_INDEX:
				{
					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_collect_counter() == 0)
					{
						group_node->set_aggregate_collect_string((strcat(group_node->get_aggregate_collect_string(), group_node->get_TITLE())));
						group_node->set_aggregate_collect_string((strcat(group_node->get_aggregate_collect_string(), ",")));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					strcat(comparison_array, list_node->get_TITLE());
					strcat(comparison_array, ",");

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_collect_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_collect_string(strcat(group_node->get_aggregate_collect_string(),comparison_array));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}
					break;
				}
				case PROVIDER_INDEX:
				{
					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_collect_counter() == 0)
					{
						group_node->set_aggregate_collect_string((strcat(group_node->get_aggregate_collect_string(), group_node->get_PROVIDER())));
						group_node->set_aggregate_collect_string((strcat(group_node->get_aggregate_collect_string(), ",")));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					strcat(comparison_array, list_node->get_PROVIDER());
					strcat(comparison_array, ",");

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_collect_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_collect_string(strcat(group_node->get_aggregate_collect_string(),comparison_array));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}
					break;
				}
				case DATE_INDEX:
				{
					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_collect_counter() == 0)
					{
						group_node->set_aggregate_collect_string((strcat(group_node->get_aggregate_collect_string(), group_node->get_DATE())));
						group_node->set_aggregate_collect_string((strcat(group_node->get_aggregate_collect_string(), ",")));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					strcat(comparison_array, list_node->get_DATE());
					strcat(comparison_array, ",");

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_collect_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_collect_string(strcat(group_node->get_aggregate_collect_string(),comparison_array));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}
					break;
				}
				case REV_INDEX:
				{
					// Convert REV from int to a string
					char temp_str[MAX_CHAR_SIZE + 1];
					sprintf(temp_str, "%d,%c", group_node->get_REV(), '\0');

					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_collect_counter() == 0)
					{
						group_node->set_aggregate_collect_string((strcat(group_node->get_aggregate_collect_string(),temp_str)));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					sprintf(temp_str, "%d,%c", list_node->get_REV(), '\0');
					strcat(comparison_array, temp_str );

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_collect_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_collect_string(strcat(group_node->get_aggregate_collect_string(),comparison_array));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}
					break;
				}
				case VIEW_TIME_INDEX:
				{
					// Convert VIEW_TIME from int to a string
					char temp_str[MAX_CHAR_SIZE + 1];
					sprintf(temp_str, "%d,%c", group_node->get_VIEW_TIME(), '\0');


					// If the count is still 0 -- Concatenate the first entry into the string before adding the new one
					if (group_node->get_aggregate_collect_counter() == 0)
					{
						group_node->set_aggregate_collect_string((strcat(group_node->get_aggregate_collect_string(),temp_str)));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}

					// Create a comparison string using the new entry from list_node
					char comparison_array[(NUM_FIELDS_PER_ENTRY * MAX_CHAR_SIZE) +1];
					comparison_array[0] = '\0';
					sprintf(temp_str, "%d,%c", list_node->get_VIEW_TIME(), '\0');
					strcat(comparison_array, temp_str );

					// If the comparison string cannot be found in the collect_string then it is distinct
					if (strstr(group_node->get_aggregate_collect_string(), comparison_array) == NULL) 
					{
						// New distinct value, add it to the collection and update the counter
						group_node->set_aggregate_collect_string(strcat(group_node->get_aggregate_collect_string(),comparison_array));
						group_node->set_aggregate_collect_counter(group_node->get_aggregate_collect_counter() + 1);
					}
					break;
				}
			}
		}
	}  
}


void EntryList::perform_all_aggregate_functions(Entry * group_node, Entry * list_node)
{
	// Call all of the aggregate functions on the matching nodes
	do_min_aggregate(group_node, list_node);
	do_max_aggregate(group_node, list_node);
	do_sum_aggregate(group_node, list_node);
	do_count_aggregate(group_node, list_node);
	do_collect_aggregate(group_node, list_node);
}



void EntryList::print_order_grouped_list(EntryList * grouped_list)
{
	EntryList * ordered_group_list = new EntryList();
	*ordered_group_list = *grouped_list;
	ordered_group_list->set_head(NULL);

	Entry * this_node = grouped_list->get_head();
	while(this_node != NULL)
	{
		ordered_group_list->add_entry_in_sorted_order(this_node);
		this_node = this_node->get_next_entry();
	}

	// Print ordered list
	ordered_group_list->print_selected_fields();
	delete ordered_group_list;
}


