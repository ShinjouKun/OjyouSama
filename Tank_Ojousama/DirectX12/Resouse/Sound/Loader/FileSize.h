#pragma once

#include <limits.h>
#include <stdio.h>

//ファイルサイズを取得

unsigned int getFileSize(const char* filename)
{
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		return 0;
	}

	if (fseek(fp, 0L, SEEK_END) == 0)
	{
		fpos_t pos;
		if (fgetpos(fp, &pos) == 0)
		{
			fclose(fp);
			return static_cast<unsigned int>(pos);
		}
	}
	fclose(fp);
	return 0;
}