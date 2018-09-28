#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>

#include "pds.h"

// Define the global variable
struct PDS_RepoInfo repo_handle;

int pds_open( char *repo_name, int rec_size )
{
	char repo_file[30];
	char index_file[30];
	// Check if status is already open in repo_handle
	if( repo_handle.repo_status == PDS_REPO_OPEN ) //TO-DO
		return PDS_REPO_ALREADY_OPEN;

	strcpy(repo_handle.pds_name,repo_name);

	strcpy(repo_file,repo_name);
	strcat(repo_file,".dat");
	strcpy(index_file,repo_name);
	strcat(index_file,".ndx");

	// Open the repository file in binary read-write mode
	repo_handle.pds_data_fp = fopen(repo_file,"ab+");// TO-DO
	repo_handle.pds_ndx_fp = fopen(index_file,"ab+");
	repo_handle.rec_size = rec_size;
	if( repo_handle.pds_data_fp == NULL ){
		perror(repo_file);
	}

	repo_handle.repo_status = PDS_REPO_OPEN;// TO-DO set the status appropriately from pds.h
	return PDS_SUCCESS;
}

// Seek to the end of the data file
// Write the record at the current file location
// Set status to PDS_SUCCESS on success or appropriate error status as defined in pds.h

int put_rec_by_key( int key, void *rec )
{
	int status, writesize;
  long int offset=0;
  writesize=repo_handle.rec_size;

  if( repo_handle.repo_status == PDS_REPO_CLOSED ){
    status = PDS_REPO_CLOSED;
    return status;
	}



	fseek(repo_handle.pds_data_fp,offset,SEEK_END);
	int ptr = ftell(repo_handle.pds_data_fp);

  fwrite(rec,writesize,1,repo_handle.pds_data_fp);
	fflush(repo_handle.pds_data_fp);

  if(ferror(repo_handle.pds_data_fp)){
    clearerr(repo_handle.pds_data_fp);
    status = PDS_ADD_FAILED;
    return status;
  }

	struct PDS_NdxInfo temp_ndx;

	temp_ndx.key=key;
	temp_ndx.offset=ptr;

	fseek(repo_handle.pds_ndx_fp,offset,SEEK_END);
	fwrite(&temp_ndx,sizeof(struct PDS_NdxInfo),1,repo_handle.pds_ndx_fp);
	fflush(repo_handle.pds_ndx_fp);

  if(ferror(repo_handle.pds_ndx_fp)){
    clearerr(repo_handle.pds_ndx_fp);
    status = PDS_ADD_FAILED;
    return status;
  }
	// TO-DO
  status = PDS_SUCCESS;
	return status;
}

// Read one-record-at a time in a loop until EOF from the beginning of the file
// Compare the key of the record being read with the key being searched
// Set status to PDS_SUCCESS on success or appropriate error status as defined in pds.h

int get_rec_by_key( int key, void *rec )
{
	int offset, status, readsize,ptr;
	offset=0;
  readsize = sizeof(struct PDS_NdxInfo);
  struct PDS_NdxInfo *temp;
	// TO-DO
  status = PDS_REC_NOT_FOUND;
	temp=malloc(readsize);
	if( repo_handle.repo_status == PDS_REPO_CLOSED ){
    status = PDS_REPO_CLOSED;
    return status;
	}
	fseek(repo_handle.pds_ndx_fp,offset,SEEK_SET);
  while(1) {

		// printf("key:%d\n", key );
			fread(temp,readsize,1,repo_handle.pds_ndx_fp);
			if(ferror(repo_handle.pds_ndx_fp)){
        clearerr(repo_handle.pds_ndx_fp);
        status = PDS_FILE_ERROR;
				// printf("%d\n", status );
        return status;
      }

      if(key == temp->key){
				ptr=temp->offset;

				break;
      }
			if( feof(repo_handle.pds_data_fp) ) {
				return status;
			}
    }

	free(temp);

	fseek(repo_handle.pds_data_fp,ptr,SEEK_SET);
	fread(rec,repo_handle.rec_size,1,repo_handle.pds_data_fp);

	status = PDS_SUCCESS;
	return status;
}

int pds_close()
{
	strcpy(repo_handle.pds_name, "");
	fclose(repo_handle.pds_data_fp);
	fclose(repo_handle.pds_ndx_fp);
	repo_handle.rec_size = 0;
	repo_handle.repo_status = PDS_REPO_CLOSED;

	return PDS_SUCCESS;
}
