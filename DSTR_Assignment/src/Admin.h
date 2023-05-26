#pragma once

#include <iostream>
#include "User.h"
#include "Admin.h"
#include "Customer.h"
#include "Favorite.h"
#include "Feedback.h"
#include "LinkedList.h"
#include "University.h"
#include "Util.h"
#include <fstream>
#include <sstream>
#include <string>
#include "FileIO.h"

using namespace std;

// Admin class
class Admin : public User {
protected:

public:
    // Constructor
    Admin() : User() {}

    Admin(int UID, string username, string email, string password, string phoneNo)
        : User(UID, username, email, password, phoneNo) {
    }

    // Login
    Admin* login(LinkedList<Admin>* list) {
        string tmpUsername, tmpPass;

        cout << "Username: ";
        getline(cin, tmpUsername);
        cout << "Password: ";
        tmpPass = Util::getPassword();

        // Check if username and password match
        return list->lookUpProfile(tmpUsername, tmpPass);
    }

    //Generate Report
    void generateReport(LinkedList<University>* uniList, LinkedList<Customer>* custList, LinkedList<Feedback>* feedbackList, LinkedList<Favorite>* favList) {
        ofstream outputFile1("university.csv"); // Open the input CSV file

        outputFile1 << "Rank, Institution, Location Code, Location, AR Score, AR Rank, ER Score, ER Rank, FSR Score, FSR Rank, CPF Score, CPF Rank, IFR Score, IFR Rank, ISR Score, ISR Rank, IRN Score, IRN Rank, GER Score, GER Rank, Score Scaled\n";
        if (outputFile1.is_open()) {
            node<University>* current = uniList->head;
            while (current != nullptr) {
                University tmp = current->data;

                // Create a new sheet for attribute1
                outputFile1 << tmp.getRank() << ", "
                    << tmp.getInstitution() << ", "
                    << tmp.getLocationCode() << ", "
                    << tmp.getLocation() << ", "
                    << tmp.getArScore() << ", "
                    << tmp.getArRank() << ", "
                    << tmp.getErScore() << ", "
                    << tmp.getErRank() << ", "
                    << tmp.getFsrScore() << ", "
                    << tmp.getFsrRank() << ", "
                    << tmp.getCpfScore() << ", "
                    << tmp.getCpfRank() << ", "
                    << tmp.getIfrScore() << ", "
                    << tmp.getIfrRank() << ", "
                    << tmp.getIsrScore() << ", "
                    << tmp.getIsrRank() << ", "
                    << tmp.getIrnScore() << ", "
                    << tmp.getIrnRank() << ", "
                    << tmp.getGerScore() << ", "
                    << tmp.getGerRank() << ", "
                    << tmp.getScoreScaled() << "\n";

                current = current->nextAddress;
            }
            outputFile1.close(); // Close the output file
        }
        else {
            std::cout << "Error opening the output file for attribute: " << std::endl;
        }

        outputFile1.close(); // Close the input file

        ofstream outputFile2("customer.csv"); // Open the input CSV file

        outputFile2 << "Username, Email, PhoneNo, Postcode, Address, State, Country\n";
        if (outputFile2.is_open()) {
            node<Customer>* current = custList->head;
            while (current != nullptr) {
                Customer tmp = current->data;

                // Create a new sheet for attribute1
                outputFile2 << tmp.getUsername() << ", "
                    << tmp.getEmail() << ", "
                    << tmp.getPhoneNo() << ", "
                    << tmp.getPostcode() << ", "
                    << tmp.getAddress() << ", "
                    << tmp.getState() << ", "
                    << tmp.getCountry() << "\n";

                current = current->nextAddress;
            }
            outputFile2.close(); // Close the output file
        }
        else {
            std::cout << "Error opening the output file for attribute: " << std::endl;
        }

        outputFile2.close(); // Close the input file

        ofstream outputFile3("favorite.csv"); // Open the input CSV file

        outputFile3 << "ID, UID, insitution\n";
        if (outputFile3.is_open()) {
            node<Favorite>* current = favList->head;
            while (current != nullptr) {
                Favorite tmp = current->data;

                // Create a new sheet for attribute1
                outputFile3 << tmp.getID() << ", "
                    << tmp.getUID() << ", "
                    << tmp.getInstitutionRank() << "\n";


                current = current->nextAddress;
            }
            outputFile3.close(); // Close the output file
        }
        else {
            std::cout << "Error opening the output file for attribute: " << std::endl;
        }

        outputFile3.close(); // Close the input file

        ofstream outputFile4("feedback.csv"); // Open the input CSV file

        outputFile4 << "ID, UID, comment, reply, status\n";
        if (outputFile4.is_open()) {
            node<Feedback>* current = feedbackList->head;
            while (current != nullptr) {
                Feedback tmp = current->data;

                // Create a new sheet for attribute1
                outputFile4 << tmp.getID() << ", "
                    << tmp.getUID() << ", "
                    << tmp.getComment() << ", "
                    << tmp.getReply() << ", "
                    << tmp.getStatus() << "\n";


                current = current->nextAddress;
            }
            outputFile4.close(); // Close the output file
        }
        else {
            std::cout << "Error opening the output file for attribute: " << std::endl;
        }

        outputFile4.close(); // Close the input file

    }
};

void modifyUser(LinkedList<Customer>* editUser) {
   

    // Display the list of users
    cout << "Customer List:" << endl;
    cout << "---------------------------------------" << endl;
    node<Customer>* currentNode = editUser->head;
    while (currentNode != nullptr) {
        cout << "ID: " << currentNode->data.getUID() << endl;
        cout << "Username: " << currentNode->data.getUsername() << endl;
        cout << "Email: " << currentNode->data.getEmail() << endl;
        cout << "Phone: " << currentNode->data.getPhoneNo() << endl;
        cout << "Password: " << currentNode->data.getPassword() << endl;
        cout << "Postcode: " << currentNode->data.getPostcode() << endl;
        cout << "Address: " << currentNode->data.getAddress() << endl;
        cout << "State: " << currentNode->data.getState() << endl;
        cout << "Country: " << currentNode->data.getCountry() << endl;
        cout << "---------------------------------------" << endl;

        currentNode = currentNode->nextAddress;
    }

    // Ask for user selection
    string index;
    while (true) {
        try {
            cout << "Enter the index of the user you want to modify: ";
            getline(cin, index);
            stoi(index);
            break;
        }
        catch (exception) {
        }
    }

    Util::cleanScreen();

    // Get the user from the linked list based on UID
    Customer* selectedUser = nullptr;
    node<Customer>* editNode = editUser->head;
    while (editNode != nullptr) {
        if (editNode->data.getUID() == stoi(index)) {
            selectedUser = &(editNode->data);
            break;
        }
        editNode = editNode->nextAddress;
    }

    if (selectedUser) {
        // Edit the profile of the selected user
        if (selectedUser->editProfile()) {
            selectedUser->toString();
        }
    }
    else {
        cout << "User with UID " << index << " not found." << endl;
        Util::sleepClean(1);
        modifyUser(editUser);


    }

};
