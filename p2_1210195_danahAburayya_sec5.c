//danah abu raya 1210195 sec5
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Node structure for the expression tree
typedef struct TreeNode {
    char data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Node structure for linked list
typedef struct Node {
    char data[100];
    struct Node* next;
} Node;

// Stack structure for the linked list nodes
typedef struct {
    int top;
    Node** array;
} Stack;

// Function to create a new tree node
TreeNode* createTreeNode(char data) {
    // Memory allocation for the new node
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initializing the node
    newNode->data = data;
    newNode->left = newNode->right = NULL;

    return newNode;
}

// Function to create a new linked list node
Node* createNode(const char* data) {
    // Memory allocation for the new node
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initializing the node
    strcpy(newNode->data, data);
    newNode->next = NULL;

    return newNode;
}

// Function to initialize the stack
void initializeStack(Stack* stack, int size) {
    stack->top = -1;
    stack->array = (Node**)malloc(size * sizeof(Node*));
    if (stack->array == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
}

// Function to get the top element of the stack
Node* top(Stack* stack) {
    if (stack->top == -1) {
        return NULL;
    }
    return stack->array[stack->top];
}

// Function to check if a character is an operator
int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

// Function to get the precedence of an operator
int getPrecedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    } else if (op == '*' || op == '/') {
        return 2;
    }
    return 0;
}

// Function to evaluate the expression tree
int evaluateExpressionTree(TreeNode* root) {
    if (root == NULL) {
        printf("Error: Null pointer encountered\n");
        return 0;
    }

    // Check if the node contains a digit
    if (isdigit(root->data)) {
        return root->data - '0';
    }

    // Check if the node contains an operator
    if (root->data == '+' || root->data == '-' || root->data == '*' || root->data == '/' || root->data == '%') {
        if (root->left == NULL || root->right == NULL) {
            printf("Error: Operand(s) missing for operator %c\n", root->data);
            return 0;
        }

        // Evaluate left and right operands
        int leftValue = evaluateExpressionTree(root->left);
        int rightValue = evaluateExpressionTree(root->right);

        // Perform the operation based on the operator
        switch (root->data) {
            case '+':
                return leftValue + rightValue;
            case '-':
                return leftValue - rightValue;
            case '*':
                return leftValue * rightValue;
            case '/':
                if (rightValue != 0) {
                    return leftValue / rightValue;
                } else {
                    printf("Error: Division by zero\n");
                    return 0;
                }
            case '%':
                if (rightValue != 0) {
                    return leftValue % rightValue;
                } else {
                    printf("Error: Modulo by zero\n");
                    return 0;
                }
            default:
                printf("Error: Unknown operator %c\n", root->data);
                return 0;
        }
    } else {
        printf("Error: Unexpected character %c\n", root->data);
        return 0;
    }
}

// Function to free the memory allocated for the expression tree
void freeExpressionTree(TreeNode* root) {
    if (root != NULL) {
        freeExpressionTree(root->left);
        freeExpressionTree(root->right);
        free(root);
    }
}

// Function to check if the linked list is empty
int isEmpty(Node* stack) {
    return stack == NULL;
}

// Function to push a node onto the stack
void push(Node** stack, const char* data) {
    Node* newNode = createNode(data);
    newNode->next = *stack;
    *stack = newNode;
}

// Function to pop a node from the stack
char pop(Node** stack) {
    if (*stack == NULL) {
        printf("Error: Stack is empty\n");
    }
    Node* temp = *stack;
    char data = temp->data;
    *stack = temp->next;
    free(temp);

    return data;
}

// Function to convert infix expression to postfix
void infixToPostfix(Node* infix, Node** postfix) {
    Stack stack;
    initializeStack(&stack, 2 * 100 + 1);

    while (infix != NULL) {
        char currentChar = infix->data[0];

        if ('0' <= currentChar && currentChar <= '9') {
            // Operand - add to postfix expression
            Node* operandNode = (Node*)malloc(sizeof(Node));
            if (operandNode == NULL) {
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            strcpy(operandNode->data, infix->data);
            operandNode->next = NULL;

            if (*postfix == NULL) {
                *postfix = operandNode;
            } else {
                Node* temp = *postfix;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = operandNode;
            }
        } else if (currentChar == '(') {
            //push onto the stack
            push(&stack, infix);
        } else if (currentChar == ')') {

            while (top(&stack) != NULL && top(&stack)->data[0] != '(') {
                Node* operatorNode = (Node*)malloc(sizeof(Node));
                if (operatorNode == NULL) {
                    printf("Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(operatorNode->data, top(&stack)->data);
                operatorNode->next = NULL;

                Node* temp = *postfix;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = operatorNode;
                pop(&stack);
            }
            pop(&stack); // Pop the left parenthesis
        } else if (isOperator(currentChar)) {
            //  pop and add to postfix while precedence is greater or equal
            while (top(&stack) != NULL && getPrecedence(currentChar) <= getPrecedence(top(&stack)->data[0]) && top(&stack)->data[0] != '(') {
                Node* operatorNode = (Node*)malloc(sizeof(Node));
                if (operatorNode == NULL) {
                    printf("Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(operatorNode->data, top(&stack)->data);
                operatorNode->next = NULL;

                Node* temp = *postfix;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = operatorNode;
                pop(&stack);
            }
            push(&stack, infix);
        } else {
            printf("Error: Unexpected character '%c'\n", currentChar);
            exit(EXIT_FAILURE);
        }

        infix = infix->next;
    }

    // Pop any remaining operators from the stack and add to postfix
    while (top(&stack) != NULL) {
        Node* operatorNode = (Node*)malloc(sizeof(Node));
        if (operatorNode == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strcpy(operatorNode->data, top(&stack)->data);
        operatorNode->next = NULL;

        Node* temp = *postfix;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = operatorNode;
        pop(&stack);
    }
}

// Function to display equations in the linked list
void displayEquations(Node* head) {
    while (head != NULL) {
        for (int i = 0; head->data[i] != '\0'; ++i) {
            printf("%c ", head->data[i]);
        }
        printf("\n");

        head = head->next;
    }
    printf("\n");
}

// Function to build an expression tree from a postfix expression
TreeNode* buildExpressionTree(Node* postfix) {
    Node* stack = NULL;

    while (postfix != NULL) {
        char currentChar = postfix->data[0];
        TreeNode* newNode = createTreeNode(currentChar);

        if (isdigit(currentChar)) {
            // push onto the stack
            push(&stack, newNode);
        } else {

            TreeNode* operand2 = pop(&stack);
            TreeNode* operand1 = pop(&stack);

            newNode->right = operand2;
            newNode->left = operand1;

            push(&stack, newNode);
        }

        postfix = postfix->next;
    }

    return pop(&stack);
}

// Function to free the memory allocated for the linked list
void freeList(Node* head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

// Function to read infix expression from a file
Node* readFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    Node* infix = NULL;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        char* token = strtok(buffer, " ");
        while (token != NULL) {
            if (strlen(token) > 0) {
                push(&infix, token);
            }
            token = strtok(NULL, " ");
        }
    }

    fclose(file);
    return infix;
}

// Function to write postfix expression and result to an output file
void writeFile(const char* filename, Node* postfix, int result) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Postfix expression: ");
    while (postfix != NULL) {
        fprintf(file, "%c ", postfix->data);
        postfix = postfix->next;
    }
    fprintf(file, "\n");

    fprintf(file, "Result: %d\n", result);

    fclose(file);
}

// Function to display the main menu options
void displayMainMenu() {
    printf("1-Read equation\n2-Print equation\n3-Evaluate using expression tree\n4-Print postfix expression\n5-Save to output file (postfix and result)\n6-Exit\n");
}

// Main program
int main() {
    Node* infix = NULL;
    Node* postfix = NULL;
    TreeNode* expressionTree = NULL;

    int key = 1;

    const char* input = "input.txt";
    const char* output = "output.txt";

    printf("Welcome!\n");

    while (key != 0) {
        displayMainMenu();
        scanf("%d", &key);

        switch (key) {
            case 1:
                // Read infix expression from file, convert to postfix
                infix = readFromFile(input);
                infixToPostfix(infix, &postfix);
                break;

            case 2:
                // Print infix expression
                if (isEmpty(infix)) {
                    printf("The list is empty! Load the data first.\n");
                } else {
                    printf("Infix expression:\n");
                    displayEquations(infix);
                }
                break;

            case 3:
                // Evaluate expression using expression tree
                if (isEmpty(infix)) {
                    printf("The list is empty! Fill it with data first.\n");
                } else {
                    expressionTree = buildExpressionTree(postfix);
                    int result = evaluateExpressionTree(expressionTree);
                    printf("Result of expression: %d\n", result);
                    writeFile(output, postfix, result);
                    freeExpressionTree(expressionTree);
                }
                break;

            case 4:
                // Print postfix expression
                if (isEmpty(infix)) {
                    printf("The list is empty! Load the data first.\n");
                } else {
                    printf("Postfix expression:\n");
                    displayEquations(postfix);
                }
                break;

            case 5:
                // Save postfix expression and result to file
                if (!isEmpty(postfix)) {
                    writeFile(output, postfix, 0);
                } else {
                    printf("No postfix expression to save. Convert infix to postfix first.\n");
                }
                break;

            case 6:
                // Free memory and exit
                freeList(infix);
                exit(0);

            default:
                // Invalid operation
                printf("No such operation! Please try again.\n");
                break;
        }
    }

    return 0;
}
