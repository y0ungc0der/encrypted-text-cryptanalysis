#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <locale.h>
#include <math.h>
#include <io.h>

void manager(char *, long);
long getTextSize(char *);
char * readTextFromFile(char *);
void textFrequencyAnalise( char *, long);
void numberOfLettersInWord( char *,  long );
char * readTextFromFile(char *);
struct word * createWordInStruct(char *);
void displayTextRowByRow(char *);
void autoCorrect(float*, char *, long);
void writeToFile(char *, char*, long);
int letterUnencrypted(char *str);
void numberOfNoUnencryptedLettersInWord(char *, long);
void replacementOfLettersByUser(char *, long);
void myShakerSort(float *, char *, int);
void swapEl(float *, char *, int);
void lettersExchange(char *, long, char, char);
void cancelLettersExchange(char *, long, char , char);
void exchangeManager(char *, long, char, char);
void writeResultToFile(char *, long);
void showListOfCards();
void clean_stdin();


const int N = 34; // ������� ��������
int count=0;
char lettersRusAlphabetBig[N] = "�����Ũ��������������������������"; // ������� ��������� ����
char lettersRusAlphabetSmall[N] = "���������������������������������"; // ������� ��������� ���� �� ������� ��������� (������� � ��������)
typedef struct word // ������ ����� � ��� �����, �������� ����������� ������ ����
{
	char *str; // �����
	int length;// ����� �����
	int num; // ���-�� ������ � ������ ������ � ������
	int numLetterNoCipher; // ���-�� ���������������� ���� � �����

	struct word *next;// ��������� �� ��������� �������, ��� ����������� ������������ ������
};
word *firstWord = NULL; // ������ ������������ ������ ����


void main()
{
	setlocale(LC_ALL, "RUS");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char fileName[100];
	char *getTextFromFile = NULL;
	while (true)
	{
		printf("������� ��� ������������� �����: "); gets_s(fileName);
		getTextFromFile = readTextFromFile(fileName);

		if (getTextFromFile != NULL) break;
	}

	long fileLenght = getTextSize(fileName);

	manager(getTextFromFile, fileLenght);

	system("pause");
}

void manager(char *getTextFromFile, long fileLenght)
{
	system("cls");

	printf(" * * * * * * * * * * * * *\n");
	printf(" * * * ~������������ * * *\n");
	printf(" * * * * * * * * * * * * *\n\n");

	printf("1 - ������ ������� ����, ����� �������������� �����\n");
	printf("2 - ����� �� ����� ����,�������������� �� ����������� ����\n");
	printf("3 - ������ ������ ����\n");
	int command; printf("������� �������: "); scanf("%d", &command); printf("\n"); clean_stdin();

	switch (command)
	{
	case 1:
		textFrequencyAnalise(getTextFromFile, fileLenght);
		break;
	case 2:
		numberOfLettersInWord(getTextFromFile, fileLenght);
		break;
	case 3:
		replacementOfLettersByUser(getTextFromFile, fileLenght);
		break;

	default:
		system("cls");
		manager(getTextFromFile, fileLenght);
		return;
	}
	
	printf("������� Enter...\n");
	getchar();
	//clean_stdin();
	manager(getTextFromFile, fileLenght);
}

/* ������ ����� ����� */
void clean_stdin()
{
	int c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}

/* �������� ���-�� �������� � ����� */
 long getTextSize(char *filePath)
{
	FILE *file = fopen(filePath, "rb");

	if (file == NULL || fileno(file) == -1)
	{
		printf(" * ���������� ��������� ����\n");
		return 0;
	}

	long fileLen = filelength(fileno(file));
	fclose(file);

	return fileLen;
}

/* �������� ����� �� ����� */
char * readTextFromFile(char *fileName)
 {
	 FILE* f = fopen(fileName, "rb");

	 if (f == NULL || fileno(f) == -1)
	 {
		 printf(" * ���������� ��������� ����\n");
		 return NULL;
	 }

	 long fileLen = filelength(fileno(f));
	 char *buf = (char*)malloc((fileLen + 1) * sizeof(char));

	 for (int i = 0; i < fileLen; i++)
		 *(buf + i) = (char)fgetc(f);
	 buf[fileLen] = '\0';
	 //printf("%s\n",buf);
	 return buf;
 }

 /* ������ ������� ����, ����� �������������� ����� */
void textFrequencyAnalise(char *getTextFromFile, long fileLen)
{
	float tFreq[N - 1] = { 0.0 };
	char currentChar;
	int a = N-2;

	for (int j = 0; j < N - 1; j++)
	{
		for (int i = 0; i < fileLen; i++)
		{
			currentChar = *(getTextFromFile + i);

			if (currentChar == lettersRusAlphabetBig[j]) tFreq[j] += 1.0;

			else continue;
		}
	}

		myShakerSort(tFreq, lettersRusAlphabetBig, N - 1);//����������
		printf("|----------------------------------|\n");
		printf("|�����|    |  �������  |    |������|\n");
		for (int j = 0; j < N - 1; j++)
		{
			printf("|#[%c] |----|<%0.7f>|----| #[%c] |\n", lettersRusAlphabetBig[j], tFreq[j] / fileLen, lettersRusAlphabetSmall[a]);
			a--;
		}
		printf("|----------------------------------|\n");

		printf("������ �� �� ���������� ����������?\n");
		printf("1-�� \n2-��� \n");
		int command; printf("������� �������: "); scanf("%d", &command); printf("\n"); clean_stdin();

		if (command==1) autoCorrect(tFreq, getTextFromFile, fileLen);
	}

/* ����� �� ����� ����, �������������� �� ����������� ���� */
void numberOfLettersInWord(char *text, long fileLen)
{
	char *getTextFromFile = strdup(text);
	char sep[23] = { '.',' ', ';' , ')', '(', ',' ,':','�' ,'�','!' ,'\'' ,'"','*','?' ,'�' ,'�' ,'\n' ,'\r','�' };
	char *istr;// ����������, � ������� ����� ���������� ��������� ������ ������ c����� str
	int maxLen=0;//������������ ����� �����
	
	istr = strtok(getTextFromFile, sep); // ��������� ������ ����� ������

	//printf("%s \n", istr);
	// ��������� ����������� ������
	while (istr != NULL)
	{
		if (strlen(istr) > maxLen) maxLen= strlen(istr);
		// ����� ��������� ���������� �����
		//printf("%s \n", istr);
		createWordInStruct(istr);
		// ��������� ��������� ����� ������
		istr = strtok(NULL, sep);
	}

	for (int i = maxLen; i > 0; i--)
	{
		printf("�����, ������ %d: \n", i);
		word *tmp = firstWord;
		while (tmp)
		{
			if (tmp->length == i) printf(" * %s : ���-�� ��������� ����� � ������: %d\n", tmp->str, tmp->num);
			tmp = tmp->next;
		}
	}

	//printf("������������ ����� ����� = %d\n", maxLen);
}
 
/* ������� ��������� ����� � �������� ��� � ����������� ������ ���� */
word * createWordInStruct(char *s)
{
	word *tmp = firstWord;
	while (tmp)
	{
		if (strcmp(tmp->str, s) == 0) { tmp->num++; return tmp; }
		tmp = tmp->next;
	}

	// ������� ���������
	word * res = (word *) malloc(sizeof(word));
	
	res->str = s;
	res->length = strlen(s);
	res->numLetterNoCipher = letterUnencrypted(s);
	res->next = NULL;
	res->num = 1;

	// ��������� ��������� � ����������� ������
	if (!firstWord) firstWord = res;
	else
	{
		tmp = firstWord;
		while (tmp)
		{
			if (tmp->next == NULL) { tmp->next = res; break; }
			tmp = tmp->next;
		}
	}

	return res;
}

/* ������� ����� ��������� �� ����� */
void displayTextRowByRow(char *text)
{
	printf("����� ������ ���������\n");
	int i = 0, rowNum = 0; char ch = *text;

	printf("\n # ������ %d:\n", ++rowNum);
	while (ch != '\0')
	{
		if (ch == '\n') { printf("\n\n # ������ %d:", ++rowNum); }
		if (ch != '\r') printf("%c", ch);
		ch = *(text + ++i);
	}

	printf("\n\n����� ������ �������\n\n");
}

/* ���������� �� ������� */
void autoCorrect(float *tFreq, char *getTextFromFile, long fileLen)
{
	char currentChar;
	char *text=strdup(getTextFromFile);
	int a = N-1;

	for (int j = 0; j < N - 1; j++)
	{
		for (int i = 0; i < fileLen; i++)
		{
			currentChar = *(text + i);

			if (currentChar == lettersRusAlphabetBig[j]) *(text + i) = lettersRusAlphabetSmall[a];
			else continue;
		}
		a--;
	}
	//printf("%s\n", getTextFromFile);
	writeToFile("autoEncoded.txt", text, fileLen);
	printf("����������, ������ �� ���������� �������, ���������. ��������� ������� � ���� autoEncoded.txt\n");

}

/* ������ � ���� */
void writeToFile(char *fileName, char* wh, long fileLen)
{
	FILE *file;

	if (!wh)
	{
		printf(" * ������ � ���� ����������\n");
		return;
	}

	if ((file = fopen(fileName, "wb")) == NULL)
	{
		printf(" * ���� \"%s\" ���������� ������� ��� �������\n", fileName);
		return;
	}
	else
	{
		for (int i = 0; i <fileLen; i++)
			fwrite( (wh + i), 1, 1, file);
	}

	fclose(file);
}


/* ������� ���������� ���������������� ���� � ����� */
int letterUnencrypted(char *str)
{
	int len = 0;
	char currentChar;

	for (int j = 0; j < N - 1; j++)
	{
		for (int i = 0; i < strlen(str); i++) // ������� ���-�� ��������� ������� ����� � �����
		{
			currentChar = *(str + i);

			if (currentChar == lettersRusAlphabetBig[j]) len++;
            else continue;
		}
	}
	return len;
}

/* ����� �� ����� ����, �������������� �� ����������� ���������������� ���� */
void numberOfNoUnencryptedLettersInWord(char *text, long fileLen)
{
	char sep[23] = { '.',' ', ';' , ')', '(', ',' ,':','�' ,'�','!' ,'\'' ,'"','*','?' ,'�' ,'�' ,'\n' ,'\r', '�' };
	char *istr;// ����������, � ������� ����� ���������� ��������� ������ ������ c����� str
	int maxLen = 0;//������������ ����� �����

	char *getTextFromFile = strdup(text);

	istr = strtok(getTextFromFile, sep); // ��������� ������ ����� ������

										 // ��������� ����������� ������
	while (istr != NULL)
	{
		if (strlen(istr) > maxLen) maxLen = strlen(istr);
		// ����� ��������� ���������� �����
		//printf("%s \n", istr);
		createWordInStruct(istr);
		// ��������� ��������� ����� ������
		istr = strtok(NULL, sep);
	}

	for (int i = maxLen; i > -1 ; i--)
	{
		printf("�����, � ������� �� ������������(�/�) %d ����(�/�): \n", i);
		word *tmp = firstWord;
		while (tmp)
		{
			if (tmp->numLetterNoCipher == i) printf(" * %s \n", tmp->str);
			tmp = tmp->next;
		}
	}
}

/* ������ ���� ������������� */
void replacementOfLettersByUser(char *getTextFromFile, long fileLenght)
{
	char buf1 = '1', buf2 = '2'; // ���������� ����������� ��������� ������ ����, ��� ����������� �� ������
	char *buffer = strdup(getTextFromFile); exchangeManager(buffer, fileLenght, buf1, buf2);
}

/* ������ ���������� � ����*/
void writeResultToFile(char *buffer, long fileLenght)
{
	writeToFile("handEncoded.txt", buffer, fileLenght);
	printf("������ ������ ���������. ��������� ������� � ���� handEncoded.txt\n");
}

/* "������"-���������� */
void myShakerSort(float *arr, char *s,  int size)
{
	int leftMark = 1;
	int rightMark = size - 1;
	while (leftMark <= rightMark)
	{
		for (int i = rightMark; i >= leftMark; i--)
			if (arr[i - 1] > arr[i]) swapEl(arr, s, i);
		leftMark++;


		for (int i = leftMark; i <= rightMark; i++)
			if (arr[i - 1] > arr[i]) swapEl(arr, s, i);
		rightMark--;
	}
}
/* ����� �������� ����� */
void swapEl(float *arr, char *s, int i)
{
	int buff;
	char buffC;
	buff = arr[i];
	arr[i] = arr[i - 1];
	arr[i - 1] = buff;
	buffC = s[i];
	s[i] = s[i - 1];
	s[i - 1] = buffC;
}

/* ����� ������ ��������� */
void showListOfCards()
{
	printf("\n********************************************************************\n");
	printf("\n~ ������� �� ����� ����� ����� �������: \n");
	printf(" *�����������: [�]\n *�����: [�] [�]\n *��������: [�] [�] [�] [�] [�]\n");
	printf("\n********************************************************************\n");
	printf("\n~ ������� �� ���� ���� ����� �������: \n");
	printf(" *�����������: [��] [��] [��] [��] [��]\n *����: [��] \n *��������: [��] [��] [��] [��] [��] [��] [��] [��] [��]\n");
	printf(" *�������: [��] [��] [��] [��] [��]\n *���������������: [��] [��] [��] [��] [��] [��] [��] [��]\n *����������: [��] [��] [��] [��]\n");
	printf("\n********************************************************************\n");
	printf("\n~ ��������� ��������������:\n -�� | -�� | -�� | -�� | -�� | -��� | -��� | -��� |-��� | -�� | -�� | -�� | -�� | -�� | -�� | -�� | -�� | -�� | -��\n");
	printf("\n********************************************************************\n");
	printf("\n~ ����� ������������� ����������: \n");
	printf("\n*���\n*���\n*���\n*���\n*���\n*���\n*����\n*���\n*����\n*������\n*�����\n*����\n*�����\n*����\n*����\n");
}

/* ��������������� ���� ������ */
void lettersExchange(char *text, long fileLen, char lettersSmall, char lettersBig)
{
	char character;
	
	for (int i = 0; i < fileLen; i++)
	{
		character = *(text+i);
		if (character == lettersBig) *(text + i) = lettersSmall;
	}
}

/* �������� ������ ������*/
void exchangeManager(char *buffer, long fileLenght, char buf1, char buf2)
{
	system("cls");

	printf("1 - ����� �� ����� ����, �������������� �� ����������� �������������� ����\n");
	printf("2 - ����������� ������������ � ��������������� �� ������ ������ ������ ���������\n");
	printf("3 - ���������� ������\n");
	printf("4 - ������ ��������� ������\n");

	int command; printf("������� �������: "); scanf("%d", &command); printf("\n"); clean_stdin();

	switch (command)
	{
	case 1:
		numberOfNoUnencryptedLettersInWord(buffer, fileLenght);
		break;
	case 2:
		displayTextRowByRow(buffer);
		break;
	case 3:
		printf("%s", buffer); //����� �� ����� �������� ��������� ������������
		showListOfCards(); //����� �� ����� ������ ���������
		char lettersSmall, lettersBig;
		printf("����� ����� ����� ��������(������� ��������� �����)? ����� �����:"); scanf("%c", &lettersBig); printf("\n"); clean_stdin();
		printf("�� ����� ����� ����� ��������(������� �������� �����)? ����� �����:"); scanf("%c", &lettersSmall); printf("\n"); clean_stdin();
		buf1 = lettersSmall;
		buf2 = lettersBig;

		firstWord = NULL;
		lettersExchange(buffer, fileLenght, lettersSmall, lettersBig);
		count++;
		break;
	case 4:
		cancelLettersExchange(buffer, fileLenght, buf1, buf2);
		break;

	default:
		system("cls");
		exchangeManager(buffer, fileLenght, buf1, buf2);
		return;
	}

		printf("������� Enter...\n");
		getchar();
		if (count!=N) exchangeManager(buffer, fileLenght, buf1, buf2);
		else writeResultToFile(buffer, fileLenght);
	
}

/* ������ ��������� ������ */
void cancelLettersExchange(char *text, long fileLen, char buf1, char buf2)
{
	char character;
	for (int i = 0; i < fileLen; i++)
	{
		character = *(text + i);
		if (character == buf1) *(text + i) = buf2;
	}
	if (buf1 != '1')
	{
		printf("������ ��������� ������ ������� �����������\n");
		printf("\n����� %c �������� �� ����� %c\n\n", buf1, buf2);
	}

}

