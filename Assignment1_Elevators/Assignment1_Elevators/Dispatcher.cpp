#include "Dispatcher.h"






Dispatcher::Dispatcher() : m_exit(FALSE)
{
	
}

Dispatcher::Dispatcher(IOProgram* pIoProgram): m_pIoProgram(pIoProgram),m_exit(FALSE){}

// Thread to read pipeline with commands from IOProgram
int Dispatcher::ReadFromPipeline3(void *args){ 

	CPipe		DCommands("DispatcherCommands", 1024);
	struct UserCommands userCommands;

	do{
			
		DCommands.Read(&userCommands, sizeof(struct UserCommands));
		printf("Direction = %c and Floor = %c\n", userCommands.Direction,userCommands.Floor);

		if(userCommands.Direction == 'E' && userCommands.Floor == 'E')
		{ // if command is exit, then set exit flag and terminate this thread
			printf("Posting Message\n");
			m_pIoProgram->Post(3000); //3000 is defined as the exit command
			m_exit = TRUE;
		}
	}while(!m_exit);
	return 0;
}

int Dispatcher::main()
{

	CDataPool	dp1("ElevatorStatus1", sizeof(struct ElevatorStatus));
	CDataPool	dp2("ElevatorStatus2", sizeof(struct ElevatorStatus));
	CPipe		ECommands1("Elevator1Commands", 1024);
	CPipe		ECommands2("Elevator2Commands", 1024);
	CPipe		DCommands("DispatcherCommands", 1024);
	ClassThread<Dispatcher>	 IoToDispatcherPipeline(this,&Dispatcher::ReadFromPipeline3,ACTIVE, NULL);

	//struct ElevatorStatus		*EStatus1 = (struct ElevatorStatus*)(dp1.LinkDataPool());
	//struct ElevatorStatus		*EStatus2 = (struct ElevatorStatus*)(dp2.LinkDataPool());

	do{


		if(m_exit == TRUE)
		{
			printf("breaking from dispatcher loop\n");
			break;
		}
	}while(1);

	IoToDispatcherPipeline.WaitForThread();
	printf("Exiting dispatcher\n");
	//delete EStatus1;
	//delete EStatus2;
	return 0;
}