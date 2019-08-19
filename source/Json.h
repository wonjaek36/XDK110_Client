#ifndef JSON_H_
#define JSON_H_

//#define MAIN_FUNC

typedef struct Element_S
{
	char* name;
	char* value;
} Element_T;

int createJson(char** json, char** sensors, int numOfSensors);

#define MAX_ELEMENT_LENGTH 50

#endif /* JSON_H_ */
