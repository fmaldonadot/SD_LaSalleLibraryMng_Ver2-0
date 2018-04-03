/********************************* SD_LaSalleLibraryMng.cpp *****************************************************/
// Version: 2.0
// Nom: Francisco Maldonado
// Date: 07/11/2017
// Description:  Application to manage the inventory of bokks in a library
// 
// Objetives:
//	1 - Add a new book,
//	2 - Search for a book by isbn,
//	3 - Search for a book by published date(in descending order),
//	4 - Show all books from the library,
//	5 - Sort books by isbn,
//	6 - Sort books by published date(in descending order),
//	7 - Move the oldest books in another room(2014 and less)
//	8 - Show old books
//  9 - Load book list from a file.
//  10 - Save book's list to a file.
//	11 - Exit the application
// 
// Version historic:
// 1. Ver 1.0 Creation.  Create a late binding array 
// 2. Ver 1.1 Implementing Sequentials Sorting Functions. Adding Swap Function with template
// 3. Ver 2.0 Changing operation to using vectors.  Adding File Handler functions Load/Save file options
//
//
//
/************************************************************************************************************/

#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
#include <assert.h>
#include <cmath>
#include <vector>   // Ver 2.0
#include <fstream>  // Ver 2.0
#include <ios>      // Ver 2.0


using namespace std;

// Struct definition

enum EnumCategory
{
	Drama = 1,
	Science_Fiction,
	Suspense,
	Thriller,
	Comedy,
	Others
};

enum EnumBookStatus
{
	Available = 1,
	Not_Available
};

struct Date
{
	int day;
	int month;
	int year;
};

struct Book
{
	string isbn;
	string title;
	string autor;
	string publisher;
	EnumCategory category;
	EnumBookStatus status;
	Date published_date;
	Date retirement_date;
	Date expected_date;
	Date return_date;	
	
};


// Ver 2.0
/****************** FUNCTORS ******************/
// For Sorting Books functions
long AssignValue(string str);

struct isbnComp
{
	bool operator ()(Book book1 ,Book book2)
	{
		return AssignValue(book1.isbn) > AssignValue(book2.isbn);
	}
}isbnDesc;
struct DateComp
{
	bool operator ()(Book book1, Book book2)
	{
		string t1 = to_string(book1.published_date.year) + "/" + to_string(book1.published_date.month) + "/" +
			to_string(book1.published_date.day);
		string t2 = to_string(book2.published_date.year) + "/" + to_string(book2.published_date.month) + "/" +
			to_string(book2.published_date.day);

		return (strcmp(t1.c_str(), t2.c_str()) > 0);;
	}
}DateDesc;

/*************************************************************/
/*************************** Prototypes **********************/
/*************************************************************/

/*********************** ONLY FOR TESTING ********************/
int FillList(vector<Book> &); // Ver 2.0
/*************************************************************/

// Adding Books function
void AddBook(vector<Book> &); //Ver 2.0

// To verify if ID is Unique
bool confirmID(string , vector<Book>);  //Ver 2.0

// Displaying List function
void DisplayList(vector<Book>); //Ver 2.0

//Ver 2.0 Searching by published date											   
int search_books(vector<Book>, vector<Book> &, Date);

// Transfering old Books Function.
void transferOldBooks(vector<Book> &,vector<Book> &, int ); // Ver 2.0

//Ver 2.0
// File Handler Functions
void writeToFile(char *, vector<Book>);
void readFromFile(char *, vector<Book> &);

// Ver 2.0
// To prepare the string to send to a file (overloaded function)
string ToString(Book);
string ToString(Date);

// Overloaded opperators
ostream & operator << (ostream &, Book &);
ostream & operator << (ostream &, EnumCategory &);
ostream & operator << (ostream &, EnumBookStatus &);
ostream & operator << (ostream &, Date &);

istream & operator >> (istream &, Book &);
istream & operator >> (istream &, Date &);
istream & operator >> (istream &, EnumCategory &);
istream & operator >> (istream &, EnumBookStatus &);

bool operator == (Book &, string); // Ver 2.0 To use to search a book by ISBN
bool operator == (Book &, Date); // Ver 2.0 To use to search a book by Published Date

EnumCategory get_Category(string strcat);   //Ver 2.0 to get the Enum value
EnumBookStatus get_Status(string strstat);  //Ver 2.0 to get the Enum value

int main()
{
	char * bfile = "book_list.txt";
	char * old_bfile = "oldbooks.txt";
	vector<Book>::iterator book_temp_info;
	Date searchdate;
	int option;
	int list_size = 0;
	int curr_index = 0;
	int old_books_size = 0;
	int temp = 0;
	string strtemp = "";

	// Ver 2.0 Vectors
	vector<Book> book_list; // Vector of all of books
	vector<Book> old_books; // Vector of old books
	vector<Book> temp_list; // Temporal vector of books, for searching purposes

	cout << "********************** VER 1.1 ****************************\n\n";
	cout << "***********************************************************\n";
	cout << "******* LaSalle Books Library Management System ***********\n";
	cout << "***********************************************************\n\n";
	
	/*********************** ONLY FOR TESTING ********************/
	//curr_index = FillList(book_list);
	/*************************************************************/

	while (true)
	{
		system("cls");
		cout << "********************** VER 2.0 ****************************\n\n";
		cout << "***********************************************************\n";
		cout << "******* LaSalle Books Library Management System ***********\n";
		cout << "***********************************************************\n\n";
/*DONE*/cout << "1. Add new Book.\n";
/*DONE*/cout << "2. Search Book by ISBN.\n";
/*DONE*/cout << "3. Search Books by published date. \n";
/*DONE*/cout << "4. Show all Books. \n";
/*DONE*/cout << "5. Sort Books by ISBN. \n";
/*DONE*/cout << "6. Sort Books by published date. \n";
/*DONE*/cout << "7. Move older Books to another room. \n";
/*DONE*/cout << "8. Show old Books. \n";
/*DONE*/cout << "9. Load book list from a file. \n";
/*DONE*/cout << "10. Save book's list to a file. \n";
/*DONE*/cout << "11. Exit.\n";
		cout << "Choise an option: ";
		cin >> option;

		switch (option)
		{
		case 1:
			system("cls");
			cout << "+++++++++++++++++++++ Add new Book +++++++++++++++++++++++\n\n";
			AddBook(book_list);
			
			cout << "+++++++++++++++++++++ Book Added! +++++++++++++++++++++++\n\n";			

			system("pause");
			break;

		case 2:
			system("cls");
			cout << "-------------------- Search Book by ISBN -----------------\n\n";
			cout << "Enter the book's ISBN: ";
			cin >> strtemp;
			
			book_temp_info = find(book_list.begin(), book_list.end(), strtemp);
			
			if (book_temp_info != book_list.end())
			{
				cout << "ISBN" << "\t" << "Title" << "\t" << "Autor" << "\t" << "Publisher" << "\t" <<
					"Category" << "\t" << "Status" << "\t" << "Published's Date" << "\t" <<
					"Retirement's Date" << "\t" << "Expected return's date" << "\t" << "Return Date";
				cout << "\n============================================================================================================================================================\n";
				cout << *book_temp_info;
			}
			else
				cout << "ISBN doesn't exist\n";

			system("pause");
			break;

		case 3:
			system("cls");
			cout << "-------------------- Search Books by published date -----------------\n\n";
			cout << "Enter a day: ";
			cin >> searchdate.day;
			cout << "Enter a month: ";
			cin >> searchdate.month;
			cout << "Enter a year: ";
			cin >> searchdate.year;

			temp_list.clear();  // Cleaning vector from old searchs
			temp = search_books(book_list, temp_list, searchdate);
			if (temp > 0)
			{
				//sort(temp_list, temp_list + temp, sort_booksISBN);
				DisplayList(temp_list);
			}
			else
				cout << "There are not books with this published date\n";

			system("pause");
			break;

		case 4:
			system("cls");
			cout << "##################### Show all Books #################\n\n";
			DisplayList(book_list);

			system("pause");
			break;
		case 5:
			system("cls");
			cout << "##################### Sort books by isbn #################\n\n";
			sort(book_list.begin(), book_list.end(), isbnDesc);

			DisplayList(book_list);

			system("pause");
			break;
		case 6:
			system("cls");
			cout << "######### Sort books by published date (in descending order) ########\n\n";
			sort(book_list.begin(), book_list.end(), DateDesc);

			DisplayList(book_list);

			system("pause");
			break;

		case 7:
			system("cls");
			cout << "######### Move the oldest books in another room (2014 and less) ########\n\n";

			transferOldBooks(book_list, old_books, 2014);

			if (old_books.size() > 0)
				cout << "Old Books moved to other list\n";
			else 
				cout << "There are no books older than year 2014\n";

			system("pause");

			break;
		case 8:
			system("cls");
			cout << "######### Show old books ########\n\n";
			if (old_books.size() > 0)
				DisplayList(old_books);
			else
				cout << "There are no books older than year 2014\n";

			system("pause");

			break;
		case 9:
			system("cls");

			// Clean all the list
			book_list.clear();
			old_books.clear();

			cout << "Loading Books from " << bfile << "\n\n";
			readFromFile(bfile, book_list);

			cout << "Loading Old Books from " << old_bfile << "\n\n";
			readFromFile(old_bfile, old_books);

			cout << "Books Loaded \n\n";

			system("pause");
			
			break;
		case 10:
			system("cls");
			cout << "Saving Books in " << bfile << "\n\n";
			writeToFile(bfile, book_list);

			cout << "Saving Old Books in " << old_bfile << "\n\n";
			writeToFile(old_bfile, old_books);

			cout << "Books Saved \n\n";
			
			system("pause");
			break;
		case 11:
			// Exit Application
			return 0;
			break;

		default:
			cout << "Invalid option. \n";
			system("pause");
			break;
		}

	}
	return 0;
}


/******************************* FUNCTIONS IMPLEMENTATTIONS *************************/
// Assure unique ISBN
// Receive isbn , book's list vector
// Return true if isbn is unique and false if exist it
bool confirmID(string isbn, vector<Book> bookarr) // Ver 2.0
{
	
	for (int i = 0; i < (int)bookarr.size(); i++)
	{
		if (isbn == bookarr[i].isbn)
			return false;
	}
	return true;

}

/*************************** Overloaded opperators ************************************/
ostream & operator << (ostream &out_stream, Book &item)
{
	out_stream << item.isbn << "\t" << item.title << "\t" << item.autor << "\t" <<
		item.publisher << "\t" << item.category << "\t" << item.status << "\t" <<
		item.published_date << "\t" << item.retirement_date << "\t" <<
		item.expected_date << "\t" << item.return_date << endl;

	return out_stream;
}
ostream & operator << (ostream &out_stream, EnumCategory &cat)
{
	switch (cat)
	{
	case 1:
		out_stream << "Drama";
		break;
	case 2:
		out_stream << "Science Fiction";
		break;
	case 3:
		out_stream << "Suspense";
		break;
	case 4:
		out_stream << "Thriller";
		break;
	case 5:
		out_stream << "Comedy";
		break;
	case 6:
		out_stream << "Others";
		break;

	}
	return out_stream;
}
ostream & operator << (ostream &out_stream, EnumBookStatus &stat)
{
	switch (stat)
	{
	case 1:
		out_stream << "Available";
		break;
	case 2:
		out_stream << "Not Available";
		break;

	}
	return out_stream;
}
ostream & operator << (ostream &out_stream, Date &bdate)
{
	out_stream << bdate.day << "/" << bdate.month << "/" << bdate.year;

	return out_stream;
}

istream & operator >> (istream &in_stream, Date &bdate)
{
	cout << "Enter the day: ";
	in_stream >> bdate.day;

	cout << "Enter the month: ";
	in_stream >> bdate.month;

	cout << "Enter the year: ";
	in_stream >> bdate.year;


	return in_stream;
}
istream & operator >> (istream &in_stream, EnumCategory &bookcat)
{
	int option;
	while (true)
	{
		cout << "Category: \n";
		cout << "1. Drama \n";
		cout << "2. Science Fiction \n";
		cout << "3. Suspense \n";
		cout << "4. Thriller \n";
		cout << "5. Comedy \n";
		cout << "6. Others \n";
		cout << "Choose an option: ";

		in_stream >> option;

		if ((option >= 1) && (option <= 6))
		{
			switch (option)
			{
			case 1:
				bookcat = EnumCategory{ Drama };
				break;
			case 2:
				bookcat = EnumCategory{ Science_Fiction };
				break;
			case 3:
				bookcat = EnumCategory{ Suspense };
				break;
			case 4:
				bookcat = EnumCategory{ Thriller };
				break;
			case 5:
				bookcat = EnumCategory{ Comedy };
				break;
			case 6:
				bookcat = EnumCategory{ Others };
				break;
			}

			return in_stream;
		}
		else
			cout << "Invalid option...Try again\n";

	}

}
istream & operator >> (istream &in_stream, EnumBookStatus &bstatus)
{
	int option;
	while (true)
	{
		cout << "Category: \n";
		cout << "1. Available \n";
		cout << "2. Not Available \n";

		cout << "Choose an option: ";

		in_stream >> option;;

		if ((option >= 1) && (option <= 2))
		{
			switch (option)
			{
			case 1:
				bstatus = EnumBookStatus{ Available };
				break;
			case 2:
				bstatus = EnumBookStatus{ Not_Available };
				break;
			}
			return in_stream;
		}
		else
			cout << "Invalid option...Try again\n";

	}

}
istream & operator >> (istream &in_stream, Book &item)
{

	cout << "Enter the tittle of the book: ";
	getline(in_stream, item.title);

	cout << "Enter the autor: ";
	getline(in_stream, item.autor);

	cout << "Enter the publisher: ";
	getline(in_stream, item.publisher);

	cout << "Enter the book's category: ";
	in_stream >> item.category;
	cin.ignore();

	cout << "Enter the book's status: ";
	in_stream >> item.status;
	cin.ignore();

	cout << "Enter the book's published's date: ";
	in_stream >> item.published_date;
	cin.ignore();

	cout << "Enter the book's retirement's date: ";
	in_stream >> item.retirement_date;
	cin.ignore();

	cout << "Enter the book's expected's date: ";
	in_stream >> item.expected_date;
	cin.ignore();

	cout << "Enter the book's return's date: ";
	in_stream >> item.return_date;
	cin.ignore();

	return in_stream;
}

// Ver 2.0 To use to search a book
bool operator == (Book &b1, string isbn) 
{
	return b1.isbn == isbn;
}
// Ver 2.0 To use to search a book by Published Date
bool operator == (Book &b1, Date pubdate) 
{
	return ((b1.published_date.day == pubdate.day)&
		    (b1.published_date.month == pubdate.month)&
			(b1.published_date.year == pubdate.year));
}

/************************ Adding Books function.  **********************/
// Ver 2.0 Receive the vector of books to add the new book).
// Return the index to next available position to add in the list.
void AddBook(vector<Book> &bookarr) //Ver 2.0
{
	Book tbook;
	bool flag = false;
	string isbn;
	do
	{
			cout << "Enter ISBN: ";
			cin >> isbn;
			cin.ignore();
			flag = confirmID(isbn, bookarr);

			if (flag == false)
				cout << "ISBN exist in the list...\n\n";

	} while (!flag);

	tbook.isbn = isbn;
	cin >> tbook;
	bookarr.push_back(tbook);

}

/*********************** Displaying List function *************************/
// Receive book list and size of the list to display it;
void DisplayList(vector<Book> bookarr) //Ver 2.0
{
	cout << "************************** BOOK'S LIST ***************************\n";
	cout << "ISBN" << "\t" << "Title" << "\t" << "Autor" << "\t" << "Publisher" << "\t" <<
		"Category" << "\t" << "Status" << "\t" << "Published's Date" << "\t" <<
		"Retirement's Date" << "\t" << "Expected return's date" << "\t" << "Return Date";
	cout << "\n============================================================================================================================================================\n";
	for (int i = 0; i < (int)bookarr.size(); i++)
	{
		cout << bookarr[i];
	}
}

/******************** Searching Books functions ********************/
//Ver 2.0 Searching by published date
// Searching by Published Date
// Receive the vector of books, the vector of searched books to fill
// and the date of refference to search 
// Returns the quantity of books finded
int search_books(vector<Book> list, vector<Book> &searched, Date searchdate) 
{
	int i = 0;
	for(vector<Book>::iterator itr = list.begin() ; itr != list.end() ; itr++)
	{
		if (*itr == searchdate)
		{
			searched.push_back(*itr);
			i++;
		}
	}
	return i;
}

// Transfering old Books Function.
// Receive List of Books , list of olds books , max old year.
// Returns the new size of old book's list
void transferOldBooks(vector<Book> &bookarr, vector<Book> &oldarr, int year) // Ver 2.0
{
	vector<Book>::iterator itr = bookarr.begin();
	while(itr != bookarr.end())
	{
		if (itr->published_date.year <= year)
		{
			oldarr.push_back(*itr);
			itr = bookarr.erase(itr);
		}
		else
			itr++;
	}

}

// Assigning a number value to a string
// Receive a string to calcule a value
// Return a number value as result to add all number values of each char of the string
long AssignValue(string str)
{
	long value = 0;
	long temp = str.size() - 1;
	for (int i = 0; i < (int)str.size(); i++)
	{
		value = value + (long)str[i] * pow(10, temp);
		temp--;
	}
	value = value + str.size();

	return value;
}

// Ver 2.0
// To prepare the string to send to a file
string ToString(Book book)
{
	return book.isbn + "|" + book.title + "|" + book.autor + "|" + book.publisher + "|" + 
			to_string(book.category)+ "|" + to_string(book.status) + "|" + 
			ToString(book.published_date) + "|" + ToString(book.retirement_date) + "|" +
		    ToString(book.expected_date) + "|" + ToString(book.return_date);

}

string ToString(Date date)
{
	
	return to_string(date.day) + "|" + to_string(date.month) + "|" + to_string(date.year); 

}

// Ver 2.0  To assign Enum's values from a string
EnumCategory get_Category(string strcat)
{
	if (strcat == "1")
		return EnumCategory{ Drama };
	else if (strcat == "2")
		return EnumCategory{ Science_Fiction };
	else if (strcat == "3")
		return EnumCategory{ Suspense };
	else if (strcat == "4")
		return EnumCategory{ Thriller };
	else if (strcat == "5")
		return EnumCategory{ Comedy };
	else if (strcat == "6")
		return EnumCategory{ Others };		
}
EnumBookStatus get_Status(string strstat)
{
	if (strstat == "1")
		return EnumBookStatus{ Available };
	else if (strstat == "2")
		return EnumBookStatus{ Not_Available };
	
}



//Ver 2.0
/************************ File Handler Functions ******************/
// Write the Books List in a file
void writeToFile(char *filepath, vector<Book> list)
{
	ofstream outfile(filepath, ios::out );

	for (vector<Book> ::iterator itr = list.begin(); itr != list.end(); itr++)
	{
		outfile << ToString(*itr) << endl;
	}
}

// Read from a file and asign data to book's list
void readFromFile(char *filepath, vector<Book> &list)
{
	ifstream infile(filepath, ios::in);
	string line;
	Book sbook;

	if( infile.good() )
	{
		while (infile.peek() != EOF)
		{
			getline(infile, line , '|');
			sbook.isbn = line;

			getline(infile, line, '|');
			sbook.title = line;

			getline(infile, line, '|');
			sbook.autor = line;

			getline(infile, line, '|');
			sbook.publisher = line;

			getline(infile, line, '|');
			sbook.category = get_Category(line);

			getline(infile, line, '|');
			sbook.status = get_Status(line);
			
			getline(infile, line, '|');
			sbook.published_date.day = stoi(line);
			getline(infile, line, '|');
			sbook.published_date.month = stoi(line);
			getline(infile, line, '|');
			sbook.published_date.year = stoi(line);
			
			getline(infile, line, '|');
			sbook.retirement_date.day = stoi(line);
			getline(infile, line, '|');
			sbook.retirement_date.month = stoi(line);
			getline(infile, line, '|');
			sbook.retirement_date.year = stoi(line);

			getline(infile, line, '|');
			sbook.expected_date.day = stoi(line);
			getline(infile, line, '|');
			sbook.expected_date.month = stoi(line);
			getline(infile, line, '|');
			sbook.expected_date.year = stoi(line);

			getline(infile, line, '|');
			sbook.return_date.day = stoi(line);
			getline(infile, line, '|');
			sbook.return_date.month = stoi(line);
			getline(infile, line);
			sbook.return_date.year = stoi(line);

			list.push_back(sbook);
		}
	}
	else
		cout << "File not found \n";
	
}


/*********************** ONLY FOR TESTING ********************/
int FillList(vector<Book> &bookarr) // Ver 2.0
{
	bookarr.push_back({ "15-511" , "TITLE1" , "AUTOR1" , "PUBLISHER1" , EnumCategory{ Drama } , EnumBookStatus{ Available } ,
		Date{ 1 , 1 , 2013 } , Date{ 5 , 5 , 2000 } , Date{ 8 , 6 , 2017 } , Date{ 8 , 12 , 2000 } });
	bookarr.push_back({ "15-2113" , "TITLE2" , "AUTOR2" , "PUBLISHER2" , EnumCategory{ Comedy } , EnumBookStatus{ Available } ,
		Date{ 15 , 1 , 2017 } , Date{ 20 , 5 , 2010 } , Date{ 8 , 9 , 2012 } , Date{ 16 , 8 , 2010 } });
	bookarr.push_back({ "15-1611" , "TITLE3" , "AUTOR3" , "PUBLISHER3" , EnumCategory{ Thriller } , EnumBookStatus{ Not_Available } ,
		Date{ 22 , 1 , 2014 } , Date{ 15 , 9 , 2015 } , Date{ 20 , 9 , 2015 } , Date{ 20 , 9 , 16 } });
	bookarr.push_back({ "15-11199" , "TITLE4" , "AUTOR4" , "PUBLISHER4" , EnumCategory{ Comedy } , EnumBookStatus{ Not_Available } ,
		Date{ 25 , 1 , 2017 } , Date{ 31 , 9 , 2000 } , Date{ 20 , 12 , 2014 } , Date{ 20 , 9 , 16 } });
	bookarr.push_back({ "15-1511" , "TITLE5" , "AUTOR5" , "PUBLISHER5" , EnumCategory{ Comedy } , EnumBookStatus{ Not_Available } ,
		Date{ 25 , 1 , 2017 } , Date{ 31 , 9 , 2000 } , Date{ 20 , 12 , 2014 } , Date{ 20 , 9 , 16 } });

	return 5;
}