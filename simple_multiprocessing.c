#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void print_message(int fd)
{
	char resstring[13];
	size_t size = read(fd, resstring, 13);
        if(size < 0){
		/* Если прочитать не смогли, сообщаем об ошибке и
		завершаем работу */
		printf("Can\'t read string\n"); 
		exit(-1); 
	} 
        /* Печатаем прочитанную строку */
        printf("%s\n",resstring);
}

void send_message(int fd, char message[13])
{
	size_t size = write(fd, message, 13);
        if(size != 13){
		/* Если записалось меньшее количество байт, сообщаем
		об ошибке и завершаем работу */
		printf("Can\'t write all string\n"); 
		exit(-1); 
        } 
}

void create_pipe(int fd[2])
{
	if(pipe(fd) < 0)
	{
        	/* Если создать конвейер не удалось, печатаем об этом сообщение и прекращаем работу */
        	printf("Can\'t create pipe\n");
        	exit(-1); 
	}
}

int main(){
    int child_fd[2], parent_fd[2], result;
    
    /* Попытаемся создать конвейер */
    create_pipe(child_fd);
    create_pipe(parent_fd);
    /* Порождаем новый процесс */ 
    result = fork(); 
    if (result > 0) {
        send_message(child_fd[1], "Hello, child!");
        printf("Child: ");
        print_message(parent_fd[0]);
        close(parent_fd[0]);
        close(parent_fd[1]);
        printf("Parent exit\n");
        sleep(1);
    } else if (!result){
        send_message(parent_fd[1], "Hello, parent");
        printf("Parent: ");
        print_message(child_fd[0]);
        close(child_fd[0]);
        close(child_fd[1]);
        printf("Child exit\n");
    }
    else {
    	/* Если создать процесс не удалось, сообщаем об этом и 
        завершаем работу */
        printf("Can\'t fork child\n");
        exit(-1);
    }
    return 0;
}
