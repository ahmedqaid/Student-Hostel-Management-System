#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#pragma warning (disable: 4996)
#pragma warning (disable: 6001)
#pragma warning (disable: 6031)
#pragma warning (disable: 6054)

void mainMenu();
void registration();
void textFilesCheck();
int idCheck(char idEntry[10]);
void roomBooking(char genderCheck1[10]);
void payment();
void change();
void removeStudent(char stdId[10]);
void roomCancelling();
void searchInfo();
void idSearch();
void printInfo();
void roomSearch();
int blockRoomCheck(char BNC[3], int RNC);
void billPayment();
void checkAvailability();
void checkByBlock();
int checkByRoom();
void studentInvoiceCheck();
void deleteRecord();
void checkPayment();
void overallInvoice(char cc[3]);


void fontColor(int cvar)
{
	WORD window;
	HANDLE outputstd = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO catt;

	if (GetConsoleScreenBufferInfo(outputstd, &catt))
	{
		window = (catt.wAttributes & (int)176) + (cvar & (int)11);
		SetConsoleTextAttribute(outputstd, window);
	}
}

struct studentInfo
{
	char studentID[10];
	char firstName[25];
	char lastName[25];
	char studentGender[10];
	char phoneNumber[20];
	char emailAddress[30];
	struct roomInfo
	{
		int roomNumber;
		char blockNumber[3];
		int weeksBooked;
		char mealsSub[4];
		char gymSub[4];
		int amountPayable;
		int totalCollected;
		int outstanding;
	}RI;
}SI;

int main()
{
	fontColor(30);
	textFilesCheck();
	mainMenu();
}

void mainMenu()
{
	system("CLS");
	char  timeBuf[26];
	time_t timer;
	struct tm* tm_info;
	time(&timer);
	tm_info = localtime(&timer);
	strftime(timeBuf, 26, "%Y-%m-%d %H:%M:%S", tm_info);
	printf("MAIN MENU\t%s\n_______________________________________\n", timeBuf);
	printf("1. Register a student\n");
	printf("2. Change room\n");
	printf("3. Search hosteler\'s room detail\n");
	printf("4. Check room availability\n");
	printf("5. Pay bills\n");
	printf("6. Check invoice payments\n");
	printf("7. Remove student\n");
	printf("8. Exit\n");
	printf("Enter your choice: ");
	switch (getche())
	{
	case '1':
		registration();
		break;
	case '2':
		change();
		break;
	case '3':
		searchInfo();
		break;
	case '4':
		checkAvailability();
		break;
	case '5':
		billPayment();;
		break;
	case '6':
		checkPayment();
		break;
	case '7':
		deleteRecord();
		break;
	case '8':
		exit(1);
		break;
	default:
		mainMenu();
	}
	mainMenu();
}

int idCheck(char idEntry[10])
{
	FILE* idc;
	idc = fopen("Student Record.txt", "r");
	while (!feof(idc))
	{
		fread(&SI, sizeof(SI), 1, idc);
		if (strcmp(idEntry, SI.studentID) == 0)
		{
			fclose(idc);
			return 1;
		}
	}
	fclose(idc);
	return 0;
}

void textFilesCheck()
{
	int totalRoomsBlock = 100, i;
	FILE* createFile;
	if (createFile = fopen("Block A1.txt", "r") == NULL)
	{
		createFile = fopen("Block A1.txt", "w");
		fprintf(createFile, "Block A1\n\n");
		for (i = 1; i <= totalRoomsBlock; i++)
			fprintf(createFile, "F A1-%d\n", i);
		fclose(createFile);

		createFile = fopen("Block B1.txt", "w");
		fprintf(createFile, "Block B1\n\n");
		for (i = 1; i <= totalRoomsBlock; i++)
			fprintf(createFile, "F B1-%d\n", i);
		fclose(createFile);

		createFile = fopen("Block B2.txt", "w");
		fprintf(createFile, "Block B2\n\n");
		for (i = 1; i <= totalRoomsBlock; i++)
			fprintf(createFile, "FF B2-%d\n", i);
		fclose(createFile);

		createFile = fopen("Block B4.txt", "w");
		fprintf(createFile, "Block B4\n\n");
		for (i = 1; i <= totalRoomsBlock; i++)
			fprintf(createFile, "FFFF B4-%d\n", i);
		fclose(createFile);

		createFile = fopen("Student Record.txt", "w");
		fclose(createFile);
	}
}

void registration()
{
	int idRead;
	char genderCheck[10], studentIdRead[10];
	system("CLS");
	printf("Student Registration\n_______________________________________\n");
	printf("Student ID: ");
	scanf("%s", &studentIdRead);
	idRead = idCheck(studentIdRead);
	if (idRead == 1)
	{
		printf("This ID has already been checked in\n");
		system("PAUSE");
		system("CLS");
		mainMenu();
	}
	else if (idRead == 0)
	{
		strcpy(SI.studentID, studentIdRead);
		printf("Student First Name: ");
		scanf("%s", &SI.firstName);
		printf("Student Last Name: ");
		scanf("%s", &SI.lastName);
		printf("Gender: ");
		scanf("%s", &genderCheck);
		printf("Phone Number: ");
		scanf("%s", &SI.phoneNumber);
		printf("Email Address: ");
		scanf("%s", &SI.emailAddress);
		switch (genderCheck[0])
		{
		case 'M':
		case 'm':
			strcpy(SI.studentGender, "male");
			printf("Due to construction delay, only block A1 is available for male students\n");
			roomBooking(SI.studentGender);
			break;
		case 'F':
		case 'f':
			strcpy(SI.studentGender, "female");
			roomBooking(SI.studentGender);
			break;
		default:
			printf("The gender you entered is invalid\n");
			system("PAUSE");
			system("CLS");
			mainMenu();
		}
	}
}

void roomBooking(char genderCheck1[10])
{
	char fileName[13];
	if (strcmp(genderCheck1, "male") == 0)
	{
		strcpy(fileName, "Block A1.txt");
		strcpy(SI.RI.blockNumber, "A1");
	}
	else if (strcmp(genderCheck1, "female") == 0)
	{
	bedChoice:
		system("PAUSE");
		printf("There are 3 block available for female students\n");
		printf("1. Block B1 (Single-bedded rooms)\n");
		printf("2. Block B2 (Double-bedded rooms)\n");
		printf("3. Block B4 (Four-bedded rooms)\n");
		printf("4. Return to main menu\n");
		printf("Enter your choice here: ");
		switch (getche())
		{
		case '1':
			strcpy(fileName, "Block B1.txt");
			strcpy(SI.RI.blockNumber, "B1");
			break;
		case '2':
			strcpy(fileName, "Block B2.txt");
			strcpy(SI.RI.blockNumber, "B2");
			break;
		case '3':
			strcpy(fileName, "Block B4.txt");
			strcpy(SI.RI.blockNumber, "B4");
			break;
		case '4':
			system("CLS");
			mainMenu();
			break;
		default:
			printf("Invalid choice\n");
			system("PAUSE");
			system("CLS");
			goto bedChoice;
		}
	}
	FILE* roomBookFile;
	int countLine = 0;
	char fileRead;
	roomBookFile = fopen(fileName, "r+");

	fileRead = getc(roomBookFile);
	while (fileRead != EOF)
	{
		if (fileRead == 'F')
		{
			fseek(roomBookFile, -1, SEEK_CUR);
			fputc('T', roomBookFile);
			fseek(roomBookFile, 0, SEEK_CUR);
			break;
		}
		if (fileRead == '\n')
			countLine++;
		fileRead = getc(roomBookFile);
	}
	fclose(roomBookFile);
	SI.RI.roomNumber = countLine - 1;
	printf("\nRoom number (%d) in Block %s is successfully booked\nYou may now proceed with the payment\n", SI.RI.roomNumber, SI.RI.blockNumber);
	system("PAUSE");
	system("CLS");
	payment();
}

void payment()
{
	int mealsRate, gymRate, roomRate, amountPayable;
	if (strcmp(SI.RI.blockNumber, "A1") == 0)
		roomRate = 400;
	else if (strcmp(SI.RI.blockNumber, "B1") == 0)
		roomRate = 400;
	else if (strcmp(SI.RI.blockNumber, "B2") == 0)
		roomRate = 200;
	else if (strcmp(SI.RI.blockNumber, "B4") == 0)
		roomRate = 100;
	else
	{
		printf("An error occured\n");
		system("PAUSE");
		system("CLS");
		mainMenu();
	}
	printf("Payment facility\n__________________________\n");
	printf("For how long is the student staying (in weeks): ");
	scanf("%d", &SI.RI.weeksBooked);
	if (SI.RI.weeksBooked < 1)
	{
		printf("The number of weeks booked is invalid\n");
		system("PAUSE");
		system("CLS");
		payment();
	}
meals:
	printf("Meals subscription (Y / N): ");
	switch (getche())
	{
	case 'Y':
	case 'y':
		strcpy(SI.RI.mealsSub, "yes");
		mealsRate = 120;
		break;
	case 'N':
	case 'n':
		strcpy(SI.RI.mealsSub, "no");
		mealsRate = 0;
		break;
	default:
		printf("Invalid Entry\n");
		system("PAUSE");
		system("CLS");
		goto meals;
	}
gym:
	printf("\nGym subscription (Y / N): ");
	switch (getche())
	{
	case 'Y':
	case 'y':
		strcpy(SI.RI.gymSub, "yes");
		gymRate = 10;
		break;
	case 'N':
	case 'n':
		strcpy(SI.RI.gymSub, "no");
		gymRate = 0;
		break;
	default:
		printf("Invalid Entry\n");
		system("PAUSE");
		system("CLS");
		goto gym;
	}
	SI.RI.amountPayable = SI.RI.weeksBooked * (roomRate + mealsRate + gymRate);
	printf("\nAmount payable: RM %d", SI.RI.amountPayable);
	printf("\nHow much are you willing to pay now: ");
	scanf("%d", &SI.RI.totalCollected);
	SI.RI.outstanding = SI.RI.amountPayable - SI.RI.totalCollected;
	printInfo();
	FILE* saveInfo;
	saveInfo = fopen("Student Record.txt", "a+");
	fwrite(&SI, sizeof(SI), 1, saveInfo);
	fclose(saveInfo);
	system("PAUSE");
	system("CLS");
	mainMenu();
}

void change()
{
	char studentIdRead1[10];
	int idRead1;
	system("CLS");
	printf("Room change\n______________________\n");
	printf("Enter ID number: ");
	scanf("%s", &studentIdRead1);
	idRead1 = idCheck(studentIdRead1);
	if (idRead1 == 0)
	{
		printf("The ID entered is invalid!\n");
		system("PAUSE");
		system("CLS");
		mainMenu();
	}
	else if (idRead1 == 1)
	{
		if (strcmp(SI.studentGender, "male") == 0)
		{
			printf("\nNOTE: There is only one block and one room type for male students\n");
			system("PAUSE");
			system("CLS");
			mainMenu();
		}
		else
		{
			char gend[10];
			strcpy(gend, SI.studentGender);
			removeStudent(studentIdRead1);
			roomCancelling();
			roomBooking(gend);
		}
	}
}

void removeStudent(char stdId[10])
{
	FILE* removeStd;
	FILE* temp;
	removeStd = fopen("Student Record.txt", "r");
	temp = fopen("Temp.txt", "w");
	while (fread(&SI, sizeof(SI), 1, removeStd))
		if (strcmp(SI.studentID, stdId) != 0)
			fwrite(&SI, sizeof(SI), 1, temp);
	fclose(removeStd);
	fclose(temp);
	removeStd = fopen("Student Record.txt", "w");
	temp = fopen("temp.txt", "r");
	while (fread(&SI, sizeof(SI), 1, temp))
		fwrite(&SI, sizeof(SI), 1, removeStd);
	fclose(removeStd);
	fclose(temp);
	return;
}

void roomCancelling()
{
	FILE* openBlock;
	int countLine1 = 0;
	char charRead, fileName2[13] = "Block ";
	strcat(fileName2, SI.RI.blockNumber);
	strcat(fileName2, ".txt");
	openBlock = fopen(fileName2, "r+");
	charRead = getc(openBlock);
	while (charRead != EOF)
	{
		if (charRead == '\n')
		{
			if (countLine1 == SI.RI.roomNumber)
			{
				if (fgetc(openBlock) == 'T')
				{
					fseek(openBlock, -1, SEEK_CUR);
					fputc('F', openBlock);
					fseek(openBlock, 0, SEEK_CUR);
					break;
				}
				else
					printf("\nThis room is not occupied\n");
			}
			countLine1++;
		}
		charRead = getc(openBlock);
	}
	fclose(openBlock);
	return;
}

void deleteRecord()
{
	char stuID[10];
	int idCheck6;
	printf("\nRemove student\n__________________________\n");
	printf("Enter student ID: ");
	scanf("%s", &stuID);
	idCheck6 = idCheck(stuID);
	if (idCheck6 == 0)
		printf("\nInvalid ID!\n");
	else if (idCheck6 == 1)
	{
		removeStudent(stuID);
		roomCancelling();
		printf("\nStudent record deleted successfully\n");
	}
	system("PAUSE");
	system("CLS");
	struct studentInfo
	{
		char studentID[10];
		char firstName[25];
		char lastName[25];
		char studentGender[10];
		char phoneNumber[20];
		char emailAddress[30];
		struct roomInfo
		{
			int roomNumber;
			char blockNumber[3];
			int weeksBooked;
			char mealsSub[4];
			char gymSub[4];
			int amountPayable;
			int totalCollected;
			int outstanding;
		}RI;
	}SI;
	mainMenu();
}

void searchInfo()
{
	system("CLS");
	printf("Search\n_________________________\n");
	printf("1. Search by ID\n");
	printf("2. Search by room number\n");
	printf("Enter your choice: ");
	switch (getche())
	{
	case '1':
		idSearch();
		break;
	case '2':
		roomSearch();
		break;
	default:
		printf("Invalid Entry!\n");
		system("PAUSE");
		system("CLS");
		searchInfo();
	}
}

void idSearch()
{
	char idRead3[10];
	int studentIdCheck2;
	system("CLS");
	printf("Search by ID\n_____________________________\n");
	printf("Enter student ID: ");
	scanf("%s", &idRead3);
	studentIdCheck2 = idCheck(idRead3);
	if (studentIdCheck2 == 0)
	{
		printf("\nThe entered ID is invalid\n");
		system("PAUSE");
		system("CLS");
		mainMenu();
	}
	else if (studentIdCheck2 == 1)
	{
		printInfo();
		system("PAUSE");
		system("CLS");
		mainMenu();
	}
}

void printInfo()
{
	printf("\nReport\n_______________________________________________________");
	printf("\nStudent ID: %s", SI.studentID);
	printf("\nStudent Name: %s %s", SI.firstName, SI.lastName);
	printf("\nGender: %s", SI.studentGender);
	printf("\nPhone Number: %s", SI.phoneNumber);
	printf("\nEmail Address : %s", SI.emailAddress);
	printf("\nRoom Number: %s-%d", SI.RI.blockNumber, SI.RI.roomNumber);
	printf("\nWeeks booked: %d", SI.RI.weeksBooked);
	printf("\nMeals Subscription: %s\t\tGym Subscription: %s", SI.RI.mealsSub, SI.RI.gymSub);
	printf("\nAmount Payable: RM %d", SI.RI.amountPayable);
	printf("\nTotal Collected: RM %d", SI.RI.totalCollected);
	printf("\nOutstanding: RM %d", SI.RI.outstanding);
	printf("\n______________________________________________________\n\n");
	return;
}

void roomSearch()
{
	int roomNumberCheck, roomCheck;
	char blockNumberCheck[3];
	system("CLS");
	printf("Search by room\n____________________________\n");
	printf("\nEnter block number (EG: A1): ");
	scanf("%s", &blockNumberCheck);
	printf("\nEnter room number (EG: 10): ");
	scanf("%d", &roomNumberCheck);
	roomCheck = blockRoomCheck(blockNumberCheck, roomNumberCheck);
	if (roomCheck == 0)
		printf("This room is inouccupied\n");
	else if (roomCheck == 1)
		printInfo();
	system("PAUSE");
	system("CLS");
	mainMenu();
}

int blockRoomCheck(char BNC[3], int RNC)
{
	FILE* roomc;
	roomc = fopen("Student Record.txt", "r");
	while (!feof(roomc))
	{
		fread(&SI, sizeof(SI), 1, roomc);
		if ((stricmp(BNC, SI.RI.blockNumber) == 0) && (RNC == SI.RI.roomNumber))
		{
			fclose(roomc);
			return 1;
		}
	}
	fclose(roomc);
	return 0;
}

void billPayment()
{
	struct studentInfo
	{
		char studentID[10];
		char firstName[25];
		char lastName[25];
		char studentGender[10];
		char phoneNumber[20];
		char emailAddress[30];
		struct roomInfo
		{
			int roomNumber;
			char blockNumber[3];
			int weeksBooked;
			char mealsSub[4];
			char gymSub[4];
			int amountPayable;
			int totalCollected;
			int outstanding;
		}RI;
	}SI;
	char idRead4[10];
	int idCheck4, partPay;
	system("CLS");
	printf("Payment\n_____________________________\n");
	printf("Enter student ID: ");
	scanf("%s", &idRead4);
	FILE* idc1;
	idc1 = fopen("Student Record.txt", "r");
	while (!feof(idc1))
	{
		fread(&SI, sizeof(SI), 1, idc1);
		if (strcmp(idRead4, SI.studentID) == 0)
		{
			fclose(idc1);
			idCheck4 = 1;
			goto point;
		}
	}
	fclose(idc1);
	idCheck4 = 0;
point:
	if (idCheck4 == 0)
		printf("\nInvalid ID entered\n");
	else if (idCheck4 == 1)
	{
		printf("Outstanding: RM %d\nHow much will the student pay: ", SI.RI.outstanding);
		scanf("%d", &partPay);
		if (partPay > SI.RI.outstanding || partPay < 0)
		{
			printf("\nInvalid Amount!\n");
			system("PAUSE");
			system("CLS");
			billPayment();
		}
		else
		{
			SI.RI.outstanding = SI.RI.outstanding - partPay;
			SI.RI.totalCollected = SI.RI.totalCollected + partPay;
			removeStudent(idRead4);
			system("PAUSE");
			FILE* updateInfo;
			updateInfo = fopen("Student Record.txt", "a+");
			fwrite(&SI, sizeof(SI), 1, updateInfo);
			fclose(updateInfo);
			idCheck(idRead4);
			printInfo();
		}
	}
	system("PAUSE");
	system("CLS");
	mainMenu();
}

void checkAvailability()
{
	int roomC;
	system("CLS");
	printf("Check Availability\n________________________________\n");
	printf("1. Check by Block\n");
	printf("2. Check by room\n");
	printf("Enter your choice: ");
	switch (getche())
	{
	case '1':
		checkByBlock();
		break;
	case '2':
		roomC = checkByRoom();
		if (roomC == 1)
			printf("\nThis room is not available\n");
		else if (roomC == 0)
			printf("\nThis room is available\n");
		system("PAUSE");
		system("CLS");
		mainMenu();
		break;
	default:
		printf("Invalid choice!\n");
		system("PAUSE");
		system("CLS");
		checkAvailability();
	}
}

void checkByBlock()
{
	int i, j;
	char fileName1[13] = "", cblock[3], charRead[12];
	system("CLS");
	printf("Check by Block\n__________________________________\n");
	printf("Enter the block number (A1, B1, B2, B4): ");
	scanf("%s", &cblock);
	sprintf(fileName1, "Block %s.txt", cblock);
	FILE* blockAva;
	blockAva = fopen(fileName1, "r");
	fgets(charRead, 12, blockAva);
	fgets(charRead, 12, blockAva);
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 5; j++)
		{
			fscanf(blockAva, "%s", charRead);
			printf("%s ", charRead);
			fscanf(blockAva, "%s", charRead);
			if (i < 2 && (cblock[1] == '1' || cblock[1] == '2'))
				printf("%s\t\t", charRead);
			else if (i > 1 && cblock[1] == '1')
				printf("%s\t\t", charRead);
			else
				printf("%s\t", charRead);
		}
		printf("\n");
	}
	printf("\n\nT: Full\nF: Empty\n");
	fclose(blockAva);
	system("PAUSE");
	system("CLS");
	mainMenu();
}

int checkByRoom()
{
	char roomNumber[10], fileName2[13], chRead[10], avaSymbol[5];
	int i;
	system("CLS");
	printf("Check by Room\n______________________________\n");
	printf("Enter room number (EG: A1-1): ");
	scanf("%s", &roomNumber);
	if (roomNumber[0] == 'A' && roomNumber[1] == '1')
	{
		strcpy(fileName2, "Block A1.txt");
		strcpy(avaSymbol, "F");
	}
	else if (roomNumber[0] == 'B' && roomNumber[1] == '1')
	{
		strcpy(fileName2, "Block B1.txt");
		strcpy(avaSymbol, "F");
	}
	else if (roomNumber[0] == 'B' && roomNumber[1] == '2')
	{
		strcpy(fileName2, "Block B2.txt");
		strcpy(avaSymbol, "FF");
	}
	else if (roomNumber[0] == 'B' && roomNumber[1] == '4')
	{
		strcpy(fileName2, "Block B4.txt");
		strcpy(avaSymbol, "FFFF");
	}
	else
	{
		printf("\nInvalid Entry!\n");
		system("PAUSE");
		system("CLS");
		mainMenu();
	}
	FILE* roomAva;
	roomAva = fopen(fileName2, "r");
	for (i = 0; i < 1000; i++)
	{
		fscanf(roomAva, "%s", &chRead);
		if (strcmp(chRead, avaSymbol) == 0)
		{
			fscanf(roomAva, "%s", &chRead);
			if (strcmp(chRead, roomNumber) == 0)
			{
				fclose(roomAva);
				return 0;
			}
		}
	}
	fclose(roomAva);
	return 1;
}

void checkPayment()
{
	system("CLS");
	printf("Analytics\n_____________________\n");
	printf("1. Check invoices from each block\n");
	printf("2. Check overall invoice data\n");
	printf("3. Check a student\'s invoice data\n");
	printf("Enter your choice: ");
	char blockInv[3];
	switch (getche())
	{
	case '1':
		system("CLS");
		printf("Enter block number (EG: A1): ");
		scanf("%s", &blockInv);
		overallInvoice(blockInv);
		break;
	case '2':
		overallInvoice("ALL");
		break;
	case '3':
		studentInvoiceCheck();
		break;
	default:
		printf("\nInvalid choice!\n");
		system("PAUSE");
		system("CLS");
		checkPayment();
	}
}

void overallInvoice(char cc[3])
{
	int totalAmountPayable = 0, totalTotalCollected = 0, totalOutstanding = 0;
	FILE* readAllInvoice;
	readAllInvoice = fopen("Student Record.txt", "r");
	while (!feof(readAllInvoice))
	{
		fread(&SI, sizeof(SI), 1, readAllInvoice);
		if ((stricmp(SI.RI.blockNumber, cc) == 0) || (strcmp("ALL", cc) == 0))
		{
			totalAmountPayable += SI.RI.amountPayable;
			totalTotalCollected += SI.RI.totalCollected;
			totalOutstanding += SI.RI.outstanding;
		}
	}
	fclose(readAllInvoice);
	printf("\nOveall hostel data\n");
	printf("\nTotal amount payable = RM %d", totalAmountPayable);
	printf("\nTotal amount collected = RM %d", totalTotalCollected);
	printf("\nTotal amount outstanding = RM %d", totalOutstanding);
	puts("");
	system("PAUSE");
	system("CLS");
	mainMenu();
}

void studentInvoiceCheck()
{
	char idRead5[10];
	int checkId5;
	system("CLS");
	printf("Invoice check\n____________________________\n");
	printf("Enter student ID: ");
	scanf("%s", &idRead5);
	checkId5 = idCheck(idRead5);
	if (checkId5 == 0)
	{
		printf("\nInvalid ID\n");
		system("PAUSE");
		system("CLS");
		studentInvoiceCheck();
	}
	else if (checkId5 == 1)
	{
		printf("\n_______________________________________________________");
		printf("\nStudent ID: %s", SI.studentID);
		printf("\nStudent Name: %s %s", SI.firstName, SI.lastName);
		printf("\nAmount Payable: RM %d", SI.RI.amountPayable);
		printf("\nTotal Collected: RM %d", SI.RI.totalCollected);
		printf("\nOutstanding: RM %d", SI.RI.outstanding);
		puts("");
		system("PAUSE");
		system("CLS");
		mainMenu();
	}
}
