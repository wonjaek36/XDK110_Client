#include "Json.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#define DEBUG 1

void makeJsonArrayLagerToPutNewElement(char** json, int nextLen, int *size) {

	if ( nextLen < *size ) return;

	for(;*size<nextLen;(*size)*=2);
	char* newJson = (char*)malloc(sizeof(char)*(*size));
	memset(newJson, 0, sizeof(char)*(*size));

	strcpy(newJson, *json);

	free(*json);
	*json = newJson;
}

void insertNewElementToJson(char* json_element, int len, char* json) {

	if (DEBUG)
		printf("Json.c insertNewElementToJson: insert json_element into json\n");
	strcpy(json + len, json_element);
}

char* createJson(char* json, int args, ...) {

	va_list ap;
	va_start(ap, args);
	int size = 16;

	if (json == NULL) {
		char* json = (char*)malloc(sizeof(char)*size);
		memset(json, 0, sizeof(char)*size);
	}

	int len = 1;
	char* json_element = (char*)malloc(sizeof(char)*MAX_ELEMENT_LENGTH);
	memset(json_element,0, sizeof(char)*MAX_ELEMENT_LENGTH);

	//Start
	json[0] = '{';

	for(int i=0;i<args;i+=2) {

		char* name = va_arg(ap, char*);
		char* value = va_arg(ap, char*);

		if (i == 0)
			sprintf(json_element, "\n\t\"%s\": \"%s\"", name, value);
		else
			sprintf(json_element, ",\n\t\"%s\": \"%s\"", name, value);

		makeJsonArrayLagerToPutNewElement(&json, len+strlen(json_element), &size);
		insertNewElementToJson(json_element, len, json);

		len += strlen(json_element);
	}
	va_end(ap);

	strcpy(json+len, "\n}");
	if (DEBUG)
		printf("%s\n", json);

	free(json_element);
	return json;
	printf("\n");
}

#ifdef MAIN_FUNC

int main() {

	char* json;
	json = (char*)malloc(sizeof(char)*16);
	memset(json, 0, sizeof(char)*16);
	json = createJson(json, 4, "temp", "34.65", "Pressure", "123");
	free(json);

	return 0;
}

#endif


