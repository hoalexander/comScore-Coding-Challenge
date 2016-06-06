#include <iostream>
using namespace std;

class Entry {
   public:
      Entry(); 
      ~Entry();
      char * get_STB();
      void set_STB(char*);
      char * get_TITLE();
      void set_TITLE(char*);
      char * get_PROVIDER();
      void set_PROVIDER(char*);
      char * get_DATE();
      void set_DATE(char*);
      int get_REV();
      void set_REV(int);
      int get_VIEW_TIME();
      void set_VIEW_TIME(int);
   private:
      char * STB;
      char * TITLE;
      char * PROVIDER;
      char * DATE;
      int REV;
      int VIEW_TIME;
};
