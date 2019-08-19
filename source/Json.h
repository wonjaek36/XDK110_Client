
#ifndef JSON_H_
#define JSON_H_

//#define MAIN_FUNC

typedef struct Element_S
{
	char* name;
	char* value;
} Element_T;

char* createJson(char* json, int args, ...);

#define MAX_ELEMENT_LENGTH 100

#endif /* JSON_H_ */
