#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "TestDesc.h"
#include "Semihosting.h"
#include "FPGA.h"
#include "IORunner.h"
#include "ArrayMemory.h"
#include <stdlib.h>
using namespace std;

#ifdef BENCHMARK
#define MEMSIZE 300000
#else
#define MEMSIZE 230000
#endif

// Dummy (will be generated by python scripts)
// char* array describing the tests and the input patterns.
// Reference patterns are ignored in this case.
#include "TestDrive.h"
#include "Patterns.h"



int testmain()
{
    char *memoryBuf=NULL;



  
    memoryBuf = (char*)malloc(MEMSIZE);
    if (memoryBuf !=NULL)
    {
        try
        {
           // Choice of a memory manager.
           // Here we choose the Array one (only implemented one)
           Client::ArrayMemory memory(memoryBuf,MEMSIZE);

           // There is also possibility of using "FPGA" io
           //Client::Semihosting io("../TestDesc.txt","../Patterns","../Output","../Parameters");
           Client::FPGA io(testDesc,patterns);

    
           // Pattern Manager making the link between IO and Memory
           Client::PatternMgr mgr(&io,&memory);

    
           // A Runner to run the test.
           // An IO runner is driven by some IO
           // In future one may have a client/server runner driven
           // by a server running on a host.
           //Client::IORunner runner(&io,&mgr,Testing::kTestAndDump);
           Client::IORunner runner(&io,&mgr,Testing::kTestOnly);

    
           // Root object containing all the tests
           Root d(1);

           // Runner applied to the tree of tests
           d.accept(&runner);
    
        }
        catch(...)
        {
            printf("Exception\n");
        }
        
        
        free(memoryBuf);
    }
    else
    {
      printf("NOT ENOUGH MEMORY\n");
    }

    /* code */
    return 0;
}