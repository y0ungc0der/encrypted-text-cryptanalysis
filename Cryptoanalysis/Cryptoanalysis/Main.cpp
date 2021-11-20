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


const int N = 34; // Ёмкость алфавита
int count=0;
char lettersRusAlphabetBig[N] = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; // словарь заглавных букв
char lettersRusAlphabetSmall[N] = "оеаинтсрвлкмдпуяыьгзбчйхжшюцщэфъё"; // словарь прописных букв по частоте появления (начиная с большего)
typedef struct word // храним слово и его длину, образуем односвязный список слов
{
	char *str; // слово
	int length;// длина слова
	int num; // кол-во встреч с данным словом в тексте
	int numLetterNoCipher; // кол-во нерасшифрованных букв в слове

	struct word *next;// указатель на следующий элемент, для образования односвязного списка
};
word *firstWord = NULL; // начало односвязного списка слов


void main()
{
	setlocale(LC_ALL, "RUS");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char fileName[100];
	char *getTextFromFile = NULL;
	while (true)
	{
		printf("Укажите имя декодируемого файла: "); gets_s(fileName);
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
	printf(" * * * ~Криптоанализ * * *\n");
	printf(" * * * * * * * * * * * * *\n\n");

	printf("1 - анализ частоты букв, вывод предполагаемых замен\n");
	printf("2 - вывод на экран слов,сгрупированных по колличеству букв\n");
	printf("3 - ручная замена букв\n");
	int command; printf("Введите команду: "); scanf("%d", &command); printf("\n"); clean_stdin();

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
	
	printf("Введите Enter...\n");
	getchar();
	//clean_stdin();
	manager(getTextFromFile, fileLenght);
}

/* Чистит буфер ввода */
void clean_stdin()
{
	int c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}

/* Получить кол-во символов в файле */
 long getTextSize(char *filePath)
{
	FILE *file = fopen(filePath, "rb");

	if (file == NULL || fileno(file) == -1)
	{
		printf(" * Невозможно прочитать файл\n");
		return 0;
	}

	long fileLen = filelength(fileno(file));
	fclose(file);

	return fileLen;
}

/* Получаем текст из файла */
char * readTextFromFile(char *fileName)
 {
	 FILE* f = fopen(fileName, "rb");

	 if (f == NULL || fileno(f) == -1)
	 {
		 printf(" * Невозможно прочитать файл\n");
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

 /* Анализ частоты букв, вывод предполагаемых замен */
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

		myShakerSort(tFreq, lettersRusAlphabetBig, N - 1);//переделать
		printf("|----------------------------------|\n");
		printf("|Буква|    |  Частота  |    |Замена|\n");
		for (int j = 0; j < N - 1; j++)
		{
			printf("|#[%c] |----|<%0.7f>|----| #[%c] |\n", lettersRusAlphabetBig[j], tFreq[j] / fileLen, lettersRusAlphabetSmall[a]);
			a--;
		}
		printf("|----------------------------------|\n");

		printf("Хотите ли вы произвести автозамену?\n");
		printf("1-Да \n2-Нет \n");
		int command; printf("Введите команду: "); scanf("%d", &command); printf("\n"); clean_stdin();

		if (command==1) autoCorrect(tFreq, getTextFromFile, fileLen);
	}

/* Вывод на экран слов, сгрупированных по колличеству букв */
void numberOfLettersInWord(char *text, long fileLen)
{
	char *getTextFromFile = strdup(text);
	char sep[23] = { '.',' ', ';' , ')', '(', ',' ,':','—' ,'–','!' ,'\'' ,'"','*','?' ,'»' ,'«' ,'\n' ,'\r','•' };
	char *istr;// Переменная, в которую будут заноситься начальные адреса частей cтроки str
	int maxLen=0;//Максимальная длина слова
	
	istr = strtok(getTextFromFile, sep); // Выделение первой части строки

	//printf("%s \n", istr);
	// Выделение последующих частей
	while (istr != NULL)
	{
		if (strlen(istr) > maxLen) maxLen= strlen(istr);
		// Вывод очередной выделенной части
		//printf("%s \n", istr);
		createWordInStruct(istr);
		// Выделение очередной части строки
		istr = strtok(NULL, sep);
	}

	for (int i = maxLen; i > 0; i--)
	{
		printf("Слова, длиной %d: \n", i);
		word *tmp = firstWord;
		while (tmp)
		{
			if (tmp->length == i) printf(" * %s : кол-во появлений слова в тексте: %d\n", tmp->str, tmp->num);
			tmp = tmp->next;
		}
	}

	//printf("Максимальная длина слова = %d\n", maxLen);
}
 
/* Создали структуру слова и добавили его в односвязный список слов */
word * createWordInStruct(char *s)
{
	word *tmp = firstWord;
	while (tmp)
	{
		if (strcmp(tmp->str, s) == 0) { tmp->num++; return tmp; }
		tmp = tmp->next;
	}

	// создаем структуру
	word * res = (word *) malloc(sizeof(word));
	
	res->str = s;
	res->length = strlen(s);
	res->numLetterNoCipher = letterUnencrypted(s);
	res->next = NULL;
	res->num = 1;

	// добавляем структуру в односвязный список
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

/* Выводит текст построчно на экран */
void displayTextRowByRow(char *text)
{
	printf("Вывод текста построчно\n");
	int i = 0, rowNum = 0; char ch = *text;

	printf("\n # Строка %d:\n", ++rowNum);
	while (ch != '\0')
	{
		if (ch == '\n') { printf("\n\n # Строка %d:", ++rowNum); }
		if (ch != '\r') printf("%c", ch);
		ch = *(text + ++i);
	}

	printf("\n\nВывод текста окончен\n\n");
}

/* Автозамена по частоте */
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
	printf("Автозамена, исходя из частотного анализа, выполнена. Результат записан в файл autoEncoded.txt\n");

}

/* Запись в файл */
void writeToFile(char *fileName, char* wh, long fileLen)
{
	FILE *file;

	if (!wh)
	{
		printf(" * Запись в файл невозможна\n");
		return;
	}

	if ((file = fopen(fileName, "wb")) == NULL)
	{
		printf(" * Файл \"%s\" невозможно открыть или создать\n", fileName);
		return;
	}
	else
	{
		for (int i = 0; i <fileLen; i++)
			fwrite( (wh + i), 1, 1, file);
	}

	fclose(file);
}


/* Считаем количество нерасшифрованных букв в слове */
int letterUnencrypted(char *str)
{
	int len = 0;
	char currentChar;

	for (int j = 0; j < N - 1; j++)
	{
		for (int i = 0; i < strlen(str); i++) // Считаем кол-во вхождений текущей буквы в слово
		{
			currentChar = *(str + i);

			if (currentChar == lettersRusAlphabetBig[j]) len++;
            else continue;
		}
	}
	return len;
}

/* Вывод на экран слов, сгрупированных по колличеству нерасшифрованных букв */
void numberOfNoUnencryptedLettersInWord(char *text, long fileLen)
{
	char sep[23] = { '.',' ', ';' , ')', '(', ',' ,':','—' ,'–','!' ,'\'' ,'"','*','?' ,'»' ,'«' ,'\n' ,'\r', '•' };
	char *istr;// Переменная, в которую будут заноситься начальные адреса частей cтроки str
	int maxLen = 0;//Максимальная длина слова

	char *getTextFromFile = strdup(text);

	istr = strtok(getTextFromFile, sep); // Выделение первой части строки

										 // Выделение последующих частей
	while (istr != NULL)
	{
		if (strlen(istr) > maxLen) maxLen = strlen(istr);
		// Вывод очередной выделенной части
		//printf("%s \n", istr);
		createWordInStruct(istr);
		// Выделение очередной части строки
		istr = strtok(NULL, sep);
	}

	for (int i = maxLen; i > -1 ; i--)
	{
		printf("Слова, в которых не расшифровано(ы/а) %d букв(ы/а): \n", i);
		word *tmp = firstWord;
		while (tmp)
		{
			if (tmp->numLetterNoCipher == i) printf(" * %s \n", tmp->str);
			tmp = tmp->next;
		}
	}
}

/* Замена букв пользователем */
void replacementOfLettersByUser(char *getTextFromFile, long fileLenght)
{
	char buf1 = '1', buf2 = '2'; // переменные сохраняющие последнюю замену букв, для возможности ее отмены
	char *buffer = strdup(getTextFromFile); exchangeManager(buffer, fileLenght, buf1, buf2);
}

/* Запись результата в файл*/
void writeResultToFile(char *buffer, long fileLenght)
{
	writeToFile("handEncoded.txt", buffer, fileLenght);
	printf("Ручная замена выполнена. Результат записан в файл handEncoded.txt\n");
}

/* "Шейкер"-сортировка */
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
/* Обмен значений ячеек */
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

/* Вывод списка подсказок */
void showListOfCards()
{
	printf("\n********************************************************************\n");
	printf("\n~ Словами из одной буквы могут являтся: \n");
	printf(" *местоимение: [я]\n *союзы: [а] [и]\n *предлоги: [в] [к] [о] [с] [у]\n");
	printf("\n********************************************************************\n");
	printf("\n~ Словами из двух букв могут являтся: \n");
	printf(" *местоимения: [мы] [вы] [их] [он] [ты]\n *союз: [но] \n *предлоги: [за] [из] [до] [во] [ко] [на] [об] [от] [по]\n");
	printf(" *частицы: [бы] [не] [ни] [да] [же]\n *существительные: [ёж] [уж] [ум] [ус] [щи] [юг] [яд] [як]\n *междометия: [ну] [ах] [ох] [ой]\n");
	printf("\n********************************************************************\n");
	printf("\n~ Окончания прилагательных:\n -ой | -ый | -ий | -ое | -ее | -ого | -его | -ому |-ему | -ым | -им | -ом | -ем | -ой | -ей | -ою | -ею | -ую | -юю\n");
	printf("\n********************************************************************\n");
	printf("\n~ Часто встречающиеся словоформы: \n");
	printf("\n*что\n*как\n*это\n*все\n*так\n*был\n*если\n*для\n*меня\n*только\n*когда\n*было\n*может\n*быть\n*этот\n");
}

/* Непосредственно сама замена */
void lettersExchange(char *text, long fileLen, char lettersSmall, char lettersBig)
{
	char character;
	
	for (int i = 0; i < fileLen; i++)
	{
		character = *(text+i);
		if (character == lettersBig) *(text + i) = lettersSmall;
	}
}

/* Менеджер ручной замены*/
void exchangeManager(char *buffer, long fileLenght, char buf1, char buf2)
{
	system("cls");

	printf("1 - вывод на экран слов, сгрупированных по колличеству расшифрованных букв\n");
	printf("2 - отображение криптограммы и расшифрованного на данный момент текста построчно\n");
	printf("3 - выполнение замены\n");
	printf("4 - отмена последней замены\n");

	int command; printf("Введите команду: "); scanf("%d", &command); printf("\n"); clean_stdin();

	switch (command)
	{
	case 1:
		numberOfNoUnencryptedLettersInWord(buffer, fileLenght);
		break;
	case 2:
		displayTextRowByRow(buffer);
		break;
	case 3:
		printf("%s", buffer); //вывод на экран текущего состояния криптограммы
		showListOfCards(); //вывод на экран списка подсказок
		char lettersSmall, lettersBig;
		printf("Какую букву нужно заменить(вводить заглавные буквы)? Выбор буквы:"); scanf("%c", &lettersBig); printf("\n"); clean_stdin();
		printf("На какую букву нужно заменить(вводить строчные буквы)? Выбор буквы:"); scanf("%c", &lettersSmall); printf("\n"); clean_stdin();
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

		printf("Введите Enter...\n");
		getchar();
		if (count!=N) exchangeManager(buffer, fileLenght, buf1, buf2);
		else writeResultToFile(buffer, fileLenght);
	
}

/* Отмена последней замены */
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
		printf("Отмена последней замены успешно произведена\n");
		printf("\nБуква %c заменена на букву %c\n\n", buf1, buf2);
	}

}

