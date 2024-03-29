#pragma once

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include "Admin.h"
#include "Customer.h"
#include "Favorite.h"
#include "Feedback.h"
#include "LinkedList.h"
#include "University.h"
#include "User.h"
#include "Util.h"
#include "Sort.h"
#include "Validation.h"
using namespace std;

// Admin class
class Admin : public User {
public:
	// Constructor
	Admin() : User() {}

	Admin(int UID, string username, string email, string password, string phoneNo)
		: User(UID, username, email, password, phoneNo) {}

	/*
		Return specified column value
		* Note: Used in sort and search function.

		@param index - column index
		@return T - column value
	*/
	template <typename T>
	T getColumn(int index) {
		/*
		*	Column Index:
		*	0 - UID
		*	1 - Username
		*	2 - Email
		*/
		switch (index) {
		case 0:
			return getUID();
		case 1:
			return getUsername();
		case 2:
			return getEmail();
		}
	}

	/*
		Login

		@param list - admin list
		@return Admin* - pointer to the admin object if login successful, nullptr otherwise
	*/
	Admin* login(LinkedList<Admin>* list) {
		Util::printHeader("Admin Login");

		// Initialise temporary variables
		string tmpUsername, tmpPass;

		// Get username and password
		cout << endl << "Username: ";
		getline(cin, tmpUsername);
		cout << "Password: ";
		tmpPass = Util::getPassword();

		// Check if username and password match
		return list->lookUpProfile(tmpUsername, tmpPass);
	}

	/*
		Add new university to list

		@param list - university list
	*/
	void addUniversity(LinkedList<University>* list) {
		string tmpName, tmpLocationCode, tmpLocation, input;
		float data[18] = { 0 };
		string dataLabels[18] = { "Rank", "AR Score", "AR Rank", "ER Score", "ER Rank", "FSR Score", "FSR Rank",
								"Cpf Score", "Cpf Rank", "Ifr Score", "Ifr Rank", "ISr Score", "ISr Rank",
								"Irn Score", "Irn Rank", "Ger Score", "Ger Rank", "Score Scaled" };

		while (true) {
			Util::printHeader("Add University");

			cout << "* Enter -1 to exit" << endl << endl;
			cout << "Enter University Name: ";
			getline(cin, tmpName);

			if (tmpName == "-1") {
				return;
			}
			else if (Validation::isEmpty(tmpName) || !Validation::isString(tmpName)) {
				cout << "Error: University name should only contain alphabetic characters and cannot be empty.";
				Util::sleep(1);
				return;
			}
			else if (list->head->data.isUniExist(list, tmpName)) {
				cout << "Error: University name already exists.";
				Util::sleep(1);
				return;
			}

			cout << "Enter Location Code: ";
			getline(cin, tmpLocationCode);
			if (tmpLocationCode == "-1") {
				return;
			}
			else if (Validation::isEmpty(tmpLocationCode) || !Validation::isString(tmpLocationCode)) {
				cout << "Error: Location code should only contain alphabetic characters and cannot be empty. " << endl;
				Util::sleep(1);
				return;
			}

			cout << "Enter Location: ";
			getline(cin, tmpLocation);
			if (tmpLocation == "-1") {
				return;
			}
			else if (Validation::isEmpty(tmpLocation) || !Validation::isString(tmpLocation)) {
				cout << "Error: Location code should only contain alphabetic characters and cannot be empty." << endl;
				Util::sleep(1);
				return;
			}

			data[0] = -1;
			for (int i = 1; i < 18; i++) {
				cout << "Enter " << dataLabels[i] << ": ";
				getline(cin, input);

				if (input == "-1") {
					return;
				}
				else if (Validation::isEmpty(input)) {
					cout << "Error: " << dataLabels[i] << " cannot be empty.";
					Util::sleep(1);
					return;
				}
				else if (!Validation::isNumber(input)) {
					cout << "Error: Invalid input for " << dataLabels[i] << ". Expected a number.";
					Util::sleep(1);
					return;
				}
				else if (!Validation::isNumberInRange(stoi(input), 0, 1000)) {
					cout << "Error: Invalid input for " << dataLabels[i] << endl;
					Util::sleep(1);
					return;
				}
				else if (i % 2 != 0 || i == 0) {
					if (stoi(input) < 0 || stoi(input) > 100) {
						cout << "Error: Invalid input for " << dataLabels[i] << ". Expected a number between 0 and 100.";
						Util::sleep(1);
						return;
					}
				}

				data[i] = stoi(input);
			}
			break;
		}

		// Add University to List
		list->insertToEndList(new University(tmpName, tmpLocationCode, tmpLocation, data));

		cout << "University added successfully!" << endl;
		Util::sleep(1);
	}

	/*
		Change INACTIVE account to FREEZE account (AKA Soft Delete Inactive Account)

		@param custList: customer list
	*/
	void changeInactiveToFreeze(LinkedList<Customer>* custList) {
		Util::printHeader("Inactive User Platform");

		node<Customer>* tmp = custList->head;
		int counter = 0;

		// Display inactive accounts

		while (tmp != nullptr) {
			if (tmp->data.getAccountStatus() == tmp->data.accountStatusToString(AccountStatus::INACTIVE)) {
				if (counter == 0) {
					cout << "\t" << "Customer ID" << "\t" << "Customer Name" << endl;
				}
				cout << "[" << counter + 1 << "] " << "\t   " << tmp->data.getUID() << "\t\t" << tmp->data.getUsername() << endl;
				counter++;
			}
			tmp = tmp->nextAddress;
		}

		if (counter == 0) {
			cout << endl << "No Inactive Account" << endl;
			Util::sleep(1);
			return;
		}

		cout << endl << endl << "Total Inactive Accounts: " << counter << endl << endl;

		string choice;
		cout << "Enter your choice:" << endl;
		cout << "[1] Select and Delete an INACTIVE account" << endl;
		cout << "[2] Delete all INACTIVE account" << endl;
		cout << "[3] Back" << endl << endl;
		cout << "Option: ";
		getline(cin, choice);
		cout << endl;

		// Change status based on user choice
		tmp = custList->head;
		int tmpCounter = 0;
		string customerId = "-1";
		bool isAllDelete = false;

		if (choice == "1") {
			cout << endl << "Enter customer ID to Delete: ";
			getline(cin, customerId);
		}
		else if (choice == "2") {
			isAllDelete = true;
		}
		else if (choice == "3") {
			return;
		}
		else {
			cout << "Invalid Option." << endl;
			Util::sleep(1);
			return;
		}

		while (tmp != nullptr) {
			if (tmp->data.getUID() == stoi(customerId)) {
				if (tmp->data.getAccountStatus() == tmp->data.accountStatusToString(AccountStatus::INACTIVE)) {
					tmp->data.setAccountStatus(AccountStatus::FREEZE);

					cout << "Account status changed to FREEZE for customer ID " << customerId << endl;
					Util::sleep(1);
					return;
				}
				else {
					cout << "Error: Account is not INACTIVE" << endl;
					Util::sleep(1);
					return;
				}
			}
			else if (tmp->data.getAccountStatus() == tmp->data.accountStatusToString(AccountStatus::INACTIVE)) {
				if (isAllDelete) {
					tmp->data.setAccountStatus(AccountStatus::FREEZE);
					cout << "Account status changed to FREEZE for customer ID " << tmp->data.getUID() << endl;
				}
			}
			tmp = tmp->nextAddress;
		}
		cout << endl;
		Util::pause();
	}

	/*
		Delete all existing freeze function.
		* Note: Not implement in system yet.

		@param custList: customer list
	*/
	void deleteFreezeAccount(LinkedList<Customer>* custList) {
		node<Customer>* tmp = custList->head;
		node<Customer>* prev = nullptr;

		while (tmp != nullptr) {
			if (tmp->data.getAccountStatus() == tmp->data.accountStatusToString(AccountStatus::FREEZE)) {
				// Delete the node
				if (prev == nullptr) {
					// Deleting the head node
					custList->head = tmp->nextAddress;
				}
				else {
					prev->nextAddress = tmp->nextAddress;
				}
				node<Customer>* toDelete = tmp;
				tmp = tmp->nextAddress;
				delete toDelete;
			}
			else {
				prev = tmp;
				tmp = tmp->nextAddress;
			}
		}
		delete tmp;
		delete prev;
	}

	/*
		Display Registered User Detail

		@param cusList: customer list
	*/
	void displayAndModifyUser(LinkedList<Customer>* cusList) {
		Util::printHeader("Registered User Detail");

		//If the list is empty
		if (cusList->head == nullptr) {
			cout << "No user registered yet." << endl;
			return;
		}

		//Display the list
		cout << "UID\tUsername\tEmail\t\t\tAccount Status" << endl;

		node<Customer>* current = cusList->head;
		while (current != nullptr) {
			Customer tmp = current->data;

			cout << tmp.getUID() << "\t" << tmp.getUsername() << "\t\t" << tmp.getEmail() << "       \t" << tmp.getAccountStatus() << endl
				<< "-----------------------------------------------------------------" << endl;
			current = current->nextAddress;
		}

		// Ask for user selection
		string index;
		try {
			cout << endl << "* Enter -1 to exit *" << endl << endl;
			cout << "Enter the UID you want to modify: ";
			getline(cin, index);

			if (index == "-1") return;
			else if (stoi(index) < 1 || stoi(index) > cusList->size) {
				cout << "Invalid option." << endl;
				Util::sleepClean(1);
				return;
			}
			else if (stoi(index) == -1) {
				return;
			}
		}
		catch (exception) {
			cout << "Invalid option." << endl;
			Util::sleepClean(1);
			return;
		}

		Util::cleanScreen();

		// Get the user from the linked list based on UID
		Customer* selectedUser = NULL;
		node<Customer>* editNode = cusList->head;

		while (editNode != nullptr) {
			if (editNode->data.getUID() == stoi(index)) {
				selectedUser = &(editNode->data);
				break;
			}
			editNode = editNode->nextAddress;
		}

		if (selectedUser != NULL) {
			// Edit the profile of the selected user
			if (selectedUser->editProfile(cusList)) {
				Util::printHeader("User Profile");
				selectedUser->display();
				cout << endl;
				Util::pause();
			}
		}
		else {
			cout << "User with UID " << index << " not found." << endl;
			Util::sleepClean(1);
		}
	}

	/*
		Delete any user account

		@param currentAdmin: Pointer to current admin node
		@param cusList: customer list
	*/
	void deleteUserAccount(Admin* currentAdmin, LinkedList<Customer>* cusList) {
		// If the list is empty
		if (cusList->head == nullptr) {
			cout << "No user registered yet." << endl;
			return;
		}

		// Display the list
		currentAdmin->displayAndModifyUser(cusList);

		// Ask for the UID to delete
		int UID;
		cout << "Enter the UID to delete: ";
		cin >> UID;

		// Check if the UID is valid
		while (UID <= 0 || UID > cusList->size) {
			cout << "Error: Invalid UID. Please re-enter: ";
			cin >> UID;
		}

		// Delete the user
		if (UID == 1) {
			cusList->deleteFromFrontList();
		}
		else if (UID == cusList->size) {
			cusList->deleteFromEndList();
		}
		else {
			cusList->deleteFromSpecificLocation(UID - 1);
		}

		cout << "User deleted successfully!" << endl;
	}

	// Return string of data for csv export
	string toDataString() {
		return to_string(this->getUID()) + "," + this->getUsername() + "," + this->getEmail() + ","
			+ this->getPassword() + "," + this->getPhoneNo();
	}

	/*
		Eliminate duplicate code when generating report

		@param filename: name of the file to export
		@param header: header of the file
		@param dataList: data list to export
		@param custList: customer list
		@param uniList: university list
	*/
	template <class T>
	void exportData(string filename, string header, LinkedList<T>* dataList, LinkedList<Customer>* custList, LinkedList<University>* uniList) {
		ofstream outputFile(filename);

		if (outputFile.is_open()) {
			outputFile << header << '\n';

			node<T>* current = dataList->head;
			while (current != nullptr) {
				string dataString = current->data.toDataString();
				outputFile << dataString << '\n';
				current = current->nextAddress;
			}

			outputFile.close();
		}
		else {
			cout << "Error opening the output file: " << filename << endl;
		}
	}

	/*
		Generate CSV reports upon system close. Include university, customer, feedback, and favorite.

		@param uniList: Universities list
		@param custList: Customers list
		@param adminList: Admins list
		@param feedbackList: Feedbacks list
		@param favList: Favorites list
	*/
	void generateReport(LinkedList<University>* uniList, LinkedList<Customer>* custList, LinkedList<Admin>* adminList, LinkedList<Feedback>* feedbackList, LinkedList<Favorite>* favList) {
		// Open the input CSV file
		string uniFileName = "output/university.csv";
		string uniHeader = "Rank,Institution,Location Code,Location,AR Score,AR Rank,ER Score,ER Rank,FSR Score,FSR Rank,CPF Score,CPF Rank,IFR Score,IFR Rank,ISR Score,ISR Rank,IRN Score,IRN Rank,GER Score,GER Rank,Score Scaled";

		string custFileName = "output/customer.csv";
		string custHeader = "UID,Username,Email,Password,Phone No.,Postcode,Address,State,Country,Last Login,Account Status";

		string adminFileName = "output/admin.csv";
		string adminHeader = "UID, Username, Email, Password, Phone No.";

		string feedbackFileName = "output/feedback.csv";
		string feedbackHeader = "ID,Name,Role,Title,Comment,Reply ID,Status,Date";

		string favFileName = "output/favorite.csv";
		string favHeader = "ID,Username,Institution Name";

		exportData(uniFileName, uniHeader, uniList, custList, uniList);
		exportData(custFileName, custHeader, custList, custList, uniList);
		exportData(adminFileName, adminHeader, adminList, custList, uniList);
		exportData(feedbackFileName, feedbackHeader, feedbackList, custList, uniList);
		exportData(favFileName, favHeader, favList, custList, uniList);
	}

	/*
		Generate summary report for top 10 preferred universities

		@param favoritesList - favorites list
	*/
	void summarizeTop10Preferred(LinkedList<Favorite>* favoritesList) {
		Util::printHeader("Top 10 Preferred Universities");

		const int MAX_UNIVERSITIES = 10;
		int universityIDs[MAX_UNIVERSITIES] = { 0 };
		int universityFrequencies[MAX_UNIVERSITIES] = { 0 };

		// Count the frequency of each university in the favorites list
		node<Favorite>* current = favoritesList->head;
		while (current != NULL) {
			int universityID = current->data.getInstitutionRank();

			for (int i = 0; i < MAX_UNIVERSITIES; i++) {
				if (universityIDs[i] == 0) {
					// Found an empty slot, add the university ID and set frequency to 1
					universityIDs[i] = universityID;
					universityFrequencies[i] = 1;
					break;
				}
				else if (universityIDs[i] == universityID) {
					// Found a matching university ID, increase the frequency
					universityFrequencies[i]++;
					break;
				}
			}
			current = current->nextAddress;
		}

		// Sort the universities based on their frequency in descending order
		for (int i = 0; i < MAX_UNIVERSITIES; i++) {
			for (int j = i + 1; j < MAX_UNIVERSITIES; j++) {
				if (universityFrequencies[j] > universityFrequencies[i]) {
					// Swap university IDs
					int tempID = universityIDs[i];
					universityIDs[i] = universityIDs[j];
					universityIDs[j] = tempID;

					// Swap frequencies
					int tempFrequency = universityFrequencies[i];
					universityFrequencies[i] = universityFrequencies[j];
					universityFrequencies[j] = tempFrequency;
				}
			}
		}

		// Print the top 10 preferred universities
		cout << "Top 10 Preferred Universities by Parents in Malaysia:" << endl << endl;
		for (int i = 0; i < MAX_UNIVERSITIES; i++) {
			if (universityIDs[i] == 0) {
				break;
			}
			node<Favorite>* currentUniversity = favoritesList->head; // New node pointer for iterating through the linked list
			while (currentUniversity != NULL) {
				int universityID = currentUniversity->data.getInstitutionRank();
				if (universityID == universityIDs[i]) {
					University* university = currentUniversity->data.getUniversity(universityID);
					if (university != NULL) {
						string universityName = university->getInstitution(); // Assuming the University class has a `getInstitution` method
						cout << "[" << i + 1 << "] " << "University Name: " << universityName << ", Frequency: " << universityFrequencies[i] << endl;
						break; // Exit the loop once the university is found
					}
				}
				currentUniversity = currentUniversity->nextAddress;
			}
		}
		cout << endl;
		Util::pause();
	}

	/*
		Display Feedback sort by latest date

		@param feedbackList - feedback list
	*/
	void displayFeedbackByDate(LinkedList<Feedback>* feedbackList) {
		while (true) {
			// Initialize variables
			node<Feedback>* current = feedbackList->tail;
			int ticketCounter = 1;

			Util::printHeader("Feedback");

			// Display ticket in brief info
			while (current != NULL) {
				// Ignore reply feedback
				if (current->data.getIsReply()) {
					current = current->prevAddress;
					continue;
				}

				if (ticketCounter == 1) {
					cout << "   No." << "\t\t" << "Created Date" << "\t" << "Status" << "\t\t\t" << "Title" << endl;
				}

				if (ticketCounter < 10) {
					if (current->data.getStatus() == "WAITING FOR CUSTOMER" || current->data.getStatus() == "WAITING FOR SUPPORT") {
						cout << "   [" << ticketCounter << "] " << "\t\t"
							<< current->data.getDate() << "\t"
							<< current->data.getStatus() << "\t"
							<< current->data.getTitle() << endl;
					}
					else {
						cout << "   [" << ticketCounter << "] " << "\t\t"
							<< current->data.getDate() << "\t"
							<< current->data.getStatus() << "\t\t\t"
							<< current->data.getTitle() << endl;
					}
				}

				else {
					if (current->data.getStatus() == "WAITING FOR CUSTOMER" || current->data.getStatus() == "WAITING FOR SUPPORT") {
						cout << "   [" << ticketCounter << "] " << "\t"
							<< current->data.getDate() << "\t"
							<< current->data.getStatus() << "\t"
							<< current->data.getTitle() << endl;
					}
					else {
						cout << "   [" << ticketCounter << "] " << "\t"
							<< current->data.getDate() << "\t"
							<< current->data.getStatus() << "\t\t\t"
							<< current->data.getTitle() << endl;
					}
				}

				ticketCounter++;
				current = current->prevAddress;
			}
			cout << endl;

			// Prompt if no ticket found
			if (ticketCounter == 1) {
				cout << "No ticket at the moment." << endl;
				Util::sleepClean(2);
				return;
			}
			else {
				cout << "Please select your action:" << endl;
				cout << "[1] Check Ticket Details" << endl;
				cout << "[2] Back" << endl << endl;
				cout << "Option: ";
			}

			string selection, indexSelection;
			getline(cin, selection);

			if (selection == "1") {
				cout << endl << "Please enter the index number you wish to view: ";
				getline(cin, indexSelection);

				try {
					int indexInt = stoi(indexSelection);
					if (indexInt > 0 && indexInt < ticketCounter) {
						current = feedbackList->tail;
						int counter = 0;

						while (current != NULL) {
							// Ignore reply feedback
							if (current->data.getIsReply()) {
								current = current->prevAddress;
								continue;
							}

							if (counter == indexInt - 1) {
								current->data.display(feedbackList, true);
								break;
							}
							counter++;
							current = current->prevAddress;
						}
					}
					else {
						cout << "Invalid option." << endl;
						Util::sleep(1);

						return;
					}
				}
				catch (exception) {
					cout << "Invalid option." << endl;
					Util::sleep(1);
				
					return;
				}
			}
			else if (selection == "2") {
				return;
			}
			else {
				cout << "Invalid option." << endl;
				Util::sleepClean(1);
				return;
			}
			
		}
	};

	/*
		Function to calculate the time complexity of search operation

		@param list - Searched List
		@param key - Key to be searched
		@param colIndex - Column index to be searched
		@param searchMethod - Search method
		@return Time complexity in microseconds
	*/
	double countTimeComplexitySearch(LinkedList<University>* list, string key, int colIndex, string searchMethod) {
		string** arr = list->convertTo2DArray();
		int tmpCount = 0;

		auto startTime = std::chrono::steady_clock::now();

		// Perform the search operation
		if (searchMethod == "linear") {
			linearSearch(list, key, colIndex);
		}
		else {
			binarySearch(arr, list->size, key, colIndex, &tmpCount);
		}

		auto endTime = chrono::steady_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

		Util::destroy2dArray(arr, list->size);

		return duration.count();
	}

	/*
		Function to calculate the time complexity of sort operation

		@param list - Unsorted List
		@param colIndex - Column index to be sorted
		@param sortMethod - Sort method
		@return Time complexity in microseconds
	*/
	double countTimeComplexitySort(LinkedList<University>* list, int colIndex, string sortMethod) {
		string** arr = list->convertTo2DArray();

		auto startTime = chrono::steady_clock::now();

		// Perform the sort operation
		if (sortMethod == "selection") {
			selectionSort(arr, list->size, colIndex);
		}
		else {
			quicksort(arr, 0, list->size - 1, colIndex, true);
		}

		auto endTime = chrono::steady_clock::now();
		auto duration = chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

		Util::destroy2dArray(arr, list->size);

		return duration.count();
	}

	/*
		Function to compare the time complexity of search and sort operations

		@param list - List to be searched and sorted
	*/
	void compareTimeComplexity(LinkedList<University>* list) {
		Util::printHeader("Compare Time Complexity");

		cout << "Search Algorithm" << "\t" << ": " << "Linear Search, Binary Search" << endl;
		cout << "Sort Algorithm " << "\t\t" << ": " << "Selection Sort, Quick Sort" << endl << endl;
		cout << "Please select the operation you wish to perform:" << endl;
		cout << "[1] Search Comparison" << endl;
		cout << "[2] Sort Comparison" << endl;
		cout << "Option: ";

		string selection;
		string method[2] = { };
		double time1 = 0.0, time2 = 0.0;
		getline(cin, selection);

		if (selection != "1" && selection != "2") {
			cout << "Invalid option." << endl;
			Util::sleepClean(1);
			return;
		}
		cout << "Performing Test..." << endl << endl;

		// Compare search operations
		if (selection == "1") {
			method[0] = "Linear Search";
			method[1] = "Binary Search";

			time1 += countTimeComplexitySearch(list, "UCL", 1, "linear");
			time1 += countTimeComplexitySearch(list, "University of Ghana", 1, "linear");
			time1 += countTimeComplexitySearch(list, "Universiti Malaya (UM)", 1, "linear");

			time2 += countTimeComplexitySearch(list, "UCL", 1, "binary");
			time2 += countTimeComplexitySearch(list, "University of Ghana", 1, "binary");
			time2 += countTimeComplexitySearch(list, "Universiti Malaya (UM)", 1, "binary");
			cout << "Test Case 1 Completed" << endl << endl;

			time1 += countTimeComplexitySearch(list, "University of Northern British Columbia", 1, "linear");
			time1 += countTimeComplexitySearch(list, "University of Lincoln", 1, "linear");
			time1 += countTimeComplexitySearch(list, "University of Messina (UniME)", 1, "linear");

			time2 += countTimeComplexitySearch(list, "University of Northern British Columbia", 1, "binary");
			time2 += countTimeComplexitySearch(list, "University of Lincoln", 1, "binary");
			time2 += countTimeComplexitySearch(list, "University of Messina (UniME)", 1, "binary");
			cout << "Test Case 2 Completed" << endl << endl;

			time1 += countTimeComplexitySearch(list, "King Fahd University of Petroleum & Minerals", 1, "linear");
			time1 += countTimeComplexitySearch(list, "Hanyang University", 1, "linear");
			time1 += countTimeComplexitySearch(list, "Xi'an Jiaotong Liverpool University", 1, "linear");

			time2 += countTimeComplexitySearch(list, "King Fahd University of Petroleum & Minerals", 1, "linear");
			time2 += countTimeComplexitySearch(list, "Hanyang University", 1, "linear");
			time2 += countTimeComplexitySearch(list, "Xi'an Jiaotong Liverpool University", 1, "linear");
			cout << "Test Case 3 Completed" << endl << endl;
		}
		// Compare sort operations
		else if (selection == "2") {
			method[0] = "Selection Sort";
			method[1] = "Quick Sort";

			time1 += countTimeComplexitySort(list, 1, "selection");
			time1 += countTimeComplexitySort(list, 2, "selection");
			time1 += countTimeComplexitySort(list, 3, "selection");

			time2 += countTimeComplexitySort(list, 1, "quicksort");
			time2 += countTimeComplexitySort(list, 2, "quicksort");
			time2 += countTimeComplexitySort(list, 3, "quicksort");
			cout << "Test Case 1 Completed" << endl << endl;

			time1 += countTimeComplexitySort(list, 4, "selection");
			time1 += countTimeComplexitySort(list, 5, "selection");
			time1 += countTimeComplexitySort(list, 6, "selection");

			time2 += countTimeComplexitySort(list, 4, "quicksort");
			time2 += countTimeComplexitySort(list, 5, "quicksort");
			time2 += countTimeComplexitySort(list, 6, "quicksort");
			cout << "Test Case 2 Completed" << endl << endl;

			time1 += countTimeComplexitySort(list, 7, "selection");
			time1 += countTimeComplexitySort(list, 8, "selection");
			time1 += countTimeComplexitySort(list, 9, "selection");

			time2 += countTimeComplexitySort(list, 7, "quicksort");
			time2 += countTimeComplexitySort(list, 8, "quicksort");
			time2 += countTimeComplexitySort(list, 9, "quicksort");
			cout << "Test Case 3 Completed" << endl << endl;
		}

		cout << "Average Time Complexity: " << endl
			<< method[0] << "\t" << ": " << round(time1 / 9) << " ms" << endl
			<< method[1] << "\t" << ": " << round(time2 / 9) << " ms" << endl << endl;

		cout << "Conclusion: ";
		if (time1 < time2) {
			cout << method[0] << " is faster than " << method[1] << endl;
		}
		else if (time1 > time2) {
			cout << method[1] << " is faster than " << method[0] << endl;
		}
		else {
			cout << method[0] << " and " << method[1] << " are equally fast" << endl;
		}
		Util::pause();
	}
};
