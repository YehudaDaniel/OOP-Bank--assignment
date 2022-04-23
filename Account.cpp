//NAME: Yehuda Daniel
//ID: 211789680

#define _CRT_SECURE_NO_WARNINGS
#include "Account.h"

//Default constructor, initializes the members
Account::Account(){
	m_transactionList = nullptr;
	m_numberOfTransaction = 0;
	m_persons = nullptr;
	m_totalPersons = 0;
	m_accountNumber = 0;
	m_balance = 0;
}

//Constructor: sets the members with the parameters provided
Account::Account(Person** persons, int count, double balance){
	m_balance = balance;
	SetPersons(persons, count);
	m_transactionList = nullptr;
	m_numberOfTransaction = 0;
	m_accountNumber = 0;
	for (int i = 0; i < m_totalPersons; i++) {
		m_accountNumber += m_persons[i]->GetId();
	}
}

Account::Account(const Person& person, double balance){
	m_totalPersons = 1;
	SetBalance(balance);
	m_transactionList = nullptr;
	m_numberOfTransaction = 0;
	m_persons = new Person * [1];
	m_persons[0] = new Person(person);
	m_accountNumber = 0;
	//using copy cons of Person to copy the person recieved by addres to the m_persons[0]
	for (int i = 0; i < m_totalPersons; i++) {
		m_accountNumber += m_persons[i]->GetId();
	}
}

//Copy constructor for the Account class
Account::Account(const Account& other){
	if(other.m_transactionList) SetTransactions(other.m_transactionList, other.m_numberOfTransaction);
	
	if(other.m_totalPersons) SetPersons(other.m_persons, other.m_totalPersons);
	m_accountNumber = other.m_accountNumber;
	m_balance = other.m_balance;
}


//The dTor for Account: deleting every allocation made
Account::~Account(){
	for (int i = 0; i < m_numberOfTransaction; i++) {
		delete[] m_transactionList[i]->GetDate();
	}
	clearPersons();
}

//Sets the array of persons from the array given
void Account::SetPersons(Person** persons, int count){
	m_totalPersons = count;
	m_persons = new Person * [count];

	for (int i = 0; i < m_totalPersons; i++) {
		m_persons[i] = new Person(*persons[i]);
	}
}


void Account::SetAccountNumber(int number){
	m_accountNumber = number;
}

void Account::SetBalance(double balance){
	m_balance = balance;
}

//Sets each of the transactions instances to the transactions array
void Account::SetTransactions(Transaction** newTransaction, int count){
	m_transactionList = new Transaction * [count];
	m_numberOfTransaction = count;

	for (int i = 0; i < m_numberOfTransaction; i++) {
		m_transactionList[i] = new Transaction(*newTransaction[i]);
	}
}

Transaction** Account::GetTransactions(){
	return m_transactionList;
}

int Account::GetNumOfTransactions(){
	return m_numberOfTransaction;
}

Person** Account::GetPersons() const{
	return m_persons;
}

int Account::GetTotalPersons() const{
	return m_totalPersons;
}

int Account::GetAccountNumber() const{
	return m_accountNumber;
}

double Account::GetBalance() const{
	return m_balance;
}

//Withdraws money, the money just gets substracted from the balance
void Account::Withdraw(double amount, const char* date){
	Transaction newTran(this, this, amount, date);
	AddTransaction(newTran);
	m_balance -= amount;
}

//Depositing money to the balance
void Account::Deposit(double amount, const char* date){
	Transaction newTran(this, this, amount, date);
	AddTransaction(newTran);
	m_balance += amount;
}

//Adds a person to the array, and adds the amount to the current balance
void Account::AddPerson(const Person& newPerson, double amount){
	//checking if the person already exist in the array, if it does, return
	for (int i = 0; i < m_totalPersons; i++) {
		if (newPerson.GetId() == m_persons[i]->GetId()) {
			return;
		}
	}

	//if the person doesnt exist already
	Person** temp = new Person * [m_totalPersons + 1];

	int i;
	for (i = 0; i < m_totalPersons; i++) {
		temp[i] = new Person(*m_persons[i]);
	}
	temp[i] = new Person(newPerson);
	clearPersons();
	m_totalPersons++;
	m_balance += amount;
	m_persons = temp;
}

void Account::DeletePerson(const Person& oldPerson){
	//checking if the person in the account
	bool existFlag = false;
	
	for (int i = 0; i < m_totalPersons; i++) {
		if (oldPerson.GetId() == m_persons[i]->GetId()) {
			existFlag = true;
			break;
		}
	}

	//if the person is not in the array, end the function
	if (!existFlag) return;

	//if the person exist in the account, delete him and realloc a new array
	Person** temp = new Person * [m_totalPersons - 1];
	
	for (int i = 0, j = 0; i < m_totalPersons; i++) {
		if (oldPerson.GetId() != m_persons[i]->GetId() ) { //if the current person is not the person to delete, add it to temp
			temp[j] = new Person(*m_persons[i]); //dereferencing m_persons[i]
			j++;
		}
	}

	clearPersons();
	m_totalPersons--;
	m_persons = temp;
}

void Account::AddTransaction(const Transaction& newTransaction){
	int i;
	Transaction** tmp = new Transaction * [newTransaction.GetSource()->GetNumOfTransactions() + 1];
	for (i = 0; i < newTransaction.GetSource()->GetNumOfTransactions(); i++) {
		tmp[i] = new Transaction(*newTransaction.GetSource()->GetTransactions()[i]);
	}
	tmp[i] = new Transaction(newTransaction);
	newTransaction.GetSource()->clearTransactions();
	newTransaction.GetSource()->SetTransactions(tmp, newTransaction.GetSource()->GetNumOfTransactions() + 1);
	for (i = 0; i < m_numberOfTransaction; i++) {
		m_transactionList[i] = new Transaction(*tmp[i]);
		delete tmp[i];
	}
	delete[] tmp;

	// If source and destination are different add -> Add the transaction to the destination account
	if (newTransaction.GetDes() != newTransaction.GetSource()) {
		tmp = new Transaction * [newTransaction.GetDes()->GetNumOfTransactions() + 1];
		for (i = 0; i < newTransaction.GetDes()->GetNumOfTransactions(); i++) {
			tmp[i] = new Transaction(*newTransaction.GetDes()->GetTransactions()[i]);
		}
		tmp[i] = new Transaction(newTransaction); // add the new transaction
		newTransaction.GetDes()->clearTransactions();
		newTransaction.GetDes()->SetTransactions(tmp, newTransaction.GetDes()->GetNumOfTransactions() + 1);
		for (i = 0; i < m_numberOfTransaction; i++) {
			m_transactionList[i] = new Transaction(*tmp[i]);
			delete tmp[i]; // delete tmp list
		}
		delete[] tmp;
	}
	newTransaction.GetSource()->SetBalance(newTransaction.GetSource()->GetBalance() - newTransaction.GetAmount());
	newTransaction.GetDes()->SetBalance(newTransaction.GetDes()->GetBalance() + newTransaction.GetAmount());
}

void Account::clearTransactions(){
	for (int i = 0; i < m_numberOfTransaction; i++) {
		delete m_transactionList[i]->GetDate();
	}
	delete[] m_transactionList;
}

void Account::clearPersons(){
	if (m_totalPersons > 0) {
		for (int i = 0; i < m_totalPersons; i++) {
			delete m_persons[i];
		}
		delete[] m_persons;
	}
}






