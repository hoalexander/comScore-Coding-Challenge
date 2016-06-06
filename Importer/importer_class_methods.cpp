#include <iostream>
#include <string.h>
#include "importer_classes.h"

using namespace std;

const int MAX_CHAR_SIZE = 64;
const int MAX_DATE_SIZE = 10;
const int NUM_FIELDS_PER_ENTRY = 6;

// Constructor for class Entry
Entry::Entry() 
{
   STB = new char[MAX_CHAR_SIZE+1];
   TITLE = new char[MAX_CHAR_SIZE+1];
   PROVIDER = new char[MAX_CHAR_SIZE+1];
   DATE = new char[MAX_DATE_SIZE+1];
   REV = 0;
   VIEW_TIME = 0;
}

// Destructor for class Entry
Entry::~Entry() 
{
   delete [] STB;
   delete [] TITLE;
   delete [] PROVIDER;
   delete [] DATE;
   REV = 0;
   VIEW_TIME = 0;
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