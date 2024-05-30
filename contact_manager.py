import pickle
import os

# Path to the pickle file
PICKLE_FILE = 'contacts.pkl'

def load_contacts_for_c():
    contacts = load_contacts()
    return [(contact["name"], contact["phone"], contact["address"]) for contact in contacts]

def load_contacts():
    if os.path.exists(PICKLE_FILE):
        with open(PICKLE_FILE, 'rb') as f:
            return pickle.load(f)
    return []

def add_contact(name, phone, address):
    contacts = load_contacts()
    new_contact = {"name": name, "phone": phone, "address": address}
    contacts.append(new_contact)
    save_contacts(contacts)

def delete_contact(name, phone):
    contacts = load_contacts()
    contact_to_delete = next((contact for contact in contacts if contact["name"] == name and contact["phone"] == phone), None)
    if contact_to_delete:
        contacts.remove(contact_to_delete)
        save_contacts(contacts)
    else:
        print("Contact not found")

def save_contacts(contacts):
    with open(PICKLE_FILE, 'wb') as f:
        pickle.dump(contacts, f)

def initialize_pickle_with_defaults():
    if not os.path.exists(PICKLE_FILE):
        contacts = [
            {"name": "Hong", "phone": "010-1234-5678", "address": "50 Yonsei-ro, Seodaemun-gu, Seoul"},
            {"name": "Kim", "phone": "010-8765-4321", "address": "1 Gwanak-ro, Gwanak-gu, Seoul"},
            {"name": "Jack", "phone": "010-1234-1234", "address": "Songdogwahak-ro, Yeonsu-gu, Incheon"},
            {"name": "Julia", "phone": "010-1234-4321", "address": "145 Anam-ro, Seongbuk-gu, Seoul"}
        ]
        save_contacts(contacts)