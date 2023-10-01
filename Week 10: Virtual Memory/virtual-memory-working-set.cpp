#include <iostream>
#include <bitset>
#include <vector>
#include <array>
#include <algorithm>
#include <set>

using namespace std;

constexpr int numPages = 10;	// number of pages
constexpr int numRefBits = 4;	// number of reference bits per page
constexpr int delta = 16;		// window size
constexpr int numInstructionsPerTimer = delta / numRefBits;	// number of instructions in each iteration (timer)

struct {
	// page number, frame number and other stuff go here, but not considered in this program!
	// int page;
	// int frame;

	bitset<numRefBits> refBits;
} pages[numPages];

int currentRefBit = 0;	// current reference bit



int main() {
	srand(time(NULL));

	// initialize page-reference table
    vector<int> pageRef;
	for (int i = 0; i < numInstructionsPerTimer * 15; i++)
		pageRef.push_back(rand() % numPages);

	cout << "Page reference table: ";
	for (int p : pageRef) cout << p << " ";
	cout << endl;

	for (int i = 0; i < pageRef.size(); ) {
		// reset all reference bits
		for (auto& p : pages) p.refBits[currentRefBit] = 0;

		// update reference bits for referenced pages
		cout << "References pages: [ ";
		for (int j = 0; j < numInstructionsPerTimer; j++, i++) {
			cout << pageRef[i] << " ";
			pages[pageRef[i]].refBits[currentRefBit] = 1;
		}
		cout << "]";

		currentRefBit = (currentRefBit + 1) % numRefBits;

		// calculate the working set
		set<int> ws;
		for (int i = 0; i < numPages; i++) {
			if (pages[i].refBits.any()) ws.insert(i);
		}

		cout << "; WS: [ ";
		for (int p : ws) cout << p << " ";
		cout << "]" << endl;
	}


    return 0;
}