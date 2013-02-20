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
	printf("%" PRIu64 ",%" PRIu64 ",%u\n", min_size, max_size, flags);
#else
	printf("%" PRIu32 ",%" PRIu32 ",%u\n", min_size, max_size, flags);
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

int main(void)
{
	SIZE_T prev_min_size = 0, prev_max_size = 0;
	DWORD prev_flags = 0, flags;

	adjust_token();

	if(!GetSystemFileCacheSize(&prev_min_size, &prev_max_size, &prev_flags)) {
		printf("%u\n", GetLastError());
	}
#if defined(__x86_64)
	printf("%" PRIu64 ",%" PRIu64 ",%u\n", prev_min_size, prev_max_size, prev_flags);
#else
	printf("%" PRIu32 ",%" PRIu32 ",%u\n", prev_min_size, prev_max_size, prev_flags);
#endif
	if(!SetSystemFileCacheSize(0, 64 * 1024 * 1024, FILE_CACHE_MAX_HARD_ENABLE)) {
		printf("Set1: %u\n", GetLastError());
	}

	output_configuration();

	flags = (prev_flags & FILE_CACHE_MIN_HARD_ENABLE) ? FILE_CACHE_MIN_HARD_ENABLE : FILE_CACHE_MIN_HARD_DISABLE;
	flags |= (prev_flags & FILE_CACHE_MAX_HARD_ENABLE) ? FILE_CACHE_MAX_HARD_ENABLE : FILE_CACHE_MAX_HARD_DISABLE;
	if(!SetSystemFileCacheSize(prev_min_size, prev_max_size, flags)) {
		printf("Set2: %u\n", GetLastError());
	}

	output_configuration();

	return 0;
}
