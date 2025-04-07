#include <bits/stdc++.h>
using namespace std;
class Member;
class Library;
class Book
{
private:
    string isbn;
    int copiesAvailable;
    int totalCopies;

public:
    string title;
    string author;
    string get_isbn()
    {
        return isbn;
    }
    int get_copiesAvailable()
    {
        return copiesAvailable;
    }
    int get_totalCopies()
    {
        return totalCopies;
    }
    Book()
    {
        title = "UnknownTitle";
        author = "UnknownAuthor";
        isbn = "ISBN";
        copiesAvailable = 0;
        totalCopies = 5;
    }
    Book(string t, string a, string isbn_par, int ca, int tc)
    {
        title = t;
        author = a;
        isbn = isbn_par;
        copiesAvailable = ca;
        totalCopies = tc;
    }
    Book(Book &b1, string isbn_new)
    {
        title = b1.title;
        author = b1.author;
        isbn = isbn_new;
        copiesAvailable = b1.get_copiesAvailable();
        totalCopies = b1.get_totalCopies();
    }
    bool borrowBook()
    {
        if (copiesAvailable > 0)
        {
            copiesAvailable -= 1;
            return true;
        }
        else
        {
            cout << "Invalid request! Copy of book not available" << endl;
            return false;
        }
    }
    bool checkborrow()
    {
        if (copiesAvailable > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool returnBook()
    {
        if (copiesAvailable < totalCopies)
        {
            copiesAvailable += 1;
            return true;
        }
        else
        {
            cout << "Invalid request! Copy of book exceeds total copies" << endl;
            return false;
        }
    }
    bool checkreturn()
    {
        if (copiesAvailable < totalCopies)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void printDetails()
    {
        cout << title << " " << author << endl;
    }
    void updateCopies(int count)
    {
        if (totalCopies + count < 0 || copiesAvailable + count < 0)
        {
            cout << "Invalid request! Count becomes negative" << endl;
        }
        else
        {
            totalCopies += count;
            copiesAvailable += count;
        }
    }
    friend class Member;
    friend class Library;
};
class Member
{
private:
    string memberID;
    map<string, int> borrowedBooks;
    int borrowLimit;

public:
    string name;
    int no_of_borrowed;
    Member() {} // default Constructor with no arguments required for resizing of vector of objects
    Member(string mID, string name_par, int bor_lim)
    {
        memberID = mID;
        name = name_par;
        borrowLimit = bor_lim;
        no_of_borrowed = 0;
    }
    Member(string mID, string name_par)
    {
        memberID = mID;
        name = name_par;
        borrowLimit = 3;
        no_of_borrowed = 0;
    }
    string get_id()
    {
        return memberID;
    }
    bool borrowBook(string isbn)
    {
        if (no_of_borrowed >= borrowLimit)
        {
            cout << "Invalid request! Borrow limit exceeded" << endl;
            return false;
        }
        else
        {
            borrowedBooks[isbn]++;
            no_of_borrowed++;
            return true;
        }
    }
    bool checkborrow()
    {
        if (no_of_borrowed >= borrowLimit)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    bool returnBook(string isbn)
    {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); it++)
        {
            if (it->first == isbn)
            {
                it->second--;
                if (it->second == 0)
                {
                    borrowedBooks.erase(it);
                }
                no_of_borrowed--;
                return true;
            }
        }
        cout << "Invalid request! Book not borrowed" << endl;
        return false;
    }
    bool checkreturn(string isbn)
    {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); it++)
        {
            if (it->first == isbn)
            {
                return true;
            }
        }
        return false;
    }
    void printDetails()
    {
        if (no_of_borrowed > 0)
        {
            for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); it++)
            {
                cout << get_id() << " " << name << " " << it->first << " " << it->second << endl;
            }
        }
    }
    friend class Library;
};
class Library
{
private:
    vector<Book> book_list;
    int bi = 0; // size or next empty index for book_list
    vector<Member> member_list;
    int mi = 0; // size or next empty index for member_list

public:
    unordered_map<string, Book *> book_map;
    unordered_map<string, Member *> member_map;
    Library()
    {
        book_list.resize(50);
        member_list.resize(150);
    }
    bool addBook(Book &b)
    {
        bool present = false;
        for (auto i = 0; i < bi; ++i)
        {
            if (book_list[i].get_isbn() == b.get_isbn())
            {
                present = true;
                break;
            }
        }
        if (!present)
        {
            book_list[bi] = b;
            book_map[b.get_isbn()] = &book_list[bi];
            bi++; // increase size
            return true;
        }
        else
        {
            cout << "Invalid request! Book with same isbn already exists" << endl;
            return false;
        }
    }
    bool registerMember(Member &m)
    {
        bool present = false;
        for (long long int i = 0; i < mi; ++i)
        {
            if (member_list[i].get_id() == m.get_id())
            {
                present = true;
                break;
            }
        }
        if (!present)
        {
            member_list[mi] = m;
            member_map[m.get_id()] = &member_list[mi];
            mi++; // increase size
            return true;
        }
        else
        {
            cout << "Invalid request! Member with same id already exists" << endl;
            return false;
        }
    }
    bool borrowBook(string memberID, string isbn)
    {
        bool present = false;
        for (long long int i = 0; i < bi; ++i)
        {
            if (book_list[i].get_isbn() == isbn)
            {
                present = true;
                break;
            }
        }
        if (!present)
        {
            cout << "Invalid request! Copy of book not available" << endl;
            return false;
        }
        bool is_available = (*book_map[isbn]).checkborrow();
        Member *m = member_map[memberID];
        bool is_borrowable = (*m).checkborrow();
        if (is_available)
        {
            // book can be borrowed from book pov
            if (is_borrowable)
            {
                // book can be borrowed from both pov
                (*book_map[isbn]).borrowBook();
                return (*m).borrowBook(isbn);
            }
            else
            {
                // book can't be borrowed from member pov
                return (*m).borrowBook(isbn);
            }
        }
        else
        {
            // book can't be borrowed from book pov
            return (*book_map[isbn]).borrowBook();
        }
    }
    bool returnBook(string memberID, string isbn)
    {
        Book *b = book_map[isbn];
        Member *m = member_map[memberID];
        bool is_returnable = (*b).checkreturn();
        bool is_borrowed = (*m).checkreturn(isbn);
        if (is_returnable)
        {
            // book can be returned from book pov
            if (is_borrowed)
            {
                // book can be returned from both pov
                (*b).returnBook();
                return (*m).returnBook(isbn);
            }
            else
            {
                // book can't be returned from member pov
                return (*m).returnBook(isbn);
            }
        }
        else
        {
            // book can't be returned from book pov
            return (*b).returnBook();
        }
    }
    void printLibraryDetails()
    {
        for (auto i = 0; i < bi; ++i)
        {
            cout << book_list[i].title << " " << book_list[i].author << " " << book_list[i].get_copiesAvailable() << endl;
        }
        for (auto i = 0; i < mi; ++i)
        {
            cout << member_list[i].get_id() << " " << member_list[i].name << endl;
        }
    }
};
int main()
{
    Library lib;
    while (1)
    {
        string cmd;
        cin >> cmd;
        if (cmd == "Book")
        {
            string s;
            cin >> s;
            if (s == "ExistingBook")
            {
                string isbn1, isbn2;
                cin >> isbn1 >> isbn2;
                Book b(*(lib.book_map[isbn1]), isbn2);
                lib.addBook(b);
            }
            else if (s == "None")
            {
                Book b;
                lib.addBook(b);
            }
            else
            {
                // string title=s;
                string author;
                cin >> author;
                string isbn;
                cin >> isbn;
                int ca, tc;
                cin >> ca >> tc;
                Book b(s, author, isbn, ca, tc);
                lib.addBook(b);
            }
        }
        else if (cmd == "UpdateCopiesCount")
        {
            string isbn;
            cin >> isbn;
            int cnt;
            cin >> cnt;
            (*lib.book_map[isbn]).updateCopies(cnt);
        }
        else if (cmd == "Member")
        {
            string s;
            cin >> s;
            if (s == "NoBorrowLimit")
            {
                string id;
                cin >> id;
                string name;
                cin >> name;
                Member m(id, name);
                lib.registerMember(m);
            }
            else
            {
                string name;
                cin >> name;
                int borrow_limit;
                cin >> borrow_limit;
                Member m(s, name, borrow_limit);
                lib.registerMember(m);
            }
        }
        else if (cmd == "Borrow")
        {
            string mID, isbn;
            cin >> mID >> isbn;
            lib.borrowBook(mID, isbn);
        }
        else if (cmd == "Return")
        {
            string mID, isbn;
            cin >> mID >> isbn;
            lib.returnBook(mID, isbn);
        }
        else if (cmd == "PrintBook")
        {
            string isbn;
            cin >> isbn;
            (*lib.book_map[isbn]).printDetails();
        }
        else if (cmd == "PrintMember")
        {
            string mID;
            cin >> mID;
            (*lib.member_map[mID]).printDetails();
        }
        else if (cmd == "PrintLibrary")
        {
            lib.printLibraryDetails();
        }
        else
        {
            return 0;
        }
    }
}
