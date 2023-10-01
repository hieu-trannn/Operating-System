#include <iostream>
#include <fstream>
#include <string.h>
#include <list>
#include <algorithm>

using namespace std;

constexpr int maxEntries = 100;
constexpr int nameMaxLength = 50;
constexpr char* filename = "contacts.db";

// variable size
struct contact {
	char* name;
	char* tel;
};


// fixed size
struct header {
	int numEntries;

	struct {
		char name[nameMaxLength];
		int location;
	} entries[maxEntries];
};



void createDataFile() {
	// sample data
	list<contact> contacts{
		{ "Le Tuan Anh", "0900000000"  },
		{ "Le Viet Anh", "0911111111" },
		{ "Tran Van Anh", "0922222222" },
		{ "Vu Hai Dang", "0933333333" },
		{ "Nguyen Van Duc", "0944444444" },
		{ "Nguyen Ngoc Duong", "0955555555" },
		{ "Nguyen Dinh Duy	", "0966666666" },
		{ "Vu The Duy", "0977777777" },
		{ "Tran Thuy Hien", "0988888888" },
		{ "Tran Trung Hieu", "0999999999" },
	};

	header h;
	h.numEntries = contacts.size();

	ofstream f{filename, ios::binary};
	f.seekp(sizeof(h), ios::beg);

	// write contact data first, and calculate the index at the same time
	int i = 0;
	for_each(contacts.begin(), contacts.end(), [&](const contact& c) {
		strncpy(h.entries[i].name, c.name, nameMaxLength);
		h.entries[i].location = f.tellp();

		int len = strlen(c.name);
		f.write((char*)&len, sizeof(len));
		f.write(c.name, len);

		len = strlen(c.tel);
		f.write((char*)&len, sizeof(len));
		f.write(c.tel, len);

		i++;
	});

	// then go back to the beginning and write the header
	f.seekp(0, ios::beg);
	f.write((char*)&h, sizeof(h));

	cout << "Data file is created!" << endl;
}


void searchContact() {
	string name;
	cout << "Search name: ";
	getline(cin, name);

	if (name.empty()) {
		cerr << "Please enter a name!" << endl;
		return;
	}

	header h;
	ifstream f(filename, ios::binary);
	if (!f) {
		cerr << "Data file not found!" << endl;
		return;
	}

	f.read((char*)&h, sizeof(h));

	contact c;
	bool found = false;
	for (int i = 0; i < h.numEntries; i++) {
		if (h.entries[i].name == name) {
			f.seekg(h.entries[i].location);

			int len;
			f.read((char*)&len, sizeof(len));

			c.name = new char[len + 1];
			f.read(c.name, len);
			c.name[len] = 0;

			f.read((char*)&len, sizeof(len));

			c.tel = new char[len + 1];
			f.read(c.tel, len);
			c.tel[len] = 0;

			found = true;
			break;
		}
	}

	if (found) {
		cout << "Name: " << c.name << endl
			<< "Tel: " << c.tel << endl;
	} else cout << "Contact not found: " << name << endl;
}


int main(int argc, const char* argv[]) {
	if (argc != 2 || (strcmp(argv[1], "-c") != 0 && strcmp(argv[1], "-s") != 0)) {
		const string path{argv[0]};
		const string filename = path.substr(path.find_last_of("/\\") + 1);

		cout << "Usage:" << endl;
		cout << "- Create sample data file:" << endl;
		cout << "  " << filename << " -c" << endl;
		cout << "- Search contact:" << endl;
		cout << "  " << filename << " -s" << endl;
		return 0;
	}

	if (strcmp(argv[1], "-c") == 0) createDataFile();
	else if (strcmp(argv[1], "-s") == 0) searchContact();

    return 0;
}