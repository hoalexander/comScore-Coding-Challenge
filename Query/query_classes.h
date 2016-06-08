#include <iostream>
using namespace std;

#define NUM_FIELDS_PER_ENTRY 6

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
		Entry * get_next_entry();
		void set_next_entry(Entry*);
		char * get_order_string();
		void set_order_string(char *);
		void create_ordered_string(string order_array[], int);
		char * get_aggregate_count_string();
		void set_aggregate_count_string(char *);
		int get_aggregate_count_counter();
		void set_aggregate_count_counter(int);
		char * get_aggregate_collect_string();
		void set_aggregate_collect_string(char *);
		int get_aggregate_collect_counter();
		void set_aggregate_collect_counter(int);

	private:
		char * STB;
		char * TITLE;
		char * PROVIDER;
		char * DATE;
		int REV;
		int VIEW_TIME;

		Entry * next_entry;
		char * order_string;

		char * aggregate_collect_string;
		char * aggregate_count_string;
		int aggregate_collect_counter;
		int aggregate_count_counter;
};


class EntryList {
	public:
		EntryList();
		~EntryList();
		Entry * get_head();
		void set_head(Entry*);
		void add_entry_in_sorted_order(Entry *);
		void add_entry_in_received_order(Entry*);
		void print_selected_fields();
		void parse_filter_flag(char * argv[], int);
		void parse_order_flag(char * argv[], int);
		void parse_select_flag(char * argv[], int);
		void parse_group_flag(char * argv[], int);
		bool get_SELECT_FLAG();
		bool get_ORDER_FLAG();
		bool get_FILTER_FLAG();
		bool get_FILTER_MATCH();
		bool get_GROUP_FLAG();
		bool get_AND_OR_FLAG();
		int get_num_order_fields();
		int get_num_select_fields();
		void check_filter_match(string inputFields[]);
		Entry * create_new_entry_from_datastore(string inputFields[]);
		void add_new_entry_to_list(Entry *);
		string* get_select_array();		
		string* get_order_array();
		string* get_filter_array();
		void do_group_processing(EntryList *);
		void do_min_aggregate(Entry *, Entry *);
		void do_max_aggregate(Entry *, Entry *);
		void do_sum_aggregate(Entry *, Entry *);
		void do_count_aggregate(Entry *, Entry *);
		void do_collect_aggregate(Entry *, Entry *);
		void perform_all_aggregate_functions(Entry *, Entry *);
		void print_order_grouped_list(EntryList *);
		void delete_entry_list(EntryList *);

	private:
		Entry * head;
		string select_array[NUM_FIELDS_PER_ENTRY+1];
		string order_array[NUM_FIELDS_PER_ENTRY+1];
		string filter_array[NUM_FIELDS_PER_ENTRY+1];
		string group_array[NUM_FIELDS_PER_ENTRY+1];
		bool SELECT_FLAG;
		bool ORDER_FLAG;
		bool FILTER_FLAG;
		bool GROUP_FLAG;
		bool FILTER_MATCH;
		bool AND_OR_FLAG;
		int num_order_fields;
		int num_select_fields;
		int num_group_fields;
};
