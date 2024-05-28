#include <stdio.h>
#include <string.h>
#include <Python.h>

#define MAX_CONTACTS 100

typedef struct {
    char name[50];
    char phone[20];
    char address[50];
} Contact;

Contact contacts[MAX_CONTACTS];
int contactCount = 0;

char logged_in_address[50]; // Global variable to store the address

void call_python_function_initialize_contacts() {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C:/Users/Gyugyu/Desktop/DeliveryApp')");

    PyObject *pName = PyUnicode_DecodeFSDefault("contact_manager");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        // Call the function to initialize the pickle with default contacts if it doesn't exist
        PyObject *pFunc = PyObject_GetAttrString(pModule, "initialize_pickle_with_defaults");
        if (PyCallable_Check(pFunc)) {
            PyObject_CallObject(pFunc, NULL);
        } else {
            PyErr_Print();
            fprintf(stderr, "Cannot find function 'initialize_pickle_with_defaults'\n");
        }
        Py_XDECREF(pFunc);

        // Call the function to load contacts for C
        pFunc = PyObject_GetAttrString(pModule, "load_contacts_for_c");
        if (PyCallable_Check(pFunc)) {
            PyObject *pValue = PyObject_CallObject(pFunc, NULL);
            if (PyList_Check(pValue)) {
                contactCount = PyList_Size(pValue);
                for (int i = 0; i < contactCount; ++i) {
                    PyObject *pContact = PyList_GetItem(pValue, i);
                    PyObject *pName = PyTuple_GetItem(pContact, 0);
                    PyObject *pPhone = PyTuple_GetItem(pContact, 1);
                    PyObject *pAddress = PyTuple_GetItem(pContact, 2);

                    strncpy(contacts[i].name, PyUnicode_AsUTF8(pName), sizeof(contacts[i].name) - 1);
                    strncpy(contacts[i].phone, PyUnicode_AsUTF8(pPhone), sizeof(contacts[i].phone) - 1);
                    strncpy(contacts[i].address, PyUnicode_AsUTF8(pAddress), sizeof(contacts[i].address) - 1);
                }
            }
            Py_XDECREF(pValue);
        } else {
            PyErr_Print();
            fprintf(stderr, "Cannot find function 'load_contacts_for_c'\n");
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        fprintf(stderr, "Failed to load 'contact_manager' module\n");
    }

    Py_Finalize();
}
void initializeContacts() {
    call_python_function_initialize_contacts();
}
void call_python_function(const char *function_name, const char *name, const char *phone, const char *address, int index) {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C:/Users/Gyugyu/Desktop/DeliveryApp')");

    PyObject *pName = PyUnicode_DecodeFSDefault("contact_manager");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, function_name);
        if (PyCallable_Check(pFunc)) {
            PyObject *pArgs;
            if (index >= 0) {
                pArgs = PyTuple_Pack(4, PyLong_FromLong(index), PyUnicode_FromString(name), PyUnicode_FromString(phone), PyUnicode_FromString(address));
            } else {
                pArgs = PyTuple_Pack(3, PyUnicode_FromString(name), PyUnicode_FromString(phone), PyUnicode_FromString(address));
            }
            PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
        } else {
            PyErr_Print();
            fprintf(stderr, "Cannot find function '%s'\n", function_name);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        fprintf(stderr, "Failed to load 'contact_manager' module\n");
    }

    Py_Finalize();
}

int selectContact() {
    int index;
    printf("Enter#: ");
    scanf("%d", &index);
    getchar();  // 버퍼 비우기
    if (index < 1 || index > contactCount) {
        printf("Wrong Input.\n");
        return -1;
    }
    return index - 1;
}
void addContact() {
    if (contactCount >= MAX_CONTACTS) {
        printf("Error: Overflow.\n");
        return;
    }
    printf("Name: ");
    fgets(contacts[contactCount].name, sizeof(contacts[contactCount].name), stdin);
    contacts[contactCount].name[strcspn(contacts[contactCount].name, "\n")] = 0;

    printf("Phone#: ");
    fgets(contacts[contactCount].phone, sizeof(contacts[contactCount].phone), stdin);
    contacts[contactCount].phone[strcspn(contacts[contactCount].phone, "\n")] = 0;

    printf("Address: ");
    fgets(contacts[contactCount].address, sizeof(contacts[contactCount].address), stdin);
    contacts[contactCount].address[strcspn(contacts[contactCount].address, "\n")] = 0;

    call_python_function("add_contact", contacts[contactCount].name, contacts[contactCount].phone, contacts[contactCount].address, -1);
    contactCount++;
    printf("Added New Contact.\n");
}

void editContact() {
    int index = selectContact();
    if (index == -1) return;

    printf("New Name (Current : %s): ", contacts[index].name);
    fgets(contacts[index].name, sizeof(contacts[index].name), stdin);
    contacts[index].name[strcspn(contacts[index].name, "\n")] = 0;

    printf("New Phone# (Current : %s): ", contacts[index].phone);
    fgets(contacts[index].phone, sizeof(contacts[index].phone), stdin);
    contacts[index].phone[strcspn(contacts[index].phone, "\n")] = 0;

    printf("New Address (Current : %s): ", contacts[index].address);
    fgets(contacts[index].address, sizeof(contacts[index].address), stdin);
    contacts[index].address[strcspn(contacts[index].address, "\n")] = 0;

    call_python_function("edit_contact", contacts[index].name, contacts[index].phone, contacts[index].address, index);
    printf("Contact Updated.\n");
}

void deleteContact() {
    int index = selectContact();
    if (index == -1) return;

    call_python_function("delete_contact", "", "", "", index);
    for (int i = index; i < contactCount - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    contactCount--;
    printf("Contact Deleted.\n");
}

int login() {
    char name[50];
    char phone[20];

    printf("Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Phone#: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = 0;

    for (int i = 0; i < contactCount; i++) {
        if (strcmp(name, contacts[i].name) == 0 && strcmp(phone, contacts[i].phone) == 0) {
            strcpy(logged_in_address, contacts[i].address); // Copy the address to the global variable
            return 1; // Login successful
        }
    }

    printf("Login failed. Incorrect name or phone number.\n");
    return 0; // Login failed
}
void call_python_menu(const char* address) {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C:/Users/Gyugyu/Desktop/DeliveryApp')");

    PyObject *pName = PyUnicode_DecodeFSDefault("Menu");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, "Menu");
        if (PyCallable_Check(pFunc)) {
            PyObject *pArgs = PyTuple_Pack(1, PyUnicode_FromString(address));
            PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
        } else {
            PyErr_Print();
            fprintf(stderr, "Cannot find function 'Menu'\n");
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        fprintf(stderr, "Failed to load 'Menu' module\n");
    }

    Py_Finalize();
}
void displayContacts() {
    if (contactCount <= 0) {
        printf("Contact doesn't exist.\n");
    }
    for (int i = 0; i < contactCount; i++) {
        printf("%d. Name : %s, Phone# : %s, Address : %s\n", i + 1, contacts[i].name, contacts[i].phone, contacts[i].address);
    }
}
int main() {
    initializeContacts();

    while (!login()) {
        printf("Please try again.\n");
    }

    call_python_menu(logged_in_address);
    system("pause");
    return 0;
}