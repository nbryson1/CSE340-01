// Rida Bazzi (c) 2018
#include <iostream>
#include <set>
#include <iterator>

using namespace std;

// The following is a declaration of a struct type called set_struct
// I will use it in the examples below to illustrate the set data type
// behavior
struct node {
       int a;
       int b;

	node(int x, int y) {
		a = x; b = y;
	}
};

// this function defines a comparison operator on two node
// objects the object with the smaller a field is considered smaller, 
// otherwise, the two objects are considered equal
bool operator<(const node& n1, const node& n2) {
	return n1.a < n2.a;
}


int main()
{
    // to declare a set, you need to specify two things
    //
    //  1. the type of the elemets of the set
    //  2. A function to compare elements in the set. This function
    //     is used by the set implementation to keep the elements sorted
    //     order
    //
    // In the example below the object set1 is a set of integers
	// the less function is the default < operator for <int>
 	// initially set1 is empty
	set <int, less <int> > set1;	 

    // we can check how many elements are in the set using the method
    // set::size(). For set1, at this point the set is empty and has
	// 0 size
        cout << "Initially set1 size is: " << set1.size() << "\n";

    // we also declare another object set2
	set <int, less <int> > set2;	 

    // and another object set3
	set <int, less <int> > set3;	 

    // Now that we have declared set1, set2 and set3, let us examine how
    // we can operate on them

    // we start by inserting elements in set1
	set1.insert(40); 	// inserts the integer 40
	set1.insert(30); 	// inserts the integer 30
	set1.insert(60); 	// inserts the integer 60
	set1.insert(20); 	// inserts the integer 20
	set1.insert(40); 	// since 40 was already inserted, this
       	                	// insert will have no effect
	set1.insert(10); 	// insert the integer 10

        // We can check how many elements are in the set using the method
        // set::size(). After inserting the elements, the size of set1 should 
        // be 5 because we have inserted only 5 unique elements: 10, 20, 
        // 30, 40, and 60
        cout << "After inserting elements, set1 size is:" << set1.size() << "\n";

        // We can print the elements in the set, by iterating over them.
        // we declare an iterator by specifying the element type and 
        // the comparison operator for elements in the set
	set <int, less <int> > :: iterator itr1;

        // we print the elements of set1 using itr1
	cout << "set1 is: ";
	for (itr1 = set1.begin(); itr1 != set1.end(); itr1++)
	{
		cout << "  " << *itr1 ;
	}
	cout << " \n\n";


        // now we insert some elements in set2
	set2.insert(40); 	// inserts the integer 40
	set2.insert(50); 	// inserts the integer 30
	set2.insert(60); 	// inserts the integer 60
	set2.insert(70); 	// inserts the integer 20

        // at this point there are 4 elements in set2
        cout << "set2 size is: " << set2.size() << "\n";

        // we print the elements of set2 using itr1
	cout << "set1 is: ";
	for (itr1 = set2.begin(); itr1 != set2.end(); itr1++)
	{
		cout << "  " << *itr1 ;
	}
	cout << " \n\n";

        // At this point, set1 has 5 elements, set2 has 4 elements and
        // set3 is empty. We would like to calculate the union of set1 and
        // set2 and store the result in set3. 
        // We can do the following
        //     1. insert all elements in set1 in set3 
        //     2. insert all elements in set2 in set3 
        set3.insert(set1.begin(), set1.end());
        set3.insert(set2.begin(), set2.end());

        // We can check that set3 has the correct elements by printing its
	// size and elements
        cout << "set3 size is: " << set3.size() << "\n";
	cout << "set3 is: ";
	for (itr1 = set3.begin(); itr1 != set3.end(); itr1++)
	{
		cout << "  " << *itr1 ;
	}
	cout << " \n\n";

        // Working with sets is not restricted to integers obviously.
        // I give an example of a set of structures
        set < node, less <node> > set_of_nodes;

        // We insert 3 nodes into the set_of_nodes
        set_of_nodes.insert(node(4,5));
        set_of_nodes.insert(node(4,6));
        set_of_nodes.insert(node(5,4));

        // Since the first two inserted nodes are equal according
        // to the define < operator for nodes, node(4,6) is not inserted
        // and the size of the set_of_nodes is only 2
        cout << "set_of_nodes size is: " << set_of_nodes.size() << "\n";

	// We can check if an element is in a set using the find() method
	// which returns an iterator (pointer) to the element if
	// the element is in the set. If the element is not in the set, 
	// find() returns a pointer to the end() of the set (past the 
	// last element). The examples below show both usages
	set < node , less <node> > :: iterator itr2;

    // the following looks for element with key 4 (which we have previously
	// inserted, so it should be found and the value will be printed
	// note that the node being passed has not been itself inserted 
	// but a node with the same key (node(4,5) has been inserted
	itr2 = set_of_nodes.find(node(4,6));
	if (itr2 != set_of_nodes.end())
		cout << "Element found : (" << itr2->a << " , " 
					 << itr2->b << " ) \n";
	else	
		cout << "Element not found!" "\n";

        // the following looks for element with key 3 (which we have not
	// previously inserted, so it should not be found 
	itr2 = set_of_nodes.find(node(3,5));
	if (itr2 != set_of_nodes.end())
		cout << "Element found : (" << itr2->a << " , " 
					 << itr2->b << " ) \n";
	else	
		cout << "Element not found!" "\n";

        // for your project, you can add a unique ide field to all the nodes
        // you create and define < as a function of the id field. This will
	// ensure that all nodes inserted in a set will indeed be inserted

	return 0;

}


