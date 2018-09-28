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
	char repo_file[30];
	char index_file[30];

	if (repo_handle.repo_status == PDS_REPO_OPEN)
	return PDS_REPO_ALREADY_OPEN;

	strcpy(repo_handle.pds_name, repo_name);
	repo_handle.rec_size = rec_size;

	strcpy(repo_file, repo_name);
	strcat(repo_file, ".dat");
	repo_handle.pds_data_fp = fopen(repo_file, "ab+");

	strcpy(index_file, repo_name);
	strcat(index_file, ".ndx");
	repo_handle.pds_ndx_fp = fopen(index_file, "ab+");

	if (repo_handle.pds_data_fp == NULL) {
	perror(repo_file);
	}

	if (repo_handle.pds_ndx_fp == NULL) {
	perror(index_file);
	}

	int indexSize = sizeof(struct PDS_NdxInfo);
	fseek(repo_handle.pds_ndx_fp, 0, SEEK_SET);
	repo_handle.pds_bst = NULL;

	struct PDS_NdxInfo *ndx_inf = (struct PDS_NdxInfo *) malloc(indexSize);
	while (fread(ndx_inf, indexSize, 1, repo_handle.pds_ndx_fp)) {
		if (!feof(repo_handle.pds_ndx_fp)) {
				bst_add_node(&repo_handle.pds_bst, ndx_inf->key, ndx_inf);
				ndx_inf = (struct PDS_NdxInfo *) malloc(indexSize);
		}
	}
	repo_handle.repo_status = PDS_REPO_OPEN;
	return PDS_SUCCESS;
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
	{
		return PDS_FILE_ERROR;
	}
	else
	{
		fseek(repo_handle.pds_data_fp, 0, SEEK_END);
		offset = ftell(repo_handle.pds_data_fp);
		status = fwrite(rec, writesize, 1, repo_handle.pds_data_fp);
		if (status == 0)
		{
			return PDS_ADD_FAILED;
		}
		ndx_entry = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
		ndx_entry->key = key;
		ndx_entry->offset = offset;

		bst_add_node(&repo_handle.pds_bst, ndx_entry->key, ndx_entry);
	}
	
  return PDS_SUCCESS;
}

// Search for index entry in BST
// Seek to the file location based on offset in index entry
// Read the record at the current file location

int get_rec_by_key( int key, void *rec )
{
	struct PDS_NdxInfo *ndx_entry;
	struct BST_Node *bst_node;
	int offset, status, readsize;
	if (repo_handle.pds_data_fp == NULL || repo_handle.repo_status == PDS_REPO_ALREADY_OPEN)
	{
		return PDS_FILE_ERROR;
	}
	else
	{
		bst_node = bst_search(repo_handle.pds_bst, key);
		if (bst_node == NULL)
		{
			return PDS_REC_NOT_FOUND;
		}
		else
		{
			ndx_entry = bst_node->data;
			offset = ndx_entry->offset;
			status = fseek(repo_handle.pds_data_fp, offset, SEEK_SET);
			if (status == 1)
			{
				return PDS_FILE_ERROR;
			}
			fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
		}
	}

	return PDS_SUCCESS;

}

// Search for index entry in BST
// Delete from BST, delete from data file

int delete_rec_by_key( int key )
{
	struct PDS_NdxInfo *ndx_entry;
	struct BST_Node *bst_node;
	int offset, status;
	if (repo_handle.pds_data_fp == NULL || repo_handle.repo_status == PDS_REPO_ALREADY_OPEN)
	{
		return PDS_FILE_ERROR;
	}
	else
	{
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
	}
	
	return status;
}

static void writeInFile(struct BST_Node *root)
{
	int indexSize = sizeof(struct PDS_NdxInfo);
	if (root == NULL)
	{
		return;
	}
	fwrite(root->data, indexSize, 1, repo_handle.pds_ndx_fp);
	fseek(repo_handle.pds_ndx_fp, 0, SEEK_END);
	writeInFile(root->right_child);
	writeInFile(root->left_child);
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
	writeInFile(repo_handle.pds_bst);
	fclose(repo_handle.pds_ndx_fp);

	bst_free(repo_handle.pds_bst);
	strcpy(repo_handle.pds_name, "");
	repo_handle.rec_size = 0;
	repo_handle.repo_status = PDS_REPO_CLOSED;

	return PDS_SUCCESS;
}
