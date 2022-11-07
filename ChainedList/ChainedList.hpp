#ifndef CHAINEDLIST_HPP
#define CHAINEDLIST_HPP

//------- INCLUDE -------//
#include <assert.h>
#include <iostream>

//------- DEFINE -------//
#define USI unsigned short int

//------- CLASS -------//

/**
Template class to create chained list that are mainly use as dynamic table.
A Chained list is a list composed of cells. Each cell contain a value and a pointer to the next cell.
They are heavier to read than std::vector, but we will never need to copy all the list somewhere else in memory due to lake of place at the current location, because each cells are independent from others.
*/
template<class T>
class ChainedList
{
public:

	ChainedList() : m_firstCell(nullptr), m_size(0) {}
	~ChainedList() { Clear(); }

	T& operator[](USI _id)
	{
		return GetCell(_id)->value;
	}

	template<class T>
	friend std::ostream& operator<<(std::ostream& _os, const ChainedList<T>& _list);

	/**
	Add a value at the end of the list

	@param _value : Value to insert
	*/
	void Push(const T& _value)
	{
		Cell* newCell = new Cell;
		newCell->value = _value;

		if (m_firstCell)
		{
			GetCell(m_size - 1)->nextCell = newCell;
		}
		else
		{
			m_firstCell = newCell;
		}

		m_size++;
	}

	/**
	Insert a value in the list at the given ID

	@param _value : Value to insert
	@param _id : Insertion ID
	*/
	void Insert(const T& _value, USI _id)
	{
		Cell* newCell = new Cell;
		newCell->value = _value;

		if (_id > 0)
		{
			Cell* cell = GetCell(_id - 1);

			//////////////////////////////
			//       [newCell]
			// 
			// 
			// [_id-1] ------> [_id] 

			newCell->nextCell = cell->nextCell;

			//       [newCell]
			//                \
			//                 \
			// [_id-1] ------> [_id] 

			cell->nextCell = newCell;

			//       [newCell]
			//      /         \
			//     /           \
			// [_id-1]         [_id]
			//////////////////////////////
		}
		else
		{
			// Same logic as above
			newCell->nextCell = m_firstCell;
			m_firstCell = newCell;
		}

		m_size++;
	}

	/**
	Remove the last value of the list

	@return The value who was removed
	*/
	T Pop(void)
	{
		_ASSERT_EXPR(m_size != 0, L"Trying to delete a value in a empty chained list");

		if (m_size == 1)
		{
			T value = m_firstCell->value;
			delete m_firstCell;
			m_firstCell = nullptr;

			m_size--;
			return value;
		}
		else
		{
			Cell* cell = GetCell(m_size - 2);

			T value = cell->nextCell->value;
			delete cell->nextCell;
			cell->nextCell = nullptr;

			m_size--;
			return value;
		}
	}

	/**
	Remove a value in the list at the given ID

	@param _id : ID of the value to remove

	@return The value who was removed
	*/
	T Remove(USI _id)
	{
		_ASSERT_EXPR(m_size != 0, L"Trying to delete a value in a empty chained list");
		_ASSERT_EXPR(_id < m_size, L"Chained list index out of range");

		if (_id == 0)
		{
			Cell* cell = m_firstCell;
			m_firstCell = m_firstCell->nextCell;

			T value = cell->value;
			delete cell;
			m_size--;
			return value;
		}
		else
		{
			Cell* previousCell = GetCell(_id - 1);

			Cell* cell = previousCell->nextCell;
			if (cell->nextCell)
			{
				previousCell->nextCell = cell->nextCell;
			}

			T value = cell->value;
			delete cell;
			m_size--;
			return value;
		}
	}

	/**
	Destroy all the list and resize it to 0
	*/
	void Clear(void)
	{
		if (m_firstCell)
		{
			ClearCell(m_firstCell);
		}

		m_firstCell = nullptr;
		m_size = 0;
	}

	/**
	*@return List size
	*/
	inline USI GetSize(void) { return m_size; }

	/**
	 * @return True if the list is empty, false else
	 */
	inline bool IsEmpty(void) { return m_size == 0; }

private:

	struct Cell
	{
		T value;
		Cell* nextCell = nullptr;
	};

	Cell* m_firstCell;
	USI m_size;

	/**
	Recursive function to clear all cell from the given cell
	*/
	void ClearCell(Cell* _cell)
	{
		if (_cell->nextCell)
		{
			ClearCell(_cell->nextCell);
		}

		delete _cell;
		m_size--;
	}

	/**
	*@param _id : ID of the cell to get
	*
	*@return Pointer to the cell at [_id]
	*/
	Cell* GetCell(USI _id)
	{
		_ASSERT_EXPR(_id < m_size, L"Chained list index out of range");

		Cell* cell = m_firstCell;
		while (_id > 0)
		{
			cell = cell->nextCell;
			_id--;
		}

		return cell;
	}
};

/**
 * Allow you to print a chainedList with a std::cout.
 * Unknown behaviour with complex variable type without operator<< overcharge.
 * 
 * \param _os 
 * \param _list
 * \return 
 */
template<class T>
std::ostream& operator<<(std::ostream& _os, const ChainedList<T>& _list)
{
	_os << "[";
	if (_list.m_size > 0)
	{
		_os << _list.m_firstCell->value;

		const ChainedList<T>::Cell* cell = _list.m_firstCell->nextCell;
		while (cell)
		{
			_os << ", " << cell->value;
			cell = cell->nextCell;
		}
	}

	_os << "]";
	return _os;
}

#endif