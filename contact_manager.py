import pickle
import os

# Path to the pickle file
PICKLE_FILE = 'contacts.pkl'

def load_contacts():
    if os.path.exists(PICKLE_FILE):
        with open(PICKLE_FILE, 'rb') as f:
            return pickle.load(f)
    return []

def save_contacts(contacts):
    with open(PICKLE_FILE, 'wb') as f:
        pickle.dump(contacts, f)
    print(f"Contacts have been saved to '{PICKLE_FILE}'.")

def initialize_pickle_with_defaults():
    if not os.path.exists(PICKLE_FILE):
        contacts = [
            {"name": "Hong", "phone": "010-1234-5678", "address": "50 Yonsei-ro, Seodaemun-gu, Seoul"},
            {"name": "Kim", "phone": "010-8765-4321", "address": "1 Gwanak-ro, Gwanak-gu, Seoul"},
            {"name": "Jack", "phone": "010-1234-1234", "address": "Songdogwahak-ro, Yeonsu-gu, Incheon"},
            {"name": "Julia", "phone": "010-1234-4321", "address": "145 Anam-ro, Seongbuk-gu, Seoul"}
        ]
        save_contacts(contacts)

def load_contacts_for_c():
    contacts = load_contacts()
    return [(contact["name"], contact["phone"], contact["address"]) for contact in contacts]

def main():
    initialize_pickle_with_defaults()
    contacts = load_contacts()
    print(contacts)

if __name__ == "__main__":
    main()
