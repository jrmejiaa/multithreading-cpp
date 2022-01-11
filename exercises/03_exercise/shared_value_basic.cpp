#include <iostream>
#include <string>
#include <thread>
#include <mutex>

int sharedValue = 0;
void IncrementSharedValue10000000Times ( )
{
    int count = 0;
    while ( count < 10000000)
    {
        sharedValue++;
        count++;
    }
}
/*
 * Questions about the code
 *
 * a) What would be the expected output of this program?
 *      R/ The expected output of the program would be 20000000 in the
 *      sharedValue variable after both threads has finished.
 * a.1) Is it consistent between executions? If not, give a reason why.
 *      R/ No, every time that the code is executed, there are problems of consistency with the variable, because
 *      there are two threads trying to access the same global variable at the same time and there are no memory
 *      consistency
*/

int main (int argc , char * argv [] )
{
    sharedValue = 0;
    std::thread thread2 ( IncrementSharedValue10000000Times ) ;
    IncrementSharedValue10000000Times ( ) ;
    thread2.join() ;
    printf("sharedValue=%d \n", sharedValue) ;
    return 0;
}