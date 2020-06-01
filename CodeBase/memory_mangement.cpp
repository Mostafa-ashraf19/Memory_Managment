#include "memory_mangement.h"

void memory_mangement::ClearAllData()
{
    this->memory_section.clear();
    this->free_table.clear();
    this->waiting_queue.clear();
    this->allocated_table.clear();
    this->process_information.clear();
    this->type_of_process_in_waiting_queue.clear();
    process_index = 0;//initial
    allocated_index = 0;
    starting_temp = 0;
}

void memory_mangement::deallocation_old_process(int start_address)
{
    auto it = allocated_table.begin();
    for (auto i : allocated_table)
    {
//        if (i.process_name == "allocated space")
//        {
            if (i.base_address == start_address)
            {
                free_table.push_back({ i.base_address, i.limit });
                it = allocated_table.erase(it);
                break;
            }
//        }
        it++;
    }
    free_table = arrange_free_table_according_to_starting_address(free_table);
    follow_free_space();

}

void memory_mangement::set_size_of_memory(int size)
{
	size_of_memory = size;
    initial_allocated_table();
}

vector<string> memory_mangement::ProcessNames() const
{
    vector<string> v;
    for(auto& vec: this->process_information)
            v.push_back(vec.front().process_name);
    return v;
}

void memory_mangement::initial_allocated_table()
{

	allocated_table.push_back({ "allocated space", "allocated space", 0, size_of_memory, 0 });
	allocated_space_index();
}

void memory_mangement::allocated_space_index()
{
	allocated_index = 0;
	for (int i = 0; i < allocated_table.size(); i++)
	{
		if (allocated_table[i].process_name == "allocated space")
		{
			allocated_index--;
			allocated_table[i].process_index = allocated_index;
		}
	}
}

void memory_mangement::set_free_space(int starting_address, int limit_space)
{
//	static int starting_temp = 0;
	for (int i = 0; i < free_table.size(); i++)
	{
		if (free_table[i].base_address == starting_address)
		{
			if (free_table[i].limit >= limit_space)
			{
				return;
			}
			else
			{

				free_table[i].limit = limit_space;
				int x = allocated_table[i + 1].base_address + allocated_table[i + 1].limit;
				allocated_table[i + 1].base_address = starting_address + limit_space;
				allocated_table[i + 1].limit = x - allocated_table[i + 1].base_address;
				follow_free_space();
				allocated_space_index();
				return;

			}
		}
	}

	allocated_table.pop_back();
	free_table.push_back({ starting_address,limit_space });
	////////////////////////////////////////////////////////////////////
	//if(starting_address - starting_temp>0)
	allocated_table.push_back({ "allocated space","allocated space",starting_temp,starting_address - starting_temp,0 });
	starting_temp = starting_address + limit_space;
	//if (size_of_memory - starting_temp>0)
	allocated_table.push_back({ "allocated space","allocated space",starting_temp,size_of_memory - starting_temp,0 });

	follow_free_space();
	allocated_space_index();

	up_to_size();

}

void memory_mangement::follow_free_space()

{
	auto temp = free_table.begin();
	for (int i = 0; i < free_table.size() - 1; i++)
	{
		temp++;

		if (free_table[i].base_address + free_table[i].limit == free_table[i + 1].base_address)
		{
			free_table[i].limit = free_table[i].limit + free_table[i + 1].limit;
			delete_from_allocated_table(free_table[i + 1].base_address);
			free_table.erase(temp);

		}
		else if (free_table[i].base_address + free_table[i].limit > free_table[i + 1].base_address)
		{
			delete_from_allocated_table(free_table[i].base_address + free_table[i].limit);
			if ((free_table[i + 1].base_address + free_table[i + 1].limit) > (free_table[i].base_address + free_table[i].limit))
			{

				int num = free_table[i].base_address + free_table[i].limit - free_table[i + 1].base_address;
				free_table[i].limit = (free_table[i + 1].limit - num) + free_table[i].limit;
			}
			else
			{
				allocated_table.pop_back();
				int start_temp = free_table[i].base_address + free_table[i].limit;
				allocated_table.push_back({ "allocated space","allocated space",start_temp,size_of_memory - start_temp,0 });
			}
			free_table.erase(temp);

		}


	}
}

void memory_mangement::delete_from_allocated_table(int element)

{
	auto temp = allocated_table.begin();
	for (auto it : allocated_table)
	{
		if (it.base_address == element)
		{
			allocated_table.erase(temp);
			return;
		}
		temp++;
	}
}

void memory_mangement::create_new_process_first_fit(vector<segment> new_process)

{

	vector <free_item_information>free_table_temp(free_table);
	vector <allocated_item_information>allocated_table_temp(allocated_table);

	process_index++;
	int exit = 0;
	auto temp = free_table_temp.begin();
	for (auto i : new_process)

	{
		temp = free_table_temp.begin();

        if (free_table_temp.empty())
        {
        waiting_queue.push_back(new_process);
        type_of_process_in_waiting_queue.push_back("first fit");
        process_index--;
        return;
        }

		for (auto j : free_table_temp)
		{
			if (i.limit == j.limit)// i = process  //j=free table
			{
				allocated_table_temp.push_back({ i.process_name,i.segment_name,j.base_address,i.limit,process_index });
				free_table_temp.erase(temp);
				free_table_temp = arrange_free_table_according_to_starting_address(free_table_temp);
				exit = 0;
				break;
			}
			else if (i.limit < j.limit)
			{
				allocated_table_temp.push_back({ i.process_name,i.segment_name,j.base_address,i.limit,process_index });

				free_table_temp.erase(temp);
				free_table_temp.push_back({ j.base_address + i.limit,j.limit - i.limit });
				free_table_temp = arrange_free_table_according_to_starting_address(free_table_temp);
				exit = 0;
				break;
			}
			else
			{
				exit = 1;

			}

			temp++;
		}
		if (exit == 1)
		{
			type_of_process_in_waiting_queue.push_back("first fit");;
			waiting_queue.push_back(new_process);
			return;
		}
	}

	free_table = free_table_temp;
	allocated_table = allocated_table_temp;
	arrange_allocated_table();
	free_table = arrange_free_table_according_to_starting_address(free_table);
	/////////////////////
	vector<allocated_item_information > process_informatin_temp;
	for (auto temp : allocated_table)
	{
		if (temp.process_index == process_index)
			process_informatin_temp.push_back(temp);
	}
	process_information.push_back(process_informatin_temp);
}

void memory_mangement::create_new_process_best_fit(vector<segment> new_process)

{

	vector <free_item_information>free_table_temp(free_table);
	vector <allocated_item_information>allocated_table_temp(allocated_table);

	process_index++;
	int exit = 0;

	auto temp = free_table_temp.begin();
	free_table_temp = arrange_free_table_according_to_limit_size(free_table_temp);
	for (auto i : new_process)

	{
        temp = free_table_temp.begin();

        if (free_table_temp.empty())
        {
        waiting_queue.push_back(new_process);
        type_of_process_in_waiting_queue.push_back("best fit");
        process_index--;
        return;
        }

		for (auto j : free_table_temp)
		{
			if (i.limit == j.limit)// i = process  //j=free table
			{
				allocated_table_temp.push_back({ i.process_name,i.segment_name,j.base_address,i.limit,process_index });
				free_table_temp.erase(temp);
				free_table_temp = arrange_free_table_according_to_limit_size(free_table_temp);
				exit = 0;
				break;
			}
			else if (i.limit < j.limit)
			{
				allocated_table_temp.push_back({ i.process_name,i.segment_name,j.base_address,i.limit,process_index });

				free_table_temp.erase(temp);
				free_table_temp.push_back({ j.base_address + i.limit,j.limit - i.limit });
				free_table_temp = arrange_free_table_according_to_limit_size(free_table_temp);
				exit = 0;
				break;
			}
			else
			{

				exit = 1;
			}

			temp++;
		}
		if (exit == 1)
		{
			waiting_queue.push_back(new_process);
			type_of_process_in_waiting_queue.push_back("best fit");
			return;
		}
	}
	free_table = free_table_temp;
	allocated_table = allocated_table_temp;
	arrange_allocated_table();
	free_table = arrange_free_table_according_to_starting_address(free_table);
	/////////////////////////////////
	vector<allocated_item_information > process_informatin_temp;
	for (auto temp : allocated_table)
	{
		if (temp.process_index == process_index)
			process_informatin_temp.push_back(temp);
	}
	process_information.push_back(process_informatin_temp);

}

void memory_mangement::arrange_allocated_table()

{
	for (int i = 0; i < allocated_table.size(); i++)
	{
		for (int j = 0; j < allocated_table.size() - 1; j++)
		{
			if (allocated_table[j].base_address > allocated_table[j + 1].base_address)
			{
				swap(allocated_table[j], allocated_table[j + 1]);
			}
		}
	}

}

void memory_mangement::up_to_size()
{
	if (free_table.back().base_address > size_of_memory)
	{
		free_table.pop_back();
		//allocated_table.pop_back();
		allocated_table.back().limit = size_of_memory - allocated_table.back().base_address;

	}
	else if (free_table.back().base_address + free_table.back().limit > size_of_memory)
	{
		free_item_information temp = { free_table.back().base_address ,size_of_memory - free_table.back().base_address };
		free_table.pop_back();
		//allocated_table.pop_back();
		free_table.push_back(temp);
		allocated_table.back().limit = temp.base_address - allocated_table.back().base_address;

	}

	delete_limit__lower_than_zero_from_allocated_table();

}





vector<free_item_information> memory_mangement::arrange_free_table_according_to_starting_address(vector<free_item_information> temp)

{
	for (int i = 0; i < temp.size(); i++)
	{
		for (int j = 0; j < temp.size() - 1; j++)
		{
			if (temp[j].base_address > temp[j + 1].base_address)
			{
				swap(temp[j], temp[j + 1]);
			}
		}
	}
	return temp;
}

vector<free_item_information> memory_mangement::arrange_free_table_according_to_limit_size(vector<free_item_information> temp)

{
	for (int i = 0; i < temp.size(); i++)
	{
		for (int j = 0; j < temp.size() - 1; j++)
		{
			if (temp[j].limit > temp[j + 1].limit)
			{
				swap(temp[j], temp[j + 1]);
			}
		}
	}
	return temp;
}

void memory_mangement::deallocated(string deallocated_process_name)

{
    for (auto it = allocated_table.begin();it!= allocated_table.end();)
        {
            if (it->process_name == deallocated_process_name)
            {
                free_table.push_back({ it->base_address,it->limit });
                it = allocated_table.erase(it);
            }
            else
            {
                it++;
            }
        }
        free_table = arrange_free_table_according_to_starting_address(free_table);
        follow_free_space();
        mangement_waiting_queue();
        ////////////////////////
        ////////////////////////
        auto temporary = process_information.begin();
        for (auto k : process_information)
        {
            for (auto r : k)
            {
                if (r.process_name == deallocated_process_name)
                {
                    process_information.erase(temporary);
                    return;
                }

            }
            temporary++;
        }
}
void memory_mangement::mangement_waiting_queue()
{
    auto waiting_queue_temp = waiting_queue;
        auto type_of_process_in_waiting_queue_temp = type_of_process_in_waiting_queue;
        waiting_queue.clear();
        type_of_process_in_waiting_queue.clear();
        for (int k=0;k< waiting_queue_temp.size();k++)
        {
            if (type_of_process_in_waiting_queue_temp[k] == "first fit")
            {
                create_new_process_first_fit(waiting_queue_temp[k]);
            }
            else
            {
                create_new_process_best_fit(waiting_queue_temp[k]);
            }

        }
}
void memory_mangement::delete_limit__lower_than_zero_from_allocated_table()
{
	auto it = allocated_table.begin();
	for (int i = 0; i < allocated_table.size() - 1; i++)
	{
		if (allocated_table[i].limit <= 0)
		{

			it = allocated_table.erase(it);


		}

		it++;
	}


}

vector<free_item_information> memory_mangement::return_free_table()

{
	return free_table;
}

vector<allocated_item_information> memory_mangement::return_allocated_table()

{
	if (allocated_table.back().base_address >= size_of_memory)
		allocated_table.pop_back();
	return allocated_table;
}

vector<vector<segment>> memory_mangement::return_waiting_queue()

{
	return waiting_queue;
}

vector<vector<allocated_item_information>> memory_mangement::return_process_information()

{
	return process_information;
}

vector<allocated_item_information> memory_mangement::return_memory_section()
{
    memory_section.clear();
	auto pointer_to_allocoated_table = allocated_table.begin();
	auto pointer_to_free_table=free_table.begin();
	while(pointer_to_free_table!=free_table.end()&& pointer_to_allocoated_table!=allocated_table.end())
	{
		if (pointer_to_allocoated_table->base_address < pointer_to_free_table->base_address)
		{
			memory_section.push_back(*pointer_to_allocoated_table);
			pointer_to_allocoated_table++;

		}
		else
		{
			memory_section.push_back({"free space","free space",pointer_to_free_table->base_address,pointer_to_free_table ->limit,0});
			pointer_to_free_table++;
		}
	}
	if (pointer_to_free_table == free_table.end()&& pointer_to_allocoated_table != allocated_table.end())
	{
		for (auto i = pointer_to_allocoated_table; i != allocated_table.end(); i++)
		{
			memory_section.push_back(*i);
		}
	}
	else if(pointer_to_allocoated_table == allocated_table.end()&& pointer_to_free_table != free_table.end())
	{
		for (auto i = pointer_to_free_table; i != free_table.end(); i++)
		{
			memory_section.push_back({ "free space","free space",i->base_address ,i->limit,0});
		}

	}
	return memory_section;
}
