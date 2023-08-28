/*OTUS C-2023-07 Барашева Ангелина ДЗ к 4 занятию "Типы данных 2 Ч."
вход: путь файла, заданная кодировка, выходной файл
выход: выходной файл в кодировке UTF-8 или сообщение об ощибке, если есть
ошибки на этапе открытия, чтения или записи файлов
возврат: 0 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <locale.h>
#include <string.h>

#define CP_1251     0
#define KOI8_R      1
#define ISO_8859_5  2

uint16_t koi8r2unicode[128] = {
    0x2500, 0x2502, 0x250c, 0x2510, 0x2514, 0x2518, 0x251c, 0x2524, 0x252c, 0x2534, 0x253c, 0x2580, 0x2584, 0x2588, 0x258c, 0x2590,
    0x2591, 0x2592, 0x2593, 0x2320, 0x25a0, 0x2219, 0x221a, 0x2248, 0x2264, 0x2265, 0,      0x2321, 0x00b0, 0x00b2, 0x00b7, 0x00f7,
	0x2550, 0x2551, 0x2552, 0x0451, 0x2553, 0x2554, 0x2555, 0x2556, 0x2557, 0x2558, 0x2559, 0x255a, 0x255b, 0x255c, 0x255d, 0x255e,
	0x255f, 0x2560, 0x2561, 0x0401, 0x2562, 0x2563, 0x2564, 0x2565, 0x2566, 0x2567, 0x2568, 0x2569, 0x256a, 0x256b, 0x256c, 0x00a9,
	0x044e, 0x0430, 0x0431, 0x0446, 0x0434, 0x0435, 0x0444, 0x0433, 0x0445, 0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e,
	0x043f, 0x044f, 0x0440, 0x0441, 0x0442, 0x0443, 0x0436, 0x0432, 0x044c, 0x044b, 0x0437, 0x0448, 0x044d, 0x0449, 0x0447, 0x044a,
	0x042e, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413, 0x0425, 0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e,
	0x041f, 0x042f, 0x0420, 0x0421, 0x0422, 0x0423, 0x0416, 0x0412, 0x042c, 0x042b, 0x0417, 0x0428, 0x042d, 0x0429, 0x0427, 0x042a
};

uint8_t koi8[128] = {
    0x80,   0x81,   0x82,   0x83,   0x84,   0x85,   0x86,   0x87,   0x88,   0x89,   0x8a,   0x8b,   0x8c,   0x8d,   0x8e,   0x8f,
    0x90,   0x91,   0x92,   0x93,   0x94,   0x95,   0x96,   0x97,   0x98,   0x99,   0x9a,   0x9b,   0x9c,   0x9d,   0x9e,   0x9f,
    0xa0,   0xa1,   0xa2,   0xa3,   0xa4,   0xa5,   0xa6,   0xa7,   0xa8,   0xa9,   0xaa,   0xab,   0xac,   0xad,   0xae,   0xaf,
    0xb0,   0xb1,   0xb2,   0xb3,   0xb4,   0xb5,   0xb6,   0xb7,   0xb8,   0xb9,   0xba,   0xbb,   0xbc,   0xbd,   0xbe,   0xbf,
	0xc0,   0xc1,   0xc2,   0xc3,   0xc4,   0xc5,   0xc6,   0xc7,   0xc8,   0xc9,   0xca,   0xcb,   0xcc,   0xcd,   0xce,   0xcf,
	0xd0,   0xd1,   0xd2,   0xd3,   0xd4,   0xd5,   0xd6,   0xd7,   0xd8,   0xd9,   0xda,   0xdb,   0xdc,   0xdd,   0xde,   0xdf,
    0xe0,   0xe1,   0xe2,   0xe3,   0xe4,   0xe5,   0xe6,   0xe7,   0xe8,   0xe9,   0xea,   0xeb,   0xec,   0xed,   0xee,   0xef,
    0xf0,   0xf1,   0xf2,   0xf3,   0xf4,   0xf5,   0xf6,   0xf7,   0xf8,   0xf9,   0xfa,   0xfb,   0xfc,   0xfd,   0xfe,   0xff
};

uint16_t cp12512unicode[64] = {
	0x402,  0x403,  0x201a, 0x453,  0x201e, 0x2026, 0x2020, 0x2021, 0x20ac, 0x2030, 0x409,  0x2039, 0x40a,  0x40c,  0x40b,  0x40f,
	0x452,  0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014, 0,      0x2122, 0x459,  0x203a, 0x45a,  0x45c,  0x45b,  0x45f,
	0,      0x40e,  0x45e,  0x408,  0xa4,   0x490,  0xa6,   0xa7,   0x401,  0xa9,   0x404,  0xab,   0xac,   0,      0xae,   0x407,
	0xb0,   0xb1,   0x406,  0x456,  0x491,  0xb5,   0xb6,   0xb7,   0x451,  0x2116, 0x454,  0xbb,   0x458,  0x405,  0x455,  0x457
};

uint8_t cp1251[64] = {
    0x80,   0x81,   0x82,   0x83,   0x84,   0x85,   0x86,   0x87,   0x88,   0x89,   0x8a,   0x8b,   0x8c,   0x8d,   0x8e,   0x8f,
    0x90,   0x91,   0x92,   0x93,   0x94,   0x95,   0x96,   0x97,   0x98,   0x99,   0x9a,   0x9b,   0x9c,   0x9d,   0x9e,   0x9f,
    0xa0,   0xa1,   0xa2,   0xa3,   0xa4,   0xa5,   0xa6,   0xa7,   0xa8,   0xa9,   0xaa,   0xab,   0xac,   0xad,   0xae,   0xaf,
    0xb0,   0xb1,   0xb2,   0xb3,   0xb4,   0xb5,   0xb6,   0xb7,   0xb8,   0xb9,   0xba,   0xbb,   0xbc,   0xbd,   0xbe,   0xbf
};

/* Функция переводит строку из кодировки CP-1251 в UTF-8
вход: входной массив, выходной массив, размер
выход: выходной массив
возврат: 0*/
void CP_1251_to_UTF_8(uint8_t * str_in, off_t size, uint16_t * str_out) {
	int i, j;

	for(i = 0; i < size; i++) {
		if(str_in[i] < 0x80) {str_out[i] = (uint16_t)str_in[i];}	// ASCII
		else {
			if(str_in[i] <= 0xbf) {
				for(j = 0; j < 64; j++) {
					if(str_in[i] == cp1251[j]) {str_out[i] = cp12512unicode[j]; break;}
				}
			}
			else {str_out[i] = str_in[i] + 0x350;}	// между позициями букв
		}
	}
	//printf("Я функция CP_1251_to_UTF_8, я поработала");
}

/* Функция переводит строку из кодировки KOI8-R в UTF-8
вход: входной массив, выходной массив, размер
выход: выходной массив
возврат: 0*/
void KOI8_R_to_UTF_8(uint8_t * str_in, off_t size, uint16_t * str_out) {
	int i, j;

	for(i = 0; i < size; i++) {
		if(str_in[i] < 0x80) {str_out[i] = (uint16_t)str_in[i];}	// ASCII
		else {
			for(j = 0; j < 128; j++) {
					if(str_in[i] == koi8[j]) {str_out[i] = koi8r2unicode[j]; break;}
			}
		}
	}
	//printf("Я функция KOI8_R_to_UTF_8, я поработала");
}

/* Функция переводит строку из кодировки ISO-8859-5 в UTF-8
вход: входной массив, выходной массив, размер
выход: выходной массив
возврат: 0*/
void ISO_8859_5_to_UTF_8(uint8_t * str_in, off_t size, uint16_t * str_out) {
	int i, j;

	for(i = 0; i < size; i++) {
		if( (str_in[i] < 0xa1) || (str_in[i] == 0xad) ) {str_out[i] = (uint16_t)str_in[i];}	// ASCII
		else {str_out[i] = str_in[i] + 0x360;}	// между позициями букв
	}
	//printf("Я функция ISO_8859_5_to_UTF_8, я поработала");
}

/* Функция представляет собой конвертор из заданной кодировки (одна из CP-1251, KOI8-R, ISO-8859-5) в UTF-8*/
int convertor_to_UTF_8() {
	FILE * file_input;		// сюда будет считан входной файл
	FILE * file_output;		// сюда будет считан выходной файл
	char path[300];		// путь к входному, потом выходному файлу
	int encoding = 3;	// кодировка 0, 1 или 2
	uint8_t * string_in; 	// сюда запишется входной файл
	uint16_t * string_out;	// сюда запишется информация после конвертации входного файла
	struct stat file_in;

	setlocale(LC_ALL, "");

	printf("Введите путь к входному файлу до концвертации: \n");
	scanf("%s", path);	// считываем путь к файлу
	file_input = fopen(path, "r");	// открываем файл, который находится по адресу path
	if(file_input == NULL) {printf("Input file error"); return 0;}
	stat(path, &file_in);	// в экземпляр структуры file_in записываем данные о входном файле по введённому пути
	memset(path, 0, 300);

	printf("Введите путь к выходному файлу после конвертации:\n");
	scanf("%s", path);	// считываем путь к файлу
	file_output = fopen(path, "w");	// открываем файл, который находится по адресу path
	if(file_output == NULL) {printf("Output file error"); fclose(file_input); return 0;}

	printf("Введите кодировку x to UTF-8: 0 - CP-1251, 1 - KOI8-R, 2 - ISO-8859-5, another key - выход из программы:\n");
	scanf("%d", &encoding);	// считываем кодировку

	string_in = (uint8_t*)malloc(file_in.st_size*sizeof(uint8_t));
	if(!string_in) {
		printf("Out of memory. Press any key: ");
		fclose(file_input);
		fclose(file_output);
		exit(1);
	}

	if(fread(string_in, sizeof(uint8_t), file_in.st_size, file_input) != file_in.st_size) {
		printf("Read file error\n");
		fclose(file_input);
		fclose(file_output);
		free(string_in);
		exit(1);
	}

	string_out = (uint16_t*)calloc(file_in.st_size, sizeof(uint16_t));
	if(!string_out) {
		printf("Out of memory. Press any key: ");
		fclose(file_input);
		fclose(file_output);
		free(string_in);
		exit(1);

	}

	switch(encoding) {
		case CP_1251: {
			CP_1251_to_UTF_8(string_in, file_in.st_size, string_out);
			if(fwrite(string_out, sizeof(uint16_t), file_in.st_size, file_output) != file_in.st_size) {printf("File write error"); return -1;}
			break;
		}

		case KOI8_R: {
			KOI8_R_to_UTF_8(string_in, file_in.st_size, string_out);
			if(fwrite(string_out, sizeof(uint16_t), file_in.st_size, file_output) != file_in.st_size) {printf("File write error"); return -1;}
			break;
		}

		case ISO_8859_5: {
			ISO_8859_5_to_UTF_8(string_in, file_in.st_size, string_out);
			if(fwrite(string_out, sizeof(uint16_t), file_in.st_size, file_output) != file_in.st_size) {printf("File write error"); return -1;}
			break;
		}

		default:
			return 0;
	}
	free(string_in);
	free(string_out);
	fclose(file_input);
	fclose(file_output);
	return 0;
}

int main() {
	convertor_to_UTF_8();
	return 0;
}
