#ifndef MEMORY_MAGMENT_H
#define MEMORY_MAGMENT_H

#include<string>
#include<vector>

#include<algorithm>

using namespace std;
struct free_item_information
{
	int base_address;
	int limit;

};
struct allocated_item_information
{
	string process_name;
	string segment_name;
	int base_address;
	int limit;
	int process_index;
};
struct segment {
	string process_name;
	string segment_name;
	int limit;
};
class memory_mangement {
protected:
	int size_of_memory;
	vector <free_item_information>free_table;
	vector <allocated_item_information>allocated_table;
	vector <vector<segment>>waiting_queue;
	int process_index = 0;//initial
	int allocated_index = 0;
    int starting_temp = 0;
	vector<string>type_of_process_in_waiting_queue;
	vector<vector<allocated_item_information>>process_information;//process table
    vector<allocated_item_information>memory_section;


public:

    void deallocation_old_process(int start_address);
    vector<allocated_item_information>return_memory_section();
    void ClearAllData();
	void set_size_of_memory(int size);
    vector<std::string> ProcessNames() const;
	void initial_allocated_table();

	void allocated_space_index();

	void set_free_space(int starting_address, int limit_space);

	void follow_free_space();

	void delete_from_allocated_table(int element);
	void create_new_process_first_fit(vector<segment> new_process);
   
	void create_new_process_best_fit(vector<segment> new_process);
	void arrange_allocated_table();
	void up_to_size();
	void mangement_waiting_queue();
	vector <free_item_information> arrange_free_table_according_to_starting_address(vector<free_item_information>temp);
	vector <free_item_information> arrange_free_table_according_to_limit_size(vector <free_item_information>temp);
    void deallocated(string deallocated_process_name);
	void delete_limit__lower_than_zero_from_allocated_table();
	vector <free_item_information>return_free_table();
	vector <allocated_item_information>return_allocated_table();
	vector <vector<segment>>return_waiting_queue();
	vector<vector<allocated_item_information>>return_process_information();

};


#endif
