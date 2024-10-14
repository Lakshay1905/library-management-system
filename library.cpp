#include <iostream>
#include <unordered_map>
#include <string>
#include <ctime>
#include <iomanip>
#include <queue>

using namespace std;

struct TreeNode
{
    string title;
    string author;
    bool isIssued;
    TreeNode *left;
    TreeNode *right;

    TreeNode(string t, string a) : title(t), author(a), isIssued(false), left(nullptr), right(nullptr) {}
};

class User
{
public:
    string name;
    unordered_map<string, time_t> issuedBooks;

    User() : name("") {}
    User(string name) : name(name) {}
};

class Library
{
private:
    TreeNode *root;
    unordered_map<string, User> users;
    queue<pair<string, time_t>> returnQueue;

    void insert(TreeNode *&node, const string &title, const string &author)
    {
        if (!node)
        {
            node = new TreeNode(title, author);
        }
        else if (title < node->title)
        {
            insert(node->left, title, author);
        }
        else
        {
            insert(node->right, title, author);
        }
    }

    TreeNode *search(TreeNode *node, const string &title)
    {
        if (!node || node->title == title)
        {
            return node;
        }
        if (title < node->title)
        {
            return search(node->left, title);
        }
        return search(node->right, title);
    }

public:
    Library() : root(nullptr) {}

    void addBook(const string &title, const string &author)
    {
        insert(root, title, author);
    }

    void registerUser(const string &userName)
    {
        users.emplace(userName, User(userName));
    }

    bool issueBook(const string &userName, const string &bookTitle)
    {
        TreeNode *bookNode = search(root, bookTitle);
        if (bookNode && !bookNode->isIssued)
        {
            bookNode->isIssued = true;
            time_t dueDate = time(nullptr) + 7 * 24 * 60 * 60;
            users[userName].issuedBooks[bookTitle] = dueDate;
            returnQueue.push({bookTitle, dueDate});
            return true;
        }
        return false;
    }

    void returnBook(const string &userName, const string &bookTitle)
    {
        if (users.find(userName) != users.end())
        {
            auto &issuedBooks = users[userName].issuedBooks;
            if (issuedBooks.find(bookTitle) != issuedBooks.end())
            {
                issuedBooks.erase(bookTitle);
                TreeNode *bookNode = search(root, bookTitle);
                if (bookNode)
                {
                    bookNode->isIssued = false;
                    returnQueue.push({bookTitle, 0});
                    return;
                }
            }
        }
    }

    void displayIssuedBooks(const string &userName)
    {
        if (users.find(userName) != users.end())
        {
            for (const auto &pair : users[userName].issuedBooks)
            {
                cout << pair.first << ", Due Date: "
                     << put_time(localtime(&pair.second), "%Y-%m-%d") << "\n";
            }
        }
        else
        {
            cout << "No books issued for user " << userName << ".\n";
        }
    }

    void displayReturnQueue()
    {
        while (!returnQueue.empty())
        {
            auto item = returnQueue.front();
            returnQueue.pop();
            if (item.second != 0)
            {
                cout << item.first << ", Due Date: "
                     << put_time(localtime(&item.second), "%Y-%m-%d") << "\n";
            }
            else
            {
                cout << item.first << " has been returned.\n";
            }
        }
    }

    ~Library()
    {
        // Destructor for cleaning up the BST would go here
    }
};

int main()
{
    Library library;
    int choice;
    string title, author, userName;

    while (true)
    {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Register User\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Display Issued Books\n";
        cout << "6. Display Return Queue\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter book title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter book author: ";
            getline(cin, author);
            library.addBook(title, author);
            break;
        case 2:
            cout << "Enter user name: ";
            cin.ignore();
            getline(cin, userName);
            library.registerUser(userName);
            break;
        case 3:
            cout << "Enter user name: ";
            cin.ignore();
            getline(cin, userName);
            cout << "Enter book title: ";
            getline(cin, title);
            if (library.issueBook(userName, title))
            {
                cout << "Book issued successfully.\n";
            }
            else
            {
                cout << "Book is not available or already issued.\n";
            }
            break;
        case 4:
            cout << "Enter user name: ";
            cin.ignore();
            getline(cin, userName);
            cout << "Enter book title: ";
            getline(cin, title);
            library.returnBook(userName, title);
            cout << "Book returned successfully.\n";
            break;
        case 5:
            cout << "Enter user name: ";
            cin.ignore();
            getline(cin, userName);
            library.displayIssuedBooks(userName);
            break;
        case 6:
            library.displayReturnQueue();
            break;
        case 7:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
