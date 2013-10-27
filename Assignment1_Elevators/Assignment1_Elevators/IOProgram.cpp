#include "IOProgram.h"

struct ElevatorStatus{   // struct for elevator status

};

IOProgram::IOProgram(){}




int IOProgram::main()
{
	CDataPool	dp1("ElevatorStatus1", sizeof(struct ElevatorStatus));
	CDataPool	dp2("ElevatorStatus2", sizeof(struct ElevatorStatus));
	CPipe		DCommands("DispatcherCommands", 1024);

	struct ElevatorStatus		*EStatus1 = (struct ElevatorStatus*)(dp1.LinkDataPool());
	struct ElevatorStatus		*EStatus2 = (struct ElevatorStatus*)(dp2.LinkDataPool());

	UINT		Message ;			// A variable to hold a message read for message queue

	CMailbox 	MyMailBox ;		// Create a message queue object for this particular child thread
	do {
		printf("Child Thread 2: Polling Message Queue\n") ;
		Sleep(500) ;						// sleep for 1/2 second

		if(MyMailBox.TestForMessage() == TRUE) {		// see if a message exists in the message queue
			Message = MyMailBox.GetMessage() ;		// if so get the message
			if(Message == 2000)	{			// decide what message means
				printf("Child Thread 2 received Message 2000.....\n") ;
			}
			else if(Message == 2001)	{		// if not 2000, is it 2001
				printf("Child Thread 2 received END Message.....\n") ;
				break ;							// break from loop
			}
		}
	}while(1) ;	

	delete EStatus1;
	delete EStatus2;
	return 0;
}