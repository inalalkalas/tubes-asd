#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BookNode {
    char title[250];
    char name_writer[200];
    struct BookNode* next;
} BookNode;

typedef struct CategoryNode {
    char name[100];
    struct BookNode* books;
    struct CategoryNode* left;
    struct CategoryNode* right;
    int depth;
} CategoryNode;

CategoryNode* createNode(char name[], int depth) {
    CategoryNode* newCategory = (CategoryNode*)malloc(sizeof(CategoryNode));
    if (newCategory == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newCategory->name, name);
    newCategory->books = NULL;
    newCategory->left = NULL;
    newCategory->right = NULL;
    newCategory->depth = depth;
    return newCategory;
}

void addChildCategory(CategoryNode* parent, char name[], int depth, char* direction) {
    if (parent == NULL) {
        printf("Parent category is NULL. Cannot add child category.\n");
        return;
    }
    
    if (strcmp(direction, "left") == 0) {
        if (parent->left == NULL) {
            parent->left = createNode(name, depth);
        } else {
            printf("Left child already exists for this category. Adding to the right instead.\n");
            addChildCategory(parent->left, name, depth, "right");
        }
    } else if (strcmp(direction, "right") == 0) {
        if (parent->right == NULL) {
            parent->right = createNode(name, depth);
        } else {
            printf("Right child already exists for this category.\n");
        }
    } else {
        printf("Invalid direction. Use 'left' or 'right'.\n");
    }
}

void addBook(CategoryNode* category, char title[], char name_writer[]) {
    if (category == NULL) {
        printf("Category is NULL. Cannot add book.\n");
        return;
    }
    
    BookNode* newBook = (BookNode*)malloc(sizeof(BookNode));
    if (newBook == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newBook->title, title);
    strcpy(newBook->name_writer, name_writer);
    newBook->next = NULL;

    if (category->books == NULL) {
        category->books = newBook;
    } else {
        BookNode* currentBook = category->books;
        while (currentBook->next != NULL) {
            currentBook = currentBook->next;
        }
        currentBook->next = newBook;
    }
}

BookNode* searchBook(CategoryNode* category, char title[]) {
    if (category == NULL) return NULL;

    BookNode* foundBook = NULL;
    
    foundBook = searchBook(category->left, title);
    if (foundBook != NULL) return foundBook;
    
    BookNode* currentBook = category->books;
    while (currentBook != NULL) {
        if (strcmp(currentBook->title, title) == 0) {
            return currentBook;
        }
        currentBook = currentBook->next;
    }

    return searchBook(category->right, title);
}

void preOrderTraversal(CategoryNode* category) {
    if (category == NULL) return;

    for (int i = 0; i < category->depth; i++) {
        printf("  ");
    }
    if (category->depth > 0) {
        printf("|_ ");
    }
    printf("%s\n", category->name);

    BookNode* currentBook = category->books;
    while (currentBook != NULL) {
        for (int i = 0; i < category->depth + 1; i++) {
            printf("    ");
        }
        printf("|_ %s\n", currentBook->title);
        currentBook = currentBook->next;
    }

    preOrderTraversal(category->left);
    preOrderTraversal(category->right);
}

void inOrderTraversal(CategoryNode* category) {
    if (category == NULL) return;

    // Traverse the left subtree
    inOrderTraversal(category->left);

    // Print the books in the current category
    BookNode* currentBook = category->books;
    while (currentBook != NULL) {
        for (int i = 0; i < category->depth + 1; i++) {
            printf("  ");
        }
        printf("|_ %s\n", currentBook->title);
        currentBook = currentBook->next;
    }

    // Print the current category
    for (int i = 0; i < category->depth; i++) {
        printf("  ");
    }
    if (category->depth > 0) {
        printf("|_ ");
    }
    printf("%s\n", category->name);

    // Traverse the right subtree
    inOrderTraversal(category->right);
}

// perbaikan
void postOrderTraversal(CategoryNode* category) {
    if (category == NULL) return;

    // Traverse the left subtree
    postOrderTraversal(category->left);

    // Print the books in the current category in reverse order
    if (category->books != NULL) {
        BookNode* currentBook = category->books;
        BookNode* prevBook = NULL;
        // Reverse the linked list of books
        while (currentBook != NULL) {
            BookNode* nextBook = currentBook->next;
            currentBook->next = prevBook;
            prevBook = currentBook;
            currentBook = nextBook;
        }
        // Print the reversed list
        currentBook = prevBook;
        while (currentBook != NULL) {
            for (int i = 0; i < category->depth + 1; i++) {
                printf("    ");
            }
            printf("|_ %s\n", currentBook->title);
            currentBook = currentBook->next;
        }
        // Reverse the list back to original
        currentBook = prevBook;
        prevBook = NULL;
        while (currentBook != NULL) {
            BookNode* nextBook = currentBook->next;
            currentBook->next = prevBook;
            prevBook = currentBook;
            currentBook = nextBook;
        }
        category->books = prevBook;
    }

    // Print the current category
    if (strcmp(category->name, "Book") != 0) {
        for (int i = 0; i < category->depth; i++) {
            printf("  ");
        }
        if (category->depth > 0) {
            printf("|_ ");
        }
        printf("%s\n", category->name);
    }

    // Traverse the right subtree
    postOrderTraversal(category->right);

    // Print the root/category "Book" if it's the current category
    if (strcmp(category->name, "Book") == 0) {
        for (int i = 0; i < category->depth; i++) {
            printf("  ");
        }
        if (category->depth > 0) {
            printf("|_ ");
        }
        printf("%s\n", category->name);
    }
}

void printBooks(CategoryNode* category) {
    if (category == NULL) return;

    for (int i = 0; i < category->depth; i++) {
        printf("  ");
    }
    if (category->depth > 0) {
        printf("|_ ");
    }
    printf("%s\n", category->name);

    BookNode* currentBook = category->books;
    while (currentBook != NULL) {
        for (int i = 0; i < category->depth + 1; i++) {
            printf("    ");
        }
        printf("|_ %s; Writer: %s\n", currentBook->title, currentBook->name_writer);
        currentBook = currentBook->next;
    }

    printBooks(category->left);
    printBooks(category->right);
}

void printLibrary(CategoryNode* library) {
    printf("\nCurrent Library Structure:\n");
    printBooks(library);
    printf("\n");
}

int main() {
    char searchTitle[100];
    int choice, input;

    CategoryNode* library = createNode("Book", 0);
    printLibrary(library);

    addChildCategory(library, "Fantasy", 1, "left");
    printLibrary(library);

    addBook(library->left, "A Game of Thrones", "George R.R Martin");
    printLibrary(library);

    addBook(library->left, "Eragon", "Christopher Paolini");
    printLibrary(library);

    addBook(library->left, "Dune", "Frank Herbert");
    printLibrary(library);

    addBook(library->left, "The Giver", "Lois Lowry");
    printLibrary(library);

    addChildCategory(library, "Biology", 1, "right");
    printLibrary(library);

    addBook(library->right, "Biology", "Michael L.Cain");
    printLibrary(library);

    addBook(library->right, "Microbiology: An Introduction", "Gerard J.Tortora");
    printLibrary(library);

    addBook(library->right, "Prentice Hall Biology", "Joseph S.Levine");
    printLibrary(library);

    addChildCategory(library->right, "Computer Science", 1, "right");
    printLibrary(library);

    addBook(library->right->right, "Art of Computer Programing", "Donald Ervin Knuth");
    printLibrary(library);

    addBook(library->right->right, "Introduction to Artificial Intelligence", "Eugene Charniak");
    printLibrary(library);

    addBook(library->right->right, "Unix in a Nutshell", "Arnold Robbins");
    printLibrary(library);

    addChildCategory(library->right->right, "Animals", 1, "right");
    printLibrary(library);

    addBook(library->right->right->right, "Farm Animals", "Lucy Cousins");
    printLibrary(library);

    addBook(library->right->right->right, "Animals", "Rebecca Rissman");
    printLibrary(library);

    addBook(library->right->right->right, "The Emotional Lives of Animals: A Leading Scientist Explores Animal Joy, Sorrow, and Empathy -- And Why They Matter", "Marc Bekoff");
    printLibrary(library);

    addChildCategory(library->right->right->right, "Astronomy & Space Science", 1, "right");
    printLibrary(library);

    addBook(library->right->right->right->right, "Cosmos", "Carl Sagan");
    printLibrary(library);

    addBook(library->right->right->right->right, "Rocket Boys", "Homer Hickam");
    printLibrary(library);

    addBook(library->right->right->right->right, "The Right Stuff", "Tom Wolfe");
    printLibrary(library);

    addChildCategory(library->right->right->right->right, "Horror", 1, "right");
    printLibrary(library);

    addBook(library->right->right->right->right->right, "It", "Stephen King");
    printLibrary(library);

    addBook(library->right->right->right->right->right, "The Institute", "Stephen King");
    printLibrary(library);

    addBook(library->right->right->right->right->right, "Black House", "Peter Straub");
    printLibrary(library);

    do 
    {
        printf("Menu\n");
        printf("1. Search Book\n");
        printf("2. Traversal Book\n");
        printf("3. Show Book\n");
        printf("0. Exit\n");
        printf("Pilihan: \n");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                printf("\n");

                printf("\nEnter the title to search: ");
                getchar();
                fgets(searchTitle, sizeof(searchTitle), stdin);
                searchTitle[strcspn(searchTitle, "\n")] = 0;
                BookNode* foundBook = searchBook(library, searchTitle);
                if (foundBook != NULL) {
                    printf("Book Found:\n");
                    printf("Title: %s\n", foundBook->title);
                    printf("Writer: %s\n", foundBook->name_writer);
                } else 
                {
                    printf("Book not found.\n");
                }

                printf("\n");
                break;
            case 2:
                printf("\n");
                
                printf("1. Pre Order\n2. In Order\n3. Post Order\n");
                scanf("%d", &input);

                switch (input) 
                {
                    case 1:
                             printf("\nPreorder Traversal:\n");
                             preOrderTraversal(library);
                             break;
                     case 2:
                             printf("\nInorder Traversal:\n");
                             inOrderTraversal(library);
                             break;
                     case 3:
                             printf("\nPostorder Traversal:\n");
                             postOrderTraversal(library);
                             break;
                     default:
                             return -1;
                }
                printf("\n");
                break;
            case 3:
            
                printf("Tree Structure:\n");
                printBooks(library);
                printf("\n");
                break;
            case 0:
                printf("thank you\n");
                break;
            default:
                return 0;
                break;
        }

} while (choice != 0);

return 0;

}
