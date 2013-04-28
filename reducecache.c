/************************************************************************

    reducecache.c: Source file for diskcache reducer for x64 Windows

    Written by Yasutaka ATARASHI <yak_ex@mx.scn.tv>

    Distributed under the terms of NYSL License, Version 0.9982
    See http://www.kmonos.net/nysl/

 ***********************************************************************/

#define _WIN32_WINNT 0x0502
#include <windows.h>
#ifndef FILE_CACHE_FLAGS_DEFINED
#define FILE_CACHE_MAX_HARD_ENABLE      0x00000001
#define FILE_CACHE_MAX_HARD_DISABLE     0x00000002
#define FILE_CACHE_MIN_HARD_ENABLE      0x00000004
#define FILE_CACHE_MIN_HARD_DISABLE     0x00000008
#endif // FILE_CACHE_FLAGS_DEFINED
#include <stdio.h>
#include <inttypes.h>

void output_configuration(void)
{
	SIZE_T min_size = 0, max_size = 0;
	DWORD flags = 0;
	if(!GetSystemFileCacheSize(&min_size, &max_size, &flags)) {
		printf("%u\n", GetLastError());
	}
#if defined(__x86_64)
	printf("min,max,flag: %" PRIu64 ",%" PRIu64 ",%u\n", min_size, max_size, flags);
#else
	printf("min,max,flag: %" PRIu32 ",%" PRIu32 ",%u\n", min_size, max_size, flags);
#endif
}

void adjust_token(void)
{
	HANDLE token;;
	TOKEN_PRIVILEGES priv;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,  &token);

	LookupPrivilegeValue( NULL, SE_INCREASE_QUOTA_NAME, &(priv.Privileges[0].Luid) );
	priv.PrivilegeCount = 1;
	priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(token, FALSE, &priv, 0, NULL, NULL);
}

void usage(void)
{
	puts(
		"usage:\n"
		"\treducecache -h : show this help\n"
		"\n"
		"\treducecache [-v] [<max>] : Specify max size of disk cache\n"
		"\t\tDefaults to 64 * 1024 * 1024 = 67108864\n"
		"\t\tYou can specify the value in decimal, octal, or hexadecimal.\n"
		"\t\tYou need to have appropriate privileage.\n"
		"\t\tIf -v specified, trace messages are output.\n"
	);
	exit(0);
}

int main(int argc, char **argv)
{
	SIZE_T prev_min_size = 0, prev_max_size = 0;
	DWORD prev_flags = 0, flags;
	DWORD max_size = 64 * 1024 * 1024;

	int verbose = 0;
	char** arg = argv;
	if(*arg) ++arg;
	while(*arg) {
		if((*arg)[0] == '-' || (*arg)[0] == '/') {
			if((*arg)[1] == 'h' && (*arg)[2] == '\0') usage();
			else if((*arg)[1] == 'v' && (*arg)[2] == '\0') verbose = 1;
			else {
				puts("Unknown option");
				usage();
			}
		} else {
			char *endptr;
			max_size = strtoul(*arg, &endptr, 0);
			if(*endptr != '\0') usage();
			++arg;
			break;
		}
		++arg;
	}
	if(*arg) {
		puts("Extra arguments");
		usage();
	}

	adjust_token();

	if(!GetSystemFileCacheSize(&prev_min_size, &prev_max_size, &prev_flags)) {
		printf("%u\n", GetLastError());
	}
	if(verbose) output_configuration();

	if(!SetSystemFileCacheSize(0, max_size, FILE_CACHE_MAX_HARD_ENABLE)) {
		printf("Set1: %u\n", GetLastError());
	}

	if(verbose) output_configuration();

	flags = (prev_flags & FILE_CACHE_MIN_HARD_ENABLE) ? FILE_CACHE_MIN_HARD_ENABLE : FILE_CACHE_MIN_HARD_DISABLE;
	flags |= (prev_flags & FILE_CACHE_MAX_HARD_ENABLE) ? FILE_CACHE_MAX_HARD_ENABLE : FILE_CACHE_MAX_HARD_DISABLE;
	if(!SetSystemFileCacheSize(prev_min_size, prev_max_size, flags)) {
		printf("Set2: %u\n", GetLastError());
	}

	if(verbose) output_configuration();

	return 0;
}
