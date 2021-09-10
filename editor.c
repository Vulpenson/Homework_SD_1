#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct node {
    char ch;
    struct node* prev;
    struct node* next;
} node_t;

typedef struct stk_n {
    char comm[50];
    struct stk_n* prev;
} stack_node;

typedef struct {
    stack_node* top;
    int size;
} stack;

// Functia de alocare a unei stive
stack* alloc_stack() {
    stack* stk = (stack*) malloc(sizeof(stack));
    stk -> top = (stack_node*) malloc(sizeof(stack_node));
    stk -> top = NULL;
    stk -> size = 0;
    return stk;
}

// Functia care aloca un nou nod de lista
node_t* new_node(char character) {
    node_t* node = (node_t*) malloc(sizeof(node_t));
    node -> ch = character;
    node -> next = NULL;
    return node;
}

// Functia care aloca un nou nod de stiva
stack_node* new_stack_node(char command[50]) {
    stack_node* node = (stack_node*) malloc(sizeof(stack_node));
    strcpy(node -> comm, command);
    node -> prev = NULL;
    return node;
}

// Functia de schimbare din mod text in mod editor
int mode_change_verifier(node_t* head) {
    if((head -> ch == 'i') && (head -> prev -> ch == ':')) {
        return 1;
    } else {
        return 0;
    }
}

// Functia care returneaza echivalentul unui caracter in int
int ctoi(char cf) {
    if(cf == '1') return 1;
    if(cf == '2') return 2;
    if(cf == '3') return 3;
    if(cf == '4') return 4;
    if(cf == '5') return 5;
    if(cf == '6') return 6;
    if(cf == '7') return 7;
    if(cf == '8') return 8;
    if(cf == '9') return 9;
    return 0;
}

// Functia care returneaza ultimul element din lista
node_t* last_of_list(node_t* head) {
    node_t* cpy_head = head;
    while(cpy_head -> next) {
        cpy_head = cpy_head -> next;
    }
    return cpy_head;
}

// Functia care adauga un element la finalul listei
void push(node_t* head, node_t** cursor, char value) {
    node_t* cpy_head = head;
    while(cpy_head -> next) {
        cpy_head = cpy_head -> next;
    }
    node_t* new_n = new_node(value);
    cpy_head -> next = new_n;
    new_n -> prev = cpy_head;
    new_n -> next = NULL;
    *cursor = new_n;
}

// Functia care adauga un element la finalul stivei
void push_stack(stack* stk, char* command) {
    stack_node* new_n = new_stack_node(command);
    new_n -> prev = stk -> top;
    stk -> top = new_n;
    (stk -> size) = (stk -> size) + 1;
}

// Functia de stergere a unui caracter din spatele cursorului
void backspace(node_t** node) {
    node_t* temp = (*node);
    (*node) = (*node) -> prev;
    if(temp -> next) {
        temp -> next -> prev = temp -> prev;
    }
    if(temp -> prev) {
        temp -> prev -> next = temp -> next;
    }
    free(temp);
}

// Functia care sterge un caracter din fata cursorului
void d(node_t** head, node_t** cursor, char *command) {
    if(strlen(command) <= 2) {
        backspace(&((*cursor) -> next));
    } else {
        int i;
        int c = ctoi(command[2]);
        for(i = 0; i < c; i++) {
            backspace(&((*cursor) -> next));
        }
    }
}

// Functie helper care sterge aparitia lui ::i in text
void delete_i(node_t** head, node_t** cursor) {
    int n = 3;
    int i;
    node_t* temp = *head;
    while(temp -> next) {
        temp = temp -> next;
    }
    for(i = 0; i < n; i++) {
        temp = temp -> prev;
        temp -> next = NULL;
    }
    *cursor = last_of_list(*head);
}

// Functia care muta cursorul pe linia specificata
void gl(node_t** head, node_t** cursor, char *command) {
    int k = 1;
    node_t* temp = *head;
    int c = ctoi(command[3]);
    // Parcurgerea listei, '\n' considerat delimitator de linii
    while(k < c) {
        temp = temp -> next;
        if(temp -> ch == '\n') {
            k++;
        }
    }
    *cursor = temp -> next;
}

// Functia care sterge o linie specificata, sau linia curenta
// daca nu se specifica parametrul
void dl(node_t** head, node_t** cursor, char *command) {
    node_t* temp = *cursor;
    if(strlen(command) > 2) {
        gl(head, &temp, command);
    }
    // Parcurgem lista pana la capatul liniei 
    while(temp -> next -> ch != '\n') {
        temp = temp -> next;
    }
    // Stergem toate caracterele pana pe randul din spate
    while(temp -> ch != '\n') {
        backspace(&temp);
    }
    if(strlen(command) > 2) {
        backspace(&temp);
    }
} 

// Functia care muta cursorul pe o anumita pozitie fie intr o linie
// specificata, fie pe linia curenta, in cazul lipsei de parametru de line
void gc(node_t** head, node_t** cursor, char *command) {
    if(strlen(command) > 5) {
        int k = 1;
        int i;
        node_t* temp = *head;
        int line = ctoi(command[5]);
        int position = ctoi(command[3]);
        while(k < line) {
            temp = temp -> next;
            if(temp -> ch == '\n') {
                k++;
            }
        }
        *cursor = temp -> next;
        for(i = 1; i < position; i++) {
            *cursor = (*cursor) -> next;
        }
    } else {
        int i;
        int position = ctoi(command[3]);
        for(i = 1; i < position; i++) {
            *cursor = (*cursor) -> next;
        }
    }
}

// Functia cu rol de undo
void undo(stack* stk, int inserted_characters, node_t** head, node_t** cursor, char* deleted) {
    // Cazul de undo de inserare text
    if(!(strcmp((stk -> top -> prev -> prev) -> comm, "::i")) ||
    !(strcmp((stk -> top -> prev -> prev) -> comm, "::i "))) {
        int i;
        // - 3 pentru ca ia inserted_characters numara si ::i
        for(i = 0; i < inserted_characters - 3; i++) {
            strncat(deleted, &((*cursor) -> ch), 1);
            backspace(cursor);
        }
    }
}

// Functia cu rol de redo
void redo(stack* stk, int inserted_characters, node_t** head, node_t** cursor, char* deleted) {
    // Cazul in care undo stersese o inserare text
    if(!(strcmp((stk -> top -> prev -> prev -> prev) -> comm, "::i")) ||
    !(strcmp((stk -> top -> prev -> prev -> prev) -> comm, "::i "))) {
        int i;
        for(i = strlen(deleted) - 1; i > 0; i--) {
            push(*head, cursor, deleted[i]);
        }
        push(*head, cursor, '\n');
    }
}

//Functia de afisare printeaza textul in fisier
void afisare(node_t* head, FILE* out) {
    node_t* temp = head;
    fprintf(out, "%c", temp->ch);
    if(temp -> next != NULL) {
        afisare(temp -> next, out);
    }
}

// Functia save, folosita pentru a salva documentul
void save(node_t* head, stack* stack_commands) {
    FILE* out = fopen("editor.out", "w");
    afisare(head, out);
    fclose(out);
    stack_commands -> size = 0;
}

// Functia editor prin care recunoastem functia apelata
void editor(stack* stack_commands, int* quit, node_t** cursor, node_t* head,
    char* command, int inserted_characters, char* deleted) {
    if(!(strcmp(command, "s"))) {
        save(head, stack_commands);
    }
    if(!(strcmp(command, "q"))) {
        quit = 0;
    }
    if(strstr(command, "gl")) {
        gl(&head, cursor, command);
    }
    if(!(strcmp(command, "b"))) {
        backspace(cursor);
    }
    if(strstr(command, "dl")) {
        dl(&head, cursor, command);
    }
    if(strstr(command, "gc")) {
        gc(&head, cursor, command);
    }
    if(strstr(command, "d") && strlen(command) <= 3) {
        d(&head, cursor, command);
    }
    if(!(strcmp(command, "u"))) {
        undo(stack_commands, inserted_characters, &head, cursor, deleted);
    }
    if(!(strcmp(command, "r"))) {
        redo(stack_commands, inserted_characters, &head, cursor, deleted);
    }
}

int main() {
    FILE* in = fopen("editor.in", "r");
    stack* stack_commands = alloc_stack(); // Stiva de comenzi

    node_t* head = (node_t*) malloc(sizeof(node_t)); // Lista de text
    head -> next = NULL;
    head -> prev = NULL;

    node_t* cursor = NULL; // Cursorul
    char character; // Stocheaza un caracter din text
    char command[10]; // Stocheaza o comanda
    char* deleted = (char*) malloc(10 * sizeof(char)); //Stocheaza textul sters
    command[0] = '\0';
    int mod = 0; /* Contor folosit pentru interschimbarea 
    intre mod editor si mod text */
    int quit = 1; // Contor folosit pentru inchiderea programului
    int first_step = 0; // Contor folosit pentru realizarea primului pas
    
    // Contor ce retine nr de caractere inserate in modul text 
    // daca s-a intrat cel putin o data in mod editor
    int inserted_characters = 0; 

    while(((character = fgetc(in)) != EOF) && quit)  {
        if(!first_step) {
            head -> ch = character;
            cursor = head;
            first_step = 1;
        } else {
            if(mod % 2 == 0) {
                if (mod > 1) {
                    inserted_characters++;
                }
                push(head, &cursor, character);
                if(mode_change_verifier(last_of_list(head))) {
                    mod = mod + mode_change_verifier(last_of_list(head));
                    delete_i(&head, &cursor);
                }
            } else {
                if (character == '\n') {
                    if(!(strcmp(command, "::i")) || !(strcmp(command, "::i "))) {
                        push_stack(stack_commands, command);
                        mod++;
                        inserted_characters = 0;
                        memset(command, '\0', sizeof(char));
                    } else {
                        push_stack(stack_commands, command);
                        editor(stack_commands, &quit, &cursor, head, command, inserted_characters, deleted);
                        memset(command, '\0', sizeof(char));
                    }
                } else {
                    strncat(command, &character, 1);
                }
            }
        }
    }
    free(stack_commands);
    fclose(in);
    return 0;
}