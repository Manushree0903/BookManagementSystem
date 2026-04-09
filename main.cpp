/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

// -----------------------------
// Book Structure
// -----------------------------
struct Book {
    string isbn;
    string title;
    string author;
    string publisher;
    int year;
    int copies;
};

// -----------------------------
// AVL Tree Node
// -----------------------------
struct Node {
    Book data;
    Node* left;
    Node* right;
    int height;

    Node(Book b) {
        data = b;
        left = right = nullptr;
        height = 1;
    }
};

// -----------------------------
// AVL Tree Class
// -----------------------------
class AVLTree {
private:
    Node* root;

    // Get height
    int getHeight(Node* node) {
        return (node == nullptr) ? 0 : node->height;
    }

    // Get balance factor
    int getBalance(Node* node) {
        return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
    }

    // Right Rotation
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    // Left Rotation
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    // Insert node by ISBN
    Node* insertNode(Node* node, Book book) {
        if (node == nullptr)
            return new Node(book);

        if (book.isbn < node->data.isbn)
            node->left = insertNode(node->left, book);
        else if (book.isbn > node->data.isbn)
            node->right = insertNode(node->right, book);
        else {
            // Duplicate ISBN
            node->data.copies += book.copies;
            cout << "ISBN already exists. Copies updated.\n";
            return node;
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balance = getBalance(node);

        // Left Left
        if (balance > 1 && book.isbn < node->left->data.isbn)
            return rightRotate(node);

        // Right Right
        if (balance < -1 && book.isbn > node->right->data.isbn)
            return leftRotate(node);

        // Left Right
        if (balance > 1 && book.isbn > node->left->data.isbn) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left
        if (balance < -1 && book.isbn < node->right->data.isbn) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Minimum value node
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    // Delete by ISBN
    Node* deleteNode(Node* node, string isbn) {
        if (node == nullptr)
            return node;

        if (isbn < node->data.isbn)
            node->left = deleteNode(node->left, isbn);
        else if (isbn > node->data.isbn)
            node->right = deleteNode(node->right, isbn);
        else {
            // Node with one child or no child
            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = (node->left) ? node->left : node->right;

                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }

                delete temp;
            }
            // Node with two children
            else {
                Node* temp = minValueNode(node->right);
                node->data = temp->data;
                node->right = deleteNode(node->right, temp->data.isbn);
            }
        }

        if (node == nullptr)
            return node;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balance = getBalance(node);

        // Left Left
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        // Left Right
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right
        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        // Right Left
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Search by ISBN
    Node* searchByISBN(Node* node, string isbn) {
        if (node == nullptr || node->data.isbn == isbn)
            return node;

        if (isbn < node->data.isbn)
            return searchByISBN(node->left, isbn);

        return searchByISBN(node->right, isbn);
    }

    // Inorder display
    void inorder(Node* node) {
        if (node != nullptr) {
            inorder(node->left);
            printBook(node->data);
            inorder(node->right);
        }
    }

    // Search by title
    void searchByTitle(Node* node, const string& title, bool &found) {
        if (node == nullptr)
            return;

        searchByTitle(node->left, title, found);

        if (node->data.title == title) {
            printBook(node->data);
            found = true;
        }

        searchByTitle(node->right, title, found);
    }

    // Collect ISBNs of books with matching title
    void collectISBNsByTitle(Node* node, const string& title, vector<string>& isbns) {
        if (node == nullptr)
            return;

        collectISBNsByTitle(node->left, title, isbns);

        if (node->data.title == title) {
            isbns.push_back(node->data.isbn);
        }

        collectISBNsByTitle(node->right, title, isbns);
    }

    // Print one book
    void printBook(const Book& b) {
        cout << "-----------------------------------\n";
        cout << "ISBN      : " << b.isbn << "\n";
        cout << "Title     : " << b.title << "\n";
        cout << "Author    : " << b.author << "\n";
        cout << "Publisher : " << b.publisher << "\n";
        cout << "Year      : " << b.year << "\n";
        cout << "Copies    : " << b.copies << "\n";
    }

public:
    AVLTree() {
        root = nullptr;
    }

    // Insert book
    void insertBook(Book book) {
        root = insertNode(root, book);
        cout << "Book inserted successfully.\n";
    }

    // Delete by ISBN
    void deleteBookByISBN(string isbn) {
        if (searchByISBN(root, isbn) == nullptr) {
            cout << "Book with ISBN " << isbn << " not found.\n";
            return;
        }

        root = deleteNode(root, isbn);
        cout << "Book deleted successfully.\n";
    }

    // Delete by Title
    void deleteBookByTitle(string title) {
        vector<string> isbns;
        collectISBNsByTitle(root, title, isbns);

        if (isbns.empty()) {
            cout << "No book found with title: " << title << "\n";
            return;
        }

        cout << "\nBooks deleted with title \"" << title << "\":\n";
        for (string isbn : isbns) {
            Node* bookNode = searchByISBN(root, isbn);
            if (bookNode != nullptr) {
                printBook(bookNode->data);
                root = deleteNode(root, isbn);
            }
        }

        cout << "Deletion by title completed successfully.\n";
    }

    // Search by ISBN
    void findByISBN(string isbn) {
        Node* result = searchByISBN(root, isbn);

        if (result == nullptr) {
            cout << "Book not found.\n";
        } else {
            cout << "Book found:\n";
            printBook(result->data);
        }
    }

    // Search by Title
    void findByTitle(string title) {
        bool found = false;
        searchByTitle(root, title, found);

        if (!found) {
            cout << "No book found with title: " << title << "\n";
        }
    }

    // Display all books
    void displayBooks() {
        if (root == nullptr) {
            cout << "No books available in the library.\n";
            return;
        }

        cout << "\nBooks in sorted order by ISBN:\n";
        inorder(root);
    }
};

// -----------------------------
// Main Function
// -----------------------------
int main() {
    AVLTree library;
    int choice;

    do {
        cout << "\n========== Library Book Management ==========\n";
        cout << "1. Insert Book\n";
        cout << "2. Search Book by ISBN\n";
        cout << "3. Search Book by Title\n";
        cout << "4. Delete Book by ISBN\n";
        cout << "5. Delete Book by Title\n";
        cout << "6. Display All Books\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        cin.ignore();

        switch (choice) {
            case 1: {
                Book b;

                cout << "Enter ISBN: ";
                getline(cin, b.isbn);

                cout << "Enter Title: ";
                getline(cin, b.title);

                cout << "Enter Author: ";
                getline(cin, b.author);

                cout << "Enter Publisher: ";
                getline(cin, b.publisher);

                cout << "Enter Year: ";
                cin >> b.year;

                cout << "Enter Copies: ";
                cin >> b.copies;
                cin.ignore();

                library.insertBook(b);
                break;
            }

            case 2: {
                string isbn;
                cout << "Enter ISBN to search: ";
                getline(cin, isbn);
                library.findByISBN(isbn);
                break;
            }

            case 3: {
                string title;
                cout << "Enter Title to search: ";
                getline(cin, title);
                library.findByTitle(title);
                break;
            }

            case 4: {
                string isbn;
                cout << "Enter ISBN to delete: ";
                getline(cin, isbn);
                library.deleteBookByISBN(isbn);
                break;
            }

            case 5: {
                string title;
                cout << "Enter Title to delete: ";
                getline(cin, title);
                library.deleteBookByTitle(title);
                break;
            }

            case 6:
                library.displayBooks();
                break;

            case 7:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 7);

    return 0;
}