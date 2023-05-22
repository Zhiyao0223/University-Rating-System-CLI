#pragma once

#include <iostream>
#include "Util.h"
using namespace std;

template <class T>
struct node {
	T data;
	node<T>* prevAddress;
	node<T>* nextAddress;
};


template <class T>
class LinkedList {
public:
	node<T>* head;
	node<T>* tail;
	int size;

	LinkedList<T>() {
		node<T>* newNode = NULL;
		head = tail = newNode;
	}


	node<T>* createNewNode(T* data) {
		node<T>* newNode = new node<T>;
		newNode->data = *data;
		newNode->nextAddress = newNode->prevAddress = NULL;
		return newNode;
	}

	/*
		Set to previous element according to index
		@param data - Class node
		@param counter - Number of element to move forward  
		@return New pointer location of the node
	*/
	node<T>* setToPreviousElement(node<T>* data, int counter) {
		while (counter > 0) {
			data = data->prevAddress;
			if (data == NULL) return NULL;
			counter--;
		}
		return data;
	}


	// Display info by pages
	void displayAll() {
		node<T>* tmp = head;
		Util util;
		T test;
		int counter = 0, option = 0;
		const int MAX_ITEM_PER_PAGE = 5;

		while (tmp != NULL) {
			if (counter < 5) {
				test = tmp->data;
				test.display();
				tmp = tmp->nextAddress;
				counter++;
			}
			else {
				cout << endl << "Wish to proceed?" << endl;
				cout << "1. Previous Page" << endl;
				cout << "2. Next Page" << endl;
				cout << "3. Return" << endl;
				cout << "Option: ";

				try {
					cin >> option;
				}
				catch (exception) {
					cerr << "Invalid option. Returning..." << endl;
					util.sleep(2);
					return;
				}

				// Set to previous five records
				if (option == 1) {
					node<T>* newPointer = setToPreviousElement(tmp, 2 * MAX_ITEM_PER_PAGE);

					if (newPointer == NULL) {
						cout << "No previous page available!" << endl;
						continue;
					}
					else tmp = newPointer;
				}
				// Set to next five records
				else if (option == 2) {
					// Do nothing
				}
				// Return to previous page
				else if (option == 3) {
					return;
				}
				// Also return to previous page if invalid numeric input
				else {
					cerr << "Invalid option. Returning..." << endl;
					util.sleep(2);
					return;
				}
				counter = 0;
				util.cleanScreen();
			}


		}
	}


	/*
		Insert class data to head of linked list
		@param data - Class data
	*/
	void insertToFrontList(T* data) {
		node<T>* newNode = createNewNode(data);

		if (head == NULL) {
			head = tail = newNode;
		}
		else {
			newNode->nextAddress = head;
			head->prevAddress = newNode;
			head = newNode;
		}
		size++;
	}

	
	/*
		Insert node to specific location in linked list
		@param index - Index location of new node
		@param data - node to be inserted
		@return Insertion status
	*/
	bool insertToSpecificLocation(int index, T* data) {
		if (index >= size) {
			cout << "Error: Invalid request for adding data to specific location." << endl;
			return false;
		}

		int counter = 0;
		node<T>* tmp = head;
		node<T>* newNode = createNewNode(data);

		while (tmp != NULL) {
			if (counter == index) {
				newNode = tmp->prevAddress;
				newNode->nextAddress = tmp;
				tmp->prevAddress = newNode;

				return true;
			}
			tmp = tmp->nextAddress;
			counter++;
		}
		return false;
	}


	/*
		Insert node to end of list
		@param data - Inserted data
	*/
	void insertToEndList(T* data) {
		node<T>* newNode = createNewNode(data);

		if (tail == NULL) {
			head = tail = newNode;
		}
		else {
			newNode->prevAddress = tail;
			tail->nextAddress = newNode;
			tail = newNode;
		}
		size++;
	}


	/*
		Delete node from head of linked list
		@return Deleted data in class format
	*/
	T deleteFromFrontList() {
		if (isEmpty) {
			cout << "No item in the list." << endl;
			return NULL;
		}

		node<T>* temp = head;
		T data = temp->data;

		head = head->nextAddress;
		delete temp;
		size--;

		return data;
		
	}


	/*
		Delete node from specific location of linked list
		@param index - Location of node to be deleted
		@return Deleted data in class format
	*/
	T deleteFromSpecificLocation(int index) {
		if (isEmpty) {
			cout << "No item in the list." << endl;
			return NULL;
		} else if (index >= size) {
			cout << "Error: Invalid request for adding data to specific location." << endl;
			return NULL;
		}

		int counter = 0;
		node<T>* current = head;
		T data = NULL;

		while (current != NULL) {
			if (counter == index) {
				data = current->data;
				
				node<T>* prev = current->prevAddress;
				prev->nextAddress = current->nextAddress;
				node<T>* next = current->nextAddress;
				next->prevAddress = prev;
				
				delete current;

				return data;
			}
			current = current->nextAddress;
			counter++;
		}
	}


	/*
		Delete node from end of linked list
		@return Deleted data in class format
	*/
	T deleteFromEndList() {
		if (isEmpty) {
			cout << "No item in the list." << endl;
			return NULL;
		}

		node<T>* temp = tail;
		T data = temp->data;

		tail = tail->prevAddress;
		delete temp;
		size--;

		return data;
	}


	/*
		Check if list is empty
		@return True if list is empty
	*/
	bool isEmpty() {
		return (size == 0);
	}




	//// Getter Function
	//node<T>* getFirstElement() {
	//	return front;
	//}

	//node<T>* getLastElement() {
	//	return rear;
	//}
};