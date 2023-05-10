#include<stdio.h>  
//#include<time.h>
#include<string.h>

int main() {
	FILE* file;
	if (fopen_s(&file, "C:/Users/Виктория/Desktop/test_vicky.mvo", "r+")) {
		printf_s("Error on open file!\n");
		getchar();
		return 0;
	}
	
	char buffer[1024] = { 0 };
	fread(buffer, sizeof(char), 1024, file);
	printf_s("% s", buffer);
	printf_s("\n");

	//time_t rawtime;
	//struct tm info;
	//time(&rawtime);
	//localtime_s(&info, &rawtime);

	memset(buffer, '\0', 1024);
	strcat_s(buffer, "Moshnikova_Victoria_group_201_331");
	//size_t len = strlen(buffer);

	fseek(file, 0, 0);
	fwrite(buffer, sizeof(char), 1024, file);

	fclose(file);

	getchar();

	return 0;

}
