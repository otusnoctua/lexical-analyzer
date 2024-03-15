#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define BUFSIZE 256 

FILE* fp;
char buf[BUFSIZE];
char c;
int d; //переменная для формирования числового значения константы;
int k;
int bufCurrent = 0;
int strCount = 1;
int colCount = 0;

int i = 0;
int j = 0;

char TW[][BUFSIZE] = { "", "if", "else", "for", "and", "or", "begin", "end", "not", "loop", "until", "do" };
char TD[][BUFSIZE] = { "", "+", "-", "/", "*", "<", ">", "!=", "=", ":=", "(", ")", ";", "EOF", "." };
char TID[BUFSIZE][BUFSIZE];
int TNUM[BUFSIZE];

int lex[4 * BUFSIZE][2]; //таблица лексем (номер_класса, номер_в_классе)
int lexCount = 0;
int TIDcount = 1; //размер таблицы
int TNUMcount = 1;


void add(void) { //считывание очередного символа из файла и занесение в буфер 
	buf[bufCurrent] = c;
	bufCurrent++;
	buf[bufCurrent] = '\0';
}

void clear(void) { //очистка буфера
	int i;
	for (i = 0; i < BUFSIZE; i++) {
		buf[i] = '\0';
	}
	bufCurrent = 0;
}

void gc() { //считывание символа и занесение в переменную c
	c = fgetc(fp);
	colCount++;
}

int look(char** T) { //TW/TD/TID
	int i = 1;
	int temp = 0;
	if (T == TW) {
		temp = 12;
	}
	if (T == TD) {
		temp = 15;
	}
	if (T == TID) {
		temp = TIDcount;
	}
	int j = 0;
	if (T == TW) {
		for (i = 1; i < temp; i++) {
			int lookCount = 0;
			if (strlen(buf) == strlen(TW[i])) //если длины определенных строк буфера и таблицы равны
			{
				for (j = 0; j < strlen(buf); j++)
				{
					if (buf[j] == TW[i][j]) //посимвольно сравниваем значения в буфере со значениями в таблице TW/TD
					{
						lookCount++;
					}
				}
				if (lookCount == strlen(buf)) { //идем до конца строки
					return i; //если нашли номер слова, возвращаем его номер в таблице
				}
			}
		}
	}
	if (T == TD) {
		for (i = 1; i < temp; i++) {
			int lookCount = 0;
			if (strlen(buf) == strlen(TD[i])) //если длины определенных строк буфера и таблицы равны
			{
				for (j = 0; j < strlen(buf); j++)
				{
					if (buf[j] == TD[i][j]) //посимвольно сравниваем значения в буфере со значениями в таблице TW/TD
					{
						lookCount++;
					}
				}
				if (lookCount == strlen(buf)) { //идем до конца строки
					return i;
				}
			}
		}
	}
	if (T == TID) {
		for (i = 1; i < temp; i++) {
			int lookCount = 0;
			if (strlen(buf) == strlen(TID[i])) //если длины определенных строк буфера и таблицы равны
			{
				for (j = 0; j < strlen(buf); j++)
				{
					if (buf[j] == TID[i][j]) //посимвольно сравниваем значения в буфере со значениями в таблице TW/TD
					{
						lookCount++;
					}
				}
				if (lookCount == strlen(buf)) { //идем до конца строки
					return i;
				}
			}
		}
	}
	return 0;
}

int makelex(int k, int i) {
	lex[lexCount][0] = k; //заносим в строку таблицы номер класса
	lex[lexCount][1] = i; //номер в классе
	printf("<%d, %d> ", lex[lexCount][0], lex[lexCount][1]);
	lexCount++; //увеличиваем счетчик лексем
}

int putl() {
	int lexString = look(TID); //проверка на отсутствие лексемы в таблице
	if (lexString == 0) { //лексемы нет в TID
		for (i = 0; i < strlen(buf); i++) {
			TID[TIDcount][i] = buf[i]; //записываем в TID лексему из buf
		}
		TIDcount++;
		return TIDcount - 1;
	}
}

int putnum() {
	TNUM[TNUMcount] = d;
	TNUMcount++;
	return TNUMcount - 1; //первое занятое место в таблице
}

void main(int argc, char** argv) {
	TID[0][0] = ""; //начинаем с 1 строки вместо 0
	enum state { H, ID, NUM, COM, eASS, dASS, DLM, ER, FIN };
	enum state TC;
	TC = H;
	fp = fopen(argv[1], "r");
	c = fgetc(fp);
	do
	{
		switch (TC)
		{
		case H:
			if (c == ' ' || c == '\n') {
				if (c == '\n') {
					printf("\n");
					strCount++;
					colCount = 0;
				}
				gc();
			}
			else {
				if (c == '(') {
					gc();
					if (c == '*') {
						TC = COM;
					}
					else
					{
						makelex(2, 10);
					}
				}
				else {
					if (c == '!') {
						TC = eASS;
					}
					else {
						if (c == ':')
							TC = dASS;
						else {
							if (c == EOF) {
								makelex(2, 13);
								printf("OK!\n\n");

								printf("1) TW:\n");
								for (i = 1; i < 12; i++)
									printf("%s\t%d\n", TW[i], i);

								printf("\n\n2) TD:\n");
								for (i = 1; i < 15; i++)
									printf("%s\t%d\n", TD[i], i);

								printf("\n\n3) TNUM:\n");
								for (i = 1; i < TNUMcount; i++) {
									printf("%d\t%d\n", TNUM[i], i);
								}

								printf("\n\n4) TID:\n");
								for (i = 1; i < TIDcount; i++) {
									printf("%s\t%d\n", TID[i], i);
								}

								TC = FIN;
							}
							else {
								if (isalpha(c)) {
									clear();
									add();
									gc();
									TC = ID;
								}
								else {
									if (isdigit(c)) {
										d = c - '0';
										gc();

										TC = NUM;
									}
									else {
										TC = DLM;
									}
								}
							}
						}
					}
				}
			}
			break;

		case ID:
			if (strlen(buf) > 254) {
				printf("The string is too long.\n");
				TC = ER;
				break;
			}
			if (isalpha(c) || isdigit(c))
			{
				add();
				gc();
			}
			else {
				if (look(TW) != 0) {
					makelex(1, look(TW));
					TC = H;
				}
				else {
					makelex(4, putl(TID));
					TC = H;
				}
			}
			break;

		case eASS:
			gc();
			if (c == '=') {
				gc();
				makelex(2, 7);
				TC = H;
			}
			else {
				printf("\nUnexpected symbol encountered (expected '=').\n");
				TC = ER;
				break;
			}
			break;

		case dASS:
			gc();
			if (c == '=') {
				gc();
				makelex(2, 9);
				TC = H;
			}
			else {
				printf("\nUnexpected symbol encountered (expected '=').\n");
				TC = ER;
				break;
			}
			break;

		case NUM:
			if (d > 65000) {
				printf("The number is too big.\n");
				TC = ER;
				break;
			}
			if (isdigit(c))
			{
				if (d == 0 && isdigit(c))
				{
					printf("\nLeading zero encountered.\n");
					TC = ER;
					break;
				}
				d = d * 10 + (c - '0');
				gc();

			}
			else {
				if (isalpha(c))
				{
					printf("Wrong ID\n");
					TC = ER;
				}
				else {
					makelex(3, putnum());
					TC = H;
				}
			}
			break;

		case COM:
			if (c == EOF)
			{
				printf("Unexpected end of the file.\n");
				TC = ER;
			}
			if (c == '\n') {
				printf("\n");
				colCount = 0;
				strCount++;
			}
			if (c != '*') {
				gc();
			}
			else {
				gc();
				if (c != ')') {
					gc();
				}
				else {
					gc();
					TC = H;
				}
			}
			break;

		case FIN:
			break;

		case ER:
		{
			printf("Error! (string %d, column %d)", strCount, colCount);
			break;
		}

		case DLM:
			clear();
			add();
			if (look(TD) != 0) {
				gc();
				makelex(2, look(TD));
				TC = H;
			}
			else
				TC = ER;
			break;
		}

	} while (TC != FIN && TC != ER);
	if (TC == ER) printf("Error! (string %d, column %d)", strCount, colCount);
	else printf("\nOK!");
}