#include "Json.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#define DEBUG 0
#define PADDING_SIZE 16
#define INITIAL_SIZE 32


/* For AppController */
#define NUMBER_OF_SENSOR_VAR 15
#define MAX_LENGTH_NAME 10
#define MAX_LENGTH_INT_32 13
#define MAX_LENGTH_FLOAT 13
#define MAX_LENGTH_DOUBLE 20

/*
void allocateMemoryOnSensors(char*** sensors) {

	(*sensors) = (char**)malloc(sizeof(char*)*(NUMBER_OF_SENSOR_VAR*2));
	int i = 0;
	for(;i<(NUMBER_OF_SENSOR_VAR-2)*2;) {
		*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_NAME);
		*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_INT_32+2);
	}
	*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_NAME);
	*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_FLOAT+2);
	*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_NAME);
	*(*sensors+i++) = (char*)malloc(sizeof(char)*MAX_LENGTH_DOUBLE+2);

}

void extractSensorValueAsString(char** sensors) {

	sprintf(sensors[0], "Accel_X");
	sprintf(sensors[1], "%ld", 100);
	sprintf(sensors[2], "Accel_Y");
	sprintf(sensors[3], "%ld", 100);
	sprintf(sensors[4], "Accel_Z");
	sprintf(sensors[5], "%ld", 100);

	sprintf(sensors[6], "Mag_X");
	sprintf(sensors[7], "%ld", 100);
	sprintf(sensors[8], "Mag_Y");
	sprintf(sensors[9], "%ld", 100);
	sprintf(sensors[10], "Mag_Z");
	sprintf(sensors[11], "%ld", 100);
	sprintf(sensors[12], "Mag_R");
	sprintf(sensors[13], "%ld", 100);

	sprintf(sensors[14], "Gyro_X");
	sprintf(sensors[15], "%ld", 100);
	sprintf(sensors[16], "Gyro_Y");
	sprintf(sensors[17], "%ld", 100);
	sprintf(sensors[18], "Gyro_Z");
	sprintf(sensors[19], "%ld", 100);

	sprintf(sensors[20], "RH");
	sprintf(sensors[21], "%lu", 100);
	sprintf(sensors[22], "Pressure");
	sprintf(sensors[23], "%lu", 100);
	sprintf(sensors[24], "Light");
	sprintf(sensors[25], "%lu", 100);

	sprintf(sensors[26], "Noise");
	sprintf(sensors[27], "%g", 100.0);
	sprintf(sensors[28], "Temp");
	sprintf(sensors[29], "%g", 100.0);
}

*/


void makeJsonArrayLagerToPutNewElement(char** json, int nextLen, int *size) {

	if ( nextLen < *size ) return;

	for(;*size<nextLen;(*size)+=PADDING_SIZE);
	char* newJson = (char*)malloc(sizeof(char)*(*size));
	memset(newJson, 0, sizeof(char)*(*size));

	strcpy(newJson, *json);

	free(*json);
	*json = newJson;
}

void insertNewElementToJson(char* json_element, int len, char** json) {

	if (DEBUG)
		printf("Json.c insertNewElementToJson: insert json_element into json\n");
	strcpy((*json) + len, json_element);
}

int createJson(char** json, char** sensors, int numOfSensors) {

	int size = INITIAL_SIZE;
	int len = 0;

	if (*json == NULL) {
		if (DEBUG) {
			printf("CreateJson: json is NULL, allocate memory as initial size %d\n", size);
		}
		(*json) = (char*)malloc(sizeof(char)*size);
		memset(*json, 0, sizeof(char)*size);
	}

	char* json_element = (char*)malloc(sizeof(char)*MAX_ELEMENT_LENGTH);
	memset(json_element,0, sizeof(char)*MAX_ELEMENT_LENGTH);

	if (DEBUG)
		printf("CreateJson: Initialization finished\n");

	(*json)[0] = '{';
	len = 1;
	for(int i=0;i<numOfSensors;i+=2) {

		if (i == 0)
			sprintf(json_element, "\n\t\"%s\": \"%s\"", sensors[i], sensors[i+1]);
		else
			sprintf(json_element, ",\n\t\"%s\": \"%s\"", sensors[i], sensors[i+1]);

		if (DEBUG) {
			printf("CreateJson: Create json_element as a pair");
		}
		makeJsonArrayLagerToPutNewElement(json, len+strlen(json_element), &size);
		insertNewElementToJson(json_element, len, json);

		len += strlen(json_element);
	}

	strcpy(*json+len, "\n}\n");
	len += 3;

	if (DEBUG)
		printf("%s\n", json);

	free(json_element);
	return len;
}

#ifdef MAIN_FUNC

int main() {

	char* json = NULL;
	char** sensors = NULL;

	allocateMemoryOnSensors(&sensors);
	extractSensorValueAsString(sensors);

	int length_json = createJson(&json, sensors, 30);
	printf("%s\n", json);
	printf("%d\n", strlen(json));
	printf("%d\n", length_json);

	printf("%d\n", sensors);
	printf("%d\n", json);
	if (sensors != NULL) {
		for(int i=0;i<NUMBER_OF_SENSOR_VAR*2;i++) {
			free(sensors[i]);
		}
		free(sensors);
		printf("Free sensors");
	}
	if (json != NULL) {
		free(json);
		printf("Free json");
	}



	return 0;
}

#endif


