#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>

#include "bst.h"
#include "pds.h"

// Define the global variable
struct PDS_RepoInfo repo_handle;

// Open data file, (if repo_name is "demo", then data file name is demo.dat)
// Open index file (if repo_name is "demo", index file is demo.ndx)
// Load data from index file into Binary Search Tree variable present in the repo handle

int pds_open( char *repo_name, int rec_size )
{
	int status;
	char repo_file[30];
	char index_file[30];
	// Check if status is already open in repo_handle
	if( repo_handle.repo_status == PDS_REPO_OPEN )
	{
		return PDS_REPO_ALREADY_OPEN;
	}
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
	if( repo_handle.pds_ndx_fp == NULL ){
		perror(index_file);
	}
	struct PDS_NdxInfo *temp;
	int readsize=sizeof(struct PDS_NdxInfo);
	fseek(repo_handle.pds_ndx_fp, 0, SEEK_SET);
	temp=malloc(readsize);
	repo_handle.pds_bst = NULL;

	while(1){

		if( feof(repo_handle.pds_ndx_fp) ) {
			break;
		}
		fread(temp,readsize,1,repo_handle.pds_ndx_fp);
		bst_add_node(&repo_handle.pds_bst,temp->key, temp);
	}
	free(temp);
	repo_handle.repo_status = PDS_REPO_OPEN;// TO-DO set the status appropriately from pds.h
	status=PDS_SUCCESS;
	return status;
}

// Seek to the end of the data file
// Create an index entry with the current file location
// Add index entry to BST
// Write the record at the current file location

int put_rec_by_key( int key, void *rec )
{
	int offset, status, writesize;
	struct PDS_NdxInfo *ndx_entry;

	writesize = repo_handle.rec_size;
	if (repo_handle.pds_data_fp == NULL || repo_handle.pds_ndx_fp == NULL)
		return PDS_FILE_ERROR;

	fseek(repo_handle.pds_data_fp, 0, SEEK_END);
	offset = ftell(repo_handle.pds_data_fp);

	fwrite(rec, writesize, 1, repo_handle.pds_data_fp);
	fflush(repo_handle.pds_data_fp);
  if(ferror(repo_handle.pds_data_fp)){
    clearerr(repo_handle.pds_data_fp);
    status = PDS_ADD_FAILED;
    return status;
  }

	ndx_entry = malloc(sizeof(struct PDS_NdxInfo));
	ndx_entry->key = key;
	ndx_entry->offset = offset;

	bst_add_node(&repo_handle.pds_bst, ndx_entry->key, ndx_entry);


	status=PDS_SUCCESS;
	// TO-DO

	return status;
}

// Search for index entry in BST
// Seek to the file location based on offset in index entry
// Read the record at the current file location

int get_rec_by_key( int key, void *rec )
{
	struct PDS_NdxInfo *ndx_entry;
	struct BST_Node *bst_node;
	int offset, status, readsize;

	if (repo_handle.pds_data_fp == NULL || repo_handle.pds_ndx_fp == NULL)
		return PDS_FILE_ERROR;

	bst_node = bst_search(repo_handle.pds_bst, key);
	if (bst_node == NULL)
		return PDS_REC_NOT_FOUND;
	ndx_entry = bst_node->data;
	offset = ndx_entry->offset;
	fseek(repo_handle.pds_data_fp, offset, SEEK_SET);

	fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);

	if(ferror(repo_handle.pds_data_fp)){
    clearerr(repo_handle.pds_data_fp);
    status = PDS_ADD_FAILED;
    return status;
  }

	status=PDS_SUCCESS;
	return status;

}

// Search for index entry in BST
// Delete from BST, delete from data file

int delete_rec_by_key( int key )
{
	struct PDS_NdxInfo *ndx_entry;
	struct BST_Node *bst_node;
	int offset, status;

	if (repo_handle.pds_data_fp == NULL || repo_handle.pds_ndx_fp == NULL)
		return PDS_FILE_ERROR;

	bst_node = bst_search(repo_handle.pds_bst, key);
	if (bst_node != NULL)
	{
		ndx_entry = bst_node->data;
		offset = ndx_entry->offset;
		repo_handle.pds_bst = bst_delete(repo_handle.pds_bst, key);
	}
	if (bst_search(repo_handle.pds_bst, key) == NULL)
	{
		status = PDS_SUCCESS;
	}


	return status;
}

static void wrtie_in_file(struct BST_Node *root)
{
	int readsize = sizeof(struct PDS_NdxInfo);
	if (root == NULL)
		return;

	fwrite(root->data, readsize, 1, repo_handle.pds_ndx_fp);
	fseek(repo_handle.pds_ndx_fp, 0, SEEK_END);
	wrtie_in_file(root->right_child);
	wrtie_in_file(root->left_child);
}

// Close file pointers
// Free BST index
// Clear data in repo handle
// Update repository status in repo handle

int pds_close()
{
	fclose(repo_handle.pds_data_fp);
	fclose(repo_handle.pds_ndx_fp);

	char index_file[30];
	strcpy(index_file, repo_handle.pds_name);
	strcat(index_file, ".ndx");
	repo_handle.pds_ndx_fp = fopen(index_file, "wb");
	wrtie_in_file(repo_handle.pds_bst);
	fclose(repo_handle.pds_ndx_fp);

	bst_free(repo_handle.pds_bst);
	strcpy(repo_handle.pds_name, "");
	repo_handle.rec_size = 0;
	repo_handle.repo_status = PDS_REPO_CLOSED;

	return PDS_SUCCESS;
}
