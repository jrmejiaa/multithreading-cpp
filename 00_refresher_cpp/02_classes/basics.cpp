// C++ program for the above approach
#include <iostream>

using namespace std;

// Box Class
class box {
private:
    int  length;
    int* breadth;
    int  height;

public:
    // Constructor
    box() {
        length = 0;
        breadth = new int;
        height = 0;
    }
    // Custom Constructor to avoid Copy Constructor
    box(const box& copy) {
        length = copy.length;
        height = copy.height;
        breadth = new int;
        *breadth = *(copy.breadth);
    }
    // Custom = operator to avoid Default Assigment Operator
    box& operator=(const box& copy) {
        if (&copy != this) {
            length = copy.length;
            height = copy.height;
            breadth = new int;
            *breadth = *(copy.breadth);
        }
        return *this;
    }
    // When copying the data of object
    // at the time of initialization
    // then copy is made through
    // COPY CONSTRUCTOR
    // box B2 = B1;

    // When copying the data of object
    // after initialization then the
    // copy is done through DEFAULT
    // ASSIGNMENT OPERATOR
    // box B1, B3;
    // B1.set_dimensions(...)
    // B3 = B1;

    // Function that sets the dimensions
    void set_dimensions(int length1, int breadth1,
                        int height1) {
        length = length1;
        *breadth = breadth1;
        height = height1;
    }

    // Function to display the dimensions
    // of the Box object
    void show_data() {
        cout << " Length = " << length
             << "\n Breadth = " << *breadth
             << "\n Height = " << height
             << endl;
    }
    void setBreadth(int new_breadth){
        *breadth = new_breadth;
    }
    ~box(){
        delete breadth;
    }
};