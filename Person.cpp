//NAME: Yehuda Daniel
//ID: 211789680

#define _CRT_SECURE_NO_WARNINGS
#include "Person.h"

//Default constructur: members are initialized with default values
Person::Person(){
	m_name = NULL;
	m_id = 0;
}

//parameters constructor
Person::Person(const char* name, int id){
	SetName(name);
	m_id = id;
}

//Copy constructor: copies the content of one instance to a new instance
Person::Person(const Person& other){
	SetName(other.m_name);
	m_id = other.m_id;
}

//Distructor
Person::~Person(){
	delete[] m_name;
}

//Sets the name of the instance
void Person::SetName(const char* newName){
	int i;
	m_name = new char[strlen(newName) + 1];
	for (i = 0; i < strlen(newName); i++) {
		m_name[i] = newName[i];
	}
	m_name[i] = '\0';
}

//Sets the id of the instance
void Person::SetId(int newId){
	m_id = newId;
}

//Returns a pointer to the person's name
char* Person::GetName() const{
	return m_name;
}

//Returns the person's id
int Person::GetId() const{
	return m_id;
}
