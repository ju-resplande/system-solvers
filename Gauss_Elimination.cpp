#include <iostream>
#define NOTFOUND -1

using namespace std;

int main(void){
    bool is_identity = true, no_answer = false, zero;
	int nrow, ncol, i, j, k, fst, pos;
    float **matrix, aux;
    char var = 'a';

	cout << "N of rows: ";
	cin >> nrow;
	cout << "N of columns: ";
	cin >> ncol;

	matrix = new float*[nrow];

	for (i = 0; i < nrow; ++i)
		matrix[i] = new float[ncol];

    cout << endl << "Insert system coefficients" << endl;

	for (i = 0; i < nrow; ++i)
		for (j = 0; j < ncol; ++j)
			cin >> matrix[i][j];

    pos = 0; //Line position
    for (j = 0; j < ncol; ++j){
		
        //index of fst nonzero number in column
        fst = NOTFOUND;
		
        for (i = pos; i < nrow; ++i){
			
            if (fst == NOTFOUND and matrix[i][j] != 0){
                fst = i;
				
                aux = matrix[i][j]; //get the value of the inde

                for (k = 0; k < ncol; k++)	
					matrix[i][k] /= aux; //"normalize"
            
            }else if(fst != NOTFOUND){ //get zeros
				aux = matrix[i][j];

				for (k = 0; k < ncol; k++)
					matrix[i][k] -= matrix[fst][k]*aux;
			}
		}

		if (fst != NOTFOUND){
			for (i = 0; i < pos; ++i){
				if (matrix[i][j] != 0){
					aux = matrix[i][j];

					for (k = 0; k < ncol; ++k)
						matrix[i][k] -= matrix[fst][k]*aux;
				}
			}

			swap(matrix[pos], matrix[fst]);
			pos++;
		}
	}


    for (int i = 0; i < nrow; ++i){
        zero = true; //check if all line is zero
        
        for (int j = 0; j < ncol-1; ++j){
            if (i == j){
                if (matrix[i][j] != 1)
                    is_identity = false;  
            }else{
                if (matrix[i][j] != 0)
                    is_identity = false;
            }

            if (matrix[i][j] != 0)
                zero = false;
        }

        if (zero and matrix[i][ncol-1] != 0){
            no_answer = true;
            break;
        }
    }

    if (not is_identity)
        cout << endl << "System is not determined!" << endl;

    if (no_answer)
        cout << "There is no possible answer" << endl;
    else{
        cout << endl;

        for (i = 0; i < nrow; ++i){
            zero = true;
            for (j = 0; j < ncol; ++j)
                if (matrix[i][j] != 0)
                    zero = false;

            if (zero)
                continue;

            fst = false; 
            //if it is the fst nonzero
            for (j = 0; j < ncol; ++j){
                if (j == ncol-1)
                    cout << "= "  << matrix[i][j];
                else if (matrix[i][j] == 0)
                    continue;
                else{
                    if (fst)
                        cout << "+";

                    if (matrix[i][j] != 1)
                        cout << matrix[i][j];

                    cout << var++ << " "; 
                }

                if (matrix[i][j] != 0)
                    fst = true;
            }
            cout << endl;
        }  
    }
    
for (i = 0; i < nrow; ++i)
    delete matrix[i];
delete matrix;

	return 0;
}