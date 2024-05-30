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
char logged_in_name[50];
char logged_in_phone[20];
char logged_in_address[50]; // Global variable to store the address

void call_python_function_initialize_contacts() {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('C:/Users/Gyugyu/Desktop/DeliveryApp')");

    PyObject *pName = PyUnicode_DecodeFSDefault("contact_manager");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, "initialize_pickle_with_defaults");
        if (PyCallable_Check(pFunc)) {
            PyObject_CallObject(pFunc, NULL);
        } else {
            PyErr_Print();
            fprintf(stderr, "Cannot find function 'initialize_pickle_with_defaults'\n");
        }
        Py_XDECREF(pFunc);

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
                    contacts[i].name[sizeof(contacts[i].name) - 1] = '\0'; // Ensure null-termination

                    strncpy(contacts[i].phone, PyUnicode_AsUTF8(pPhone), sizeof(contacts[i].phone) - 1);
                    contacts[i].phone[sizeof(contacts[i].phone) - 1] = '\0'; // Ensure null-termination

                    strncpy(contacts[i].address, PyUnicode_AsUTF8(pAddress), sizeof(contacts[i].address) - 1);
                    contacts[i].address[sizeof(contacts[i].address) - 1] = '\0'; // Ensure null-termination
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

void call_python_function(const char *function_name, const char *arg1, const char *arg2, const char *arg3) {
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
            if (arg3 != NULL) {
                pArgs = PyTuple_Pack(3, PyUnicode_FromString(arg1), PyUnicode_FromString(arg2), PyUnicode_FromString(arg3));
            } else {
                pArgs = PyTuple_Pack(2, PyUnicode_FromString(arg1), PyUnicode_FromString(arg2));
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

    call_python_function("add_contact", contacts[contactCount].name, contacts[contactCount].phone, contacts[contactCount].address);
    contactCount++;
    printf("Added New Contact.\n");
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
            strcpy(logged_in_name, contacts[i].name);
            strcpy(logged_in_phone, contacts[i].phone);
            strcpy(logged_in_address, contacts[i].address); // Copy the address to the global variable
            return 1; // Login successful
        }
    }
    printf("Login failed. Incorrect name or phone number.\n");
    return 0; // Login failed
}

void deleteContact() {
    call_python_function("delete_contact", logged_in_name, logged_in_phone, NULL);

    // Update local contacts array
    int i;
    for (i = 0; i < contactCount; i++) {
        if (strcmp(contacts[i].name, logged_in_name) == 0 && strcmp(contacts[i].phone, logged_in_phone) == 0) {
            break;
        }
    }

    if (i < contactCount) {
        for (int j = i; j < contactCount - 1; j++) {
            contacts[j] = contacts[j + 1];
        }
        contactCount--;
        printf("Contact Deleted.\n");
    } else {
        printf("Contact not found.\n");
    }
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
    //displayContacts();
    char choice;
    do {
        printf("[1] Log-in\n[2] Sign-up\n[3] Exit\nChoose: ");
        choice = getchar();
        getchar(); // Clear buffer
        switch (choice) {
            case '1':
                if (login()) {
                    char sub_choice;
                    do {
                        printf("[1] Order\n[2] Delete Contact\n[3] Log out\nChoose: ");
                        sub_choice = getchar();
                        getchar(); // Clear buffer
                        switch (sub_choice) {
                            case '1':
                                call_python_menu(logged_in_address);
                                break;
                            case '2':
                                deleteContact();
                                break;
                            case '3':
                                printf("Logging out.\n");
                                break;
                            default:
                                printf("Please try again.\n");
                        }
                    } while (sub_choice != '3');
                }
                break;
            case '2':
                addContact();
                //displayContacts();
                break;
            case '3':
                printf("Exiting Program.\n");
                break;
            default:
                printf("Please try again.\n");
        }
    } while (choice != '3');
    return 0;
}