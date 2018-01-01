#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

#define Max 100

using namespace std;

struct Array {
    int elements[Max];
    int Size = 0;
    int getSize () {
        return Size;
    }

    void pushBack (int i) {
        elements[Size++] = i;
    }

    int At(int i) {
        return elements[i];
    }

    Array & operator = (const Array &arr) {
        for (int i = 0; i < arr.Size; i++)
            this->elements[i] = arr.elements[i];
        this->Size = arr.Size;
        return *this;
    }

    bool operator > (Array arr) {
        if (this->getSize() > arr.getSize())
            return true;
        return false;
    }

    bool operator < (Array arr) {
        if (this->getSize() < arr.getSize())
            return true;
        return false;
    }
};

void initialize_matrix (int **, int, int);
void input_matrix (int **, int, int);
void print_matrix (int **, int, int);
void print_array (Array);
void print_vector (vector <Array>);
Array Union (Array, Array);
Array Union_intents (Array, Array);
bool Equal (Array, Array);
void find_extents (int **, int, int, vector <Array> &);
void find_intents (int **, int, int, vector <Array> &, vector <Array> &);
void addDifferent (Array , vector <Array> &, bool &);
Array Extent2Intent (int **, int, int, Array, bool &, bool &);


/***************/
void quicksortExtents(vector <Array> &, int , int );
void swap_array (Array &a, Array &b);

int main () {

    //initialize and input
    int row, column;
    cin >> row >> column;
    int **matrix = new int *[row];
    for (int i = 0 ; i < row ; i++)
        matrix[i] = new int [column];
    initialize_matrix (matrix, row, column);
    input_matrix(matrix, row, column);
    vector <Array> intents;
    vector <Array> extents;
    //end
    find_extents(matrix, row, column, extents);
    find_intents(matrix, row, column, extents, intents);
    print_vector(extents);
    cout<<endl<<endl;
    quicksortExtents(extents,0,extents.size());
    print_vector(extents);
    /*
    int A[20];
    srand(time(NULL));
    for (int i = 0; i < 20; i++)
        A[i] = rand()%40;
    quickSort(A,0,20);
    for (int i = 0; i < 20; i++)
        cout<< A[i] <<" ";*/

    return 0;
}

void initialize_matrix (int **matrix, int row, int column) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
            matrix[i][j] = 0;
    }
}

void print_matrix (int **matrix, int row, int column) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
            cout<<matrix[i][j]<<" ";
        cout<<endl;
    }
}

//Ham da tang len gia tri la 1
void print_array (Array arr) {
    if (arr.getSize() == 0) {
        cout<<"Empty";
        return;
    }
    for (int i = 0; i < arr.getSize(); i++)
        cout << arr.At(i)+1 << " ";
}

void print_vector (vector <Array> arr) {
    for (int i = 0 ; i < arr.size(); i++) {
        print_array(arr.at(i));
        cout<<endl;
    }
}

void input_matrix (int **matrix, int row, int column) {
    for (;;) {
        int temp_r, temp_c;
        cin >> temp_r >> temp_c;
        if (temp_r == temp_c && temp_r == -99)
            return;
        matrix[temp_r][temp_c] = 1;
    }
}

Array Union (Array a, Array b) {
    Array temp;
    for (int i = 0 ; i < a.getSize(); i++) {
        for (int j = 0; j < b.getSize(); j++) {
            if (a.At(i) == b.At(j)) {
                temp.pushBack(a.At(i));
                break;
            }
        }
    }
    return temp;
}

Array Union_intents (Array a, Array b) {
    Array temp;
    for (int i = 0; i < a.getSize(); i++) {
        if (a.At(i) == b.At(i) && a.At(i) == 1)
            temp.pushBack(1);
        else
            temp.pushBack(0);
    }
    return temp;
}

bool Equal (Array a, Array b) {
    if (a.getSize() != b.getSize())
        return false;
    for (int i = 0 ; i < a.getSize(); i++)
        if (a.At(i) != b.At(i))
            return false;
    return true;
}

Array Extent2Intent (int **matrix, int row, int column, Array ex, bool &extent_all, bool &extent_empty) {

        //check xem co phai extent chua tat ca cac doi tuong khong
        if (ex.getSize() == row)
            extent_all = true;
        if (ex.getSize() == 0)
            extent_empty = true;

        Array *temp_intents = new Array [ex.getSize()];
        for (int index = 0; index < ex.getSize(); index++)
            for (int j = 0; j < column; j++)
                    temp_intents[index].pushBack(matrix[ex.At(index)][j]);

        for (int index = 1; index < ex.getSize(); index++)
            temp_intents[0] = Union_intents (temp_intents[0], temp_intents[index]);

        Array one_intent;
        for (int index = 0; index < column; index++)
            if (temp_intents[0].At(index) == 1)
                one_intent.pushBack(index);

        return one_intent;
}

void find_intents (int **matrix, int row, int column, vector <Array> &extents, vector <Array> &intents) {
    //Xem da xet truong hop extent bang rong va bang tat ca chua
    bool extent_empty = false;
    bool extent_all = false;
    bool add = false;

    for (int i = 0; i < extents.size(); i++) {
        add = false;
        Array intent_temp;
        intent_temp = Extent2Intent(matrix,row,column,extents.at(i),extent_all,extent_empty);
        addDifferent(intent_temp,intents,add);
        if (add == false) {
            extents.erase(extents.begin() + i);
            i--;
        }
    }
    if (extent_all == false) {
        Array temp_ex;
        for (int i = 0; i < row; i++)
            temp_ex.pushBack(i);

        Array temp_in;
        temp_in = Extent2Intent(matrix,row,column,temp_ex, extent_all, extent_empty);
        intents.push_back(temp_in);
        extents.push_back(temp_ex);
    }

    if (extent_empty == false) {
        Array temp_ex;
        Array temp_in;
        for (int i = 0; i < column; i++)
            temp_in.pushBack(i);
        add = false;
        addDifferent(temp_in,intents,add);
        if (add == true)
            extents.push_back(temp_ex);
    }
}

void find_extents (int **matrix, int row, int column, vector <Array> &extents) {
    for (int i = 0 ; i < column; i++) {
        Array temp_extent;
        for (int j = 0; j < row; j++)
            if (matrix[j][i] == 1)
                temp_extent.pushBack(j);
        extents.push_back(temp_extent);
    }

    bool continueFindExtents = false;
    int start = 0;
    do {
        continueFindExtents = false;
        int size_before_union = extents.size();
        for (int i = start; i < size_before_union-1; i++) {
            for (int j = i + 1; j < size_before_union ; j++) {
                Array temp_extent;
                temp_extent = Union(extents.at(i),extents.at(j));
                if (temp_extent.getSize() > 0)
                    addDifferent(temp_extent,extents,continueFindExtents);
            }
        }
        start = size_before_union;
    }while (continueFindExtents == true);
}

void addDifferent (Array a, vector <Array> &v, bool &add) {
    for (int i = 0 ; i < v.size(); i++)
        if (Equal(a,v.at(i)) == true)
            return;
    v.push_back(a);
    add = true;
}


//MINIMIZE CONCEPT
void quicksortExtents(vector <Array> &extents, int l, int r)
{
    Array key = extents.at((l + r)/2);
    int i = l, j = r;
    while(i <= j)
    {
        while(extents.at(i) < key) i++;
        while(extents.at(j) > key) j--;
        if(i <= j)
        {
            if (i < j)
                swap_array(extents.at(i), extents.at(j));
            i++;
            j--;
        }
    }
    if (l < j) quicksortExtents(extents, l, j);
    if (i < r) quicksortExtents(extents, i, r);
}

void swap_array (Array &a, Array &b) {
    Array temp = a;
    a = b;
    b = temp;
}












