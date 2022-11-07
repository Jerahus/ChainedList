//------- INCLUDE -------//

#include <iostream>
#include <string>
#include <vector>

#include "ChainedList.hpp"

#define PRINT(_value) std::cout << _value << std::endl

int main(void)
{
	ChainedList<int> list;
	PRINT(list);

	for (USI i = 0; i < 10; i++)
	{
		list.Push(i);
	}
	PRINT(list);

	list.Insert(10, 2);
	PRINT(list);

	PRINT("Remove return the value : " << list.Remove(7));
	PRINT(list);

	PRINT("Pop return the value : " << list.Pop());
	PRINT(list);

	PRINT("Remove return the value : " << list.Remove(0));
	PRINT(list);

	list.Insert(11, 0);
	PRINT(list);

	list.Clear();
	PRINT(list.IsEmpty());

	system("pause");
	return 1;
}