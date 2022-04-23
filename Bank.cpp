//NAME: Yehuda Daniel
//ID: 211789680

#define _CRT_SECURE_NO_WARNINGS
#include "Bank.h"

//Default Ctor for the bank
Bank::Bank(){
	m_name = nullptr;
	m_account = nullptr;
	m_bankCode = 0;
	m_numbeOfAccounts = 0;
	m_totalBalance = 0;
}

//parameters Ctor for the bank with name and code
Bank::Bank(const char* name, int code){
	SetBankName(name);
	m_account = nullptr;
	SetCode(code);
	m_totalBalance = 0;
}

//Dtor
Bank::~Bank(){
	for (int i = 0; i < m_numbeOfAccounts; i++) {
		delete m_account[i];
	}
	delete[] m_account;
	delete[] m_name;
}

//------Setters-------------//
void Bank::SetBankName(const char* name){
	int i;
	m_name = new char[strlen(name) + 1];
	for (i = 0; i < strlen(name); i++) {
		m_name[i] = name[i];
	}
	m_name[i] = '\0';
}

//Dynamically assigning each account to the accounts array
void Bank::SetAccount(Account** account, int numbeOfAccounts){
	//first delete the current array
	if (m_numbeOfAccounts > 0) {
		for (int i = 0; i < m_numbeOfAccounts; i++) {
			delete m_account[i];
		}
		delete[] m_account;
	}

	m_numbeOfAccounts = numbeOfAccounts;
	m_account = new Account * [numbeOfAccounts];

	for (int i = 0; i < m_numbeOfAccounts; i++) {
		m_account[i] = new Account(*account[i]);
	}
}

void Bank::SetTotal(double total){
	m_totalBalance = total;
}

void Bank::SetCode(int code){
	m_bankCode = code;
}
//---------------------------//


//--------Getters-----------//
const char* Bank::GetBankName() const{
	return m_name;
}

Account** Bank::GetAccounts() const{
	return m_account;
}

int Bank::GetNumberOfAccounts() const{
	return m_numbeOfAccounts;
}

double Bank::GetTotal() const{
	return m_totalBalance;
}

int Bank::GetCode() const{
	return m_bankCode;
}

//--------------------------//



//-------Method Functions-----------//

void Bank::AddAccount(const Account& account){
	//checking if the account already exists in the array, if it does, return
	for (int i = 0; i < m_numbeOfAccounts; i++) {
		if (m_account[i]->GetAccountNumber() == account.GetAccountNumber()) return;
	}

	//if the account doesnt exist, proceed assigning it
	Account** temp = new Account * [m_numbeOfAccounts + 1];

	int i;
	for (i = 0; i < m_numbeOfAccounts; i++) {
		temp[i] = new Account(*m_account[i]);
		delete m_account[i];
	}
	temp[m_numbeOfAccounts] = new Account(account);
	delete[] m_account;
	m_account = temp;
	m_numbeOfAccounts++;
	m_totalBalance += account.GetBalance();
}

//assigning all account to temp array, and adding it a new account
void Bank::AddAccount(const Person& per, double amount){
	Account* temp = new Account(per, amount);
	AddAccount(*temp);
	delete temp;
}

//locates the account, adds to it the person, and updates the balance of the account
//we will check if the account exist, and if the person exists
void Bank::AddPerson(const Person& newPerson, const Account& account, double amount){
	int flag = 1;
	AddAccount(account);
	for (int i = 0; i < m_numbeOfAccounts; i++) {
		if (account.GetAccountNumber() == m_account[i]->GetAccountNumber()) {
			// Found the account
			for (int j = 0; j < m_account[i]->GetTotalPersons(); j++) {
				if (newPerson.GetId() == m_account[i]->GetPersons()[j]->GetId())
					flag = 0; // Person in the account
			}
			if (flag) {
				// Person not in the account -> ADD THE PERSON
				m_account[i]->AddPerson(newPerson, amount);
			}
		}
	}
}

void Bank::DeleteAccount(const Account& account){
	int flag = 0;
	for (int i = 0; i < m_numbeOfAccounts; i++) {
		if (m_account[i]->GetAccountNumber() == account.GetAccountNumber())
			flag = 1; // Account exist in the bank -> Delete the account
	}

	if (!flag) return; // Account isn't in the bank -> DO NOTHING

	// Delete the account from the bank
	Account** tmp = new Account * [m_numbeOfAccounts - 1];
	int i, j;
	for (i = 0, j = 0; i < m_numbeOfAccounts; i++) {
		if (m_account[i]->GetAccountNumber() != account.GetAccountNumber()) {
			tmp[j] = new Account(*m_account[i]);
			j++;
		}
	}
	delete[] m_account;
	m_account = tmp;
	tmp = nullptr;
	m_numbeOfAccounts--;
}

void Bank::DeletePerson(const Person& p){
	int i;
	for (i = 0; i < m_numbeOfAccounts; i++) {
		m_account[i]->DeletePerson(p);
		if (m_account[i]->GetTotalPersons() == 0) {
			DeleteAccount(*m_account[i]);
		}
	}
}
