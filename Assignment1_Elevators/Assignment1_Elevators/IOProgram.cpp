#include "IOProgram.h"
#include <conio.h>

IOProgram::IOProgram(): m_exit(FALSE){}


int IOProgram::ReadFromMailbox(void* args)
{
	UINT		Message;	//variable for message command
	CMailbox*	MyMailBox = (CMailbox*)args;		

	do{
		
		if(MyMailBox->TestForMessage() == TRUE) 
		{		
				Message = MyMailBox->GetMessage() ;	
				printf("Message = %d\n", Message);
				if(Message == 3000)	
				{			
					printf("Received Message 3000.....\n") ;
					m_exit = TRUE;
				}
			
		}
		else
		{
			printf("Mailbox is empty\n");
			Sleep(150);
		}
	}while(!m_exit);
	printf("ReadFromMailbox thread terminating...\n");
	return 0;
}

int IOProgram::main()
{
	CDataPool	dp1("ElevatorStatus1", sizeof(struct ElevatorStatus));
	CDataPool	dp2("ElevatorStatus2", sizeof(struct ElevatorStatus));
	CPipe		DCommands("DispatcherCommands", 1024);
	CMailbox	MyMailbox;
	ClassThread<IOProgram>	DispatcherToIoMailbox(this, &IOProgram::ReadFromMailbox, ACTIVE, &MyMailbox);
	

	struct UserCommands userCommands;
	//struct ElevatorStatus		*EStatus1 = (struct ElevatorStatus*)(dp1.LinkDataPool());
	//struct ElevatorStatus		*EStatus2 = (struct ElevatorStatus*)(dp2.LinkDataPool());
	
	do{
		userCommands.Direction = _getch();
		_putch(userCommands.Direction);
		userCommands.Floor = _getch();
		_putch(userCommands.Floor);
		printf("\nReceived two values\n");
		if( userCommands.Direction == 'U' || userCommands.Direction == 'D' || userCommands.Direction == 'E')
		{
			if( ((userCommands.Direction == 'U' || userCommands.Direction == 'D') && (userCommands.Floor <= '9' && userCommands.Floor >= '0' ) ) || ( userCommands.Direction == 'E' && userCommands.Floor == 'E'))
			{
				printf("Sending commands\n");
				DCommands.Write(&userCommands, sizeof(struct UserCommands));
			}
			else
				printf("Error: invalid command\n");
		}
		else
				printf("Error: invalid command\n");

		

	}while(!m_exit);

	DispatcherToIoMailbox.WaitForThread();
	printf("Exiting from IO\n");
	//delete EStatus1;
	//delete EStatus2;

	return 0;
}