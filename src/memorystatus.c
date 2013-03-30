#include <windows.h>
#include <stdio.h>

#define SLEEP_TIME 10000
#define LOGFILE "D:\\memstatus.txt"


int WriteToLogstr(char* str);
int WriteToLoglong(long num);
int WriteToLogsize_t(size_t sizet);//size_t����� 


SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

void ServiceMain(int argc, char** argv);
void ControlHandler(DWORD request);
int InitService();


int InitService()
{
int result;
result = WriteToLogstr("[��ʼ]");
result = WriteToLogstr("\n");
		 WriteToLogstr("\n");
return(result);
}

int main()
{
    SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = "MemoryStatus";
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
    
    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;

    //
    StartServiceCtrlDispatcher(ServiceTable);
return 0;
}


void ServiceMain(int argc, char** argv)
{
   int error;
   int result; 

   ServiceStatus.dwServiceType =    SERVICE_WIN32;
   ServiceStatus.dwCurrentState =    SERVICE_START_PENDING;
   ServiceStatus.dwControlsAccepted   =     SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
   ServiceStatus.dwWin32ExitCode = 0;
   ServiceStatus.dwServiceSpecificExitCode = 0;
   ServiceStatus.dwCheckPoint = 0;
   ServiceStatus.dwWaitHint = 0;

   hStatus = RegisterServiceCtrlHandler(
      "MemoryStatus",
      (LPHANDLER_FUNCTION)ControlHandler);
      
      
	error = InitService();
	if (error)
	{
// ��ʼ��ʧ�ܣ���ֹ����
	ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	ServiceStatus.dwWin32ExitCode = -1;
	SetServiceStatus(hStatus, &ServiceStatus);
// �˳� ServiceMain
	return;
	}

// �� SCM ��������״̬

   ServiceStatus.dwCurrentState = SERVICE_RUNNING;
   SetServiceStatus (hStatus, &ServiceStatus);

			MEMORYSTATUS lpmemory;//���� 

   while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
   {
			lpmemory.dwLength =sizeof(lpmemory); //��ʼ�� 
			GlobalMemoryStatus(&lpmemory);//��ֵ 

      result = WriteToLogstr("�������ڴ�: ");
      result = WriteToLogsize_t(lpmemory.dwTotalPhys/1024/1024);
      result = WriteToLogstr("MB\n");
      result = WriteToLogstr("�ڴ���ʹ��: ");
      result = WriteToLoglong((long)lpmemory.dwMemoryLoad);
      result = WriteToLogstr("%\n");
      result = WriteToLogstr("���������ڴ�: ");
      result = WriteToLogsize_t(lpmemory.dwAvailPhys/1024/1024);
      result = WriteToLogstr("MB\n");
//      result = WriteToLogstr("�ܿɵ���������洢��: ");
//      result = WriteToLogsize_t(lpmemory.dwTotalPageFile/1024/1024);
//      result = WriteToLogstr("MB\n");
//      result = WriteToLogstr("���ÿɵ���������洢��: ");
//      result = WriteToLogsize_t(lpmemory.dwAvailPageFile/1024/1024);
//      result = WriteToLogstr("MB\n");
//      result = WriteToLogstr("�������ڴ�: ");
//      result = WriteToLogsize_t(lpmemory.dwTotalVirtual/1024/1024);
//      result = WriteToLogstr("MB\n");
//      result = WriteToLogstr("���������ڴ�: ");
//      result = WriteToLogsize_t(lpmemory.dwAvailVirtual/1024/1024);
//      result = WriteToLogstr("MB\n");
      result = WriteToLogstr("\n");
      if (result)
      {
         ServiceStatus.dwCurrentState = SERVICE_STOPPED;
         ServiceStatus.dwWin32ExitCode = -1;
         SetServiceStatus(hStatus, &ServiceStatus);
         return;
      }
      Sleep(SLEEP_TIME);
      
	  while (ServiceStatus.dwCurrentState == SERVICE_PAUSED)//
		{													//������ͣ������ 
															//
		} 													//
   }
   return;
}


void ControlHandler(DWORD request)
{
   switch(request)
   {
      case SERVICE_CONTROL_PAUSE:
         WriteToLogstr("[��ͣ]");
		 WriteToLogstr("\n");
		 WriteToLogstr("\n");

         ServiceStatus.dwCurrentState = SERVICE_PAUSED;
         SetServiceStatus (hStatus, &ServiceStatus);
         return;

      case SERVICE_CONTROL_CONTINUE:
         WriteToLogstr("[����]");
		 WriteToLogstr("\n");
		 WriteToLogstr("\n");

         ServiceStatus.dwCurrentState = SERVICE_RUNNING;
         SetServiceStatus (hStatus, &ServiceStatus);
         return;

      case SERVICE_CONTROL_STOP:
         WriteToLogstr("[������ֹ.ֹͣ��¼]");
		 WriteToLogstr("\n");
		 WriteToLogstr("\n");

         ServiceStatus.dwWin32ExitCode = 0;
         ServiceStatus.dwCurrentState = SERVICE_STOPPED;
         SetServiceStatus (hStatus, &ServiceStatus);
         return;

      case SERVICE_CONTROL_SHUTDOWN:
         WriteToLogstr("[�ػ�.ֹͣ��¼]");
		 WriteToLogstr("\n");
		 WriteToLogstr("\n");

         ServiceStatus.dwWin32ExitCode = 0;
         ServiceStatus.dwCurrentState = SERVICE_STOPPED;
         SetServiceStatus (hStatus, &ServiceStatus);
         return;
        
      default:
         break;
    }

    SetServiceStatus (hStatus, &ServiceStatus);

    return;
}

int WriteToLogstr(char* str)
{
    FILE* log;
    log = fopen(LOGFILE, "a+");
    if (log == NULL)
    return -1;
    fprintf(log, "%s", str);
    fclose(log);
    return 0;
}

int WriteToLoglong(long num)
{
    FILE* log;
    log = fopen(LOGFILE, "a+");
    if (log == NULL)
    return -1;
    fprintf(log, "%ld", num);
    fclose(log);
    return 0;
}

int WriteToLogsize_t(size_t sizet)
{
    FILE* log;
    log = fopen(LOGFILE, "a+");
    if (log == NULL)
    return -1;
    fprintf(log, "%Iu", sizet);
    fclose(log);
    return 0;
}
